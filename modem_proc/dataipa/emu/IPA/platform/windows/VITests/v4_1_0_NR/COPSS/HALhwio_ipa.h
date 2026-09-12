#ifndef HAL_HWIO_IPA_H
#define HAL_HWIO_IPA_H
/*
==============================================================================

FILE:         HALhwio_ipa.h

DESCRIPTION:  
  Hardware IO abstraction APIs.

==============================================================================

                             Edit History

$Header: //components/rel/dataipa.mpss/3.2/emu/IPA/platform/windows/VITests/v4_1_0_NR/COPSS/HALhwio_ipa.h#1 $

==============================================================================
          Copyright © 2017 Qualcomm Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================
*/


/* -----------------------------------------------------------------------
** Includes
** ----------------------------------------------------------------------- */

/*
 * Common types.
 */
#include "msmhwio.h"
#include "msmhwioreg.h"
#include "msmhwiobase.h"

/* ----------------------------------------------------------------------- 
**                     IPA MACRO'S ASSISTANT
** ----------------------------------------------------------------------- */

/*
* The following set provides the service to construct the name of a macro by concatenation of its parts. 
*/
#define IPA_UNITE_ASSIST(X,Y)  X ## _ ## Y                     /* uniting (concatenation) of two part of a name into the one whole */
#define IPA_UNITE(X,Y)         IPA_UNITE_ASSIST(X,Y)           /* = X_Y */
#define IPA_HWIO_UNITE(X,Y)    IPA_UNITE(IPA_UNITE(HWIO,X),Y)  /* = HWIO_X_Y */

/* ----------------------------------------------------------------------- 
**                              IPA MACROS
** ----------------------------------------------------------------------- */

#define IPA_BASE_ADDRESS                        IPAH_SYM(CFG_REG_BASE_PHYS)

#ifdef DUT0_BASE
#define HAL_IPA_GEN_EMU_AXI_BASE                (DUT0_BASE)
#define HAL_IPA_GEN_EMU_PCIE_AMBA_BASE          MAX(GE_PCIE_DMA_BASE_PHYS, 0x100000)  /*ARM7 can not access lower 1M address space. Therefore in order to work with trace32 we would like base address to be more than 1M*/
#define HAL_IPA_GEN_EMU_USER_BASE               (GE_DUT_USER_REGS_REG_BASE_PHYS                          - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_INT_CTL_OFFSET          (GE_INT_CTL_REG_BASE                                     - HAL_IPA_GEN_EMU_AXI_BASE)
#else
#define HAL_IPA_GEN_EMU_AXI_BASE                (GEN_EMU_BASE_PHYS)
#define HAL_IPA_GEN_EMU_PCIE_AMBA_BASE          MAX(PCIE_DMA_BASE_PHYS, 0x100000)  /*ARM7 can not access lower 1M address space. Therefore in order to work with trace32 we would like base address to be more than 1M*/
#define HAL_IPA_GEN_EMU_USER_BASE               (R3PC_V3_HSP_USER_REGS_REG_BASE_PHYS                     - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_INT_CTL_OFFSET          (HSP_INT_CTL_REG_BASE_PHYS                               - HAL_IPA_GEN_EMU_AXI_BASE)
#endif

#define HAL_IPA_GEN_EMU_IPA_WRAPPER             (IPA_BASE_ADDRESS - 0x40000                              - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_IPA_BASE                (IPA_BASE_ADDRESS                                        - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_BAM_BASE                (BAM_REG_BASE_PHYS                                       - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_GSI_BASE                (GSI_TOP_GSI_REG_BASE_PHYS                               - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_IPA_VMIDMT_BASE         (IPA_VMIDMT_REG_BASE_PHYS   - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_BAM_XPU_BASE            (GSI_TOP_XPU3_REG_BASE_PHYS                              - HAL_IPA_GEN_EMU_AXI_BASE)
#define HAL_IPA_GEN_EMU_GSI_XPU_BASE            (GSI_TOP_XPU3_REG_BASE_PHYS                              - HAL_IPA_GEN_EMU_AXI_BASE)

