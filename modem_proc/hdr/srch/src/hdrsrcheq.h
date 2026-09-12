#ifndef HDRSRCHEQ_H
#define HDRSRCHEQ_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R    E Q U A L I Z E R   D R I V E R S

                          Equalizer Drivers Header File

GENERAL DESCRIPTION

  This file contains the databases, typedefs, enums and defines used by
  the HDR Rake Receiver Demodulation operation.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright(c) 2000-2011  by Qualcomm Technologies, Incorporated. All Rights Reserved.
                
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrcheq.h#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
08/30/2011   lyl     Added hdrsrcheq_is_grice_active().
03/03/2011   vlc     Added field grice_is_enabled to hdrsrcheq_struct_type. 
01/06/2011   kss     Added hdrsrcheq_is_grice_enabled(). 
11/04/2010   lyl     Removed obsoleted function hdrsrcheq_update_diversity().
04/26/2010   ljl     Removed mdspext.h. 
03/25/2010   pxu     Removed FEATURE_HDR_EQ feature checks. 
06/30/2009   lyl     Updated for generic responses process.
05/18/2009   lyl     Updated for firmware MSGR interface.
12/12/2008   lyl     Fixed EQ data log packet.
12/18/2007   ljl     Updated for demod carrier mask.
10/31/2007   ljl     Merged from Rev B branch.
10/18/2007   ljl     Supported RevB equalizers.
03/28/2007   rmg     Added hdrsrcheq_override_mode_pref() and 
                     hdrsrcheq_eq_config()
11/22/2005   ds      Added edit history header tags
10/05/2005   ds      Added device selection mode and enum type
09/06/2005   ds      Moved  rotator struct and functions to hdrsrchafc.c
06/28/2005   ds      Added demod pref enum type and structure variable
04/22/2004   ds      Added equalizer structure defs and function protoypes
01/16/2004   ds      Created this file

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
#include "hdrsrchdrv.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/
#define HDRSRCHEQ_DEMOD_CARRIER_0_MASK 1
  /* Demod carrier 0 bit mask */

#define HDRSRCHEQ_ANT0         0         /* select ant 0 for equalizer */
#define HDRSRCHEQ_ANT1         1         /* select ant 1 for equalizer */
#define HDRSRCHEQ_MULTIPASS    2         /* Number of multipasses allowed
                                            Total = 3 numbered {0,1,2}; 
                                            Pass 0 is the default pass */

#define HDRSRCHEQ_COM_ENG_THRESH           6
  /* Search path energy threshold ( 6dB ) relative to peak path energy to decide 
     whether to consider a search result for center of mass computation */

/*-------------------------------*
 * Equalizers Count related defines *
 *-------------------------------*/

#define HDRSRCHEQ_MAX_EQS_CNT    3
  /* Total number of equalizers supported by the demod dsp */

#define HDRSRCHEQ_EQ_PASSES_NUM  5
  /* Number of equalizer passes */

#define HDRSRCHEQ_DEMDOD_CARRIER_0_MASK            1
  /* Demod carrier 0 mask */


typedef enum
{
  HDRSRCHEQ_NO_MULTIPASS      = 0x01,
    /* No Multipass enabled */

  HDRSRCHEQ_MULTIPASS2_ENABLE = 0x02,
    /* Second Multipass */

  HDRSRCHEQ_MULTIPASS3_ENABLE = 0x03
    /* Third multipass */

} hdrsrcheq_multipass_enable_enum_type;

typedef enum
{
  HDRSRCHEQ_DEMOD_MODE_BEST,             /* Best of Rake/Eq demod */
  HDRSRCHEQ_DEMOD_MODE_RAKE,             /* Rake only demod */
  HDRSRCHEQ_DEMOD_MODE_EQ,               /* Equalizer only demod */
  HDRSRCHEQ_DEMOD_MODE_MAX
} hdrsrcheq_demod_pref_enum_type; /* The demod preference enum type*/

/* Enum type for device( equalizer or Rake } selection mode */
typedef enum
{
  HDRSRCHEQ_BEST_CI_DEVICE_SELECT_MODE, /* Best C/I based device selection */
  HDRSRCHEQ_DEVICE_SELECT_ALGO_MODE     /* Device selection algorithm      */
} hdrsrcheq_device_select_enum_type;

/*========================================================================

                     MACROS FOR EQUALIZER BIT FIELD SETTINGS

 ========================================================================*/

/*----------------------*
 * Equalizer Mask Macro *
 *----------------------*/
#define HDRSRCHEQ_MASK_ENABLE( e, var )          ( var |= (0x1 << e) )
  /* Enables a particular equalizer bit in the variable passed to this macro */

/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/

/*----------------------------------------------------------------------*
                      hdrsrcheq_common_params_struct_type
 *----------------------------------------------------------------------*/

/* Equalizer parameters common to all the equalizers */

typedef struct
{
  uint8                        max_demod_carriers;
    /* maximum number of demod carriers */

  uint8                        max_eqs;
    /* Maximum number of equalizers */
    
  boolean                      default_ant;
    /* default equalizer antenna select */

  uint8                        demod_carrier_mask;
    /* Mask indicating which carriers are enabled */

}
hdrsrcheq_common_params_struct_type;

