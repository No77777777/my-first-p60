/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                   G L 1  A R B I T R A T O R  U T I L S

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_utils.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/03/18   sn        CR2187111 CXM timelines correction at GL1 for GBTA collision scenarios 
29/10/17   km        CR2133917 GL1 support for L+L
06/11/15   ab        CR927639 : CXM In X2G QDTA use desense_band_id as freq id to avoid X blanking.
19/11/15   ab        CR939771 In G+G , make sure both SUBS operate in same GARB multi-sim mode in SR/DR mode.
20/07/15   shr       CR866607 Observing yield/proceed on both subs when in collision during data transfer on one sub
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
17/06/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
27/05/15   mc        CR.840262 : COEX handling for Dual-Multimode DR-DSDS
14/05/15   mc        CR.821519 : TH 2.0: WLAN COEX timeline reporting errors for FACCH and SDCCH
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
17/03/15   ab        GL1 COEX : CR804948 Handling of multiple RRBP during conflict resolution
07/04/15   shr       CR808932: COEX support for DTM
13/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
23/12/14   ggu       CR772195: No need to continue check gbta_allowed status when there is no any G sub is under TRANS state
23/12/14   shr       CR747635: Unification of micro priority tiers for GERAN
12/12/14   pa        CR763859: Handling of RxD clients in GL1 TRM structures.
03/12/14   npt       CR.759467 : Fix compiler warnings
13/11/14   npt       CR.746180 : Remove KW errors 
29/10/14   ab        CR.745429 : Featurization correction for PCHXFER for SS / CR merges.
26/09/14   mc        CR.719955 : COEX : X+G[IDLE] DSDA - IDLE monitor not protected COEX with TIER_10 assignment
23/08/14   mc        CR.707745 : [BOLT2.0][SG+G]: Incorrect CXM priority assignment policy in use for SGLTE+G
08/04/14   mc        CR.700892 : Incorrect CXM priority assignment policy used in SGLTE+G
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
17/06/14   cs/ch     CR.651795 : TRM Updated changes for DR-DSDS
21/05/14   ssh       CR.668643 : gl1_hw_qta_gap_active() should return FALSE for multimode sub
15/04/14   zc        CR.649122 : Treat SGLTE+G DSDA rx/tx priority as X+G DSDA priorities for multisim mode
11/04/14   mc        CR.639138 : COEX : DL slots during UL FACCH blocks should be identified as type TCH
18/03/14   zc        CR.634586 : Treat Single-Sim SGLTE/TDS same as X+G DSDA
10/03/14   mc        CR.628061 : Incorrect FNMod counter used for IDLE and SACCH frame identification
19/02/14   ab        CR.615709 : DSDA: Priortise FACCH over SACCH for quicker L2 Link Establishment or Handover
12/02/14   am        CR.611265 : RACH timing update to GARB
29/11/13   js        CR.574402 : COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
09/11/13   mc        CR.556742 : COEX : G+G : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
15/11/13   ap        CR.575548 : FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
09/10/13   cs        CR.532298 : Syncup for wakeup-mgr changes
01/10/13   sk        CR.552259 : CR sync from 1.0/1.1
23/08/13   sk        CR.533467 : Partial QBTA bringup on 2.0 in DSDS
15/08/13   sk        CR.524039 : GBTA bringup changes
29/07/13   sk        CR.519663 : Partial QBTA bringup changes
06/06/13   mc        CR.492352 : COEX Support : Per-Slot Channel Indication (for RRBP) + COEX params for monitors
15/05/13   mc        CR.476825 : COEX Support - GERAN arbitrator SW CXM integration - Bring-up fixes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_COEX_SW_CXM

#include <stdlib.h> /* rand(), RAND_MAX */
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_types.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_if_g.h"
#include "gmacsigl1.h"
#include "gmacsigl1_g.h"
#include "gpl1_gprs_rrbp.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_ptcch.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gpl1_dual_sim.h"
#include "gl1_hw_g.h"
#include "l1_utils.h"
#include "wwcoex_action_iface.h"


/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
 /* GSM Channel frequency definitions (in kHz) */
#define BASE_FREQ_GSM_850_BAND    824200
#define BASE_FREQ_GSM_900_BAND    890000
#define BASE_FREQ_DCS_1800_BAND   1710200
#define BASE_FREQ_PCS_1900_BAND   1850200

#define FREQ_GSM_850_BAND_OFFSET  45000
#define FREQ_GSM_900_BAND_OFFSET  45000
#define FREQ_DCS_1800_BAND_OFFSET 95000
#define FREQ_PCS_1900_BAND_OFFSET 80000

#define GARB_MSGBUFF_SIZE 500

#define VBATT_BACKOFF_THRESHOLD 5
#define VBATT_SHARE_PAIN_1 1.0
#define VBATT_SHARE_PAIN_2 0.0
#define VBATT_TXP_BUDGET_DBM 33
#define EXTRA_GUARD_RXLM_PROG 100
#define EXTRA_GUARD_TXLM_PROG 155

#define SGN(a) ((a) > 0) ? 1 : (((a) < 0) ? -1 : 0)
#define ABS(a) ((a)<0?0-(a):(a))

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[]; /* GPLUSG_OPERATION_IS_ACTIVE() */

extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_COEX
extern boolean L1_is_rach_next ( uint16 fn_mod51, gas_id_t gas_id );
#endif /* FEATURE_GSM_COEX */
/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION      garb_peer_gas_id

DESCRIPTION   Returns the GAS ID of the peer SUB

RETURN VALUE  gas_id

===========================================================================*/
gas_id_t garb_peer_gas_id(gas_id_t gas_id)
{
  gas_id_t peer_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

#ifdef FEATURE_TRIPLE_SIM
  gas_id_t ptm_gas_id = gl1_get_gas_id_for_ptm_client();

  if( ptm_gas_id == gas_id )
  {
    peer_gas_id = gl1_get_peer_gas_id_for_ptm_client();
  }
  else if( ptm_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    peer_gas_id = ptm_gas_id;
  }
  else if ( gl1_hw_qta_gap_active(gas_id))
  {
    peer_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }
#else
#ifdef FEATURE_DUAL_SIM
  peer_gas_id = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? GERAN_ACCESS_STRATUM_ID_2 : GERAN_ACCESS_STRATUM_ID_1);
#else
  peer_gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_TRIPLE_SIM */

  if(peer_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED)
  {
    #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    peer_gas_id = gl1_get_gas_id_of_other_g_sub(gas_id);
    #endif
    
    return ((peer_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED) ? gas_id : peer_gas_id);
  }

  return peer_gas_id;
}

/*===========================================================================

FUNCTION      garb_is_g_plus_g

DESCRIPTION   This function indicates that G+G is active

RETURN VALUE  gas_id

===========================================================================*/
boolean garb_is_g_plus_g(void)
{
   boolean rc = FALSE;
#ifdef FEATURE_DUAL_SIM
   rc = GONLY_OPERATION_IS_ACTIVE();
#endif /* FEATURE_DUAL_SIM */
   return rc;
}


/*===========================================================================

FUNCTION      garb_get_rx_power_in_find_bcch

DESCRIPTION   This function gets rx power during specific bcch/reselection.
              
RETURN VALUE  rx power

===========================================================================*/
int32 garb_get_rx_power_in_find_bcch(gas_id_t gas_id)
{
  if (arbitrated_gas[gas_id].params_find_bcch.arfcn.num != INVALID_ARFCN)
  {
    return (arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[0]);
  }
  else
  {
    return GL1_CXM_UNKNOWN_POWER;
  }
}

/*===========================================================================

FUNCTION      garb_arfcn_to_centre_freq_kHz

DESCRIPTION   Returns the centre frequency in kHz for the specified ARFCN
              and band.

RETURN VALUE  Frequency in kHz

===========================================================================*/
uint32 garb_arfcn_to_centre_freq_kHz(ARFCN_T arfcn, garb_rxtx_t rxtx, gas_id_t gas_id)
{
   int32 freq = 0;

   switch ( arfcn.band )
   {
      case SYS_BAND_DCS_1800:
         freq = ( BASE_FREQ_DCS_1800_BAND + ( 200 * ( arfcn.num - 512 ) ) + FREQ_DCS_1800_BAND_OFFSET );
         if(rxtx==GARB_TX) freq-=95000;
         break;

      case SYS_BAND_PCS_1900:
         freq = ( BASE_FREQ_PCS_1900_BAND + ( 200 * ( arfcn.num - 512 ) ) + FREQ_PCS_1900_BAND_OFFSET );
         if(rxtx==GARB_TX) freq-=80000;
         break;

      case SYS_BAND_CELL_850:
         freq = ( BASE_FREQ_GSM_850_BAND  + ( 200 * ( arfcn.num - 128 ) ) + FREQ_GSM_850_BAND_OFFSET );
         if(rxtx==GARB_TX) freq-=45000;
         break;

      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
         if ( arfcn.num <= 124 )
         {
            freq = ( BASE_FREQ_GSM_900_BAND + ( 200 * arfcn.num ) + FREQ_GSM_900_BAND_OFFSET );
         }
         else
         {
            freq = ( BASE_FREQ_GSM_900_BAND + ( 200 * ( arfcn.num - 1024 ) ) + FREQ_GSM_900_BAND_OFFSET );
         }

         if(rxtx==GARB_TX) freq-=45000;
         break;

     default:
         MSG_GERAN_ERROR_1_G("GARB: Unsupported RF band: %d", arfcn.band);
         break;
   }

   return ( freq );
 }

