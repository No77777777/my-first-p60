#ifndef L1POS_H
#define L1POS_H

/*==========================================================================

             L1 Positioning Manager Header

DESCRIPTION
  This file contains code that is used by the WCDMA L1 subsystem to support
  UMTS Positioning Features.

  Copyright (c) 2003-2013 by Qualcomm Technologies, Inc.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/l1pos.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/03/18   rn      FR 47254: WCDMA- Global Time Services Support
10/06/15   us      Created independent l1pos_lsm_timetag_req_pending for subs
06/30/15   sks     Do not do a sleep abort for timetag request in any state.
01/20/15   jd      Global re-org for W + W
02/20/13   hk      Mainlined FEATURE_WCDMA_DL_ENHANCED
08/24/11   scm     Remove #ifndef FEATURE_XO from wl1_freeze_and_set_trk_lo_pdm().
04/28/11   mmr     Added support for Feature XO.
04/25/11   geg     Replaced *.h with *_v.h
01/27/10   scm     Resume trk_lo freeze if aborted earlier during service loss.
04/10/09   scm     Fix access_mdsp_for_freq_error() comment.
04/05/09   scm     Featurize l1pos_set_tau_dpch().
03/27/09   scm     Fix code to undefine FEATURE_USER_PLANE_MRL.
05/29/06   kps     Support for CGPS
03/20/07   scm     Extern access_mdsp_for_trk_lo_adj_pdm().
11/13/06   kps     TCXO Rotator push support
                   Extern access_mdsp_for_freq_error()
10/31/06   kps     Remove tcxo_srm if we're using TCXOMGR 3.0
10/13/06   kps     Extern access_mdsp_for_trk_lo_adj_pdm() to use it in wl1trm.c
04/25/05    mc     Added P4 DateTime and Author header.
04/18/05    ld     Added Sleep timetag related code.
07/07/04   scm     Add extern to wl1_freeze_and_set_trk_lo_pdm().
07/02/04   scm     Add extern to wl1_vctcxo_state_processor_reset().
06/08/04   scm     Add extern to wl1_vctcxo_set_client_frozen_status();
04/09/04   scm     Add extern to wl1_is_vctcxo_err_too_big().
02/10/04   scm     Support for VCTCXO Shared Resource Manager (SRM).
01/06/03   scm     Remove unnecessary ARGSUSED (lint) comment.
12/18/03   scm     Add extern to l1m_cphy_lsm_cell_meas_req().
04/28/03   djm     Initial revision.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "wcdma_variation.h"
#include "l1task_v.h"

#ifndef FEATURE_WCDMA_PLT
#include "l1lsmif_v.h"
#endif



typedef enum
{
  IDLE__TRACKING = 0,
  IDLE__FROZEN,
  NO_SERVICE,
  VCTCXO_ERR_EXCEEDED,
  INVALID_STATE,
  NUM_VCTCXO_STATES = INVALID_STATE
} vctcxo_state_type;

typedef struct
{
  void (*entry)(wsub_id_e_type wsub_id);
  void (*exit_fn)(wsub_id_e_type wsub_id);
} vctcxo_state_processor_type;

typedef struct
{
  #if defined(FEATURE_MMGPS) ||     defined(FEATURE_CGPS_TIME_TAGGING)
  /* Allows us to delay the TIMETAG in DRX mode until just before we go to
** sleep at the end of a cycle, so we get good STMR count, etc. */
  wl1_cell_update_e_type l1pos_lsm_timetag_req_pending[WCDMA_NUM_SUBS];
  #endif
}l1pos_ext_cntrl_params_type;

extern l1pos_ext_cntrl_params_type l1pos_ext_cntrl_params;


typedef struct
{
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  boolean f_vctcxo_err_exceeded;
  #endif
  #if defined(FEATURE_CGPS_UMTS_UP) && defined(FEATURE_USER_PLANE_MRL)
  /* WL1->GPS callback function pointer. */
  l1_cgps_event_cb_type l1_cgps_event_cb;
  #endif
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  boolean f_wl1_frozen;
  #endif
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  /* REX timer to peridically check the error in VCTCXO. */
  rex_timer_type check_vctcxo_timer;
  #endif
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  vctcxo_state_type wl1_vctcxo_state;
  #endif
  #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
  boolean f_client_frozen;
  #endif
}l1pos_int_cntrl_params_type;

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#if defined(FEATURE_MMGPS) ||        defined(FEATURE_CGPS_TIME_TAGGING)

