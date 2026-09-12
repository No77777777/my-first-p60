/*===========================================================================
                        R R C    T A S K  

DESCRIPTION

   This file contains the task entry point for RRC task. It implements the 
   RRC task initialization, including initialization of various procedures.
   and setting of the RRC state. The task entry point function also implements
   the task startup protocol with the Task & Mode Controller. The RRC Dispatcher
   is called once startup is done.



EXTERNALIZED FUNCTIONS   
   tdsrrc_task()
        It is the main entry point for RRC Task.
   rrc_set_state()
        Sets the state of RRC.
   tdsrrc_get_state()
        Gets the current RRC state.
      
INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctask.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/02/14  sarao   Removed the inclusion of tmc.h as its being discontinued in core.
08/20/12   hx      Upmerge to M9615ACEHWTAAM4100184.1, mainline FEATURE_INTERRAT_HANDOVER_WTOG
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msg.h"
#include "err.h"
#include "rex.h"
#include "tdsrrctask.h"
#include "tdsrrcdispatcher.h"
#include "tdsrrcossinit.h"
#include "tdsrrctmr.h"
#include "tdsdiag.h"

/* Include header files from each procedure */
#include "tdsrrccsp.h"
#include "tdsrrcumi.h"
#include "tdsrrcsmc.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmdi.h"
#include "tdsdiag.h"
#include "tdsrrcllc.h"
#include "tdsrrclcm.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcccm.h"
#include "tdsrrcrbe.h"
#include "tdsrrcrbr.h"
#include "tdsrrcrce.h"
#include "tdsrrcrcr.h"
#include "tdsrrcscmgr.h"
#include "tdsrrctmr.h"
#include "tdsrrcdt.h"
#include "tdsrrcpg1.h"
#include "tdsrrcuece.h"
#include "tdsrrcueci.h"
#include "tdsrrcmeas.h"
#include "tdsrrclbt.h"
#include "tdsrrccu.h"
#include "tdsrrccspdb.h"
#include "tdsrrctcreconfig.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrcpcreconfig.h"
#include "tdsrrcmisc.h"
#include "tdsrrccho.h"
#include "tdsrrcmcm.h"
#include "tdsrrcchk.h"
#include "tdsrrcnv.h"
#include "tdsrrcossinit.h"
#include "tdsrrciho.h"
#include "tdsCFAParser.h"
#include "tdsCFAParser_i.h"


/* Include the Task Contoller file */
/*#ifndef FEATURE_MODEM_RCINIT 
#include "tmc.h"
#endif */

/* Include the RRC Log file */
#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */


#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
#include "tdsrrcmeasi.h"
#endif

#include "tdsrrctfcc.h"


#include "tdsrrcdormancy.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /*FEATURE_SEGMENT_LOADING*/

#include "tdsrrcqsh.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*extern rex_crit_sect_type rrc_ul_tx_pwr_mutex;*/  /*Delete in tdsrrcllcoc.c, because it is a FDD item*/
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
extern rex_crit_sect_type tdssib7_in_fach_active_mutex;
#endif
#endif

extern rex_crit_sect_type tdsrrc_wrm_lock_flag_mutex;

#ifdef FEATURE_MODEM_RCINIT
extern rex_crit_sect_type tdsrrccfa_add_entry_cs;
extern rex_crit_sect_type tdsrrccfa_dsds_add_entry_cs;
#endif

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_SEGMENT_LOADING
extern void tds_task_null (void);
#endif /*FEATURE_SEGMENT_LOADING*/

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION RRC_INIT_PROCEDURES

DESCRIPTION

  This function calls the init functions for all procedures so as to put them
  in a defined state. The init function for a procedure may initialize its
  static data.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

NOTE
  The Initialization functions for procedures are NOT allowed to send any 
  Signal or Command to RRC or any other task.

