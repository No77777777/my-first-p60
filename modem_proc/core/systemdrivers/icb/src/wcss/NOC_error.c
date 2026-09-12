/*==============================================================================

FILE:      NOC_error.c

DESCRIPTION: This file implements NOC Error Handler.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A

Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/wcss/NOC_error.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/01/21  pm     Branch for Adrastea (Modem User PD)
2015/11/23  pm     Added Sideband Manager support
2015/09/14  ddk    Port to 8937/8952.
2015/05/18  tb     Switched lock to the shared AC lock and XPU/SMMU API calls added
2015/05/11  tb     New handling of NoC errors in one pass added
2014/12/09  tb     Split OEM modifiable data from internal data
2014/10/20  tb     Added new NoC revID for FlexNoC version 2.11.1
2014/09/03  tb     Updated for DAL dev config
2013/11/07  pm     Added HW version for Bear family FlexNoC version 2.8.6-1
2013/11/07  pm     Fixed missing break when switching on HW version
2013/08/28  pm     Removed Clock_EnableBusAccess() call in interrupt context
2013/05/17  pm     Moved NOCInfo initialization before use in NOC_REG_ADDR() 
2013/05/16  pm     Enabled clocks at right time in NOC_error_init() 
2013/05/08  pm     Added FlexNoC version 2.8 
2012/10/03  av     Created

        Copyright (c) 2012 - 2016 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "NOC_error.h"
#include "NOC_error_platform.h"
#include "err.h"
#include "ABT_internal.h"
#include "msg.h"

/*============================================================================
                          MACROs
============================================================================*/
#define NOC_OUT8(addr, data)   (*((volatile uint8_t *) (addr)) = ((uint8_t) (data)))
#define NOC_OUT16(addr, data)  (*((volatile uint16_t *) (addr)) = ((uint16_t) (data)))
#define NOC_OUT32(addr, data)  (*((volatile uint32_t *) (addr)) = ((uint32_t) (data)))

#define NOC_IN8(addr)   ((uint8_t) (*((volatile uint8_t *) (addr))))
#define NOC_IN16(addr)  ((uint16_t) (*((volatile uint16_t *) (addr))))
#define NOC_IN32(addr)  ((uint32_t)(*((volatile uint32_t *) (addr))))

#define NOC_REG_ADDR(x,offset)   (((uint8_t*)NOCInfo[(x)].base_addr) + (offset))
#define NOC_SIDEBAND_REG_ADDR(x,offset)   (((uint8_t*)NOCInfo[(x)].sb_base_addr) + (offset))

/*============================================================================
                          DEFINEs
============================================================================*/
/* Reserved value of offset that indicates that the register does not apply to
 * this version of hardware
 */
#define REGISTER_NOT_APPLICABLE 0xFF  

/*============================================================================
                          STRUCTURE DECLARATIONS
============================================================================*/

/**
 * NOC Register Interface
 */
typedef struct
{
   uint8_t  core_id;       /**< NOC_OBS_ID_COREID offset */
   uint8_t  revision_id;   /**< NOC_OBS_ID_REVISIONID offset */
   uint8_t  errctl;        /**< NOC_OBS_ERRCTL offset -- Pre-FlexNoC 2.8 only */
   uint8_t  errstatus;     /**< NOC_OBS_ERRSTATUS offset -- Pre-FlexNoC 2.8 only */
   uint8_t  errlog0;       /**< NOC_OBS_ERRLOG0 offset */
   uint8_t  errlog1;       /**< NOC_OBS_ERRLOG1 offset */
   uint8_t  errlog2;       /**< NOC_OBS_ERRLOG2 offset */
   uint8_t  errlog3;       /**< NOC_OBS_ERRLOG3 offset */
   uint8_t  errlog4;       /**< NOC_OBS_ERRLOG4 offset */
   uint8_t  errlog5;       /**< NOC_OBS_ERRLOG5 offset */
   uint8_t  faulten;       /**< NOC_OBS_FAULTEN offset -- FlexNoC 2.8 only */
   uint8_t  errvld;        /**< NOC_OBS_ERRVLD offset -- FlexNoC 2.8 only */
   uint8_t  errclr;        /**< NOC_OBS_ERRCLR offset -- FlexNoC 2.8 only */
   uint8_t  errlog6;       /**< NOC_OBS_ERRLOG6 offset -- FlexNoC 2.8 only */
   /* New fields for new NoC versions go here */
}NOC_hw_type;