#ifdef GE_GEN_EMU_BASE
#define HAL_IPA_GEN_EMU_IMEM_OFFSET             (GE_QSB_IMEM_REG_BASE - HAL_IPA_GEN_EMU_AXI_BASE)
/* Please, do not change it without consulting with emulation team */
#define HAL_IPA_GEN_EMU_IMEM_SIZE               (0x1c0000)
#else
#define HAL_IPA_GEN_EMU_IMEM_OFFSET             (R3PC_V3_HSP_QSB_IMEM_REG_BASE   - HAL_IPA_GEN_EMU_AXI_BASE)
/* Please, do not change it without consulting with emulation team */
#define HAL_IPA_GEN_EMU_IMEM_SIZE               (2 * 1024 * 1024)
#define HAL_IPA_GEN_EMU_IMEM_HS2_OFFSET         (R3PC_V3_HS2_QSB_IMEM_REG_BASE   - HAL_IPA_GEN_EMU_AXI_BASE)
/* Please, do not change it without consulting with emulation team */
#define HAL_IPA_GEN_EMU_IMEM_HS2_SIZE           0
#endif

#define HAL_IPA_GEN_EMU_IPA_INTERRUPT_LINE      0
#define HAL_IPA_GEN_EMU_GSI_INTERRUPT_LINE      4
#define HAL_IPA_GEN_EMU_BAM_INTERRUPT_LINE      HAL_IPA_GEN_EMU_GSI_INTERRUPT_LINE 


#define HAL_DEBUG_COUNTER_CTRL               DEBUG_CNT_CTRL
#define HAL_DEBUG_COUNTER_REG                DEBUG_CNT_REG

#define IPAH_ADDR	                                          IPAH_OFFST
#define IPAH_ADDRn	                                       IPAH_OFFSTn

#define IPA_HWIO_ADDR(hwiosym)                              IPA_HWIO_UNITE(IPAH_SYM(hwiosym),ADDR)
#define IPA_HWIO_ADDRI(hwiosym, index)                      IPA_HWIO_UNITE(IPAH_SYM(hwiosym),n_ADDR(index))
#define IPA_HWIO_ADDRI2(hwiosym, index1, index2)            IPA_HWIO_UNITE(IPAH_SYM(hwiosym),m_n_ADDR((index1), (index2)))
#define IPA_HWIO_PHYS(hwiosym)                              IPA_HWIO_ADDR(hwiosym)
#define IPA_HWIO_PHYSI(hwiosym, index)                      IPA_HWIO_ADDRI(hwiosym, index)
#define IPA_HWIO_PHYSI2(hwiosym, index1, index2)            IPA_HWIO_ADDRI2(hwiosym, index1, index2)
#define IPA_HWIO_POR(hwiosym)                               IPA_HWIO_UNITE(IPAH_SYM(hwiosym),POR)
#define IPA_HWIO_PORI(hwiosym)                              IPA_HWIO_UNITE(IPAH_SYM(hwiosym),n_POR)
#define IPA_HWIO_RMSK(hwiosym)                              IPA_HWIO_UNITE(IPAH_SYM(hwiosym),RMSK)
#define IPA_HWIO_RMSKI(hwiosym)                             IPA_HWIO_UNITE(IPAH_SYM(hwiosym),n_RMSK)

#define IPAH_OFFST(hwiosym)                                 ((IPA_HWIO_PHYS(hwiosym)) - IPA_BASE_ADDRESS)   
#define IPAH_OFFSTn(hwiosym, index)                         (IPA_HWIO_PHYSI(hwiosym, index) - IPA_BASE_ADDRESS)
#define IPAH_OFFSTn2(hwiosym, index1, index2)               (IPA_HWIO_PHYSI2(hwiosym, (index1), (index2)) - IPA_BASE_ADDRESS)

#define IPAH_BSMSK(hwiosym,field)                           IPA_HWIO_UNITE(IPAH_SYM(hwiosym),field##_BMSK) 
#define IPAH_SHFT(hwiosym,field)                            IPA_HWIO_UNITE(IPAH_SYM(hwiosym),field##_SHFT)
#define IPAH_BSMSKI(hwiosym,field)                          IPA_HWIO_UNITE(IPAH_SYM(hwiosym),n_##field##_BMSK) 
#define IPAH_SHFTI(hwiosym,field)                           IPA_HWIO_UNITE(IPAH_SYM(hwiosym),n_##field##_SHFT)