typedef struct
{
 
  hdrsrcheq_multipass_enable_enum_type  enable;
    /* Enable equalizer  multipass pilot training */

  uint8                                 lms_step[HDRSRCHEQ_MULTIPASS];
    /* LMS step size shift factor for the equalizer */

  uint8                                 multipass_c2i_used;
    /* Use C/I computed from the result of 1st /2nd /3rd training pass, 
       in Rake vs Equalizer demodulation path decision. */

}
hdrsrcheq_multipass_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrcheq_config_struct_type
 *----------------------------------------------------------------------*/
/* Equalizer parameters to be configured by writing to DSP */

typedef struct
{
  boolean                          enable;
    /* Enable equalizer */ 

  boolean                          filt_reset;
    /* FIR filter reset for equaliser */

  uint8                            asp_idx;
    /* active set pilot map for this equalizer. */
  
  uint8                            ant_sel;
    /* antenna select for equalizer 0: ANT0, 1: ANT1 */

  boolean                          div;
    /* controls the diversity mode for this equalizer. 
       Diversity Mode:1 otherwise 0 */

  uint8                            lms_step;
    /* LMS step size shift factor for the equalizer */

  uint8                            filt_len;
    /* Filter length for the equalizer */

  
  hdrsrcheq_multipass_struct_type  multipass;
    /* Multipass configuration parameters*/
   
}
hdrsrcheq_config_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrcheq_info_struct_type
 *----------------------------------------------------------------------*/

typedef struct
{
  uint32                       c2i;
    /* Unfiltered C/I estimate */

  uint32                       c2i_avr;
    /* C/I estimate averaged over two half slots*/

  uint32                       c2i_filt;
    /* C/I estimate filtered through an IIR */

}
hdrsrcheq_c2i_struct_type;


/*----------------------------------------------------------------------*
                      hdrsrcheq_info_struct_type
 *----------------------------------------------------------------------*/

typedef struct
{
  boolean                      modified;
    /* indicates that at least one element of equalizer record has been
       modified. 1 : modified, 0 : unmodified */

  uint8                        demod_decision;
    /* Decision on whether Rake or equalizer was used for demod */

  uint8                        eq_select;
    /* Decision on which equalizer used for demod if equalizer is used. 
       Invalid if Rake is used */

  uint32                       best_c2i;
    /* Unfiltered C/I estimate for the best equalizer */

  uint32                       best_c2i_avr;
    /* C/I estimate for the best equalizer, averaged over two half slots */

  hdrsrcheq_c2i_struct_type    eq_pass_c2i[HDRSRCHEQ_EQ_PASSES_NUM];
    /* C/I estimate for each equalizer pass */

  /*--------------------*
   * EQ Config params *
   *--------------------*/
  hdrsrcheq_config_struct_type  config;
    /* structure which contains equalizer specific configuration parameters */

}
hdrsrcheq_info_struct_type;

/*----------------------------------------------------------------------*
                      hdrsrcheq_asp_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  int16                       com_posx2;
    /* Center of mass RTC offset in chipx2 units to be set to ASP variable */

  uint8                       path_used;
    /* Store decision on peaks used for center of mass computation- useful
       for logging */

} hdrsrcheq_asp_struct_type;

/*----------------------------------------------------------------------*
                      hdrsrcheq_alt_rake_type
 *----------------------------------------------------------------------*/
typedef struct
{
  uint32                       c2i;
    /* Alternate CI computaion for Rake */

  uint32                       c2i_filt;
    /* Filtered value of Alternate CI computaion for Rake */

} hdrsrcheq_alt_rake_struct_type;

/*----------------------------------------------------------------------*
                      hdrsrcheq_fw_msg_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  hdrfw_eq_cfg_msg_t                  cfg_msg;
    /* Equalizer config message */

  hdrfw_eq_override_msg_t             override_msg;
    /* Equalizer override message */
}
hdrsrcheq_fw_msg_struct_type;

/*----------------------------------------------------------------------*
                      hdrsrcheq_struct_type
 *----------------------------------------------------------------------*/
typedef struct
{
  /*--------------------*
   * EQ ASP Table      *
   *--------------------*/
  hdrsrcheq_asp_struct_type           asp[HDRSRCH_MAX_DEMOD_CARRIERS_CNT]
                                         [HDRSRCH_ASET_MAX_SIZE + 1];
    /* structure which contains center of mass to be written to the ASP MAP */

  /*------------------*
   * HDR EQ Info   *
   *------------------*/
  hdrsrcheq_info_struct_type          eqs[HDRSRCH_MAX_DEMOD_CARRIERS_CNT];
    /* structure which contains the equalizer information */

  /*--------------------*
   * EQ Common params *
   *--------------------*/
  hdrsrcheq_common_params_struct_type params;
    /* Common parameters for all the equalizers */

  hdrsrcheq_alt_rake_struct_type      alt_rake;
    /* Alternate Rake structure */

  boolean                             flag;
    /* Flag to check if equalizer is on or not */

  hdrsrcheq_demod_pref_enum_type      demod_mode_pref;
    /* User demod mode preference */
  
  hdrsrcheq_device_select_enum_type   device_select_mode;
    /* Device selection mode */

  hdrsrcheq_fw_msg_struct_type        fw_msg;
    /* Firmware messages */

  boolean                             grice_is_allowed;
    /* GRICE is allowed per NV setting */

  boolean                             grice_is_enabled;
    /* GRICE is enabled */

} hdrsrcheq_struct_type;


