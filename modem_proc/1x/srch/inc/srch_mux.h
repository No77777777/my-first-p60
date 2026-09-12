#ifndef SRCH_MUX_H
#define SRCH_MUX_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             S R C H  _ M U X ---  H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains header information pertaining to srch_mux.  srch_mux
  contains functions which allow the MUX layer to interface with the SRCH
  task.

EXTERNALIZED FUNCTIONS
  srch_mux_finger_finder
  srch_mux_scomb_get_ref_time
  srch_mux_get_curr_20ms_frame
  srch_mux_page_match
  srch_mux_get_fing_rssi
  srch_mux_get_pilot_ecio
  srch_mux_owns_tx
  srch_mux_get_tx_device
  srch_mux_get_ulpn_state
  srch_mux_start_pc_state
  srch_mux_get_rx_agc
  srch_mux_get_tx_adj
  srch_mux_set_tx_adj
  srch_mux_report_frame
  srch_mux_notify_fpc_mode
  srch_mux_update_rc
  srch_mux_log_agc_pctl
  srch_mux_get_active_cell_size
  srch_mux_register_mdsp_isr
  srch_mux_install_mdsp_isr_handler
  srch_mux_get_band_chan
  srch_mux_mdsp_demod_running
  srch_mux_notify_page_decode
  srch_mux_get_rtc_lcstate_at_sync80

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2003 - 2016 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_mux.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/23/16   eye     1x2XTA during SMS/MMS when 1x is on non-DDS sub.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
03/30/16   srk     Pass valid RxLM handle to FW on RX_STOP.
02/19/16   pk      Changes to not to do TA for TDSO call
02/05/16   pk      1x2GTA Feature changes
11/25/15   srk     Export an API to Mux to check WTR Hopping status.
10/02/15   srk     DRX SRCH response deadlock design change and cleanup.
09/29/15   jh      Added more Time DRX enable/disable
09/07/15   bb      Changes to skip searches while HHO is in progress
06/16/15   eye     1x MCVS changes for TA.
05/11/15   jh      RXTX split - FR 21976
03/20/15   ab      Changes to fix race condition between Drx enable/disable
03/12/15   ab      Changes to disable Drx when finger is disabled
03/06/15   ab      Add NV support for DRX enable/disable
02/27/15   ab      Added initial changes for 1x/1xA DRX feature for THOR.
11/23/14   eye     Added support for 0x1877 Power Reporting Log Packet.
09/04/14   sst     Added functionality to page decode stats
04/17/14   cjb     Dynamically handle RF_DEV assignment from TRM for TX case
02/18/14   cjb     Changes to support SHDR on BOLT modem with WTR3925
09/19/13   vks     Export api to return combiner delay
09/10/13   vks     Export api to return lc state, rtc at last sync80 boundary
08/27/13   pap     Mainlining feature FEATURE_MODEM_1X_USE_ATMR_IN_RC11.
08/16/13   vks     Mainline FEATURE_ZZ2_2
05/21/13   srk     Add guaranteed frame information to the API reported by mux.
05/09/13   cjb     Exporting MEMPOOL capture API outside of 1x-L1.
04/19/13   pap     Adding mempool capture capability for diag and fade timers.
12/10/12   trc     Layering/Cleanup for Triton
11/07/11   jtm     Integrate ZZ2_2 support.
06/03/11   adw     SV support.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
08/19/10   jtm     Added srch_mux_mdsp_demod_running.
08/04/10   jtm/adw Added function to notify SRCH of 1x Adv traffic.
04/13/10   vks     Provide API for MUX to know if mdsp is enabled.
12/16/09   jtm     Added srch_mux_get_band_chan().
11/07/09   vks     Removed obsolete srch_mux_set_tx_rate_adj().
05/28/09   jtm     Added functions and typedefs to support Mux's need to
                   register an mDSP interrupt through Searcher.
