#ifndef HDRSRCHAFC_H
#define HDRSRCHAFC_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         H D R   S E A R C H   A U T O M A T I C   F R E Q U E N C Y
                   C O N T R O L   F U N C T I O N S

GENERAL DESCRIPTION

  This module provides routines for handling frequency error via FLL and
  phase rotators.

EXTERNALIZED FUNCTIONS (Global)
  
  hdrsrchafc_init
  hdrsrchafc_start_fll_acq
  hdrsrchafc_stop_fll_tracking
  hdrsrchafc_service_request_pending
  hdrsrchafc_afc_uses_rot
  hdrsrchafc_fll_rot_update_diversity
  hdrsrchafc_enable_rot
  hdrsrchafc_disable_rot
  hdrsrchafc_afc_event
  hdrsrchafc_set_tap
  hdrsrchafc_request_fast_acq

EXTERNALIZED FUNCTIONS (Regional)


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2004 - 2014
                by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchafc.h#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
--------     ---     ---------------------------------------------------------
06/09/2014   ljl     Added HDRSRCH_AFC_ENABLE_RGS_REPORT_CMD.
09/27/2012   ljl     Added hdrsrchafc_update_rgs().
08/31/2011   smd     Exposed hdrsrchafc_xo_disable_mnd. 
07/26/2011   smd     Exposed hdrsrchafc_xo_enable_mnd.
03/10/2011   ljl     Supported XO acquisition range improvement.
03/03/2011   vlc     Added hdrsrchafc_config_pll().
06/29/2010   vlc     Added hdrsrchafc_request_fast_acq ().
06/04/2010   smd     Exposed hdrsrchafc_send_rot_override_msg().
04/26/2010   lyl     Added brute force acquisition support.
03/25/2010   pxu     Removed FEATURE_HDRSRCH_ROTATORS feature checks. 
11/09/2009   ljl     Passed fll lock status from acq response to AFC stm.
06/30/2009   lyl     Updated for generic responses process.
05/18/2009   ljl     Updated for firmware MSGR interface.
05/14/2008   ljl     Added hdrsrchafc_disable_rot_div().
04/28/2008   ljl     Added hdrsrchafc_get_tap_status().
12/18/2007   rkc     Added RevB multi-carrier changes.
07/07/2007   ljl     Supported rotator push.
06/20/2007   rkc     Added hdrsrchafc_xo_cal_mode_enum_type
03/28/2007   rmg     Added hdrsrchafc_override_nv().
11/16/2006   ljl     Added hdrsrchafc_set_chan().
10/17/2006   ljl     Added HDR SRCH AFC stm.
05/22/2006   ljl     Added support for FLAT.
04/24/2004   ljl     Added supported of TCXOMgr 2.1.
04/09/2006   grl     Added API to indicate an AFC service request is pending
10/25/2005   ds      Changed input parameter for hdrsrchafc_init_fll()
09/06/2005   ds      Added support for Rotator Based Acq and Track
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR. Added new init function.
05/31/2005   grl     Added support for TCXOMgr.
03/31/2005   grl     Created module

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "comdef.h"
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "tcxomgr.h"
#include "hdrsrchsm.h"


/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
typedef int32 fll_accum_type;  /* FLL VCO and rotator Accum data type */

/* AFC Acquisition type */
typedef enum
{
  HDRSRCHAFC_VAFC_ACQ,
    /* VCO pullin */

  HDRSRCHAFC_RAFC_ACQ
    /* RAFC pullin */
}
hdrsrchafc_acq_type;

/* Possible tcxo_states requested by TCXOMgr. */
typedef enum
{
  HDRSRCHAFC_WAITING_FOR_TCXO_OPEN_LOOP ,   
    /* TCXO in open loop mode */

  HDRSRCHAFC_WAITING_FOR_TCXO_CLOSE_LOOP,  
    /* TCXO tracking with closed loop */

  HDRSRCHAFC_WAITING_FOR_TCXO_CLOSE_LOOP_TRACKING

}
hdrsrchafc_req_tcxo_state_enum_type;

