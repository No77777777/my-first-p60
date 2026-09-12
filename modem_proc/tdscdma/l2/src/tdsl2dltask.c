/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

        L A Y E R  2   D O W N L I N K   TASK

GENERAL DESCRIPTION
  This file contains the Downlink Layer 2 task function.

Copyright (c) 20010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

=============================================================================================*/
/*=============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/L2/vcs/l2dltask.c_v   1.11   11 Jun 2002 18:09:22   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/l2/src/tdsl2dltask.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------------------------
10/13/14  sarao  CFCM changes for RLC
06/02/14  sarao   Removed the inclusion of tmc.h as its being discontinued in core.
10/24/10   guo   Initial release Upmerged WCDMA to TDS

=============================================================================================*/

/*===============================================================================================

              I N C L U D E    F I L E S

=============================================================================================*/
#include "tdscdma_variation.h"
#include  "rex.h"
#include  "task.h"
#ifdef FEATURE_DOG
  //#include  "dog.h"  
#include "dog_hb_rex.h"   //using DOG HB mechanism
#endif

#include  "msg.h"
#include  "err.h"
#include  "tdsl2dltask.h"
#include  "tdsmacdlsubtask.h"
#include  "tdsrlcdl.h"
#include  "tdsrlcdlsubtask.h"
#include  "tdsrlcdlam.h"
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
#include  "tdsrlcfc.h"
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif
#include "tdsCFAParser.h"
#include  "log_msgs_tdscdma.h"
#ifdef FEATURE_MODEM_RCINIT 
#include "rcinit.h"
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /*FEATURE_SEGMENT_LOADING*/



/*=============================================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

=============================================================================================*/

/* L2 DL TCB handle */
static rex_tcb_type* tds_l2_dl_tcb_ptr = RCINIT_NULL;


/* Downlink layer 2 command queue */
q_type          tdsl2_dl_cmd_q;

#if 0  //using DOG HB mechanism
#ifdef FEATURE_DOG
  //If defined with legacy Dog, then undef it
  #undef  DOG_TDS_L2DL_RPT
  #define DOG_TDS_L2DL_RPT tds_l2dl_dog_rpt_var
  dog_report_type   tds_l2dl_dog_rpt_var           = 0;      /* Initial Safety */
  //If defined with legacy Dog, then undef it

  #undef  DOG_TDS_L2DL_RPT_TIME
  #define DOG_TDS_L2DL_RPT_TIME tds_l2dl_dog_rpt_time_var
  uint32  tds_l2dl_dog_rpt_time_var      = 0xFFFF; /* Initial Safety */


/* L2 DL watchdog report timer block  */
rex_timer_type  tdsl2_dl_dog_rpt_timer_blk;
#endif
#endif

#ifdef FEATURE_SEGMENT_LOADING
extern void tds_task_null (void);
#endif /*FEATURE_SEGMENT_LOADING*/

void tds_l2_dl_init_tcb(void)
{
  if (tds_l2_dl_tcb_ptr == RCINIT_NULL)
  {
    tds_l2_dl_tcb_ptr = rcinit_lookup_rextask("tds_l2_dl"); 
    if (tds_l2_dl_tcb_ptr == RCINIT_NULL)
    {
      TDSL2_MSG0(MSG_LEGACY_ERROR,"Lookup for tds_l2_dl_tcb failed");
      ASSERT(0);
    }
  } 
}

rex_tcb_type* tds_l2_dl_get_tcb(void)
{
  if (tds_l2_dl_tcb_ptr == RCINIT_NULL)
  {
    tds_l2_dl_init_tcb();
  }
  return tds_l2_dl_tcb_ptr; 
}


/*=============================================================================================

FUNCTION  tdsl2_dl_wait

DESCRIPTION
  This function waits for any signal in a specified signal mask to be set.
  If this procedure detects that any signal in the specified signal mask is
  set, it will return the entire mask of signals set in the task's tcb.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

=============================================================================================*/

uint32  tdsl2_dl_wait
(
  /* signals to wait on */
  uint32  wait_sigs
)
{
  /* signals returned by rex_wait call */
  uint32  sigs;

    /* Wait for requested signals */
  while (((sigs = rex_get_sigs( rex_self()) ) & wait_sigs) == 0 ) {

    (void)rex_wait(wait_sigs);

  }

  return( sigs );

} /* tdsl2_dl_wait */

