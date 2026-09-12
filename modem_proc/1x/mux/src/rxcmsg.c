/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    C D M A    R E C E I V E   M E S S A G E   D E L I V E R Y   L A Y E R

GENERAL DESCRIPTION
  This module delivers L3 messages to the calling entity (MC).

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001 - 2013 Qualcomm Technologies, Inc.
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

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                           Edit History

$PVCSPath: O:/src/asw/COMMON/vcs/rxcmsg.c_v   1.1   16 Jan 2002 15:16:42   ldefauw  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcmsg.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
09/15/12   ag      Changes for 1xCP memory optimization.
01/11/12   srk     Feature Cleanup
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
01/28/11   jtm     Additional FTM decoupling.
01/19/11   jtm     Eliminate dependency on FTM.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
08/05/09   jtm     Lint fixes.
07/29/09   jtm     Added FTM support for RelA common channels.
04/21/09   jtm     Replaced assert.h with amssassert.h. Assert.h is being
                   phased out.
03/27/09   jtm     Eliminated implicit includes.
01/20/06   fh      Added the support for AKA and MI.
08/11/05   bn      Added logic to relay the page match event to MC.
04/20/04   jrp     Fixed lint warnings.
12/06/01   lad     Created file.

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

#ifdef FEATURE_IS2000_REL_A

//cp
#include "caix.h"
#include "caix_i.h"
#include "cai_v.h"
#include "mccrxtx.h"

//diag
#include "onex_non_sig.h"

//other
#include "amssassert.h"
#include "err.h"
#include "msg.h"
#include "m1x_diag.h"
#include "queue.h"
#include "onex_non_sig.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Flag indicating if a message has been sent to TEST_MCC. The message
   notifies TEST_MCC that RXC has received a BCCH message */
boolean ftm_ccch_msg_sent = FALSE;

/*===========================================================================

FUNCTION RXCMSG_DELIVER

DESCRIPTION
  This function delivers a message to the mcc subtask from the rxtx task.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicating success.

SIDE EFFECTS
  None.

===========================================================================*/

boolean rxcmsg_deliver (
  caix_chan_type chan,
  cai_rx_msg_type *ext_msg_ptr,
  unsigned int length,
  qword *frame_num,
  boolean page_match
)
{
  boolean success = FALSE;
  mccrxtx_cmd_type *msg_ptr;
  word status;

  ASSERT (ext_msg_ptr);
#ifdef FEATURE_1X_CP_MEM_OPT
  msg_ptr = (mccrxtx_cmd_type *) mc_alloc_queue_buf( MC_MCC_RXTX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
  msg_ptr = (mccrxtx_cmd_type *) q_get (&mcc_rxtx_free_q);
#endif /* FEATURE_1X_CP_MEM_OPT */

  if (msg_ptr == NULL) {
#ifdef FEATURE_1X_CP_MEM_OPT
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on modem heap for MC_MCC_RXTX_Q");
#else /* !FEATURE_1X_CP_MEM_OPT */
    M1X_MSG( MUX, LEGACY_ERROR,
      "No buffers on mcc_rxtx_free_q");
#endif /* FEATURE_1X_CP_MEM_OPT */
  } else {
    status = caix_ext_to_int ((word)length, chan, ext_msg_ptr, &msg_ptr->msg.msg);

    if ((status != CAIX_DONE_S)
#ifdef FEATURE_IS2000_REL_A
        /* If Ext. Encryption failure occurs, we want to send the message to
           MC anyway so it can take appropriate action by examining the
           Ext. Encryption fields. */
        && (status != CAIX_INV_ENC_MODE_NOT_SUP_S)
#endif
      ) {
      M1X_MSG( MUX, LEGACY_MED,
        "Bad PC translation: status %d",
        status);
      /* Put item back on to free q */
#ifdef FEATURE_1X_CP_MEM_OPT
     mc_free_queue_buf( msg_ptr, MC_MCC_RXTX_Q );
#else /* !FEATURE_1X_CP_MEM_OPT */
      q_put (&mcc_rxtx_free_q, &msg_ptr->msg.hdr.cmd_hdr.link);
#endif /* FEATURE_1X_CP_MEM_OPT */
    }
    else
    {
      if ( (!onex_nonsig_is_ftm_mode()) ||
           (onex_nonsig_wait_for_pc_msg() &&
            !ftm_ccch_msg_sent ) )
      {
          /* User waits for a PC message */
#ifdef FEATURE_IS2000_REL_A
        if (status == CAIX_INV_ENC_MODE_NOT_SUP_S) {
          /* Ext. Encryption failure at parser, log and pass to MC. */
          M1X_MSG( MUX, LEGACY_HIGH,
            "mccrxtx: Ext. Encryption failure, pass to MC");
          msg_ptr->msg.parser_decryption_failure = TRUE;
        } else {
          msg_ptr->msg.parser_decryption_failure = FALSE;
        }
#endif
        msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;
        msg_ptr->msg.chn = chan;

        /* message is from Paging Channel */
        qw_equ (msg_ptr->msg.frm.frame, *frame_num);
#ifndef FEATURE_1X_CP_MEM_OPT
        msg_ptr->msg.hdr.cmd_hdr.done_q_ptr = &mcc_rxtx_free_q;
#endif /* !FEATURE_1X_CP_MEM_OPT */
        msg_ptr->msg.hdr.cmd_hdr.task_ptr = NULL;
        msg_ptr->msg.page_match_counter_incremented = page_match;

        /* Put message on queue for MC */
        mccrxtx_cmd (msg_ptr);
      }

      success = TRUE;

    }
  }

  return success;
}
#endif /* FEATURE_IS2000_REL_A */
