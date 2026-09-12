#ifndef TDSDLSCCPCH_H
#define TDSDLSCCPCH_H

/*===========================================================================
                 T D S D L S C C P C H. H

GENERAL DESCRIPTION
  This file contains the declarations and definitions of dlsccpchmgr needed
  across modules.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlsccpch.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
                    
                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"
#include "tdsdrxmgr.h"

/* Various type of shared services on SCCPCH channel */
typedef enum
{
  DLSHMGR_SVC_TYPE_R99_PCH,  /* R99 PCH channel */
  DLSHMGR_SVC_TYPE_R99_FACH, /* R99 FACH channel */
  DLSHMGR_NUM_SVC_TYPE       /* Number of DL shared service types */
} tdsdlsccpch_svr_type_enum_type;

/*The maximum number of sccpch that has to be managed by sccpch manager at a 
time is 2.*/
#define TDSDLSCCPCH_CFG_DB_SIZE 1

#ifdef FEATURE_TDSCDMA_PCH_FET
#define TDSDLSCCPCH_PCH_FET_SW_SELF_DROP_DELAY 8   /*fet error handling for self-drop, 30ms later*/
#endif /*FEATURE_TDSCDMA_PCH_FET*/



/*Structure that holds all the information pertaining to sccpch*/
typedef struct {
  /*Whether the entry is in use*/
  boolean in_use;

  /*Index for the SCCPCH channel in to the DlChMgr cfg db*/
  tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx;

  /*Whether an associated PICH channel setup was requested by RRC*/
  boolean assoc_pich_involved;

  /** TrCh ref type enable bit field indicating the TrCh ref type data will be
   * passed up to MAC.*/
  uint8 trch_enable_bf;

  /*drx handle associated with the sccpch*/
  tdsdrxmgr_entity_handle_type drx_handle; 

  /*Type of entity to be used in registration with DRX*/
  tdsdrxmgr_entity_type drx_entity_type;

  /* Number of frame offset.  Integer (0...Repetition Period -1).  
  ** SFN modulo Repetition period = offset. 
  ** Repetition period is the one indicated 
  **in the accompanying Common timeslot info IE.  (10.3.6.71)  
  */
  uint8 offset;

  /*Integer(1, 2,4,8,16,32,64). 
  **Default is continuous allocation. 
  **Value 1 indicate continuous. (10.3.6.10)
  */
  uint8 repetition_period;
  
  /*Integer(1.. Repetition period -1 ). (10.3.6.10)
  **NOTE:This is empty if repetition period is set to 1.  
  */
  uint8 repetition_length;

  /*Call back functions to be registered with DRX*/
  tdsdrxmgr_channel_cb_struct_type *drx_cb_functions;

#ifdef FEATURE_TDSCDMA_PCH_FET

  boolean cmd_pch_fet_enable;		 /*need to CMD TFW enable PCH_FET */
  boolean cmd_pch_fet_immd_drop;    /*for next_frame_wanted seperated the immediate drop scenaro*/
  /*dec isr statics when do FET decode [0] 10ms dec pass count [1] 15ms dec pass count [2] 20ms dec pass count */
  boolean pch_fet_just_suspend;
  uint16  pch_fet_suspend_subframe;

#endif


} tdsdlsccpch_cfg_db_struct_type;
#ifdef FEATURE_TDSCDMA_PCH_FET
extern boolean tdsdlsccpch_pch_fet_get_enable(void);
#endif /* FEATURE_TDSCDMA_PCH_FET*/
/*The global that holds cfg information for all sccpch channels currently active*/
extern tdsdlsccpch_cfg_db_struct_type tdsdlsccpch_cfg_db;

/*callback functions to be registered with DRX for PCH*/
extern tdsdrxmgr_channel_cb_struct_type  tdsdlsccpch_drx_cb_functions;

