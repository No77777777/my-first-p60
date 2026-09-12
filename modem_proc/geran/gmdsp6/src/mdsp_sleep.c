/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L A Y E R  1  - M D S P   I N T E R F A C E
                    mDSP Sleep Interface Module

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS
  mdsp_display_sleep_error
  mdsp_sleep_error_pending
  mdsp_go_to_sleep,mdsp_gsm_drv_go_to_sleep,mdsp_gsm_drv_wakeup [deprecated]

  mdsp_wakeup
  mdsp_sleep_init
  mdsp_sleep_inact
  mdsp_awake
  mdsp_gsm_sleep_cmd_done_isr
  mdsp_prepare_for_sleep
  mdsp_ready_for_sleep


  Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when         who     what, where, why
----------   ------- ------------------------------------------------------
27/01/20      rks    CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18/12/15      zf     CR952643: Check gfwSleepState word-wide not each element
31/04/16      ksb    CR995601 Debug code when no reponse in last 4 for mdsp-sleep
20/04/15      ip     CR811868 Dual Multisim mode code changes + debug overhaul
07/01/15      smd    CR774443 Debug crash when GFW returns "sleep not ready" for more than 4 frames.
20/11/14      js     CR733966 Debug crach code when GFW is stuck for more than 2 frames.
30/10/14      cja    CR747066 For X2G do not wake up MDSP if already disabled  by CM terminate
07/10/14      pg     CR730132 F3 cleanup 
06/08/14      df     CR536059 Full asynchronous sleep support in idle
09/07/14      cja    CR690810 Do not wait for ISR lock in mdsp sleep cmd if G2X in progress
28/05/14      cja    CR657368 Fix Klockwork issue of NULL data space pointer
30/04/14      cja    CR657910 Check GFW ccs pending during prepare_for_sleep
27/04/14      nk     CR654749 Add check for asyncCmdPending to mdsp_ready_for_sleep()
24/04/14      cja    CR655328 Prevent deadlock by doing nothing for mdsp sleep cmd in X2G or QTA
18/04/14      dv     CR651891 Do not interact with GFW if its not awake.  
07/04/14      cja    CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
03/04/14      ws     CR 642097 - Remove check for gstmr_is_int_pending() in mdsp_is_mdsp_idle()
22/03/13      ws     CR636679 - Add check for asyncCmdPending to mdsp_is_mdsp_idle()
17/03/14      pg     CR632107 Check GFW power scan activity during prepare_for_sleep 
11/02/14      ap     CR612459: Deactivation and sync rx occurring simultaneously resulting in GFW crash
10/12/13      ws     CR 587586 - Remove DUAL_SIM featurisation in API
11/27/13      nm     CR571729 - Remove incorrect checking of 3rd command buffer inside mdsp_ready_for_sleep()
22/11/13      cja    CR455527 Remove featurisation from files in API directory
04/10/13      ws     CR 570719 - Correct MDSP sleep external API for TDS L1
05/10/13      gk     CR 531225 dont return ready for sleep if tx is scheduled
10/07/13      ws     CR 510980 Implement external API gl1_cm_mdsp_ready_for_sleep()
18/07/13      jj     CR 514515 check tx shared variable before panic reset mdsp sleep
21/06/13      sk     CR502900 Ensure GSTMR2 uses correct host interface segment to populate the commands
21/06/13      cs     Major Triton TSTS Syncup
10/05/13      cs     TSTS Updates
29/01/13      cja    CR446356 Set msgr_init_hdr 'from' parameter as MSGR_GERAN_GL1
22/01/13      cja    CR443834 Extend mdsp_awake to indicate FW enable/disable
22/10/12      cja    CR410672 MDSP not ready for sleep if commands pending
11/01/13      ws     Fixed bug in mdsp_ready_for_sleep() Wrong UMID cmd was being sent
22/10/12      cja    CR410672 MDSP not ready for sleep if commands pending
03/07/12      ky     Async message router sleep interface changes
14/03/12      pg     Add support for DAL interrupt controller (tramp deprecated)
02/02/12      cja    CR328278 Do not use semaphore in MDSP command buffer header in CM
04/11/11      cja    CR316556 Check sleep cmd id rather than interrupt for sleep.
31/10/11      og     CR315998. Ignore the sleep isr pending change when waiting
                     for the start sleep ack isr from GFW.
