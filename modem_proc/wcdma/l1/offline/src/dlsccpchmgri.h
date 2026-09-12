#ifndef DLSCCPCHMGR_I_H
#define DLSCCPCHMGR_I_H

/*===========================================================================
                               D L S C C P C H M G R I. H

GENERAL DESCRIPTION

  This file contains the declarations and definitions that are internal to
  dlsccpchmgr.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2007 - 2008 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/dlsccpchmgri.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
10/07/15    rs      CTCH lock management for DSDS
08/17/15    skk     Double dl_bmc_db for both the SUBs.
05/01/15    kr      FR 26325: CB/BMC support for W/G/L+G MSIM DSDS
03/09/15    sr      DLCHMGR changes for W+W
12/10/08    ks      Mainlining FEATURE_WCDMA_DL_ENHANCED
10/03/08    hk      Bringing in 7k Mailine fixes
01/25/08    vsr     Checking in Enhanced DL files onto mainline
01/23/08    vsr     Lint fixes
01/15/08    vp      Added code comments and Removed static db
11/06/07    vp      Moved DRX handle from dlchmgr to channel specific managers
                    and added shared channel support in sccpchmgr.
08/20/07    vp      Added support for new sccpch0/pich reponse functions.
02/21/07    vp      Initial version
                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "l1const.h"
#include "l1dlphychancfg.h"
#include "l1msetup.h"
#include "dlchmgr.h"

#ifdef FEATURE_UMTS_BMC
/* This defines the state machine for receiving broadcast messages on CTCH
** mapped to FACH on SCCPCH.
** - DL_BMC_CTCH_NOT_CONFIGURED - In this state we have not yet received the
**   CTCH configuration information from RRC.  The serving cell may not
**   support CTCH, or we may not have received that SIB yet, etc.
** - DL_BMC_CTCH_CONFIGURED_NOT_SCHEDULED - In this state RRC has sent CTCH
**   config info (frame offset and allocation period: 25.331, 10.3.8.3), but
**   BMC has not yet enabled CTCH reception in WL1.
** - DL_BMC_CTCH_NON_DRX_SCHEDULING - BMC has informed WL1 to receive CTCH
**   frames in non-DRX mode (Level 1 scheduling), i.e. using only the
**   scheduling information received from RRC.
** - DL_BMC_CTCH_DRX_PENDING - WL1 is in non-DRX mode, and BMC has given a DRX
**   schedule to WL1. We remain in this state until we reach the first frame
**   number of the SFN list sent to WL1 by BMC, when we switch to DRX mode.
** - DL_BMC_CTCH_DRX_SCHEDULING - BMC has informed WL1 to receive CTCH frames
**   in DRX mode (Level 2 scheduling), i.e. using the SFN list sent to WL1 by
**   BMC that identifies the exact CTCH block sets to receive.
**   DL_BMC_CTCH_SCHEDULE_SUSPENDED - WCDMA suspend has been triggered. So CTCH is dropped 
**   but the scheduling info is retained and state info backed up. On resuming in WCDMA, the 
**   schedule would be used again. If stop WCDMA is triggered, then sched expiry ind would be
**   sent to BMC.
** (The DRX scheduling described above is not the same as paging cycle DRX.
**  DRX and non-DRX are modes of CTCH block set reception on SCCPCH.) */
typedef enum
{
  DL_BMC_CTCH_NOT_CONFIGURED = 0,
  DL_BMC_CTCH_CONFIGURED_NOT_SCHEDULED,
  DL_BMC_CTCH_NON_DRX_SCHEDULING,
  DL_BMC_CTCH_DRX_PENDING,
  DL_BMC_CTCH_DRX_SCHEDULING,
  DL_BMC_CTCH_SCHEDULE_SUSPENDED
} dl_bmc_ctch_state_type;

#ifdef FEATURE_UMTS_BMC_SLEEP_OPT
/* This is the value we use to return an invalid sfn */
#define DL_BMC_INVALID_SFN (-1)

/* This is the value we use to return an invalid index into the SFN list */
#define DL_BMC_INVALID_INDEX (0xFF)

/*  This constant is used to make the status of frames in the immediate past
** not be cleared */
#define DL_BMC_FRAMES_BACK_FOR_CLEAR 1024

/* This is the max value by which the sfn can step backwards in 
** next_frame_wanted */
#define DL_BMC_SFN_MAX_STEP_BACK 10

/* Don't discard current DRX schedule until this many frames past schedule end. */
#define DL_BMC_SCHED_END_BUFFER   3
#endif /*FEATURE_UMTS_BMC_SLEEP_OPT*/
        