/*===========================================================================

FUNCTION      garb_is_peer_tech_active_tdd

DESCRIPTION   Returns if peer tech is TDD/FDD

RETURN VALUE  None

===========================================================================*/
boolean garb_is_peer_tech_active_tdd(gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
   if(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
   { 
      return (arbitrated_gas[gas_id].peer_x_tech_is_tdd);
   }      
   else
#endif /*  (FEATURE_DUAL_SIM) || (FEATURE_SGLTE) */   
   {
     return FALSE;
   }   
}  

/*===========================================================================

FUNCTION      garb_get_multi_rat_mode

DESCRIPTION   This function determines the current Multi-RAT Mode

RETURN VALUE  Multi-RAT Mode

===========================================================================*/
garb_multi_sim_mode_t garb_get_multi_rat_mode(gas_id_t gas_id)
{
   garb_multi_sim_mode_t mode = GARB_MODE_INVALID;

#ifdef FEATURE_SGLTE
   gas_id_t   the_other_gas_id;
   gl1_trm_state_t trm_status = GL1_TRM_NULL;
#endif /* FEATURE_SGLTE */

   sys_modem_device_mode_e_type multi_sim_sys_mode = gl1_msg_get_multi_sim_sys_mode();
      
   /* atleast make sure in G+G , GARB multisim_mode consider drds mode of both sub */
   if (  GONLY_OPERATION_IS_ACTIVE() )
   {
      if ( (multi_sim_sys_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) &&
           (gl1_drdsds_enabled(gas_id))  && 
           (gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))))
      {
        /* Override multi sim mode  to DSDA for all COEX protection */
        multi_sim_sys_mode = SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE;
      }
   }
   else if ( (multi_sim_sys_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) && (gl1_drdsds_enabled(gas_id)))
   {
      /* Override multi sim mode  to DSDA for all COEX protection */
      multi_sim_sys_mode = SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE;
   }

   switch(multi_sim_sys_mode)
   {
   case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
      mode = garb_is_g_plus_g() ? GARB_MODE_GPG_DSDS : GARB_MODE_XPG_DSDS;
      break;
   case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
      /* The CXM peer TECH is expected to contain at most ONE tech (with the exception of SGLTE + G configs)
         IRAT target techs are been filtered out, as has the GSM1 tech in SGLTE + G */
      switch(arbitrated_gas[gas_id].cxm_peer_tech)
      {
      case (1<<CXM_TECH_ONEX): /* C+G DSDA */
      case (1<<CXM_TECH_HDR): /* C+G DSDA */
         mode = GARB_MODE_CPG_DSDA;
         break;
      case (1<<CXM_TECH_WCDMA):     /* W is my DSDA peer */
      case (1<<CXM_TECH_WCDMA2):    /* W2 is my DSDA peer */
         mode = GARB_MODE_WPG_DSDA;
         break;
      case (1<<CXM_TECH_TDSCDMA):   /* T is my DSDA peer */
         mode = GARB_MODE_TPG_DSDA;
         break;
      case (1<<CXM_TECH_LTE):       /* L is my DSDA peer */
      case (1<<CXM_TECH_LTE2):    /* L2 is my DSDA peer */

         mode = GARB_MODE_LPG_DSDA;
         break;
      case (1<<CXM_TECH_LTE) + (1<<CXM_TECH_GSM1):     /* I (GSM2) am the G only SUB in an SGLTE + G configuration */
      case (1<<CXM_TECH_LTE) + (1<<CXM_TECH_GSM2):     /* I (GSM1) am the G only SUB in an SGLTE + G configuration */
         mode = GARB_MODE_LPG_DSDA;
         break;
      case (1<<CXM_TECH_WCDMA2) + (1<<CXM_TECH_GSM1):  /* I (GSM2) am the G only SUB in an SGLTE + G configuration */
      case (1<<CXM_TECH_WCDMA ) + (1<<CXM_TECH_GSM2):  /* I (GSM1) am the G only SUB in an SGLTE + G configuration */
         mode = GARB_MODE_LPG_DSDA;
         break;
      case (1<<CXM_TECH_TDSCDMA) + (1<<CXM_TECH_GSM1): /* I (GSM2) am the G only SUB in an SGLTE + G configuration */
      case (1<<CXM_TECH_TDSCDMA) + (1<<CXM_TECH_GSM2): /* I (GSM1) am the G only SUB in an SGLTE + G configuration */
         mode = GARB_MODE_LPG_DSDA;
         break;
      default:
         mode = garb_is_g_plus_g() ? GARB_MODE_GPG_DSDA : GARB_MODE_XPG_DSDA;
         break;
      }
#ifdef FEATURE_SGLTE
      /* when phone is in SGLTE+G mode, G has to apply either G+G or X+G policy based on the
         current situation. It could be changed from frame to frame.
         TODO : This needs a better solution as we are commiting an entire frames worth of
         activity based on the QTA gap status at the time of this ISR tick. This can cause the
         first PCH burst to get de-sensed. */
      if ( gl1_hw_get_sglte_mode(gas_id) )
      {
         /* get gas_id of the other G */
         if ( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
         {
            the_other_gas_id = GERAN_ACCESS_STRATUM_ID_2;
         }
         else
         {
            the_other_gas_id = GERAN_ACCESS_STRATUM_ID_1;
         }
         trm_status = grm_get_trm_status(l1_tsk_buffer[the_other_gas_id].client_id, the_other_gas_id);
         /* Don't know how to handle triple G yet. ???*/
         if ( ( GL1_TRM_GRANTED == trm_status ) ||
              ( GL1_TRM_RETAINED_FOR_ACCESS == trm_status) )
         {
            /* The other G has RF chain. It means it is either in TX or RX. Should apply G+G policy */
            mode = GARB_MODE_GPG_DSDA;
         }
         /* else we keep the XPG mode which is set in above correctly */
      }
#endif /* FEATURE_SGLTE */
      break;
#ifdef FEATURE_TRIPLE_SIM
   case SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY:
      mode = GARB_MODE_TSTS;
      break;
#endif /* FEATURE_TRIPLE_SIM */
#ifdef FEATURE_SGLTE
   case SYS_MODEM_DEVICE_MODE_SINGLE_SIM:
	  /* The CXM peer TECH is expected to contain at most ONE tech. IRAT target techs are been filtered out */
      switch(arbitrated_gas[gas_id].cxm_peer_tech)
      {
      case (1<<CXM_TECH_ONEX): /* C+G DSDA */
      case (1<<CXM_TECH_HDR): /* C+G DSDA */
         mode = GARB_MODE_CPG_DSDA;
         break;
      case (1<<CXM_TECH_WCDMA):     /* W is my DSDA peer */
         mode = GARB_MODE_WPG_DSDA;
         break;
      case (1<<CXM_TECH_TDSCDMA):   /* T is my DSDA peer */
         mode = GARB_MODE_TPG_DSDA;
         break;
      case (1<<CXM_TECH_LTE):       /* L is my DSDA peer */
         mode = GARB_MODE_LPG_DSDA;
         break;
      default:
         mode = garb_is_g_plus_g() ? GARB_MODE_GPG_DSDA : GARB_MODE_XPG_DSDA;
         break;
      }
	  break;
#endif /* FEATURE_SGLTE */
   default:
      break;
   }

   return mode;
}


#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION      garb_get_channel_type

DESCRIPTION   This function maps the channel type reported by the message
              layer (gl1_hw_channel_type) to the arbitrator's internal
              representation of channel type (garb_channel_t).

RETURN VALUE  garb_channel_t