04/07/09   adw     Added srch_mux_get_active_cell_size().
04/01/09   adw     Categorized included header files and standardized format.
03/31/09   adw     Cleaned up includes.
03/25/09   adw     Removed unnecessary customer.h include.
03/19/09   adw     Include deint_v.h for deint_rc_type.
03/18/09   adw     Removed unnecessary includes.
03/17/09   tjc     Add function so MUX can report if we are in a data call
02/10/09   adw     Merge from 2h08 branch, based on VU_MODEM_1X_SRCH.13.00.01
11/19/08   adw     Merge from CMI sandbox, based on VU_MODEM_1X_SRCH.12.00.13
03/24/06   kwo     Rename frame ser to frame sec
03/22/06   kwo     Added srch_mux_idle_frame_stats()
09/08/05   ejv     Add a host of new interface functions for MUX.
07/19/05   grl     Added srch_mux_owns_tx call for mux to call.
05/24/05   ejv     Added srch_get_fing_rssi from srch.h.
05/04/05   kwo     Added srch_mux_page_match()
11/05/04   ejv     Added srch_mux_finger_finder.
10/25/04   ejv     Removed unused srch_mux_update_sch_walsh_len.
07/24/03   ejv     Implementation, first cut.
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"

/* Cp */
#include "cai_v.h"

/* Drivers */
#include "dec5000_v.h"
#include "deint_i.h"
#include "m1x_stmr_i.h"

/* MUX */
#include "rxctraffic.h"

/* Srch */
#include "srch_rx.h"
#include "ulpn.h"

/* Other */
#include "rfm_device_types.h"
#include "lm_types.h"

/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/

#ifdef FEATURE_MODEM_1X_DRX
typedef enum
{
  CGPS_DRX_DISABLE,
  CGPS_DRX_ENABLE,
  DIV_DRX_DISABLE,
  DIV_DRX_ENABLE,
  FINGER_DRX_DISABLE,
  FINGER_DRX_ENABLE,
  TIME_DRX_DISABLE,
  TIME_DRX_ENABLE,
  TRAFFIC_EXIT_DRX_DISABLE,
} drx_state_type;
#endif /* FEATURE_MODEM_1X_DRX */

/* This is used to find out which 20ms frame the combiner is currently in*/
typedef enum
{
         FRAME_0_MS,
         FRAME_20_MS,
         FRAME_40_MS,
         FRAME_60_MS,
         FRAME_NOT_AVAILABLE

} srch_mux_20ms_frame_type;

#ifdef FEATURE_IS2000_1X_ADV
typedef enum {

  SRCH_MUX_DECODE_DONE_IRQ,
  SRCH_MUX_LOG_PENDING_IRQ,
  SRCH_MUX_NUM_IRQ
} srch_mux_mdsp_irq_type;

typedef void (*srch_mux_irq_handler_type) (void);
#endif /* FEATURE_IS2000_1X_ADV */

/* Params needed for 1x2GTA during SMS/MMS */
typedef struct
{
  boolean is_non_dds_sub; /* Is 1x on DDS sub or not */
  boolean is_sms_call;    /* Is current SO is SMS or not */
  boolean is_data_call;   /* Is current SO is DATA SO */
  boolean is_tdso_call;   /* Is current SO is TDSO or not */
} srch_mux_so_dds_type;

extern srch_mux_so_dds_type srch_mux_so_dds_info;



/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION       SRCH_MUX_FINGER_FINDER
DESCRIPTION    This function takes a pilot PN and returns which fingers
               are assigned to this pilot

DEPENDENCIES   None

RETURN VALUE   Bit mask of the fingers assigned to this pilot,
               ie. if bit0 is set, then finger0 is assigned
SIDE EFFECTS   None

===========================================================================*/
extern uint8 srch_mux_finger_finder
(
  uint16 pilot_pn  /* Pilot PN to check on */
);