#define IPA_HWIO_IN(base, hwiosym)                          in_dword(base + IPAH_OFFST(hwiosym))                          
#define IPA_HWIO_OUT(base, hwiosym, val)                    out_dword(base + IPAH_OFFST(hwiosym), (val))         
#define IPA_HWIO_INI(base, hwiosym, index)                  in_dword(base + IPAH_OFFSTn(hwiosym,index))
#define IPA_HWIO_INI2(base, hwiosym, index1, index2)        in_dword(base + IPAH_OFFSTn2(hwiosym, (index1), (index2)))
#define IPA_HWIO_OUTI(base, hwiosym, index, val)            out_dword(base + IPAH_OFFSTn(hwiosym,index), (val))
#define IPA_HWIO_OUTI2(base, hwiosym, index1, index2, val)  out_dword(base + IPAH_OFFSTn2(hwiosym, (index1), (index2)), (val))

#define IPA_HWIO_INM(base, hwiosym, mask)                   ( IPA_HWIO_IN((base), hwiosym) & (mask))
#define IPA_HWIO_OUTM(base, hwiosym, mask, val)             IPA_HWIO_OUT((base), hwiosym, ((IPA_HWIO_IN((base), hwiosym) & (~(mask))) | ((val) & (mask))))
#define IPA_HWIO_INMI(base, hwiosym, index, mask)           ( IPA_HWIO_INI(base, hwiosym, index) & (mask))
#define IPA_HWIO_OUTMI(base, hwiosym, index, mask, val)     IPA_HWIO_OUTI(base, hwiosym, index, ((IPA_HWIO_INI(base, hwiosym, index) & (~(mask))) | ((val) & (mask))))

#define IPA_HWIO_INF(base, hwiosym, field)                  ( (IPA_HWIO_IN((base), hwiosym) & IPAH_BSMSK(hwiosym,field))>>IPAH_SHFT(hwiosym,field))
#define IPA_HWIO_OUTF(base, hwiosym, field, val)            IPA_HWIO_OUT((base), hwiosym, ((IPA_HWIO_IN((base), hwiosym) & (~(IPAH_BSMSK(hwiosym,field)))) | ((val<<IPAH_SHFT(hwiosym,field)) & (IPAH_BSMSK(hwiosym,field)))))
#define IPA_HWIO_INFI(base, hwiosym, index, field)          ( (IPA_HWIO_INI((base), hwiosym , index) & IPAH_BSMSKI(hwiosym,field))>>IPAH_SHFTI(hwiosym,field))
#define IPA_HWIO_OUTFI(base, hwiosym, index, field, val)    IPA_HWIO_OUTI((base), hwiosym, index, ((IPA_HWIO_INI((base), hwiosym,index) & (~(IPAH_BSMSKI(hwiosym,field)))) | ((val<<IPAH_SHFTI(hwiosym,field)) & (IPAH_BSMSKI(hwiosym,field)))))

#define IPA_HWIO_OUTF2I(base, hwiosym, index, field1, field2, val1, val2) \
   IPA_HWIO_OUTI(base, hwiosym, index, \
   ((IPA_HWIO_INI((base), hwiosym,index) & (~(IPAH_BSMSKI(hwiosym,field1) | IPAH_BSMSKI(hwiosym,field2)))) | \
   ((((val1) << IPAH_SHFTI(hwiosym, field1)) & IPAH_BSMSKI(hwiosym, field1)) | \
   (((val2) << IPAH_SHFTI(hwiosym, field2)) & IPAH_BSMSKI(hwiosym, field2)))))

/* ----------------------------------------------------------------------- 
**                              IPA SECURITY MACROS
** ----------------------------------------------------------------------- */

/* Translator IPA's RG offset to address
*/
#define IPA_RG_ADDR(offset)    ((IPA_BASE_ADDRESS) - 0x40000 + (offset))

/* ----------------------------------------------------------------------- 
**              MACROS for IPA_REG_BASE and IPA_WRAPPER_BASE              
** ----------------------------------------------------------------------- */

/**
* @brief   Converts the API HWIO address (actual address) to offset based IPA_REG_BASE 
*
* @note    Can be applied ONLY for an API core address
*
* @param   address       the actual address (belonging to the IPA core)
*/
#define IPA_CONVERT_ADDR_TO_REG_BASE_OFFSET( address )     ( address - IPA_BASE_ADDRESS )


/* ----------------------------------------------------------------------- 
**                              XPU MACROS
** ----------------------------------------------------------------------- */
/* see MACROS definitions for "MODULE: BAM" in msmhwioreg.h */

#define APU_IS_DEFINED            /* comment this line if APU is not defined */
/*#define MPU_IS_DEFINED            comment this line if MPU is not defined */
/*#define RPU_IS_DEFINED            comment this line if RPU is not defined */

