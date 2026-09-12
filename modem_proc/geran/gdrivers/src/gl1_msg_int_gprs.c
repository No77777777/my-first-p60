/*===========================================================================

		 IMPLEMENTATION OF NPL1 INTERFERENCE MEASUREMENT(PDCH specific) FUNCTIONS

GENERAL DESCRIPTION
	 This file implements NPL1 PDCH specific power measurement functions.

EXTERNALIZED FUNCTIONS
gl1_msg_cfg_int_meas
gl1_msg_do_int_meas
gl1_msg_int_meas_hdlr

INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the order in which functions should be called
gl1_msg_cfg_int_meas
gl1_msg_do_int_meas

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                 EDIT HISTORY FOR MODULE

	This section contains comments describing changes made to the module.
	Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_int_gprs.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
17/04/15   ak       CR813711 Debug code for SCE power monitors
16/04/15   ak       CR820145 Problems with empty BA list during ded mode and with HMSC tests
25/02/15   ak       CR792349: Abort interference measurement on l1_sc_monscan_abort_gprs()
20/06/13   cs       Major Triton TSTS Syncup
07/29/05   gfr      DTM support
06/02/05   gfr      Add time_stamp to handler
04/22/05   gfr      Abort interference measures if a conflict is detected
04/20/05   yh       Remove use of RF fifo
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
05/13/03   gfr      Common power handler support.
05/12/03   gfr      Schedule power measures in the host.
05/01/03   gfr      Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
02/24/03   gfr      Lint cleanup.
11/11/02   gfr      Extra error checking to make sure scheduling is ok.
11/07/02   gfr      Added abort support.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
03/14/02   bk       Interface changes
02/08/02   bk       changed interface to add agc info.
01/14/02   sg       Initial Revision

===========================================================================*/

/*===========================================================================

                INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"

#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_sys_algo.h"

#include "geran_dual_sim.h"

/* NPL1 interference measurement states transitions */
typedef enum
{
  SM_INT_NOT_SCHEDULED = 0,
  SM_INT_MEAS_PENDING,
  SM_INT_MDSP_MEASURES,
  SM_INT_MDSP_COMPUTES,
  SM_INT_RETRIEVE
} gl1_msg_int_state_type;

typedef struct
{
 gl1_msg_int_meas_cb_type     int_meas_cb;
 gl1_hw_meas_signal_type      signal_info[GL1_DEFS_MAX_ASSIGNED_DL_TS];
 uint8			       num_ts;
 gl1_hw_rx_tags_type	       tags[GL1_DEFS_MAX_ASSIGNED_DL_TS];
 gl1_defs_int_meas_result_type	 *int_results;
 gl1_msg_int_state_type	  int_state;
}gl1_msg_int_meas_params_type;

/* NCell SCH stored parameters */
static gl1_msg_int_meas_params_type int_meas_params[NUM_GERAN_DATA_SPACES];

/* forward function decl. */
boolean gl1_msg_int_meas_hdlr(boolean is_next_frame_idle, uint32 time_stamp,gas_id_t gas_id);


/*===========================================================================
FUNCTION gl1_msgi_int_meas_init

DESCRIPTION
	 This function initializes data structures used for interference measurements.

DEPENDENCIES
	None

RETURN VALUE
	None

SIDE EFFECTS
	State reset
===========================================================================*/
void gl1_msgi_int_meas_init (gas_id_t gas_id)
{
  int_meas_params[gas_id].int_meas_cb = NULL;
  L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");
}


/*===========================================================================
FUNCTION gl1_msgi_int_meas_terminate

DESCRIPTION
  Terminates processing of any current interference measurements

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  State reset
===========================================================================*/
void gl1_msgi_int_meas_terminate (gas_id_t gas_id)
{
	 int_meas_params[gas_id].int_meas_cb = NULL;
	 if (int_meas_params[gas_id].int_state != SM_INT_NOT_SCHEDULED)
	 {
			MSG_GERAN_ERROR_2_G("Terminating active int meas: state=%d, FN=%d", int_meas_params[gas_id].int_state, GSTMR_GET_FN_GERAN(gas_id));
			L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");

			GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_TERMINATE_INT_MEAS, gas_id);

			gl1_msgi_remove_ft_handler(gl1_msg_int_meas_hdlr,gas_id);
	 }
}