21/10/11      cja    CR313399 Optimise wait for MDSP sleep, plus allow longer maximum time
20/09/11      cja    Remove unexpected ISR error message for OPT3 not enabled.
19/08/11      cgc    Cr291164 add mutex protection for QDSP6 - GDRV_ISR_LOCK/UNLOCK + header gl1_mutex.h
07/08/11      ws     CR 299800 - Corection to use rxRfTriggersPending in gfw intf
                     update with GFW
05/08/11      ws     CR 299800 - Enable Sleep for Nikel MSM8960
04/06/11      ws     tramp_send_interrupt() deprecated on Nikel target, write
                     directly to SW_QDSP6SS_INTn for ASYNC sleep interrupt
20/04/11      cja    Nikel GFW interface changes
08/02/11      cja    Initial changes for Nikel
14/02/11      npt    CR264682 Add support for asynch dsp completion interrupt
11/01/11      cgc    Fix compiler warnings add gl1_hw.h
10/12/10      nf     9K mdsp_awake_flag now shows the actual state of the GFW
02/12/10      nf     9K changed the way clk delays and clk switching is handled
08/11/10      nf     9K suspend resume changes
09/10/10      nf     9K added FWS suspend/resume
09/10/10      nf     CR259188 increased _RSP delays to take into accoutn suspend/resume
23/09/10     tjw     Replace deprecated clk_busy_wait with DALSYS_BusyWait
09/07/10      pg     CR255064: Check MDSP is both AWAKE and INACTIVE when determining whether
                     MDSP is ready for sleep.
22/03/10      ky     Sleep interrupts between SW and FW are changed and also removed
                     usage of tramp_is_interrupt_pending function in QDSP6. Intlock, IntFree
                     usage is removed for QDSP6
09/03/10      ky     Sleep changes for QDSP6 target
12/11/09      ws     Commented out QDSP4 code for MDM 9K
08/03/07      rc     Removed MDSP process Err function call until CR116045 and CR120513 is fixed, Else causes MDSP HALT when MS is in idle in EDGE Cell,
04/07/06      gfr    Added missing header file
01/19/06      gfr    Increase number of parameters to mdsp_process_error
11/07/05      gfr    Removed FEATURE_GSM_MDSP_DM_ACTIVE_INT
08/17/05      gfr    QCLint cleanup - shorten lines to less than 90 chars
06/01/05      gfr    Dump memory on sleep nack errors
04/28/05      gfr    Add support for the DM active interrupt and sleep_inact
04/13/05      gfr    Print cmd status and awake state if missed interrupt
02/24/05      yh     Add a parameter to print sleep message only on the third
                     try for a NACKED error.
01/24/05      gfr    Initial version.
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_async_intf.h"
#include "mdsp_cmd.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "msg.h"
#include "DALSys.h"
#include "fws.h"

#include "gl1_mutex.h"

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1_sc_int.h"

#include "msgr.h"
#include "geran_msg.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

/*==========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

==========================================================================*/

/* Bit masks for the sleep command */
#define MDSP_SLEEP_CMD_SLEEP   0x0001
#define MDSP_SLEEP_CMD_WAKEUP  0x0002
#define MDSP_SLEEP_CMD_QUERY   0x8000

/* Masks for the gsmAwakeState variable */
#define MDSP_AWAKE_STATE_MASK_STATE  0x8000