#ifndef XPUH_SYM                    /* allow redefinition in platform level */
#define XPUH_SYM(hwiosym)           GSI_TOP_XPU3_##hwiosym
#endif

#ifndef IPA_XPU_REG_BASE_ADDRESS     /* allow redefinition in platform level */
#define IPA_XPU_REG_BASE_ADDRESS     GSI_TOP_XPU3_REG_BASE_PHYS
#endif

#define XPUH_ADDR                                            XPUH_OFFST
#define XPUH_ADDRn                                           XPUH_OFFSTn

#define XPU_HWIO_ADDR(hwiosym)                               IPA_HWIO_UNITE(XPUH_SYM(hwiosym),ADDR)
#define XPU_HWIO_ADDRI(hwiosym, index)                       IPA_HWIO_UNITE(XPUH_SYM(hwiosym),ADDR(index))
#define XPU_HWIO_ADDRI2(hwiosym, index1, index2)             IPA_HWIO_UNITE(XPUH_SYM(hwiosym),ADDR(index1, index2))
#define XPU_HWIO_PHYS(hwiosym)                               XPU_HWIO_ADDR(hwiosym)
#define XPU_HWIO_PHYSI(hwiosym, index)                       XPU_HWIO_ADDRI(hwiosym, index)
#define XPU_HWIO_PHYSI2(hwiosym, index1, index2)             XPU_HWIO_ADDRI2(hwiosym, index1, index2)
#define XPU_HWIO_POR(hwiosym)                                IPA_HWIO_UNITE(XPUH_SYM(hwiosym),POR)
#define XPU_HWIO_PORI(hwiosym, index)                        IPA_HWIO_UNITE(XPUH_SYM(hwiosym),POR(index))

#define XPUH_OFFST(hwiosym)                                 ( (XPU_HWIO_PHYS(hwiosym))                   - IPA_XPU_REG_BASE_ADDRESS )	
#define XPUH_OFFSTn(hwiosym, index)                         ( XPU_HWIO_PHYSI(hwiosym, index)             - IPA_XPU_REG_BASE_ADDRESS )
#define XPUH_OFFSTnm(hwiosym, index1, index2)               ( XPU_HWIO_PHYSI2(hwiosym, index1, index2)   - IPA_XPU_REG_BASE_ADDRESS )

#define XPUH_BSMSK(hwiosym,field)                           IPA_HWIO_UNITE(XPUH_SYM(hwiosym),field##_BMSK) 
#define XPUH_SHFT(hwiosym,field)                            IPA_HWIO_UNITE(XPUH_SYM(hwiosym),field##_SHFT)
#define XPUH_BSMSKI(hwiosym,field)                          IPA_HWIO_UNITE(XPUH_SYM(hwiosym),field##_BMSK) 
#define XPUH_SHFTI(hwiosym,field)                           IPA_HWIO_UNITE(XPUH_SYM(hwiosym),field##_SHFT)

#define XPU_HWIO_IN(base, hwiosym)                          in_dword(base + XPUH_OFFST(hwiosym))                          
#define XPU_HWIO_OUT(base, hwiosym, val)                    out_dword(base + XPUH_OFFST(hwiosym), (val))         
#define XPU_HWIO_INI(base, hwiosym, index)                  in_dword(base + XPUH_OFFSTn(hwiosym, index))
#define XPU_HWIO_OUTI(base, hwiosym, index, val)            out_dword(base + XPUH_OFFSTn(hwiosym, index), (val))
#define XPU_HWIO_INI2(base, hwiosym, index1, index2)        in_dword(base + XPUH_OFFSTnm(hwiosym, index1, index2))
#define XPU_HWIO_OUTI2(base, hwiosym, index1, index2, val)  out_dword(base + XPUH_OFFSTnm(hwiosym, index1, index2), (val))

#define XPU_HWIO_INM(base, hwiosym, mask)                   ( XPU_HWIO_IN((base), hwiosym) & (mask))
#define XPU_HWIO_OUTM(base, hwiosym, mask, val)             XPU_HWIO_OUT((base), hwiosym, ((XPU_HWIO_IN((base), hwiosym) & (~(mask))) | ((val) & (mask))))
#define XPU_HWIO_INMI(base, hwiosym, index, mask)           ( XPU_HWIO_INI(base, hwiosym, index) & (mask))
#define XPU_HWIO_OUTMI(base, hwiosym, index, mask, val)     XPU_HWIO_OUTI(base, hwiosym, index, ((XPU_HWIO_INI(base, hwiosym, index) & (~(mask))) | ((val) & (mask))))