===========================================================================*/
garb_channel_t garb_get_channel_type(gl1_hw_channel_type gl1_hw_channel, garb_rxtx_t rxtx, gas_id_t gas_id)
{
   garb_channel_t channel_type = garb_map_gl1_hw_channel_type_lut[gl1_hw_channel];

   /* For Early TCH assignment - depriotise Filler FACCH TX , Keep FACCH Rx high */
   if( (rxtx == GARB_TX) &&
       (channel_type == GARB_CHAN_ULDL_FACCH) &&
       (arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME].tx.slots[0].channel_type == GARB_CHAN_ULDL_TCH)
      )
   {
     MSG_GERAN_MED_0_G("GARB : Filler FACCH TX : hw_channel_type FACCH garb_chan_type TCH");
     channel_type = GARB_CHAN_ULDL_TCH;
   }

   if((arbitrated_gas[gas_id].state==GARB_CS) || (arbitrated_gas[gas_id].state==GARB_CS_PS))
   {
     /* For FR-FACCH the message layer only identifies the first 4 bursts of
      * the FACCH block as being of channel type FACCH, however the FACCH is
      * interleaved over 8 (or 9 in the case of the middle FACCH block)
      * frames. Similar problem arises for HR-FACCH.
      *
      * This is where we override the channel type indicated by the message
      * layer as the GL1 arbitrator knows best and can correctly indicate
      * that these bursts contain FACCH data that needs to be protected if
      * possible.
      *
      */
      if( (rxtx==GARB_TX) && (arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME].tx.slots[0].channel_type==GARB_CHAN_ULDL_FACCH) )
      {
         channel_type = GARB_CHAN_ULDL_FACCH;
      }
   }

   return channel_type;
}

/*===========================================================================

FUNCTION      garb_get_rx_channel_type

DESCRIPTION   Returns the channel type. The channel type as supplied in the
              arbitrated frame data may be overridden here if a stealing
              channel block Tx is in progress e.g. TCH may be replaced by
              FACCH or RATSCCH.

RETURN VALUE  channel type

===========================================================================*/
garb_channel_t garb_get_rx_channel_type(uint32 fn, uint8 ts, gas_id_t gas_id)
{
   uint32 FnMod52 = fn % 52;

   garb_channel_t channel_type = GARB_CHAN_ULDL_PDTCH;

   if((FnMod52==11) || (FnMod52==37)) /* FnMod51 - 11 and 37 are PTCCH frames */
   {
      channel_type = GARB_CHAN_ULDL_PTCCH;
   }

   return channel_type;
}

/*===========================================================================

FUNCTION      garb_get_tx_channel_type

DESCRIPTION   Returns the channel type associated with the specified frame
              and timeslot.

RETURN VALUE  channel type

===========================================================================*/
garb_channel_t garb_get_tx_channel_type(uint32 fn, uint8 slammed_ts, gas_id_t gas_id)
{
   garb_channel_t channel_type = GARB_CHAN_UNKNOWN;

   if((arbitrated_gas[gas_id].state == GARB_PS) || (arbitrated_gas[gas_id].state == GARB_CS_PS))
   {
      channel_type = GARB_CHAN_ULDL_PDTCH;

      if( (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn != GL1_DEFS_INVALID_FN) && 
          (slammed_ts == arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[slammed_ts]) ) /* RRBP */
      {
         if( ( IS_FRAME_NUM_LATER(fn, SUB_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) )&&
             (!IS_FRAME_NUM_LATER(fn, ADD_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) ) )
         {
            channel_type = ( (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_msg[slammed_ts] == PDAN_MSG ) || 
                             (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_msg[slammed_ts] == PMR_MSG) ) ? 
                             GARB_CHAN_UL_RRBP_LO : GARB_CHAN_UL_RRBP_HI;
            MSG_GERAN_MED_2_G("GARB : RRBP - returning RRBP channel type at slammed_ts %d fn=%d", slammed_ts,fn);
         }
      }
   }

   return channel_type;
}


/*===========================================================================

FUNCTION      garb_is_access_burst

DESCRIPTION   This function indicates if the specified slot is an access
              burst. This is used to determing if a timing advance should
              be applied.

RETURN VALUE  TRUE if the specified burst is an access burst

===========================================================================*/
boolean garb_is_access_burst(garb_channel_t channel, uint32 fn, uint8 slot, gas_id_t gas_id)
{
   boolean is_access_burst = FALSE;

   switch(channel)
   {
      case GARB_CHAN_UL_RRBP_HI:
         if(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_burst[slot]!=NB)  // slot is slammed tn
         {
            is_access_burst = TRUE;
         }
         break;

      case GARB_CHAN_UL_RACH:
      case GARB_CHAN_ULDL_PTCCH:
         {
            is_access_burst = TRUE;
         }
         break;

      default:
         {
            is_access_burst = FALSE;
         }
         break;
   }

   return is_access_burst;
}

/*===========================================================================

FUNCTION      garb_get_slot_s_time

DESCRIPTION   This function gets the USTMR start time of the specified slot,
              adjusted to account for Guard Time, RF Tuning Time and Search
              Width and Timing Advance.

RETURN VALUE  USTMR

===========================================================================*/
uint32 garb_get_slot_s_time(garb_frame_t* frame, garb_rxtx_t rxtx, uint8 slot, gas_id_t gas_id)
{
   uint32 s_time = 0, offset = 0, advance = 0, alpha = 0 ,extra_guard =0;

   /* Guard Period + RF Tune Time (first Rx slot only) + Search Width */
   if(rxtx==GARB_RX)
   {
      alpha   = gl1_msg_get_multi_sim_standby_mode() ? USTMR_FROM_QS(gl1_hw_get_rf_gsm_rx_setup_time(gas_id)) : 0 ;

      s_time  = USTMR_ADD(frame->UstmrRefStart, (slot+0)*USTMR_PER_SLOT);

      /*In G(IDLE)+G(DATA) case , GFW is adding 100 qs Extra offset for scheduling PCH when it postpones RXLM configuration
       Same needs to be considered at Gl1.*/
      if((arbitrated_gas[gas_id].state == GARB_IDLE)&&(OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))&&
         (l1_fm_get_running_activity(gas_id) == FM_PRI_CCCH))
      {
         extra_guard = USTMR_FROM_QS(EXTRA_GUARD_RXLM_PROG);
      }
      
      offset  = USTMR_SLOT_GUARD_PERIOD + ( ( (slot==0) || (frame->rx.slots[slot-1].configured == FALSE) ) ? alpha : 0 ) + frame->rx.search_width;


      s_time  = USTMR_SUB(s_time, offset+ extra_guard);
   }
   else
   /* Guard Period + RF Tune Time (first Tx slot only) - Timing Advance */
   if(rxtx==GARB_TX)
   {
      /* Gl1 is using extended Alpha for TX at TX scheduling. Same should be considered at Gl1 for CXM timeline calculations for TX*/
      if(gl1_msg_get_multi_sim_standby_mode())
      {
#ifdef FEATURE_VSWR
         if(gl_hw_use_extended_tx_alpha [gas_id]) 
         {
             alpha = USTMR_FROM_QS(gl1_hw_get_rf_gsm_tx_vswr_setup_time(gas_id));
         }
         else
         {  
             alpha = USTMR_FROM_QS(gl1_hw_get_rf_gsm_tx_setup_time(gas_id));
         }
#else
         alpha = USTMR_FROM_QS(gl1_hw_get_rf_gsm_tx_setup_time(gas_id));
#endif
     }

      /*There is always 155 qs differnce between Gl1 offset and GFW TX start offset. So CXM conflicts are not seen in some cases.*/
      extra_guard = USTMR_FROM_QS(EXTRA_GUARD_TXLM_PROG);

      s_time  = USTMR_ADD(frame->UstmrRefStart, (slot+3)*USTMR_PER_SLOT);

      offset  = USTMR_SLOT_GUARD_PERIOD + ( ( (slot==0) || (frame->tx.slots[slot-1].configured == FALSE) ) ? alpha : 0 );

      advance = garb_is_access_burst(frame->tx.slots[slot].channel_type, frame->fn, slot, gas_id) ? 0 : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;

      s_time  = USTMR_SUB(s_time, offset + advance + extra_guard );
   }

   return s_time;
}