/*===========================================================================

FUNCTION       SRCH_MUX_SCOMB_GET_REF_TIME
DESCRIPTION    This function reads the symbol combiner's time reference
               register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   The 16-bit time reference register value.  Note that this
               value is not exactly binary.  It is binary encoded, but
               its maximum value is 0xBFFF.  Each LSB is two PN chips.
               The top two bits are the 26.666... ms phase within an
               80ms superframe.
SIDE EFFECTS   The time reference latch-on-micro-write register is
               updated.

===========================================================================*/
extern uint16  srch_mux_scomb_get_ref_time( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_CURR_20MS_FRAME
DESCRIPTION    This function returns the appropriate 20ms frame enum

DEPENDENCIES   None

RETURN VALUE   20ms frame enum
SIDE EFFECTS   None

===========================================================================*/
extern srch_mux_20ms_frame_type srch_mux_get_curr_20ms_frame( void );

/*===========================================================================

FUNCTION       SRCH_MUX_PAGE_MATCH
DESCRIPTION    This function is used to inform the idle state that
               a fast page match has occured.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_page_match
(
  boolean page_match_succ   /* flag whether page match succeesed or not */
);

/*===========================================================================

FUNCTION       SRCH_MUX_GET_FING_RSSI
DESCRIPTION    This function gets the RSSI of the best finger.
               This is intended to be called as a diagnostic tool.

DEPENDENCIES   None

RETURN VALUE   Word RSSI of best fing assigned and in lock.
               "-1" implies no fingers are in lock.
               "-2" implies no fingers were assigned.
SIDE EFFECTS   None

===========================================================================*/
extern int16 srch_mux_get_fing_rssi( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_PILOT_ECIO
DESCRIPTION    This function returns a byte representing the current
               Page Channel active pilot Ec/Io value.

DEPENDENCIES   srch_aset[0]

RETURN VALUE   The 6-bit Ec/Io value.  If the energy is out of range,
               the return value is clipped to 0 or 63. This is in
               1/2 db unit.
SIDE EFFECTS   None

===========================================================================*/
extern uint8 srch_mux_get_pilot_ecio( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_TX_DEVICE
DESCRIPTION    This function returns the Tx device.
DEPENDENCIES   None

RETURN VALUE   The RF Tx device in use.
SIDE EFFECTS   None

===========================================================================*/
extern rfm_device_enum_type srch_mux_get_tx_device( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_RX_DEVICE
DESCRIPTION    This function returns the Primary Rx device.
DEPENDENCIES   None

RETURN VALUE   The RF Rx device in use.
SIDE EFFECTS   None

===========================================================================*/
extern rfm_device_enum_type srch_mux_get_rx_device( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_ULPN_STATE
DESCRIPTION    This function reads the User Long PN Code state from
               the demod ASIC.

DEPENDENCIES   Calling this function asynchronously with the 26.666... ms
               boundary is dangerous.  If the 26.666... ms boundary passes
               while this function is reading the registers, a bogus value
               will be returned. Also, srch_dmod_reset should have been
               previously called,

RETURN VALUE   No formal return value,  but the User Long PN Code state
               is returned in a ulpn_type passed in by reference.  This
               state is the one that was valid at the last
               26.666... ms boundary.
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_get_ulpn_state
(
  ulpn_type   *ulpn  /* Pointer where to put the ULPN value read from demod */
);

/*===========================================================================

FUNCTION       SRCH_MUX_START_PC_STATE
DESCRIPTION    This function returns TRUE the first time it is called
               after Search enters PC state.

DEPENDENCIES   srch_state

RETURN VALUE   boolean indicating if this is first call (TRUE).
SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_mux_start_pc_state
(
  boolean reset_only
);

/*===========================================================================

FUNCTION       SRCH_MUX_GET_RX_AGC
DESCRIPTION    This function returns the current value of the rx agc register.
               For targets using MSM2 or higher, this is simply a passthru
               for RF CAGC function calls.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern uint8 srch_mux_get_rx_agc( void );

/*===========================================================================

FUNCTION       SRCH_MUX_GET_TX_ADJ
DESCRIPTION    This function returns the current value of the tx gain
               adjust register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern uint8 srch_mux_get_tx_adj( void );

/*===========================================================================

FUNCTION       SRCH_MUX_SET_TX_ADJ
DESCRIPTION    This function allows external tasks (namely the Transmit
               Task) to have access to the tx gain adjust register of
               the demod chip.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None
SIDE EFFECTS   Changes the contents of the tx gain adjust register of the
               demod chip.

===========================================================================*/
extern void  srch_mux_set_tx_adj
(
  uint8  tx_adj  /* 8-bit value to be written to the tx gain adjust reg */

  /* Note that this is a signed value. For maximum power, use the
     value -128 (0x80); for minimum power, use +127 (0x7F). */
);

/*===========================================================================

FUNCTION       SRCH_MUX_REPORT_FRAME
DESCRIPTION    This function tells search that there was a frame event, and
               whether frame was an erasure or not and if the frame is a
               guaranteed frame. It also passes information about the rate
               decision and raw metrics for FCH.

               NOTE - This is valid for FCH ONLY. DCCH is not
               handled in this function at present.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_report_frame
(
  boolean is_erasure,                    /* Frame was bad if TRUE, else good */
  boolean is_guaranteed,                 /* Frame is guaranteed or not       */
  uint16 cur_rate,                       /* Current rate decision on frame   */
  uint16 cur_rc,                         /* Current RC during frame          */
  const dec_fch_status_type *fch_metrics /* Energy and SER metrics for frame */
);

/*===========================================================================
FUNCTION       SRCH_MUX_REPORT_DATA_CALL
DESCRIPTION    This function tells search whether the current SO specifies a
               data call is in progress.Also, if data call is TDSO or not.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_report_data_call
(
  srch_mux_so_dds_type so_dds_info
);

#ifdef FEATURE_MODEM_1X_TA_MCVS
/*===========================================================================

FUNCTION       SRCH_MUX_IS_DATA_SO
DESCRIPTION    This function tells search whether the current SO specifies a
               data call is in progress.

DEPENDENCIES   None

RETURN VALUE   True if data SO, FALSE otehrwise
SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_mux_is_data_so( void );

/*===========================================================================

FUNCTION       SRCH_MUX_SET_DATA_SO
DESCRIPTION    This function tells search whether the current SO specifies a
               data call is in progress.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_set_data_so
(
  boolean is_data_so
);
#endif /* FEATURE_MODEM_1X_TA_MCVS */
/*===========================================================================

FUNCTION       SRCH_MUX_NOTIFY_FPC_MODE
DESCRIPTION    This function tells search that FPC is enabled.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_notify_fpc_mode
(
  boolean fpc_enabled,    /* FPC on-off status */
  uint16 mode             /* Mode passed as a word for logging */
);

/*===========================================================================

FUNCTION       SRCH_MUX_UPDATE_RC
DESCRIPTION    This function updates the srch's local copy of the FCH RC
               value which is necessary for logging.  This function is
               called from the MUX layer where the RC value is updated.

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_update_rc
(
  deint_rc_type  fch_rc_val,    /* Fundamental channel RC value */
  deint_rc_type  sch0_rc_val,   /* Supplemental channel 0 RC value */
  deint_rc_type  sch1_rc_val,   /* Supplemental channel 1 RC value */
  deint_rc_type  dcch_rc_val    /* Dedicated Control channel RC value */
);

/*===========================================================================

FUNCTION       SRCH_MUX_LOG_AGC_PCTL
DESCRIPTION    This function is invoked to log AGC and power control values
               for DIAG.

DEPENDENCIES   srch_agc_pctl_vars

RETURN VALUE   None
SIDE EFFECTS   Messages can be sent to DIAG task.

===========================================================================*/
extern void srch_mux_log_agc_pctl( void );

/*===========================================================================

FUNCTION       SRCH_MUX_IDLE_FRAME_STATS
DESCRIPTION    This function saves off the frame stats for logging

DEPENDENCIES   None

RETURN VALUE   None
SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_idle_frame_stats
(
  cai_rx_channel_id_type chan,
  uint16  sec,
  uint32  energy,
  boolean crc
);

/*===========================================================================

FUNCTION       SRCH_MUX_GET_ACTIVE_CELL_SIZE
DESCRIPTION    This function returns the number of unique power control bits
               received at the mobile station.

DEPENDENCIES   The mobile must be in the traffic state.

RETURN VALUE   The active cell size.
SIDE EFFECTS   None.

===========================================================================*/
extern word srch_mux_get_active_cell_size( void );

#ifdef FEATURE_IS2000_1X_ADV
/*===========================================================================

FUNCTION       SRCH_MUX_INSTALL_MDSP_ISR_HANDLER
DESCRIPTION    Function for 1x MUX to install 1xA-related MDSP ISR handler
               capability

DEPENDENCIES   Should be called ONLY ONCE upon startup.

RETURN VALUE   None
SIDE EFFECTS   None.

===========================================================================*/
extern void srch_mux_install_mdsp_isr_handler( void );

/*===========================================================================

FUNCTION      SRCH_MUX_NOTIFY_1X_ADV_TRAFFIC

DESCRIPTION   This function notifies SRCH that we are in 1x Advanced traffic.

DEPENDENCIES  srch_pwr_init() must have been called previously.

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_mux_notify_1x_adv_traffic
(
  boolean is_1x_adv_traffic      /* mobile is in 1x Adv traffic */
);
#endif /* FEATURE_IS2000_1X_ADV */