/**
 * NOC SidebandManager Register Interface
 */
typedef struct
{
   uint8_t  core_id;        /**< NOC_SBM_COREID offset */
   uint8_t  revision_id;    /**< NOC_SBM_REVISIONID offset */
   uint8_t  faulten;        /**< NOC_SBM_FAULTEN offset */
   uint8_t  faultstatus;    /**< NOC_SBM_FAULTSTATUS offset */
   uint8_t  flaginen0;      /**< NOC_SBM_FLAGINEN offset */
   uint8_t  flaginstatus0;  /**< NOC_SBM_FLAGINSTATUS0 offset */
   uint8_t  sensein0;       /**< NOC_SBM_SENSEIN0 offset */
   uint8_t  flagoutset0;    /**< NOC_SBM_FLAGOUTSET0 offset */
   uint8_t  flagoutclr0;    /**< NOC_SBM_FLAGOUTCLR0 offset */
   uint8_t  flagoutstatus0; /**< NOC_SBM_FLAGOUTSTATUS0 offset */
   /* New fields for new NoC SidebandManager versions go here */
}NOC_sideband_hw_type;


/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

static const NOCERR_propdata_type* NOCERR_propdata;
static const NOCERR_info_type* NOCInfo;

static NOC_hw_type* hw;
static NOC_sideband_hw_type* sideband;

NOCERR_syndrome_type NOCERR_syndrome_info[6];

/*============================================================================
                        NOC HW Register DEFINITIONS 
============================================================================*/
/* NOC HW Register Offsets Version 0.0 */
static NOC_hw_type NOC_0_0 = {
    0x00,                 /**< NOC_OBS_ID_COREID offset */
    0x04,                 /**< NOC_OBS_ID_REVISIONID offset */
    0x08,                 /**< NOC_OBS_ERRCTL offset */
    0x0C,                 /**< NOC_OBS_ERRSTATUS offset */
    0x10,                 /**< NOC_OBS_ERRLOG0 offset */
    0x14,                 /**< NOC_OBS_ERRLOG1 offset */
    0x18,                 /**< NOC_OBS_ERRLOG2 offset */
    0x1C,                 /**< NOC_OBS_ERRLOG3 offset */
    0x20,                 /**< NOC_OBS_ERRLOG4 offset */
    0x24,                 /**< NOC_OBS_ERRLOG5 offset */   
    REGISTER_NOT_APPLICABLE,
    REGISTER_NOT_APPLICABLE,
    REGISTER_NOT_APPLICABLE,
    REGISTER_NOT_APPLICABLE,
   /* When adding a new HW version, fill with REGISTER_NOT_APPLICABLE from
      here down to the length of NOC_hw_type */
};

/* NOC HW Register Offsets Version 2.8 */
static NOC_hw_type NOC_2_8 = {
    0x00,                 /**< NOC_OBS_ID_COREID offset */
    0x04,                 /**< NOC_OBS_ID_REVISIONID offset */
    REGISTER_NOT_APPLICABLE,
    REGISTER_NOT_APPLICABLE,
    0x14,                 /**< NOC_OBS_ERRLOG0 offset */
    0x18,                 /**< NOC_OBS_ERRLOG1 offset */
    0x1C,                 /**< NOC_OBS_ERRLOG2 offset */
    0x20,                 /**< NOC_OBS_ERRLOG3 offset */
    0x24,                 /**< NOC_OBS_ERRLOG4 offset */ 
    0x28,                 /**< NOC_OBS_ERRLOG5 offset */ 

    0x08,                 /**< NOC_OBS_FAULTEN offset */
    0x0C,                 /**< NOC_OBS_ERRVLD offset */
    0x10,                 /**< NOC_OBS_ERRCLR offset */

    0x2C,                  /**< NOC_OBS_ERRLOG6 offset */ 
   /* When adding a new HW version, fill with REGISTER_NOT_APPLICABLE from
      here down to the length of NOC_hw_type */
};