/*=============================================================================================

FUNCTION  tdsproc_l2_dl_cmd_queue

DESCRIPTION
  This function processes the downlink layer 2 command queue data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/

void  tdsproc_l2_dl_cmd_queue ( void )
{
  tdsl2_dl_cmd_enum_type   cmd_id;
  tdsl2_dl_cmd_type        *cmd_ptr;

  while ((cmd_ptr = q_get(&tdsl2_dl_cmd_q)) != NULL) {

    /* Get the command ID */
    cmd_id = cmd_ptr->cmd_hdr.cmd_id;

    /* Process the command queue data based on the command ID */
    switch (cmd_id) {

      /* DL MAC Commands */
      case TDSCMAC_DL_CONFIG_REQ:
        TDSL2_MSG0(MSG_LEGACY_LOW,"Rcvd TDSCMAC_DL_CONFIG_REQ");
        tdsproc_dl_mac_commands (cmd_id, &cmd_ptr->cmd_data);
        break;

      /* DL RLC Commands */
      case TDSRLCI_DL_TX_AM_STATUS:
      case TDSCRLC_DL_TM_CONFIG_REQ:
      case TDSCRLC_DL_UM_CONFIG_REQ:
      case TDSCRLC_DL_CIPHER_CONFIG_REQ:
      case TDSRLC_DL_REGISTER_SRVC_REQ:
      case TDSRLC_DL_DEREGISTER_SRVC_REQ:
      case TDSRLC_REGISTER_AM_SDU_DISCARD_CALLBACK_REQ:
      case TDSRLCI_DL_CONFIG_REQ:
      case TDSRLCI_DL_RESET_REQ:       /* RESET request for DL AM RLC  */
      case TDSRLCI_UL_RESET_DONE:      /* Confirm the RESET completion of UL AM RLC  */
      case TDSRLCI_DL_RELEASE_ALL_REQ:
      case TDSRLC_PROCESS_MAC_HS_RESET_REQ:

      case TDSCRLC_DL_ABORT_CIPHER_CONFIG_REQ:

     /* Command to handle SDU discard timeout  */
     case TDSRLC_HANDLE_SDU_DISCARD_TIMEOUT_EVENT:
      case TDSRLC_PDCP_CLEANUP_WM:

      case TDSCRLC_DL_CONTINUE_REQ:

        tdsproc_dl_rlc_commands(cmd_id, &cmd_ptr->cmd_data);

        break;

      /* Invalid command */
      default:
        ERR ("Invalid command:%d", cmd_ptr->cmd_hdr.cmd_id, 0,0  );
        break;
    }/* switch cmd-id */

    /* Free the command buffer */
    /*lint -e424 */
    modem_mem_free (cmd_ptr,MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    /*lint +e424 */

  }/* while (cmd_ptr != NULL) */

} /* tdsproc_l2_dl_cmd_queue */

/*=============================================================================================

FUNCTION  tds_l2_dl_task_init

DESCRIPTION
  This task initializes uplink layer 2 queues.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void tds_l2_dl_task_init (void) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void  tds_l2_dl_task_init ( void )
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;

  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

  /* Initialize downlink layer 2 task command and free command queues */
  (void)q_init(&tdsl2_dl_cmd_q);
#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  tdsrlc_dl_crit_sect_init();
#endif
}


