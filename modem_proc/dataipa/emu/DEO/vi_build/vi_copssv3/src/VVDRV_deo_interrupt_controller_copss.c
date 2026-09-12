/**---------------------------------------------------------------------------
 * @file    VVDRV_deo_interrupt_controller_copss.c
 *
 * @brief   Interrupt controller implementation for COPSS Generic Emulation
 *
 * Copyright (c) 2016-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/************************************************************************/
/*                           INCLUDES                                   */
/************************************************************************/

#include "VVDRV_deo_interrupt_controller_copss.h"
#include "VVDRV_deo_debug.h"

/************************************************************************/
/*                            MACROS                                    */
/************************************************************************/

#ifndef in_dword
/** @brief  Input a 32-bit value. Defined to remove dependency on hwio.h */
#define __inpdw(port)       (*((volatile uint32 *) (port)))
/** @brief  Input a 32-bit value. Defined to remove dependency on hwio.h */
#define in_dword(addr)      (__inpdw(addr))
#endif /* !in_dword */

#ifndef out_dword
/** @brief  Output a 32-bit value. Defined to remove dependency on hwio.h */
#define __outpdw(port, val) (*((volatile uint32 *) (port)) = ((uint32) (val)))
/** @brief  Output a 32-bit value. Defined to remove dependency on hwio.h */
#define out_dword(addr, val)    __outpdw(addr,val)
#endif /* !out_dword */

/* Interrupt controller registers.
 * Descriptions taken from the COPSS interrupt controller SWI.
 * - There is only one Master Enable register
 * - Each group of 32 interrupt lines (range) is controlled by 8 registers,
 *   which are consecutive in memory:
 *      DEO_IC_INT_ENABLE_n
 *      DEO_IC_INT_ENABLE_CLEAR_n
 *      DEO_IC_INT_ENABLE_SET_n
 *      DEO_IC_INT_TYPE_n
 *      DEO_IC_IRQ_STATUS_n
 *      DEO_IC_RAW_STATUS_n
 *      DEO_IC_INT_CLEAR_n
 *      DEO_IC_SOFT_INT_n
 * - After the above 8 registers, there are the registers of the next
 *   group (range) of 32 interrupt lines, and so on.
 */

/** @brief The interrupt controller version and interrupt count register.
 *         Specifies interrupt controller version (upper 16 bits ) and the
 *         number of interrupt lines supported by HW (lower 16 bits).
 */
#define DEO_IC_REG_INT_CTL_VER_CNT      0x0000

/** @brief The IRQ master enable register.
 *         Bit #0: IRQ_ENABLE, set 0 to disable, 1 to enable.
 */
#define DEO_IC_REG_INT_MASTER_ENABLE    0x0008

/** @brief Each bit disables (bit=0, default) or enables (bit=1) the
 *         corresponding interrupt source
 */
#define DEO_IC_REG_INT_ENABLE_0         0x0010

/** @brief Write bit=1 to clear (to 0) the corresponding bit(s) in INT_ENABLE.
 *         Does nothing for bit=0
 */
#define DEO_IC_REG_INT_ENABLE_CLEAR_0   0x0014

/** @brief Write bit=1 to set (to 1) the corresponding bit(s) in INT_ENABLE.
 *         Does nothing for bit=0
 */
#define DEO_IC_REG_INT_ENABLE_SET_0     0x0018

/** @brief Select level (bit=0, default) or edge (bit=1) sensitive input
 *         detection logic for each corresponding interrupt source
 */
#define DEO_IC_REG_INT_TYPE_0           0x001C

/** @brief Shows the interrupt sources captured in RAW_STATUS that have been
 *         steered to irq_n by INT_SELECT. Interrupts must also be enabled by
 *         INT_ENABLE and MASTER_ENABLE. Read only register.
 *         Bit values: 1=active, 0=inactive
 */
#define DEO_IC_REG_IRQ_STATUS_0         0x0020

/** @brief Shows the interrupt sources that have been latched by the input
 *         logic of the Interrupt Controller. Read only register.
 *         Bit values: 1=active, 0=inactive
 */
#define DEO_IC_REG_RAW_STATUS_0         0x0024

