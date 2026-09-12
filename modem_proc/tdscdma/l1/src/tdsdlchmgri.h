#ifndef TDSDLCHMGR_I_H
#define TDSDLCHMGR_I_H

/*===========================================================================
                               D L C H M G R I. H

GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlchmgr.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2007 - 2010 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdlchmgri.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------

===========================================================================*/
#define TDSDLCHMGR_NOTIFY_ASAP 0
#define TDSDLCHMGR_NOTIFY_NORMAL 1
#define TDSDLCHMGR_NOTIFY_DELAY 2
#define TDSDLCHMGR_NOTIFY_PROTECT 3

/*tdsdlchmgr_gen_op_stat_db_idx_enum_type holds the type of general operations.*/
typedef enum {
TDSDLCHMGR_GEN_OP_STAT_TYPE_SLEEP,
TDSDLCHMGR_GEN_OP_STAT_TYPE_CELL_TRANS,
TDSDLCHMGR_GEN_OP_STAT_TYPE_IDLE_REQ,
TDSDLCHMGR_GEN_OP_STAT_TYPE_TOTAL_NUM
} tdsdlchmgr_gen_op_stat_db_idx_enum_type;

/*if the type or maximum possible value of tdsdlchmgr_cfg_db_idx_type changes from
  the current values(uint8, 6), then the type of dlchmgr_dynamic_db_idx_bitmask_type 
  has to be changed accordingly*/
typedef uint32 tdsdlchmgr_cfg_db_idx_genop_bmask_type;

/*tdsdlchmgr_gen_op_dyn_db_struct_type holds all the information pertaining to 
  a general operation.*/
typedef struct {
  /*Status (pass/failed) of the general operation*/
  boolean gen_op_status;

  /*Index in to the gen-op static db corresponding to the general operation 
  in progress*/
  tdsdlchmgr_gen_op_stat_db_idx_enum_type gen_op_stat_db_idx;

  /*gen_op_dyn_db_idx_bmask holds the channel indexes that are involved 
  in the general operation*/
  tdsdlchmgr_cfg_db_idx_genop_bmask_type gen_op_cfg_db_idx_bmask;
} tdsdlchmgr_gen_op_dyn_db_struct_type;

  /*tdsdlchmgr_gen_op_map_struct_type holds the mapping of general operation 
  type from tdsdlchmgr_dl_phychan_action_reason_enum_type to 
  tdsdlchmgr_gen_op_stat_db_idx_enum_type*/
typedef struct {
  tdsdlchmgr_dl_phychan_action_reason_enum_type gen_op_ext_type;

  tdsdlchmgr_gen_op_stat_db_idx_enum_type gen_op_stat_db_idx;
} tdsdlchmgr_gen_op_map_struct_type;

  /*Structure that holds all semi-static information corresponding to a general 
  operation. The information is decided to compile time.*/
typedef struct {
  boolean drx_rel_needed;

  TDSDLCHMGR_GEN_OP_PROC_FUNC_TYPE *gen_op_proc_func;

  TDSDLCHMGR_GEN_OP_CHECK_FUNC_TYPE *gen_op_check_func;

  TDSDLCHMGR_GEN_OP_DONE_CB_FUNC_TYPE *gen_op_done_cb;
} tdsdlchmgr_gen_op_stat_db_struct_type;

  /*Structure that holds all channel independent information for DlChMgr*/
typedef struct {
  tdsdlchmgr_gen_op_dyn_db_struct_type gen_op_dyn_db;
} tdsdlchmgr_db_struct_type;

#define TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_NONE (0x0)

#define TDSDLCHMGR_GEN_OP_INVALID_STAT_TYPE (TDSDLCHMGR_GEN_OP_STAT_TYPE_TOTAL_NUM)