/* Possible tcxo_states granted by TCXOMgr. */
typedef enum
{
  HDRSRCHAFC_TCXO_NO_CONTROL  = 0x1,  
    /* Active Client not controlling tcxo */

  HDRSRCHAFC_TCXO_OPEN_LOOP   = 0x2,   
    /* TCXO in open loop mode */

  HDRSRCHAFC_TCXO_TRACK_FAST  = 0x4,  
    /* TCXO tracking with fast or no restriction */

  HDRSRCHAFC_TCXO_TRACK_SLOW  = 0x8,  
    /* TCXO tracking with slow restriction */

  HDRSRCHAFC_TCXO_TRACK_SRL   = 0x10,
    /* TCXO tracking with Slew rate limiting (SRL) restriction */

  HDRSRCHAFC_TCXO_FREEZE      = 0x20,      
    /* TCXO PDM Frozen */

  HDRSRCHAFC_TCXO_CLOSE_LOOP  = 0x40 
    /* Closing Loop */
}
hdrsrchafc_tcxo_state_enum_type;


/* The possible requests for TCXO permission supported. */
typedef enum
{
  HDRSRCHAFC_TCXO_REQ_NO_CONTROL,    
    /* Request no control over the TCXO */

  HDRSRCHAFC_TCXO_REQ_OPEN_LOOP,      
    /* Request open loop control over the TCXO */

  HDRSRCHAFC_TCXO_REQ_CONTINUE_TRACKING,
    /* Request ability to continue tracking where we left off.  No
       drastic movements of the TCXO will occur. */

  HDRSRCHAFC_TCXO_REQ_CLOSE_LOOP,
    /* Request the ability to move the TCXO in any way even if it temporarily
       increases frequency error. */

  HDRSRCHAFC_TCXO_NUM_REQS
    /* The total number of request options. */
}
hdrsrchafc_tcxo_service_req_enum_type;

/*----------------------------------------------------------------------*
                      hdrsrchafc_antenna_select_type
 *----------------------------------------------------------------------*/
/* Enum type  to select antennas for FLL rotator */
typedef enum
{
  HDRSRCHAFC_ANT0, /* Antenna 0 */
  HDRSRCHAFC_ANT1  /* Antenna 1 */
} 
hdrsrchafc_antenna_select_type;   /* Antenna selections for FLL rotator */

/*----------------------------------------------------------------------*
                      hdrsrchafc_rvafc_bandwidth_type
 *----------------------------------------------------------------------*/
/* Enum type  to select RVAFC bandwidth mode */
typedef enum
{
  HDRSRCHAFC_RVAFC_CONST_BANDWIDTH,  /* Constant RVAFC outer loop bandwidth */
  HDRSRCHAFC_RVAFC_VAR_BANDWIDTH     /* Variable RVAFC outer loop bandwidth */
} 
hdrsrchafc_rvafc_bandwidth_type;     /* RVAFC bandwidth mode enum type */


/*----------------------------------------------------------------------*
                      hdrsrchafc_rvafc_bandwidth_type
 *----------------------------------------------------------------------*/
/* Enum type to choose between default and high DLL Gains and drift limits */
typedef enum   
{
  HDRSRCHAFC_DLL_PARAMS_DEFAULT,      /* Default gains, drifts for DLL */
  HDRSRCHAFC_DLL_PARAMS_HIGH          /* Increased gains, drifts for DLL*/
} hdrsrchafc_dll_params_enum_type;    /* DLL paramters enum type */

/*----------------------------------------------------------------------*
                      hdrsrchafc_rot_error_thresh_type
 *----------------------------------------------------------------------*/
/* Enum type to choose between high and low rotator error thresholds */
typedef enum   
{
 HDRSRCHAFC_ROT_ERR_THRESH_HIGH,   /* Higher rotator error threshold */
 HDRSRCHAFC_ROT_ERR_THRESH_LOW,    /* Lower rotator error threshold */
 HDRSRCHAFC_ROT_ERR_THRESH_SRL     /* Threshold below which SRL can be used */
} hdrsrchafc_rot_error_thresh_type;      /* Rotator error threshold enum type */

/*----------------------------------------------------------------------*
                      hdrsrchafc_xo_cal_mode_enum_type
 *----------------------------------------------------------------------*/
#ifdef FEATURE_XO

/* Modes supported for Freq/Temp information Accumulator indicator values
   from tcxomgr.  Not Calibrated, RGS - Recent Good System,
   FTx - Freq/Temp table */
typedef enum
{
  HDRSRCHACQ_NOT_CAL,
  HDRSRCHACQ_RGS_MODE,
  HDRSRCHACQ_FT1_MODE,
  HDRSRCHACQ_FT2_MODE,
  HDRSRCHACQ_FT_WIDE_MODE
}
hdrsrchafc_xo_cal_mode_enum_type;
  /* Supported XO modes determined from TXCOMGR tcxomgr_vco_indicator_type */
