/*============================================================================
  FILE:         rr_cell_acq_if.c

  OVERVIEW:     This module provides functions to send signals to/from the
                RR cell acquisition module (i.e. RR_CELL_ACQ_IMSG, RR_CELL_ACQ_OMSG).

  DEPENDENCIES: None

                Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_cell_acq_if.c#1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "customer.h"
#include "comdef.h"
#include "rr_seg_load.h"
#include "rr_cell_acq_if.h"
#include "rr_rr.h"
#include "rr_multi_sim.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 *--------------------------------------------------------------------------*/


void rr_cell_acq_send_imsg_start_ind(
  ARFCN_T                        target_cell,
  uint8                          RXLEV_average,
  rr_cell_acq_bsic_info_t      * bsic_info,
  boolean                        recent_reselection_flag,
  rr_cell_acq_action_e           action,
  boolean                        sync_flag,
  boolean                        clear_si_database,
  gprs_scell_info_t            * gprs_cell_info,
  PLMN_id_T                    * requested_plmn,
  const gas_id_t                 gas_id
)
{
  rr_cell_acq_imsg_start_ind_t msg;

  msg.target_cell             = target_cell;
  msg.RXLEV_average           = RXLEV_average;

  if (bsic_info != NULL)
  {
    msg.bsic_info = *bsic_info;
  }
  else
  {
    BSIC_T invalid_bsic = {INVALID_PLMN_CC, INVALID_BSIC_CC};

    msg.bsic_info.bsic = invalid_bsic;
    msg.bsic_info.mismatch_action = RR_CELL_ACQ_BSIC_MISMATCH_ACTION_NONE;
    msg.bsic_info.send_to_l1 = FALSE;
  }

  msg.recent_reselection_flag = recent_reselection_flag;
  msg.action                  = action;
  msg.sync_flag               = sync_flag;
  msg.clear_si_database       = clear_si_database;
  msg.gprs_cell_info          = gprs_cell_info;

  if (requested_plmn != NULL)
  {
    msg.requested_plmn_valid = TRUE;
    msg.requested_plmn = *requested_plmn;
  }
  else
  {
    msg.requested_plmn_valid = FALSE;
  }

  MSG_GERAN_MED_0_G("RR_CELL_ACQ_IMSG_START_IND:");
  MSG_GERAN_MED_2_G("  target_cell:             %d (band %d)", target_cell.num, target_cell.band);
  MSG_GERAN_MED_1_G("  RXLEV_average:           %d", RXLEV_average);
  if (bsic_info != NULL)
  {
    MSG_GERAN_MED_2_G("  bsic:                    %d, %d",
                      bsic_info->bsic.PLMN_colour_code, bsic_info->bsic.BS_colour_code);
    MSG_GERAN_MED_1_G("  mismatch_action:         %d", bsic_info->mismatch_action);
    MSG_GERAN_MED_1_G("  send_to_l1:              %d", bsic_info->send_to_l1);
  }
  else
  {
    MSG_GERAN_MED_0_G("  No BSIC information");
  }
  MSG_GERAN_MED_1_G("  recent_reselection_flag: %d", recent_reselection_flag);
  MSG_GERAN_MED_1_G("  action:                  %d", action);
  MSG_GERAN_MED_1_G("  sync_flag:               %d", sync_flag);
  MSG_GERAN_MED_1_G("  clear_si_database:       %d", clear_si_database);
  MSG_GERAN_MED_1_G("  requested_plmn_valid:    %d", msg.requested_plmn_valid);
  if (msg.requested_plmn_valid)
  {
    boolean            plmn_is_undefined;
    boolean            mnc_includes_pcs_digit;
    sys_mcc_type       mcc;
    sys_mnc_type       mnc;

    sys_plmn_get_mcc_mnc(msg.requested_plmn,
                         &plmn_is_undefined,
                         &mnc_includes_pcs_digit,
                         &mcc,
                         &mnc);

    if (plmn_is_undefined)
    {
      MSG_GERAN_MED_0_G("  requested_plmn:          undefined");
    }
    else
    {
      MSG_GERAN_MED_3_G("  requested_plmn:          %d-%d (%d)", mcc, mnc, mnc_includes_pcs_digit);
    }
  }

  rr_rr_send(&msg.header,
             (uint8) RR_CELL_ACQ_IMSG_START_IND,
             sizeof(msg),
             RR_CELL_ACQ_IMSG,
             gas_id);
}


void rr_cell_acq_send_imsg_abort_req(const gas_id_t gas_id)
{
  rr_message_header_t header;

  rr_rr_send(&header,
             (uint8) RR_CELL_ACQ_IMSG_ABORT_REQ,
             sizeof(header),
             RR_CELL_ACQ_IMSG,
             gas_id);
}


void rr_cell_acq_send_imsg_surrounding_si_received_ind(const gas_id_t gas_id)
{
  rr_message_header_t header;

  rr_rr_send(&header,
             (uint8) RR_CELL_ACQ_IMSG_SURROUNDING_SI_RECEIVED_IND,
             sizeof(header),
             RR_CELL_ACQ_IMSG,
             gas_id);
}


void rr_cell_acq_send_imsg_nacc_acq_status_ind(
  boolean         pncd_enough_to_camp,
  uint16          si_messages_received,
  const gas_id_t  gas_id
)
{
  rr_cell_acq_imsg_nacc_acq_status_ind_t msg;

  MSG_GERAN_HIGH_2_G(
    "IMSG_NACC_ACQ_STATUS_IND: pncd=%d si=0x%x",
    (int)pncd_enough_to_camp,
    (int)si_messages_received
  );

  msg.pncd_enough_to_camp = pncd_enough_to_camp;
  msg.si_messages_received = si_messages_received;

  rr_rr_send(&msg.header,
             (uint8) RR_CELL_ACQ_IMSG_NACC_ACQ_STATUS_IND,
             sizeof(msg),
             RR_CELL_ACQ_IMSG,
             gas_id);
}


void rr_cell_acq_send_omsg_finished_ind(rr_cell_acq_status_e acq_status, const gas_id_t gas_id)
{
  rr_cell_acq_omsg_finished_ind_t msg;

  MSG_GERAN_HIGH_1_G(
    "OMSG_FINISHED_IND: status=%d",
    (int)acq_status
  );

  msg.cell_acq_status = acq_status;

  rr_rr_send(&msg.header,
             (uint8) RR_CELL_ACQ_OMSG_FINISHED_IND,
             sizeof(msg),
             RR_CELL_ACQ_OMSG,
             gas_id);
}


void rr_cell_acq_send_omsg_abort_cnf(const gas_id_t gas_id)
{
  rr_message_header_t header;

  rr_rr_send(&header,
             (uint8) RR_CELL_ACQ_OMSG_ABORT_CNF,
             sizeof(header),
             RR_CELL_ACQ_OMSG,
             gas_id);
}


void rr_cell_acq_send_omsg_mac_suspended_ind(const gas_id_t gas_id)
{
  rr_message_header_t header;

  rr_rr_send(&header,
             (uint8) RR_CELL_ACQ_OMSG_MAC_SUSPENDED_IND,
             sizeof(header),
             RR_CELL_ACQ_OMSG,
             gas_id);
}


void rr_cell_acq_send_omsg_reselected_ind(const gas_id_t gas_id)
{
  rr_message_header_t header;

  rr_rr_send(&header,
             (uint8) RR_CELL_ACQ_OMSG_RESELECTED_IND,
             sizeof(header),
             RR_CELL_ACQ_OMSG,
             gas_id);
}


/* EOF */

