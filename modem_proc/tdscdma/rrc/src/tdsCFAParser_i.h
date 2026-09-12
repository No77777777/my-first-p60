#ifndef TDSCFAPARSER_I_H
#define TDSCFAPARSER_I_H
/*===========================================================================
                      TDS CFA PARSER Internal

DESCRIPTION

   This module contains external definitions for the interface to CFA logging


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------

06/30/11   ps      created file

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "mmtask_v.h"
#include "rabmtask.h"
#include "comptask_api.h"
#include "cmwll.h"
#include "tctask_v.h"
#include "rr_tdsrrc_if.h"
#include "tdsrrccmd_v.h"

#ifdef FEATURE_TDSCDMA_TO_WCDMA
#include "rrc_tdsrrc_if.h"
#endif

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
#include "msgr_types.h"
#endif

/*===========================================================================
   Following functions are wrappers over external command put functions
   for adding command histroy and CFA parsing.
===========================================================================*/

void tdscfa_mm_put_cmd(mm_cmd_type *cmd_ptr);
void tdscfa_rabm_put_cmd(rabm_cmd_type *cmd_ptr);
void tdscfa_comp_put_cmd(comp_cmd_type *cmd_ptr);
void tdscfa_cm_put_cmd(cm_rpt_type *cmd_ptr);
void tdscfa_tc_put_cmd(tc_cmd_type *cmd_ptr);

#ifdef FEATURE_TDSCDMA_TO_WCDMA
void tdscfa_rrc_put_tdsrrc_cmd(rrc_tdsrrc_cmd_type *cmd_ptr);
#endif

void tdscfa_tdsrr_send_rrc_message_to_rr(void *                message_ptr,
                                         tdsrr_rrc_cmd_e_type  rrc_cmd_id,
                                         word                  length);

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
void tdscfa_send_message_to_lte_rrc(msgr_hdr_struct_type *  message_ptr, uint32  length);
#endif

void tdscfa_send_message_to_qmi(msgr_hdr_struct_type *  message_ptr, uint32  length);

void tdsrrc_wait_ms(uint32 wait_time);

void tdsrrccfa_init(void);

void tdsrrccfa_free(void);

void tdsrrc_print_trm_status(void);

#endif /* TDSCFAPARSER_I_H */