/*=============================================================================================

FUNCTION  tds_l2_dl_task

DESCRIPTION
  This task performs downlink layer 2 processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/

#ifdef FEATURE_SEGMENT_LOADING
void tds_l2_dl_task (dword dummy) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void  tds_l2_dl_task
(
  dword dummy
    /* Required for REX, ignore */
    /*lint -esym(715,dummy)
    ** Have lint not complain about the ignored parameter 'dummy' which is
    ** specified to make this routine match the template for rex_def_task().
    */
)
{
  uint32    sigs;
#ifdef FEATURE_DOG
  dog_report_type     dogRpt;
#endif	
  #ifndef FEATURE_MODEM_RCINIT
  /* Initialize downlink layer 2 task command and free command queues */
  (void)q_init(&tdsl2_dl_cmd_q);
  #endif

#ifdef FEATURE_MODEM_RCINIT
  /* initialize the threshold count for counting Events from TRM */
  RCEVT_THRESHOLD current;
#endif  
  
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;

  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    tds_task_null();
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

  /* Waits for start signal from TMC */
  #ifdef FEATURE_MODEM_RCINIT 
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();
  #else
  /* Perform Startup Handshaking with TMC */
  /* Sets ACK signal when receives the task start signal */
  tmc_task_start();
  #endif
  tds_l2_dl_init_tcb();

  tdsrlc_dl_init();

  /* Initilize the parameters for DL mac */
  tdsinit_dl_mac();

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  /* Initialize RLC-DL flow control */
  tdsrlc_dl_fc_init();
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_MODEM_RCINIT
#if !(defined(TEST_FRAMEWORK) || defined(TF_UNIT_TEST))
  current = rcevt_getcount_name("TRM_INIT_COMPLETE");	
  if(0==current) { 
	(void) rcevt_wait_name("TRM_INIT_COMPLETE");
	}
#endif	
#endif

#ifdef FEATURE_DOG
#if 0 //using DOG HB mechanism
  /*Register with DOG, get the Task ID*/
  tds_l2dl_dog_rpt_var			= dog_register(tds_l2_dl_get_tcb(), \
																DOG_DEFAULT_TIMEOUT, DOG_DEADLKCHK_DISABLE);
  /*Get report interval*/
  tds_l2dl_dog_rpt_time_var = dog_get_report_period(tds_l2dl_dog_rpt_var);

  /* If the return value from dog_get_report_period() was zero then we MUST
  ** NOT report to dog directly, as DOG will monitor us in a different manner.
  ** Hence we won't initialize or start any timer.
  */
  if ( tds_l2dl_dog_rpt_time_var > 0 ) 
  {
    /* Initialize taskname watchdog report timer */
    rex_def_timer(&tdsl2_dl_dog_rpt_timer_blk, tds_l2_dl_get_tcb(), TDSL2_DL_DOG_RPT_TIMER_SIG);
    (void)rex_set_sigs (tds_l2_dl_get_tcb(), TDSL2_DL_DOG_RPT_TIMER_SIG);
  }
  else
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_l2dl_dog_rpt_time_var);
  }
#else
  /* Register with Dog HB. NOTE: Do not register multiple times per task context! */
  dogRpt = dog_hb_register_rex((rex_sigs_type)TDSL2_DL_DOG_RPT_TIMER_SIG);
#endif
#endif

  /* Task loop */
  /*lint -e716 */
  while (1)
  /*lint +e716 */
  {

    /* Wait for DL L2 signal to be set */
      sigs = tdsl2_dl_wait(TDSL2_DL_CMD_Q_SIG |
#ifdef FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER
                      TDSRLC_DL_TIMER_BACKUP_SIG |
#endif

#ifdef FEATURE_DOG
                        TDSL2_DL_DOG_RPT_TIMER_SIG |
#endif

                        TDSMAC_PHY_DL_CONFIG_IND |
                        TDSMAC_PHY_DL_CONFIG_AT_L1_FAILURE_IND |
                        TDSMAC_PHY_HS_CONFIG_IND|


#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
                        TDSRLC_DL_FC_SIG|
                        TDSRLC_DL_FC_TIMER_SIG|
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                        TDSRLC_DL_AM_LOG_TIMER_EXP_SIG |
                        TDSRLC_DL_LOG_CIPHER_PKT_SIG |
#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */
#if 0
//#ifdef FEATURE_Q6_MT
                        RLC_DL_DATA_REASSEMBLY_SIG |
#endif
                        TDSRLC_DL_AM_BASE_TIMER_SIG  |
                        TDSRLC_DL_DATA_IND_SIG
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
		  #error code not present
#endif/*TEST_FRAMEWORK*/     

                        );
    
#ifdef FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER
    if (sigs & TDSRLC_DL_TIMER_BACKUP_SIG)
    {
      /* Clear DL watchdog report timer sig */
      (void)rex_clr_sigs (rex_self(), TDSRLC_DL_TIMER_BACKUP_SIG);
      tdsrlc_dl_am_backup_timer            ();
    }
#endif /* FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER */

#ifdef FEATURE_DOG
    /* Downlink L2 Command Queue Signal is set */
    if (sigs & TDSL2_DL_DOG_RPT_TIMER_SIG)
    {
      /* Clear DL watchdog report timer sig */
      (void)rex_clr_sigs (rex_self(), TDSL2_DL_DOG_RPT_TIMER_SIG);
#if 0 //using DOG HB mechanism
      if (tds_l2dl_dog_rpt_time_var > 0) 
      {    /* Zero means don't report */
        (void)rex_set_timer(&tdsl2_dl_dog_rpt_timer_blk, tds_l2dl_dog_rpt_time_var);
        dog_report(tds_l2dl_dog_rpt_var);
      }
      else
      {
        TDSL2_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_l2dl_dog_rpt_time_var);
      }
#else
      /* Report to the Dog HB */
      dog_hb_report(dogRpt);
#endif
    }
