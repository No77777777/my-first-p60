#ifndef TDSDLPICH_I_H
#define TDSDLPICH_I_H

/*===========================================================================
                               D L P I C H M G R I. H

GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlpichmgr.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlpichi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
09/21/10    jintaoh      Initial version
                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"

typedef struct {
  boolean in_use;

  uint8 pich_sccpch_op_status;

  tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx;

  tdsdrxmgr_entity_handle_type drx_handle;

} tdsdlpich_cfg_db_struct_type;

/*The maximum number of PICH that can be active at a time.*/
#define TDSDLPICH_CFG_DB_SIZE 1

/*Macros to help in maintaining the status of completion of SCCPCH0 and 
PICH operations.*/
#define TDSDLPICH_NO_CHANNEL_OP_DONE (0x0)
#define TDSDLPICH_PICH_CHANNEL_OP_DONE (0x1)
#define TDSDLPICH_SCCPCH_CHANNEL_OP_DONE (0x2)
#define TDSDLPICH_ALL_CHANNEL_OP_DONE (TDSDLPICH_PICH_CHANNEL_OP_DONE | TDSDLPICH_SCCPCH_CHANNEL_OP_DONE)


#define TDSDLPICH_GET_PHCH_HDLE_FROM_CFG_DB_IDX()    \
  (TDSDLCHMGR_GET_PHCH_HDLE_FROM_CFG_DB_IDX(TDSDLPICH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLPICH_IS_CFG_DB_IDX_NOT_IN_USE()    \
  (tdsdlpich_cfg_db.in_use == FALSE )

#define TDSDLPICH_IS_CFG_DB_IDX_IN_USE()    \
  (tdsdlpich_cfg_db.in_use == TRUE )

#define TDSDLPICH_GET_CHMGR_CFG_DB_IDX()    \
    (tdsdlpich_cfg_db.chmgr_cfg_db_idx)

#define TDSDLPICH_UPD_PICH_SCCPCH_STATUS_TO_NO_OP_DONE()  \
  (tdsdlpich_cfg_db.pich_sccpch_op_status = TDSDLPICH_NO_CHANNEL_OP_DONE)

#define TDSDLPICH_UPD_PICH_SCCPCH_STATUS_TO_PICH_DONE()  \
  (tdsdlpich_cfg_db.pich_sccpch_op_status |= TDSDLPICH_PICH_CHANNEL_OP_DONE)

#define TDSDLPICH_UPD_PICH_SCCPCH_STATUS_TO_SCCPCH_DONE()  \
  (tdsdlpich_cfg_db.pich_sccpch_op_status |= TDSDLPICH_SCCPCH_CHANNEL_OP_DONE)

#define TDSDLPICH_IS_BOTH_PICH_SCCPCH_OP_DONE()  \
  ((tdsdlpich_cfg_db.pich_sccpch_op_status & TDSDLPICH_ALL_CHANNEL_OP_DONE) == TDSDLPICH_ALL_CHANNEL_OP_DONE)

#define TDSDLPICH_GET_DRX_HDLE_FROM_CFG_DB_IDX()    \
    (tdsdlpich_cfg_db.drx_handle)

#define TDSDLPICH_IS_DRX_HDLE_INVALID_FROM_CFG_DB_IDX()    \
    (TDSDLPICH_GET_DRX_HDLE_FROM_CFG_DB_IDX() == TDSDRXMGR_ENTITY_HANDLE_INVALID)

#define TDSDLPICH_GET_FREQ()    \
    (TDSDLCHMGR_GET_FREQ_FROM_CFG_DB_IDX(TDSDLPICH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLPICH_GET_CPI()    \
    (TDSDLCHMGR_GET_CPI_FROM_CFG_DB_IDX(TDSDLPICH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLPICH_GET_CHANNEL_ADD_STATUS()  \
     ((TDSDLCHMGR_GET_CURR_ADD_STATE_FROM_CFG_DB_IDX(TDSDLPICH_GET_CHMGR_CFG_DB_IDX()))== TDSDLPHCH_STATE_ACTIVE)

#define TDSDLPICH_GET_AWAKE_STATUS()   \
      (TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLPICH_GET_CHMGR_CFG_DB_IDX()) == TDSDLCHMGR_RESUMED_STATUS)
     
/*===========================================================================
FUNCTION        tdsdldpch_alloc_cfg_db_idx

DESCRIPTION     This function finds a free entry in the pich manager cfg 
                db and returns it to the caller.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The allotted index in pich manager cfg db.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL tdsdlchmgr_dlch_spec_cfg_db_idx_type
  tdsdlpich_alloc_cfg_db_idx(tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_dealloc_cfg_db_idx

DESCRIPTION     This function frees entry in the pich manager cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL void tdsdlpich_dealloc_cfg_db_idx(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_pre_add_cb(
  tdsdlchmgr_dlch_spec_cfg_db_idx_type  cfg_db_idx,
  tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdlpich_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_post_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_pre_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_post_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_pre_resume_cb

DESCRIPTION     This function does any processing needed before the resume 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-resume operations were successful.
                FALSE: At least one pre-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_pre_resume_cb(tdsdrxmgr_entity_handle_type entity_handle);

/*===========================================================================
FUNCTION        tdsdlpich_pre_suspend_cb

DESCRIPTION     This function does any processing needed before the suspend 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-suspend operations were successful.
                FALSE: At least one pre-suspend operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_pre_suspend_cb(tdsdrxmgr_entity_handle_type entity_handle);

/*===========================================================================
FUNCTION        tdsdlpich_post_resume_cb

DESCRIPTION     This function does any processing needed after the resume 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-resume operations were successful.
                FALSE: At least one post-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_post_resume_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_post_suspend_cb

DESCRIPTION     This function does any processing needed after the suspend 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-suspend operations were successful.
                FALSE: At least one post-suspend operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlpich_post_suspend_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlpich_parameter_calculate

DESCRIPTION     This function calculate pich parameters for schedule use.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
LOCAL boolean tdsdlpich_parameter_calculate(
  /* pointer to the dl_phychan_db part of ordered config */
  tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdlpich_get_drxpara_cb

DESCRIPTION     This function calculate the parameters for pich.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_spec_drx_para_type tdsdlpich_get_drxpara_cb(void);

#endif