/* This is the value by which we skip backwards in the sfn list to find
** the first value in the skip list */
#define DL_BMC_SCHED_ENTRIES_BACK_FOR_SKIP 1

/* This is the number of frames by which we look back 
** to find the current sfn in the schedule */
#define DL_BMC_FRAMES_BACK_FOR_FIND 16

#ifdef FEATURE_BMC_EXTENDED_SFN_SUPPORT
/* To handle long BMC DRX cycle lengths, we use an extended SFN between BMC
** and WL1 that is 20 bits instead of just 12. */
#define DL_BMC_MAX_EXTENDED_SFN             1048576

/* This is the value we OR with a schedule element in order to invalidate it
** when it matches an element in a skip list */
#define DL_BMC_INVALIDATE_SCHEDULE_ELEMENT  0x80000000

/* This is the value we OR with a schedule element in order to invalidate it
** when it matches an element in a skip list */
#define DL_BMC_INVALIDATE_QUEUED_SCHED_ELEMENT  0xC0000000
#else
#define DL_BMC_MAX_EXTENDED_SFN             MAX_SFN_COUNT
#define DL_BMC_INVALIDATE_SCHEDULE_ELEMENT  0x8000
#endif

/* This macro checks if a schedule element is valid */
#define DL_BMC_SCHEDULE_ELEMENT_IS_VALID(element) \
  ((element) < DL_BMC_MAX_EXTENDED_SFN)

/* This macro checks if a queued schedule element is valid */
#define DL_BMC_QUEUED_SCHED_ELEMENT_IS_VALID(element) \
  ((element & (~DL_BMC_PRIORITY_SFN_BIT)) < DL_BMC_MAX_EXTENDED_SFN)

/* This is the macro used to convert an sfn element into an invalid one */
#define DL_BMC_INVALIDATE_SFN(sfn) ((sfn) | DL_BMC_INVALIDATE_SCHEDULE_ELEMENT)

/* This is the macro used to convert an sfn element (valid or invalid) into an valid one */
#define DL_BMC_REVALIDATE_SFN(sfn) ((sfn) & ~DL_BMC_INVALIDATE_SCHEDULE_ELEMENT)

/* This is the macro used to convert a queued sfn element (valid or invalid) into an valid one */
#define DL_BMC_REVALIDATE_QUEUED_SFN(sfn) ((sfn) & ~DL_BMC_INVALIDATE_QUEUED_SCHED_ELEMENT)

/* Bit mask for getting priority SFN from schedule given by BMC.*/
#define DL_BMC_PRIORITY_SFN_BIT 0x60000000  /*Mask to get 29th and 30th Bit*/
#define DL_BMC_PRIORITY_SFN_SHIFT 29        /*Shifting to be done to get priority of SFN*/
#define DL_BMC_CRITICAL_PRIORITY_SFN_BIT 0x1/*to get whether SFN is critical or not*/
#define DL_BMC_HIGH_PRIORITY_SFN_BIT 0x2    /*to know whether its a SM or not*/

/* This is the macro used to check if SFN in queued schedule is priority SFN.*/
#define IS_QUEUED_SFN_PRIORITY_SFN(sfn) \
  ((sfn & (DL_BMC_PRIORITY_SFN_BIT)) >> DL_BMC_PRIORITY_SFN_SHIFT)

#define SKIP_BITMASK_LEN 512

/* Macros to help with setting/getting priority bits in priority sfn mask 
   offset is the array index 8 SFN will be in 1st index so offset/8  
   prio-->its the priority of that SFN and contain 2 bits which is set in the mask

for ex for say for index 3(starting from 0)  having priority set as 01
(starting from 0)
 6th bit set to 0 and 7th bit set 1

*/
#define set_mask_bit_with_prio(bitmask, offset,prio)  \
           (((uint16 *)(bitmask))[(offset) >> 3] |= (prio << (2*((offset) & 0x7))))

#define get_mask_bit_with_prio(bitmask, offset)  \
           (((uint16 *)(bitmask))[(offset) >> 3]  & (3 << (2*((offset) & 0x7))))


/* Macros to help with setting bits in the mask */
#define set_mask_bit(bitmask, offset)  \
           (((uint8 *)(bitmask))[(offset) >> 3] |= (1 << ((offset) & 0x7)))
#define get_mask_bit(bitmask, offset)  \
           (((uint8 *)(bitmask))[(offset) >> 3]  & (1 << ((offset) & 0x7)))

