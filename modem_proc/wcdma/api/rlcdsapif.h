#ifndef RLCDSAPIF_H
#define RLCDSAPIF_H
/*===========================================================================
                  R L C - D A T A S A P  I N T E R F A C E

                         H E A D E R  F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  RLC - DSAP Interface.

Copyright (c) 2001 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2002-2004, 2006, 2008-2009 Qualcomm Technologies, Inc. 
All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcdsapif.h_v   1.9   08 May 2002 19:13:48   tliou  $
$Header: //components/rel/wcdma.mpss/8.4.0/api/rlcdsapif.h#3 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/02/15   ac      FR 26288: DS downlink quality measurement reporting
03/19/15   ac      FR 25733: UL/DL throughput reporting 
12/11/13   gp      FR:3693 EUL stats API for data services
01/04/13   kc      Removing duplicate/redundant code
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
04/26/12   grk     Added new API file exposing L2 command structures
02/01/12   grk     Feature cleanup
01/17/11   vp      Moved the file from api to rlc\inc
10/18/10   ssg     Added changes for new WCDMA-L2 interface.
05/11/09   ssg     Updated Copyright Information
03/01/09   rm      Added new featurization for FEATURE_CMI.
11/10/06   vg      added structure for WM cleanup cmd for PDCP.
04/12/06   ttl     Give the failure reason of rlc_am_sdu_cnf_cb().
01/20/04   mv      Replaced the FEATURE_RLC_ENHANCED_CB with the
                   FEATURE_RLC_SDU_RCVD_ENHANCED_CB.
12/29/03   vsk     Replace FEATURE_MULTIPLE_PDP with FEATURE_RLC_ENHANCED_CB
09/23/03   mv      Added a third parameter to the rlc_post_rx_func_ptr()
                   to support multiple PDP contexts under the feature
                   FEATURE_MULTIPLE_PDP.
05/08/02   ttl     Changed the function prototype of rlc_post_rx_proc_func_ptr().
02/13/02   ttl     Added service de-registration function.
11/04/01   ttl     Removed application/voice service callback function feature.
10/26/01   ttl     Added application/voice service callback function feature.
08/30/01   rc      Fixed some comments.
06/18/01   rc      Added support for Processing Downlink TM PDUS in Task or
                   interrupt context.
05/21/01   rc      Cleaned up code.
03/23/01   ttl     Added callback function type for SDU confirmation and SDU
                   discard.
01/08/01   rc      Modified code to use the data types defined in uecomdef.h.
11/22/00   rc      Created file.

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "uecomdef.h"
#include "dsmutil.h"
#include "dsm.h"
#include "queue.h"
#include "wl2api.h"

/* ----------------------------------------------------------------------- */
/* Private Section.                                                        */
/* ----------------------------------------------------------------------- */

typedef struct 
{
  /* Allowed data rate based on minimum of Grant and power */
  uint32 allowed_datarate;

  /* Achievable data rate based on power */
  uint32 achievable_power_limited_datarate;

  /* Indication to higher layer to increase the data rate to utilize maximum
   * EUL channel capacity  */
  boolean increase_data_rate_flag;
}wl1_eul_stats_struct_type;

typedef enum {
  WCDMA_MODEM_TX_POWER_LOW = 0,
  WCDMA_MODEM_TX_POWER_MEDIUM,
  WCDMA_MODEM_TX_POWER_HIGH
} wl1_ul_modem_tx_power_enum_type;

typedef struct
{
  /* Available throughput in kbps based on CQI */
  int32 available_throughput;
} wl1_hsdpa_stats_struct_type;

/* Quality estimation reporting states used by
   wcdma_qual_estimation_change_report_state() */
typedef enum
{
  STOP_REPORTING,
  START_REPORTING,
} wcdma_qual_estimation_report_state;

/* Quality estimation report data passed to DS periodically */
typedef struct
{
  /* Estimated potential throughput in kbps*/
  uint16  dl_rate;

  /* Confidence level of estimation (1-7) */
  uint8   confidence_level; 

  /* Set to indicate this report should be discarded */
  boolean data_freeze;

  /* Set to indicate reporting is being stopped by WL1. DS should not expect
     any more periodic reports until WL1 sends the next report. */
  boolean move_out;

} wcdma_qual_estimation_report_data;

/*===========================================================================
FUNCTION   WL1_EUL_STATS

DESCRIPTION
  This function provides allowed data rate based on min of serving grant and power.
  It also provides indication to DS to increase data rate for ultilizing max channel
  capacity. If EUL call is not ON, Invalid data rate is reported and indication to 
  increase data rate is FALSE.

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE indicates values are valid(EUL call is ON)
 
SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_eul_stats(wl1_eul_stats_struct_type *eul_stats);

/*===========================================================================
FUNCTION   wl1_start_tput_data_reporting_ind

DESCRIPTION
  This function indicates to L1 when it must start accumulation of achievable data rate
  on EUL based on power.

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_start_tput_data_reporting_ind(void);

/*===========================================================================
FUNCTION   wl1_stop_tput_data_reporting_ind

DESCRIPTION
  This function indicates to L1 when it must stop accumulation of achievable data rate
  on EUL based on power.

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_stop_tput_data_reporting_ind(void);


/*===========================================================================
FUNCTION        WL1_QUERY_MODEM_TX_POWER 

DESCRIPTION
   This function computes Tx Modem power in IDLE/FACH and connected state as
   LOW, MEDIUM or HIGH.

DEPENDENCIES
  None

RETURN VALUE
  wl1_ul_modem_tx_power_enum_type: Modem power is HIGH/MEDIUM or LOW.
 
SIDE EFFECTS
  None
===========================================================================*/
extern wl1_ul_modem_tx_power_enum_type wl1_query_modem_tx_power( void );

/*===========================================================================
FUNCTION wl1_get_hsdpa_stats

DESCRIPTION
  This function populates a hsdpa_stats structure with the current
  HSDPA statistics.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if data is valid. FALSE otherwise (e.g. if HSDPA is disabled).

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_get_hsdpa_stats(wl1_hsdpa_stats_struct_type *hsdpa_stats);

/*===========================================================================
FUNCTION wcdma_qual_estimation_set_report_period

DESCRIPTION
  Sets the DL quality reporting period to the given value. If the passed-in
  period is out of the allowed range then the nearest allowed value will be
  used instead. This API must be called before reporting is started.
  
DEPENDENCIES
  None

RETURN VALUE
  0 if reporting feature is not present on this build
  Else the actual reporting period (in ms) used by WL1. May be different than the
  value passed in.

SIDE EFFECTS
  None
===========================================================================*/
extern uint16 wcdma_qual_estimation_set_report_period(uint16 report_period,        /* Requested reporting period in ms*/
                                                      sys_modem_as_id_e_type as_id /* AS ID for this request */
                                                      );

/*===========================================================================
FUNCTION wcdma_qual_estimation_change_report_state

DESCRIPTION
  Changes the quality estimation reporting state. Periodic reporting will be
  started once START_REPORTING command is sent at a rate determined by the
  last call to wcdma_qual_estimation_set_report_period(). Periodic reporting
  will be immediately stopped once the STOP_REPORTING command is sent.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if the requested state change was performed
  FALSE if current reporting state is the same as the requested state
  FALSE if requesting START but reporting period was not set
  FALSE if reporting feature is not present on this build
        

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wcdma_qual_estimation_change_report_state(wcdma_qual_estimation_report_state state,  /* Requested reporting period in ms*/
                                                         sys_modem_as_id_e_type as_id               /* AS ID for this request */
                                                         );

#endif