/*===========================================================================

FUNCTION      garb_get_slot_e_time

DESCRIPTION   This function gets the USTMR start time of the specified slot,
              adjusted to account for Guard Time, RF Tuning Time and Search
              Width and Timing Advance.

RETURN VALUE  USTMR

===========================================================================*/
uint32 garb_get_slot_e_time(garb_frame_t* frame, garb_rxtx_t rxtx, uint8 slot, gas_id_t gas_id)
{
   uint32 e_time = 0, offset = 0, advance = 0;

   /* Guard Period + Search Width */
   if(rxtx==GARB_RX)
   {
      e_time  = USTMR_ADD(frame->UstmrRefStart, (slot+1)*USTMR_PER_SLOT);

     /*Extra guard period in GSM symbols required at the end of SYNC_RX & ASYNC_RX bursts in DSDS. Required for RFFE bus contention*/
      offset  = USTMR_SLOT_GUARD_PERIOD + frame->rx.search_width +

     ((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) ? USTMR_FROM_QS(GL1_FW_EXTENDED_GUARD_PERIOD * 4) : 0);

      e_time  = USTMR_ADD(e_time, offset);
   }
   else
   /* Guard Period - Timing Advance */
   if(rxtx==GARB_TX)
   {
      e_time  = USTMR_ADD(frame->UstmrRefStart, (slot+4)*USTMR_PER_SLOT);

      advance = garb_is_access_burst(frame->tx.slots[slot].channel_type, frame->fn, slot, gas_id) ? 0 : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;

      e_time  = USTMR_ADD( USTMR_SUB(e_time, advance),  USTMR_SLOT_GUARD_PERIOD);
   }

   return e_time;
}


/*===========================================================================

FUNCTION      garb_get_vbatt_tx_power_backoffs

DESCRIPTION   This function takes the Tx power (dBm) of the P1 (high tier)
              and P2 (low tier) SUBs along with a total Tx power budget. If
              the total Tx power is in excess of the specified budget the
              budgeted power will be distributed between the two SUBs
              accoring to priority.

              This function passes back the adjusted Tx power (dBm) and
              backoff (dB) required to mitigate VBATT droop.

RETURN VALUE  TRUE is power backoff should be applied

===========================================================================*/
boolean garb_get_vbatt_tx_power_backoffs(
   double   TxP_SUB1_dBm,
   double   TxP_SUB2_dBm,
   uint8    pri_SUB1,
   uint8    pri_SUB2,
   double*  TxP_SUB1_backoff_dB,
   double*  TxP_SUB2_backoff_dB,
   gas_id_t gas_id)
{
   uint8    TxP_delta_dB;
   double   TxP_total_dBm;
   double   TxP1_dBm;
   double   TxP2_dBm;
   double   TxP1_mod_dBm;
   double   TxP2_mod_dBm;
   double   TxP1_backoff_dB = 0.0;
   double   TxP2_backoff_dB = 0.0;

   double   TxP1_final_dBm = TxP_SUB1_dBm;
   double   TxP2_final_dBm = TxP_SUB2_dBm;

   *TxP_SUB1_backoff_dB = 0.0;
   *TxP_SUB2_backoff_dB = 0.0;

   /* Calculate the total Tx power in dBm (TxP_delta_dB) */
   if(TxP_SUB1_dBm > TxP_SUB2_dBm)
   {
      TxP_delta_dB  = (uint8)(TxP_SUB1_dBm - TxP_SUB2_dBm);
      TxP_total_dBm = TxP_SUB1_dBm + (((uint8)TxP_delta_dB < LUTX_MAX_IDX) ? LUTx[TxP_delta_dB] : LUTX_MIN_STEP_SIZE);
   }
   else
   {
      TxP_delta_dB  = (uint8)(TxP_SUB2_dBm - TxP_SUB1_dBm);
      TxP_total_dBm = TxP_SUB2_dBm + (((uint8)TxP_delta_dB < LUTX_MAX_IDX) ? LUTx[TxP_delta_dB] : LUTX_MIN_STEP_SIZE);
   }

   if(VBATT_TXP_BUDGET_DBM > TxP_total_dBm)
   {
      /* Total Tx power in dBm (TxP_total_dBm) is within budget - no backoff is required */
      return FALSE;
   }

   if(pri_SUB1 == pri_SUB2)
   {
      /* Tx's are of equal priority - back off both by (TxP_total_dBm - TxP_Budget_dBm) dB */
      *TxP_SUB1_backoff_dB = TxP_total_dBm - VBATT_TXP_BUDGET_DBM;
      *TxP_SUB2_backoff_dB = TxP_total_dBm - VBATT_TXP_BUDGET_DBM;
      return TRUE;
   }
   else
   if(pri_SUB1 > pri_SUB2)
   {
      /* Map TxP1 (high priority) to SUB1 */
      TxP1_dBm = TxP_SUB1_dBm;
      TxP2_dBm = TxP_SUB2_dBm;
   }
   else
   {
      /* Map TxP1 (high priority) to SUB2 */
      TxP1_dBm = TxP_SUB2_dBm;
      TxP2_dBm = TxP_SUB1_dBm;
   }

   /* Each of the Tx powers are capped to a max of TxP_Budget_dBm */
   TxP1_mod_dBm = (TxP1_dBm > VBATT_TXP_BUDGET_DBM) ? VBATT_TXP_BUDGET_DBM : TxP1_dBm;
   TxP2_mod_dBm = (TxP2_dBm > VBATT_TXP_BUDGET_DBM) ? VBATT_TXP_BUDGET_DBM : TxP2_dBm;

   /* If the Tx powers are not within 8dB of each other the highest Tx power is backed off by 0.6dB */
   if( (TxP1_mod_dBm > TxP2_mod_dBm) && ( (TxP1_mod_dBm - TxP2_mod_dBm) > 8 ) )
   {
      TxP1_backoff_dB = 0.6;
      TxP2_backoff_dB = 0.0;
   }
   else
   if( (TxP2_mod_dBm > TxP1_mod_dBm) && ( (TxP2_mod_dBm - TxP1_mod_dBm) > 8 ) )
   {
      TxP1_backoff_dB = 0.0;
      TxP2_backoff_dB = 0.6;
   }
   else
   if((TxP1_mod_dBm>=25) && (TxP2_mod_dBm>=25))
   {
      /* If the Tx powers are within 8dB of each other then the budgeted Tx power is distributed according to priority */
      TxP1_backoff_dB = LUTy[p1p2_dbm_to_backoff_index[(uint8)TxP1_mod_dBm-25][(uint8)TxP2_mod_dBm-25]][LUTY_BACKOFF_1];
      TxP2_backoff_dB = LUTy[p1p2_dbm_to_backoff_index[(uint8)TxP1_mod_dBm-25][(uint8)TxP2_mod_dBm-25]][LUTY_BACKOFF_2];
   }
   else /* Shouldn't ever hit this! */
   {
      MSG_GERAN_ERROR_2_G("GARB : VBATT : Failed to find VBATT power distribution for TxP1=%d TxP2=%d", TxP1_mod_dBm, TxP2_mod_dBm);
   }

   /* Map the priority based backoffs back to the correct SUB */
   if(pri_SUB1 > pri_SUB2)
   {
       TxP1_final_dBm = VBATT_TXP_BUDGET_DBM - TxP1_backoff_dB;
       TxP2_final_dBm = VBATT_TXP_BUDGET_DBM - TxP2_backoff_dB;
   }
   else
   {
       TxP1_final_dBm = VBATT_TXP_BUDGET_DBM - TxP2_backoff_dB;
       TxP2_final_dBm = VBATT_TXP_BUDGET_DBM - TxP1_backoff_dB;
   }

  *TxP_SUB1_backoff_dB = TxP_SUB1_dBm - TxP1_final_dBm;
  *TxP_SUB2_backoff_dB = TxP_SUB2_dBm - TxP2_final_dBm;

   if(*TxP_SUB1_backoff_dB < 0){*TxP_SUB1_backoff_dB=0;}
   if(*TxP_SUB2_backoff_dB < 0){*TxP_SUB2_backoff_dB=0;}

   MSG_GERAN_LOW_2_G("GARB : VBATT : Requested : This SUB=%d dBm100 Peer SUB=%d dBm100\n", (uint32)(TxP_SUB1_dBm*100), (uint32)(TxP_SUB2_dBm*100) );
   MSG_GERAN_LOW_2_G("GARB : VBATT : Backoffs  : This SUB=%d dB100 Peer SUB=%d dB100\n", (uint32)(*TxP_SUB1_backoff_dB*100), (uint32)((*TxP_SUB2_backoff_dB)*100) );
   MSG_GERAN_LOW_2_G("GARB : VBATT : Final     : This SUB=%d dBm100 Peer SUB=%d dBm100\n", (uint32)(TxP1_final_dBm*100), (uint32)(TxP2_final_dBm*100) );

   return TRUE;
}

