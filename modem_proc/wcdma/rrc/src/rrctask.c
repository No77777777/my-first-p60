/*===========================================================================
                        R R C    T A S K  

DESCRIPTION

   This file contains the task entry point for RRC task. It implements the 
   RRC task initialization, including initialization of various procedures.
   and setting of the RRC state. The task entry point function also implements
   the task startup protocol with the Task & Mode Controller. The RRC Dispatcher
   is called once startup is done.



EXTERNALIZED FUNCTIONS   
   rrc_task()
        It is the main entry point for RRC Task.
   rrc_set_state()
        Sets the state of RRC.
   rrc_get_state()
        Gets the current RRC state.
      
INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrctask.c_v   1.39   24 Jun 2002 18:03:52   xgao  $   
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrctask.c#4 $    $DateTime: 2020/07/14 01:35:01 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/14/20   ak      Made changes for FR 61612
01/14/20   sp      Mini dump phase-2 changes
11/20/18   ak      Made changes for FR 53313
05/14/18   sp      Made changes to temporarily disable DC
09/09/16   sp      Made changes to update CU cause to QSH
06/10/16   sp      Made changes to save metric config start address into a new db for dual sim cases.
05/04/16   nr      Added support for QSH handler framework
10/26/15   sp      Made changes to add call end metrics - FR#30856
09/02/15   sp      Made changes to add resel failure, rrc conn rel events
08/26/15   gv      Made changes to process the alternate/free floating sub command queue till 
                   it reaches NULL before exiting the loop
07/20/15   sr      Made changes for FR29345 EFS/NV Settings to be Read per Sub
07/08/15   sas     Funtion rrc_main() is placed under .task_text section.
06/30/15   sp      Fixed QSH indentation errors
06/26/15   sp      Fixed compilation errors with QSH feature enabled
06/15/15   sp      Made changes for ULOG framework
06/10/15   sg      Made changes to update review comments for QSH changes
06/03/15   pr      W+W Phase8 Checkin.
02/19/14   sp      Made changes for Mav debug snapshot
03/24/15   vi      Made changes to pass uint32 variable to Diag api.
03/23/15   rsr     W + W Phase 6 Checkin.
07/17/14   sr      Changes for NV refresh FR
05/07/14   vi      Made changes to use mutex under critical section to avoid race condition in FF cmds.
03/13/14   sp      Removed code under FEATURE_WCDMA_HS_PCH feature flag
01/09/14   sp      Removed code under FEATURE_CS_VOICE_OVER_HSPA feature
11/26/13   geg     Segment loading FSL: add dummy task 
03/22/13   mp      Removed tmc.h include file
10/09/12   gv      Made changes to post RRC task ready status
03/08/12   amj     Made changes for RC init task initialization support.
12/12/11   gv      Added code to support free floating of tasks
03/28/11   rl      Merged the HS-FACH/PCH changes from mainline
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/07/11   su      Added code to support device type NV.
02/15/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.91.50
01/31/11   ad      Fixed compilation warnings
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
01/04/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.87.50
12/07/10   kp      Added code to init MVS for feature FEATURE_WCDMA_SIMULATE_AMR_PACKETS
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
10/11/10   ss      Upgraded SIB11Bis code to ASN 8.a and added code to init sibs lookup table 
                   after initialization of rrc_nv_rel_indicator.
10/04/10   prk     Added code to support FEATURE_WCDMA_HS_PCH.
09/20/10   prk/kp  Added code to support FEATURE_WCDMA_HS_FACH
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
06/29/10   rm      Added code to support LTE<->WCDMA Inter RAT
04/26/10   ss      Added code changes for Fast Dormancy feature under feature flag
                   FEATURE_3GPP_FAST_DORMANCY
02/15/10   as      Deleted QTF TEST_FRAMEWORK stub code
02/10/10   as      Added feature falg WCDMA_UNIT_TEST for rrc stub code
02/05/10   rmsd    To Enable Inter-RAT testing, added WCDMA_UNIT_TEST along with TEST_FRAMEWORK
12/22/09   gkg     Made changes for CSoHS so that NV 6878 controls CSoHS operation.
12/14/09   as      Fixed lint issues
12/07/09   ss      Changes to add Stub code under feature flag TEST_FRAMEWORK
11/05/09   rm      Added code to read WTOG NACC support from NV using
                   rrcueci_get_wtog_nacc_nv_item
07/14/09   dm      Fixed lint errors
05/14/09   sks     Made changes to suppress compilation errors for RRC UTF.
05/08/09   ss      Updated Copyright Information
04/29/09   ss      Made changes to compiler warnings on gcc compilers.
01/19/08   ps      Made changes for ASn1 migration 25.331v7.9  
                   i.e.May 2008 version  (2008-05) 
01/05/09   ps      Made changes to call dsm_queue_init for watermark queues
                   only at RRC initialisation stage
03/06/08   da      Changed FEATURE_MBMS to FEATURE_MODEM_MBMS
12/21/07   da      Support for FEATURE_MBMS
11/14/07   rm      Made changes to init TFCC procedure by calling rrctfcc_init_procedure
08/31/07   ps      rrcgps.h file is removed and code is move to rrcmeasi.h. So 
                   adding rrcmeasi.h in header instead of rrcgps.h
08/08/07   ps      Added support for feature cell id reading in DCH state, 
                   under feature flag FEATURE_WCDMA_GPS_CELL_ID_READING.
07/12/07   bd      Moved wcdmadiag_init() from diagpkt.c
03/09/07   vm      Added support for FEATURE_RRC_HEAP.
02/02/06   vm      Added fix for CR 108919. This is to meet NAS requirement 
                   that RRC_REL_IND should always be received before RRC_SERVICE_IND
                   (with NO SERVICE). To achieve this, the order in which RCE
                   and CSP procedures register with State Change Manager (SCM) has been
                   swapped. Now RCE proc registers with SCM before CSP so that state
                   change indication is rcvd by RCE before CSP, which helps us achieve
                   the above requirement.
08/29/06   sm/da   Made changes required for R6 ASN.1 migration.  Also merged
                   FEATURE_WCDMA_REL6 and FEATURE WCDMA HSUPA.
04/17/06   vk      While initializing RRC Task, invoked rrcllc_get_cm_hsdpa_nv_item()
                   to read the HSDPA/CM NV Item under FEATURE HSDPA
03/10/06   da      Added Initial support for PDCP under FEATURE_UMTS_PDCP
02/22/06   vk      Added rrcueci_get_nv_items() in function rrc_init_procedures()
                   under FEATURE HSDPA
11/10/05   sm      Added support to read rrc-rel-indicator using 
                   rrc_get_wcdma_rrc_version_nv()
10/17/05   vr      Added rrcchk_init_procedure() in rrc_init_procedures()
04/06/05   da      Added rrcbmc_init_procedure() in rrc_init_procedures()
04/05/05   da      Added rrcrce_get_nv_items() in function rrc_task()
11/22/04   vr      Initialize MCM procedure 
09/16/04   vm      Included rrcdiag.h file and added call to rrcdiag_init() function 
                   in rrc_init_procedures().
08/30/04   sgk     Removed declaration for 'rrc_csp_read_nv_acq_info' to fix 
                   lint error Redundantly declared symbol 
                   'rrc_csp_read_nv_acq_info(void)' previously declared in 
                   file rrccspdb.h.
03/15/04   bu      Removed the use of FEATURE DUALMODE BASELINE. This feature is
                   always defined (even for singlemode builds).
07/06/03   rj      Added init function for CHO procedure.
05/14/03   ram     rrctmr_create_timers was being called within rrc_init_procedures
                   such that timers were not initialized before some procedures were
                   initialized. Changed it so that timers are initialized first before
                   any procedures are initialized.
02/04/03   ram     Checked in lint changes, updated copyright.
10/11/02   xfg     Added inilization of IHO module
09/10/02   bu      Added intialization of MISC procedure.
08/02/02   kc      Added calls to read NV items Integrity, Ciphering, Fake
                   Security Enabled/Disabled
06/24/02   xfg     Removed starting RSSI report timer from rrc_task for 
                   supporting dual-mode
05/09/02   vn      Added calls for initialization of RB, Transport and Physical
                   Channel Reconfig procedures.
03/12/02   kc      Added Init function for SMC procedure.
02/28/02   rj      Added Init function for Cell Update procedure.
02/20/02   kc      Added initialization of UMI procedure and also initialization
                   of ue_conn_timers_and_constants
02/14/02   kc      Fixed compilation errors
02/14/02   kc      Added a function call to rrcpg2_init_procedure().
02/12/02   bu      Added a function call to rrc_read_nv_acq_info().
01/30/02   xfg     Added a function call to LBT initialization procedure
09/07/01   xfg     Added initialization procedure for ASU:rrcasu_init_procedure
                   and added a call to start RSSI reporting timer in rrc_task
09/04/01   vk      Added initialisation procedure for Measurements
                   (rrcmeas_init_procedure)
08/06/01   rj      Renamed FEATURE_RRC_DISK_LOGGING to T_WINNT. Added Init
                   functions for UE Capability Enquiry and UE Capability
                   Information procedures.
06/15/01   vn      Updated to pass initialization type in rrcllc_init.
05/30/01   ram     Added function call to initialize the Radio Bearer Release
                   Procedure.
05/17/01   rj      Updated FEATURE_RRC_MSG_LOG to FEATURE_RRC_DISK_LOGGING.
04/29/01   rj      Added function call to initialize the Paging Type procedure
                   to set IMSI, TMSI and PTMSI are invalid at startup.
04/26/01   ram     Added function calls to initialize the Radio Bearer Establishment
                   Procedure and the UE variable "ESTABLSIHED_RABS".
04/23/01   ram     Moved the functionality related to RRC state change to the rrcscmgr
                   module.
03/30/01   ram     Included customer.h header file.
03/14/01   rj      Added Init function for RRC Direct transfer procedures.
02/19/01   rj      Added Init function for RRC Connection Release procedure.
01/25/01   rj      Added Init function for RRC Connection Establishment procedure.
                   Added a function to create RRC Timers at RRC Startup.
01/23/01   kmp     Added new LLC init function in rrc_init_procedures().
01/18/01   vn      Added a call to initialize Cell Change Manager.
01/09/01   ram     Removed test code that was incorrectly included in previous
                   checkin.
01/09/01   ram     Modified rrc_set_state function to send a command RRC_NEW_STATE_IND
                   to RRC's state change manager when the state changes.
                   The function also verifies that the new state is a valid
                   RRC state. Also, rrc_set_state should not be used at power-up
                   so created another function rrc_init_state to be called at
                   power-up to set the default state of RRC.
12/19/00   vn      Added a call to initialize OSS ASN1 subsystem.
12/08/00   ram     Added a function call to initialize SIB procedure.
12/08/00   ram     Added a function call to initialize RRC's command queues.
                   This is called before initializing RRC procedures since
                   the procedures may queue commands to other procedures as
                   part of their initialization.
12/07/00   rj      Added code to create RRC.LOG file at RRC task startup. 
                   Feature FEATURE_RRC_MSG_LOG needs to enabled for logging.
11/30/00   rj      The function names rrclcp_init_ul_rlc_lc_info, 
                   rrclcp_init_dl_rlc_lc_info, and rrclcp_init_l2_ack_info are
                   changed to rrclcm_init_ul_rlc_lc_info, 
                   rrclcm_init_dl_rlc_lc_info, and rrclcm_init_l2_ack_info
11/29/00   vn      Changes to transition from zREX to REX++.
11/15/00   vn      Incorporated code review comments.
11/09/00   kmp     Added support for RLC and other events that need to be
                   sourced.
11/07/00   kmp     Updated to include the MAC,RLC and TC header files. Also 
                   updated to use the correct SIB processing Event name.
10/31/00   vn      Included init functions for LLC, LCP and CSP.
10/27/00   vn      Added Initialization of procedures and RRC State.
10/26/00   kmp     Added support for L1, RLC and MAC Event Pools. Added
                   support for CPHY_ACQ_REQ and CPHY_SETUP_REQ events from
                   L1. Added all of the MAC events.
10/23/00   vn      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "wcdma_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msg.h"
#include "err.h"
#include "rex.h"
#include "rrctask.h"
#include "rrcdispatcher.h"
#include "rrcossinit.h"
#include "rrctmr.h"

/* Include header files from each procedure */
#include "rrccsp.h"
#include "rrcumi.h"
#include "rrcsmc.h"
#include "rrccmd_v.h"
#include "rrccmdi.h"
#include "rrcdata_v.h"
#include "rrcdiag.h"
#include "rrcllc.h"
#include "rrclcm.h"
#include "rrcsibproc.h"
#include "rrcccm.h"
#include "rrcrbe.h"
#include "rrcrbr.h"
#include "rrcrce.h"
#include "rrcrcr.h"
#include "rrcscmgr.h"
#include "rrctmr.h"
#include "rrcdt.h"
#include "rrcpg1.h"
#include "rrcuece.h"
#include "rrcueci.h"
#include "rrcmeas.h"
#include "rrcasu.h"
#include "rrclbt.h"
#include "rrccu.h"
#include "rrccspdb.h"
#include "rrctcreconfig.h"
#include "rrcrbreconfig.h"
#include "rrcpcreconfig.h"
#include "rrcmisc.h"
#include "rrccho.h"
#include "rrcmcm.h"
#include "rrcchk.h"
#include "rrcnv.h"
#include "rrciho.h"
#include "rrculog.h"