/*Bit masks that indicate the clean-up operation to be performed.*/
#define TDSDLCHMGR_CLEANUP_BIT_NONE (0x0)
#define TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB (0x1)
#define TDSDLCHMGR_CLEANUP_BIT_RESP_CB (0x2)
#define TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB (0x4)
#define TDSDLCHMGR_CLEANUP_BIT_DRX (0x8)
#define TDSDLCHMGR_CLEANUP_BIT_CCTRCH (0x10)
#define TDSDLCHMGR_CLEANUP_BIT_SRCH (0x20)
#define TDSDLCHMGR_CLEANUP_BIT_PHCH (0x40)

/*Macros that indicate the clean-ups to be performed based on the type 
of failure.*/
#define TDSDLCHMGR_CLEANUP_FOR_CHMGR_DYN_DB_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB)
#define TDSDLCHMGR_CLEANUP_FOR_CHSPEC_DB_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CCTRCH)

#define TDSDLCHMGR_CLEANUP_FOR_CCTRCH_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB)
#define TDSDLCHMGR_CLEANUP_FOR_SRCH_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CCTRCH)
#define TDSDLCHMGR_CLEANUP_FOR_PRE_ADD_DRX_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CCTRCH |  \
  TDSDLCHMGR_CLEANUP_BIT_SRCH)
#define TDSDLCHMGR_CLEANUP_FOR_PHCH_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB |  \
  TDSDLCHMGR_CLEANUP_BIT_CCTRCH |  \
  TDSDLCHMGR_CLEANUP_BIT_SRCH |  \
  TDSDLCHMGR_CLEANUP_BIT_DRX)

#define TDSDLCHMGR_CLEANUP_FOR_RECFG_PHCH_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB)
#define TDSDLCHMGR_CLEANUP_FOR_RECFG_DRX_FAILURE (TDSDLCHMGR_CLEANUP_BIT_RESP_CB)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_RESP_CB_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_RESP_CB) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_CHMGR_DB_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_CHMGR_DYN_DB) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_CHSPEC_DB_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_CHSPEC_DB) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_DRX_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_DRX) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_CCTRCH_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_CCTRCH) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_SRCH_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_SRCH) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_CLEANUP_BMASK_HAS_PHCH_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask & TDSDLCHMGR_CLEANUP_BIT_PHCH) != TDSDLCHMGR_CLEANUP_BIT_NONE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_CHSPEC_DB_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_CHSPEC_DB_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_PRE_ADD_DRX_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_PRE_ADD_DRX_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_CCTRCH_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_CCTRCH_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_SRCH_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_SRCH_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_PHCH_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_PHCH_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_RECFG_DRX_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_RECFG_DRX_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_RECFG_PHCH_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_RECFG_PHCH_FAILURE)

#define TDSDLCHMGR_SET_CLEANUP_FOR_CHSPEC_DB_FAILURE(cfg_db_idx)  \
  (tdsdlchmgr_cfg_db[cfg_db_idx].dyn_db.cleanup_bmask = TDSDLCHMGR_CLEANUP_FOR_CHSPEC_DB_FAILURE)

#define TDSDLCHMGR_GET_GEN_OP_STAT_DB_IDX_FROM_DYN_IDX()  \
  (tdsdlchmgr_db.gen_op_dyn_db.gen_op_stat_db_idx)

#define TDSDLCHMGR_GET_CFG_DB_IDX_GEN_OP_BMASK_FROM_DYN_IDX()  \
  (tdsdlchmgr_db.gen_op_dyn_db.gen_op_cfg_db_idx_bmask)

#define TDSDLCHMGR_GET_GEN_OP_STATUS_FROM_DYN_IDX()  \
  (tdsdlchmgr_db.gen_op_dyn_db.gen_op_status)

#define TDSDLCHMGR_IS_DRX_REL_NEEDED_FOR_GEN_OP_FROM_STAT_IDX(stat_db_idx)  \
  (tdsdlchmgr_gen_op_stat_db[stat_db_idx].drx_rel_needed)