#endif /* FEATURE_DOG */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

    if (sigs & TDSRLC_DL_AM_LOG_TIMER_EXP_SIG)
    {
      /* Clear DL AM log Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_DL_AM_LOG_TIMER_EXP_SIG);

      /* Call AM log timer expiration handler  */
      tdsrlci_dl_am_handle_qxdm_log_timer  (TDSRLC_DL_AM_LOG_TIMER_EXP_SIG);
    }

    if (sigs & TDSRLC_DL_LOG_CIPHER_PKT_SIG)
    {
      /* Clear the signal */
      (void)rex_clr_sigs(rex_self(), TDSRLC_DL_LOG_CIPHER_PKT_SIG);

      /*---------------------------------------------------------------------
        Process the DL cipher packet logging request.
      ---------------------------------------------------------------------*/
      tdsrlc_dl_log_cipher_pkt             ();    
    }

#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

    if (sigs & TDSL2_DL_CMD_Q_SIG)
    {

      /* Clear downlink Layer 2 Command Queue Signal */
      (void)rex_clr_sigs (rex_self(), TDSL2_DL_CMD_Q_SIG);

      /* Process downlink layer 2 command queue */
      tdsproc_l2_dl_cmd_queue ();

    }

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
    if (sigs & TDSRLC_DL_FC_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRLC_DL_FC_SIG);
      tdsrlc_fc_cmd_hdlr(TDSRLC_FC_DOWNLINK);
    }

    if (sigs & TDSRLC_DL_FC_TIMER_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRLC_DL_FC_TIMER_SIG);
      tdsrlc_fc_timer_cmd_hdlr(TDSRLC_FC_DOWNLINK);
    }
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

    if (sigs & TDSRLC_DL_DATA_IND_SIG)
    {

      /* Clear downlink MAC HFN Request Signal */
      (void)rex_clr_sigs (rex_self(), TDSRLC_DL_DATA_IND_SIG);

      /* Process RLC Data Ind */
      tdsrlc_enh_dl_pdu_data_handler       ();
    }


//#ifdef FEATURE_Q6_MT
#if 0
    if (sigs & RLC_DL_DATA_REASSEMBLY_SIG)
    {
      TDSL2_MSG0(MSG_LEGACY_MED,"L2 DL Task: Processing Reassembly SIGNAL");
      /* Clear downlink MAC HFN Request Signal */
      (void)rex_clr_sigs (rex_self(), RLC_DL_DATA_REASSEMBLY_SIG);

      /* Process RLC Data Ind */
      rlc_enh_dl_data_reassembly_signal();
    }
#endif


    if (sigs & TDSMAC_PHY_DL_CONFIG_IND)
    {
      /* Clear DL config signal received from L1 */
      (void)rex_clr_sigs (rex_self(), TDSMAC_PHY_DL_CONFIG_IND );

      /* Process the signal to configure DL MAC */
      tdsproc_dl_mac_config_signal();
    }

    if (sigs & TDSMAC_PHY_DL_CONFIG_AT_L1_FAILURE_IND)
    {
      /* Clear DL config failed signal received from L1 */
      (void)rex_clr_sigs (rex_self(), TDSMAC_PHY_DL_CONFIG_AT_L1_FAILURE_IND );

      /* Process the signal to configure DL MAC */
      tdsproc_dl_mac_config_at_l1_failure_signal();
    }

    if (sigs & TDSMAC_PHY_HS_CONFIG_IND)
    {
      /* Clear MAC HS config signal received from L1 */
      (void)rex_clr_sigs (rex_self(), TDSMAC_PHY_HS_CONFIG_IND );

      /* Process the signal to configure DL MAC */
      tdsproc_dl_mac_hs_config_signal();

    }



    if (sigs & TDSRLC_DL_AM_BASE_TIMER_SIG)
    {
      /* Clear MAC HS config signal received from L1 */
      (void)rex_clr_sigs (rex_self(), TDSRLC_DL_AM_BASE_TIMER_SIG );

      /* Process the signal to configure DL MAC */
      tdsrlci_dl_am_base_timer_handler();

    }

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
       #error code not present