/*===========================================================================

FUNCTION      SRCH_MUX_GET_BAND_CHAN

DESCRIPTION   This function returns the current band and channel for the
              chain that 1x is currently active on.

DEPENDENCIES  None

RETURN VALUE  band and channel are returned through *band and *channel.
              boolean value returns false if 1x does not own any RF chains.

SIDE EFFECTS  None

===========================================================================*/
extern boolean srch_mux_get_band_chan
(
  uint16         *band,          /* returned band class */
  uint16         *channel        /* returned channel number */
);

/*===========================================================================

FUNCTION       SRCH_MUX_MDSP_DEMOD_RUNNING
DESCRIPTION    Wrapper to call srch_mdsp_demod_running(), which
               Checks if the demod is running on the DSP
DEPENDENCIES   None

RETURN VALUE   TRUE if demod is running; FALSE otherwise
SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_mux_mdsp_demod_running(void);

/*===========================================================================

FUNCTION      SRCH_MUX_NOTIFY_PAGE_DECODE

DESCRIPTION   This function notifies searcher that there was a failed
              paging decode attempt.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_mux_notify_page_decode
(
  boolean success
);

/*===========================================================================

FUNCTION      SRCH_MUX_CLEAR_PAGE_DECODE_CNT

DESCRIPTION   This function clears the page decode counters.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_mux_clear_page_decode_cnt( void );

/*===========================================================================

FUNCTION      SRCH_MUX_GET_PAGE_DECODE_CONF

DESCRIPTION   This function gets the confidence of current channel conditions
              based on the page decode counters. The failure count, counts the
              number of failures since the last page decode success. By
              default the confidence threshold is 10, which is about 200 ms
              of bad page decode following a good page decode. This is the
              threshold where confidence is lost.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern boolean srch_mux_get_page_decode_conf( void );

/*===========================================================================

FUNCTION      SRCH_MUX_GET_RTC_LCSTATE_AT_SYNC80

DESCRIPTION   This function returns the rtc and long code state at the last
              sync80 boundary.

DEPENDENCIES  None

RETURN VALUE  Implicitly through the passed in pointer

SIDE EFFECTS  None

===========================================================================*/
extern void srch_mux_get_rtc_lcstate_at_sync80
(
  boolean  ta_is_enabled,            /* TA Status                          */
  uint32   *rtc,                     /* RTC at last sync80 boundary        */
  uint64   *lcstate                  /* long code state at last sync80     */
);