#define TDSDLCHMGR_IS_DRX_REL_NEEDED_FOR_GEN_OP_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  (TDSDLCHMGR_IS_DRX_REL_NEEDED_FOR_GEN_OP_FROM_STAT_IDX(gen_op_stat_db_idx))

#define TDSDLCHMGR_IS_GEN_OP_STATIC_DB_IDX_INVALID(static_db_idx)  \
  (static_db_idx >= TDSDLCHMGR_GEN_OP_STAT_TYPE_TOTAL_NUM)

#define TDSDLCHMGR_GET_GEN_OP_PROC_FUNC_FROM_STAT_IDX(stat_db_idx)  \
  (tdsdlchmgr_gen_op_stat_db[stat_db_idx].gen_op_proc_func)

#define TDSDLCHMGR_GET_GEN_OP_CHECK_FUNC_FROM_STAT_IDX(stat_db_idx)  \
  (tdsdlchmgr_gen_op_stat_db[stat_db_idx].gen_op_check_func)

#define TDSDLCHMGR_GET_GEN_OP_DONE_CB_FROM_STAT_IDX(stat_db_idx)  \
  (tdsdlchmgr_gen_op_stat_db[stat_db_idx].gen_op_done_cb)

#define TDSDLCHMGR_GET_GEN_OP_PROC_FUNC_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  (TDSDLCHMGR_GET_GEN_OP_PROC_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx))

#define TDSDLCHMGR_IS_GEN_OP_PROC_FUNC_VALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_PROC_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx)) != NULL)

#define TDSDLCHMGR_IS_GEN_OP_PROC_FUNC_INVALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_PROC_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx)) == NULL)

#define TDSDLCHMGR_GET_GEN_OP_CHECK_FUNC_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  (TDSDLCHMGR_GET_GEN_OP_CHECK_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx))

#define TDSDLCHMGR_IS_GEN_OP_CHECK_FUNC_VALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_CHECK_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx)) != NULL)

#define TDSDLCHMGR_IS_GEN_OP_CHECK_FUNC_INVALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_CHECK_FUNC_FROM_STAT_IDX(gen_op_stat_db_idx)) == NULL)

#define TDSDLCHMGR_GET_GEN_OP_DONE_CB_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  (TDSDLCHMGR_GET_GEN_OP_DONE_CB_FROM_STAT_IDX(gen_op_stat_db_idx))

#define TDSDLCHMGR_IS_GEN_OP_DONE_CB_VALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_DONE_CB_FROM_STAT_IDX(gen_op_stat_db_idx)) != NULL)

#define TDSDLCHMGR_IS_GEN_OP_DONE_CB_INVALID_FROM_DYN_IDX(gen_op_stat_db_idx)  \
  ((TDSDLCHMGR_GET_GEN_OP_DONE_CB_FROM_STAT_IDX(gen_op_stat_db_idx)) == NULL)

#define TDSDLCHMGR_IS_GEN_OP_INVALID_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_GEN_OP_STAT_DB_IDX_FROM_DYN_IDX() >= TDSDLCHMGR_GEN_OP_INVALID_STAT_TYPE)

  #define TDSDLCHMGR_IS_GEN_OP_RESEL_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_GEN_OP_STAT_DB_IDX_FROM_DYN_IDX() == TDSDLCHMGR_GEN_OP_STAT_TYPE_CELL_TRANS)

  #define TDSDLCHMGR_IS_GEN_OP_SLEEP_DROP_ALL_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_GEN_OP_STAT_DB_IDX_FROM_DYN_IDX() == TDSDLCHMGR_GEN_OP_STAT_TYPE_SLEEP)

#define TDSDLCHMGR_GET_STAT_DB_IDX_FOR_GEN_OP(gen_op_ext_type)  \
  (tdsdlchmgr_get_stat_db_idx_for_gen_op(gen_op_ext_type))