#define XPU_HWIO_INF(base, hwiosym, field)                  ( (XPU_HWIO_IN((base), hwiosym) & XPUH_BSMSK(hwiosym,field))>>XPUH_SHFT(hwiosym,field))
#define XPU_HWIO_OUTF(base, hwiosym, field, val)            XPU_HWIO_OUT((base), hwiosym, ((XPU_HWIO_IN((base), hwiosym) & (~(XPUH_BSMSK(hwiosym,field)))) | ((val<<XPUH_SHFT(hwiosym,field)) & (XPUH_BSMSK(hwiosym,field)))))
#define XPU_HWIO_INFI(base, hwiosym, index, field)          ( (XPU_HWIO_INI((base), hwiosym , index) & XPUH_BSMSKI(hwiosym,field))>>XPUH_SHFTI(hwiosym,field))
#define XPU_HWIO_OUTFI(base, hwiosym, index, field, val)    XPU_HWIO_OUTI((base), hwiosym, index, ((XPU_HWIO_INI((base), hwiosym,index) & (~(XPUH_BSMSKI(hwiosym,field)))) | ((val<<XPUH_SHFTI(hwiosym,field)) & (XPUH_BSMSKI(hwiosym,field)))))


/* ----------------------------------------------------------------------- 
**                           IPA VMIDMT MACROS
** ----------------------------------------------------------------------- */
/* see MACROS definitions for "MODULE: IPA_VMIDMT" in msmhwioreg.h */

#define IPA_VMIDMT_IS_DEFINED             /* comment this line if IPA_VMIDMT is not defined */

#ifndef IPA_VMIDMTH_SYM                   /*  allow redefinition in platform level */
#define IPA_VMIDMTH_SYM(hwiosym)          IPA_VMIDMT_##hwiosym 
#endif

#ifndef IPA_VMIDMT_REG_BASE_ADDRESS       /* allow redefinition in platform level */
#define IPA_VMIDMT_REG_BASE_ADDRESS       IPA_VMIDMT_REG_BASE
#endif

#define IPA_VMIDMTH_ADDR                                          IPA_VMIDMTH_OFFST
#define IPA_VMIDMTH_ADDRn                                         IPA_VMIDMTH_OFFSTn

#define IPA_VMIDMT_HWIO_ADDR(hwiosym)                             IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),ADDR)
#define IPA_VMIDMT_HWIO_ADDRI(hwiosym, index)                     IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),ADDR(index))
#define IPA_VMIDMT_HWIO_ADDRI2(hwiosym, index1, index2)           IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),ADDR(index1, index2))
#define IPA_VMIDMT_HWIO_PHYS(hwiosym)                             IPA_VMIDMT_HWIO_ADDR(hwiosym)
#define IPA_VMIDMT_HWIO_PHYSI(hwiosym, index)                     IPA_VMIDMT_HWIO_ADDRI(hwiosym, index)
#define IPA_VMIDMT_HWIO_PHYSI2(hwiosym, index1, index2)           IPA_VMIDMT_HWIO_ADDRI2(hwiosym, index1, index2)
#define IPA_VMIDMT_HWIO_POR(hwiosym)                              IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),POR)
#define IPA_VMIDMT_HWIO_PORI(hwiosym, index)                      IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),POR(index))

#define IPA_VMIDMTH_OFFST(hwiosym)                                 ( (IPA_VMIDMT_HWIO_PHYS(hwiosym))                   - IPA_VMIDMT_REG_BASE_ADDRESS )   
#define IPA_VMIDMTH_OFFSTn(hwiosym, index)                         ( IPA_VMIDMT_HWIO_PHYSI(hwiosym, index)             - IPA_VMIDMT_REG_BASE_ADDRESS )
#define IPA_VMIDMTH_OFFSTnm(hwiosym, index1, index2)               ( IPA_VMIDMT_HWIO_PHYSI2(hwiosym, index1, index2)   - IPA_VMIDMT_REG_BASE_ADDRESS )