/*===========================================================================

FUNCTION SRCH_MUX_GET_COMBINER_DELAY_CX8

DESCRIPTION    This function returns the combiner delay (offset beteen finger
               and combiner) in chipx8.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern uint32 srch_mux_get_combiner_delay_cx8( void );

/*=============================================================================

FUNCTION       SRCH_MUX_GET_ASET_SIZE

DESCRIPTION    Get the Active Set size.

DEPENDENCIES   None

RETURN VALUE   ASET size.

SIDE EFFECTS   None

=============================================================================*/
extern uint8 srch_mux_get_aset_size( void );

/*=============================================================================

FUNCTION       SRCH_MUX_GET_FING_COUNT

DESCRIPTION    This function return total fingers (ASET+NSET)

DEPENDENCIES   None

RETURN VALUE   ( ASET + NSET )size.

SIDE EFFECTS   None

=============================================================================*/
extern uint8 srch_mux_get_fing_count( void );

/*===========================================================================

FUNCTION      SRCH_MUX_WTR_HOP_IN_PROGRESS

DESCRIPTION   This function returns if wtr hop is in progress.

DEPENDENCIES  None.

RETURN VALUE  TRUE  If HOP is in Progress.
              FALSE Otherwise

SIDE EFFECTS  None.

===========================================================================*/
extern boolean srch_mux_wtr_hop_in_progress( void );