#if defined (FEATURE_GSM_COEX_BLANK_CS_UL) || defined(FEATURE_GSM_COEX_BLANK_CS_DL)
/*===========================================================================

FUNCTION      garb_is_xpg_blanked_burst

DESCRIPTION   This function determines if the specified burst should be
              voluntarily blanked according to the appropriate X+G DSDA
              policy.

RETURN VALUE  TRUE if the specified burst should be blanked

===========================================================================*/
static boolean garb_is_xpg_blanked_burst(uint32 fn, garb_rxtx_t rxtx, gas_id_t gas_id )
{
#if defined (FEATURE_GSM_COEX_BLANK_CS_UL) || defined(FEATURE_GSM_COEX_BLANK_CS_DL)
   boolean dl_blanking_enabled = !(gl1_hw_efs_get_debug(gas_id) & GL1_EFS_DEBUG_COEX_MODE_B0); /* ENABLED BY DEFAULT */
   boolean ul_blanking_enabled =  (gl1_hw_efs_get_debug(gas_id) & GL1_EFS_DEBUG_COEX_MODE_B1); /* DISABLED BY DEFAULT */
   boolean mode = (gl1_hw_efs_get_debug(gas_id) & GL1_EFS_DEBUG_COEX_MODE_B2); /* 0 = FINAL 1 = RANDOM */
   uint8 FnMod13 = frame_counters[gas_id].FNmod13;
   static uint8 burst[NUM_GERAN_DATA_SPACES];

   if( ( (FnMod13==12) || (FnMod13==3) || (FnMod13==7) ) && (rxtx==GARB_RX) ) /* select burst once per FR-TCH block */
   {
      burst[gas_id] = (mode) ? (rand() % 4) /* RANDOM */ : 3 /* FINAL */ ;
   }
#endif /* (FEATURE_GSM_COEX_BLANK_CS_UL) || defined(FEATURE_GSM_COEX_BLANK_CS_DL) */

#ifdef FEATURE_GSM_COEX_BLANK_CS_DL
   if( (rxtx==GARB_RX) && (dl_blanking_enabled) )
   {
      return (gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id) == burst[gas_id]);
   }
   else
#endif /* FEATURE_GSM_COEX_BLANK_CS_DL */
#ifdef FEATURE_GSM_COEX_BLANK_CS_UL
   if( (rxtx==GARB_TX) && (ul_blanking_enabled) )
   {
      return (gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id) == burst[gas_id]);
   }
   else
#endif /* FEATURE_GSM_COEX_BLANK_CS_UL */
   {
      return FALSE;
   }
}
#endif /* #if defined (FEATURE_GSM_COEX_BLANK_CS_UL) || defined(FEATURE_GSM_COEX_BLANK_CS_DL) */

/*===========================================================================

FUNCTION      garb_get_cxm_priority_tier

DESCRIPTION   This function determines the CXM priority tier for a specified
              slot based on the channel type and Multi-RAT mode.

              For G+G modes the CXM priority is left undecided as this will
              later be determined by G+G conflict detection.

RETURN VALUE  Multi SIM Mode

===========================================================================*/
cxm_activity_type garb_get_cxm_priority_tier(garb_rxtx_t rxtx, garb_channel_t channel_type, uint8 ts, gas_id_t gas_id)
{
   cxm_activity_type tier = ACTIVITY_TIER_20;
   uint32            fn   = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn;

   if ( (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
#ifdef FEATURE_SGLTE
         &&
        (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif /* FEATURE_SGLTE */
       )
   {
     tier = garb_map_channel_to_cxm_activity_gpg_lut[channel_type];
     return tier;
   }

   if(rxtx==GARB_RX)
   {
      uint8 max_protected_slots_per_frame = max_dl_tier_10_per_frame[arbitrated_gas[gas_id].multi_sim_mode] - 1;

      switch(arbitrated_gas[gas_id].multi_sim_mode)
      {
      /************************
      *  G PLUS G DSDS
      *  G PLUS G DSDA
      *  TSTS
      *************************/
      case GARB_MODE_GPG_DSDS:
      case GARB_MODE_GPG_DSDA:
#ifdef FEATURE_TRIPLE_SIM
      case GARB_MODE_TSTS:
#endif /* FEATURE_TRIPLE_SIM */
         tier = garb_map_channel_to_cxm_activity_gpg_lut[channel_type];
         break;
      /************************
      *  X PLUS G DSDS
      *************************/
      case GARB_MODE_XPG_DSDS:
         tier = garb_map_channel_to_cxm_activity_xpg_lut[channel_type];
         break;

      /************************
      *  X PLUS G DSDA
      *  C PLUS G DSDA
      *  W PLUS G DSDA
      *  L PLUS G DSDA
      ************************/
      case GARB_MODE_XPG_DSDA:
      case GARB_MODE_CPG_DSDA:
      case GARB_MODE_WPG_DSDA:
      case GARB_MODE_LPG_DSDA:
         tier = (ts < max_protected_slots_per_frame ) ? garb_map_channel_to_cxm_activity_xpg_lut[channel_type] : ACTIVITY_TIER_20;

#ifdef FEATURE_GSM_COEX_BLANK_CS_DL
         /* Final burst blanking for FR TCH */
         if( ((arbitrated_gas[gas_id].state == GARB_CS) ||(arbitrated_gas[gas_id].state == GARB_CS_PS)) &&
             (arbitrated_gas[gas_id].params_cs.is_fr == TRUE ) &&
             (channel_type == GARB_CHAN_ULDL_TCH) && garb_is_xpg_blanked_burst(fn, rxtx, gas_id) )
         {
            tier = ACTIVITY_TIER_20;
         }
#endif /* FEATURE_GSM_COEX_BLANK_CS_DL */

         break;

      /*  T PLUS G DSDA */
      case GARB_MODE_TPG_DSDA:
         tier = (ts < max_protected_slots_per_frame ) ? garb_map_channel_to_cxm_activity_xpg_lut[channel_type] : ACTIVITY_TIER_25;
         break;
      /************************
      * DKDC (Don't Know, Don't Care)
      *************************/
      case GARB_MODE_INVALID:
         tier = ACTIVITY_TIER_20;
         break;
      }

      if((tier == ACTIVITY_TIER_10) || (tier == ACTIVITY_TIER_5)) {arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.num_protected_slots++;}
   }
   else
   if(rxtx==GARB_TX)
   {
      uint8 max_protected_slots_per_frame = max_ul_tier_10_per_frame[arbitrated_gas[gas_id].multi_sim_mode];
      uint8 num_protected_slots_per_frame = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.num_protected_slots;

      switch(arbitrated_gas[gas_id].multi_sim_mode)
      {
      /************************
      *  G PLUS G DSDS
      *  G PLUS G DSDA
      *  TSTS
      *************************/
      case GARB_MODE_GPG_DSDS:
      case GARB_MODE_GPG_DSDA:
#ifdef FEATURE_TRIPLE_SIM
      case GARB_MODE_TSTS:
#endif /* FEATURE_TRIPLE_SIM */
         tier = garb_map_channel_to_cxm_activity_gpg_lut[channel_type];
         break;

      /************************
      *  X PLUS G DSDS
      *************************/
      case GARB_MODE_XPG_DSDS:
         tier = garb_map_channel_to_cxm_activity_xpg_lut[channel_type];
         break;

      /************************
      *  X PLUS G DSDA
      *  C PLUS G DSDA
      *  W PLUS G DSDA
      *  L PLUS G DSDA
      ************************/
      case GARB_MODE_XPG_DSDA:
      case GARB_MODE_CPG_DSDA:
      case GARB_MODE_WPG_DSDA:
      case GARB_MODE_LPG_DSDA:
      case GARB_MODE_TPG_DSDA: 
         tier = (num_protected_slots_per_frame < max_protected_slots_per_frame) ? garb_map_channel_to_cxm_activity_xpg_lut[channel_type] : ACTIVITY_TIER_20;

#ifdef FEATURE_GSM_COEX_BLANK_CS_UL
         /* Final burst blanking for FR TCH */
         if( ((arbitrated_gas[gas_id].state == GARB_CS ) || (arbitrated_gas[gas_id].state==GARB_CS_PS)) &&
             (arbitrated_gas[gas_id].params_cs.is_fr == TRUE ) &&
             (channel_type == GARB_CHAN_ULDL_TCH) && garb_is_xpg_blanked_burst(fn, rxtx, gas_id) )
         {
            tier = ACTIVITY_TIER_20;
         }
#endif /* FEATURE_GSM_COEX_BLANK_CS_UL */

         break;

      /************************
      * DKDC (Don't Know, Don't Care)
      ************************/

      case GARB_MODE_INVALID:
         tier = ACTIVITY_TIER_20;
         break;
      }

      if((tier == ACTIVITY_TIER_10) || (tier == ACTIVITY_TIER_5)) {arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.num_protected_slots++;}

   }

   return tier;
}

/*===========================================================================

FUNCTION      garb_is_sdcch_saach_frame

DESCRIPTION   Returns TRUE if the FN is a SDCCH or SACCH frame. This
              function also updates the SDCCH parameters with information
              about the current block type (SDCCH Rx, SDCCH Tx, SACCH Rx or
              SACCH Tx).

RETURN VALUE  TRUE if the FN is a SDCCH or SACCH frame

===========================================================================*/
boolean garb_is_sdcch_saach_frame(uint32 fn, uint8 offset, gas_id_t gas_id)
{
   boolean rc = FALSE;

   /* The command for SDCCH4 / SDCCH8 is only checked on block boundaries */
   if(arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining==0)
   {
      uint8 index, command, subchannel = arbitrated_gas[gas_id].params_sdcch.subchannel;

      if(arbitrated_gas[gas_id].params_sdcch.is_sdcch_8)
      {
         /* SDCCH 8*/
         index   = ( (SUB_FN(fn, 1) % 102) + (((7 - subchannel) % 4) << 2)) % 102;
         command = ded_sdcch_8_table[index][subchannel >> 2][0];
      }
      else
      {
         /* SDCCH 4*/
         index   = (SUB_FN(fn, 1) % 102);
         command = ded_sdcch_4_table[index][subchannel][0];
      }

      /* The SDCCH parameters are updated here to reflect the current block type */
      switch(command)
      {
      case L1_DED_READ_SDCCH: /* SDCCH RX */
         arbitrated_gas[gas_id].params_sdcch.channels[offset].dl_channel = GARB_CHAN_ULDL_SDCCH;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].ul_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining = 3;
         rc = TRUE;
         break;
      case L1_DED_SEND_SDCCH: /* SDCCH TX */
         arbitrated_gas[gas_id].params_sdcch.channels[offset].ul_channel = GARB_CHAN_ULDL_SDCCH;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].dl_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining = 3;
         rc = TRUE;
         break;
      case L1_DED_READ_SACCH: /* SACCH RX */
         arbitrated_gas[gas_id].params_sdcch.channels[offset].dl_channel = GARB_CHAN_ULDL_SACCH;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].ul_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining = 3;
         rc = TRUE;
         break;
      case L1_DED_SEND_SACCH: /* SDCCH TX */
         arbitrated_gas[gas_id].params_sdcch.channels[offset].ul_channel = GARB_CHAN_ULDL_SACCH;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].dl_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining = 3;
         rc = TRUE;
         break;
      default: /* Not interested ! */
         arbitrated_gas[gas_id].params_sdcch.channels[offset].dl_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].ul_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining = 0;
         break;
      }
   }
   /* If we hit this else clause, we're in the middle of a SACCH block */
   else
   {
      /* This tracks how many frames we have remaining in the current SDCCH or SACCH block */
      arbitrated_gas[gas_id].params_sdcch.channels[offset].frames_remaining--;
      rc = TRUE;
   }

   return rc;
}


