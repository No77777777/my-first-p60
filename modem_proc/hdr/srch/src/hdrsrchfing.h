#ifndef HDRSRCHFING_H
#define HDRSRCHFING_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R    F I N G E R   D R I V E R S

                          Finger Drivers Header File

GENERAL DESCRIPTION

   This file contains the databases, typedefs, enums and defines used by
   the HDR Rake Receiver Demodulation operation.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
                
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchfing.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
09/11/2018   vke     Reset all fingers to clear stray fingers if any
06/26/2018   vke     Changes to process slam response status from fw
04/25/2014   ljl     Supported finger assignment and slam message.
01/13/2010   ljl     Limit one finger per sector. 
12/14/2010   ljl     Limited the max fingers for revB on 9k.
05/04/2010   smd     Updated typo in HDRSRCHFING_DEMOD_CARRIER_0_MASK.
10/01/2009   kss     Remove include of mdspext.h
07/29/2009   etv     Included hdrfw_msg.h instead of hdrmsg.h
07/15/2009   etv     Moved sw module definitions to hdrmsg.h
06/30/2009   lyl     Updated for generic responses process.
05/18/2009   ljl     Updated for firmware MSGR interface.
04/09/2008   ljl     Added HDRSRCHFING_DEMOD_CARRIER_ENABLED().
10/17/2007   ljl     Merged Rev B changes from branch.
05/30/2007   ljl     Added hdrsrchfing_are_fings_locked().
12/11/2006   ljl     Added hdrsrchfing_are_fings_assigned().
09/06/2005   ds      Moved VERIFY_ACQ_FING_CNT from hdrsrchacq.c
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
08/30/2004   aaj     Changed finger assignment wait to 900ms from 1700ms
10/06/2003   aaj     Mainlined T_MSM6500
07/30/2003   sq      Added definition for HDR_NUM_FINGERS for T_MSM6500 
03/11/2003   ajn     Updated Copyright notice
02/26/2003   aaj     Changes to mps threshold logic 
02/19/2003   sq/aaj  Restore finger thresholds according to MPS flag
                     Added hdrsrchfing_mps_struct_type
01/30/2003   sq      Moved MPS related #defines to hdrsrchfing.c
01/21/2003   sq      Added support for MPS testing
04/29/2002   aaj     Added define for delay to wait for fing assignment
04/02/2002   aaj     Changed return type of jump_to_hyperspace function
01/09/2002   aaj     Change number of fingers from 8 to 12
08/02/2001   aaj     Antenna Diversity Changes for fingers
04/23/2001   aaj     Major changes to drivers to support TAZ3
03/26/2001   aaj     Added hdrsrchfing struct type to include all fing info
03/08/2001   ajn     Added disable all fingers function
02/28/2001   aaj     Changed max finger per pilot to 3 for non-div
11/22/2000   ajn     Code review changes
10/23/2000   ajn     Added VCS Header line
10/19/2000   ajn     Added finger/sector update function.
10/09/2000   ajn/aaj Added finger/sector deassign functions
09/25/2000   aaj     Added finger unlock counter update function
09/15/2000   aaj     Added finger driver init routine
08/10/2000   aaj     Removed fing_idx from the finger struct
07/20/2000   aaj     Removed path pointer from finger struct
06/27/2000   aaj     Compiled the code successfully
06/20/2000   aaj     Added function prototypes
06/09/2000   aaj     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "hdrsrchsect.h"
#include "hdrfw_msg.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/
/* HLD has an error. It has swapped defines for RX0 and RX1. Correct
   defines are RX0 = 0, RX1 = 1 */
#define HDR_FING_ANT0      0x0            /* select ant 0 ip for finger */
#define HDR_FING_ANT1      0x1            /* select ant 1 ip for finger */

#define HDR_FING_MAX_UNLOCK_AGE   165
/* As the triage is executed every 30ms (appox). The finger unlock/lock
  age is incremented every triage. The unlock timers are implemented using
  the triage rate. Number 165 corresponds to 165*30ms = 4.95 sec */

/*-------------------------------*
 * Fingers Count related defines *
 *-------------------------------*/

/*#define HDR_NUM_FINGERS                 8*/
  /* MSM6500 DSP image does not contain definition for number of fingers */

/* TBD */
/* Shpould we get the number of fingers from firmware? */
#define HDR_MAX_FINGS_CNT               8
  /* Total number of fingers supported by the demod dsp */