#endif/*TEST_FRAMEWORK*/     

  } /* while loop */

#ifdef FEATURE_DOG
  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);
#endif
}/* l2_dl_task */

/*==============================================================================================

FUNCTION  tdsl2_dl_get_cmd_buf

DESCRIPTION
  This function should be called to get a command buffer from downlink Layer 2
  task command free queue

DEPENDENCIES
  None.

RETURN VALUE

  Pointer to the downlink L2 command buffer if one is available
  NULL  otherwise

SIDE EFFECTS

=============================================================================================*/
tdsl2_dl_cmd_type    *tdsl2_dl_get_cmd_buf
(
  void
)
{
  void * return_ptr =NULL;
  size_t cmd_size = sizeof(tdsl2_dl_cmd_type);

  return_ptr = modem_mem_calloc(1,cmd_size,
          MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);

  if(return_ptr == NULL)
  {
    ERR_FATAL("Failed to allocate from Modem Mem Heap: size:0x%x",cmd_size,0,0);
  }

  return (tdsl2_dl_cmd_type *)return_ptr;

} /* tdsl2_dl_get_cmd_buf */




/*=============================================================================================

FUNCTION  tdsl2_dl_put_cmd

DESCRIPTION
  This function should be called to send a command to the downlink Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for downlink Layer 2 task and signal will be set to
  indicate that the item has been enqueued.

=============================================================================================*/

void  tdsl2_dl_put_cmd
(

  tdsl2_dl_cmd_type    *cmd_ptr
    /* pointer to command to be enqueued */
)
{
  tds_parse_cfa(TDSCFA_L2DL_Q,(void *)cmd_ptr);
  /* initialize link field */
  (void)q_link(cmd_ptr, &cmd_ptr->cmd_hdr.link );

  /* put item on queue for tds_l2_dl_task */
  q_put( &tdsl2_dl_cmd_q, &cmd_ptr->cmd_hdr.link );

  /* Set signal for the tdsl2_dl_cmd_q */
  (void)rex_set_sigs( tds_l2_dl_get_tcb(), TDSL2_DL_CMD_Q_SIG );

} /* tdsl2_dl_put_cmd */

#ifdef FEATURE_DUAL_WCDMA
/*===========================================================================

FUNCTION:        tdsl2_dl_service_register_sub

DESCRIPTION:
  This function should be called to send a L2 command to L2-UL task 
  for water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in L2-DL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
boolean tdsl2_dl_service_register_sub
(
  sys_modem_as_id_e_type as_id,
  tdsl2_dl_service_register_type *srvc_ptr
)
{
  TDSL2_MSG1(MSG_LEGACY_HIGH,"received as_id: %d",as_id);
  return tdsl2_dl_service_register(srvc_ptr);
}

#endif

/*===========================================================================

FUNCTION:        tdsl2_dl_service_register

DESCRIPTION:
  This function should be called to send a L2 command to L2-UL task 
  for water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in L2-DL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
boolean tdsl2_dl_service_register
(
  tdsl2_dl_service_register_type *srvc_ptr
)
{
  uint16 i;
  tdsl2_dl_cmd_type *dl_srvc_cmd_ptr;  

  /* Validate # LC number */
  if (srvc_ptr->nchan > TDSL2_MAX_DL_LC)
  {
    ERR_FATAL("number of channels:%d trying to register l2 dl over limite:%d",srvc_ptr->nchan,TDSL2_MAX_DL_LC,0);    
  }

  dl_srvc_cmd_ptr = tdsl2_dl_get_cmd_buf(); 
  if (dl_srvc_cmd_ptr == NULL)
  {
    TDSL2_MSG0(MSG_LEGACY_ERROR,"Couldn't get L2 downlink cmd buffer");
    return FALSE;
  }

  if (srvc_ptr->service == TRUE)
  {
    dl_srvc_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_DL_REGISTER_SRVC_REQ;
    dl_srvc_cmd_ptr->cmd_data.dl_reg.nchan = srvc_ptr->nchan;

    for (i = 0; i < srvc_ptr->nchan; i++)
    {
      dl_srvc_cmd_ptr->cmd_data.dl_reg.rlc_data[i].lc_id = srvc_ptr->rlc_id[i];
      dl_srvc_cmd_ptr->cmd_data.dl_reg.rlc_data[i].dl_wm_ptr = srvc_ptr->dl_wm_ptr[i];
      dl_srvc_cmd_ptr->cmd_data.dl_reg.rlc_data[i].context = srvc_ptr->context[i];
      dl_srvc_cmd_ptr->cmd_data.dl_reg.rlc_data[i].rlc_post_rx_proc_func_ptr = srvc_ptr->rlc_post_rx_proc_func_ptr[i];
      dl_srvc_cmd_ptr->cmd_data.dl_reg.rlc_data[i].rlc_post_rx_func_ptr_para = srvc_ptr->rlc_post_rx_func_ptr_para[i];
    }
  }
  else 
  {
    dl_srvc_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_DL_DEREGISTER_SRVC_REQ;
    dl_srvc_cmd_ptr->cmd_data.dl_dereg.nchan = srvc_ptr->nchan;

    for (i = 0; i < srvc_ptr->nchan; i++)
    {
      dl_srvc_cmd_ptr->cmd_data.dl_dereg.rlc_id[i] = srvc_ptr->rlc_id[i];
    }        
  }
  

  tdsl2_dl_put_cmd(dl_srvc_cmd_ptr);

  return TRUE;
}