/*===========================================================================
FUNCTION gl1_msg_abort_int_meas

DESCRIPTION
	 Terminates processing of any current interference measurements

===========================================================================*/
void gl1_msg_abort_int_meas (gas_id_t gas_id)
{
	 MSG_GERAN_MED_1_G("gl1_msg_abort_int_meas(): int_state: %d", int_meas_params[gas_id].int_state);

	 GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_ABORT_INT_MEAS, gas_id);

	 switch (int_meas_params[gas_id].int_state)
	 {
			case SM_INT_NOT_SCHEDULED:
				 break;

			case SM_INT_MDSP_MEASURES:
			case SM_INT_MDSP_COMPUTES:
			case SM_INT_RETRIEVE:
				 /* Abort the measure - note this also aborts power measures */
				 gl1_hw_pwr_meas_abort(gas_id);

				 /* fall through */

			case SM_INT_MEAS_PENDING:
				 MSG_GERAN_HIGH_2_G("Aborting int measures: state=%d, FN=%d", int_meas_params[gas_id].int_state, GSTMR_GET_FN_GERAN(gas_id));
				 L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");
				 gl1_msgi_remove_ft_handler(gl1_msg_int_meas_hdlr,gas_id);
				 break;

	 }
}


/*=============================================================================
FUNCTION gl1_msg_cfg_int_meas

DESCRIPTION
	 Configures the message layer to retrieve interference measurements. The
	 message layer is now ready to be commanded to retrieve interference
	 measurements

DEPENDENCIES
	 The message layer must be initialized.

RETURN VALUE
	 None

SIDE EFFECTS
	 None
============================================================================= */
void gl1_msg_cfg_int_meas
(
	 gl1_msg_int_meas_cb_type results_callback,
	 gas_id_t                 gas_id
)
{
	 /* if measurement in progress, return */
	 if (int_meas_params[gas_id].int_state != SM_INT_NOT_SCHEDULED)
			 return;

	 int_meas_params[gas_id].int_meas_cb = results_callback;
}



/*=============================================================================
FUNCTION gl1_msg_do_int_meas

DESCRIPTION
	 This function is to be called one frame prior to which interference
	 measurements are to be taken. The measurement results are copied into the
	 results buffer and the callback is invoked when all the specified
	 measurements are taken

DEPENDENCIES
	 The message layer must be configured for interference measurements.

RETURN VALUE
	 None

SIDE EFFECTS
	 Sets the measures' local info
============================================================================= */
void gl1_msg_do_int_meas
(
	 gl1_msg_meas_signal_type           signal_inf,
	 const gl1_defs_tn_type             tn[GL1_DEFS_MAX_ASSIGNED_DL_TS],
	 uint8                              num_tn,
	 const gl1_defs_int_meas_result_type *int_results_info,
	 gas_id_t                           gas_id
)
{
	 int i;

	 /* Do not schedule if we are already scheduling */
	 if (int_meas_params[gas_id].int_state != SM_INT_NOT_SCHEDULED ||
			 /* or if num_tn is invalid */
			 num_tn == 0 || num_tn > GL1_DEFS_MAX_ASSIGNED_DL_TS)
	 {
			MSG_GERAN_ERROR_1_G("Bad int meas sched (#=%d).", num_tn);
			return;
	 }

	 int_meas_params[gas_id].int_results = (gl1_defs_int_meas_result_type *)int_results_info;

	 for (i = 0; i < num_tn; i++)
	 {
			int_meas_params[gas_id].signal_info[i].arfcn                = signal_inf.arfcn;
			int_meas_params[gas_id].signal_info[i].expected_power       = signal_inf.agc_ptr->pwr_dBm_x16;
			int_meas_params[gas_id].signal_info[i].tn                   = tn[i];
			int_meas_params[gas_id].int_results->tn[i]                  = tn[i];
	 }
	 int_meas_params[gas_id].int_results->arfcn = signal_inf.arfcn;
	 int_meas_params[gas_id].int_results->num_tn = int_meas_params[gas_id].num_ts = num_tn;

	 /* Set the initial state and add the handler */
	 L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_MEAS_PENDING, "SM_INT_MEAS_PENDING");
	 gl1_msgi_add_ft_handler(gl1_msg_int_meas_hdlr, GL1_MSG_FT_HDLR_INT_MEAS,gas_id);

	 GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_DO_INT_MEAS, gas_id);
}