#define TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_SET(bmask, cfg_db_idx)  \
  (bmask |= (uint32)((uint32)1 << cfg_db_idx))

#define TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_RESET(bmask, cfg_db_idx)  \
  (bmask &= (uint32)(~((uint32)1 << cfg_db_idx)))

#define TDSDLCHMGR_CLEAR_ALL_DYN_DB_IDX_GEN_OP_BITS_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_CFG_DB_IDX_GEN_OP_BMASK_FROM_DYN_IDX() = TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_NONE)

#define TDSDLCHMGR_IS_GEN_OP_ATLEAST_ONE_CH_OP_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_CFG_DB_IDX_GEN_OP_BMASK_FROM_DYN_IDX() != TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_NONE)

#define TDSDLCHMGR_IS_GEN_OP_NO_CH_OP_FROM_DYN_IDX()  \
  (TDSDLCHMGR_GET_CFG_DB_IDX_GEN_OP_BMASK_FROM_DYN_IDX() == TDSDLCHMGR_DYN_DB_IDX_GEN_OP_BMASK_NONE)

#define TDSDLCHMGR_IS_GEN_OP_COMPLETED_FROM_DYN_IDX()  \
  (TDSDLCHMGR_IS_GEN_OP_NO_CH_OP_FROM_DYN_IDX())

#define TDSDLCHMGR_ADD_CH_OP_STATUS_TO_GEN_OP_STATUS(cfg_db_idx)  \
  (TDSDLCHMGR_GET_GEN_OP_STATUS_FROM_DYN_IDX() &= TDSDLCHMGR_GET_OP_STATUS_FROM_CFG_DB_IDX(cfg_db_idx))

#define TDSDLCHMGR_IS_PHCH_NEEDED_FOR_RESEL_FROM_CFG_DB_IDX(cfg_db_idx)  \
  ((TDSDLCHMGR_GET_STAT_DB_IDX_FROM_CFG_DB_IDX(cfg_db_idx) == TDSDLCHMGR_STATIC_TYPE_BCH_N_SIB_READ) ||  \
  (TDSDLCHMGR_GET_STAT_DB_IDX_FROM_CFG_DB_IDX(cfg_db_idx) == TDSDLCHMGR_STATIC_TYPE_BCH_S_SIB_READ))

#define TDSDLCHMGR_IS_PHCH_NEEDED_FOR_SLEEP_DROP_ALL_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (TDSDLCHMGR_GET_STAT_DB_IDX_FROM_CFG_DB_IDX(cfg_db_idx) == TDSDLCHMGR_STATIC_TYPE_PICH)

#define TDSDLCHMGR_IS_PHCH_EXCLUDED_FROM_DROP_ALL(cfg_db_idx)  \
  (TDSDLCHMGR_IS_GEN_OP_RESEL_FROM_DYN_IDX() &&  \
    TDSDLCHMGR_IS_PHCH_NEEDED_FOR_RESEL_FROM_CFG_DB_IDX(cfg_db_idx))

#define TDSDLCHMGR_IS_PHCH_NOT_EXCLUDED_FROM_DROP_ALL(cfg_db_idx)  \
  (!TDSDLCHMGR_IS_PHCH_EXCLUDED_FROM_DROP_ALL(cfg_db_idx))

#define TDSDLCHMGR_IS_PHCH_EXCLUDED_FROM_SUSPEND_ALL(cfg_db_idx)  \
  (TDSDLCHMGR_IS_GEN_OP_SLEEP_DROP_ALL_FROM_DYN_IDX() &&  \
    TDSDLCHMGR_IS_PHCH_NEEDED_FOR_SLEEP_DROP_ALL_FROM_CFG_DB_IDX(cfg_db_idx))

#define TDSDLCHMGR_IS_PHCH_NOT_EXCLUDED_FROM_SUSPEND_ALL(cfg_db_idx)  \
  (!TDSDLCHMGR_IS_PHCH_EXCLUDED_FROM_DROP_ALL(cfg_db_idx))

