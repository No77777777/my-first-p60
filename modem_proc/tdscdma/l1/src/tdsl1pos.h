#ifndef TDSL1POS_H
#define TDSL1POS_H

/*==========================================================================

             L1 Positioning Manager Header

DESCRIPTION
  This file contains code that is used by the WCDMA L1 subsystem to support
  UMTS Positioning Features.

  Copyright (c) 2003-2010 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1pos.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/02/2012  weijunz  initial edit for TDSCDMA SW 
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsl1mtask.h"

#include "tdsl1lsmif.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#if defined(FEATURE_MMGPS) || defined(FEATURE_CGPS_TIME_TAGGING)
extern cgps_CellUpdateEnumType tdsl1pos_lsm_timetag_req_pending;
#endif

#ifdef FEATURE_TDSCDMA_GTS
/* Happen every 1 second in FACH/DCH state to update time information to GTS */
extern rex_timer_type tdsl1pos_update_time_info_timer;
#endif

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_TDSCDMA_TCXO_RESOURCE_SUPPORT

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
extern void tdsl1_freeze_and_set_trk_lo_pdm(uint16 tcxo_pdm_val);

/*===========================================================================
FUNCTION     tdsl1pos_vctcxo_set_wl1_frozen_status

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
extern boolean tdsl1pos_vctcxo_set_wl1_frozen_status(boolean wl1_frozen);

/*===========================================================================
FUNCTION     tdsl1pos_vctcxo_set_client_frozen_status

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
extern boolean tdsl1pos_vctcxo_set_client_frozen_status(boolean client_frozen);


/*===========================================================================
FUNCTION     tdsl1pos_access_mdsp_for_trk_lo_adj_pdm

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
extern uint16 tdsl1pos_access_mdsp_for_trk_lo_adj_pdm(void);

/*===========================================================================
FUNCTION     tdsl1pos_access_mdsp_for_freq_error

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
extern int16 tdsl1pos_access_mdsp_for_freq_error(void);

/*===========================================================================
FUNCTION     tdsl1pos_is_vctcxo_err_too_big

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
extern boolean tdsl1pos_is_vctcxo_err_too_big(void);

/*===========================================================================
FUNCTION     tdsl1pos_vctcxo_state_processor_reset

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
extern void tdsl1pos_vctcxo_state_processor_reset(void);

/*===========================================================================
FUNCTION     tdsl1pos_vctcxo_state_processor_init

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
extern void tdsl1pos_vctcxo_state_processor_init(void);

#endif  /* #ifdef FEATURE_TDSCDMA_TCXO_RESOURCE_SUPPORT */

#if(defined(FEATURE_MMGPS) ||  defined(FEATURE_CGPS_TIME_TAGGING))

/*===========================================================================
FUNCTION     tdsl1pos_send_cell_info_to_lsm

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
extern void tdsl1pos_send_cell_info_to_lsm(cgps_CellUpdateEnumType update_type);

/*===========================================================================
FUNCTION     tdsl1pos_l1m_cphy_lsm_timetag_req

DESCRIPTION
  This function handles the TDSL1_CPHY_LSM_TIMETAG_REQ primitive from LSM.  LSM
  sends this request to WCDMA L1 when it wants to associate GPS time with
  WCDMA time.  It may then disable GPS time tracking to save power.  Then
  when it wants to get GPS time back, it can get the current WCDMA time with
  this same primitive.  Given the WCDMA time that has elapsed, it can
  determine what the current GPS time must be.

DEPENDENCIES
  None.

PARAMETERS
  tdsl1_ext_cmd_type *cmd_ptr - Command pointer from LSM, which is not used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1pos_l1m_cphy_lsm_timetag_req(tdsl1_ext_cmd_type *cmd_ptr);


/*===========================================================================
FUNCTION     tdsl1pos_l1m_cphy_lsm_sleep_timetag_req

DESCRIPTION
  This function handles the TDSL1_CPHY_LSM_TIMETAG_REQ primitive from LSM.  LSM
  sends this request to WCDMA L1 when it wants to associate GPS time with
  WCDMA time.  It may then disable GPS time tracking to save power.  Then
  when it wants to get GPS time back, it can get the current WCDMA time with
  this same primitive.  Given the WCDMA time that has elapsed, it can
  determine what the current GPS time must be.
  This function is called only when WCDMA L1 is in TDSL1M_PCH_SLEEP state.

DEPENDENCIES
  None.

PARAMETERS
  tdsl1_ext_cmd_type *cmd_ptr - Command pointer from LSM, which is not used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1pos_l1m_cphy_lsm_sleep_timetag_req(tdsl1_ext_cmd_type *cmd_ptr);


#endif  /* #if defined(FEATURE_MMGPS) ||   defined(FEATURE_CGPS_TIME_TAGGING) */

#if defined(FEATURE_MMGPS) ||  defined(FEATURE_USER_PLANE_MRL) 
/*===========================================================================
FUNCTION     tdsl1pos_l1m_cphy_lsm_cell_meas_req

DESCRIPTION
  This function handles the TDSL1_CPHY_LSM_CELL_MEAS_REQ primitive from LSM.  LSM
  sends this request to get cell signal strength information from WCDMA L1.
  WL1 returns Ec/Io and RSCP measurements on all active cells and
  synchronous neighbor cells.  LSM can then pass this data in Network
  Measurement Reports to the network, which can make position estimates
  based upon these signal strengths and known cell locations.

DEPENDENCIES
  None.

PARAMETERS
  tdsl1_ext_cmd_type *cmd_ptr - Command pointer from LSM, which is not used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1pos_l1m_cphy_lsm_cell_meas_req(tdsl1_ext_cmd_type *cmd_ptr);
#endif

#ifdef  FEATURE_TDSCDMA_PLT
extern  tdsrrc_state_e_type  tdsrrc_get_state( void );
#endif //#ifdef  FEATURE_TDSCDMA_PLT

#ifdef FEATURE_TDSCDMA_GTS
/*===========================================================================
FUNCTION     tdsl1pos_time_info_update_timer_cb

DESCRIPTION
  This is a timer callback function triggered in traffic state. It will update 
  time information to GTS and restart the timer to make it happen periodically.

DEPENDENCIES
  TDS is in traffic state, i.e. FACH/DCH state.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1pos_time_info_update_timer_cb(uint32 unused);

/*===========================================================================
FUNCTION     tdsl1pos_send_time_info_to_gts

DESCRIPTION
  This function reports time information to GTS, which is triggered periodically
  when TDS is in service. Specifically, it's reported every DRX cycle (in idle
  state) or every second (in traffic state)

DEPENDENCIES
  TDS is in service.

PARAMETERS
  None.

RETURN VALUE
  boolean
    TRUE - if time information is sent successfully.
    FALSE - if time information is not sent for any reason.

SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsl1pos_send_time_info_to_gts(void);
#endif /* #ifdef FEATURE_TDSCDMA_GTS */

#endif  /* #ifndef TDSL1POS_H */
