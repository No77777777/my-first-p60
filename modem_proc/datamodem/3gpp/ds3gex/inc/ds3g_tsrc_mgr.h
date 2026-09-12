#ifndef DS3G_TSRC_MGR_H
#define DS3G_TSRC_MGR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   T S R C   M G R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services event handling.
  It provides APIs for DS3G event registration and notification.

  Copyright (c) 2017-2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/20/17   mvb     Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "queue.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#ifdef FEATURE_DATA_TSRC
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Macro to check if a radio mask has a specified DSD RADIO 
---------------------------------------------------------------------------*/
#define DS3G_TSRC_RADIO_MASK_HAS_RADIO( radio_mask, radio )  \
                        ((radio_mask & radio) == radio)

/*-----------------------------------------------------------------------------
  TSRC Stretch value efs structure to be configurable based on NV later.
  - tsrw_feature_enabled:  This field is to indicate whether or not the
                           entire Timestamp Rewriting feature is enabled
                           or disabled.  Enabled by default
 
  - stretch_ratio:         Unitless ratio used by IPA to aid in their timestamp
                           stretch calculations.
 
  - X_stretch_info_value:  These RAT specific stretch info values are the
                           timer values used to by IPA to alter the timestamp
                           values (aka how much to stretch the timestamp by)
                           based on the CRAT situation.  If device is camped
                           on multisim, L+G, single stack mode, then TSRC
                           will advertise gsm_stretch_info_value.  If there
                           are multiple RATs to take into account, then we
                           will use the maximum value.
  
-----------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  boolean                         tsrw_feature_enabled;
  uint32                          stretch_ratio;               /* unitless */
  uint32                          cdma_stretch_info_value;     /* in msec */       
  uint32                          lte_stretch_info_value;      /* in msec */       
  uint32                          gsm_stretch_info_value;      /* in msec */     
  uint32                          wcdma_stretch_info_value;    /* in msec */
  uint32                          reserved[4];
  uint8                           reserved2[2];
}ds3g_tsrc_stretch_efs_info_type;

/*-----------------------------------------------------------------------------
  Structure to store all the internal CP Data Information.
  enabled:             Tells us whether strech is enabled
  stretch_val:         Tells us the currently used stretch time if
                       stretch is enabled.
  efs_stretch_info:    Stores all of the hardcoded static EFS configuration
                       that is relevant to TSRC for the current DDS sub.
  stretch_rats_mask:   Radio mask that tells us what RATs are valid for the
                       current concurrent RAT situation.  If we are camped
                       on L+G, then it will have GSM in the radio mask.
                       If we are camped on SRLTE+G, then it will have 1x and G
                       in the radio mask.  If we are in L+OOS and OOS SUB has
                       GWL mode pref, then it will have GWL in the radio mask.
                       In general, if we are in single stack mode, this will
                       track the RATs enabled for stretch in the non DDS Sub.
                       The only exception to the non DDS Sub is when device
                       is dual stack enabled, in which case DS will add 1x. 
  is_dual_stack_mode:  Per subscription storage indicating whether that sub is
                       dual stack or single stack mode.
  use_rat_capability:  Tells us whether the non DDS sub is OOS and we need to
                       instead rely on rat capability (which takes mode pref
                       and subs capability into account)
-----------------------------------------------------------------------------*/
typedef struct
{
  uint32                           stretch_val;
  ds3gsubsmgr_subs_id_e_type       dds_id;
  /* Below fields are used for stretch calculation */
  uint16                           stretch_rats_mask;
  boolean                          is_dual_stack_mode[DS3GSUBSMGR_SUBS_ID_MAX];
  boolean                          use_rat_capability;
} ds3g_tsrc_mgr_stretch_info_s;

/*-----------------------------------------------------------------------------
  Default TSRC Stretch time values
-----------------------------------------------------------------------------*/
#define DS3G_TSRC_STRETCH_CDMA_DEFAULT_VAL        338  /* in msec */
#define DS3G_TSRC_STRETCH_GSM_DEFAULT_VAL         237  /* in msec */
#define DS3G_TSRC_STRETCH_WCDMA_DEFAULT_VAL       396  /* in msec */
#define DS3G_TSRC_STRETCH_LTE_DEFAULT_VAL         440  /* in msec */
#define DS3G_TSRC_STRETCH_RATIO_DEFAULT_VAL       100  /* unitless */

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3G_TSRC_MGR_INIT

DESCRIPTION   This function performs initialization of ds3g_tsrc_mgr.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_tsrc_mgr_init( void );

/*=============================================================================
FUNCTION      DS3G_TSRC_GET_DDS_STRETCH_EFS_INFO

DESCRIPTION   This function returns the structure holding all of the static 
              stretch related values and fields (feature enabled, stretch
              values per RAT, stretch ratio, etc)

DEPENDENCIES  None

RETURN VALUE  stretch efs info

SIDE EFFECTS  None
=============================================================================*/
ds3g_tsrc_stretch_efs_info_type * ds3g_tsrc_get_dds_stretch_efs_info(void);

/*=============================================================================
FUNCTION      DS3G_TSRC_IS_STRETCH_ENABLED

DESCRIPTION   This function returns whether stretch is enabled or not. 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
boolean ds3g_tsrc_is_stretch_enabled(void);

/*=============================================================================
FUNCTION      DS3G_TSRC_MGR_PROCESS_SS_EVENT_FOR_STRETCH

DESCRIPTION   This function processes the ss event from CM and populates 
              1)  the valid rats available for stretch calculation
              2)  the number of operational stacks
              3)  multisim mode
              4)  whether preconditions are validated on a given sub
              Note that this function is called per sub

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
void ds3g_tsrc_process_ss_event_for_stretch
(
  sys_modem_as_id_e_type           cm_subs_id
);

/*=============================================================================
FUNCTION      DS3G_TSRC_IS_TSRW_ENABLED

DESCRIPTION   This function returns whether stretch is enabled or not. 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
boolean ds3g_tsrc_is_tsrw_enabled(void);

/*=============================================================================
FUNCTION      DS3G_TSRC_GET_CURRENT_STRETCH_VALUE

DESCRIPTION   This function returns what the current stretch value being used is. 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
uint32 ds3g_tsrc_get_current_stretch_value(void);

/*=============================================================================
FUNCTION      DS3G_TSRC_GET_STRETCH_RATS_MASK

DESCRIPTION   This function returns the current stretch rats mask used to 
              determine what the stretch value. If stretch RAT mask contains
              more than 1 RAT, then it will find the max stretch value using
              this rats mask.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
uint16 ds3g_tsrc_get_stretch_rats_mask(void);

/*===========================================================================
FUNCTION ds3g_tsrc_read_tsrw_config_nv

DESCRIPTION
  This function reads the timestamp rewriting configuration NV.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds3g_tsrc_read_tsrw_config_nv
(
  sys_modem_as_id_e_type  subs_id
);

#endif /* FEATURE_DATA_TSRC */
#endif /* DS3G_TSRC_MGR_H */