#define HDR_MAX_FINGS_PER_PILOT         4
  /* max fings/pilot in non-div. Limit it to 4. Can go up if necessary */

#define HDR_LIMITED_MAX_FINGS_CNT               2
  /* Total number of fingers supported by the demod dsp */

#define HDR_LIMITED_MAX_FINGS_PER_PILOT         1
  /* max fings/pilot in non-div. Limit it to 4. Can go up if necessary */

#define HDR_NO_FING            0xff       /* index of of "No valid finger" */
#define HDR_MAX_RSSI           0xffff     /* max possible value of fing rssi */


#define HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US   1000
  /* Wait for these many msec for finger assignment to take place */

#define HDRSRCH_MPS_PILOT_THRESH             0x18
  /* Pilot value to indicate whether we are in MPS testing (-12dB)      */

#define HDRSRCHFING_VERIFY_ACQ_FING_CNT              2
  /* Only 2 fingers used during Acq verify */

#define HDRSRCHFING_DEMOD_CARRIER_0_MASK             1
  /* Demod carrier 0 mask */

#define HDRSRCHFING_DEMOD_CARRIER_ENABLED( c ) \
          ( hdrsrchfing.params.demod_carrier_mask & ( 1 << ( c ) ) )
  /* To check whether a demod carrier is enabled */

/*========================================================================

                     MACROS FOR FINGER BIT FIELD SETTINGS

 ========================================================================*/


/*-------------------*
 * Finger Mask Macro *
 *-------------------*/
#define HDR_FING_MASK_ENABLE( f, var )          ( (var) |= (0x1 << (f)) )
/* Enables a particular finger bit in the variable passed to this macro */

/*-------------------*
 * Finger Lock Macro *
 *-------------------*/
#define HDR_FING_LOCKED( fptr ) ( (fptr)->modified ? TRUE : (fptr)->locked )
/* This macro returns the lock status of the given finger. If the finger
   is newly modified, it returns the finger status as LOCKED. */

/*---------------------*
 * Finger Assign Macro *
 *---------------------*/
#define HDR_FING_ASSIGNED( c, f ) \
     ( hdrsrchfing.fings[(c)][(f)].pn_offset != HDR_NO_PN_OFFSET ? TRUE : FALSE )
/* This macro returns the assignment status of a finger. If a finger is
   assigned then it returns TRUE else returns FALSE.  Triage uses
   PN_OFFSET field in the finger structure to indicate the assignment status
   of a finger */

/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/

/*----------------------------------------------------------------------*
                      hdrsrchfing_com_params_struct_type
 *----------------------------------------------------------------------*/

/* Finger parameters common to all the fingers */

typedef struct
{

  uint8   max_demod_carriers;
    /* Maximum number of demod carriers */

  uint8   max_fings;
   /* Maximum number of fingers */

  uint8   max_fings_pp;
   /* Maximum number of fingers per pilot */

  boolean default_ant;
    /* default finger antenna select */

  uint8   demod_carrier_mask;
    /* Mask indicating which carriers are enabled */

}
hdrsrchfing_com_params_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrchfing_fing_shadow_struct_type
 *----------------------------------------------------------------------*/

typedef struct
{
  boolean                     ant_sel;
   /* antenna select for finger 0: ANT0, 1: ANT1 */

  uint32                      rtc_offsetx8;
   /* the initial RTC offset to be programmed to the finger */

  byte                        mac_idx;
   /* MAC index of the finger. The DSP converts this to a walsh index for
      RPC and preamble detect */

  byte                        rpc_cell_idx;
   /* cell map for this finger. RPC symbols of the same cell are soft combined
      before thresholding and hard combining according to or-of-downs. This is
      a 3 bit value [0 to 6]. Each number corrsponds to a cell */

  byte                        asp_idx;
   /* active set pilot map for this finger. Ior/Nt estimates of multipaths of
      same active set pilots are combined to get total Ior/Nt */

  uint16                      init_sinr;
   /* initialization value of the finger Ior/Nt */

  boolean                     div;
   /* controls the diversity mode for this finger. Applicable only to fingers
      0 thru 3. Diversity Mode:1 otherwise 0 */

}
hdrsrchfing_assign_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrchfing_fing_struct_type
 *----------------------------------------------------------------------*/