/*currently, CTCH-FACH cannot be enabled by means of en_dis_trch mechanism*/
#define TDSDLCHMGR_IS_SHMGR_SVC_TYPE_INVALID(shmgr_svc_type)  \
  (!((shmgr_svc_type == DLSHMGR_SVC_TYPE_R99_FACH) || \
    (shmgr_svc_type == DLSHMGR_SVC_TYPE_R99_PCH)))

/*===========================================================================
FUNCTION        tdsdlchmgr_alloc_cfg_db_idx

DESCRIPTION     This function allocates a free index in the ChMgr dynamic db.

DEPENDENCIES    None

RETURN VALUE
                dyn-db-idx: The allotted index in dynamic db.

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_cfg_db_idx_type tdsdlchmgr_alloc_cfg_db_idx( void );

/*===========================================================================
FUNCTION        dlchmgr_alloc_dyn_db_idx

DESCRIPTION     This function frees the index in the ChMgr dynamic db passed 
                as input parameter.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_dealloc_cfg_db_idx
  (tdsdlchmgr_cfg_db_idx_type dlchmgr_dyn_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_get_cfg_db_idx_from_phch_hdle

DESCRIPTION     This function finds the config db index associated with phch 
                handle provided as input parameter.

DEPENDENCIES    None

RETURN VALUE
                config-db-idx: Config db index associated with the phch handle.

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_cfg_db_idx_type tdsdlchmgr_get_cfg_db_idx_from_phch_hdle
  (tdsdlphch_cfg_handle_type phch_handle);

/*===========================================================================
FUNCTION        tdsdlchmgr_get_cfg_db_idx_from_dl_param

DESCRIPTION     This function finds the config db index associated with the CCTrCh
                or the phch handle in that order.

DEPENDENCIES    None

RETURN VALUE
                config-db-idx: Config db index associated with the input params

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_cfg_db_idx_type tdsdlchmgr_get_cfg_db_idx_from_dl_param
  (uint8 cctrch_idx, tdsdlphch_cfg_handle_type phch_handle);

/*===========================================================================
FUNCTION        tdsdlchmgr_alloc_ch_spec_idx

DESCRIPTION     This function allocates a free index in the channel specific 
                manager's config db.

DEPENDENCIES    None

RETURN VALUE
                ch_spec_idx: The allotted index in channel specific manager dynamic db.

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_dlch_spec_cfg_db_idx_type tdsdlchmgr_alloc_ch_spec_idx
  (tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_handle_cfg_idx_failure_ext_op

DESCRIPTION     This function handles all the operations after a external 
                operation (RRC initiated) has failed because of lack of free 
                entry in dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsdlchmgr_handle_cfg_idx_failure_ext_op(
  tdsl1_dl_phychan_enum_type phch, tdsl1_setup_ops_enum_type op, uint8 cctrch_index);

/*===========================================================================
FUNCTION        tdsdlchmgr_handle_cfg_idx_failure_int_op

DESCRIPTION     This function handles all the operations after a internal 
                operation (L1 initiated) has failed because of lack of free 
                entry in dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_handle_cfg_idx_failure_int_op(
  tdsdlchmgr_int_setup_struct_type *ch_info, tdsl1_setup_ops_enum_type op);

/*===========================================================================
FUNCTION        tdsdlchmgr_get_static_db_idx

DESCRIPTION     This function finds the static db index associated with the channel identified by 
                the input parameter.

DEPENDENCIES    None

RETURN VALUE
                static-db-idx: The static dn index of the channel identified by the input parameter.

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_static_db_idx_type tdsdlchmgr_get_static_db_idx(tdsdlphch_add_type *add_info);

/*===========================================================================
FUNCTION        tdsdlchmgr_handle_cleanup_bmask

DESCRIPTION     This function handles the clean-up after any failure during 
                channel addition or re-cfg.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_handle_cleanup_bmask(tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_init_db

DESCRIPTION     This function initializes a entry identified by the input 
                parameter in dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_init_db(tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_get_stat_db_idx_for_gen_op

DESCRIPTION     This function finds the general operation static db index 
                associated with the general operation identified by the input parameter.

DEPENDENCIES    None

RETURN VALUE
                static-db-idx: The general operation static db index for the operation.

SIDE EFFECTS    None
===========================================================================*/
tdsdlchmgr_gen_op_stat_db_idx_enum_type tdsdlchmgr_get_stat_db_idx_for_gen_op(
  tdsdlchmgr_dl_phychan_action_reason_enum_type gen_op_ext_type);

