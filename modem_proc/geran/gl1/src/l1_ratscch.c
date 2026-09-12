/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               L 1   D E D I C A T E D - R A T S S C H   H A N D L E R

GENERAL DESCRIPTION
    This module contains the procedures and global variables required to
    implement RATSSCH channel and protocol processing needed for GSM AMR Full
    and Half rate as per GSM 05.09 standard.

EXTERNALIZED FUNCTIONS
    ratscch_process_req
         This function processes RATSCCH request received on the D/L
    ratscch_update_req_activation_counter
         This function provides an interface to update Req_Activation_Counter
    ratscch_update_ack_activation_counter
         This function provides an interface to update Ack_Activation_Counter
    ratscch_start_ack_activation_counter
         This function provides an interface to start the Ack_Activation_Counter
    ratscch_reset_pending_proc
         This function provides an interface to reset all pending RATSCCH
         requests and counters
    ratscch_activate_req_in_dl
         This function is used by the PL1 to activate a request in the D/L
         during handovers when there is a pending RATSCCH message for which
         an ACK_OK has been sent
    ratscch_activate_req_in_ul
         This function is used by the PL1 to activate a request in the U/L
         during handovers when there is a pending RATSCCH message for which
         an ACK_OK has been sent

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                                    EDIT HISTORY FOR MODULE

   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_ratscch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
19/05/16   hd       CR1018341 Clear ratscch_ack_act_timer_started while resetting any pending RATSCCH request
27/05/15   msa      CR844425 Incorporate new diag services API for logging events with subscription id
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
10/09/13   og       CR517185. Ensure the ghdi_mvs AMR API is not called for the 
                    GSM only subscription.
07/06/13   cs       Further TSTS Updates
20/05/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
07/03/13   dv       CR461080: Introduction of new GSM VS interface
19/03/11   rc       CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
22/12/10   cja      CR262902. Add WB-AMR calls for ghdi
17/11/10   cja      Fix Klocwork issues
08/01/10   pg       CR:218676 UE does not repond with ACK_UNKNOWN message on RATSCCH
09-10-28   tjw      Clean up medium Lint errors
16/09/09   ps       Resolve Klocwork errors
11/09/09   dv       CR:194240-UE does not respond with ACK_ERROR message on RATSCCH
08/07/09   nt       CR:161980 - Further modification to ensure no changes if the
                    threshold is not set.
05/05/09   nt       CR:161980 - Unequal C/I normalization values for AMR TC 14.10.1
                    and 14.10.2.
27/11/08   ws       Added FEATURE_MVS_UNDER_GHDI to remove all direct MVS calls.
22/08/08   nt       Removed compiler warnings
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
09/07/05   pp       Initialize local variables data.dl_acs and data.ul_acs
                    (used for logging) before using them.
07/15/05   pp       Fix for 26.16.9.11 test case.
01/24/05   pp       Changes to support AMR RATSCCH request event logging. Updated
                    F3 messages
10/13/04   gsc      Fixes for 26_16_9_8 test case.
08/21/04   gsc      Ignore ACK_OK, ACK_ERR, ACK_UMKNOWN req. (to pass 26_16_9_9).
05/26/04   sv       Lint Cleanup.
03/18/04   gsc      Lint Cleanup
02/18/04   gsc      Determine if thresh/hyst pairs are defined in the RATSCCH
                    message. For AMR_CONFIG_REQ in format 2, map hystc received
                    as per specification in 05.09 section 3.4.2
01/20/04   gsc      Check for CRC and return ACK_ERR in case CRC is not valid
                    before looking at the RATSCCH request type.
11/24/03   gsc      Fill remaining entries in the ACS with some valid rates while
                    processing AMR_CONFIG_REQ if num mode received is less
                    than 4.
09/30/03   gsc      Fixed a leopard memory alignment bug.
08/11/03   gsc      Intialize the remaining entries in the threshold and
                    hysteresis tables to MAX values if less than three
                    threshold, hysteresis pairs are received in AMR_CONFIG req.
05/07/03   gsc      First revision

===========================================================================*/


/*===========================================================================

                               INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#include "comdef.h"

#ifdef FEATURE_GSM_AMR

#include "gl1_msg_g.h"
#include "gl1_voice_server.h"
#include "l1_amr.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "mcfg_nv_api.h"

/*===========================================================================

                        DEFINITIONS AND DECLARATIONS FOR MODULE

   This section contains definitions for constants, macros, types, variables
   and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
 *                       LOCAL FUNCTIONS
 *-------------------------------------------------------------------------*/

static void ratscch_process_cmi_phase_req(uint8 *data, gas_id_t gas_id);

static void ratscch_process_amr_config_req(uint8 *data,gas_id_t gas_id);

static void ratscch_process_thresh_req(uint8 *data, gas_id_t gas_id);

static boolean ratscch_req_data_consistent(gas_id_t gas_id);

static boolean ratscch_ack_act_timer_started = FALSE;

/*---------------------------------------------------------------------------
 *                       EXTERNAL DATA
 *-------------------------------------------------------------------------*/
extern cmi_phase_type curr_phase[];
extern uint8 curr_dl_icm[];
extern uint8 curr_dl_num_acm[];
extern gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern gl1_msg_amr_mode_type curr_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern uint8 curr_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_ul_icm[];
extern uint8 curr_ul_num_acm[];
extern channel_type_T amr_channel[];
extern boolean amr_thresh_hyst_defined[];

/*---------------------------------------------------------------------------
 *                       GLOBAL DATA
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * RATSCCH Response
 *-------------------------------------------------------------------------*/