#define IPA_VMIDMTH_BSMSK(hwiosym,field)                          IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),field##_BMSK)
#define IPA_VMIDMTH_SHFT(hwiosym,field)                           IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),field##_SHFT)
#define IPA_VMIDMTH_BSMSKI(hwiosym,field)                         IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),field##_BMSK) 
#define IPA_VMIDMTH_SHFTI(hwiosym,field)                          IPA_HWIO_UNITE(IPA_VMIDMTH_SYM(hwiosym),field##_SHFT)

#define IPA_VMIDMT_HWIO_IN(base, hwiosym)                         in_dword(base + IPA_VMIDMTH_OFFST(hwiosym))                          
#define IPA_VMIDMT_HWIO_OUT(base, hwiosym, val)                   out_dword(base + IPA_VMIDMTH_OFFST(hwiosym), (val))         
#define IPA_VMIDMT_HWIO_INI(base, hwiosym, index)                 in_dword(base + IPA_VMIDMTH_OFFSTn(hwiosym,index))
#define IPA_VMIDMT_HWIO_OUTI(base, hwiosym, index, val)           out_dword(base + IPA_VMIDMTH_OFFSTn(hwiosym,index), (val))
#define IPA_VMIDMT_HWIO_INI2(base, hwiosym, index1, index2)       in_dword(base + IPA_VMIDMT_OFFSTnm(hwiosym, index1, index2))
#define IPA_VMIDMT_HWIO_OUTI2(base, hwiosym, index1, index2, val) out_dword(base + IPA_VMIDMT_OFFSTnm(hwiosym, index1, index2), (val))

#define IPA_VMIDMT_HWIO_INM(base, hwiosym, mask)                  ( IPA_VMIDMT_HWIO_IN((base), hwiosym) & (mask))
#define IPA_VMIDMT_HWIO_OUTM(base, hwiosym, mask, val)            IPA_VMIDMT_HWIO_OUT((base), hwiosym, ((IPA_VMIDMT_HWIO_IN((base), hwiosym) & (~(mask))) | ((val) & (mask))))
#define IPA_VMIDMT_HWIO_INMI(base, hwiosym, index, mask)          ( IPA_VMIDMT_HWIO_INI(base, hwiosym, index) & (mask))
#define IPA_VMIDMT_HWIO_OUTMI(base, hwiosym, index, mask, val)    IPA_VMIDMT_HWIO_OUTI(base, hwiosym, index, ((IPA_VMIDMT_HWIO_INI(base, hwiosym, index) & (~(mask))) | ((val) & (mask))))

#define IPA_VMIDMT_HWIO_INF(base, hwiosym, field)                 ( (IPA_VMIDMT_HWIO_IN((base), hwiosym) & IPA_VMIDMTH_BSMSK(hwiosym,field))>>IPA_VMIDMTH_SHFT(hwiosym,field))
#define IPA_VMIDMT_HWIO_OUTF(base, hwiosym, field, val)           IPA_VMIDMT_HWIO_OUT((base), hwiosym, ((IPA_VMIDMT_HWIO_IN((base), hwiosym) & (~(IPA_VMIDMTH_BSMSK(hwiosym,field)))) | ((val<<IPA_VMIDMTH_SHFT(hwiosym,field)) & (IPA_VMIDMTH_BSMSK(hwiosym,field)))))
#define IPA_VMIDMT_HWIO_INFI(base, hwiosym, index, field)         ( (IPA_VMIDMT_HWIO_INI((base), hwiosym , index) & IPA_VMIDMTH_BSMSKI(hwiosym,field))>>IPA_VMIDMTH_SHFTI(hwiosym,field))
#define IPA_VMIDMT_HWIO_OUTFI(base, hwiosym, index, field, val)   IPA_VMIDMT_HWIO_OUTI((base), hwiosym, index, ((IPA_VMIDMT_HWIO_INI((base), hwiosym,index) & (~(IPA_VMIDMTH_BSMSKI(hwiosym,field)))) | ((val<<IPA_VMIDMTH_SHFTI(hwiosym,field)) & (IPA_VMIDMTH_BSMSKI(hwiosym,field)))))

/* ----------------------------------------------------------------------- 
**                      R3PC_V3_HSP_USER_REGS MACROS
** ----------------------------------------------------------------------- */

#ifndef IPA_HSP_USER_REGSH_SYM                   /*  allow redefinition in platform level */
#define IPA_HSP_USER_REGSH_SYM(hwiosym)          hwiosym
#endif

#ifndef IPA_HSP_USER_REGS_REG_BASE_ADDRESS       /* allow redefinition in platform level */
#define IPA_HSP_USER_REGS_REG_BASE_ADDRESS       R3PC_V3_HSP_USER_REGS_REG_BASE_PHYS
#endif