/* Include the RRC Log file */
#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */

#include "rrcbmc.h"

#include "rrcmeasi.h"

#include "rrctfcc.h"

extern void wcdmadiag_init (void);

#include "rrcdormancy.h"
#ifdef FEATURE_WCDMA_HS_FACH
#include "rrcenhstatecfg.h"
#include "rrcenhstateproc.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "rcinit.h"

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /* FEATURE_SEGMENT_LOADING */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
#if defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_WCDMA_QSH_DUMP)
#ifdef FEATURE_DUAL_WCDMA 
  qsh_ext_metric_cfg_s  wrrc_qsh_metric_cfg_arr_in_use_sim[MAX_RRC_STACKS][WRRC_QSH_METRIC_MAX];
  #define wrrc_qsh_metric_cfg_arr wrrc_qsh_metric_cfg_arr_in_use_sim[WAS_ID_M]
#else
#ifdef FEATURE_DUAL_SIM
  qsh_ext_metric_cfg_s  wrrc_qsh_metric_cfg_arr_in_use_sim[WRRC_MAX_NUM_SUBS][WRRC_QSH_METRIC_MAX];
  #define wrrc_qsh_metric_cfg_arr wrrc_qsh_metric_cfg_arr_in_use_sim[WAS_ID_NON_DUAL_WCDMA]
