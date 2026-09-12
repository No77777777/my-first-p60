#ifndef RXCPAGING_H
#define RXCPAGING_H

/*===========================================================================

                 C D M A    P A G I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the paging module that are
  used by the rxc unit.  This header file is internal to the rxc unit.

  Copyright (c) 1997 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

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

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$PVCSPath: O:/src/asw/COMMON/vcs/rxcpaging.h_v   1.3   28 Feb 2002 20:55:46   ldefauw  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcpaging.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/14/13   pap     Aggregating different global variables into structures.
10/12/11   jtm     Added rxc_get_pc_rate()
08/05/09   jtm     Lint fixes.
04/01/09   jtm     Added customer.h
02/28/02   lad     Removed frame_num from rxc_pc_init().
04/20/01   sr      Merged in from MSM5100 archive
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "comdef.h"
#include "customer.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
typedef struct
{
  /* number of Paging Channel messages with good CRCs */
  dword rxc_pc_crc_good;

  /* number of Paging Channel messages with bad CRCs */
  dword rxc_pc_crc_bad;

  /* MUX paging statistic counter for DIAG */
  word rxc_gpm_page_matched_counter;
  word rxc_spm_page_matched_counter;
  word rxc_srch_rssi_negative_counter;
  boolean rxc_page_increase_page_matched_cnt;

  /* Flag indicating if a message has been sent to TEST_MCC. The message
     notifies PLT_MCC that RXC has received a Paging Channel message */
  boolean ftm_pc_msg_sent;

} rxcpaging_data_t;

extern rxcpaging_data_t rxcpaging_data;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXC_PC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Paging Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_pc_isr( void );

/*===========================================================================

FUNCTION RXC_PC

DESCRIPTION
  Paging Channel state processing.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern rxc_state_type rxc_pc ( void );

/*===========================================================================

FUNCTION RXC_PC_INIT

DESCRIPTION
  This funtion initializes the Paging Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_pc_init
(
  rxc_state_type curr_state,  /* Current RXC state machine state */
  cai_pch_rate_type pc_rate   /* Rate of Paging Channel */
);

/*===========================================================================

FUNCTION RXC_GET_PC_RATE

DESCRIPTION
  This function returns the current pc_rate for PCH.

DEPENDENCIES
  None.

RETURN VALUE
  cai_pch_rate_type of either 1 = 4800bps or 0 = 9600bps

SIDE EFFECTS
  None.

===========================================================================*/
extern cai_pch_rate_type rxc_get_pc_rate(void);

#endif /* RXCPAGING_H */