/* Add future HW versions below...*/


/* NOC SidebandManager HW Register Offsets */
static NOC_sideband_hw_type NOC_SIDEBAND_2_12 = {
   0x00,   /**< NOC_SBM_COREID offset */
   0x04,   /**< NOC_SBM_REVISIONID offset */
   0x08,   /**< NOC_SBM_FAULTEN offset */
   0x0C,   /**< NOC_SBM_FAULTSTATUS offset */
   0x10,   /**< NOC_SBM_FLAGINEN offset */
   0x14,   /**< NOC_SBM_FLAGINSTATUS0 offset */
   0xB0,   /**< NOC_SBM_SENSEIN0 offset */
   REGISTER_NOT_APPLICABLE,   /**< NOC_SBM_FLAGOUTSET0 offset */
   REGISTER_NOT_APPLICABLE,   /**< NOC_SBM_FLAGOUTCLR0 offset */
   REGISTER_NOT_APPLICABLE,   /**< NOC_SBM_FLAGOUTSTATUS0 offset */
   /* When adding a new HW version, fill with REGISTER_NOT_APPLICABLE from
      here down to the length of NOC_sideband_hw_type */
};

/* Special version of SidebandManager that only exists on Adrastea */
static NOC_sideband_hw_type NOC_SIDEBAND_Adrastea = {
   0x00,   /**< NOC_SBM_COREID offset */
   0x04,   /**< NOC_SBM_REVISIONID offset */
   0x08,   /**< NOC_SBM_FAULTEN offset */
   0x0C,   /**< NOC_SBM_FAULTSTATUS offset */
   0x10,   /**< NOC_SBM_FLAGINEN0 offset */
   0x14,   /**< NOC_SBM_FLAGINSTATUS0 offset */
   REGISTER_NOT_APPLICABLE,   /**< NOC_SBM_SENSEIN0 offset */
   0x50,   /**< NOC_SBM_FLAGOUTSET0 offset */
   0x54,   /**< NOC_SBM_FLAGOUTCLR0 offset */
   0x58,   /**< NOC_SBM_FLAGOUTSTATUS0 offset */
   /* When adding a new HW version, fill with REGISTER_NOT_APPLICABLE from
      here down to the length of NOC_sideband_hw_type */
};

/* Add future HW versions below...*/


/*============================================================================
                                 FUNCTION PROTOTYPES
============================================================================*/

/*============================================================================
                                 FUNCTIONS
============================================================================*/