/** @brief Write bit=1 to clear the corresponding bit(s) in RAW_STATUS.
 *         Does nothing for bit=0
 */
#define DEO_IC_REG_INT_CLEAR_0          0x0028

/** @brief Write bit=1 to set the corresponding bit(s) in RAW_STATUS.
 *         Does nothing for bit=0.
 *  @note  Only functional for edge detected interrupts
 */
#define DEO_IC_REG_SOFT_INT_0           0x002C

/** @brief Maximal number of ranges in SW. Each range supports 32 interrupt
 *         lines. If HW is extended considerably, increase this value
 */
#define DEO_IC_MAX_RANGE_CNT            16  /* 16 * 32 = 512 int lines */

/** @brief Size of the registers of one range in memory, in bytes */
#define DEO_IC_RANGE_MEM_SIZE           32  /* SWI: 8 registers, no gaps */

/** @brief Minimal Interrupt controller HW version */
#define DEO_IC_INT_CTL_VER_MIN          0x0102

/************************************************************************/
/*                             DATA                                     */
/************************************************************************/

/**
 * @brief Interrupt controller register map. Contains pointers adjusted to the
 *        addresses of all relevant registers in one interrupt registers range,
 *        which handles 32 interrupt lines
 */
typedef struct DEO_IC_REG_MAP_COPSS_S
{
    uint8 *regINT_ENABLE;       /**< Register INT_ENABLE address        */
    uint8 *regINT_ENABLE_CLEAR; /**< Register INT_ENABLE_CLEAR address  */
    uint8 *regINT_ENABLE_SET;   /**< Register INT_ENABLE_SET address    */
    uint8 *regINT_TYPE;         /**< Register INT_TYPE address          */
    uint8 *regIRQ_STATUS;       /**< Register IRQ_STATUS address        */
    uint8 *regRAW_STATUS;       /**< Register RAW_STATUS address        */
    uint8 *regINT_CLEAR;        /**< Register INT_CLEAR address         */
    uint8 *regSOFT_INT;         /**< Register SOFT_INT address          */
} DEO_IC_REG_MAP_COPSS;

/**
 * @brief Contains all data used by the COPSS interrupt controller
 */
typedef struct DEO_IC_DATA_COPSS_S
{
    DEO_IC_REG_MAP_COPSS regMap[DEO_IC_MAX_RANGE_CNT];   /**< Register maps */
    uint32     intCount;            /**< Number of supported interrupts     */
    uint32     rangeCount;          /**< Number of register ranges used     */
    uint8     *icBase;              /**< Interrupt controller base address  */
    uint8     *regMASTER_ENABLE;    /**< Register INT_MASTER_ENABLE address */
} DEO_IC_DATA_COPSS;

/**
 * @brief A single instance of DEO interrupt controller data for COPSS
 */
static DEO_IC_DATA_COPSS deo_gIc = {0};

/************************************************************************/
/*                            FUNCTIONS                                 */
/************************************************************************/

/**
 * @brief       Helper function to calculate register map and register mask
 *              from interrupt number
 *
 * @param[in]   intNum      Interrupt line number
 * @param[out]  regs        Will be set to point to the register-map needed to
 *                          handle the specified interrupt number
 * @param[out]  mask        Will be set to the register-mask needed to handle
 *                          the specified interrupt number
 *
 * @return      DEO_SUCCESS if successful otherwise DEO_ERROR
 */