#define IPA_HSP_USER_REGSH_ADDR                                          IPA_HSP_USER_REGSH_OFFST
#define IPA_HSP_USER_REGSH_ADDRn                                         IPA_HSP_USER_REGSH_OFFSTn

#define IPA_HSP_USER_REGS_HWIO_ADDR(hwiosym)                             IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),ADDR)
#define IPA_HSP_USER_REGS_HWIO_ADDRI(hwiosym, index)                     IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),ADDR(index))
#define IPA_HSP_USER_REGS_HWIO_ADDRI2(hwiosym, index1, index2)           IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),ADDR(index1, index2))
#define IPA_HSP_USER_REGS_HWIO_PHYS(hwiosym)                             IPA_HSP_USER_REGS_HWIO_ADDR(hwiosym)
#define IPA_HSP_USER_REGS_HWIO_PHYSI(hwiosym, index)                     IPA_HSP_USER_REGS_HWIO_ADDRI(hwiosym, index)
#define IPA_HSP_USER_REGS_HWIO_PHYSI2(hwiosym, index1, index2)           IPA_HSP_USER_REGS_HWIO_ADDRI2(hwiosym, index1, index2)
#define IPA_HSP_USER_REGS_HWIO_POR(hwiosym)                              IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),POR)
#define IPA_HSP_USER_REGS_HWIO_PORI(hwiosym, index)                      IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),POR(index))

#define IPA_HSP_USER_REGSH_OFFST(hwiosym)                                 ( (IPA_HSP_USER_REGS_HWIO_PHYS(hwiosym))                   - IPA_HSP_USER_REGS_REG_BASE_ADDRESS )   
#define IPA_HSP_USER_REGSH_OFFSTn(hwiosym, index)                         ( IPA_HSP_USER_REGS_HWIO_PHYSI(hwiosym, index)             - IPA_HSP_USER_REGS_REG_BASE_ADDRESS )
#define IPA_HSP_USER_REGSH_OFFSTnm(hwiosym, index1, index2)               ( IPA_HSP_USER_REGS_HWIO_PHYSI2(hwiosym, index1, index2)   - IPA_HSP_USER_REGS_REG_BASE_ADDRESS )

#define IPA_HSP_USER_REGSH_BSMSK(hwiosym,field)                          IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),field##_BMSK)
#define IPA_HSP_USER_REGSH_SHFT(hwiosym,field)                           IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),field##_SHFT)
#define IPA_HSP_USER_REGSH_BSMSKI(hwiosym,field)                         IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),field##_BMSK) 
#define IPA_HSP_USER_REGSH_SHFTI(hwiosym,field)                          IPA_HWIO_UNITE(IPA_HSP_USER_REGSH_SYM(hwiosym),field##_SHFT)

#define IPA_HSP_USER_REGS_HWIO_IN(base, hwiosym)                         in_dword(base + IPA_HSP_USER_REGSH_OFFST(hwiosym))                          
#define IPA_HSP_USER_REGS_HWIO_OUT(base, hwiosym, val)                   out_dword(base + IPA_HSP_USER_REGSH_OFFST(hwiosym), (val))         
#define IPA_HSP_USER_REGS_HWIO_INI(base, hwiosym, index)                 in_dword(base + IPA_HSP_USER_REGSH_OFFSTn(hwiosym,index))
#define IPA_HSP_USER_REGS_HWIO_OUTI(base, hwiosym, index, val)           out_dword(base + IPA_HSP_USER_REGSH_OFFSTn(hwiosym,index), (val))
#define IPA_HSP_USER_REGS_HWIO_INI2(base, hwiosym, index1, index2)       in_dword(base + IPA_HSP_USER_REGS_OFFSTnm(hwiosym, index1, index2))
#define IPA_HSP_USER_REGS_HWIO_OUTI2(base, hwiosym, index1, index2, val) out_dword(base + IPA_HSP_USER_REGS_OFFSTnm(hwiosym, index1, index2), (val))