typedef struct
{
  boolean modified;
   /* indicates that at least one element of finger record has been
      modified. 1 : modified, 0 : unmodified */

  uint8   demod_carrier;
    /* demod carrier */
  
  uint16  pn_offset;
   /* pilot pn of the sector being demodulated by this finger */

  boolean locked;
   /* indicates the lock status of the fing. 1: locked, 0: unlocked */

  int32  posx8;
   /* position of the finger in chipx8 units. This is obtained by
      substracting MSTR from the rtc_offsetx8. We may keep only one of the
      two elements rtc_offsetx8 and posx8 */

  uint16 rssi;
   /* Filtered Ior/Nt estimate for this finger */

  uint16 lock_age;
   /* keeps track of the number of times this finger was found to be in lock */

  uint16 unlock_age;
   /* keeps track of number of times this finger was found to be unlocked */

  hdrsrch_sect_struct_type *sect_ptr;
   /* pointer to the corresponding triage sector list element */

  hdrsrchfing_assign_struct_type dsp_regs;
   /* This structure contains the finger parameters that are programmed
      to the DSP during finger assignment */
}
hdrsrchfing_fing_struct_type;

/*----------------------------------------------------------------------*
                      hdrsrchfing_mps_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  boolean                     pending;
    /* Finger threshold related change is pending */

  hdrsrch_pilot_strength_type thresh;
    /* the t_add for the deciding about which finger thresholds to use */
}
hdrsrchfing_mps_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrchfing_fw_msg_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  hdrfw_fing_assign_msg_t      assign_msg;
    /* Finger assignment message */

  hdrfw_fing_lock_thresh_msg_t lock_thresh_msg;
    /* Finger lock thresh message */
}
hdrsrchfing_fw_msg_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrchfing_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  /*------------------*
   * HDR FING Table   *
   *------------------*/
  hdrsrchfing_fing_struct_type fings[HDRSRCH_MAX_DEMOD_CARRIERS_CNT]
                                    [HDR_MAX_FINGS_CNT];
  /* structure which contains the finger information */

  /*--------------------*
   * FING Config params *
   *--------------------*/
  hdrsrchfing_com_params_struct_type params;
  /* Common parameters for all the fingers */

  hdrsrchfing_mps_struct_type        mps;
    /* Structure to store the status of Min Performance Spec finger thresholds*/

  hdrsrchfing_fw_msg_struct_type     fw_msg;
    /* Firmware messages */

} hdrsrchfing_struct_type;

/*==========================================================================

                    EXTERN VARIABLES and FUNCTIONS

This section contains variables that are accessible outside this module
==========================================================================*/

/*------------------*
 * extern variables *
 *------------------*/

extern hdrsrchfing_struct_type hdrsrchfing;
 /* The global finger structure table which contains the finger status, finger
    programming information and other related info for each rake demodulator */

/*------------------*
 * extern functions *
 *------------------*/