#endif /* FEATURE_XO */

/* Structure to define an HDR channel
*/
typedef struct
{
  boolean                     enable;
    /* True if channel is enabled/used */

  sys_channel_type            chan;
    /* Channel info */  

} hdrsrchafc_carrier_type;

/* Multi-link channel info */
typedef struct
{

  hdrsrchafc_carrier_type     carrier[HDRSRCH_MAX_NUM_CARRIERS];
    /* List of HDR carrier/channel info */

}
hdrsrchafc_chan_list_type;

/*===========================================================================

                      FUNCTION  DECLARATIONS

===========================================================================*/


/*=========================================================================

FUNCTION HDRSRCHAFC_INIT

DESCRIPTION
  This function initializes the HDR SRCH AFC and then registers HDR 
  with the TCXOMgr.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
=========================================================================*/

extern void hdrsrchafc_init( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_OVERRIDE_NV

DESCRIPTION
  This function overwrites hdrsrchafc_nv_ctrl with the settings prescribed
  by nv_override.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  hdrsrchafc_nv_ctrl is overwritten.
  
===========================================================================*/

extern void hdrsrchafc_override_nv
(
  uint8                     nv_override
    /* Arriving command */
);

/*============================================================================

FUNCTION HDRSRCHAFC_REPORT_CLOSE_LOOP_COMPLETED

DESCRIPTION
  This function reports the close loop status of HDR in the event we 
  transition from trying to close loop to tracking.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  A TCXOMgr command may be sent to one or more clients.

============================================================================*/

extern void hdrsrchafc_report_close_loop_completed( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_GET_TCXO_STATE

DESCRIPTION
  This function returns the current TCXO state as negotiated with TCXOMgr.

DEPENDENCIES
  None

RETURN VALUE
  The current TCXO state as negotiated with TCXOMgr.
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern hdrsrchafc_tcxo_state_enum_type hdrsrchafc_get_tcxo_state( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_CAN_TRACK_FREQ_ERROR

DESCRIPTION
  This function returns whether HDR can, given the hardware and permission
  granted by TCXO, track frequency error.

DEPENDENCIES
  None

RETURN VALUE
  Whether HDR can, given the hardware and permission granted by TCXO, track
  frequency error.
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern boolean hdrsrchafc_can_track_freq_error( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_SERVICE_REQUEST_PENDING

DESCRIPTION
  This function returns whether the searcher has an outstanding service
  request (via tcxomgr) pending. 

DEPENDENCIES
  None

RETURN VALUE
  Whether the searcher has an outstanding service request (with TCXOMgr) 
  pending. 
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern boolean hdrsrchafc_service_request_pending( void );


/*==========================================================================

FUNCTION        HDRSRCHAFC_AFC_EVENT

DESCRIPTION
  Called on AFC timer expiry.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

extern void hdrsrchafc_afc_event( void );



/*===========================================================================

FUNCTION HDRSRCHAFC_AFC_USES_ROT

DESCRIPTION
  Checks if the rotator is being used for AFC.

DEPENDENCIES
  None.
  
RETURN VALUE
  TRUE if rotator(s) to be used, FALSE if rotator(s) not to be used.

SIDE EFFECTS
  None
  
===========================================================================*/

extern boolean hdrsrchafc_afc_uses_rot( void );

/*=========================================================================

FUNCTION     : HDRSRCHAFC_FLL_ROT_UPDATE_DIVERISTY

DESCRIPTION  : This function updates the diversity settings for FLL rotators.
               
DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

extern void  hdrsrchafc_fll_rot_update_diversity
(
  boolean div_flag               /* diversity control flag */
);



/*==========================================================================

FUNCTION        HDRSRCHAFC_PULLIN_FAILED

DESCRIPTION
  This function restores VCO to a good value if VCO pullin failed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void hdrsrchafc_pullin_failed( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_SET_FLL_CHAN

DESCRIPTION
  Sets up channel information for FLL.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrsrchafc_set_fll_chan
( 
  sys_channel_type             channel 
    /* Which channel HDR is currently tuned to. */
);

/*=========================================================================

FUNCTION     : HDRSRCHAFC_SET_TAP

DESCRIPTION  : This function is called if any TAP application is running. 

DEPENDENCIES : None

PARAMETERS   : 

  tap  - whether TAP is running.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchafc_set_tap
(
  boolean                    tap
    /* Whther tap is running */
);


/*===========================================================================

FUNCTION HDRSRCHAFC_REQUEST_OPEN_LOOP

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to request open loop service.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_request_open_loop( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_REQUEST_CLOSE_LOOP

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to request close loop service for acquisition.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_request_close_loop( void );

/*===========================================================================

FUNCTION HDRSRCHAFC_START_ACQ

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to start AFC acquisition.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_start_fll_acq
(
  int16            freq_offset_ppb
    /* Freq offset in ppb */
);


