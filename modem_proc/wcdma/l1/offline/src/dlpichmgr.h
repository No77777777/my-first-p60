#ifndef DLPICHMGR_H
#define DLPICHMGR_H

/*===========================================================================
                 D L P I C H M G R . H

GENERAL DESCRIPTION
  This file contains the declarations and definitions of dlpichmgr needed
  across modules.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/dlpichmgr.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
03/09/15    sr      DLCHMGR changes for W+W
01/19/15   sks      Global re-org for W+W
02/20/13    hk      Mainlined FEATURE_WCDMA_DRX_MANAGER
07/13/12    pv      Added offline PICH proc/parallel reacq support
03/06/09    vp      Handle pre and post cell transition indications from DlChMgr.
02/20/09    vb      corrected merge issue
02/20/09    vp      Handle pre and post cell transition indication from L1M.
12/10/08    ks      Mainlining FEATURE_WCDMA_DL_ENHANCED
01/25/08    vsr     Checking in Enhanced DL files onto mainline
01/15/08    vp      Added code comments and Removed static db
11/06/07    vp      Moved DRX handle from dlchmgr to channel specific managers
                    and added shared channel support in sccpchmgr.
08/20/07    vsr     Moved in pich_hwch
08/20/07    vp      Added support for new sccpch0/pich reponse functions.
02/21/07    vp      Initial version
                    ===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1const.h"
#include "dlchmgr.h"
#include "wl1drxmanager.h"

/*The maximum number of PICH per sub that can be active at a time.*/
#define DL_PICH_CFG_DB_SIZE 2

typedef struct {
boolean in_use;

uint8 pich_sccpch_op_status;

dlchmgr_cfg_db_idx_type chmgr_cfg_db_idx;

/* Sub ID */
wsub_id_e_type wsub_id;

drx_entity_handle_type drx_handle;

} dl_pich_cfg_db_struct_type;

typedef struct
{
  boolean dl_pich_actual_wakeup;
}dlpichmgr_ext_cntrl_params_type;

extern dlpichmgr_ext_cntrl_params_type dlpichmgr_ext_cntrl_params[WCDMA_NUM_SUBS];

/* This structure holds the reponse functions for channel type: PICH*/
extern dlchmgr_resp_func_cb_struct_type dl_pich_resp_func_cb;

/* This structure holds the index management callback functions to pich manager*/
extern dlchmgr_idx_mgmt_func_struct_type dl_pich_idx_mgmt_func;

/* This structure holds the pre and post operation callbacks
   for pich*/
extern dlchmgr_op_cb_func_struct_type dl_pich_op_cb_func;

extern const channel_cb_struct_type  dl_pich_drx_cb_functions;

/* hardware channel used for the pich */
extern uint8 pich_hwch;  

extern dl_pich_cfg_db_struct_type dl_pich_cfg_db[DL_PICH_CFG_DB_SIZE];

/*===========================================================================
FUNCTION        dl_nsch_alloc_cfg_db_idx

DESCRIPTION     This function finds a free entry in the pich manager cfg 
                db and returns it to the caller.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The allotted index in pich manager cfg db.

SIDE EFFECTS    None.
===========================================================================*/
extern dl_ch_spec_cfg_db_idx_type dl_pich_alloc_cfg_db_idx(
  dlchmgr_cfg_db_idx_type chmgr_cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_dealloc_cfg_db_idx

DESCRIPTION     This function frees entry in the pich manager cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_pich_dealloc_cfg_db_idx(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_per_cfg_init

DESCRIPTION     This function initializes the pich db for perticular cfg db idx. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_pich_per_cfg_init(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION        dl_pich_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_pre_add_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx,
  l1_dl_phychan_db_struct_type *phychan_db, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_post_add_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_pre_drop_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_post_drop_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_get_chmgr_cfg_idx

DESCRIPTION     This function returns the cfg index of PICH channel in the 
                of the channel.channel manager cfg db. SCCPCH manager can 
                use the returned index to do operations on PICH.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The cfg db index of PICH in chmgr.

SIDE EFFECTS    None.
===========================================================================*/
extern dlchmgr_cfg_db_idx_type dl_pich_get_chmgr_cfg_idx( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     PICH_NEXT_FRAME_WANTED_CB

DESCRIPTION
  DRX Manager invokes this Channel Manager callback function to find out
  PICH scheduling information, for example if it wants to go to sleep and
  for how long.

DEPENDENCIES
  None.

PARAMETERS
  entity_handle - DRX handle identifying PICH.

RETURN VALUE
  sleep_info_ptr - Pointer to sleep information returned to the caller.
===========================================================================*/
extern boolean dl_pich_next_frame_wanted_cb(drx_entity_handle_type          entity_handle,
  drx_manager_sleep_struct_type  *sleep_info_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_is_both_pich_sccpch_op_done

DESCRIPTION     This function returns whether both SCCPCH0 and PICH operations 
                have been completed after they have been started together. This 
                helps in sending a single response to L1M.

DEPENDENCIES    None

RETURN VALUE
                TRUE: Both SCCPCH0 and PICH operations are done.
                FALSE: At least one among the SCCPCH0 and PICH operations is due.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_is_both_pich_sccpch_op_done(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_upd_sccpch_op_status

DESCRIPTION     This function marks that the SCCPCH operation has been 
                completed in the PICH cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_pich_upd_sccpch_op_status(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_pre_resume_cb

DESCRIPTION     This function does any processing needed before the resume 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-resume operations were successful.
                FALSE: At least one pre-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_pre_resume_cb(drx_entity_handle_type entity_handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_pre_suspend_cb

DESCRIPTION     This function does any processing needed before the suspend 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-suspend operations were successful.
                FALSE: At least one pre-suspend operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_pre_suspend_cb(drx_entity_handle_type entity_handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_post_resume_cb

DESCRIPTION     This function does any processing needed after the resume 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-resume operations were successful.
                FALSE: At least one post-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_post_resume_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_post_suspend_cb

DESCRIPTION     This function does any processing needed after the suspend 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-suspend operations were successful.
                FALSE: At least one post-suspend operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_pich_post_suspend_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_pre_cell_trans_cb

DESCRIPTION     This function should be called by DLChMgr before starting to process 
                CPHY_CELL_TRANSITION_REQ from RRC.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_pich_pre_cell_trans_cb( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_pich_post_cell_trans_cb

DESCRIPTION     This function should be called by DlChMgr before sending response for 
                CPHY_CELL_TRANSITION_REQ to RRC.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_pich_post_cell_trans_cb(boolean cell_trans_status, wsub_id_e_type wsub_id);

#endif