/*=============================================================================

DESCRIPTION
	 This function is invoked by gl1_msg_frame_tick_notification on each frame
	 tick when registered.  This function processes the intf measurement state
	 machines by scheduling the measures and retrieving the results when ready
	 in mDSP memory.

DEPENDENCIES
	 requested measures through gl1_msg_do_int_meas

RETURN VALUE
	 boolean:
			TRUE to remove the handler, FALSE otherwise

SIDE EFFECTS
	 State transitions

============================================================================ */
boolean gl1_msg_int_meas_hdlr
(
	 boolean is_next_frame_idle,
	 uint32  time_stamp,
	 gas_id_t gas_id
)
{
	 boolean remove_handler = FALSE;

	 switch (int_meas_params[gas_id].int_state)
	 {
			case SM_INT_MEAS_PENDING:
			{
				 /* hand it to mDSP a frame before the measurement */
				 if (is_next_frame_idle)
				 {
						 /* (num_ts is 0 or num_tn which is range-checked) */
						 if (!gl1_hw_int_meas_cmd(int_meas_params[gas_id].signal_info, int_meas_params[gas_id].num_ts, int_meas_params[gas_id].tags,gas_id))
						 {
								/* If we are unable to schedule, then give up */
								MSG_GERAN_ERROR_0_G("Unable to schedule interference measures");

								/* Pass results to PL1 */
								int_meas_params[gas_id].int_results->num_tn = 0;
								if (int_meas_params[gas_id].int_meas_cb)
								{
									 int_meas_params[gas_id].int_meas_cb(int_meas_params[gas_id].int_results,gas_id);
								}

								L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");

								GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_PENDING_STOP_INT_MEAS, gas_id);

								remove_handler = TRUE;
								return remove_handler;
						 }
						 int_meas_params[gas_id].int_state++; /* advance to SCHEDULED state */
				 }
				 remove_handler = FALSE;

				 break;
			}


			case SM_INT_MDSP_MEASURES:
			case SM_INT_MDSP_COMPUTES:
			{
				 /* No NPL1 processing required, advance the state */
				 int_meas_params[gas_id].int_state++;
				 remove_handler = FALSE;

				 break;
			}

			case SM_INT_RETRIEVE:
			{
				 uint32 num_measured, i;
				 gl1_hw_measurement_T measurements[MAX_MEAS_PER_FRAME];

				 /* Get results from the hardware */
				 (void) gl1_hw_pwr_meas_results
					(
						measurements,
						int_meas_params[gas_id].int_results->num_tn,
						&num_measured,
						TRUE,  /* only try once to get the results */
						int_meas_params[gas_id].tags,
						gas_id
					);

				 /* Fill in the results structure */
				 for (i = 0; i < int_meas_params[gas_id].int_results->num_tn; i++)
				 {
						int_meas_params[gas_id].int_results->measured_pwr_dBm_x16[i] = measurements[i].dBm_x16;
				 }

				 /* Pass results to PL1 */
				 if (int_meas_params[gas_id].int_meas_cb)
				 {
						int_meas_params[gas_id].int_meas_cb(int_meas_params[gas_id].int_results,gas_id);
				 }

				 /* Clean up */
				 L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");

				 GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_DONE_INT_MEAS, gas_id);

				 remove_handler = TRUE;

				 break;
			}

			default:
			{
				 /* error state, clear out the measure */
				 MSG_GERAN_ERROR_1_G("intmeas sm bad state: %d", int_meas_params[gas_id].int_state);
				 L1_STATE_CHANGE(int_meas_params[gas_id].int_state = SM_INT_NOT_SCHEDULED, "SM_INT_NOT_SCHEDULED");
				 remove_handler = TRUE;

				 break;
			}
	 }

	 return remove_handler;

/*lint -esym(715,time_stamp)*/
}

/*=============================================================================
FUNCTION gl1_msg_int_retrieving_results

DESCRIPTION
	 This function returns TRUE if i/f measurements results are waiting to be
	 retrieved

DEPENDENCIES
	 None

RETURN VALUE
	 Returns TRUE if i/f results are to be retrieved

SIDE EFFECTS
	 None
============================================================================= */
boolean gl1_msg_int_retrieving_results(gas_id_t gas_id)
{
	 /* return TRUE if i/f results are to be fetched now */
	 return (int_meas_params[gas_id].int_state == SM_INT_RETRIEVE);
}

