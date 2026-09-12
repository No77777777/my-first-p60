#ifndef TDSRRCPG1_H
#define TDSRRCPG1_H
/*===========================================================================
                  R R C  P A G I N G  T Y P E 1

DESCRIPTION
  This header file contains prototype declarations for Paging Type 1 
  procedure.

Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrcpg1.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/*Max value to set up PCCPCH by L1 is 5 frames+ some guard time of 30msec = (80msec)*/
  #define TDSRRCPG1_MAX_TIME_TO_SET_UP_PCCPCH 8

/*Value of One Frame is 10msec for SFN*/
  #define TDSRRCPG1_VALUE_OF_ONE_FRAME 10

  /*ASN1 valid values for BCCH-ModificationTime 
  are in the range of::= INTEGER (0 . . 511 ) */
  #define TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL 512

  /*MIB-ValueTag ::= INTEGER (1 . . 8 ) */
  #define TDSRRCPG1_INVALID_MIB_V_TAG 9
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Enum which indicates different phases of  BCCH 
Modification info I.E process */
typedef enum
{
  TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS,                   /* Currently BCCH ModInfo i.e is not processed.*/
  TDSRRCPG1_BCCH_MOD_INFO_IN_PROCESS,   /* Currently BCCH ModInfo i.e is being processed.*/
  TDSRRCPG1_BCCH_MOD_INFO_DUPLICATE /* Currently BCCH ModInfo i.e is being processed & received I.E is duplicate of the one in progress.*/
}tdsrrcpg1_bcch_mod_info_status_e_type;

/* Data structure to store information about BCCH 
Modification info I.E received over the air */
typedef struct
{
boolean bcch_mod_info_db_valid; /* Indicates if the stored BCCH
                                      mod info values are valid or not.*/
uint16 bcch_mod_timer_value; /* BCCH modification timer value 
                                       received in latest BCCH mod info I.E */
uint8 bcch_mod_mib_v_tag;    /* MIB value tag received in latest 
                                      latest BCCH mod info I.E */
}tdsrrcpg1_paging_bcch_mod_info;


#define  TDSRRC_PG_PATTERN_MONITOR_UE_MAX_NUM    32
typedef enum
{
  MONITOR_NONE,
  MONITOR_IMSI,
  MONITOR_TMSI,
  MONITOR_PTMSI,
  MONITOR_URNTI
}tdsrrc_monitor_ue_id_type;
typedef struct
{
 
 tdsrrc_monitor_ue_id_type ue_id_type;
 union{
   /* Monitored UE IMSI*/
   rrc_imsi_gsm_map_type       monitor_imsi;
   /* Monitored UE TMSI*/
   rrc_tmsi_gsm_map_type       monitor_tmsi;
   /* Monitored UE PTMSI*/
   rrc_ptmsi_gsm_map_type      monitor_ptmsi;
   /* Monitored UE URNTI*/
   tdsrrc_int_u_rnti_type      monitor_urnti;

 }u;
 /* Page pattern*/
 uint16                      page_pattern;
 uint8                       page_cycle;

}tdsrrc_pg_pattern_monitor_ue_type ;
typedef struct
{
   /* Page pattern detect status*/
   tdsrrc_pg_pattern_detect_operation_enum                     detect_status;
   /* Page pattern detect cause*/
   tdsrrc_pg_pattern_cause_enum    detect_cause;
   /* Indicates whehter error occured*/
   boolean                     error_occur;
   /* Monitor UE number*/   
   uint16    monitored_num;
   /* Final page cycle of current detection*/
   uint8                       page_cycle;
   /* Current monitored page pattern index*/
   uint8                       page_pattern_index;
   /* threshold of high loading*/
   uint8 high_loading_thesh;
   /*Monitored UE info*/
   tdsrrc_pg_pattern_monitor_ue_type monitored_ue[TDSRRC_PG_PATTERN_MONITOR_UE_MAX_NUM];
}tdsrrcpg1_page_detect_pattern_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcpg1_compare_group_release_info  

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcpg1_compare_group_release_info
(
  tdsrrc_GroupReleaseInformation * group_release_info
);