/*===========================================================================

FUNCTION      garb_is_tch_frame

DESCRIPTION   Returns TRUE if the FN is a TCH, SACCH or FACCH frame. This
              function also updates the CS parameters to indicate with
              information about the current block type (TCH, SACCH or FACCH)

RETURN VALUE  TRUE if the FN is an TCH, SACCH or FACCH frame

===========================================================================*/
boolean garb_is_tch_frame(uint32 fn, uint8 offset, boolean dtx, gas_id_t gas_id)
{
   boolean rc       = FALSE;
   uint32  FnMod104 = fn % 104, FnMod26 = fn % 26, FnMod13 = fn % 13;

   ASSERT( offset < GARB_TIMELINE_MAX_FRAMES );

   /* The number of frames of the Tx block yet to run through the N+1 frame of the look-ahead is 1 less than that yet to run through frame N */
   if( (offset > 1) && (arbitrated_gas[gas_id].params_cs.channels[offset-1].frames_remaining > 1 ) )
   {
      arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = arbitrated_gas[gas_id].params_cs.channels[offset-1].frames_remaining;
   }

   /* If there is no active block Tx then check for a new block Tx comming up on this frame */
   if (arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining==0)
   {
      if( (arbitrated_gas[gas_id].params_cs.is_fr==TRUE ) && L1_CCH_message_pending( FACCH_F, gas_id ) &&! dtx )
      {
         /* This is FR and there's a L2 message pending - check if this is a FACCH boundary */
         switch(FnMod13)
         {
         case  3: /* FR FACCH Block 0 (UL+DL) */
         case 12: /* FR FACCH Block 2 (UL+DL) */
            arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_FACCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = 7;
            rc = TRUE;
            break;
         case  7: /* FR FACCH Block 1 (UL+DL)*/
            arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_FACCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = 8; /* +1 frame due to SACCH overlap */
            rc = TRUE;
            break;
         default: /* This is a not a FACCH frame */
            arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = 0;
            rc = TRUE;
            break;
         }
      }
      else
      if( (arbitrated_gas[gas_id].params_cs.is_fr==FALSE) && L1_CCH_message_pending( FACCH_H, gas_id ) && !dtx )
      {
         /* This is HR and there's a L2 message pending - check if this is a FACCH boundary */
         switch ( ( FnMod26 - arbitrated_gas[gas_id].params_cs.subchannel + 26 ) % 26 )
         {
         case  7: /* HR FACCH Block 0 (UL only) */
         case 16: /* HR FACCH Block 1 (UL only) */
         case 25: /* HR FACCH Block 2 (UL only) */
            arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_FACCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
            arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = 5;
            rc = TRUE;
            break;
         /* GARB TODO - Might have to do something to identify candidate HR FACCH DL blocks */
         default: /* This is not a FACCH frame */
            arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining = 0;
            rc = TRUE;
            break;
         }
      }
      else
      if(FnMod26==arbitrated_gas[gas_id].params_cs.sacch_frame)
      {
         /* This is a SACCH frame */
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_SACCH;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_SACCH;
         rc = TRUE;
      }
      else
      if(FnMod26==arbitrated_gas[gas_id].params_cs.idle_frame)
      {
         /* This is an IDLE frame */
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_UNKNOWN;
         rc = FALSE;
      }
      else
      if( (dtx && ( arbitrated_gas[gas_id].params_cs.is_fr) && !IS_FR_DTX_FRAME(FnMod104)) ||
          (dtx && (!arbitrated_gas[gas_id].params_cs.is_fr) && !IS_HR_DTX_FRAME(FnMod104, arbitrated_gas[gas_id].params_cs.subchannel)) )
      {
         /* This is a DTX frame */
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_UNKNOWN;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
         rc = TRUE;
      }
      else
      if( (arbitrated_gas[gas_id].params_cs.is_fr) || (arbitrated_gas[gas_id].params_cs.subchannel == (fn % 2)) )
      {
         /* This is a TCH frame */
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_TCH;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
         rc = TRUE;
      }
   }
   /* If we hit this else clause, we're in the middle of a block Tx (e.g. FACCH or RATSCCH) */
   else
   {
      /* Need to check here is this is a SACCH as FACCH blocks can overlap SACCH frames */
      if(FnMod26==arbitrated_gas[gas_id].params_cs.sacch_frame)
      {
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_SACCH;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_SACCH;
      }
      else
      {
         arbitrated_gas[gas_id].params_cs.channels[offset].ul_channel = GARB_CHAN_ULDL_FACCH;
         arbitrated_gas[gas_id].params_cs.channels[offset].dl_channel = GARB_CHAN_ULDL_TCH;
      }

      /* This tracks how many frames we have remaining in the current FACCH (or..comming soon, RATSCCH) block */
      arbitrated_gas[gas_id].params_cs.channels[offset].frames_remaining--;
      rc = TRUE;
   }

   return rc;
}


/*===========================================================================

FUNCTION      garb_is_rach_frame

DESCRIPTION   Decrement the rach delay if rach is next

RETURN VALUE  TRUE if RACH is next

===========================================================================*/
boolean garb_is_rach_frame( uint32 fn, gas_id_t gas_id )
{
  boolean rc = FALSE;
  uint16 FnMod51 = fn % 51;

  if ( ( arbitrated_gas[gas_id].params_access.rach_delay > 0 )
        &&( L1_is_rach_next( FnMod51, gas_id ) )
     )
  {
    arbitrated_gas[gas_id].params_access.rach_delay--;

    if( arbitrated_gas[gas_id].params_access.rach_delay == 0 )
    {
      rc = TRUE;
    }
  }
  return rc;
}