/* This structure holds the reponse functions for channel type: SCPPCH_0*/
extern tdsdlchmgr_resp_func_cb_struct_type tdssccpch_resp_func_cb;

/* This structure holds the index management callback functions to sccpch manager*/
extern tdsdlchmgr_idx_mgmt_func_struct_type tdsdlsccpch_idx_mgmt_func;

/* This structure holds the pre and post operation callbacks
   for sccpch*/
extern tdsdlchmgr_op_cb_func_struct_type tdsdlsccpch_op_cb_func;

extern tdsdlchmgr_schedule_para_cb_func_struct_type tdsdlsccpch_schedule_para_cb_func;

extern uint16 tdsdlsccpch_next_frame_sfn_ind;

extern boolean tdsdlsccpch_current_status;

extern boolean tdsdlsccpch_fach_tdm_on;

#define TDSDLCHMGR_FACH_TTI 2 //FACH TTI 20ms

/*===========================================================================
FUNCTION        tdsdlsccpch_init

DESCRIPTION     This function initializes the sccpch manager. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlsccpch_init( void );

/*===========================================================================
FUNCTION        tdsdlsccpch_is_ok_for_setup

DESCRIPTION     This function examines the possibilty of SCCPCH Setup.

DEPENDENCIES    None

RETURN VALUE
                TRUE: SCCPCH can be setup
                FALSE: SCCPCH cannot be setup

SIDE EFFECTS    None
===========================================================================*/
extern boolean tdsdlsccpch_is_ok_for_setup(
                 tdsl1_dl_phychan_enum_type phch_type,
                 tdsl1_dl_phychan_enum_type drop_dl_phychan);

/*===========================================================================
FUNCTION        tdsdlsccpch_reset_assoc_pich_flag

DESCRIPTION     This function finds the PCCH channel specific cfg_idx from logical entity db and 
                resets the assoc_pich_involved flag.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlsccpch_reset_assoc_pich_flag(void);

/*===========================================================================
FUNCTION        tdsdlsccpch_set_assoc_pich_flag

DESCRIPTION     This function finds the PCCH channel specific cfg_idx from logical entity db and 
                sets the assoc_pich_involved flag.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdlsccpch_set_assoc_pich_flag(void);

/*===========================================================================
FUNCTION        tdsdlsccpch_set_trch_enable_bf

DESCRIPTION     This function set the trch bf of sccpch.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
void tdsdlsccpch_set_trch_enable_bf(uint8 trch_enable_bf);

/*===========================================================================
FUNCTION        tdsdlsccpch_no_lock_handle_for_pch

DESCRIPTION     when wake up to recevie pch, if current not get lock, will set pch sleep forvery waiting for next drx cycle

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern boolean tdsdlsccpch_no_lock_handle_for_pch(
  tdsdrxmgr_entity_handle_type         entity_handle,
  tdsdrxmgr_sleep_struct_type *sleep_info_ptr,
  uint16 subsfn_input
);

#ifdef FEATURE_TDSCDMA_PCH_FET
/*===========================================================================
FUNCTION

DESCRIPTION

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void  tdsdlsccpch_pch_fet_immed_drop(void);

#if defined(FEATURE_TABASCO_MODEM) || defined(FEATURE_JOLOKIA_MODEM)
/*===========================================================================
FUNCTION  tdsdlsccpch_set_pch_fet_suspend_info

DESCRIPTION

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsdlsccpch_set_pch_fet_suspend_info(boolean value);

/*===========================================================================
FUNCTION   tdsdlsccpch_get_pch_fet_suspend_info

DESCRIPTION

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern boolean tdsdlsccpch_get_pch_fet_suspend_info(void);

/*===========================================================================
FUNCTION    tdsdlsccpch_get_pch_fet_suspend_subframe

DESCRIPTION

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern uint16 tdsdlsccpch_get_pch_fet_suspend_subframe(void);
#endif //TA || JO
#endif //FEATURE_TDSCDMA_PCH_FET



#endif