#else
  qsh_ext_metric_cfg_s  wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_MAX];
#endif
#endif
#endif
/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_SEGMENT_LOADING
extern void wcdma_task_null (void);
#endif /* FEATURE_SEGMENT_LOADING */

#ifdef FEATURE_DUAL_WCDMA
uint32 rrc_global_diag_asid[MAX_RRC_STACKS] = {1, 2};
uint8 rrc_global_asid[MAX_RRC_STACKS] = {0, 1};
#endif
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#if defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_WCDMA_QSH_DUMP)

/*===========================================================================

FUNCTION rrc_init_wrrc_qsh_metric_cfg_arr

DESCRIPTION
Registers RRC cleint with QSH for Metric/event/dump collection

	NOTE: 
	
DEPENDENCIES
	None

RETURN VALUE
	None

SIDE EFFECTS
	None

===========================================================================*/

static void rrc_init_wrrc_qsh_metric_cfg_arr(sys_modem_as_id_e_type wrrc_as_id)
{

  qsh_ext_metric_cfg_s *metric_cfg_ptr;

  /* element count should be modified */

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RESEL_CEL_SEL]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RESEL_CEL_SEL;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_resel_cel_sel_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 100;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RLF_OOS]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RLF_OOS;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rlf_oos_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 100;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_HO]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_HO;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_ho_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 100;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_ASET]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_ASET;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_aset_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 100;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_SERVING]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_SERVING;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_serving_cell_s);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_tmr_and_const_s);
  metric_cfg_ptr->fifo.element_count_total = 3;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_OTA]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_OTA;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_ota_s);
  metric_cfg_ptr->fifo.element_count_total = 15;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_MM_TO_RRC_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_MM_TO_RRC_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_mm_to_rrc_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 15;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RRC_TO_MM_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RRC_TO_MM_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rrc_to_mm_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 15;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RR_TO_RRC_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RR_TO_RRC_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rr_to_rrc_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RRC_TO_RR_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RRC_TO_RR_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rrc_to_rr_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RRC_TO_L1_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RRC_TO_L1_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rrc_to_l1_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 30;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_L1_TO_RRC_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_L1_TO_RRC_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_l1_to_rrc_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 0;

  	metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RRC_TO_LTE_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RRC_TO_LTE_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rrc_to_lte_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_LTE_TO_RRC_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_LTE_TO_RRC_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_lte_to_rrc_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_INTERNAL_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_INTERNAL_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_internal_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_LLC_CMD]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_LLC_CMD;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_llc_cmd_s);
  metric_cfg_ptr->fifo.element_count_total = 10;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_RRC_STATE]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_RRC_STATE;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_rrc_state_history);
  metric_cfg_ptr->fifo.element_count_total = 100;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_MEAS_ASU_OTA]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_MEAS_ASU_OTA;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_meas_asu_ota_s);
  metric_cfg_ptr->fifo.element_count_total = 15;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_CONN_END_INFO]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_CONN_END_INFO;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_call_end_info_s_type);
  metric_cfg_ptr->fifo.element_count_total = 2;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_MULTI_RAB_STATUS]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_MULTI_RAB_STATUS;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_multi_rab_status_s_type);
  metric_cfg_ptr->fifo.element_count_total = 2;
  metric_cfg_ptr->sampling_period_ms = 0;

  metric_cfg_ptr = &(wrrc_qsh_metric_cfg_arr[WRRC_QSH_METRIC_CELL_UPDATE_CAUSE]);

  metric_cfg_ptr->id = WRRC_QSH_METRIC_CELL_UPDATE_CAUSE;
  metric_cfg_ptr->subs_id = wrrc_as_id;
  metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrrc_qsh_metric_cell_update_cause_s_type);
  metric_cfg_ptr->fifo.element_count_total = 5;
  metric_cfg_ptr->sampling_period_ms = 0;


}

  /*===========================================================================

  FUNCTION RRC_QSH_INIT
  
  DESCRIPTION
  Registers RRC cleint with QSH for Metric/event/dump collection
  
    NOTE: 
    
  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
  
  ===========================================================================*/