/*===========================================================================

FUNCTION      garb_is_rrbp_block_boundary

DESCRIPTION   Returns TRUE if the FN is in a candidate RRBP block

RETURN VALUE  TRUE if the FN is in a candidate RRBP block

===========================================================================*/
boolean garb_is_rrbp_block_boundary(uint32 fn, gas_id_t gas_id)
{
   uint32 FNMod13 = fn % 13;

   return ( (FNMod13==3) || (FNMod13==7) || (FNMod13==11) );
}

/*===========================================================================

FUNCTION      garb_get_next_rrbp_fn

DESCRIPTION   Returns the FN of the next RRBP and updates the PS parameters
              with the RRBP message type, burst type and slammed TN.

RETURN VALUE  FN of the next RRBP

===========================================================================*/
uint32 garb_get_next_rrbp_fn(gas_id_t gas_id)
{
   uint32 rrbp_frame_num = GPL1_INVALID_FN;

   memset(&arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[0], 0xFF, ARR_SIZE(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts));

   rrbp_frame_num = gpl1_gprs_get_lowest_rrbp_msg_FN( &arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_msg[0],
                                                      &arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_burst[0],
                                                      &arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[0], gas_id);

  return rrbp_frame_num;
}

/*===========================================================================

FUNCTION      garb_get_mon_s_time

DESCRIPTION   Returns Monitor s_time.  ( Same for DSDS/DSDA/SS )

RETURN VALUE  Returns Monitor s_time.

===========================================================================*/
uint32 garb_get_mon_s_time( garb_frame_t* frame,gas_id_t gas_id)
{
   int32 qs_position = QS_PER_SLOT-1; // CS/SDCCH

   if(arbitrated_gas[gas_id].arbitrated_frame->monitors.count)
   {
     qs_position = arbitrated_gas[gas_id].arbitrated_frame->monitors.monitor[0].qs_position;  // considers timing advance.
   }
   else  // count = 0.
   if (arbitrated_gas[gas_id].state == GARB_PS)
   {
     if (arbitrated_gas[gas_id].params_ps.num_dl_slots) // dl + ul or dl.
     {
       qs_position = (arbitrated_gas[gas_id].params_ps.num_dl_slots * qs_position);
     }
     else if (arbitrated_gas[gas_id].params_ps.num_ul_slots)  // it will have corresponding dl.
     {
       qs_position = (arbitrated_gas[gas_id].params_ps.num_ul_slots * qs_position);
     }
   }
   return (USTMR_ADD(frame->UstmrRefStart, USTMR_FROM_QS(qs_position + gl1_mon_set_up_with_sched_guard_period(gas_id))));
}

/*===========================================================================

FUNCTION      garb_get_mon_e_time

DESCRIPTION   Returns Monitor e_time.  ( Same for DSDS/DSDA/SS )

RETURN VALUE  Returns Monitor e_time.

===========================================================================*/
uint32 garb_get_mon_e_time( garb_frame_t* frame,gas_id_t gas_id)
{
  int32 qs_position = QS_PER_SLOT-1; // CS/SDCCH

  if(arbitrated_gas[gas_id].arbitrated_frame->monitors.count)
  {
    qs_position = arbitrated_gas[gas_id].arbitrated_frame->monitors.monitor[0].qs_position;  // considers timing advance.
  }
  else  // count = 0.
  if (arbitrated_gas[gas_id].state == GARB_PS)
  {
    if (arbitrated_gas[gas_id].params_ps.num_dl_slots) // dl + ul or dl.
    {
      qs_position = (arbitrated_gas[gas_id].params_ps.num_dl_slots * qs_position);
    }
    else if (arbitrated_gas[gas_id].params_ps.num_ul_slots)  // it will have corresponding dl.
    {
      qs_position = (arbitrated_gas[gas_id].params_ps.num_ul_slots * qs_position);
    }
  }

  return USTMR_ADD(frame->UstmrRefStart, USTMR_FROM_QS(qs_position + gl1_hw_pwr_meas_req_time(gas_id)));
}

/*===========================================================================

FUNCTION      garb_update_scell_ncell_arfcns

DESCRIPTION   Returns the allocated SCELL / NCELL ARFCN at FN MOD boundary.

RETURN VALUE  None.

===========================================================================*/
void  garb_update_scell_ncell_arfcns(ARFCN_T  *arfcns,  uint32  num_arfcns,uint8 offset_per_meas_arfcn,gas_id_t gas_id)
{
  uint8 i = 0;
  uint8 start_fn_mod = 0;
  uint32 over_the_air_FN = ADD_FN(gl1_get_FN(gas_id), 1);
  ARFCN_T scell_arfcns[26];

  memset( scell_arfcns,NULL,sizeof(scell_arfcns) );

  switch(arbitrated_gas[gas_id].state)
  {
   case GARB_CS :
     start_fn_mod = over_the_air_FN % 26;
     arbitrated_gas[gas_id].params_cs.num_ncell_arfcn =  num_arfcns/offset_per_meas_arfcn ;

     /* update scell arfcns for the next 12 frames */
     if (arbitrated_gas[gas_id].params_cs.frequency_list.no_of_items > 1) // freq hopping
     {
        arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns(1, 1,  13, &scell_arfcns[0],  gas_id);
        arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns(14,1,  13, &scell_arfcns[13], gas_id);  // remaining 13 arfcs after idle/sacch frame.
     }

     for (i=0;(i<26) && (arbitrated_gas[gas_id].params_cs.frequency_list.no_of_items > 1);i++)
     {
       arbitrated_gas[gas_id].params_cs.scell_arfcn[(start_fn_mod + i)%26] = scell_arfcns[i];  // not used when no_of_items = 1
     }

     for (i=0;i<arbitrated_gas[gas_id].params_cs.num_ncell_arfcn;i++)
     {
       arbitrated_gas[gas_id].params_cs.ncell_arfcn[(start_fn_mod + i) % 26] = arfcns[ i * offset_per_meas_arfcn];
     }

     break;

   case GARB_CS_SDCCH:
     start_fn_mod = over_the_air_FN % 102;
     arbitrated_gas[gas_id].params_sdcch.num_ncell_arfcn = num_arfcns;
     for (i=0;i<num_arfcns;i++)
     {
       arbitrated_gas[gas_id].params_sdcch.ncell_arfcn[(start_fn_mod + i) % 102] = arfcns[i];
     }

     break;

   case GARB_PS :
     start_fn_mod = over_the_air_FN % 13;
     arbitrated_gas[gas_id].params_ps.num_ncell_arfcn = num_arfcns;

     /* update scell arfcns for the next 12 frames */
     if (arbitrated_gas[gas_id].params_ps.frequency_list.no_of_items > 1) // freq hopping
     {
        arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns(1, 1, 13, &scell_arfcns[0], gas_id);
     }

     for (i=0;i<num_arfcns;i++)
     {
       arbitrated_gas[gas_id].params_ps.ncell_arfcn[(start_fn_mod + i)%13] = arfcns[i];
     }
     for (i=0;(i<13) && (arbitrated_gas[gas_id].params_ps.frequency_list.no_of_items > 1);i++)
     {
       arbitrated_gas[gas_id].params_ps.scell_arfcn[(start_fn_mod + i)%13] = scell_arfcns[i];  // not used when no_of_items = 1
     }

     break;

   default :
    break;
  }

}

/*===========================================================================

FUNCTION      garb_get_monscan_arfcn

DESCRIPTION   Returns NCELL ARFCN based pwr mon scheduling.

RETURN VALUE  ARFCN_T.

===========================================================================*/
ARFCN_T garb_get_monscan_arfcn(uint32 FN,gas_id_t gas_id)
{
  uint32 over_the_air_FN = ADD_FN(FN,1);
  ARFCN_T arfcn = {0};

  switch(arbitrated_gas[gas_id].state)
   {
     case GARB_CS :
       arfcn = arbitrated_gas[gas_id].params_cs.ncell_arfcn[over_the_air_FN%26];
       break;

     case GARB_CS_SDCCH:
       arfcn = arbitrated_gas[gas_id].params_sdcch.ncell_arfcn[over_the_air_FN%102];
       break;

     case GARB_PS :
        arfcn = arbitrated_gas[gas_id].params_ps.ncell_arfcn[over_the_air_FN%13];
       break;

     default :
      break;
   }

   return arfcn;
}


