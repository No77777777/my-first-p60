/*=========================================================================*//**
    @file  ipa.h

    @brief Interface with the IPA Hardware
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2013 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
------------------------------------------------------------------------------*/


#ifndef IPA_H_
#define IPA_H_

#include "global_defs.h"
#include "nvic_drv.h"
#include "msmhwiobase.h"

//**********************************************************************************************************************************************/
// IPA HW RAM Mem Map                                                                                                                           /
//**********************************************************************************************************************************************/
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
//       |Macro                              |  Start Address                                       |  Region Size             |Notes                                                      |
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
#define  IPA_RAM_UC_MEM_ADDR_LAST            (IPA_IPA_WRAPPER_BASE_PHYS + 0x52658)	//  0x27B55C8C     // 0x0004 (4)     Bytes | The last 32-bit word in IPA HW RAM region                 |
//       FREE SPACE                          (IPA_IPA_WRAPPER_BASE_PHYS + 0x53000)	//	0x27B54400     // 0x188C (6284)  Bytes |                                                           |
#define  IPA_HW_RAM_GSI_RING_ADDR_END        (IPA_IPA_WRAPPER_BASE_PHYS + 0x52658)	//	0x27B543FC     // 0x0004 (4)     Bytes | The last 32-bit word in GSI IPA HW RAM region             |
#define  IPA_HW_RAM_GSI_RING_WDI_TR_ER_END   (IPA_IPA_WRAPPER_BASE_PHYS + 0x51FFC)	//	0x27B533FC     // N/A                  | Marker for last 32-bit word in WDI TRs and ER             |
#define  IPA_HW_RAM_GSI_RING_ADDR_START      (IPA_IPA_WRAPPER_BASE_PHYS + 0x51800)	//	0x27B52C00     // 0x17FC (6140)  Bytes | The first 32-bit word in GSI IPA HW RAM region            |
#define      IPA_RAM_UC_MEM_ADDR_FIRST       (IPA_IPA_WRAPPER_BASE_PHYS + 0x51658)	//	0x0370 (880)   Bytes                   | (FREE SPACE) The first 32-bit word in IPA HW RAM region   |
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

#define IPA_DL_CONSUMER_PIPE_ID         6

#define IPA_EOT_COAL_TIMER_TRSH_CNT     2   //counting in units of 0.5ms. Coalescing EOTs for 1ms
#define IPA_EOT_COAL_TIMER_TRSH_IN_MS   IPA_EOT_COAL_TIMER_TRSH_CNT/2
#define IPA_EOT_COAL_CNTR_TRSH_CNT      31  //Only 5 bits (Bits 12:8) available. So max is 31. Also this is per EE and not per pipe

#define POWER_COLLAPSE_WAKEUP_IND_SIG     0xFADEFADE
#define PC_RESTORE_CONTEXT_STATUS_SUCCESS 0xCAFECAFE

#define POWER_COLLAPSE_SAVE_REQ_INT_VECTOR (1<<2)
#define POWER_COLLAPSE_RESTORE_REQ_INT_VECTOR (1<<3)
#define POWER_COLLAPSE_REQ_INT_VECTOR (POWER_COLLAPSE_SAVE_REQ_INT_VECTOR | POWER_COLLAPSE_RESTORE_REQ_INT_VECTOR)

extern boolean  gIsEOTCoalEnabled;

void sw_events_Handler(void);

#define PRAGMA_PLACE_DATA_IN_PCDATA __attribute__ ((__section__(".data")))

/**
 * @brief   initialize IPA
*/
void IPA_Init(void);

/**
 * @brief   Processes RPM notifcation that indicates Clk 
 *          Gate/Ungate request
 * 
*/
void ipa_process_rpm_notif(uint8_t request_type);

/**
 * @brief   Processes RPM notifcation that indicates Clk 
 *          Gate/Ungate request
 * 
*/
void  ipa_query_pc_restore_rd_reg();

#endif /* IPA_H_ */