/* When we schedule the CTCH awake end time, this is the extra number of frames
** we'll stay awake to make sure we got that CTCH block set.  Most of the time
** we actually receive the data and will initiate sleep immediately, regarless
** of this number.  This value only kicks in if we don't receive the data (like
** during a fade).  This value allows us to start sleep again after a bit. */
#define DL_BMC_EXTRA_AWAKE_FRAMES  1

/* This can be tuned at run-time if necessary.  In CTCH non-DRX mode, with network
** not transmitting CTCH, we keep CTCH active 100% of the time for all allocation
** periods less than this value.  This should help us not miss anything if/when
** network begins transmission. */
#define DL_BMC_MAX_PERIOD_FOR_CTCH_DROP    6

typedef struct
{
  dl_bmc_ctch_state_type  dl_bmc_ctch_state;

  /*This backs up the CTCH state when W is suspended.*/
  dl_bmc_ctch_state_type  dl_bmc_ctch_state_before_suspend;

/* Used to store away the period and offset used by non-DRX CTCH reception
** and the TTI of the FACH carrying CTCH.  These are sent to WL1 by RRC in
** the CPHY_SETUP_REQ for the CTCH SCCPCH. */
  uint16 ctch_allocation_period;
  uint16 cbs_frame_offset;
  uint16 ctch_tti_in_frames;

#ifdef FEATURE_UMTS_BMC_ENH_SCHEDULE
/* Sometimes we adjust ctch_allocation_period for P value in enhanced schedule
** support.  This variable saves "actual" copy, so we can put it back. */
  uint16 ctch_allocation_period_actual;
#endif

/* Used to store the offset of the ctch carrying sccpch from the frame boundary */
 uint32 ctch_sccpch_offset_chipx8;

#ifdef FEATURE_UMTS_BMC_SLEEP_OPT
/* This is the last valid value of the previous schedule which has been
** discarded. Used by frame_is_wanted() to find if a frame in a discarded
** schedule is wanted */
  int32 saved_last_valid_sfn;
#endif /*FEATURE_UMTS_BMC_SLEEP_OPT*/

/* Bit mask with info on priority SFNs. 
   10 implies priority SFN (for scheduling PDU),
   01 implies critical, 00 implies non-priority SFN (data PDU).
   11 will be traeted as critical but its not a valid scenario
   Now we have 2 bits per SFN */
  uint16 *priority_sfn_bitmask ;

/* Used as an index into scheduling_period_sfn_list in dl_bmc_next_frame_wanted().*/
  uint16 scheduling_period_sfn_list_index;

/*Last SFN which was used as argument for dl_bmc_next_frame_wanted*/
  int32  last_sfn;

/* Used as an index into scheduling_period_sfn_list in dl_bmc_frame_is_wanted()
** Different from scheduling_period_sfn_list_index since dl_bmc_next_frame_wanted 
** can be called with a value newer than dl_bmc_frame_is_wanted */
  uint16 scheduling_period_sfn_list_index_b;

/* Bit array used to store skip messages in non-DRX mode */
  uint8 nondrx_skip_mask[SKIP_BITMASK_LEN]; 

}dl_sccpch_bmc_db_struct_type;

typedef struct
{
  dl_sccpch_bmc_db_struct_type dl_bmc_db;
  
  /* Used to save the link to the current & queued schedule in DRX mode */
  ctch_scheduling_descriptor_type active_scheduling_descriptor;
  ctch_scheduling_descriptor_type queued_scheduling_descriptor;

  /* These flags are for debug use only.  One or both can get set to TRUE at run-
  ** time to force us to ignore SKIP_REQ, thus skipping nothing, and to force us
  ** to ignore DRX SCHEDULING_REQ, thus keeping us in non-DRX mode. */
  boolean  dl_bmc_ignore_skip_req;
  boolean  dl_bmc_ignore_drx_sched_req;

  /* TRUE means ignore all SCHEDULING_REQ commands from BMC.  Effectively disables
  ** WCDMA BMC support in the UE. */
  boolean  dl_bmc_disable_bmc;

  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_UMTS_BMC_MSIM)
  wcdma_priority_type dl_bmc_w_priority;
  #endif
}dlsccpchmgr_int_cntrl_params_type;

#endif /* FEATURE_UMTS_BMC */

/*Bit mask to check whether PCH service is present on a sccpch*/
#define DL_SCCPCH_PCH_SVC_CHECK_MASK (dlshmgr_svc_mask_type)(DLSHMGR_SVC_BMSK(DLSHMGR_SVC_TYPE_R99_PCH))