/*===========================================================================

FUNCTION      gpl1_get_serving_cell_arfcn

DESCRIPTION   Returns SCELL ARFCN based on Hopping List.

RETURN VALUE  ARFCN_T.

===========================================================================*/
ARFCN_T gpl1_get_serving_cell_next_arfcn(uint32 FN,gas_id_t gas_id)
{
  uint32 over_the_air_FN = ADD_FN(FN,1);
  ARFCN_T arfcns[4];

  memset( arfcns, NULL, sizeof(arfcns) );

  switch (arbitrated_gas[gas_id].state)
  {
    case GARB_CS :
    if (arbitrated_gas[gas_id].params_cs.frequency_list.no_of_items == 1 )
    {
       arfcns[0] = arbitrated_gas[gas_id].params_cs.frequency_list.channel[0];
    }
    else
    {
      arfcns[0] = arbitrated_gas[gas_id].params_cs.scell_arfcn[over_the_air_FN%26];
    }
    break;

    case GARB_CS_SDCCH :
      if (arbitrated_gas[gas_id].params_sdcch.frequency_list.no_of_items == 1 )
      {
         arfcns[0] = arbitrated_gas[gas_id].params_sdcch.frequency_list.channel[0];
      }
      else
      {
        arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns(SUB_FN(over_the_air_FN, GSTMR_GET_FN_GERAN(gas_id)), 1,  4, &arfcns[0], gas_id);
      }
    break;

    case GARB_PS :
      if (arbitrated_gas[gas_id].params_ps.frequency_list.no_of_items == 1 )
      {
        arfcns[0] = arbitrated_gas[gas_id].params_ps.frequency_list.channel[0];
      }
      else
      {
        arfcns[0] = arbitrated_gas[gas_id].params_ps.scell_arfcn[over_the_air_FN%13];
      }
    break;

    default :
      break;
   }
    return arfcns[0];
}

/*===========================================================================

FUNCTION      garb_get_tx_block_duration

DESCRIPTION   This function returns the duration in frames of the specified
              Tx Block (FACCH, RATSCCH etc).

RETURN VALUE  Tx block duration in frames.

===========================================================================*/
uint8 garb_get_tx_block_duration(gl1_msg_chan_type channel_type, uint32 fn, gas_id_t gas_id)
{
   uint8 duration = 0, FnMod13 = fn % 13;

   switch(channel_type)
   {
   case GL1_MSG_FACCH:
   case GL1_MSG_FACCH_AFS:
   case GL1_MSG_FACCH_WFS:
      /* FACCH FR */
      duration = (FnMod13 == 7) ? 9 : 8; /* Middle FACCH block is 9 frames due to SACCH overlap */
      break;
   case GL1_MSG_FACCH_HS:
   case GL1_MSG_FACCH_AHS:
      /* FACCH HR */
      duration = 12;
      break;
   case GL1_MSG_RATSCCH_FS:
   case GL1_MSG_RATSCCH_WFS:
      /* RATSCCH FR */
      duration = 8;
      break;
   case GL1_MSG_RATSCCH_HS:
      /* RATSCCH HR */
      duration = 12;
      break;
   default:
      break;
   }

   return duration;
}

/*===========================================================================

FUNCTION      garb_intf_monscan_query_interband_cells

DESCRIPTION   Func returns if interband cells the present in BA LIST

RETURN VALUE  TRUE/FALSE.

===========================================================================*/
sys_band_T garb_intf_monscan_query_interband_cells(sys_band_T serv_band,gas_id_t gas_id)
{
   sys_band_T interband = INVALID_BAND;
   uint8 cell_index;
   uint8        ba_list_size = arbitrated_gas[gas_id].params_monscan.ba_list_size;
   ARFCN_T      arfcn;
   
   for ( cell_index = 0; cell_index < ba_list_size; cell_index++)
   {
      arfcn = arbitrated_gas[gas_id].params_monscan.ba_list_pptr [cell_index]->bs.arfcn;
      if(arfcn.band != serv_band)
      {
        interband = arfcn.band;
        break;
      }
   }
   return  interband;
}/* garb_intf_monscan_query_interband_cells */


/*===========================================================================

FUNCTION      garb_cxm_set_desense_band

DESCRIPTION   This function gets desense band in X+G DSDA ( DR-DSDS).

RETURN VALUE  none.

===========================================================================*/
void garb_cxm_set_desense_band(gas_id_t gas_id)
{
  /*   structure type to query  action from coex*/
  cxm_highest_action_query_s  gsm_multirat_coex_desense_query;
  /* Action array for the corresponding requesting ids */
  uint32 gsm_multirat_desense_req_ids[ 2 ];
  /* Parameter type for cxm query function */  
  cxm_action_type gsm_multirat_desense_actions[ 2 ];
  ARFCN_T  serving_arfcn = gl1_get_serving_cell_arfcn(gas_id);
  sys_band_T               desense_band = serving_arfcn.band;

  /* Do not update desense_band as they will not be a desense conflict as per device allocation */
  /* It will erroneously update the desense_band */
  if( (! garb_intf_gas_is_xpg_coex_victim(gas_id)) ||
      ( arbitrated_gas[gas_id].params_monscan.ba_list_size == 0) || /* BA list does not have interband cells */
      ( arbitrated_gas[gas_id].params_idle.interband == INVALID_BAND) || // No Interband Monitors.
      (! gl1_hw_qta_gap_active(gas_id))) // Not in QxTA
  {
    return;  
  }

  gsm_multirat_coex_desense_query.num_requesting_ids = 2;
  gsm_multirat_coex_desense_query.actions            = gsm_multirat_desense_actions;

  /* Query MCS/CXM and obtain the desense band */
  switch( serving_arfcn.band )
  {
     case SYS_BAND_DCS_1800:
     case SYS_BAND_PGSM_900:
     case SYS_BAND_EGSM_900:

       gsm_multirat_desense_req_ids[0] = arbitrated_gas[gas_id].dl_band_table[SYS_BAND_DCS_1800];
       gsm_multirat_desense_req_ids[1] = arbitrated_gas[gas_id].dl_band_table[SYS_BAND_EGSM_900];

       gsm_multirat_coex_desense_query.requesting_ids =  gsm_multirat_desense_req_ids;

       if ( cxm_query_highest_action( &gsm_multirat_coex_desense_query ) )
       {
          if (gsm_multirat_desense_actions[0] != ACTION_NONE)
          {
            desense_band = SYS_BAND_DCS_1800;
            MSG_GERAN_MED_2( "Desense GSM1800: serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
          else if (gsm_multirat_desense_actions[1] != ACTION_NONE)
          {
            desense_band = SYS_BAND_EGSM_900;
            MSG_GERAN_MED_2( "Desense GSME900 :serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
          else  // no desense
          {
            desense_band = serving_arfcn.band;
            MSG_GERAN_MED_2( "No desense :serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
       }
       else
       {
          MSG_GERAN_ERROR_0( "cxm_query_highest_action ERROR ");
       }
  
  
       break;
  
    case SYS_BAND_PCS_1900:
    case SYS_BAND_CELL_850:
  
       gsm_multirat_desense_req_ids[0] = arbitrated_gas[gas_id].dl_band_table[SYS_BAND_PCS_1900];
       gsm_multirat_desense_req_ids[1] = arbitrated_gas[gas_id].dl_band_table[SYS_BAND_CELL_850];
  
       gsm_multirat_coex_desense_query.requesting_ids =  gsm_multirat_desense_req_ids;
  
       if ( cxm_query_highest_action( &gsm_multirat_coex_desense_query ) )
       {
          if (gsm_multirat_desense_actions[0] != ACTION_NONE)
          {
            desense_band = SYS_BAND_PCS_1900;
            MSG_GERAN_MED_2( "Desense GSM1900 : serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
          else if (gsm_multirat_desense_actions[1] != ACTION_NONE)
          {
            desense_band = SYS_BAND_CELL_850; 
            MSG_GERAN_MED_2( "Desense GSM850: serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
          else  // no desense
          {
            desense_band = serving_arfcn.band;
            MSG_GERAN_MED_2( "No desense : serving_band %d desense band %d", serving_arfcn.band,desense_band);
          }
       }
       else
       {
          MSG_GERAN_ERROR_0( "cxm_query_highest_action ERROR ");
       } 
  
       break;
  
    default :
      MSG_GERAN_ERROR_1( "Unsupported RF band:%d",  serving_arfcn.band );
      desense_band = serving_arfcn.band;
      break;
  }

  arbitrated_gas[gas_id].desense_band = desense_band;
} /* garb_cxm_set_desense_band */

#endif /*  FEATURE_GSM_COEX */

#endif /* FEATURE_GSM_COEX_SW_CXM */