#endif

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT
/*===========================================================================
FUNCTION     WL1_FREEZE_AND_SET_TRK_LO_PDM

DESCRIPTION
  This is a debug function only, invoked by a special debug QXDM command.
  This function freezes VCTCXO and slams it to the value specified by the
  passed-in parameter.  This function does no error checking or state
  checking or anything like that, so the user must know what he/she is
  doing.  Slamming to a value that cannot be compensated for by rotators
  results in call drop, indefinite service loss, etc.

  The command typed into the DM command prompt looks as follows:

  send_data 75 4 20 0 0x60 0x09

  The last 2 values define the new PDM setting.  For example, the above
  slams to a value of 2400 = 0x0960.  Swap the bytes of the hex value to
  slam and enter on the command line separated by spaces.

DEPENDENCIES
  None.

PARAMETERS
  uint16 tcxo_pdm_val - Value to slam into VCTCXO PDM register.

RETURN VALUE
  None.

SIDE EFFECTS
  Potentially nasty.
===========================================================================*/
extern void wl1_freeze_and_set_trk_lo_pdm(uint16 tcxo_pdm_val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_VCTCXO_SET_WL1_FROZEN_STATUS

DESCRIPTION
  WCDMA L1 calls this function when it changes its VCTCXO frozen status.
  It freezes VCTCXO during acquisition.  It unfreezes VCTCXO once pull-in
  succeeds, and WCDMA is acquired.  The VCTCXO tracking state machine then
  manages the WL1 and client requirements on VCTCXO.

DEPENDENCIES
  None.

PARAMETERS
  boolean wl1_frozen - TRUE if WL1 has frozen VCTCXO for acquisition.
                       FALSE if not frozen (have WCDMA service).
RETURN VALUE
  boolean - TRUE if earlier had to unfreeze trk_lo for large frequency error.
          \ FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_vctcxo_set_wl1_frozen_status(boolean wl1_frozen, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_VCTCXO_SET_CLIENT_FROZEN_STATUS

DESCRIPTION
  This function is invoked when the VCTCXO frozen request status of the
  client(s) changes.  Clients like GPS, BT, etc., can request that WL1 freeze
  VCTCXO.  When that status changes, this function processes the change.

DEPENDENCIES
  None.

PARAMETERS
  boolean client_frozen - TRUE if any client requests a VCTCXO freeze.
                          FALSE if no clients require a VCTCXO freeze.
RETURN VALUE
  boolean - TRUE:  VCTCXO is frozen.  Client processing may proceed.
          \ FALSE: We could NOT freeze VCTCXO.  Client processing
                   should NOT proceed.
SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_vctcxo_set_client_frozen_status(boolean client_frozen, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     ACCESS_MDSP_FOR_TRK_LO_ADJ_PDM

DESCRIPTION
  This function is called to retrieve the VCTCXO trk_lo_adj PDM value in the
  mDSP.  If the mDSP clock regime is not on, turn it on briefly for the mDSP
  access.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint16 access_mdsp_for_trk_lo_adj_pdm( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     ACCESS_MDSP_FOR_FREQ_ERROR

DESCRIPTION
  This function is called to retrieve the frequency error accumulated in the
  rotator(s) in the mDSP.  If the mDSP clock regime is not on, turn it on
  briefly for the mDSP access.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  int16 raw_freq_error - Frequency error read from WCDMA firmware.  Value has
                         units of 7.3242 Hz.
SIDE EFFECTS
  None.
===========================================================================*/
extern int16 access_mdsp_for_freq_error( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_IS_VCTCXO_ERR_TOO_BIG

DESCRIPTION
  This function returns whether or not we have rejected any active freeze
  request(s) because the VCTCXO error got too large.  If so, we are now
  allowing mDSP firmware to correct it.

  This function is called by DRX code before disabling fingers for sleep and
  putting the mDSP to sleep.  In the event that we want VCTCXO to track,
  DRX code keeps the mDSP awake and fingers tracking downlink pilot.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if we're allowing VCTCXO to track because its error became
            too large.  FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_vctcxo_err_too_big(void);

/*===========================================================================
FUNCTION     WL1_VCTCXO_STATE_PROCESSOR_RESET

DESCRIPTION
  This function is called when WCDMA is deactivated, for WCDMA is no
  longer controlling VCTCXO.  It resets the VCTCXO state machine so that if
  WCDMA is activated at some point in the future, it starts up in a
  consistent state.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_vctcxo_state_processor_reset(void);

/*===========================================================================
FUNCTION     WL1_VCTCXO_STATE_PROCESSOR_INIT

DESCRIPTION
  This function is called one time shortly after the WCDMA L1 task starts
  running.  It allows us to define the timer used to monitor VCTCXO.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_vctcxo_state_processor_init(void);

#endif  /* #ifdef FEATURE_WCDMA_TCXO_RESOURCE_SUPPORT */

#if defined(FEATURE_MMGPS) ||     defined(FEATURE_CGPS_TIME_TAGGING)

/*===========================================================================
FUNCTION     L1POS_SEND_CELL_INFO_TO_LSM

DESCRIPTION
  This function is called within WL1 software before and after a reference
  cell change.  This allows LSM to keep up-to-date with WCDMA time, which may
  change as the reference (or serving) cell changes.

DEPENDENCIES
  None.

PARAMETERS
  update_type - Why we are sending WCDMA time info to LSM:
                  WL1_TIMETAG_PRE_CELL_CHANGE
                  WL1_TIMETAG_POST_CELL_CHANGE
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1pos_send_cell_info_to_lsm(wl1_cell_update_e_type update_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_CPHY_LSM_TIMETAG_REQ

DESCRIPTION
  This function handles the CPHY_LSM_TIMETAG_REQ primitive from LSM.  LSM
  sends this request to WCDMA L1 when it wants to associate GPS time with
  WCDMA time.  It may then disable GPS time tracking to save power.  Then
  when it wants to get GPS time back, it can get the current WCDMA time with
  this same primitive.  Given the WCDMA time that has elapsed, it can
  determine what the current GPS time must be.

DEPENDENCIES
  None.

PARAMETERS
  l1_ext_cmd_type *cmd_ptr - Command pointer from LSM, which is not used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_cphy_lsm_timetag_req(l1_ext_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

#endif  /* #if defined(FEATURE_MMGPS) || defined(FEATURE_MMGPS_WL1) || \
               defined(FEATURE_CGPS_TIME_TAGGING) */

#if defined(FEATURE_MMGPS) ||     defined(FEATURE_USER_PLANE_MRL)
/*===========================================================================
FUNCTION     L1M_CPHY_LSM_CELL_MEAS_REQ

DESCRIPTION
  This function handles the CPHY_LSM_CELL_MEAS_REQ primitive from LSM.  LSM
  sends this request to get cell signal strength information from WCDMA L1.
  WL1 returns Ec/Io and RSCP measurements on all active cells and
  synchronous neighbor cells.  LSM can then pass this data in Network
  Measurement Reports to the network, which can make position estimates
  based upon these signal strengths and known cell locations.

DEPENDENCIES
  None.

PARAMETERS
  l1_ext_cmd_type *cmd_ptr - Command pointer from LSM, which is not used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_cphy_lsm_cell_meas_req(l1_ext_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION l1pos_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void l1pos_ext_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION l1pos_int_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void l1pos_int_cntrl_params_init( void );

#if  defined(FEATURE_LOC_GTS_SUPPORT)
/*===========================================================================
FUNCTION l1pos_update_gts_info

DESCRIPTION 
  This function will be used to populate the wcdma-gts datastructure
  and update GPS with the timing information.
 
  This function will be called :
  - during sleep-wakeup occasions
  - every 1 second in connected state (DCH)
    

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void l1pos_update_gts_info(wsub_id_e_type wsub_id);
#endif /* defined(FEATURE_LOC_GTS_SUPPORT) */

#endif  /* #ifndef L1POS_H */