/* Maximum time to wait when sending goto sleep command in ms */
#if defined (FEATURE_GSM_NIKEL_HW)
#define MDSP_SLEEP_CMD_MAX_SLEEP_RSP_DELAY  500
#define MDSP_SLEEP_CMD_MAX_WAKEUP_RSP_DELAY 700
#else
#define MDSP_SLEEP_CMD_MAX_SLEEP_RSP_DELAY  200
#define MDSP_SLEEP_CMD_MAX_WAKEUP_RSP_DELAY 400
#endif
#define MDSP_SLEEP_DELAY_LOOP                50
#define MDSP_SLEEP_CMD_MAX_QUERY_RSP_DELAY  300

#define MDSP_SLEEP_CMD_CM_WAKEUP_RSP_DELAY  75

/* Type to keep track of possible reasons the sleep ISR could run */
typedef enum
{
   MDSP_SLEEP_ISR_NONE,
   MDSP_SLEEP_ISR_PENDING,
   MDSP_SLEEP_ISR_MISSED
} mdsp_sleep_isr_state_type;

/* Storage for saving sleep errors that may not be displayed immediately */
typedef enum
{
   MDSP_SLEEP_ERROR_WAKEUP_ACK_MISSING,
   MDSP_SLEEP_ERROR_CMD_NOT_CLEARED_DURING_WAKEUP,
   MDSP_SLEEP_ERROR_DID_NOT_WAKEUP,
   MDSP_SLEEP_ERROR_GOTO_SLEEP_ACK_MISSING,
   MDSP_SLEEP_ERROR_CMD_NOT_CLEARED_DURING_SLEEP,
   MDSP_SLEEP_ERROR_NACKED_REQUEST,
   MDSP_SLEEP_ERROR_UNEXPECTED_CMD_WHEN_QUERYING,
   MDSP_SLEEP_ERROR_READY_FOR_SLEEP_ACK_MISSING,
   MDSP_SLEEP_ERROR_CMD_NOT_CLEARED_DURING_QUERY
} mdsp_sleep_error_type;

#define MDSP_MAX_WAKEUP_RETRIES 3
#define MDSP_MAX_PENDING_SLEEP_ERRORS (MDSP_MAX_WAKEUP_RETRIES + 1)

typedef struct
{
   mdsp_sleep_error_type error;
   uint16                sleep_cmd;
   uint16                awake_state;
} mdsp_sleep_error_info_type;


/* Prototype for the isr handler */
void mdsp_gsm_sleep_cmd_done_isr_GASID_1(void);
void mdsp_gsm_sleep_cmd_done_isr_GASID_2(void);

#ifdef FEATURE_TRIPLE_SIM
void mdsp_gsm_sleep_cmd_done_isr_GASID_3( void );
#endif /* FEATURE_TRIPLE_SIM */

void mdsp_gsm_sleep_cmd_done_isr ( gas_id_t gas_id );

/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/
typedef struct
{

  /* Flag to indicate whether switch command buffers should read from
   * mDSP or not.  Cannot read from mDSP if the mDSP clk regime has
   * been disabled.
   */
  boolean mdsp_awake_flag;

  mdsp_sleep_isr_state_type mdsp_sleep_isr_state;

  mdsp_sleep_error_info_type mdsp_sleep_error_info[MDSP_MAX_PENDING_SLEEP_ERRORS];

  /* Count of pending sleep errors, more than one may pile up during multiple
     wakeup attempts */
  uint32 mdsp_pending_sleep_errors;

  void (*mdsp_gsm_sleep_cmd_done_isr_handler)(void);

} mdsp_sleep_private_data_t;