/*===========================================================================*/
/**
@brief 
      Handler function for NOC error interrupt.
 
@param[in]  void

@return    
      void

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*==============================================================================*/
void NOC_Error_Handle_Interrupt(void)
{
 uint8_t i;
 uint8_t error_fatal = FALSE;

 // Go through all NOC blocks
 for(i=0; i < NOCERR_propdata->len; i++)
 {
     uint8_t sideband_fault = FALSE, obs_fault = FALSE;

     // If Sideband Manager exists and there is a sideband fault, 
     // log the Sideband Manager syndrome info
     if (sideband && NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->faultstatus)))
     {
        NOCERR_syndrome_info[i].name = NOCInfo[i].name;

        // Log Sideband Manager Syndrome Registers
        if (sideband->flaginstatus0 != REGISTER_NOT_APPLICABLE)  // FlexNoc 2.12.2 or later
        {
            NOCERR_syndrome_info[i].FLAGINSTATUS0 = 
               NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginstatus0));
            ABT_ERR("NOCError: %s ERROR; FLAGINSTATUS0 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].FLAGINSTATUS0, 0);
        }
        if (sideband->sensein0 != REGISTER_NOT_APPLICABLE)  // FlexNoc 2.12.2 or later
        {
            NOCERR_syndrome_info[i].SENSEIN0 = 
               NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->sensein0));
            ABT_ERR("NOCError: %s ERROR; SENSEIN0 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].SENSEIN0, 0);
        }
        sideband_fault = TRUE;
     }

     // If there is an OBS fault, then log the OBS block syndrome registers
     if (NOC_IN32(NOC_REG_ADDR(i,hw->errvld)))
     {
        NOCERR_syndrome_info[i].name = NOCInfo[i].name;

        // Log OBS Block Syndrome Registers
        if (hw->errstatus != REGISTER_NOT_APPLICABLE)  // Pre-FlexNoC 2.8 
        {
            ABT_ERR("NOCError: %s ERROR; ERRSTATUS = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOC_IN32(NOC_REG_ADDR(i,hw->errstatus)), 0);
        }

        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG0))
        {
            NOCERR_syndrome_info[i].ERRLOG0 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog0));
            ABT_ERR("NOCError: %s ERROR; ERRLOG0 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG0, 0);
        }
        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG1))
        {
            NOCERR_syndrome_info[i].ERRLOG1 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog1));
            ABT_ERR("NOCError: %s ERROR; ERRLOG1 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG1, 0);
        }
        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG2))
        {
            NOCERR_syndrome_info[i].ERRLOG2 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog2));
            ABT_ERR("NOCError: %s ERROR; ERRLOG2 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG2, 0);
        }
        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG3))
        {
            NOCERR_syndrome_info[i].ERRLOG3 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog3));
            ABT_ERR("NOCError: %s ERROR; ERRLOG3 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG3, 0);
        }
        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG4))
        {
            NOCERR_syndrome_info[i].ERRLOG4 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog4));
            ABT_ERR("NOCError: %s ERROR; ERRLOG4 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG4, 0);
        }
        if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG5))
        {
            NOCERR_syndrome_info[i].ERRLOG5 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog5));
            ABT_ERR("NOCError: %s ERROR; ERRLOG5 = 0x%08x", 
               NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG5, 0);
        }
        if (hw->errlog6 != REGISTER_NOT_APPLICABLE)    // FlexNoC 2.8
        {
            if(!(NOCInfo[i].errlog_filter & NOC_ERRLOG6))
            {
                NOCERR_syndrome_info[i].ERRLOG6 = NOC_IN32(NOC_REG_ADDR(i,hw->errlog6));
                ABT_ERR("NOCError: %s ERROR; ERRLOG6 = 0x%08x", 
                   NOCERR_syndrome_info[i].name, NOCERR_syndrome_info[i].ERRLOG6, 0);
            }
        }
        obs_fault = TRUE;
     }

     if (sideband_fault || obs_fault) 
     {
        error_fatal = error_fatal || NOCInfo[i].error_fatal;
     }

     // Clear OBS error status
     if (hw->errstatus != REGISTER_NOT_APPLICABLE)  // Pre-FlexNoC 2.8
     {
         NOC_OUT32(NOC_REG_ADDR(i,hw->errstatus), (NOC_IN32(NOC_REG_ADDR(i,hw->errstatus)) | 0x04));
     }
     if (hw->errclr != REGISTER_NOT_APPLICABLE)    // FlexNoC 2.8
     {
         NOC_OUT32(NOC_REG_ADDR(i,hw->errclr), (NOC_IN32(NOC_REG_ADDR(i,hw->errclr)) | 0x01));
     }

     // If there is a timeout, the way we clear the Sideband Manager is 
     // to clear the enable bit on the slave that is timing out.

     if (sideband && (sideband->flaginen0 != REGISTER_NOT_APPLICABLE))  // FlexNoc 2.12.2 or later
     {
         NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginen0), 
                   (NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginen0))) & 
                   (~(NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginstatus0)))));
     }

 }

 if(error_fatal)
 {
     ERR_FATAL("NOCError: FATAL ERROR", 0, 0, 0);
 }

 return;
}