/*=============================================================================

FUNCTION       SRCH_MUX_IS_RX_DIV_ENABLED

DESCRIPTION    This function check's if RxD is enabled or not.

DEPENDENCIES   None

RETURN VALUE
  TRUE  - If RxD is enabled.
  FALSE - If RxD is disabled.

SIDE EFFECTS   None

=============================================================================*/
extern boolean srch_mux_is_rx_div_enabled( void );

#ifdef FEATURE_MODEM_1X_DRX
/*===========================================================================

FUNCTION       SRCH_MUX_DRX_READ_NV_ITEM

DESCRIPTION    This function reads the NV values for DRX enable/disable

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_drx_read_nv_item( void );

/*===========================================================================

FUNCTION       SRCH_MUX_UPDATE_DRX_STATE

DESCRIPTION    This function is called to enable or disable 1x/1xA DRX feature.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_update_drx_state
(
  drx_state_type drx_state_update
);

/*===========================================================================

FUNCTION       SRCH_MUX_DRX_RESET_PARAMS

DESCRIPTION    This function is called during traffic exit to
               reset the DRX params

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_mux_drx_reset_params( void );

/*===========================================================================

FUNCTION       SRCH_MUX_CHANGE_DRX_STATE

DESCRIPTION    This function is called to enable or disable 1x/1xA DRX feature.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void srch_mux_change_drx_state
(
  boolean drx_is_enabled
);

/*===========================================================================

FUNCTION       SRCH_MUX_DRX_SKIP_SRCH_START

DESCRIPTION    This function is called to check if DRX is disabled due to finger
               are disabled

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_mux_drx_skip_srch_start(void);
#endif /* FEATURE_MODEM_1X_DRX */

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF
/*=============================================================================
FUNCTION       SRCH_MUX_SET_TXLM_HANDLE

DESCRIPTION    This function sets TXLM handle used by TX task

DEPENDENCIES   None

RETURN VALUE    None

SIDE EFFECTS   None
=============================================================================*/
extern void srch_mux_set_txlm_handle( lm_handle_type txlm_hanlde );

/*=============================================================================
FUNCTION       SRCH_MUX_GET_TXLM_BUFFER_HANDLE

DESCRIPTION    This function returns TXLM handle used by TX task

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None
=============================================================================*/
extern void srch_rx_get_txlm_buffer_handle( lm_handle_type *txlm_buf );
#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */
#endif /* SRCH_MUX_H */