/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/
static mdsp_sleep_private_data_t mdsp_sleep_private_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_sleep_private_data_t* get_private_dataspace_ptr(const gas_id_t gas_id )
{
  mdsp_sleep_private_data_t *data_ptr;

  data_ptr =
      &(mdsp_sleep_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}


/*===========================================================================

FUNCTION init_private_data_space

DESCRIPTION
  This function initialises a public data space

===========================================================================*/
static void init_private_data_space( gas_id_t gas_id )
{
  mdsp_sleep_private_data_t *data = get_private_dataspace_ptr( gas_id);

  data->mdsp_sleep_isr_state = MDSP_SLEEP_ISR_NONE;

  data->mdsp_pending_sleep_errors = 0;

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      data->mdsp_gsm_sleep_cmd_done_isr_handler =
        &mdsp_gsm_sleep_cmd_done_isr_GASID_1;
    break;

    case GERAN_ACCESS_STRATUM_ID_2:
      data->mdsp_gsm_sleep_cmd_done_isr_handler =
        &mdsp_gsm_sleep_cmd_done_isr_GASID_2;
    break;

#ifdef FEATURE_TRIPLE_SIM
    case GERAN_ACCESS_STRATUM_ID_3:
      data->mdsp_gsm_sleep_cmd_done_isr_handler =
        &mdsp_gsm_sleep_cmd_done_isr_GASID_3;
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
      MSG_GERAN_FATAL_0_G( "GAS ID is out of range" );
    break;
  }
}

/*===========================================================================

FUNCTION mdsp_display_sleep_error

DESCRIPTION
  This function will display the sleep error stored by the driver.

===========================================================================*/
void mdsp_display_sleep_error (boolean print_nacked_error_msg , gas_id_t gas_id )
{
   uint8 i;

   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   for (i = 0; i < private_data_ptr->mdsp_pending_sleep_errors; i++)
   {
      /* Nacked requests are not bad enough to log or dump memory */
      if (private_data_ptr->mdsp_sleep_error_info[i].error != MDSP_SLEEP_ERROR_NACKED_REQUEST)
      {
         MSG_GERAN_ERROR_3_G("mDSP sleep error #%d: sleepCmd=0x%04x, awakeState=0x%04x",
                   private_data_ptr->mdsp_sleep_error_info[i].error,
                   private_data_ptr->mdsp_sleep_error_info[i].sleep_cmd,
                   private_data_ptr->mdsp_sleep_error_info[i].awake_state);

      /*   mdsp_process_error
          (
            MDSP_GSM_ERROR_SLEEP,
            (uint32)private_data_ptr->mdsp_sleep_error_info[i].error,
            private_data_ptr->mdsp_sleep_error_info[i].sleep_cmd,
            private_data_ptr->mdsp_sleep_error_info[i].awake_state
            , gas_id
          ); */
      }
      else
      {
         /* Display MSG_HIGH if it is in the first 2 tries, ERROR otherwise */
         if (!print_nacked_error_msg)
         {
            MSG_GERAN_HIGH_1_G("mDSP nacked sleep request: awakeState=0x%04x",
                     private_data_ptr->mdsp_sleep_error_info[i].awake_state);
         }
         else
         {
            MSG_GERAN_ERROR_1_G("mDSP nacked sleep request: awakeState=0x%04x",
                      private_data_ptr->mdsp_sleep_error_info[i].awake_state);

        /*    mdsp_process_error
             (
               MDSP_GSM_ERROR_SLEEP,
               (uint32)private_data_ptr->mdsp_sleep_error_info[i].error,
               private_data_ptr->mdsp_sleep_error_info[i].sleep_cmd,
               private_data_ptr->mdsp_sleep_error_info[i].awake_state
               , gas_id
             ); */
         }
      }
   }
   private_data_ptr->mdsp_pending_sleep_errors = 0;
}


