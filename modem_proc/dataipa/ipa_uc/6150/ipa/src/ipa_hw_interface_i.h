/*!
  @file
  ipa_hw_interface_i.h

  @brief
  IPA HW Internal interface
 
  @note
  Struct packing is not in use. These structs are aligned along 4-byte 
  boundaries, other archs (ex: 8-byte aligned) must make necessary changes 
  for compatibility.

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/ipa_hw_interface_i.h#1 $
==============================================================================*/

#ifndef IPA_HW_INTERFACE_COMMON_I_H_
#define IPA_HW_INTERFACE_COMMON_I_H_
#include "global_defs.h"
#include "ipa_hw_interface.h"
#include "ipa_hw_interface_offload.h"
//#include "ipa_hw_interface_wdi.h"

#define IPA_HW_MAX_CMD_SIZE    MAX(IPA_HW_OFFLOAD_MAX_CMD_SIZE,IPA_HW_COMMON_MAX_CMD_SIZE)

enum rpm_clk_request_type {
  RPM_INVALID_REQ = 0x0,
  RPM_CLK_GATE_REQ,
  RPM_CLK_UNGATE_REQ
};

#define HAL_IPA_PC_REQ_MASK         (1 << 2)
#define HAL_IPA_RESTORE_REQ_MASK    (1 << 3)

/* Addresses of PPB registers (see IPAv3 HPG) */
#define  UC_PC_EXT_PPB_BASE                0xE0040000
#define  UC_PC_EXT_PPB_PC_RESTORE_WR       (UC_PC_EXT_PPB_BASE + 0x18)
#define  UC_PC_EXT_PPB_PC_RESTORE_RD       (UC_PC_EXT_PPB_BASE + 0x1C)
#define SET_IPA_PC_ACK_MASK      (0x1)
#define CLR_IPA_PC_ACK_MASK      (0x1 << 1)
#define SET_IPA_RESTORE_ACK_MASK (0x1 << 2)
#define CLR_IPA_RESTORE_ACK_MASK (0x1 << 3)

/**
 * @brief   Union to hold all the command enums
*/
typedef union
{
  IPA_CPU_2_HW_COMMANDS common;
  //Add new protocol cmds here
} IpaCmdOpcode_t;

/**
 * @brief   Union to hold all the response opcodes
*/
typedef union
{
  IPA_HW_2_CPU_RESPONSES common;
  //Add new protocol responses here
} IpaRspOpcode_t;

/**
 * @brief   Union to hold all the event opcodes
*/
typedef union
{
  IPA_HW_2_CPU_EVENTS common;
  //Add new protocol responses here
} IpaEvtOpcode_t;

/**
 * @brief   Union to hold all the Response status opcodes
*/
typedef union
{
  IPA_HW_ERRORS common;
  //Add new protocol response status here
} IpaRspStatusOpcode_t;

/**
 * @brief   Structure holding the parameters for IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED response.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef union
{
   struct IpaCommonCmdParams_t
   {
      uint32_t     cmdParams_lo;
      uint32_t     cmdParams_hi;
   }common;
   
   //Add other command params for other commands here
}IpaCmdParams_t;

/**
 * @brief   Structure holding the parameters for IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED response.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef union IpaCmdCompletedRspData_t
{
  struct IpaCmdCompletedRspParams_t
  {
    IpaCmdOpcode_t            cmdOp;           /**< The original command opcode */
    IpaRspStatusOpcode_t     status;
    uint16_t     reserved;
  }params;
  uint32_t raw32b;
}IpaCmdCompletedRspData_t;


typedef union IpaHwErr_t
{
  IPA_HW_ERRORS common;
  //Add other protocol errors here
}IpaHwErr_t;

typedef union IpaEventData_t
{
  IpaHwErr_t errortype;
  //Add other protocol events here

  uint32_t debug_addr;
}IpaEventData_t;

/**
 * @brief   128B shared memory located in offset zero of SW Partition in IPA SRAM. 
 * @note    The shared memory is used for communication between IPA HW and CPU. 
 *          This is an internal structure as seen by uC 
 *                                                                              
*/
typedef struct IpaHwSharedMemeMapping_t
{
   /*word 0 */
   uint8_t  cmdOp;          /**< CPU->HW command opcode. See IPA_CPU_2_HW_COMMANDS */
   uint8_t  reserved_01;
   uint16_t reserved_03_02;
   /*word 1 & 2 */
   uint32_t cmdParams_lo;        /**< CPU->HW command parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory (in such case the address must be accessible for HW) */
   uint32_t cmdParams_hi;        /**< CPU->HW command parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory (in such case the address must be accessible for HW) */
   /*word 3 */
   uint8_t  responseOp;       /**< HW->CPU response opcode. See IPA_HW_2_CPU_RESPONSES */
   uint8_t  reserved_0D;
   uint16_t reserved_0F_0E;
   /*word 4 */
   uint32_t responseParams;   /**< HW->CPU response parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 5 */
   uint8_t  eventOp;          /**< HW->CPU event opcode. See IPA_HW_2_CPU_EVENTS */
   uint8_t  reserved_15;
   uint16_t reserved_17_16;   
   /*word 6 */
   uint32_t eventParams;      /**< HW->CPU event parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 7 */
   uint32_t firstErrorAddress;  /**< Contains the address of first error-source on SNOC */
   /*word 8 */
   uint8_t  hwState;            /**< State of HW. The state carries information regarding the error type. See IPA_HW_MHI_ERRORS */
   uint8_t  reserved_21;
   uint16_t reserved_23_22;
   /*word 9 */
   uint16_t uCFwVersion;        /**< The Fw version as reported by HW*/
   uint16_t reserved_27_26; 	  
   
}IpaHwSharedMemeMapping_t;


/**
 * @brief   Global Structure holding the entire statistics information
*/
typedef struct IpaHwStatsInfoData_t
{
   IpaHwStatsCommonInfoData_t commonStats;    /**< common statistics info of hw */
}IpaHwStatsInfoData_t;

#define gDbgEvtLog (*((IpaHwEventLogInfoData_t*)IPA_HW_SRAM_DEBUG_EVENT_ADDR))                        /**< Debug event payload */
#define IPA_HW_SRAM_IPA_STATS_ADDR (IPA_HW_SRAM_DEBUG_EVENT_ADDR + sizeof(IpaHwEventLogInfoData_t))   /**< Location of the statistics strucure */
#define gIpaStats (*((IpaHwStatsInfoData_t*)IPA_HW_SRAM_IPA_STATS_ADDR))                              /**< Statistics Information */

#define SRAM_UC_MEM_PARTITION_SIZE 640  //offset 0 to offset 640 bytes belong to uC in SRAM
#define SRAM_OVERRUN_CHECK(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#endif /* IPA_HW_INTERFACE_COMMON_I_H_ */