static void 	rrc_qsh_init(sys_modem_as_id_e_type wrrc_as_id)
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_1;
  qsh_client_reg_s rrc_client_reg;
  uint8 max_subs = 1;

#ifdef FEATURE_DUAL_SIM
  max_subs = WRRC_MAX_NUM_SUBS;
#endif

  /* Registeration to both the tasks should be done only once. Make sure 2nd task is created before registering to QSH, 
  this is to avoid posting metric cfg cmd to sub 2 which is may not have been created yet */
#ifdef FEATURE_DUAL_WCDMA
  if(wrrc_as_id == SYS_MODEM_AS_ID_2)
#endif
  {
    /* Initialize WRRC client */
    qsh_client_reg_init(&rrc_client_reg);

    for(as_id = SYS_MODEM_AS_ID_1; as_id < max_subs; as_id++)
    {
#ifdef FEATURE_DUAL_SIM
      qsh_client_metric_cfg_init(&(wrrc_qsh_metric_cfg_arr_in_use_sim[as_id][0]), WRRC_QSH_METRIC_MAX);
#else
      qsh_client_metric_cfg_init(wrrc_qsh_metric_cfg_arr, WRRC_QSH_METRIC_MAX);
#endif
      rrc_init_wrrc_qsh_metric_cfg_arr(as_id);
    }
#ifdef FEATURE_DUAL_SIM
    rrc_client_reg.metric_info.metric_cfg_arr_ptr = &(wrrc_qsh_metric_cfg_arr_in_use_sim[0][0]);
#else
    rrc_client_reg.metric_info.metric_cfg_arr_ptr = &(wrrc_qsh_metric_cfg_arr[0]);
#endif

    rrc_client_reg.metric_info.metric_cfg_count = WRRC_QSH_METRIC_MAX * max_subs;
    rrc_client_reg.dump_info.max_size_bytes = wrrc_qsh_get_max_size_for_dump()*MAX_RRC_STACKS;

    /*Populate WRRC client stuff to QSH */
    rrc_client_reg.client = QSH_CLT_WRRC;
    rrc_client_reg.client_cb_ptr = wrrc_qsh_cb;
    rrc_client_reg.major_ver = WRRC_QSH_MAJOR_VER;
    rrc_client_reg.minor_ver = WRRC_QSH_MINOR_VER;
    rrc_client_reg.cb_action_support_mask = 0 
#ifdef FEATURE_WCDMA_QSH_DUMP
                                            | QSH_ACTION_DUMP_COLLECT
#endif
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

                                            | QSH_ACTION_METRIC_TIMER_EXPIRY | QSH_ACTION_METRIC_CFG
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                                            | QSH_ACTION_EVENT_CFG | QSH_ACTION_CFG | QSH_ACTION_EVENT_NOTIFY | QSH_ACTION_RESET
#endif
#ifdef FEATURE_QSH_MDUMP
                                            | QSH_ACTION_MDUMP 
#endif
                                            ;
    qsh_client_reg(&rrc_client_reg);

   }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   rrc_qsh_initialize_events(wrrc_as_id);