/*===========================================================================

FUNCTION mdsp_save_sleep_error

DESCRIPTION
  This function saves the sleep error parameters for later display.

===========================================================================*/
static void mdsp_save_sleep_error
(
   mdsp_sleep_error_type error_num,
   uint16 sleep_cmd,
   uint16 awake_state
  , gas_id_t gas_id
)
{
   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* If the buffer is full just overwrite the last entry, we cannot
      print an error message since we are in the sleep timeline */
   if (private_data_ptr->mdsp_pending_sleep_errors == MDSP_MAX_PENDING_SLEEP_ERRORS)
   {
      private_data_ptr->mdsp_pending_sleep_errors--;
   }

   private_data_ptr->mdsp_sleep_error_info[private_data_ptr->mdsp_pending_sleep_errors].error = error_num;
   private_data_ptr->mdsp_sleep_error_info[private_data_ptr->mdsp_pending_sleep_errors].sleep_cmd = sleep_cmd;
   private_data_ptr->mdsp_sleep_error_info[private_data_ptr->mdsp_pending_sleep_errors].awake_state = awake_state;

   private_data_ptr->mdsp_pending_sleep_errors++;
}


/*===========================================================================

FUNCTION mdsp_is_sleep_error_pending

DESCRIPTION
  Returns if an error is pending for display.

===========================================================================*/
boolean mdsp_sleep_error_pending ( gas_id_t gas_id )
{
   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   return (private_data_ptr->mdsp_pending_sleep_errors > 0 ? TRUE : FALSE);
}


/*===========================================================================

FUNCTION mdsp_go_to_sleep, mdsp_wakeup

DESCRIPTION
  These two functions change the awake state of the mDSP drivers.  When the
  drivers are awake the function mdsp_switch_command buffers will read
  from the mDSP shared memory on every frame tick.  When they are asleep
  this does not happen.  The mDSP drivers must be put in the asleep state
  (by calling mdsp_go_to_sleep) before the mDSP clk regime is disabled.

  Depending on feature definitions these functions my also vote for the
  mDSP clock regime with mdsp services.

  We do not display error messages directly in this function as it may
  disturb the sleep timeline.

===========================================================================*/
void mdsp_go_to_sleep(gas_id_t gas_id )
{
}

boolean mdsp_wakeup(boolean comp_mode
#ifdef FEATURE_DUAL_SIM
                    , sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
                    )
{
   uint32 fws_mask;
   boolean wakeup_success;
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   /* Prevent race condition with cm terminate, and only do wake up is GFW enabled */
   GDRV_ISR_SAVE_LOCK(gas_id);

   fws_mask = mdsp_get_fws_app_mask( gas_id );

   if((gl1_hw_fws_app_enabled() & fws_mask))
   {
      wakeup_success = mdsp_wakeup_geran(comp_mode, as_id);
   }
   else
   {
      MSG_GERAN_ERROR_0_G("mdsp wakup request, but GFW not enabled");
      wakeup_success = FALSE;
   }

   GDRV_ISR_SAVE_UNLOCK(gas_id);
   return wakeup_success;
}


/*===========================================================================

FUNCTION mdsp_wakeup

DESCRIPTION
  See mdsp_go_to_sleep

===========================================================================*/
boolean mdsp_wakeup_geran (boolean comp_mode, sys_modem_as_id_e_type as_id )
{
   GfwHostSleepCmd gfwHostSleepCmd;
   GfwHostSleepRsp gfwHostSleepRsp;
   uint32 fws_resume_mask;
   mdsp_sleep_private_data_t* private_data_ptr;

   boolean awake = FALSE;

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   private_data_ptr   = get_private_dataspace_ptr( gas_id);

   fws_resume_mask    = mdsp_get_fws_app_mask( gas_id );

   fws_resume(fws_resume_mask);

   if (comp_mode == FALSE)
   {
      mdsp_reset_command_buffers(gas_id );
   }
   gfwHostSleepCmd.cmd = GFW_WAKE;
   /* send msgr message to GFW */
   (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_SLEEP_CMD, &gfwHostSleepCmd, &gfwHostSleepRsp, gas_id);

   /* Check MDSP is awake, but not actively processing */
   if (gfwHostSleepRsp.gfwSleepState.gfw != 1)
   {
     mdsp_save_sleep_error
     (
       MDSP_SLEEP_ERROR_DID_NOT_WAKEUP,
       (uint16)GFW_WAKE,
       gfwHostSleepRsp.gfwSleepState.gfw,
       gas_id
     );
   }
   else
   {
      awake = TRUE;
   }

   private_data_ptr->mdsp_awake_flag = awake;

   /* Otherwise we are good to go */
   return awake;
}