#define IPA_HSP_USER_REGS_HWIO_INM(base, hwiosym, mask)                  ( IPA_HSP_USER_REGS_HWIO_IN((base), hwiosym) & (mask))
#define IPA_HSP_USER_REGS_HWIO_OUTM(base, hwiosym, mask, val)            IPA_HSP_USER_REGS_HWIO_OUT((base), hwiosym, ((IPA_HSP_USER_REGS_HWIO_IN((base), hwiosym) & (~(mask))) | ((val) & (mask))))
#define IPA_HSP_USER_REGS_HWIO_INMI(base, hwiosym, index, mask)          ( IPA_HSP_USER_REGS_HWIO_INI(base, hwiosym, index) & (mask))
#define IPA_HSP_USER_REGS_HWIO_OUTMI(base, hwiosym, index, mask, val)    IPA_HSP_USER_REGS_HWIO_OUTI(base, hwiosym, index, ((IPA_HSP_USER_REGS_HWIO_INI(base, hwiosym, index) & (~(mask))) | ((val) & (mask))))

#define IPA_HSP_USER_REGS_HWIO_INF(base, hwiosym, field)                 ( (IPA_HSP_USER_REGS_HWIO_IN((base), hwiosym) & IPA_HSP_USER_REGSH_BSMSK(hwiosym,field))>>IPA_HSP_USER_REGSH_SHFT(hwiosym,field))
#define IPA_HSP_USER_REGS_HWIO_OUTF(base, hwiosym, field, val)           IPA_HSP_USER_REGS_HWIO_OUT((base), hwiosym, ((IPA_HSP_USER_REGS_HWIO_IN((base), hwiosym) & (~(IPA_HSP_USER_REGSH_BSMSK(hwiosym,field)))) | ((val<<IPA_HSP_USER_REGSH_SHFT(hwiosym,field)) & (IPA_HSP_USER_REGSH_BSMSK(hwiosym,field)))))
#define IPA_HSP_USER_REGS_HWIO_INFI(base, hwiosym, index, field)         ( (IPA_HSP_USER_REGS_HWIO_INI((base), hwiosym , index) & IPA_HSP_USER_REGSH_BSMSKI(hwiosym,field))>>IPA_HSP_USER_REGSH_SHFTI(hwiosym,field))
#define IPA_HSP_USER_REGS_HWIO_OUTFI(base, hwiosym, index, field, val)   IPA_HSP_USER_REGS_HWIO_OUTI((base), hwiosym, index, ((IPA_HSP_USER_REGS_HWIO_INI((base), hwiosym,index) & (~(IPA_HSP_USER_REGSH_BSMSKI(hwiosym,field)))) | ((val<<IPA_HSP_USER_REGSH_SHFTI(hwiosym,field)) & (IPA_HSP_USER_REGSH_BSMSKI(hwiosym,field)))))

/* ----------------------------------------------------------------------- 
**                              IPA BAM MACROS
** ----------------------------------------------------------------------- */

#ifndef IPA_BAMH_SYM	/* allow redefinition in platform level */
#define IPA_BAMH_SYM(hwiosym)			 BAM_##hwiosym
#endif

#define IPA_BAM_BASE_ADDRESS                                IPA_BAMH_SYM(REG_BASE_PHYS)

#define IPA_BAM_HWIO_ADDR(hwiosym)                          IPA_HWIO_UNITE(IPA_BAMH_SYM(hwiosym),ADDR)
#define IPA_BAM_HWIO_ADDRI(hwiosym, index)                  IPA_HWIO_UNITE(IPA_BAMH_SYM(hwiosym),ADDR(index))
#define IPA_BAM_HWIO_PHYS(hwiosym)                          IPA_BAM_HWIO_ADDR(hwiosym)

#define IPA_BAMH_OFFST(hwiosym)                             ((IPA_BAM_HWIO_PHYS(hwiosym)) - IPA_BAM_BASE_ADDRESS)   

#define IPA_BAMH_BSMSK(hwiosym,field)                       IPA_HWIO_UNITE(IPA_BAMH_SYM(hwiosym),IPA_UNITE(field,BMSK)) 
#define IPA_BAMH_SHFT(hwiosym,field)                        IPA_HWIO_UNITE(IPA_BAMH_SYM(hwiosym),IPA_UNITE(field,SHFT))

#define IPA_BAM_HWIO_IN(base, hwiosym)                      in_dword(base + IPA_BAMH_OFFST(hwiosym))                          

#define IPA_BAM_HWIO_INF(base, hwiosym, field)              ( (IPA_BAM_HWIO_IN((base), hwiosym) & IPA_BAMH_BSMSK(hwiosym,field))>>IPA_BAMH_SHFT(hwiosym,field))


#endif /* HAL_HWIO_IPA_H */