/*=========================================================================

FUNCTION     : HDRSRCHFING_INIT

DESCRIPTION  : This function initializes finger database

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_init( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_DIVERSITY_INIT

DESCRIPTION  : This function controls the RX diversity

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_diversity_init ( boolean enable_div );


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_FING_TIMERS

DESCRIPTION  : Updates the finger lock timer statistics

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_update_fing_timers( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_READ_FING_STATUS

DESCRIPTION  : This function reads the finger status information from the
               MSM and updates the hdrsrchfing.fings[] table with it. This is
               equivalent to the srch_fing_dmp() in the IS-95 series of s/w.
               This function should be called everytime the finger databases
               needs to be refreshed.

               It is recommended that this function be called at the end of the
               search dump ISR. Thus the set evaluation and triage routines
               get to work with the latest finger status.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_read_fing_status( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROGRAM_FINGS

DESCRIPTION  : This function is very widely used by the demodulator module.
               It scans through the global hdrsrchfing.fings[] structure for the
               modified finger parameters. It then copies the those finger
               parameters to the MSM.

               It should be noted however that the finger assignment procedure
               for TAZ2 differs from its IS-95 counterparts. TAZ2 finger mask
               register, which controls the finger assignments, is sampled at
               a fixed rate of 600Hz. The DSP firmware reserves rights to act
               upon the finger assignment request when it feels appropriate.
               Hence it is possible that the uP assigns fingers to the DSP but
               the actual assignment takes place after a while.

               This function always operates on the hdrsrchfing.fings[] struct and
               hence does not accept any parameters.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_program_fings( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_PROGRAM_FINGS_AND_SLAM

DESCRIPTION  : This function assigns finger and set slam flag.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : TRUE: the FW msg is sent 
               FALSE: the FW msg is not sent 
=========================================================================*/
boolean hdrsrchfing_program_fings_and_slam( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_SET_RTC_OFFSET

DESCRIPTION  : Sets the RTC Offset of a finger

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_set_rtc_offset
(
  hdrsrchfing_fing_struct_type *fing,
    /* finger to be updated */

  uint32  rtc_offset_x8
    /* RTC Offset to program to the finger */
);


/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_FING

DESCRIPTION  : Sets the finger parameters so as to deassign it. The new
               assignment is NOT committed to hardware in this function.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_fing
(

  hdrsrchfing_fing_struct_type *fing_ptr
    /* finger to be deassigned */
);


/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_SECT_FINGS

DESCRIPTION  : Deassigns all the fingers that are assigned to this
               particular sector. However the deassignment is NOT
               committed to the hardware.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_sect_fings
(

  hdrsrch_sect_struct_type* sect_ptr
    /* sector to be deassigned */
);


/*=========================================================================

FUNCTION     : HDRSRCHFING_UPDATE_ALL_SECTORS

DESCRIPTION  : Updates all finger parameters to match the parameters of
               the sectors the fingers are assigned to.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : If any changes are made, the "modified" flag is set.

=========================================================================*/

void hdrsrchfing_update_all_sectors( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_DEASSIGN_ALL_FINGS

DESCRIPTION  : Deassigns all the fingers

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_deassign_all_fings ( void );

/*=========================================================================

FUNCTION     : HDRSRCHFING_PROCESS_PILOT_DROP

DESCRIPTION  : This function restores the finger Out-of-lock/In-lock thresholds
               according to the pilot threshold

               For MPS testing, we set finger In-lock/Out-of-lock thresholds 
               according to the pilot_drop value received from the base station.
               If the thresholds are for MPS then we need to operate fingers at
               much lower c/i so we change the finger lock hi and lo. Make sure 
               we don't write to DSP in INACTIVE/SLEEP states since the DSP 
               clock has been turned off.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Will change the lock_lo and lock_hi in the DSP registers

=========================================================================*/

void hdrsrchfing_process_pilot_drop
( 
  hdrsrch_pilot_strength_type pilot_drop
    /* pilot drop threshold based on which we will write the finger 
       thresholds*/
);

/*=========================================================================

FUNCTION     : HDRSRCHFING_RESTORE_FING_THRESH

DESCRIPTION  : This function restores the finger Out-of-lock/In-lock thresholds
               according to the MPS mode 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Will change the lock_lo and lock_hi in the DSP registers

=========================================================================*/

void hdrsrchfing_restore_fing_thresh( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_ARE_FINGS_ASSIGNED

DESCRIPTION  : This function checks whether any finger is assigned.

DEPENDENCIES : None

RETURN VALUE : TRUE=   At least one finger is assigned.
               FALSE=  No finger is assigned.

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_are_fings_assigned( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_SET_DEMOD_CARRIER_MASK

DESCRIPTION  : This function sets the mask which indicates wich demod 
               carriers are enabled.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_set_demod_carrier_mask
(
  uint8                      mask
    /* demod carrier mask */
);


/*=========================================================================

FUNCTION     : HDRSRCHFING_ARE_FINGS_LOCKED

DESCRIPTION  : This function checks whether any finger is locked.

DEPENDENCIES : None

RETURN VALUE : TRUE=   At least one finger is locked.
               FALSE=  No finger is locked.

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchfing_are_fings_locked( void );


/*=========================================================================

FUNCTION     : HDRSRCHFING_LIMIT_MAX_FINGS

DESCRIPTION  : This function sets the max fingers.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_limit_max_fings
(
  boolean          limited
    /* Whether to limit max fingers */
);

/*=========================================================================

FUNCTION     : HDRSRCHFING_PROCESS_SLAM_RSP

DESCRIPTION  : This function process slam response from FW

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchfing_process_slam_rsp
(
  boolean slam_success
);

/*=========================================================================

FUNCTION     : HDRSRCHFING_RESET_ALL_FINGS

DESCRIPTION  : Reset all the fingers for fresh assignment

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Should be used only during fast acq procedure

=========================================================================*/

void hdrsrchfing_reset_all_fings ( void );

#endif /* HDRSRCHFING_H */