/*===========================================================================

FUNCTION mdsp_sleep_init

DESCRIPTION
  Initialize sleep variables.

===========================================================================*/
void mdsp_sleep_init ( gas_id_t gas_id )
{
   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   init_private_data_space(gas_id);

   /* Start out awake */
   private_data_ptr->mdsp_awake_flag = TRUE;
}


/*===========================================================================

FUNCTION mdsp_sleep_inact

DESCRIPTION
  Inactivates sleep variables.

===========================================================================*/
void mdsp_sleep_inact (void)
{

}



/*===========================================================================

FUNCTION mdsp_awake

DESCRIPTION
  Returns if the mdsp is awake or not.

===========================================================================*/
boolean mdsp_awake ( gas_id_t gas_id )
{
   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);
   return private_data_ptr->mdsp_awake_flag;
}

/*===========================================================================

FUNCTION mdsp_awake_set

DESCRIPTION
  Returns if the mdsp is awake or not.

===========================================================================*/
void mdsp_awake_set (boolean mdsp_awake_state,gas_id_t gas_id)
{
   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   private_data_ptr->mdsp_awake_flag = mdsp_awake_state;
}

/*===========================================================================

FUNCTION mdsp_gsm_drv_go_to_sleep, mdsp_gsm_drv_wakeup

DESCRIPTION
  These functions are similar to mdsp_go_to_sleep, mdsp_wakeup, but they should
  be called when we are in WCDMA mode

===========================================================================*/
void mdsp_gsm_drv_go_to_sleep( void )
{
}

void mdsp_gsm_drv_wakeup( void )
{
}


/*===========================================================================

FUNCTION mdsp_gsm_sleep_cmd_done_isr_GASID_1

DESCRIPTION
  Interrupt handler for GASID 1 in dual dataspaced builds

===========================================================================*/
void mdsp_gsm_sleep_cmd_done_isr_GASID_1(void)
{
  mdsp_gsm_sleep_cmd_done_isr(GERAN_ACCESS_STRATUM_ID_1);
}

/*===========================================================================

FUNCTION mdsp_gsm_sleep_cmd_done_isr_GASID_2

DESCRIPTION
  Interrupt handler for GASID 2 in dual dataspaced builds


===========================================================================*/
void mdsp_gsm_sleep_cmd_done_isr_GASID_2(void)
{
  mdsp_gsm_sleep_cmd_done_isr(GERAN_ACCESS_STRATUM_ID_2);
}