===========================================================================*/
static void tdsrrctask_init_procedures( void )
{
   /* Creates RRC.LOG file if FEATURE_RRC_MSG_LOG is enabled */
#ifdef T_WINNT
  #error code not present
#endif /* T_WINNT */

  /* Call Initialization functions for each procedures */
  /* Call LLC specific initialization function */
  tdsrrcllc_init(TDSRRCLLC_STARTUP_INIT);
  tdsrrclcm_create_ul_dl_rlc_lc_q();
  /* Call initialization functions for RLC Logical Channel management */
  tdsrrclcm_init_ul_rlc_lc_info();
  tdsrrclcm_init_dl_rlc_lc_info();
  tdsrrclcm_init_l2_ack_info();
 
  /*Initialize ue connected mode timers and constants to default values*/
  tdsrrc_initialize_ue_conn_timers_and_constants();
 
  /* Initialize the SIB procedure */
  tdsrrcsib_proc_init();

  /* Initialize RRC Connection Establishment procedure */
  tdsrrcrce_init_procedure();

  /* Initialize Cell Selection Procedure */
  tdsrrccsp_init_cell_selection_procedure();

  /* Initialize Cell Change Manager */
  tdsrrcccm_init_cell_change_manager();

  /* Initialize RRC Connection Release procedure */
  tdsrrcrcr_init_procedure();

 /* Initialize Paging Type 1 procedure */
  tdsrrcpg1_init_procedure();

 /* Initialize UMI procedure */
  tdsrrcumi_init_procedure();

 /* Initialize SMC procedure */
  tdsrrcsmc_init_procedure();

  /* Initializes Direct transfer procedures */
  tdsrrcdt_init_procedure();

  /* Initialize the Radio Bearer Setup Procedure */
  tdsrrcrbe_init_procedure();

  /* Initialize the Radio Bearer Release Procedure */
  tdsrrcrbr_init_procedure();

  /* Initialize the UE Capability Enquiry Procedure */
  tdsrrcuece_init_procedure();

  /* Initialize the UE Capability Information Procedure */
  tdsrrcueci_init_procedure();

  /* Initialize RRC Measurement procedure */
  tdsrrcmeas_init_procedure();

  /* Initialize the Loopback Test Module */
  tdsrrclbt_init_procedure();

  /* Initialize the Cell Update procedure */
  tdsrrccu_init_procedure();

  /* Initialize RB Reconfig Procedure */
  tdsrrcrbrc_init_procedure();

  /* Initialize Transport Channel Reconfig Procedure */
  tdsrrctcrc_init_procedure();

  /* Initrialize Physical Channel Reconfig Procedure */
  tdsrrcpcrc_init_procedure();

  /* Initialize MISC procedure */
  tdsrrcmisc_init_procedure();

  /* Initialize CHO procedure */
  tdsrrccho_init_procedure();

   /* Initialize MCM procedure */
  tdsrrcmcm_init_procedure();

  tdsrrcdata_init_procedure();

  /* Registers RRC subsys_id with the Diag Dispatch Table */
  tdsdiag_init();

  /* Initialize IHO Module */
  tdsrrciho_init_procedure();

#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
  tdsrrcllc_init_default_configuration();
#endif

  /* Initialize Counter Check Procedure */
  tdsrrcchk_init_procedure();
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  tdsrrcgps_init_procedure();
#endif/*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

  tdsrrctfcc_init_procedure();


#ifdef FEATURE_TDSCDMA_REL7_SPECRS
  tdsrrccouec_init_procedure();
#endif

#if defined(FEATURE_3GPP_FAST_DORMANCY)
  tdsrrcdormancy_init();
#endif

}


/*===========================================================================

FUNCTION TDSRRC_INIT

DESCRIPTION
  This function does the initialization for RRC layer. It means setting the 
  state and mode of RRC and initializing all RRC global data. It calls 
  tdsrrctask_init_procedures() to initialize each procedure specific data.

  NOTE: We may add NV read here later. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_init( void )
{
  /* Init NV */
  tdsrrcnv_init();

  /* Initialize RRC Command Queues */
  tdsrrc_init_command_queues();
  /* Create RRC timers */
  tdsrrctmr_create_timers();
  /* Initialize OSS ASN1 subsystem */
  //(void)tdsrrc_oss_init();
  (void)tdsrrcoss_obj_sys_init();
  /* Set the default state of RRC's state machine */
  tdsrrc_init_state();
  /* Initialize the UE variable "ESTABLISHED_RABS" */
  tdsrrc_initialize_established_rabs();

  /* Now initialize all RRC procedures */
  tdsrrctask_init_procedures();

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifndef FEATURE_MODEM_RCINIT
  /*-------------------------------------------------------------------------
    Initialize UL RRC mutex here
  -------------------------------------------------------------------------*/

  /*rex_init_crit_sect(&rrc_ul_tx_pwr_mutex);*/ /*Delete in tdsrrcllcoc.c, because it is a FDD item*/
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
  rex_init_crit_sect(&tdssib7_in_fach_active_mutex);
