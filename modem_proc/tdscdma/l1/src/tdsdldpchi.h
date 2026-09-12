#ifndef TDSDLDPCH_I_H
#define TDSDLDPCH_I_H

/*===========================================================================
                               D L N S C H M G R I. H

GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlnschmgr.

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
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdldpchi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
                    
===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"

#define TDSDLDPCH_IS_CFG_DB_IDX_NOT_IN_USE()    \
  (tdsdldpch_cfg_db.in_use == FALSE )

#define TDSDLDPCH_IS_CFG_DB_IDX_IN_USE()    \
  (tdsdldpch_cfg_db.in_use == TRUE )

#define TDSDLDPCH_GET_FREQ()    \
    (TDSDLCHMGR_GET_FREQ_FROM_CFG_DB_IDX(TDSDLDPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLDPCH_GET_CPI()    \
  (TDSDLCHMGR_GET_CPI_FROM_CFG_DB_IDX(TDSDLDPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLDPCH_IS_CCTRCH_IDX_INVALID(cctrch_idx)    \
  (TDSDLCHMGR_IS_CCTRCH_IDX_INVALID(cctrch_idx))

#define TDSDLDPCH_GET_CHMGR_CFG_DB_IDX()    \
    (tdsdldpch_cfg_db.chmgr_cfg_db_idx)

#define TDSDLDPCH_GET_CCTRCH_IDX_FROM_DB_IDX()    \
    (TDSDLCHMGR_GET_CCTRCH_IDX_FROM_CFG_DB_IDX(TDSDLDPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLDPCH_GET_PHCH_HDLE_FROM_CFG_DB_IDX()    \
    (TDSDLCHMGR_GET_PHCH_HDLE_FROM_CFG_DB_IDX(TDSDLDPCH_GET_CHMGR_CFG_DB_IDX()))

#define TDSDLDPCH_GET_DRX_HDLE_FROM_CFG_DB_IDX()    \
    (tdsdldpch_cfg_db.drx_handle)

#define TDSDLDPCH_IS_DRX_HDLE_INVALID_FROM_CFG_DB_IDX()    \
    (TDSDLDPCH_GET_DRX_HDLE_FROM_CFG_DB_IDX() == TDSDRXMGR_ENTITY_HANDLE_INVALID)


/*===========================================================================
FUNCTION        tdsdldpch_alloc_cfg_db_idx

DESCRIPTION     This function finds a free entry in the bcch manager cfg 
                db and returns it to the caller.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The allotted index in bcch manager cfg db.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL tdsdlchmgr_dlch_spec_cfg_db_idx_type tdsdldpch_alloc_cfg_db_idx(tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_dealloc_cfg_db_idx

DESCRIPTION     This function frees entry in the nsch manager cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL void tdsdldpch_dealloc_cfg_db_idx(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_pre_add_cb(
  tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx,
  tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdldpch_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_post_add_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_next_frame_wanted_cb

DESCRIPTION     This function returns the next frame wanted to DRX. As nsch 
                channels have no schedule, it always returns ALWAYS_ACTIVE.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
LOCAL boolean tdsdldpch_next_frame_wanted_cb(
  tdsdrxmgr_entity_handle_type  entity_handle,
  tdsdrxmgr_sleep_struct_type  *sleep_info_ptr);

/*===========================================================================
FUNCTION        tdsdldpch_pre_recfg_cb

DESCRIPTION     This function does any processing needed before the re-cfg 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-recfg operations were successful.
                FALSE: At least one pre-recfg operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_pre_recfg_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx,
                                           tdsl1_dl_phycfg_phychan_db_struct_type *phychan_db);

/*===========================================================================
FUNCTION        tdsdldpch_post_recfg_cb

DESCRIPTION     This function does any processing needed after the re-cfg 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-recfg operations were successful.
                FALSE: At least one post-recfg operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_post_recfg_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_pre_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean tdsdldpch_post_drop_cb(tdsdlchmgr_dlch_spec_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdldpch_get_bitmask_cb

DESCRIPTION     This function calculate the bitmask for dpch.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL tdsdlchmgr_spec_tdm_bitmask_type tdsdldpch_get_bitmask_cb(uint16 activation_time);

#endif /* DLNSCHMGR_I_H */