/*==========================================================================

                    EXTERN VARIABLES and FUNCTIONS

This section contains variables that are accessible outside this module
==========================================================================*/

/*------------------*
 * extern variables *
 *------------------*/

extern hdrsrcheq_struct_type          hdrsrcheq;
   /* The global equalizer structure table which contains the equalizer status,
      equalizer programming information and other related info */


/*=========================================================================

FUNCTION     : HDRSRCHEQ_INIT

DESCRIPTION  : This function initializes equalizer configuration database. 

DEPENDENCIES : This is executed only one time after hdrsrch is started.
               This should not be called every time system loss happens.

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrcheq_init( void );


/*=========================================================================

FUNCTION     : HDRSRCHEQ_READ_EQ_STATUS

DESCRIPTION  : This function reads the equalizer status information from the
               MSM and updates the hdrsrcheq.eqs[] table with it. 
               This function should be called everytime the equalizer databases
               needs to be refreshed.

               It is recommended that this function be called at the end of the
               search dump ISR. 

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

========================================================================*/

void hdrsrcheq_read_eq_status( void );

/*=========================================================================

FUNCTION     : HDRSRCHEQ_UPDATE_COM

DESCRIPTION  : This function updates the Center of Mass RTC offset in chipx2
               units to the ASP MAP by assigning to MDSP.
               
DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : Writes Center of Mass RTC offset and pilot PN to the ASP map

=========================================================================*/
void hdrsrcheq_update_com( void );

/*=========================================================================

FUNCTION     : HDRSRCHEQ_ASSIGN_ALL_EQS

DESCRIPTION  : Assigns all the equalizers

DEPENDENCIES : None

INPUTS       : demod_carrier_mask: mask for eqs to be enabled

RETURN VALUE : None

SIDE EFFECTS : The FLL rotators and the equalizers are turned on

=========================================================================*/

void hdrsrcheq_assign_eqs 
( 
  uint8 demod_carrier_mask 
    /* Demod carrier mask */
);

/*=========================================================================

FUNCTION     : HDRSRCHEQ_DEASSIGN_ALL_EQS

DESCRIPTION  : Deassigns all the equalizers

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : Equalizers are turned off and FLL rotator is shut down

=========================================================================*/

void hdrsrcheq_deassign_all_eqs ( void );


/*===========================================================================

FUNCTION     : HDRSRCHEQ_SET_USER_EQ_PREF

DESCRIPTION  : Sets equalizer preference selected by the user to 
               the SW structure. This function is called during lmac 
               powerup init

DEPENDENCIES : None

INPUTS       : User equalizer preference NV control

RETURN VALUE : None

SIDE EFFECTS : None

===========================================================================*/

void hdrsrcheq_set_user_eq_pref
(
  uint8     eq_ctrl
    /* external user equalizer preference */
);


/*=========================================================================

FUNCTION     : HDRSRCHEQ_SET_DEMOD_CARRIER_MASK

DESCRIPTION  : This function sets the demod carrier mask.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrcheq_set_demod_carrier_mask
(
  uint8                      demod_carrier_mask
    /* Demod carrier mask */
);


/*=========================================================================

FUNCTION     : HDRSRCHEQ_OVERRIDE_DEMODE_PREF

DESCRIPTION  : Overrides demod preference

DEPENDENCIES : None

INPUTS       : Demod Mode Preference

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrcheq_override_mode_pref
(
  hdrsrcheq_demod_pref_enum_type demod_pref /* Demod Mode preference */
);

/*=========================================================================

FUNCTION     : HDRSRCHEQ_EQ_CONFIG

DESCRIPTION  : Overwrites equalizer configuration

DEPENDENCIES : None

INPUTS       : Equalizer Index 
               Equalizer Configuration

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrcheq_eq_config
(
  uint8 eq,                                /* Equalizer Index*/
  hdrsrcheq_config_struct_type *eq_config  /* Equalizer Configuration */
);

/*=========================================================================

FUNCTION HDRSRCHEQ_IS_GRICE_ENABLED

DESCRIPTION 
  This function checks if GRICE is enabled (does not have to be currently
  active to be enabled).

DEPENDENCIES
  None

RETURN VALUE
  TRUE -  If GRICE is enabled.
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/
boolean hdrsrcheq_is_grice_enabled( void );

/*=========================================================================

FUNCTION HDRSRCHEQ_IS_GRICE_ACTIVE

DESCRIPTION 
  This function checks if GRICE is currently active to be enabled.

DEPENDENCIES
  None

RETURN VALUE
  TRUE -  If GRICE is active.
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/
boolean hdrsrcheq_is_grice_active( void );


#endif /* HDRSRCHEQ_H */