/*===========================================================================
FUNCTION        tdsdlchmgr_check_for_gen_op_susp_all

DESCRIPTION     This function checks whether a suspend-all general operation 
                has been finished completely in dl-phch. If yes, the done-function 
                corresponding to the general operation shall be called.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_check_for_gen_op_susp_all( tdsdlchmgr_cfg_db_idx_type cfg_db_idx );

/*===========================================================================
FUNCTION        tdsdlchmgr_check_for_gen_op_susp_all

DESCRIPTION     This function checks whether a drop-all general operation 
                has been finished completely in dl-phch. If yes, the done-function 
                corresponding to the general operation shall be called.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_check_for_gen_op_drop_all( tdsdlchmgr_cfg_db_idx_type cfg_db_idx );

/*===========================================================================
FUNCTION        tdsdlchmgr_gen_op_sleep_processed

DESCRIPTION     This function sends response to DRX after a sleep all general 
                operation is completed.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_gen_op_sleep_processed( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_gen_op_cell_trans_processed

DESCRIPTION     This function sends response to L1M after a cell transition general
                operation is completed.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_gen_op_cell_trans_processed( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_gen_op_idle_req_processed

DESCRIPTION     This function sends response to L1M after a idle request general 
                operation is completed.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_gen_op_idle_req_processed( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_hdle_gen_op_post_ch_op

DESCRIPTION     This function should be called after any channel operation if 
                the channel is part of general operation. This function calls the 
                check function corresponding to the general operation.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_hdle_gen_op_post_ch_op(tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_hdle_gen_op_done

DESCRIPTION     This function should be called after a general operation has 
                been completed across all channels. This function calls the 
                Done function corresponding to the general operation.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_hdle_gen_op_done( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_drop_delay_cb //TDS

DESCRIPTION     This function does the drop delay callback.

DEPENDENCIES    None

RETURN VALUE
                TRUE: Channel Drop was started successfully
                FALSE: Channel Drop could not be started successfully

SIDE EFFECTS    None
============================================================================*/
LOCAL void tdsdlchmgr_drop_delay_cb(
/** SUBFN  */
uint16 subfn, uint32 cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlchmgr_drop_all

DESCRIPTION     This function initiates a drop in dl-phch for the channels 
                that are filtered by the general operation in progress.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_drop_all( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_suspend_all

DESCRIPTION     This function initiates a suspend in dl-phch for the channels 
                that are filtered by the general operation in progress.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_suspend_all( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_init_gen_op_db

DESCRIPTION     This function initializes dlchmgr general operation db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_init_gen_op_db( void );

/*===========================================================================
FUNCTION        tdsdlchmgr_fill_stat_info

DESCRIPTION     This function copies the static information pertaining to a channel 
                from static db to config db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsdlchmgr_fill_stat_info(tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_is_ok_for_ext_pccpch_setup

DESCRIPTION     This function finds if it is ok to setup an external pccpch(setup by RRC).
                1. If an external pccpch is already setup and if it is being
                dropped, then it is OK.
                2. Else if an external pccpch is already setup and this
                cphy_setup_req is not dropping it, then it is not OK.
                3. Else(if no external pccpch is already setup), then it is OK.

DEPENDENCIES    None

RETURN VALUE
                TRUE: if it is OK to setup external pccpch.
                FALSE: if it is not OK to setup external pccpch.

SIDE EFFECTS    None
===========================================================================*/
boolean tdsdlchmgr_is_ok_for_ext_pccpch_setup(
  /*setup req mask*/
  uint16 req_mask,

  /* drop phychan type */
  tdsl1_dl_phychan_enum_type drop_dl_phychan
);
#include "tdscdma_variation.h"
#if 0
/*===========================================================================
FUNCTION        tdsdlchmgr_en_svc

DESCRIPTION     This function enables the R99 service on the MBMS Shared Channel

DEPENDENCIES    None

RETURN VALUE
                TRUE: Enabling Service is successful.
                FALSE: Enabling Service fails.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlchmgr_en_svc(
/*cctrch index*/
uint8 cctrch_idx,

/*Service to be handled*/
tdsdlsccpch_svr_type_enum_type shmgr_svc_type
);

/*===========================================================================
FUNCTION        tdsdlchmgr_dis_svc

DESCRIPTION     This function disables the R99 service on the MBMS Shared Channel

DEPENDENCIES    None

RETURN VALUE
                TRUE: Disabling Service is successful.
                FALSE: Disabling Service fails.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlchmgr_dis_svc(
/*cctrch index*/
uint8 cctrch_idx,

/*Service to be handled*/
tdsdlsccpch_svr_type_enum_type shmgr_svc_type
);
#endif
#if 0
/*===========================================================================
FUNCTION        tdsdlchmgr_hs_drop_done

DESCRIPTION     This function is the callback after HS channel is dropped 
                before working on ASU. This resumes ASU cell drop.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Resumes ASU cell drop
===========================================================================*/

void tdsdlchmgr_hs_drop_done(boolean status);
#endif

/*===========================================================================
FUNCTION        tdsdlchmgr_set_NBCH_bitmask

DESCRIPTION     This function set N-PCCPCH bitmask

DEPENDENCIES    None

RETURN VALUE
                None

SIDE EFFECTS    None.
===========================================================================*/
void tdsdlchmgr_set_NBCH_bitmask(tdsdlchmgr_spec_drx_para_type pich_spec_drx_para,
                                 tdsdlchmgr_spec_drx_para_type pch_spec_drx_para);

/*===========================================================================
FUNCTION        tdsdlchmgr_update_NBCH_bitmask_from_cfgindex

DESCRIPTION     This function update N-PCCPCH bitmask

DEPENDENCIES    None

RETURN VALUE
                TRUE:  update N-PCCPCH bitmask is successful.
                FALSE: update N-PCCPCH bitmask  fails.

SIDE EFFECTS    None.
===========================================================================*/
boolean tdsdlchmgr_update_NBCH_bitmask_from_cfgindex(tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================

FUNCTION       DL_PICH_DONE_ISR

DESCRIPTION
  This ISR is called by tramp services when the MDSP_INT5 occurs.
  This function will read the piValue register from the mDSP to ascertain
  whether the PICH was successfully demodulated and if there was a page
  for the UE. If there was a page, enable the PCH transport block delivery.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the UE was paged, the ISR will be de-registered.
  If the UE was not pages, the mDSP will be commanded to again demodulate
  the PICH after the next drx cycle.

===========================================================================*/
void tdsdl_pich_done_isr( void );
/*===========================================================================
FUNCTION        tdsdlchmgr_get_current_add_status

DESCRIPTION     This function will be used to get the current add status from
                static database and schedule results.

DEPENDENCIES    

RETURN VALUE
                current_add_status

SIDE EFFECTS    None
===========================================================================*/
tdsdlphch_state_enum_type tdsdlchmgr_get_current_add_status
                           (tdsdlchmgr_cfg_db_idx_type cfg_db_idx);
/*===========================================================================
FUNCTION        tdsdlchmgr_get_fach_exist_status

DESCRIPTION     This function search for FACH and return the FACH exist status.

DEPENDENCIES    

RETURN VALUE
                TRUE:  Fach exist
                FALSE: Fach not exist

SIDE EFFECTS    None
===========================================================================*/
boolean tdsdlchmgr_get_fach_exist_status(
          tdsdlchmgr_cfg_db_idx_type *fach_cfg_db_idx);
#if 0
/*===========================================================================
FUNCTION        tdsdlchmgr_int_suspend

DESCRIPTION     This function suspend DL channel internally.

DEPENDENCIES    None

RETURN VALUE

SIDE EFFECTS    None
===========================================================================*/
boolean tdsdlchmgr_int_suspend (tdsdlchmgr_cfg_db_idx_type cfg_db_idx);

/*===========================================================================
FUNCTION        tdsdlchmgr_int_resume

DESCRIPTION     This function resume DL channel internally.

DEPENDENCIES    None

RETURN VALUE

SIDE EFFECTS    None
===========================================================================*/
boolean tdsdlchmgr_int_resume (tdsdlchmgr_cfg_db_idx_type cfg_db_idx);
#endif

#ifdef FEATURE_TDSCDMA_CELL_FACH_MEAS_OCCASION
/*===========================================================================
FUNCTION        SRCHFACH_MEAS_N_INTRA_INIT

FILE NAME       

DESCRIPTION     Initialize CELL_FACH non-intra-F cell reselection

DEPENDENCIES    

RETURN VALUE    void
  

SIDE EFFECTS    
===========================================================================*/
void tdsdlchmgr_fmo_parms_init( void );

/*===========================================================================

FUNCTION        tdsdlchmgr_get_sccpch_longest_fach_tti

DESCRIPTION     Get UE monitored SCCPCH longest TTI 

DEPENDENCIES    None

RETURN VALUE    FALSE for failure
  
SIDE EFFECTS    Assign to tdsdlchmgr_fmo_parms.n_tti

===========================================================================*/
LOCAL boolean tdsdlchmgr_get_sccpch_longest_fach_tti( void );

/*===========================================================================
FUNCTION        SRCHFACH_MEAS_CALC_OKSN_TMP

FILE NAME       

DESCRIPTION     Calculate FACH measurement occasions in TASK context

DEPENDENCIES    k and C_RNTI must have been obtained from RRC
                SCCPCH SFN must have been initialized 
                FACH longest TTI on SCCPCH has been obtained

RETURN VALUE    FALSE if meas oksn parameters are not right

SIDE EFFECTS    FACH GSM meas will be started
 
===========================================================================*/
boolean tdsdlmgr_fmo_calc_oksn( void );

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*===========================================================================
FUNCTION        tdsdlchmgr_log_meas_oksn_info

FILE NAME       

DESCRIPTION     Calculate FACH measurement occasions in TASK context

DEPENDENCIES    k and C_RNTI must have been obtained from RRC
                SCCPCH SFN must have been initialized 
                FACH longest TTI on SCCPCH has been obtained

RETURN VALUE    FALSE if meas oksn parameters are not right

SIDE EFFECTS    FACH GSM meas will be started
 
===========================================================================*/
static void tdsdlchmgr_log_meas_oksn_info( boolean oksn_valid );

/*===========================================================================
FUNCTION        tdsdlchmgr_log_pich_decode_result

FILE NAME       

DESCRIPTION     none

DEPENDENCIES    none

RETURN VALUE    none

SIDE EFFECTS    none
 
===========================================================================*/
void tdsdlchmgr_log_pich_decode_result(void);

#endif //FEATURE_TDSCDMA_L1_LOGGING_ENABLED

#endif

#endif /* TDSDLCHMGR_I_H */

