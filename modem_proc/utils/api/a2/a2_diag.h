#ifndef A2_DIAG_H
  #define A2_DIAG_H
/*!
  @file a2_diag.h

  @brief
   A2 diag parameters.

*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies, Inc. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/a2/a2_diag.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
11/11/19   av      CR 2538603: HA 1.0 PHY Regression Enhancement
05/27/15   mm      CR 839366: Adding support for ATF
12/16/14   mm      CR 772110: Adding diag interface for L2
03/28/14   mm      Implemented client for QSH
02/12/14   vd      CR630063: Integrating QSH feature
12/09/13   mm      Added command to set/clear A2_DBG_MISC_EN register
02/05/13   bn      Added debug command to disable ul per flow control
12/17/12   ar      added support to print a2 pc stats
10/02/12   yuw     Enabled diag to set the destination of eMBMS data as discard
07/25/12   rp      Added the A2 log task and added stats collection & display 
07/13/12   rp      CR371608. Enable/Disable A2 DL_PER bridging via A2 diag
07/06/12   rp      CR371608 Added api's to enable/disable A2 peak data rate mode
06/05/12   rp      CR367832 Created the file to support diag commands for A2 and
                   add support for variable modification in A2.
==============================================================================*/


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include <msg.h>
#include <log.h>
#include <dsm.h>
#include <a2_common.h>
#include <diagpkt.h>
#include <diagcmd.h>
#include <diagdiag.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*! @brief
     A2 Diag "RequestItem" request payload structure
*/
typedef PACK(struct)
{
  /*! Command code requested via RequestItem(enum: a2_diag_cmd_reqitem_code_e)*/
  uint16 cmd_code;      
  /*! CMD data can be used for different purposes depending on the cmd_code*/
  uint32 cmd_data_1;
  uint32 cmd_data_2;
  uint32 cmd_data_3;
} a2_diag_req_reqitem_s;

/*! @brief Prototype for callback registered by L2 */
typedef void (*a2_diag_cb_t)(a2_diag_req_reqitem_s *);

/*! @brief
     A2 Diag cmd codes currently supported
*/
enum a2_diag_cmd_reqitem_code_e
{
  A2_DIAG_CMD_CODE_REQUEST_ITEM_MIN = 0,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PEAK_DATA_RATE = 0x1000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_DUMP_DBG_REGS_SAVE = 0x2000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_A2_LOG_STATS = 0x3000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_A2_DBG_MASK = 0x4000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_A2_ASSERT_MASK = 0x5000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_A2_EMBMS_MASK = 0x6000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PRINT_A2_PC_STATS = 0x7000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PRINT_A2_LIB_VER = 0x8000, 
  A2_DIAG_CMD_CODE_REQUEST_ITEM_A2_DBG_MISC_EN_SET = 0x9000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_ATF_CMD = 0xA000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PHY_REG_TB = 0xB000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_INVOKE_QSH = 0xD000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PRINT_QSH_CFG = 0xE000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_CHANGE_QSH_CFG = 0xF000,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_INVOKE_QSH_RESET_STATS = 0xF001,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_PRINT_QSH_STATS = 0xF002,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_ASSERT = 0xDEAD,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_M_READ = 0xDEED,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_M_WRITE = 0xFEED,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_LTE_L2_CMD_MIN = 0xFF00,
  A2_DIAG_CMD_CODE_REQUEST_ITEM_LTE_L2_CMD_MAX = 0xFF0F, /* inclusive */
  A2_DIAG_CMD_CODE_REQUEST_ITEM_MAX = 0xFFFF
};

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  a2_diag_init

==============================================================================*/
/*!
  @brief
  init function for a2_diag.c file. Should not be called by clients.

  @detail
  Function registers the diagnostic call back functions for A2

  @caller
  a2_task_main()

*/
/*============================================================================*/
void a2_diag_init
(
  void
);
/* a2_diag_init() */

/*==============================================================================

  FUNCTION:  a2_diag_lte_l2_cmd_cb_register

==============================================================================*/
/*!
  @brief
  Registers a callback function to handle commands between 
  A2_DIAG_CMD_CODE_REQUEST_ITEM_LTE_L2_CMD_MIN and 
  A2_DIAG_CMD_CODE_REQUEST_ITEM_LTE_L2_CMD_MAX.
*/
/*============================================================================*/
void a2_diag_lte_l2_cmd_cb_register
(
  a2_diag_cb_t  cb
);
/* a2_diag_lte_l2_cmd_cb_register() */

#endif /* A2_DIAG_H */
