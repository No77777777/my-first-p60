#ifndef TDSDLSCCPCH_I_H
#define TDSDLSCCPCH_I_H

/*===========================================================================
                        T D S D L S C C P C H I. H

GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlsccpchmgr.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlsccpchi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------

                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsl1dlphychancfg.h"
#include "tdsl1msetup.h"
#include "tdsdlchmgr.h"
#include "tdsl1sapcommon.h"

#define TDSDLSCCPCH_INVALID_NUM 0XFF

#define TDSDLSCCPCH_IS_CFG_DB_IDX_NOT_IN_USE()    \
  (tdsdlsccpch_cfg_db.in_use == FALSE )

#define TDSDLSCCPCH_IS_CFG_DB_IDX_IN_USE()    \
  (tdsdlsccpch_cfg_db.in_use == TRUE )

#define TDSDLSCCPCH_IS_CCTRCH_IDX_INVALID(cctrch_idx)    \
  (TDSDLCHMGR_IS_CCTRCH_IDX_INVALID(cctrch_idx))

#define TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()    \
  (tdsdlsccpch_cfg_db.chmgr_cfg_db_idx)

#define TDSDLSCCPCH_GET_CCTRCH_IDX_FROM_CFG_DB_IDX()    \
  (TDSDLCHMGR_GET_CCTRCH_IDX_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_GET_PHCH_HDLE_FROM_CFG_DB_IDX()    \
  (TDSDLCHMGR_GET_PHCH_HDLE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_IS_ASSOC_PICH_ALLOWED_FROM_CFG_DB_IDX()  \
  (TDSDLCHMGR_GET_PHCH_TYPE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()) == \
   TDSL1_DL_PHYCHAN_SCCPCH0)

#define TDSDLSCCPCH_GET_PHCH_TYPE_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (TDSDLCHMGR_GET_PHCH_TYPE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_IS_ASSOC_PICH_INVOLVED_FROM_CFG_DB_IDX()  \
  (tdsdlsccpch_cfg_db.assoc_pich_involved == TRUE)

#define TDSDLSCCPCH_GET_DRX_ENTITY_TYPE() \
    (tdsdlsccpch_cfg_db.drx_entity_type)

#define TDSDLSCCPCH_GET_DRX_CB()    \
  (tdsdlsccpch_cfg_db.drx_cb_functions)

#define TDSDLSCCPCH_GET_DRX_HANDLE()    \
  (tdsdlsccpch_cfg_db.drx_handle)

#define TDSDLSCCPCH_GET_OFFSET()    \
  (tdsdlsccpch_cfg_db.offset)  

#define TDSDLSCCPCH_GET_REPETITION_LENGTH()    \
  (tdsdlsccpch_cfg_db.repetition_length)  

#define TDSDLSCCPCH_GET_REPETITION_PERIOD()    \
  (tdsdlsccpch_cfg_db.repetition_period)  

#define TDSDLSCCPCH_IS_DRX_HDLE_INVALID()    \
  (tdsdlsccpch_cfg_db.drx_handle == TDSDRXMGR_ENTITY_HANDLE_INVALID)

#define TDSDLSCCPCH_GET_FREQ()    \
  (TDSDLCHMGR_GET_FREQ_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_GET_CPI()    \
  (TDSDLCHMGR_GET_CPI_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_GET_CHANNEL_ADD_STATUS()  \
     ((TDSDLCHMGR_GET_CURR_ADD_STATE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))== TDSDLPHCH_STATE_ACTIVE)

#define TDSDLSCCPCH_GET_DL_INTERNAL_OPERATION() \
       (TDSDLCHMGR_GET_DL_INTERNAL_OPERATION_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLSCCPCH_GET_RESUMED_STATUS()    \
((TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX())) == TDSDLCHMGR_RESUMED_STATUS)

#define TDSDLSCCPCH_GET_AWAKE_STATUS()    \
((TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()) == TDSDLCHMGR_RESUMED_STATUS)  || \
    (TDSDLCHMGR_GET_SUSPEND_RESUME_STATE_FROM_CFG_DB_IDX(TDSDLSCCPCH_GET_CHMGR_CFG_DB_IDX()) == TDSDLCHMGR_RESUME_ONGOING_STATUS))


/*===========================================================================
FUNCTION        tdssccpch_post_op_cb

DESCRIPTION     This function does any processing common to add or drop 
                of a channel.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL void tdssccpch_post_op_cb(void);

/*===========================================================================
FUNCTION        tdsdlsccpch_do_drx_reg

DESCRIPTION     This function does drx registration for all the entities 
                active on the channel identified by the input parameter.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All drx registrations were successful.
                FALSE: At least one drx registration failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_do_drx_reg(tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        dl_sccpch_alloc_dyn_db_idx

DESCRIPTION     This function finds a free entry in the sccpch manager dynamic 
                db and allocates it to the caller.

DEPENDENCIES    None

RETURN VALUE
                dyn-db-idx: The allotted index in sccpch manager dynamic db.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL tdsdlchmgr_dlch_spec_cfg_db_idx_type 
        tdsdlsccpch_alloc_cfg_db_idx(tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx);

/*===========================================================================
FUNCTION        dl_sccpch_dealloc_dyn_db_idx

DESCRIPTION     This function frees entry in the sccpch manager dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL void tdsdlsccpch_dealloc_cfg_db_idx(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlsccpch_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_pre_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx,
  tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdlsccpch_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_post_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlsccpch_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_pre_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlsccpch_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_post_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlsccpch_pre_add_cb

DESCRIPTION     This function does any processing needed before the resumption 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-resumption operations were successful.
                FALSE: At least one pre-resumption operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_pre_resume_cb(tdsdrxmgr_entity_handle_type entity_handle);

/*===========================================================================
FUNCTION        tdsdlsccpch_post_resume_cb

DESCRIPTION     This function does any processing needed after the resumption 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-resumption operations were successful.
                FALSE: At least one post-resumption operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_post_resume_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlsccpch_pre_suspend_cb

DESCRIPTION     This function does any processing needed before the suspension 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-suspension operations were successful.
                FALSE: At least one pre-suspension operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_pre_suspend_cb(tdsdrxmgr_entity_handle_type entity_handle);

/*===========================================================================
FUNCTION        tdsdlsccpch_post_suspend_cb

DESCRIPTION     This function does any processing needed after the suspension 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-suspension operations were successful.
                FALSE: At least one post-suspension operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdlsccpch_post_suspend_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlsccpch_get_bitmask_cb

DESCRIPTION     This function calculate the bitmask for pccpch.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_spec_tdm_bitmask_type tdsdlsccpch_get_bitmask_cb(uint16 activation_time);

/*===========================================================================
FUNCTION        tdsdlsccpch_get_drxpara_cb

DESCRIPTION     This function calculate the parameters for sccpch.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
tdsdlchmgr_spec_drx_para_type tdsdlsccpch_get_drxpara_cb(void);

#endif