#ifdef FEATURE_TRIPLE_SIM
/*===========================================================================

FUNCTION mdsp_gsm_sleep_cmd_done_isr_GASID_3

DESCRIPTION
  Interrupt handler for GASID 1 in dual dataspaced builds

===========================================================================*/
void mdsp_gsm_sleep_cmd_done_isr_GASID_3( void )
{
  mdsp_gsm_sleep_cmd_done_isr( GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION mdsp_gsm_sleep_cmd_done_isr

DESCRIPTION
  Interrupt handler for handling the response from firmware to the goto
  sleep command.

===========================================================================*/
void mdsp_gsm_sleep_cmd_done_isr( gas_id_t gas_id )
{
   if (mdsp_cm_active(gas_id) || gl1_hw_qta_gap_active(gas_id))
   {
      MSG_GERAN_LOW_0_G(GL1_SLEEP_HDR" mDSP sleep ISR in CM or QTA");
   }
   else
   {
      mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);
      /* Check if we are expecting this interrupt */
      switch (private_data_ptr->mdsp_sleep_isr_state)
      {
         case MDSP_SLEEP_ISR_PENDING:
            /* Normal case */
            MSG_GERAN_LOW_1_G(GL1_SLEEP_HDR" mDSP sleep ISR, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
            break;

         case MDSP_SLEEP_ISR_NONE:
            if ( (gl1_is_sleep_opt3_enabled( gas_id )||
                  gl1_is_asynch_sleep_enabled( gas_id ))
                 &&
                 (!l1_sc_wcdma_srch_active(gas_id)))

            {
               /*
               ** CgC concurrency protection required for QDSP6 targets
               */
               GDRV_ISR_LOCK(gas_id);
               if ( mdsp_asynch_dsp_int_enabled( gas_id ) )
               {
                  gstmr_asynch_dsp_int(gas_id);
               }
               else
               {
                  MSG_GERAN_LOW_0_G(GL1_SLEEP_HDR" mDSP sleep ISR but aysnch dsp in just disabled");
               } 
               GDRV_ISR_UNLOCK(gas_id);
            }
            break;

         case MDSP_SLEEP_ISR_MISSED:
            /* We missed the ISR and it has now arrived */
            MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" mDSP sleep ISR arrived late, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
            break;
         default:
            MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" unexpected mdsp_sleep_isr_state=%d", private_data_ptr->mdsp_sleep_isr_state);

      }

      /* Reset the state */
      private_data_ptr->mdsp_sleep_isr_state = MDSP_SLEEP_ISR_NONE;
   }
}

/*===========================================================================

FUNCTION mdsp_prepare_for_sleep

DESCRIPTION
  Function called to check that the firmware is ready to go to sleep, and
  if so put it to sleep.  Does not disable clocks, this is done by calling
  mdsp_go_to_sleep.
  We do not display error messages directly in this function as it may
  disturb the sleep timeline.

===========================================================================*/
boolean mdsp_prepare_for_sleep (  gas_id_t gas_id  )
{
   GfwHostSleepCmd gfwHostSleepCmd;
   GfwHostSleepRsp gfwHostSleepRsp;

   uint32 fws_suspend_mask;

   boolean asleep = FALSE;

   mdsp_sleep_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   fws_suspend_mask = mdsp_get_fws_app_mask( gas_id );

   /* populate payload */
   gfwHostSleepCmd.cmd  = GFW_SLEEP;

   /* send msgr message to GFW */
   (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_SLEEP_CMD, &gfwHostSleepCmd, &gfwHostSleepRsp, gas_id);

   /* Check MDSP is awake, but not actively processing */
   if (gfwHostSleepRsp.gfwSleepState.gfwSleepState != 0)
   {
      mdsp_save_sleep_error(MDSP_SLEEP_ERROR_NACKED_REQUEST,(uint16)GFW_SLEEP,gfwHostSleepRsp.gfwSleepState.gfwSleepState, gas_id);
   }
   else
   {
      uint32 fws_status = fws_suspend(fws_suspend_mask);
      MSG_GERAN_MED_1_G(GL1_SLEEP_HDR" GFW Suspend %d",fws_status);
      mdsp_sleep_reset_mdsp_intf( gas_id );
      asleep = TRUE;
   }

   private_data_ptr->mdsp_awake_flag = !asleep;

   /* Otherwise we are good to go */
   return asleep;
}

/*===========================================================================

FUNCTION mdsp_is_mdsp_idle

DESCRIPTION
  Function called to check that the firmware is idle and not processing any
  commands

===========================================================================*/
void mdsp_is_mdsp_idle (  gas_id_t gas_id  )
{
  GfwHostSleepCmd gfwHostSleepCmd;
  GfwHostSleepRsp gfwHostSleepRsp;
  uint32 gfw_resp_cnt;
  
  /* populate payload */
  gfwHostSleepCmd.cmd  = GFW_SLEEP_QUERY;

  if (mdsp_awake(gas_id))
  {
     gfw_resp_cnt = 0;
     (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_SLEEP_CMD, &gfwHostSleepCmd, &gfwHostSleepRsp, gas_id);

     /* Check MDSP is awake, but not actively processing */
     while (gfwHostSleepRsp.gfwSleepState.gfwSleepState != 1)
     {
       gl1_hw_delay(1000, gas_id);
       if (gfw_resp_cnt % 4 == 0)
       {
          /* Avoid recoved F3s being flooded so we print the message at the first 4 sleep commands 
             and then every 2ms */
       MSG_GERAN_HIGH_2_G("qbit = %d gfwSleepState %d",
						  gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)),
						  gfwHostSleepRsp.gfwSleepState.gfwSleepState);
       }
       /* send msgr message to GFW */
       (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_SLEEP_CMD, &gfwHostSleepCmd, &gfwHostSleepRsp, gas_id);
       
       if(gfw_resp_cnt > 18)
       {
#ifdef FEATURE_QSH_MDUMP
         QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
	     /* 18*1000 us = 18 ~= 4 frames */
         ERR_FATAL("NO reponse in last 4 frames =%d",gfw_resp_cnt,0,0);
       }
       else
       {
         gfw_resp_cnt ++ ;
       }
     }
      MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR" SleepCmd sent %d times, RSP from FW is good",gfw_resp_cnt);
  }
  else
  {
    MSG_GERAN_ERROR_0_G(GL1_SLEEP_HDR" GFW Sleep: GFW  is not awake so no need for it to sleep");
  }
}