//*============================================================================*/
/**
@brief
      Performs initialization for NOC Error Handler.
      It enables interrupts required to handle NOC errors.

@param[in]  None.

@return
      None.

@dependencies
      None.

@sideeffects
      None.
*/
/*============================================================================*/
void NOC_Error_Init( void )
{
    uint8_t i;
    uint32_t revision_id = 0;

    // Get device property data for configuration
    NOCERR_propdata = NOC_Error_Platform_Get_Propdata();
    if(NOCERR_propdata == NULL)
    {
        return;  // NOC data structure is not initialized in data file
    }

    NOCInfo = NOCERR_propdata->NOCInfo;
    if(NOCInfo == NULL)
    {
        return;  // NOC data structure is not initialized in data file
    }

    hw = &NOC_0_0;  // Temporarily assign a hw type to allow query of NoC version

    // Enabling NOC cfg clocks should happen in ABT_Init()

    /* This assumes all NoC versions on the chip are the same, so use index 0.
     * Mask 31:8 (AND with 0xFFFFFF00) to get the FlexNoC ID. 
     */
    revision_id = 0xFFFFFF00 & NOC_IN32(NOC_REG_ADDR(0,hw->revision_id));

    switch (revision_id) 
    {
       case 0x00000000:  // Target pre-silicon is wrong, and giving version 0x0
       {
         return; // Remove this when pre-silicon testing is phased out
         break;
       }
       case 0x0107F300:  // FlexNoC 2.5.11
       case 0x010F9000:  // FlexNoC 2.5.12
       {
          hw = &NOC_0_0;
           break;
       }
       case 0x0113D100:  // FlexNoC 2.8.2
       case 0x01150900:  // FlexNoc 2.8.3
       case 0x011EF300:  // FlexNoC 2.8.5
       case 0x01222B00:  // FlexNoC 2.8.6-1
       case 0x00033500:  // FlexNoC 2.8.8
       case 0x00049C00:  // FlexNoC 2.8.9
       case 0x0009A600:  // FlexNoC 2.8.10
       case 0x0010D500:  // FlexNoC 2.8.11
       case 0x00091F00:  // QNoC 2.11.0
       case 0x000C2400:  // QNoC 2.11.1   QNoC 2.11.1-1 QNoC 2.11.1-2
       case 0x000E0C00:  // QNoC 2.11.1-3 QNoC 2.11.1-4 QNoC 2.11.1-5
       case 0x0010FD00:  // QNoC 2.11.2
       case 0x0016E300:  // QNoC 2.11.3
       case 0x00183C00:  // QNoC 2.11.3-1
       case 0x00197B00:  // QNoC 2.11.3-2
       case 0x001DBB00:  // QNoC 2.11.3-3
       case 0x00322300:  // QNoC 2.12.1
       case 0x0036CC00:  // QNoC 2.12.2   8998 CNOC, MNOC, AGGRE1NOC, Adrastea
       case 0x0038A800:  // QNoC 2.12.2-1 8998 SNOC, AGGRE2NOC
       case 0x00450C00:  // SDM660 CNOC, MNOC, SNOC, AGGRE2NOC
       case 0x00419700:  // SDM630 WNOC
       {
           hw = &NOC_2_8;
           break;
       }
       default:
       {
          ABT_ERR("NOCError: Unrecognized NOC Revsion ID: 0x%08x", 
             revision_id, 0, 0);
          ERR_FATAL("NOCError: FATAL ERROR", 0, 0, 0);
          return;
       }
    }

    // Sideband Manager
    switch (revision_id) 
    {
       case 0x0036CC00:  // QNoC 2.12.2   8998 CNOC, MNOC, AGGRE1NOC, Adrastea
       case 0x0038A800:  // QNoC 2.12.2-1 8998 SNOC, AGGRE2NOC
       case 0x00450C00:  // SDM660 CNOC, MNOC, SNOC, AGGRE2NOC
       case 0x00419700:  // SDM630 WNOC
       {
           sideband = &NOC_SIDEBAND_2_12;
           // Adrastea Sideband Manager has same revision_id, but different core_id
           if (0x26FD9F0B != NOC_IN32(NOC_REG_ADDR(0,sideband->core_id)))
           {
              sideband = &NOC_SIDEBAND_Adrastea;
           }
           break;
       }
       default:
       {
           // No Sideband Manager present
           sideband = NULL;
           break;
       }
    }

    for(i=0; i < NOCERR_propdata->len; i++)
    {
        // Registration for interrupts should happen in ABT_Init()

        // Enable Interrupts (Set FAULTEN)
        if(NOCInfo[i].intr_enable)
        {
            if (sideband && (sideband->faulten != REGISTER_NOT_APPLICABLE))  // FlexNoc 2.12.2 or later
            {
                NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->faulten), (NOC_IN32(NOC_SIDEBAND_REG_ADDR(i,sideband->faulten)) | 0x01));
            }
            if (sideband && (sideband->flaginen0 != REGISTER_NOT_APPLICABLE))  // FlexNoc 2.12.2 or later
            {
                NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginen0), NOCInfo[i].sb_enable_mask);
            }
            if (sideband && (sideband->flagoutset0 != REGISTER_NOT_APPLICABLE)) // Adrastea FlexNoc 2.12.2
            {
                NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->flagoutset0), NOCInfo[i].sb_flagout_mask);
            }

            if(NOCInfo[i].intr_enable)
            {
                if (hw->errctl != REGISTER_NOT_APPLICABLE)
                {
                    NOC_OUT32(NOC_REG_ADDR(i,hw->errctl), (NOC_IN32(NOC_REG_ADDR(i,hw->errctl)) | 0x02));
                }
                if (hw->faulten != REGISTER_NOT_APPLICABLE)    // FlexNoC 2.8
                {
                    NOC_OUT32(NOC_REG_ADDR(i,hw->faulten), (NOC_IN32(NOC_REG_ADDR(i,hw->faulten)) | 0x01));
                }
            }
        }
    }

    return; 
}