static int32 deo_interrupt_controller_get_regs
(
    uint32                 intNum,
    DEO_IC_REG_MAP_COPSS **regs,
    uint32                *mask
)
{
    /* Check for valid interrupt number */
    if (intNum >= deo_gIc.intCount)
    {
        if (0 == deo_gIc.intCount)  /* Check if not initializing */
        {
            DEO_DBG_E(DEO_INT, ("Interrupt controller not initialized\n"));
        }
        else                        /* Out of initialized ranges */
        {
            DEO_DBG_E(DEO_INT, ("Invalid interrupt number\n"));
        }

        return DEO_ERROR;       /* Invalid interrupt number */
    }

    /* Get register map of range; 32 lines per register range */
    *regs = &(deo_gIc.regMap[intNum >> 5]);

    /* Find the mask to set within a single register range (32 lines) */
    *mask = (0x1 << (intNum & 31));  /* (intNum & 31) == (intNum % 32) */

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_init_copss()
 */
static int32 deo_interrupt_controller_init_copss
(
   const deo_interrupt_controller_init_t *config
)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                range, val;

    /* Check for valid configuration parameters */
    if (NULL == config)
    {
        DEO_DBG_F(DEO_INT, ("Invalid interrupt controller config params\n"));
        return DEO_ERROR;
    }

    /* Base address must be non-null and 32-bit aligned */
    if ((NULL == config->base_address) ||
        (0 != (0x3 & ((size_t)(config->base_address)))))
    {
        DEO_DBG_F(DEO_INT, ("Invalid interrupt controller base address\n"));
        return DEO_ERROR;
    }

    /* Set interrupt controller base address */
    deo_gIc.icBase = (uint8 *)(config->base_address);

    /* Read Interrupt controller version and int count register */
    val = in_dword(deo_gIc.icBase + DEO_IC_REG_INT_CTL_VER_CNT);

    /* Lower 16 bits are interrupt count */
    deo_gIc.intCount = (val & 0xFFFF);

    /* Upper 16 bits are interrupt controller version */
    val = ((val >> 16) & 0xFFFF);

    /* Verify the interrupt controller version */
    if ((0                      == val) ||
        (0xFFFF                 == val) ||
        (DEO_IC_INT_CTL_VER_MIN >  val))
    {
        DEO_DBG_F(DEO_INT,
            ("Error: invalid interrupt controller version 0x%x\n", val));
        return DEO_ERROR;
    }
    else /* Verify the interrupt count */
    {
        /* Int line count must be at least one block and multiple of 32 */
        if ((32 > deo_gIc.intCount) || (0 != (deo_gIc.intCount % 32)))
        {
            DEO_DBG_F(DEO_INT,
                ("Invalid interrupt count read from HW 0x%04x\n",
                 deo_gIc.intCount));
            return DEO_ERROR;
        }
    }

    /* Calculate number of ranges used, each range handles 32 int lines */
    deo_gIc.rangeCount = (deo_gIc.intCount) / 32;
    if (DEO_IC_MAX_RANGE_CNT < deo_gIc.rangeCount)
    {
        DEO_DBG_F(DEO_INT,
            ("SW interrupt limit passed, increase DEO_IC_MAX_RANGE_CNT\n"));
        return DEO_ERROR;
    }

    /* Set the master enable register address */
    deo_gIc.regMASTER_ENABLE = deo_gIc.icBase + DEO_IC_REG_INT_MASTER_ENABLE;

    /* Init register maps of all ranges */
    for (range = 0; range < deo_gIc.rangeCount; range++)
    {
        /* Calculate the range base address */
        uint8 *rangeBase = deo_gIc.icBase + (range * DEO_IC_RANGE_MEM_SIZE);

        /* Get pointer to relevant register map */
        regs = &(deo_gIc.regMap[range]);

        /* Set all register addresses in this range */
        regs->regINT_ENABLE       = rangeBase + DEO_IC_REG_INT_ENABLE_0;
        regs->regINT_ENABLE_CLEAR = rangeBase + DEO_IC_REG_INT_ENABLE_CLEAR_0;
        regs->regINT_ENABLE_SET   = rangeBase + DEO_IC_REG_INT_ENABLE_SET_0;
        regs->regINT_TYPE         = rangeBase + DEO_IC_REG_INT_TYPE_0;
        regs->regIRQ_STATUS       = rangeBase + DEO_IC_REG_IRQ_STATUS_0;
        regs->regRAW_STATUS       = rangeBase + DEO_IC_REG_RAW_STATUS_0;
        regs->regINT_CLEAR        = rangeBase + DEO_IC_REG_INT_CLEAR_0;
        regs->regSOFT_INT         = rangeBase + DEO_IC_REG_SOFT_INT_0;
    }

    /* Disable all interrupts during config */
    out_dword(deo_gIc.regMASTER_ENABLE, 0x0);

    /* Init register maps of all ranges */
    for (range = 0; range < deo_gIc.rangeCount; range++)
    {
        /* Get pointer to relevant register map */
        regs = &(deo_gIc.regMap[range]);
        
        /* Disable all int sources by setting all enable clear bits */
        out_dword(regs->regINT_ENABLE_CLEAR, 0xFFFFFFFF);
        
        /* Clear all raw statuses */
        out_dword(regs->regINT_CLEAR, 0xFFFFFFFF);
        
        /* Init all int types */
        out_dword(regs->regINT_TYPE, 0x0);
    }
    

    /* Enable the IC */
    out_dword(deo_gIc.regMASTER_ENABLE, 0x1);

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_configure_copss()
 */
static int32 deo_interrupt_controller_configure_copss
(
    const deo_interrupt_t *pInterrupt
)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                intNum;
    uint32                mask;
    uint32                value;

    if (NULL == pInterrupt)
    {
        DEO_DBG_E(DEO_INT, ("Invalid parameter\n"));
        return DEO_ERROR;
    }

    intNum = pInterrupt->int_source;

    /* Get registers mapping and mask */
    if (DEO_SUCCESS != deo_interrupt_controller_get_regs(intNum, &regs, &mask))
    {
        DEO_DBG_E(DEO_INT, ("Invalid interrupt number\n"));
        return DEO_ERROR;       /* Invalid interrupt number */
    }

    /* Per the Generic-Emulation Interrupt Controller SWI, updates to the
     * INT_TYPE_n registers should be done only with interrupts disabled, So
     * before updating this register the interrupt source is disabled
     */

    /* Disable by setting appropriate enable clear bit.
     * Note: mask zero bits have no effect
     */
    out_dword(regs->regINT_ENABLE_CLEAR, mask);

    /* Read-modify-write to configure edge/level */
    value = in_dword(regs->regINT_TYPE);
    if (DEO_INT_TRIGGER_EDGE == pInterrupt->int_trigger)
    {
        value |= mask;       /* Edge = 1 */
    }
    else
    {
        value &= (~mask);    /* Level = 0 */
    }
    out_dword(regs->regINT_TYPE, value);

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_enable_copss()
 */
static int32 deo_interrupt_controller_enable_copss(uint32 intNum)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                mask;

    /* Get registers mapping and mask */
    if (DEO_SUCCESS != deo_interrupt_controller_get_regs(intNum, &regs, &mask))
    {
        return DEO_ERROR;       /* Invalid interrupt number */
    }

    /* Enable by setting appropriate enable set bit.
     * Note: mask zero bits have no effect
     */
    out_dword(regs->regINT_ENABLE_SET, mask);

#ifdef VVDRV_DEO_IC_DEBUG_HW
    {
        /* Check if interrupt enable bit was updated */
        uint32 value = in_dword(regs->regINT_ENABLE);
        if ((value & mask) != mask)
        {
            DEO_DBG_E(DEO_INT, ("Could not write to enable register\n"));
            return DEO_ERROR;
        }
    }
#endif /* VVDRV_DEO_IC_DEBUG_HW */

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_disable_copss()
 */
static int32 deo_interrupt_controller_disable_copss(uint32 intNum)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                mask;

    /* Get registers mapping and mask */
    if (DEO_SUCCESS != deo_interrupt_controller_get_regs(intNum, &regs, &mask))
    {
        return DEO_ERROR;       /* Invalid interrupt number */
    }

    /* Disable by setting appropriate enable clear bit.
     * Note: mask zero bits have no effect
     */
    out_dword(regs->regINT_ENABLE_CLEAR, mask);

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_clear_copss()
 */
static int32 deo_interrupt_controller_clear_copss(uint32 intNum)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                mask;

    /* Get registers mapping and mask */
    if (DEO_SUCCESS != deo_interrupt_controller_get_regs(intNum, &regs, &mask))
    {
        return DEO_ERROR;       /* Invalid interrupt number */
    }

    /* Clear appropriate raw status bit. Note: mask zero bits have no effect */
    out_dword(regs->regINT_CLEAR, mask);

#ifdef VVDRV_DEO_IC_DEBUG_HW
    {
        /* Check if raw status was cleared */
        uint32 value = in_dword(regs->regRAW_STATUS);
        if ((value & mask) != 0x0)
        {
            /* Not necessarily an error: e.g. if the HW driving the
             * interrupt signal is still providing high level, the
             * status will keep high
             */
            DEO_DBG_W(DEO_INT, ("Could not clear raw status\n"));
        }

        /* Check if IRQ status is cleared */
        value = in_dword(regs->regIRQ_STATUS);
        if ((value & mask) != 0x0)
        {
            /* Not necessarily an error: e.g. if the core generating the
             * interrupt signal is still providing high level, status will
             * keep high
             */
            DEO_DBG_W(DEO_INT, ("Could not clear IRQ status\n"));
        }
    }
#endif /* VVDRV_DEO_IC_DEBUG_HW */

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_get_interrupt_source_status_copss()
 */
static boolean deo_interrupt_controller_get_interrupt_source_status_copss
(
    uint32  intNum
)
{
    DEO_IC_REG_MAP_COPSS *regs = NULL;
    uint32                mask;
    uint32                value;

    /* Get registers mapping and mask */
    if (DEO_SUCCESS != deo_interrupt_controller_get_regs(intNum, &regs, &mask))
    {
        DEO_DBG_E(DEO_INT, ("Invalid interrupt number (get int src)\n"));
        return FALSE;       /* Invalid interrupt number, return not set */
    }

    value = in_dword(regs->regIRQ_STATUS);
    if ((value & mask) == mask)
    {
        return TRUE;    /* Interrupt line set */
    }

    return FALSE;       /* Interrupt line not set */
}

/*--------------------------------------------------------------------------*/
/**
 * @see deo_interrupt_controller_set_pending()
 */
static int32 deo_interrupt_controller_set_pending_copss
(
    const deo_interrupt_t *pInterrupt,
    uint32                 interrupt_count
)
{
    uint32 valueSoftInt[DEO_IC_MAX_RANGE_CNT] = {0};
    uint32 cnt;

    /* Scan and accumulate bits to set */
    for (cnt = 0; cnt < interrupt_count; cnt++)
    {
        const uint32 intNum = pInterrupt[cnt].int_source;
        uint32 range, mask;

        if (intNum >= deo_gIc.intCount)
        {
            DEO_DBG_E(DEO_INT, ("Invalid interrupt number (set pend)\n"));
            return DEO_ERROR;   /* Invalid interrupt number */
        }

        range = intNum >> 5; /* Calculate range; 32 lines per register range */

        /* Find the mask to set within a single register range (32 lines) */
        mask = (0x1 << (intNum & 31));  /* (intNum & 31) == (intNum % 32) */

        valueSoftInt[range] |= mask; /* Accumulate masks to register values */
    }

    /* Set soft int registers on all relevant ranges */
    for (cnt = 0; cnt < deo_gIc.rangeCount; cnt++)
    {
        DEO_IC_REG_MAP_COPSS *regs  = &(deo_gIc.regMap[cnt]);
        uint32                value = valueSoftInt[cnt];

        if ((0 != value) && (NULL != regs))
        {
            out_dword(regs->regSOFT_INT, value);
        }
    }

    return DEO_SUCCESS;
}

/*--------------------------------------------------------------------------*/
/**
 * @brief           Load the function table of this interrupt controller
 *
 * @param[in, out]  funcTable   Pointer to function table to be filled
 *
 * @return          DEO_SUCCESS if successful otherwise DEO_ERROR
 */
int32 VVDRV_deo_interrupt_controller_load_copss(DEO_INT_CTRL *funcTable)
{
    if (NULL == funcTable)
    {
        DEO_DBG_F(DEO_INT, ("NULL function table pointer\n"));
        return DEO_ERROR;
    }

    funcTable->init              = &deo_interrupt_controller_init_copss;
    funcTable->configure         = &deo_interrupt_controller_configure_copss;
    funcTable->enable            = &deo_interrupt_controller_enable_copss;
    funcTable->disable           = &deo_interrupt_controller_disable_copss;
    funcTable->clear             = &deo_interrupt_controller_clear_copss;
    funcTable->get_source_status = 
                &deo_interrupt_controller_get_interrupt_source_status_copss;
    funcTable->set_pending       = &deo_interrupt_controller_set_pending_copss;

    return DEO_SUCCESS;
}