#endif
#ifdef FEATURE_QSH_MDUMP
  qsh_add_thread_id(QSH_CLT_WRRC);
#endif
}

#endif
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
static void rrc_init_procedures(sys_modem_as_id_e_type wrrc_as_id)
{
   /* Creates RRC.LOG file if FEATURE_RRC_MSG_LOG is enabled */
#ifdef T_WINNT
  #error code not present
#endif /* T_WINNT */

  /* Call Initialization functions for each procedures */
  /* Call LLC specific initialization function */
  rrcllc_init(wrrc_as_id, RRCLLC_STARTUP_INIT);
  rrclcm_create_ul_dl_rlc_lc_q(wrrc_as_id);
  /* Call initialization functions for RLC Logical Channel management */
  rrclcm_init_ul_rlc_lc_info(wrrc_as_id);
  rrclcm_init_dl_rlc_lc_info(wrrc_as_id);
  rrclcm_init_l2_ack_info(wrrc_as_id);
 
  /*Initialize ue connected mode timers and constants to default values*/
  rrc_initialize_ue_conn_timers_and_constants(wrrc_as_id);
 
  /* Initialize the SIB procedure */
  rrc_sib_proc_init(wrrc_as_id);

  /* Initialize RRC Connection Establishment procedure */
  rrcrce_init_procedure(wrrc_as_id);

  /* Initialize Cell Selection Procedure */
  rrc_init_cell_selection_procedure(wrrc_as_id);

  /* Initialize Cell Change Manager */
  rrc_init_cell_change_manager(wrrc_as_id);

  /* Initialize RRC Connection Release procedure */
  rrcrcr_init_procedure(wrrc_as_id);

 /* Initialize Paging Type 1 procedure */
  rrcpg1_init_procedure(wrrc_as_id);

 /* Initialize UMI procedure */
  rrcumi_init_procedure(wrrc_as_id);

 /* Initialize SMC procedure */
  rrcsmc_init_procedure(wrrc_as_id);

  /* Initializes Direct transfer procedures */
  rrcdt_init_procedure(wrrc_as_id);

  /* Initialize the Radio Bearer Setup Procedure */
  rrcrbe_init_procedure(wrrc_as_id);

  /* Initialize the Radio Bearer Release Procedure */
  rrcrbr_init_procedure(wrrc_as_id);

  /* Initialize the UE Capability Enquiry Procedure */
  rrcuece_init_procedure(wrrc_as_id);

  /* Initialize the UE Capability Information Procedure */
  rrcueci_init_procedure(wrrc_as_id);

  /* Initialize RRC Measurement procedure */
  rrcmeas_init_procedure(wrrc_as_id);

  /* Initialize the Active Set Update Procedure */
  rrcasu_init_procedure(wrrc_as_id);

  /* Initialize the Loopback Test Module */
  rrclbt_init_procedure(wrrc_as_id);

  /* Initialize the Cell Update procedure */
  rrccu_init_procedure(wrrc_as_id);

  /* Initialize RB Reconfig Procedure */
  rrcrbrc_init_procedure(wrrc_as_id);

  /* Initialize Transport Channel Reconfig Procedure */
  rrctcrc_init_procedure(wrrc_as_id);

  /* Initrialize Physical Channel Reconfig Procedure */
  rrcpcrc_init_procedure(wrrc_as_id);

  /* Initialize MISC procedure */
  rrcmisc_init_procedure(wrrc_as_id);

  /* Initialize CHO procedure */
  rrccho_init_procedure(wrrc_as_id);

   /* Initialize MCM procedure */
  rrcmcm_init_procedure(wrrc_as_id);

  rrcdata_init_procedure(wrrc_as_id);

  wcdmadiag_init();

  /* Registers RRC subsys_id with the Diag Dispatch Table */
  rrcdiag_init(wrrc_as_id);

  /* Initialize IHO Module */
  rrciho_init_procedure(wrrc_as_id);

  /* Initialize BMC Module */
  rrcbmc_init_procedure(wrrc_as_id);

  /* Initialize Counter Check Procedure */
  rrcchk_init_procedure(wrrc_as_id);
  #ifdef FEATURE_CGPS_UMTS_CP_WCDMA
  rrcgps_init_procedure(wrrc_as_id);
  #endif

  rrctfcc_init_procedure(wrrc_as_id);


#ifdef FEATURE_WCDMA_REL7_SPECRS
  rrccouec_init_procedure(wrrc_as_id);
#endif

  rrc_dormancy_init(wrrc_as_id);

  rrcrbcommon_init_procedure(wrrc_as_id);

  rrc_init_rrcllcoc_data(wrrc_as_id);

  rrc_init_rrcllcpcie_data(wrrc_as_id);

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  rrc_qsh_init(wrrc_as_id);
#endif
  wrrc_ulog_init(wrrc_as_id);

  l1_set_dc_capability(TRUE,wrrc_as_id);
  
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
  rrc_pseudo_meas_init_procedure(wrrc_as_id);
#endif

#ifdef ELEVATOR_TRIGGER
  rrc_pseudo_meas_rxagc_init_procedure(wrrc_as_id);
#endif

  rrc_init_cell_depr_info_procedure(wrrc_as_id);
}