//*============================================================================*/
/**
@brief
      Performs De-initialization for NOC Error Handler.

@param[in]  None.

@return
      None.

@dependencies
      None.

@sideeffects
      None.
*/
/*============================================================================*/
void NOC_Error_DeInit( void )
{
   uint8_t i;

   for(i=0; i < NOCERR_propdata->len; i++)
   {
       // Disable Interrupts (ReSet FAULTEN)
       if(NOCInfo[i].intr_enable)
       {
           if (sideband && (sideband->faulten != REGISTER_NOT_APPLICABLE))  // FlexNoc 2.12.2 or later
           {
               NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->faulten), 0x0);
           }
           if (sideband && (sideband->flaginen0 != REGISTER_NOT_APPLICABLE))  // FlexNoc 2.12.2 or later
           {
               NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->flaginen0), 0x0);
           }
           if (sideband && (sideband->flagoutclr0 != REGISTER_NOT_APPLICABLE)) // Adrastea FlexNoc 2.12.2
           {
               NOC_OUT32(NOC_SIDEBAND_REG_ADDR(i,sideband->flagoutclr0), NOCInfo[i].sb_flagout_mask);
           }

           if (hw->faulten != REGISTER_NOT_APPLICABLE)    // FlexNoC 2.8
           {
               NOC_OUT32(NOC_REG_ADDR(i,hw->faulten), 0x0);
           }

           // Clear OBS error status
           if (hw->errclr != REGISTER_NOT_APPLICABLE)    // FlexNoC 2.8
           {
               NOC_OUT32(NOC_REG_ADDR(i,hw->errclr), (NOC_IN32(NOC_REG_ADDR(i,hw->errclr)) | 0x01));
           }
       }
   }

   memset(NOCERR_syndrome_info, 0, sizeof(NOCERR_syndrome_info));
   hw = NULL;
   sideband = NULL;
   NOCInfo = NULL;
   NOCERR_propdata = NULL;
}