/*===========================================================================

FUNCTION:        tdsl2_dl_submit_datamodem_msg

DESCRIPTION:
  This function should be called by the DATA MODEM to send a command
  to the L2 DL task
  
The pointer for cmd_ptr passed  should be freed by the caller.

DEPENDENCIES:
  None.

RETURN VALUE:
  boolean TRUCE if succes else FALSE.

SIDE EFFECTS:
  The pointer for cmd_ptr passed  should be freed by the caller.

  Command will be enqueued in L2-DL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/

boolean tdsl2_dl_submit_datamodem_msg
(
  tdsl2_dl_datamodem_api_type * api_ptr
)
{
#ifdef FEATURE_TDSCDMA
  tdsl2_dl_cmd_type * l2_cmd_ptr;
  switch(api_ptr->api_id)
  {
    case  TDSRLC_DL_MODEMDATA_REG_DEREG_SRVC_REQ: /* Register Deregister Uplink Watermark */
      if(tdsl2_dl_service_register(&api_ptr->api_data.dl_reg_dereg_cmd) == FALSE)
      {
        ERR_FATAL("Unable to allocate memory for Reg Dereg command in L2 Dl",0,0,0);
      }
    break;
    case TDSRLC_DL_MODEMDATA_PDCP_CLEANUP_WM:            /* Open the Receiving Window. */
    {
      l2_cmd_ptr = tdsl2_dl_get_cmd_buf();
      if(l2_cmd_ptr == NULL)
      {
        TDSL2_MSG0(MSG_LEGACY_ERROR,"Couldn't get L2 uplink cmd buffer");
        return FALSE;
      }
      l2_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_PDCP_CLEANUP_WM;
      l2_cmd_ptr->cmd_data.cleanup_wm_cmd.cleanup_wm_para
         = api_ptr->api_data.cleanup_wm_cmd.cleanup_wm_para;
      l2_cmd_ptr->cmd_data.cleanup_wm_cmd.lc_id
         = api_ptr->api_data.cleanup_wm_cmd.lc_id;
      l2_cmd_ptr->cmd_data.cleanup_wm_cmd.cleanup_wm_cb
         = api_ptr->api_data.cleanup_wm_cmd.cleanup_wm_cb;
      tdsl2_dl_put_cmd(l2_cmd_ptr);
    }
    break;

    default:
      TDSL2_MSG1(MSG_LEGACY_ERROR,"Unexpected command %d sent to l2 Dl by data modem",api_ptr->api_id);
  }
  return TRUE;
#else
  MSG_ERROR("TDS doesn't exist", 0,0,0);
  return FALSE;
#endif
}
#ifdef FEATURE_DUAL_WCDMA
/*===========================================================================

FUNCTION:        tdsl2_dl_submit_datamodem_msg_sub

DESCRIPTION:
  This function should be called by the DATA MODEM to send a command
  to the L2 UL task
  The pointer for cmd_ptr passed  should be freed by the caller.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  boolean TRUE if succes else FALSE.

SIDE EFFECTS:
  The pointer for cmd_ptr passed  should be freed by the caller.
  
  Command will be enqueued in L2-UL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/


boolean tdsl2_dl_submit_datamodem_msg_sub
(
  sys_modem_as_id_e_type as_id,
  tdsl2_dl_datamodem_api_type * cmd_ptr
)
{
  return tdsl2_dl_submit_datamodem_msg(cmd_ptr);
}

#endif