/*Bit mask to check whether FACH service is present on a sccpch*/
#define DL_SCCPCH_R99_FACH_SVC_CHECK_MASK (dlshmgr_svc_mask_type)(DLSHMGR_SVC_BMSK(DLSHMGR_SVC_TYPE_R99_FACH))

#define DL_SCCPCH_INVALID_DYN_DB_IDX (DLCHMGR_INVALID_CH_SPEC_IDX)

#define DL_SCCPCH_IS_CFG_DB_IDX_NOT_IN_USE(cfg_db_idx)    \
  (dl_sccpch_cfg_db[cfg_db_idx].in_use == FALSE )

#define DL_SCCPCH_IS_CFG_DB_IDX_IN_USE(cfg_db_idx)    \
  (dl_sccpch_cfg_db[cfg_db_idx].in_use == TRUE )

#define DL_SCCPCH_IS_CFG_DB_IDX_INVALID(cfg_db_idx)    \
  (cfg_db_idx >= DL_SCCPCH_CFG_DB_SIZE)

#define DL_SCCPCH_IS_DRX_DB_IDX_INVALID(drx_db_idx)    \
  (drx_db_idx >= DL_SCCPCH_R99_LOGICAL_ENTITY_MAX)

#define DL_SCCPCH_IS_DRX_DB_IDX_VALID(drx_db_idx)    \
  (drx_db_idx < DL_SCCPCH_R99_LOGICAL_ENTITY_MAX)

#define DL_SCCPCH_IS_CCTRCH_IDX_INVALID(cctrch_idx)    \
  (DLCHMGR_IS_CCTRCH_IDX_INVALID(cctrch_idx))

#define DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)    \
  (dl_sccpch_cfg_db[cfg_db_idx].chmgr_cfg_db_idx)

#define DL_SCCPCH_GET_CCTRCH_IDX_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (DLCHMGR_GET_CCTRCH_IDX_FROM_CFG_DB_IDX(DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)))

#define DL_SCCPCH_GET_PHCH_HDLE_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (DLCHMGR_GET_PHCH_HDLE_FROM_CFG_DB_IDX(DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)))

#define DL_SCCPCH_GET_CCTRCH_TO_SVC_MAP_MASK_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (dl_sccpch_cfg_db[cfg_db_idx].cctrch_to_svc_map_mask)

#define DL_SCCPCH_IS_ASSOC_PICH_ALLOWED_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (DLCHMGR_GET_PHCH_TYPE_FROM_CFG_DB_IDX(  \
    DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)) == L1_DL_PHYCHAN_SCCPCH0)

#define DL_SCCPCH_GET_PHCH_TYPE_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (DLCHMGR_GET_PHCH_TYPE_FROM_CFG_DB_IDX(  \
    DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)))

#define DL_SCCPCH_IS_ASSOC_PICH_INVOLVED_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (dl_sccpch_cfg_db[cfg_db_idx].assoc_pich_involved == TRUE)

#define DL_SCCPCH_GET_WSUB_ID_FROM_CFG_DB_IDX(cfg_db_idx)  \
  (dl_sccpch_cfg_db[cfg_db_idx].wsub_id)

#define DL_SCCPCH_IS_NO_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (DL_SCCPCH_GET_CCTRCH_TO_SVC_MAP_MASK_FROM_CFG_DB_IDX(cfg_db_idx) == DLSHMGR_SVC_MASK_NONE)

#define DL_SCCPCH_IS_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx, svc_check_mask)    \
  ((DL_SCCPCH_GET_CCTRCH_TO_SVC_MAP_MASK_FROM_CFG_DB_IDX(cfg_db_idx) &    \
  ((dlshmgr_svc_mask_type) svc_check_mask)) != 0)

#define DL_SCCPCH_IS_R99_FACH_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx)    \
  DL_SCCPCH_IS_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx, DL_SCCPCH_R99_FACH_SVC_CHECK_MASK)

#define DL_SCCPCH_IS_PCH_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx)    \
  DL_SCCPCH_IS_SVC_ON_FROM_CFG_DB_IDX(cfg_db_idx, DL_SCCPCH_PCH_SVC_CHECK_MASK)

#define DL_SCCPCH_GET_DRX_ENTITY_TYPE_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].entity_type)

#define DL_SCCPCH_GET_DRX_CB_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].drx_cb_functions)

#define DL_SCCPCH_GET_SVC_TYPE_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].trch_service_type)

#define DL_SCCPCH_GET_FREQ_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (DLCHMGR_GET_FREQ_FROM_CFG_DB_IDX(DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)))