/*===========================================================================

FUNCTION gl1_cm_mdsp_prepare_for_sleep

DESCRIPTION
  External function called to put the firmware to sleep.

===========================================================================*/
boolean gl1_cm_mdsp_prepare_for_sleep(sys_modem_as_id_e_type as_id)
{
  return(mdsp_prepare_for_sleep(geran_map_nas_id_to_gas_id(as_id)) );
}

/*===========================================================================

FUNCTION mdsp_ready_for_sleep

DESCRIPTION
  Function called to check that the firmware is ready to go to sleep.

===========================================================================*/
boolean mdsp_ready_for_sleep (  gas_id_t gas_id  )
{
   GfwHostSleepCmd gfwHostSleepCmd;
   GfwHostSleepRsp gfwHostSleepRsp;
   volatile GfwHostCmdBufType*  hostCmd0;
   volatile GfwHostCmdBufType*  hostCmd1;
   mdsp_intf_public_data_t*   mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   hostCmd0 = (GfwHostCmdBufType *)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[0];
   hostCmd1 = (GfwHostCmdBufType *)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[1];
   /* First check that there are no MDSP commands pending in either buffer */
   if( ( hostCmd0->semaphoreAndNumCommands & ~MDSP_SEMAPHORE_BIT ) || ( hostCmd1->semaphoreAndNumCommands & ~MDSP_SEMAPHORE_BIT )
   )
   {
      MSG_GERAN_MED_2_G("MDSP not ready for sleep as G1:%d G2:%d commands pending",
               hostCmd0->semaphoreAndNumCommands & ~MDSP_SEMAPHORE_BIT,
               hostCmd1->semaphoreAndNumCommands & ~MDSP_SEMAPHORE_BIT );
      return (FALSE);
   }

   /* populate payload */
   gfwHostSleepCmd.cmd  = GFW_SLEEP_QUERY;

   /* send msgr message to GFW */
   (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_SLEEP_CMD, &gfwHostSleepCmd, &gfwHostSleepRsp, gas_id);

   /* Check MDSP is awake, but not actively processing */
   if (gfwHostSleepRsp.gfwSleepState.gfwSleepState == 1)
   {
      return (TRUE);
   }
   else
   {
      return (FALSE);
   }
}

/*===========================================================================

FUNCTION gl1_cm_mdsp_ready_for_sleep

DESCRIPTION
  External function called to check that the firmware is ready to go to sleep.

===========================================================================*/
boolean gl1_cm_mdsp_ready_for_sleep(sys_modem_as_id_e_type as_id)
{
  return(mdsp_ready_for_sleep(geran_map_nas_id_to_gas_id(as_id)) );
}