/*===========================================================================

FUNCTION tdsrrcpg1_init_procedure

DESCRIPTION
  This function initializes everything that the Paging Type 1 procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcpg1_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrcpg1_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the Paging Type 1 procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcpg1_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcpg1_post_update_sibs_cmd

DESCRIPTION
  This function is used to post UPDATE_SIBS_CMD
  to SIB procedure

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcpg1_post_update_sibs_cmd
(
  void
);
/*===========================================================================
FUNCTION tdsrrcpg1_check_for_duplicate_bcch_mod_info

DESCRIPTION
This function checks for duplicate received BCCH Mod Info I.E.
If there is BCCH Mod Info I.E in process & the newly received 
I.E has the same parameter as the one under process then this 
function will Indicate duplicate. Other wise it will return appropriate 
status..

DEPENDENCIES
  None.

RETURN VALUE
  TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS = Currently BCCH ModInfo i.e is not processed. 
  TDSRRCPG1_BCCH_MOD_INFO_DUPLICATE =  Currently BCCH ModInfo i.e is being processed & received I.E
                                    is duplicate of the one in progress.
  TDSRRCPG1_BCCH_MOD_INFO_IN_PROCESS =  Currently BCCH ModInfo i.e is being processed.
===========================================================================*/
tdsrrcpg1_bcch_mod_info_status_e_type tdsrrcpg1_check_for_duplicate_bcch_mod_info
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer_val  /* BCCH modification timer value*/
);
/*===========================================================================
FUNCTION tdsrrcpg1_process_bcch_mod_info()

DESCRIPTION
This function will process BCCH ModInfo I.E.
It will take care of starting timer based on current
SFN & the SFN where BCCH modifiction will happen.

This will also update BCCH ModInfo DB with the 
MIB value tag & BCCH Mod timer received in OTA
message.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_process_bcch_mod_info
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer  /* BCCH modification timer value*/
);
/*===========================================================================
FUNCTION tdsrrcpg1_check_and_abort_bcch_mod_info_processing()

DESCRIPTION
This function will check & abort processing of BCCH
ModInfo & also it takes care of stopping this timer 
TDSRRC_BCCH_MODIFICATION_TIMER if its running. 

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_check_and_abort_bcch_mod_info_processing
(
void
);

/*===========================================================================
FUNCTION tdsrrcpg1_set_bcch_mod_info_db()

DESCRIPTION
This function will validate the input parameters & then update
bcch ModInfo db accordingly.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
boolean tdsrrcpg1_set_bcch_mod_info_db
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer  /* BCCH modification timer value*/
);
/*===========================================================================
FUNCTION tdsrrcpg1_process_bcch_mod_info_msg_ie()

DESCRIPTION
This function will process BCCHModInfo I.E received in 
OTA message.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_process_bcch_mod_info_msg_ie
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_time,  /* BCCH modification timer value*/
  boolean bcch_mod_timer_present  /*Boolean to indicate whether BCCH Time present in BCCHModInfo i.e*/
);
/*===========================================================================
FUNCTION tdsrrcpg1_return_bcch_mod_info_status()

DESCRIPTION
This function will return Bcch mod info
status 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE = If BCCH ModIfo in progress
  FALSE = If BCCH ModIfo is not in progress
===========================================================================*/
boolean tdsrrcpg1_return_bcch_mod_info_status
(
void
);
/*===========================================================================

FUNCTION tdsrrcpg1_get_num_pages

DESCRIPTION
  This function returns the value of rrcpg1_num_pages.  This indicates
  the total number of pages received [false alarm + valid pages.
  This function will be called to display counter in QXDM

DEPENDENCIES
  None.

RETURN VALUE
  uint32

===========================================================================*/
uint32 tdsrrcpg1_get_num_pages(void);
/*===========================================================================

FUNCTION tdsrrcpg1_get_num_pages_matched

DESCRIPTION
  This function returns the value of rrcpg1_num_pages_matched.  This indicates
  the total number of pages minus the false alarms.
  This function will be called to display counter in QXDM

DEPENDENCIES
  None.

RETURN VALUE
  uint32

===========================================================================*/
uint32 tdsrrcpg1_get_num_pages_matched(void);

#include "tdscdma_variation.h"
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*===========================================================================

FUNCTION tdsrrcpg1_ff_modify_sibs_for_sfn_update

DESCRIPTION
  

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcpg1_ff_modify_sibs_for_sfn_update(void);
#endif

#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
/*===========================================================================

FUNCTION tdsrrcpg1_set_ignore_cn_page_in_conn_flag

DESCRIPTION
  Set whenever goes into PCH 
  Reset whenever UE is actual paged in PCH
  No need to reset when jump out of PCH

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

void tdsrrcpg1_set_ignore_cn_page_in_conn_flag(boolean ignore_state);
#endif

/*===========================================================================

FUNCTION tdsrrcpg1_update_page_pattern_bitmask

DESCRIPTION
  Update page pattern bitmask for specific UE
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcpg1_update_page_pattern_bitmask(uint16 ue_index);
/*===========================================================================

FUNCTION tdsrrcpg1_append_monitored_ue_info

DESCRIPTION
  Append monitored UE to the tail of the array
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

===========================================================================*/
boolean tdsrrcpg1_append_monitored_ue_info
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
);

/*===========================================================================

FUNCTION tdsrrcpg1_update_page_pattern

DESCRIPTION
  Update page pattern for specific UE ID in page type1 message
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcpg1_update_page_pattern
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
);
/*===========================================================================

FUNCTION tdsrrcpg1_update_monitored_ue_info

DESCRIPTION
  Update monitored ue info during page pattern detection
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

===========================================================================*/
boolean tdsrrcpg1_update_monitored_ue_info
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
);


/*===========================================================================

FUNCTION tdsrrcpg1_update_page_cycle_from_pattern

DESCRIPTION
  Get page cycle from page pattern detected.
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcpg1_update_page_cycle_from_pattern(void);

/*===========================================================================

FUNCTION tdsrrcpg1_pg_pattern_detect

DESCRIPTION
  Page pattern detect operation function.
  START, CONTINUE or STOP page detection and return page cycle back.
  return SUCCESS -- page detect operation success;
  return FAILURE -- page detect operation fail;
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

===========================================================================*/
boolean tdsrrcpg1_pg_pattern_detect(tdsrrc_pg_pattern_detect_operation_enum l1_req, uint8 *detect_value_ptr,tdsrrc_pg_pattern_cause_enum detect_cause);

#endif /* TDSRRCPG1_H */