uint8 ratscch_rsp[NUM_GERAN_DATA_SPACES][MAX_RATSCCH_OCTETS];
boolean ratscch_rsp_pending[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

/*---------------------------------------------------------------------------
 * RATSCCH Counters
 *-------------------------------------------------------------------------*/
static uint8 ratscch_req_act_counter[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( 0 ) };

static uint8 ratscch_ack_act_counter[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( 0 ) };

/*---------------------------------------------------------------------------
 * Phase Data
 *-------------------------------------------------------------------------*/
static cmi_phase_type new_phase[NUM_GERAN_DATA_SPACES];

/*---------------------------------------------------------------------------
 * New UL/DL ACS and ICM, Number of Codec Modes in ACS
 *-------------------------------------------------------------------------*/
static uint8 new_dl_icm[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( 0 ) };

uint8        new_ul_icm[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

#define GL1_MSG_ACS_INIT { GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF, \
                           GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF }

static gl1_msg_amr_mode_type new_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES] =
{
    GL1_MSG_ACS_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_MSG_ACS_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_TRIPLE_SIM
  , GL1_MSG_ACS_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

static gl1_msg_amr_mode_type new_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES] =
{
    GL1_MSG_ACS_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_MSG_ACS_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_TRIPLE_SIM
  , GL1_MSG_ACS_INIT
#endif /* FEATURE_TRIPLE_SIM */
};
/*---------------------------------------------------------------------------
 * Num of codec modes in ACS
 *-------------------------------------------------------------------------*/
static uint8 new_dl_num_acm[NUM_GERAN_DATA_SPACES];
static uint8 new_ul_num_acm[NUM_GERAN_DATA_SPACES];

/*---------------------------------------------------------------------------
 * New Threshold/Hysteresis data
 *-------------------------------------------------------------------------*/
#define GL1_MSG_THRESH_TBL_INIT { 0x3F, 0x3F, 0x3F }