/*===========================================================================

FUNCTION RRC_INIT

DESCRIPTION
  This function does the initialization for RRC layer. It means setting the 
  state and mode of RRC and initializing all RRC global data. It calls 
  rrc_init_procedures() to initialize each procedure specific data.

  NOTE: We may add NV read here later. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void rrc_init(sys_modem_as_id_e_type wrrc_as_id) __attribute__((section(".task_text")));
#endif /* FEATURE_SEGMENT_LOADING */
void rrc_init(sys_modem_as_id_e_type wrrc_as_id)
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_wcdma_segment = NULL;

  pi_wcdma_segment = get_wcdma_interface();
  if(pi_wcdma_segment == NULL)
  {
    return;
  }
#endif /* FEATURE_SEGMENT_LOADING */

  /* Initialize RRC Command Queues */
  rrc_init_command_queues(wrrc_as_id);
  /* Create RRC timers */
  rrctmr_create_timers(wrrc_as_id);
  /* Initialize OSS ASN1 subsystem */
  //(void)rrc_oss_init();
  (void)rrc_obj_sys_init(wrrc_as_id);
  /* Set the default state of RRC's state machine */
  rrc_init_state(wrrc_as_id);
  /* Initialize the UE variable "ESTABLISHED_RABS" */
  rrc_initialize_established_rabs(wrrc_as_id);
  /* Now initialize all RRC procedures */
  rrc_init_procedures(wrrc_as_id);