/*===========================================================================

FUNCTION HDRSRCHAFC_STOP_FLL_TRACKING

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to stop FLL tracking.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_stop_fll_tracking( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_DEACTIVATE

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to deactivate AFC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_deactivate( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_REUEST_COMTINUE_TRK

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to request close loop service to continue AFC tracking.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_request_continue_trk( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_SET_CHAN

DESCRIPTION
  This function sends a command to the HDR SRCH AFC state machine 
  to set channel info.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_set_chan
(
  hdrsrchafc_chan_list_type     *chan_list
    /* Channel list info. */
);


/*===========================================================================

FUNCTION HDRSRCHAFC_ROTATOR_PUSH

DESCRIPTION
  This function sends a command to HDR SRCH AFC state machine 
  to push rotator samples.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_rotator_push( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_UPDATE_RGS

DESCRIPTION
  This function sends a command to HDR SRCH AFC state machine 
  to update RGS.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_update_rgs( void );


/*===========================================================================

FUNCTION HDRSRCHAFC_ACQ_RSP

DESCRIPTION
  This function sends a command to HDR SRCH AFC state machine 
  to for acquisition response.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_acq_rsp( uint16 fll_lock );


/*=========================================================================

FUNCTION     : HDRSRCHAFC_GET_TAP_STATUS

DESCRIPTION  : This function returns whether any TAP application is running. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchafc_get_tap_status( void );

/*===========================================================================

FUNCTION HDRSRCHAFC_REQUEST_FORCE_ROT_TRK

DESCRIPTION
  This function request AFC forcely enter rotator tracking mode. 
  Reuse RV_TRK_STATE here.

DEPENDENCIES
  None

RETURN VALUE
  RV tracking state

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_request_force_rot_trk( void );

/*===========================================================================

FUNCTION HDRSRCHAFC_CALC_CURR_ROT_VAL

DESCRIPTION
  This function caculates rotator value based on RGS and current VCO from LTE. 
  Use the equation below: 
    hdr_rot = rgs_rot + ( rgs_vco - current_vco )
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

fll_accum_type hdrsrchafc_calc_curr_rot_val ( void );


/*==========================================================================

FUNCTION HDRSRCHAFC_SEND_ROT_OVERRIDE_MSG

DESCRIPTION
  Send rotator override msg. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None 

==========================================================================*/

void hdrsrchafc_send_rot_override_msg
(
  boolean                    enable,
    /* Enable override or not */

  int32                     *value
    /* Override value */
);

/*===========================================================================

FUNCTION HDRSRCHAFC_REQUEST_FAST_ACQ

DESCRIPTION
  This function sends a command to HDR SRCH AFC state machine to request fast 
  acquisition mode which will eventually move to continue AFC tracking,
  similar to re-acq.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchafc_request_fast_acq( void );


#ifdef FEATURE_XO
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHAFC_ENABLE_MND

DESCRIPTION
  Pass the rx gain and tx/rx ratio to initialize the MND counter control. The 
  function should be called when FEATURE_XO is defined and everytime when the 
  primary channel is tuned.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  In tune_done( ) callback called

===========================================================================*/
void hdrsrchafc_xo_enable_mnd
(
  fll_accum_type       rot_value,
    /* Current rotator accum value to get the current freq error */

  sys_channel_type     channel
    /* Current rx channel info */
);


/*===========================================================================

FUNCTION HDRSRCHAFC_DISABLE_MND

DESCRIPTION
  The function disable the fw updating the mnd counter

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrsrchafc_xo_disable_mnd ( void );

#endif /* FEATURE_XO */


/*===========================================================================

FUNCTION  HDRSRHAFC_CONFIG_PLL

DESCRIPTION
  Config PLL and Th5. The function is only used for the TCXO technology 
  which uses the PLL to compensate the error in the frequency loop.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None.
  
===========================================================================*/

void hdrsrchafc_config_pll( void );

/*===========================================================================

FUNCTION HDRSRCHAFC_ENABLE_RGS_REPORT

DESCRIPTION
  This function enables RGS report after HDR passes sync state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchafc_enable_rgs_report( void );

#endif