#define DL_SCCPCH_GET_PSC_FROM_CFG_DB_IDX(cfg_db_idx)    \
  (DLCHMGR_GET_PSC_FROM_CFG_DB_IDX(DL_SCCPCH_GET_CHMGR_CFG_DB_IDX(cfg_db_idx)))

#define DL_SCCPCH_GET_CFG_TRCH_MASK_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].cfg_trch_mask)

#define DL_SCCPCH_GET_DRX_HDLE_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].drx_entity_handle)

#define DL_SCCPCH_IS_NO_TRCH_FROM_LOGICAL_ENTITY(logical_entity)    \
  (DL_SCCPCH_GET_CFG_TRCH_MASK_FROM_LOGICAL_ENTITY(logical_entity) == DLSHMGR_SVC_TO_TRCH_MAP_NONE)

#define DL_SCCPCH_IS_DRX_HDLE_INVALID_FROM_LOGICAL_ENTITY(logical_entity)    \
  (DL_SCCPCH_GET_DRX_HDLE_FROM_LOGICAL_ENTITY(logical_entity) == DRX_ENTITY_HANDLE_INVALID)

#define DL_SCCPCH_GET_LOGICAL_ENTITY_ON(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].logical_entity_on)

#define DL_SCCPCH_IS_LOGICAL_ENTITY_ON(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].logical_entity_on == TRUE)

#define DL_SCCPCH_IS_LOGICAL_ENTITY_OFF(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].logical_entity_on == FALSE)

#define DL_SCCPCH_GET_CH_SPEC_IDX_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].ch_spec_idx)

#define DL_SCCPCH_GET_STATE_FROM_LOGICAL_ENTITY(logical_entity)    \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].curr_state)

#define DL_SCCPCH_IS_LOGICAL_ENTITY_VALID(logical_entity)    \
  (logical_entity < DL_SCCPCH_R99_LOGICAL_ENTITY_MAX)

#define DL_SCCPCH_IS_LOGICAL_ENTITY_INVALID(logical_entity)    \
  (!DL_SCCPCH_IS_LOGICAL_ENTITY_VALID(logical_entity))

#define DL_SCCPCH_IS_LOGICAL_ENTITY_STATE_VALID(logical_entity_state)  \
  (logical_entity_state < DL_SCCPCH_LOGICAL_ENT_STATE_MAX)

#define DL_SCCPCH_IS_LOGICAL_ENTITY_STATE_INVALID(logical_entity_state)  \
  (!DL_SCCPCH_IS_LOGICAL_ENTITY_STATE_VALID(logical_entity_state))

#define DL_SCCPCH_UPD_LOGICAL_ENTITY_STATE(logical_entity, new_state)  \
  (dl_sccpch_r99_logical_entity_db[wsub_id][logical_entity].curr_state = new_state)

/*===========================================================================
FUNCTION        dl_sccpch_prep_db

DESCRIPTION     This function resets an entry in sccpch manager dynamic db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
LOCAL void dl_sccpch_prep_db(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_sccpch0_post_op_cb

DESCRIPTION     This function does any processing common to add or drop 
                of a channel.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
LOCAL void dl_sccpch0_post_op_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_sccpch_do_drx_reg

DESCRIPTION     This function does drx registration for all the entities 
                active on the channel identified by the input parameter.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All drx registrations were successful.
                FALSE: At least one drx registration failed.

SIDE EFFECTS    None.
===========================================================================*/
LOCAL boolean dl_sccpch_do_drx_reg(dl_ch_spec_cfg_db_idx_type cfg_db_idx,
  l1_dl_phychan_db_struct_type *phychan_db, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_sccpch_find_logical_entity_from_drx_hdle

DESCRIPTION     This function finds the logical entity associated with the 
                DRX handle passed as input parameter.

DEPENDENCIES    None

RETURN VALUE
                logical entity type: logical entity associated with the DRX handle

SIDE EFFECTS    None
===========================================================================*/
LOCAL dl_sccpch_r99_logical_entity_enum_type dl_sccpch_find_logical_entity_from_drx_hdle(
  drx_entity_handle_type entity_handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_sccpch_find_logical_entity_from_drx_hdle

DESCRIPTION     This function updates the state of a logical entity in logical entity db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
LOCAL void dl_sccpch_upd_entities_state_for_ch(
  dl_ch_spec_cfg_db_idx_type cfg_db_idx, dl_sccpch_r99_logical_state_enum_type new_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_sccpch_clean_logical_entity

DESCRIPTION     This function resets an entry in logical entity db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
LOCAL void dl_sccpch_clean_logical_entity(
  dl_sccpch_r99_logical_entity_enum_type logical_entity, wsub_id_e_type wsub_id);
#endif