#ifdef FEATURE_WCDMA_SIMULATE_AMR_PACKETS  
    mvs_dsm_amr_init();
#endif

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  /*-------------------------------------------------------------------------
    Initialize UL RRC mutex here
  -------------------------------------------------------------------------*/
  rex_init_crit_sect(&rrcllcoc_int_data.rrc_ul_tx_pwr_mutex);
  rex_init_crit_sect(&rrcsibproc_ext_data.sib7_in_fach_active_mutex);
#endif
}


/*===========================================================================

FUNCTION RRC_MAIN

DESCRIPTION
  This function is the entry point for RRC MAIN. It initializes RRC state
  and individual procedures by calling rrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, rrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
#ifdef FEATURE_SEGMENT_LOADING
void rrc_main(sys_modem_as_id_e_type wrrc_as_id) __attribute__((section(".task_text")));
#endif /* FEATURE_SEGMENT_LOADING */
void  rrc_main(sys_modem_as_id_e_type wrrc_as_id)
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_wcdma_segment = NULL;

  pi_wcdma_segment = get_wcdma_interface();
  if(pi_wcdma_segment == NULL)
  {
    wcdma_task_null ();
    return;
  }
#endif /* FEATURE_SEGMENT_LOADING */

#ifdef FEATURE_MODEM_RCINIT 
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();
  /* Initialize the RRC task and all RRC procedures */
  rrc_init(wrrc_as_id);