static uint8 new_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1] = 
{ 
    GL1_MSG_THRESH_TBL_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_MSG_THRESH_TBL_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_TRIPLE_SIM
  , GL1_MSG_THRESH_TBL_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

#define GL1_MSG_HYST_TBL_INIT { 0xF, 0xF, 0xF }

static uint8 new_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1] = 
{ 
    GL1_MSG_HYST_TBL_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_MSG_HYST_TBL_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_TRIPLE_SIM
  , GL1_MSG_HYST_TBL_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

/*---------------------------------------------------------------------------
 * The last pending request that will become active at N+12
 *-------------------------------------------------------------------------*/
ratscch_req_type ratscch_pending_req[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( RATSCCH_UNDEF_REQ ) };

/*---------------------------------------------------------------------------
 * AMR Config Request UL/DL flags, to specify the param becomes
 * valid in the UL, DL or both directions.
 *-------------------------------------------------------------------------*/
static boolean AMR_Config_DLF[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean AMR_Config_ULF[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/*===========================================================================
FUNCTION: ratscch_process_req

DESCRIPTION:
   This is an interface function to the RATSCCH handler which the PL1
   layer uses when a RATSCCH message is received on the D/L. This function
   determines the type of the request received and then acts upon it by
   calling the appropriate request handlers.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
void ratscch_process_req( uint8 *data,
                          boolean crc_status,
                          gas_id_t gas_id
                        )
{
   ratscch_req_type req;

   /*
    * Should not get a ratscch req, if the ratscch_req_act_counter is
    * greater than 0. If received the standard calls for ignoring
    * the new request.
    */
   if ( ratscch_req_act_counter[gas_id] )
   {
      MSG_GERAN_ERROR_2("Rcvd new req/prev is still pending", 0, 0 );
      return;
   }

   if (!data)
   {
      MSG_GERAN_ERROR_2("Rcvd empty RATSCCH request, data = %d", data, 0 );
      return;
   }

   if (!crc_status)
   {
     /*
      * If CRC is not ok, then respond with ACK_ERR. Reset the counter
      * to 0.
      */
      MSG_GERAN_ERROR_0("Rcvd RATSCCH msg with CRC error");
      ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
      ratscch_pending_req[gas_id] = RATSCCH_UNDEF_REQ;
      ratscch_rsp_pending[gas_id] = TRUE;
      return;
   }

   /*
    * If we receive ACK_OK/ACK_ERR/ACK_UNKNOWM, simply drop the message
    */
   if ( ((data[4] & 0x07) == 0x0) &&
        (data[3] == 0x0) &&
        (data[2] == 0x0) &&
        (data[1] == 0x0) &&
        ((data[0] == 0x1) || (data[0] == 0x2) || (data[0] == 0x3) )
        )
   {
     MSG_GERAN_ERROR_0("Rcvd Invalid ratscch req");
     return;
   }

   /*
    * Increment the counter. The counter should be 1 now.
    */
   ratscch_req_act_counter[gas_id]++;

   /*
    * Let's see what RATSCCH request we got
    */
   if ( (data[4] & 0x07) == 0x1 )
   {
      req = RATSCCH_AMR_CONFIG_REQ;
      MSG_GERAN_HIGH_0("Rcvd AMR_CONFIG_REQ");
   }
   else if ( ((data[4] & 0x07 ) == 0x0) &&
             ((data[3] & 0xC0 ) == 0x40) )
   {
      req = RATSCCH_THRESH_REQ;
      MSG_GERAN_HIGH_0("Rcvd THRESH_REQ");
   }
   else if ( ((data[4] & 0x07) == 0x0) &&
             ((data[3] & 0xff) == 0x0) &&
             ((data[2] & 0xff) == 0x0) &&
             ((data[1] & 0xff) == 0x0) &&
             ((data[0] & 0xfc) == 0x4) )
   {
      req = RATSCCH_CMI_PHASE_REQ;
      MSG_GERAN_HIGH_0("Rcvd CMI_PHASE_REQ");
   }
   else
   {
      ratscch_rsp[gas_id][4] = (uint8)ACK_UNKNOWN;
      ratscch_req_act_counter[gas_id] = 0;
      ratscch_pending_req[gas_id] = RATSCCH_UNDEF_REQ;
      ratscch_rsp_pending[gas_id] = TRUE;
      MSG_GERAN_ERROR_0("Invalid ratscch req");
      return;
   }

   /*
    * Let us process the received request.
    */
   switch ( req )
   {
      case RATSCCH_CMI_PHASE_REQ:
         ratscch_process_cmi_phase_req( data, gas_id );
         break;

      case RATSCCH_AMR_CONFIG_REQ:
         ratscch_process_amr_config_req( data, gas_id );
         break;

      case RATSCCH_THRESH_REQ:
         ratscch_process_thresh_req( data, gas_id );
         break;

      default:
         break;
   }
}

/*===========================================================================
FUNCTION: ratscch_process_cmi_phase_req

DESCRIPTION:
   This is a RATSCCH request handler for a CMI_PHASE_REQ message received on
   the D/L by the PL1. If the CRC received is good, it saves the received
   D/L phase that will activated in the N+12 frame and sends an ACK_OK.
   If the received RATSCCH frame did not pass the CRC check, an ACK_ERR
   response is sent back and the REQ_ACT_COUNTER is reset to 0.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
static void ratscch_process_cmi_phase_req( uint8 *data, gas_id_t gas_id )
{
   if (!data)
   {
      MSG_GERAN_ERROR_2("Rcvd empty AMR_CMI_PHASE_REQ, data = %d", data, 0 );
      return;
   }

   /*
    * If CRC is good, save the new phase and respond back with ACK_OK
    */
   new_phase[gas_id] = (cmi_phase_type)(data[0] & 0x1);
   ratscch_rsp[gas_id][4] = (uint8)ACK_OK;

   if ( curr_phase[gas_id] != new_phase[gas_id] )
   {
      l1_log_amr_cmi_phase_change_T log_data;
      log_data.old_phase = (uint8)curr_phase[gas_id];
      log_data.new_phase = (uint8)new_phase[gas_id];
      l1_log_amr_cmi_phase_change( &log_data, gas_id );
   }

   ratscch_pending_req[gas_id] = RATSCCH_CMI_PHASE_REQ;
   ratscch_rsp_pending[gas_id] = TRUE;
}

/*===========================================================================
FUNCTION: ratscch_process_amr_config_req

DESCRIPTION:
   This is a RATSCCH request handler for a AMR_CONFIG_REQ message received on
   the D/L by the PL1. If the CRC received is good, it extracts the Active
   Codec Set, the Initial Codec Mode, the U/L and D/L Flags and the
   thresholds/hysteresis pairs from the message payload and saves the
   information received to be activated in the N+12 frame and sends an ACK_OK.

   If the received RATSCCH message did not pass the CRC check, an ACK_ERR
   response is sent back and the REQ_ACT_COUNTER is reset to 0.

   If the received RATSCCH message is inconsistent, an ACK_ERR response is
   sent back and the REQ_ACT_COUNTER is reset to 0. Inconsistency is based
   on the following criteria: TH_1 <= TH_2 <= TH_3 and
   (TH_1 + HYS_1) <= (TH_2 + HYS_2) <= (TH_3 + HYS_3)

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
static void ratscch_process_amr_config_req ( uint8 *data,gas_id_t gas_id )
{
   uint8 ulf, dlf, icm, acs;

   uint32 acs_mask = 0x0FF00000;
   uint32 icm_mask = 0x30000000;
   uint32 ulf_mask = 0x40000000;
   uint32 dlf_mask = 0x80000000;

   uint32 temp_data = 0;
   uint8 mask = 0x1;
   uint8 num_modes_rcvd = 0;
   uint16 i;
   uint8 max_modes;
   uint8 max_codec_modes = MAX_CODEC_MODES;


   if (!data)
   {
      MSG_GERAN_ERROR_2("Rcvd empty AMR_CONFIG_REQ, data = %d", data, 0 );
      return;
   }

   temp_data |= ( data[0] | (data[1] << 8 ) | ( data[2] << 16 ) | (data[3] << 24 ) );


   if ( amr_channel[gas_id] == FULL_RATE_TRAFFIC )
   {
      max_modes = MAX_FULL_RATE_MODES;

#ifdef FEATURE_GSM_AMR_WB
       if (l1_amr_get_channel_type(gas_id) == GL1_MSG_TCH_WFS)
       {
           /*We currently only support the lowest 3 WB AMR rates*/
           max_codec_modes=max_modes = MAX_WB_FULL_RATE_MODES;
       }