#endif
#endif /*FEATURE_MODEM_RCINIT*/
#endif /*FEATURE_TDSCDMA_FREE_FLOATING_TASK*/

#ifndef FEATURE_MODEM_RCINIT
  rex_init_crit_sect(&tdsrrc_wrm_lock_flag_mutex);
#endif /*FEATURE_MODEM_RCINIT*/
}

#ifdef FEATURE_MODEM_RCINIT
/*===========================================================================

FUNCTION TDSRRC_TASK_INIT

DESCRIPTION
  This function does the initialization for mutex of RRC layer under
  RC_INIT control
  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void tds_rrc_task_init (void) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void tds_rrc_task_init(void)
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;

  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
  rex_init_crit_sect(&tdssib7_in_fach_active_mutex);
#endif
#endif

  rex_init_crit_sect(&tdsrrc_wrm_lock_flag_mutex);

  rex_init_crit_sect(&tdsrrccfa_add_entry_cs);
  rex_init_crit_sect(&tdsrrccfa_dsds_add_entry_cs);
}

#endif /*FEATURE_MODEM_RCINIT*/

/*===========================================================================

FUNCTION RRC_TASK

DESCRIPTION
  This function is the entry point for RRC Task. It initializes RRC state
  and individual procedures by calling tdsrrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, tdsrrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/

#ifdef FEATURE_SEGMENT_LOADING
void tds_rrc_task(dword param) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void  tds_rrc_task(dword param)
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;
  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    tds_task_null();
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

  /* Initialize the RRC task and all RRC procedures */
  tdsrrc_init();

  /* Read CMCC Conformance test flag. 
   * This is equivalent to NV00947: NV_GPRS_ANITE_GCF_I for WCDMA
   */
  tdsrrcnv_read_nv_special_settings_for_testing();


#ifdef FEATURE_MODEM_RCINIT 
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();

#else
  /* Send an ACK and wait for Task Start signal */
  tmc_task_start();
#endif

  tds_rrc_init_tcb();

  /* Read the acquisition database from NV here */
  tdsrrccsp_read_nv_acq_info();
  /* Read the cs domain bar timer length from NV*/
  tdsrrccsp_read_nv_cs_domain_bar_timer_length();
  /* Read NV items Fake Security, Integrity Enabled and
     Ciphering Enabled from NV */
  tdsrrcsmc_get_nv_items();
  tdsrrcdt_get_nv_items();

  /* read tds rrc version from NV */
  tdsrrc_get_tds_rrc_version_nv();

  tdsrrcueci_get_nv_items();
  tdsrrcllc_get_cm_hsdpa_nv_item();

#ifdef FEATURE_TD2G_NACC
  tdsrrcueci_get_td2g_nacc_nv_item();
#endif

#ifdef FEATURE_UMTS_PDCP
  tdsrrc_get_tds_rrc_pdcp_disabled_nv();
#endif /* FEATURE_UMTS_PDCP */


  tdsrrcnv_read_nv_items();
  tdsrrcnv_read_nv_item_default();

  tdsrrccfa_init();

#ifdef FEATURE_QSH_EVENT_METRIC
  tdsrrcqsh_init();
#endif
  
  tdsrrc_register_with_msgr_for_client_and_register_umids();

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif

/* Initialize the SIB-Proc supported database */
  /* Sib11Bis initialize is based on NV check so need to call this function after NV read */
  tdsrrcsib_initialize_supported_sibs_lookup_table();

  /* Set Band F search patterns according to NV */
  tdsrrccsp_set_band_f_search_pattern();
  /* Set Band F search patterns according to NV */
  tdsrrccsp_set_band_a_search_pattern();
  /* Set Band F search patterns according to NV */
  tdsrrccsp_set_band_e_search_pattern();
#ifdef TEST_FRAMEWORK
  #error code not present
#else
  /* Notify TDS RRC task is ready */
  (void)rcevt_signal_name("TDSRRC:ready");
#endif

#ifdef FEATURE_MODEM_CONFIG_REFRESH 
  /* register for mcfg refresh notification */
  tdsrrc_register_mcfg_refresh_notify();
#endif

  /* Call RRC Dispatcher so that RRC can enter normal processing now onwards */
  tdsrrc_dispatcher();


}
/*lint +e715*/