#else
  /* Initialize the RRC task and all RRC procedures */
  rrc_init(wrrc_as_id);
  /* Send an ACK and wait for Task Start signal */
  tmc_task_start();
#endif

  /* Read the acquisition database from NV here */
  rrc_csp_read_nv_acq_info(wrrc_as_id); 

  rrc_read_all_nv_items(wrrc_as_id);

#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
  rrc_register_with_msgr_for_client_and_register_umids(wrrc_as_id);
#endif

  /* Initialize the SIB-Proc supported database */
  /* Sib11Bis initialize is based on NV check so need to call this function after NV read */
  rrc_sib_initialize_supported_sibs_lookup_table(wrrc_as_id);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  /* Fucntion to register for callback to get SIM refresh notification */
  rrc_nv_refresh_mcfg_regiser(wrrc_as_id);
#endif

  /* Set signal to indicate rrc task ready status */
  (void)rcevt_signal_name("rrc:ready");

  /* Call RRC Dispatcher so that RRC can enter normal processing now onwards */
  rrc_dispatcher(wrrc_as_id);
}

/*===========================================================================

FUNCTION RRC_TASK

DESCRIPTION
  This function is the entry point for RRC Task. It initializes RRC state
  and individual procedures by calling rrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, rrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
#ifdef FEATURE_SEGMENT_LOADING
void rrc_0_task( dword param) __attribute__((section(".task_text")));
#endif /* FEATURE_SEGMENT_LOADING */
void  rrc_0_task(dword param)
{
  rrc_main( SYS_MODEM_AS_ID_1);
}

/*===========================================================================

FUNCTION RRC_TASK2

DESCRIPTION
  This function is the entry point for RRC Task. It initializes RRC state
  and individual procedures by calling rrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, rrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -e715*/
#ifdef FEATURE_SEGMENT_LOADING
void rrc_1_task( dword param) __attribute__((section(".task_text")));
#endif /* FEATURE_SEGMENT_LOADING */
void  rrc_1_task( dword param)
{
#ifdef FEATURE_DUAL_WCDMA 
  rrc_main(SYS_MODEM_AS_ID_2);
#endif
}