#endif
   }
   else
   {
      max_modes = MAX_HALF_RATE_MODES;
   }

   /*
    * Extract ACS, ICM, ULF and DLF from the payload.
    */
   acs = (uint8) ((temp_data & acs_mask) >> 20);
   icm = (uint8) ((temp_data & icm_mask) >> 28);
   ulf = (uint8) ((temp_data & ulf_mask) >> 30);
   dlf = (uint8) ((temp_data & dlf_mask) >> 31);

   MSG_GERAN_HIGH_2("Rcvd icm 0x%x, acs 0x%x in AMR_CONFIG_REQ", icm, acs);

   /*
    * Reset the number of codec modes in ACS to 0 for the corresponding
    * links based on the ULF and DLF flags. Also, update the ICM for
    * the appropriate link.
    */
   if ( ulf && dlf )
   {
      new_ul_icm[gas_id] = new_dl_icm[gas_id] = icm;
      new_ul_num_acm[gas_id] = new_dl_num_acm[gas_id] =  0;
   }
   else
   {
      /*
       * If ULF and DLF both are not set, then return with ACK_UNKNOWN
       * response. Make sure to reset the ratscch_req_act_counter
       */
      ratscch_rsp[gas_id][4] = (uint8)ACK_UNKNOWN;
      ratscch_req_act_counter[gas_id] = 0;
     ratscch_rsp_pending[gas_id] = TRUE;
      MSG_GERAN_ERROR_2("Invalid ULF/DLF combination in AMR_CONFIG_REQ", 0 , 0 );
      return;
   }

   /*
    * Now extract the codec modes from the received ACS.
    */
   for ( i = 0; i < max_modes; i++, mask <<= 1 )
   {
      if ( acs & mask )
      {
         num_modes_rcvd++;

         if ( num_modes_rcvd > max_codec_modes )
         {
            ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
            ratscch_req_act_counter[gas_id] = 0;
            ratscch_rsp_pending[gas_id] = TRUE;
            MSG_GERAN_ERROR_2("Invalid num_modes_rcvd(%d)in AMR_CONFIG_REQ (max=%d)",
                       num_modes_rcvd, max_codec_modes);
            return;
         }

         if ((new_ul_num_acm[gas_id] >= MAX_CODEC_MODES) || (new_dl_num_acm[gas_id] >= MAX_CODEC_MODES))
         {
            /* For Klocwork, but should not get here as already test for num_modes_rcvd > max_codec_modes) */
            MSG_GERAN_ERROR_2("RATSCCH num_acm over limit (%d,%d)", new_ul_num_acm[gas_id], new_dl_num_acm[gas_id]);
            return;
         }

         if (ulf && dlf)
         {
            new_ul_acs[gas_id][new_ul_num_acm[gas_id]] = (gl1_msg_amr_mode_type )i;
            new_dl_acs[gas_id][new_dl_num_acm[gas_id]] = (gl1_msg_amr_mode_type )i;
#ifdef FEATURE_GSM_AMR_WB
            if (l1_amr_get_channel_type(gas_id) == GL1_MSG_TCH_WFS)
            {
              new_ul_acs[gas_id][new_ul_num_acm[gas_id]] = (gl1_msg_amr_mode_type )
                                           (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
              new_dl_acs[gas_id][new_dl_num_acm[gas_id]] = (gl1_msg_amr_mode_type )
                                           (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
            }
            MSG_GERAN_HIGH_3("RATSCCH:new_ul_acs[x] =%d, new_dl_acs[x]=%d, x=%d",
                     new_ul_acs[gas_id][new_ul_num_acm[gas_id]],new_dl_acs[gas_id][new_dl_num_acm[gas_id]],i);
#endif
            new_ul_num_acm[gas_id]++;
            new_dl_num_acm[gas_id]++;
         }
         else if (ulf)
         {
            new_ul_acs[gas_id][new_ul_num_acm[gas_id]] = (gl1_msg_amr_mode_type )i;
#ifdef FEATURE_GSM_AMR_WB
            if (l1_amr_get_channel_type(gas_id) == GL1_MSG_TCH_WFS)
            {
              new_ul_acs[gas_id][new_ul_num_acm[gas_id]] = (gl1_msg_amr_mode_type )
                                           (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
            }
            MSG_GERAN_HIGH_2("RATSCCH: new_ul_acs[x] =%d, x=%d",
                     new_ul_acs[gas_id][new_ul_num_acm[gas_id]],i);
#endif
            new_ul_num_acm[gas_id]++;
         }
         else if (dlf)
         {
            new_dl_acs[gas_id][new_dl_num_acm[gas_id]] = (gl1_msg_amr_mode_type )i;
#ifdef FEATURE_GSM_AMR_WB
            if (l1_amr_get_channel_type(gas_id) == GL1_MSG_TCH_WFS)
            {
              new_dl_acs[gas_id][new_dl_num_acm[gas_id]] = (gl1_msg_amr_mode_type )
                                           (i + (uint16)GL1_MSG_AMR_WB_MODE_0660);
            }
            MSG_GERAN_HIGH_2("RATSCCH: new_dl_acs[x]=%d, x=%d",
                     new_dl_acs[gas_id][new_dl_num_acm[gas_id]],i);
#endif
            new_dl_num_acm[gas_id]++;
         }
      }
   }

   /*
    * If no codec mode is specified in the ACS, send and ACK_ERR
    */
   if ( !num_modes_rcvd )
   {
      ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
      ratscch_req_act_counter[gas_id] = 0;
      ratscch_rsp_pending[gas_id] = TRUE;
      MSG_GERAN_ERROR_2("Rcvd ACS with no modes in AMR_CONFIG_REQ", 0 , 0 );
      return;
   }

   /*
    * If the ICM is out of bounds wrt the ACS, return with ACK_ERR
    * response.
    */
   if (ulf && (new_ul_icm[gas_id] >= new_ul_num_acm[gas_id]) )
   {
      ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
      ratscch_req_act_counter[gas_id] = 0;
    ratscch_rsp_pending[gas_id] = TRUE;
      MSG_GERAN_ERROR_2("Invalid UL ICM in AMR_CONFIG_REQ", 0 , 0 );
      return;
   }

   /*
    * If the ICM is out of bounds wrt the ACS, return with ACK_ERR
    * response.
    */
   if (dlf && (new_dl_icm[gas_id] >= new_dl_num_acm[gas_id]) )
   {
      ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
      ratscch_req_act_counter[gas_id] = 0;
    ratscch_rsp_pending[gas_id] = TRUE;
      MSG_GERAN_ERROR_2("Invalid DL ICM in AMR_CONFIG_REQ", 0 , 0 );
      return;
   }

   /*
    * Set the global ULF and DLF flags here so that when the req
    * becomes active in N+12 frame, we will be correctly able to
    * apply the new parameters to appropriate links.
    */
   AMR_Config_ULF[gas_id] = ulf;
   AMR_Config_DLF[gas_id] = dlf;

   /*
    * If the number of modes received in the message is less than
    * MAX_CODEC_MODES (4), or the number of modes received is
    * equal to MAX_CODEC_MODES and thresh_1, hyst_1, thresh_2 and
    * hyst_2 bits are all set to 1, then update the thresh and hysteresis
    * table accordingly
    * Ref. Table 3.2.2.3.5a in GSM 05.09
    */
   if ( (num_modes_rcvd < MAX_CODEC_MODES) ||
        (( ( temp_data & 0x0000003F ) == 0x0000003F ) &&
        ( ( temp_data & 0x000003C0 ) == 0x000003C0 ) &&
        ( ( temp_data & 0x0000FC00 ) == 0x0000FC00 ) &&
        ( ( temp_data & 0x000F0000 ) == 0x000F0000 ) ))
   {
      uint32 thresh_mask_tbl[2] = { 0x0000003F, 0x0000FC00};
      uint8  thresh_shft[2] = { 0, 10};
      uint32 hyst_mask_tbl[2] = { 0x000003C0, 0x000F0000};
      uint8  hyst_shft[2] = { 6, 16};
      l1_log_amr_ratscch_req_T log_req;


      log_req = L1_LOG_AMR_CONFIG_REQ_FORMAT_1;
      l1_log_amr_ratscch_req( &log_req, gas_id );

      for ( i = 0; i < (MAX_CODEC_MODES - 2); i++ )
      {
         new_thresh_tbl[gas_id][i] =
            (uint8)(( temp_data & thresh_mask_tbl[i] ) >> thresh_shft[i]);
         new_hyst_tbl[gas_id][i] =
            (uint8)(( temp_data & hyst_mask_tbl[i] ) >> hyst_shft[i]);
      }

      new_thresh_tbl[gas_id][i] = 0x3F;
      new_hyst_tbl[gas_id][i] = 0x0F;

      if ( ratscch_req_data_consistent(gas_id) )
      {
         ratscch_rsp[gas_id][4] = (uint8)ACK_OK;
      }
      else
      {
         MSG_GERAN_ERROR_0("Inconsistent data rcvd in AMR_CONFIG_REQ");
         ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
         ratscch_req_act_counter[gas_id] = 0;
      }
   }
   else
   {
      /*
       * If the number of modes received is MAX_CODEC_MODES (4) and
       * thresh_1, hyst_1, thresh_2, and hyst_2 bits are not set to 1, then
       * extract thresh_1, thresh_2, thresh_3 and hyst_c from the
       * payload.
       * Ref. Table 3.2.2.3.5b in GSM 05.09
       */
      uint8 hyst_c;
      uint32 thresh_mask_tbl[3] = { 0x0000003F, 0x00000FC0, 0x0003F000};
      uint8 thresh_shft[3] = { 0, 6, 12};
      l1_log_amr_ratscch_req_T log_req;


      log_req = L1_LOG_AMR_CONFIG_REQ_FORMAT_2;
      l1_log_amr_ratscch_req( &log_req, gas_id );

      hyst_c = (uint8)(( temp_data & 0x000C0000 ) >> 18);

      for ( i = 0; i < (MAX_CODEC_MODES - 1); i++ )
      {
         new_thresh_tbl[gas_id][i] =
            (uint8)(( temp_data & thresh_mask_tbl[i] ) >> thresh_shft[i]);

         new_hyst_tbl[gas_id][i] = (uint8)(2 * (1+hyst_c ));
      }

      if ( ratscch_req_data_consistent(gas_id) )
      {
         ratscch_rsp[gas_id][4] = (uint8)ACK_OK;
      }
      else
      {
         MSG_GERAN_ERROR_0("Inconsistent data rcvd in AMR_CONFIG_REQ");
         ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
         ratscch_req_act_counter[gas_id] = 0;
      }
   }

   ratscch_pending_req[gas_id] = RATSCCH_AMR_CONFIG_REQ;
   ratscch_rsp_pending[gas_id] = TRUE;
}

/*===========================================================================
FUNCTION: ratscch_process_thresh_req

DESCRIPTION:
   This is a RATSCCH request handler for a THRESH_REQ message received on
   the D/L by the PL1. If the CRC received is good, it extracts the
   thresholds/hysteresis pairs from the message payload and saves the
   information received to be activated in the N+12 frame and sends an ACK_OK.

   If the received RATSCCH message did not pass the CRC check, an ACK_ERR
   response is sent back and the REQ_ACT_COUNTER is reset to 0.

   If the received RATSCCH message is inconsistent, an ACK_ERR response is
   sent back and the REQ_ACT_COUNTER is reset to 0. Inconsistency is based
   on the following criteria: TH_1 <= TH_2 <= TH_3 and
   (TH_1 + HYS_1) <= (TH_2 + HYS_2) <= (TH_3 + HYS_3)

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
static void ratscch_process_thresh_req( uint8 *data, gas_id_t gas_id )
{
   uint16 i;

   uint32 thresh_mask_tbl[3] = { 0x0000003F, 0x0000FC00, 0x03F00000};
   uint8 thresh_shft[3] = {0, 10, 20};
   uint32 hyst_mask_tbl[3] = { 0x000003C0, 0x000F0000, 0x3C000000};
   uint8 hyst_shft[3] = {6, 16, 26};

   uint32 temp_data = 0;
   l1_log_amr_ratscch_req_T log_req;

   if (!data)
   {
      MSG_GERAN_ERROR_2("Rcvd empty AMR_THRESH_REQ, data = %d", data, 0 );
      return;
   }

   log_req = L1_LOG_AMR_THRESH_REQ;
   l1_log_amr_ratscch_req( &log_req, gas_id );

   temp_data |= ( data[0] | (data[1] << 8 ) | ( data[2] << 16 ) | (data[3] << 24 ) );

   for ( i = 0; i < (MAX_CODEC_MODES - 1); i++ )
   {
      new_thresh_tbl[gas_id][i] =
         (uint8)(( temp_data & thresh_mask_tbl[i] ) >> thresh_shft[i]);
      new_hyst_tbl[gas_id][i] =
         (uint8)(( temp_data & hyst_mask_tbl[i] ) >> hyst_shft[i]);
   }

   if ( ratscch_req_data_consistent(gas_id) )
   {
      ratscch_rsp[gas_id][4] = (uint8)ACK_OK;
   }
   else
   {
      MSG_GERAN_ERROR_0("Inconsistent data rcvd in AMR_THRESH_REQ");
      ratscch_rsp[gas_id][4] = (uint8)ACK_ERR;
      ratscch_req_act_counter[gas_id] = 0;
   }

   ratscch_pending_req[gas_id] = RATSCCH_THRESH_REQ;
   ratscch_rsp_pending[gas_id] = TRUE;
}

/*===========================================================================
FUNCTION: ratscch_update_req_activation_counter

DESCRIPTION:
   This is an interface function to the PL1 layer and provides a mechanism
   for updating the RATSCCH Req_Activation_Counter. If the req_act_counter
   is 0 (which means there is no currently pending request) this function
   has no effect.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   If the req_act_counter reaches a maximum value of
   MAX_RATSCCH_COUNTER_VAL (12), the pending request in the D/L is activated
   and the req_act_counter is reset to 0.
===========================================================================*/
void ratscch_update_req_activation_counter( gas_id_t gas_id )
{
   /*
    * This counter should be greater than 0 only if there is a
    * request pending, in which case the counter is updated.
    */
   if ( ratscch_req_act_counter[gas_id] )
   {
      ratscch_req_act_counter[gas_id]++;

      MSG_GERAN_HIGH_1_G("Req Act Cnter = %d", ratscch_req_act_counter[gas_id]);

      if ( ratscch_req_act_counter[gas_id] == MAX_RATSCCH_COUNTER_VAL )
      {
         uint8 i;

         /*
          * Log request activation timer expiry event
          */
         l1_log_amr_req_act_timer_expiry_T data;

         data.dl_icm = curr_dl_icm[gas_id];
         data.dl_acs = 0;

         for (i = 0; i < curr_dl_num_acm[gas_id]; i++)
         {
            data.dl_acs |= (uint8)(1 << (uint8)curr_dl_acs[gas_id][i]);
         }

         for ( i = 0; i < MAX_CODEC_MODES - 1; i++ )
         {
            data.thresh_tbl[i] = curr_thresh_tbl[gas_id][i];
            data.hyst_tbl[i] = curr_hyst_tbl[gas_id][i];
         }

         l1_log_amr_req_act_timer_expiry( &data, gas_id );

         /*
          * Activate request on the D/L
          */
         ratscch_activate_req_in_dl(gas_id);
         ratscch_req_act_counter[gas_id] = 0;
      }
   }
}

/*===========================================================================
FUNCTION: ratscch_start_ack_activation_counter

DESCRIPTION:
   This is an interface function to PL1 which is used after a RATSCCH message
   is sent on the U/L.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
void ratscch_start_ack_activation_counter( gas_id_t gas_id )
{
   if ( ratscch_pending_req[gas_id] != RATSCCH_UNDEF_REQ )
   {
      ratscch_ack_act_counter[gas_id] = 0;
      ratscch_ack_act_timer_started = TRUE;
   }
}

/*===========================================================================
FUNCTION: ratscch_update_ack_activation_counter

DESCRIPTION:
   This is an interface function to the PL1 layer and provides a mechanism
   for updating the RATSCCH Ack_Activation_Counter. If the ack_act_counter
   is 0 (which means there is no currently pending request) this function
   has no effect.

DEPENDENCIES:
   None

RETURN VALUE:
   The updated Ack_Activation_Counter

SIDE EFFECTS:
   None
===========================================================================*/
uint8 ratscch_update_ack_activation_counter( gas_id_t gas_id )
{
   /*
    * This counter should be greater than 0 only if there is a
    * request pending, in which case the counter is updated.
    */
   if ( ratscch_ack_act_timer_started )
   {
      ratscch_ack_act_counter[gas_id]++;

      MSG_GERAN_HIGH_1("Ack Act Cnter = %d", ratscch_ack_act_counter[gas_id]);
   }

   return ratscch_ack_act_counter[gas_id];

}

/*===========================================================================
FUNCTION: ratscch_expire_ack_activation_timer

DESCRIPTION:
   This is an interface function to the PL1 layer and provides a mechanism
   for expiring the Ack_Activation_Timer.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   The New Codec Parameters will take effect from here onwards.
===========================================================================*/
void ratscch_expire_ack_activation_timer( gas_id_t gas_id )
{
         uint8 i;

         /*
          * Log request activation timer expiry event
          */

         l1_log_amr_ack_act_timer_expiry_T data;

         data.ul_icm = curr_ul_icm[gas_id];
         data.ul_acs = 0;

         for (i = 0; i < curr_ul_num_acm[gas_id]; i++)
         {
            data.ul_acs |= (uint8)(1 << (uint8)curr_ul_acs[gas_id][i]);
         }

         l1_log_amr_ack_act_timer_expiry( &data, gas_id );

         /*
          * Activate request on the U/L
          */

         ratscch_activate_req_in_ul(gas_id);
         ratscch_ack_act_counter[gas_id] = 0;
   ratscch_ack_act_timer_started = FALSE;
}

/*===========================================================================
FUNCTION: ratscch_activate_req_in_dl

DESCRIPTION:
   This function activates the currently pending RATSCCH request to take effect
   in the D/L. All three RATSCCH messages are applicable on the D/L and
   any one of these three message can be pending at a give time.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   The new RATSCCH parameters takes effect from the next speech frame. The
   RATSCCH parameter can be ACS, ICM, PHASE, THRESHOLD/HYSTERESIS pairs
   etc.
===========================================================================*/
void ratscch_activate_req_in_dl( gas_id_t gas_id )
{
   switch ( ratscch_pending_req[gas_id] )
   {
      case RATSCCH_CMI_PHASE_REQ:
         {
            curr_phase[gas_id] = new_phase[gas_id];
            MSG_GERAN_HIGH_1_G("Switching D/L phase to %d", curr_phase);
            new_phase[gas_id] = CMI_PHASE_INVALID;
            break;
         }
      case RATSCCH_AMR_CONFIG_REQ:
         {
            if (AMR_Config_DLF[gas_id])
            {
               uint16 i;

               MSG_GERAN_HIGH_0_G("Activating Config_Req in D/L");

               curr_dl_icm[gas_id] = new_dl_icm[gas_id];
               curr_dl_num_acm[gas_id] = new_dl_num_acm[gas_id];

               for ( i = 0; ((i < MAX_CODEC_MODES) && (curr_dl_num_acm[gas_id] != 0)); i++ )
               {
                  if ( i < curr_dl_num_acm[gas_id] )
                  {
                     curr_dl_acs[gas_id][i] = new_dl_acs[gas_id][i];
                  }
                  else
                  {
                     /*
                      * If we received less the 4 active modes then fill the
                      * entries in the curr_dl_acs with some valid modes. This
                      * is to ensure the MDSP gets four valid modes in the
                      * codec_set_configure command.
                      */
                     curr_dl_acs[gas_id][i] = new_dl_acs[gas_id][curr_dl_num_acm[gas_id] - 1];
                  }
               }

               for ( i = 0; i < (MAX_CODEC_MODES - 1); i++ )
               {
                  /*
                   * There is one less Thresh/Hyst pair valid than
                   * the number of active codec modes. For all others
                   * set the thresh value to 0x3F and Hyst value to 0xF.
                   */
                  if ( i < ( curr_dl_num_acm[gas_id] - 1 ) )
                  {
#ifdef FEATURE_GSM_MDSP_ESAIC
                    sys_band_T serving_cell_band = gl1_get_band ( gl1_get_serving_cell_arfcn(gas_id) );

                    if(mcfg_gcf_nv_get_status() &&
                      ( (serving_cell_band == SYS_BAND_PCS_1900) || (serving_cell_band == SYS_BAND_DCS_1800) ) &&
                      ( new_thresh_tbl[gas_id][i] != 0x3F )
                      )
                    {
                      /*Check to ensure the 2dB can be added to this threashold*/
                      if (new_thresh_tbl[gas_id][i] > 26)
                      {
                        curr_thresh_tbl[gas_id][i] = 31;
                      }
                      else
                      {
                        curr_thresh_tbl[gas_id][i] = (new_thresh_tbl[gas_id][i] + 4);
                      }
                    }
                    else
#endif
                    {
                      curr_thresh_tbl[gas_id][i] = new_thresh_tbl[gas_id][i];
                    }

                    curr_hyst_tbl[gas_id][i] = new_hyst_tbl[gas_id][i];
                  }
                  else
                  {
                     curr_thresh_tbl[gas_id][i] = 0x3F;
                     curr_hyst_tbl[gas_id][i] =  0xF;
                  }
               }

               if ( (curr_thresh_tbl[gas_id][0] == 0x3F) && (curr_hyst_tbl[gas_id][0] == 0xF))
               {
                  amr_thresh_hyst_defined[gas_id] = FALSE;
               }
               else
               {
                  amr_thresh_hyst_defined[gas_id] = TRUE;
               }

               /*---------------------------------------------------------
                * Inform MDSP about change in the ACS in D/L
                *--------------------------------------------------------*/
               gl1_msg_set_dl_acs( curr_dl_acs[gas_id],gas_id );
            }
            break;
         }
      case RATSCCH_THRESH_REQ:
         {
            uint16 i;

            MSG_GERAN_HIGH_0_G("Activating Thresh_Req in D/L");

            for ( i = 0; i < (MAX_CODEC_MODES - 1); i++ )
            {
               /*
                * There is one less Thresh/Hyst pair valid than
                * the number of active codec modes. For all others
                * set the thresh value to 0x3F and Hyst value to 0xF.
                */
               if ( i < ( curr_dl_num_acm[gas_id] - 1 ) )
               {
#ifdef FEATURE_GSM_MDSP_ESAIC
                 sys_band_T serving_cell_band = gl1_get_band ( gl1_get_serving_cell_arfcn(gas_id) );

                 if(mcfg_gcf_nv_get_status() &&
                   ( (serving_cell_band == SYS_BAND_PCS_1900) || (serving_cell_band == SYS_BAND_DCS_1800) ) &&
                   ( new_thresh_tbl[gas_id][i] != 0x3F )
                   )

                 {
                   /*Check to ensure the 2dB can be added to this threashold*/
                   if (new_thresh_tbl[gas_id][i] > 26)
                   {
                     curr_thresh_tbl[gas_id][i] = 31;
                   }
                   else
                   {
                     curr_thresh_tbl[gas_id][i] = (new_thresh_tbl[gas_id][i] + 4);
                   }
                 }
                 else
#endif
                 {
                  curr_thresh_tbl[gas_id][i] = new_thresh_tbl[gas_id][i];
                 }

                 curr_hyst_tbl[gas_id][i] = new_hyst_tbl[gas_id][i];
               }
               else
               {
                  curr_thresh_tbl[gas_id][i] = 0x3F;
                  curr_hyst_tbl[gas_id][i] =  0xF;
               }
            }

            if ( (curr_thresh_tbl[gas_id][0] == 0x3F) && (curr_hyst_tbl[gas_id][0] == 0xF))
            {
               amr_thresh_hyst_defined[gas_id] = FALSE;
            }
            else
            {
               amr_thresh_hyst_defined[gas_id] = TRUE;
            }

            break;
         }
      default:
         {
            MSG_GERAN_ERROR_0_G( "Invalid pending request");
            break;
         }
   }
}

/*===========================================================================
FUNCTION: ratscch_activate_req_in_ul

DESCRIPTION:
   This function activates the currently pending RATSCCH request to take effect
   in the U/L. Only AMR_CONFIG_REQ is applicable on the U/L.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   The new RATSCCH parameters takes effect from the next speech frame. The
   RATSCCH parameter can be ACS, ICM, THRESHOLD/HYSTERESIS pairs
   etc.
===========================================================================*/
void ratscch_activate_req_in_ul( gas_id_t gas_id )
{
   /*
    * The only RATSCCH that apply to the U/L is AMR_CONFIG_REQ.
    * The THRESH_REQ applies only to the D/L and is used to compute
    * the CMR.
    * The CMI_PHASE_REQ only changes the D/L phase. The U/L phase
    * is not configurable.
    */
   if ( ratscch_pending_req[gas_id] == RATSCCH_AMR_CONFIG_REQ )
   {
      if ( AMR_Config_ULF[gas_id] )
      {
         uint16 i;

         MSG_GERAN_HIGH_0("Activating Config_Req in U/L");

         curr_ul_icm[gas_id] = new_ul_icm[gas_id];
         curr_ul_num_acm[gas_id] = new_ul_num_acm[gas_id];

         for ( i = 0;((i < MAX_CODEC_MODES) && (curr_ul_num_acm[gas_id] != 0)); i++ )
         {
            if ( i < curr_ul_num_acm[gas_id] )
            {
               curr_ul_acs[gas_id][i] = new_ul_acs[gas_id][i];
            }
            else
            {
               curr_ul_acs[gas_id][i] = new_ul_acs[gas_id][curr_ul_num_acm[gas_id] - 1];
            }
         }
      }
   }
}

/*===========================================================================
FUNCTION: ratscch_check_consistency

DESCRIPTION:
   This is an internal function to check the consistency of the thershold/
   hysteresis pairs received in the AMR_CONFIG and THRESH_REQ messages.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
static boolean ratscch_req_data_consistent( gas_id_t gas_id )
{
   uint16 i;
   boolean result = TRUE;
   uint8 num_modes = 0;

   if ( AMR_Config_ULF[gas_id] )
   {
      num_modes = new_ul_num_acm[gas_id];
   }
   else if ( AMR_Config_DLF[gas_id] )
   {
      num_modes = new_dl_num_acm[gas_id];
   }

   for ( i = 0; ((i < num_modes - 2) && (num_modes <= MAX_CODEC_MODES)); i++ )
   {
      if ( (new_thresh_tbl[gas_id][i] <= new_thresh_tbl[gas_id][i+1] ) &&
           ( (new_thresh_tbl[gas_id][i] + new_hyst_tbl[gas_id][i]) <=
             (new_thresh_tbl[gas_id][i+1] + new_hyst_tbl[gas_id][i+1])))
      {
         continue;
      }
      else
      {
         result = FALSE;
         break;
      }
   }

   return result;
}

/*===========================================================================
FUNCTION: ratscch_reset_pending_proc

DESCRIPTION:
   This is an interface function for the PL1 layer to reset all the pending
   RATSCCH messages and counters.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
void ratscch_reset_pending_proc( gas_id_t gas_id )
{
   ratscch_req_act_counter[gas_id] = ratscch_ack_act_counter[gas_id] = 0;
   ratscch_rsp_pending[gas_id] = FALSE;
   ratscch_pending_req[gas_id] = RATSCCH_UNDEF_REQ;
   ratscch_ack_act_timer_started = FALSE;
}


/*===========================================================================
FUNCTION: ratscch_set_ul_voc_amr_mode_using_new_icm

DESCRIPTION:
   This function commands the vocoder to use the new U/L Initial Codec Mode.

DEPENDENCIES:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
   None
===========================================================================*/
void ratscch_set_ul_voc_amr_mode_using_new_icm( gas_id_t gas_id )
{
#ifndef FEATURE_MODEM_NO_VOICE_SUPPORT
  if( new_ul_acs[gas_id][new_ul_icm[gas_id]] != GL1_MSG_AMR_MODE_UNDEF )
  {
    l1_vs_set_ul_amr_mode( new_ul_acs[gas_id][new_ul_icm[gas_id]], gas_id );
  }
#endif /* FEATURE_MODEM_NO_VOICE_SUPPORT */
}


#endif /* FEATURE_GSM_AMR */
