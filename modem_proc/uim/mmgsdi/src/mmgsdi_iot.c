/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   I O T   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the iot functions 

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_iot.c#2 $$ $DateTime: 2019/12/25 22:20:38 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/24/19   cj      Update EF TST byte 11  
10/29/19   sch     Enabling UIM IOT capabilities for HA builds
10/03/19   sch     Added support for features UIM capabilities for HA builds
01/31/17   nc      Added support for features UIM capabilities for HE builds  
12/02/16   me      Added error check for the iot capabilities file
11/22/16   me      Added support for features UIM capabilities for CE builds
08/16/16   me      initial version 
 
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdi_nv.h"
#include "mmgsdi_iot.h"

#ifdef FEATURE_LTE
#include "lte_misc.h"
#endif /* FEATURE_LTE */

#include "uim_msg.h"
#ifdef FEATURE_UIM_ATLAS_CAPABILITIES
#include "mmgsdi_iot_capabilities_at.h"
#elif defined FEATURE_UIM_CE_CAPABILITIES
#include "mmgsdi_iot_capabilities_ce.h"
#elif defined FEATURE_UIM_HE_CAPABILITIES
#include "mmgsdi_iot_capabilities_he.h"
#elif defined FEATURE_UIM_HA_CAPABILITIES
#include "mmgsdi_iot_capabilities_ha.h"
#else
#error "IoT capabilities: IoT capabilities file for the target not found!"
#endif /* FEATURE_UIM_CAPABILITIES */

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* List of values returned by LTE module to indicate the category */
#define MMGSDI_LTE_IOT_CAT_M1                                              0xFF
#define MMGSDI_LTE_IOT_CAT_NB1                                             0xFE
#define MMGSDI_LTE_IOT_CAT1                                                0x01
#define MMGSDI_LTE_IOT_CAT2                                                0x02
#define MMGSDI_LTE_IOT_CAT3                                                0x03
#define MMGSDI_LTE_IOT_CAT4                                                0x04
#define MMGSDI_LTE_IOT_CAT5                                                0x05
#define MMGSDI_LTE_IOT_CAT6                                                0x06
#define MMGSDI_LTE_IOT_CAT7                                                0x07
#define MMGSDI_LTE_IOT_CAT8                                                0x08
#define MMGSDI_LTE_IOT_CAT9                                                0x09                                   
#define MMGSDI_LTE_IOT_CAT10                                               0x0A
#define MMGSDI_LTE_IOT_CAT11                                               0x0B
#define MMGSDI_LTE_IOT_CAT12                                               0x0C
#define MMGSDI_LTE_IOT_CAT13                                               0x0D
#define MMGSDI_LTE_IOT_CAT16                                               0x10

/*---------------------------------------------------------------------------
  To store list of status of IOT features supported on the target
---------------------------------------------------------------------------*/
typedef struct 
{
  mmgsdi_iot_feature_enum          feature;
  mmgsdi_feature_status_enum_type  enabled_status;
} mmgsdi_features_iot;

static mmgsdi_features_iot features_iot_table[] =
{
  { MMGSDI_IOT_NAS_SIGNALING_PRIORITY_SUPPORT,                         FEATURE_MMGSDI_NAS_SIG_PRIORITY_SUPPORT},
  { MMGSDI_IOT_MINIMUM_PERIODIC_SEARCH_TIMER_SUPPORT,                  FEATURE_MMGSDI_MINIMUM_PER_SEARCH_TIMER_SUPPORT},
  { MMGSDI_IOT_EXTENDED_ACCESS_BARRING_SUPPORT,                        FEATURE_MMGSDI_EXTENDED_ACCESS_BARRING_SUPPORT},
  { MMGSDI_IOT_TIMER_T3245_SUPPORT,                                    FEATURE_MMGSDI_TIMER_T3245_SUPPORT},
  { MMGSDI_IOT_OVERRIDE_NAS_SIGNALING_LOW_PRIORITY_SUPPORT,            FEATURE_MMGSDI_OVERRIDE_NAS_SIG_LOW_PRIORITY_SUPPORT},
  { MMGSDI_IOT_OVERRIDE_EXTENDED_ACCESS_BARRING_SUPPORT,               FEATURE_MMGSDI_OVERRIDE_EXTENDED_ACCESS_BARRING_SUPPORT},
  { MMGSDI_IOT_SM_RETRY_WAIT_TIME_SUPPORT,                             FEATURE_MMGSDI_SM_RETRY_WAIT_TIME_SUPPORT},
  { MMGSDI_IOT_DELAY_TOLERANT_ACCESS_SUPPORT,                          FEATURE_MMGSDI_DELAY_TOLERANT_ACCESS_SUPPORT},
  { MMGSDI_IOT_POWER_SAVING_MODE_PSM_SUPPORT,                          FEATURE_MMGSDI_POWER_SAVING_MODE_PSM_SUPPORT},
  { MMGSDI_IOT_COVERAGE_ENHANCEMENT_MODE_A_SUPPORT,                    FEATURE_MMGSDI_COVERAGE_ENH_MODE_A_SUPPORT},
  { MMGSDI_IOT_COVERAGE_ENHANCEMENT_MODE_B_SUPPORT,                    FEATURE_MMGSDI_COVERAGE_ENH_MODE_B_SUPPORT},
  { MMGSDI_IOT_IDLE_MODE_EXTENDED_DISCONTINUOUS_RECEPTION_SUPPORT,     FEATURE_MMGSDI_IDLE_MODE_EXT_DIS_RECEPTION_SUPPORT},
  { MMGSDI_IOT_CONNECTED_MODE_EXTENDED_DISCONTINUOUS_RECEPTION_SUPPORT,FEATURE_MMGSDI_CONNECTED_MODE_EXT_DIS_RECEPTION_SUPPORT},
  { MMGSDI_IOT_DEVICE_BASED_POSITIONING_SUPPORT,                       FEATURE_MMGSDI_DEVICE_BASED_POSITIONING_SUPPORT},
  { MMGSDI_IOT_SINGLE_CELL_POINT_TO_MULTIPOINT_TRANSMISSION_SUPPORT,   FEATURE_MMGSDI_SINGLE_CELL_POINT_TO_MULTIPOINT_TRANSMISSION_SUPPORT}
};


/*---------------------------------------------------------------------------
  To store list of IOT categories 
---------------------------------------------------------------------------*/
static PACKED struct PACKED_POST 
{
  uint8                             lte_category;
  mmgsdi_iot_category_enum          category;
} mmgsdi_lte_category_iot_table[] =
{
  { MMGSDI_LTE_IOT_CAT_M1,         MMGSDI_IOT_CAT_M1},
  { MMGSDI_LTE_IOT_CAT_NB1,        MMGSDI_IOT_CAT_NB1},
  { MMGSDI_LTE_IOT_CAT1,           MMGSDI_IOT_CAT1},
  { MMGSDI_LTE_IOT_CAT2,           MMGSDI_IOT_CAT2},
  { MMGSDI_LTE_IOT_CAT3,           MMGSDI_IOT_CAT3},
  { MMGSDI_LTE_IOT_CAT4,           MMGSDI_IOT_CAT4},
  { MMGSDI_LTE_IOT_CAT5,           MMGSDI_IOT_CAT5},
  { MMGSDI_LTE_IOT_CAT6,           MMGSDI_IOT_CAT6},
  { MMGSDI_LTE_IOT_CAT7,           MMGSDI_IOT_CAT7},
  { MMGSDI_LTE_IOT_CAT8,           MMGSDI_IOT_CAT8},
  { MMGSDI_LTE_IOT_CAT9,           MMGSDI_IOT_CAT9},
  { MMGSDI_LTE_IOT_CAT10,          MMGSDI_IOT_CAT10},
  { MMGSDI_LTE_IOT_CAT11,          MMGSDI_IOT_CAT11},
  { MMGSDI_LTE_IOT_CAT12,          MMGSDI_IOT_CAT12}
};


/* ============================================================================
FUNCTION MMGSDI_IS_IOT_CAPABILITY_SUPPORTED

DESCRIPTION
  This utility function return the feature status if it's disabled or enabled 

DEPENDENCIES
  None

LIMITATIONS
  none.

RETURN VALUE
  MMGSDI_FEATURE_ENABLED: if the feature is enabled
  MMGSDI_FEATURE_DISABLED: if the feature is disabled 

SIDE EFFECTS
  None

============================================================================*/
mmgsdi_feature_status_enum_type mmgsdi_is_iot_capability_supported(
  mmgsdi_iot_feature_enum  feature 
)
{
  mmgsdi_feature_status_enum_type    feature_status   = MMGSDI_FEATURE_DISABLED;
  uint16                             i                = 0;

  /* Return feature_status if feature is enabled or disabled */
  for (i = 0; i < sizeof(features_iot_table) / sizeof(features_iot_table[0]); i++)
  {
    if (features_iot_table[i].feature == feature)
    {
      feature_status = features_iot_table[i].enabled_status;
      break;
    }
  }
  return feature_status;
}/* mmgsdi_is_iot_capability_supported */


/* ============================================================================
FUNCTION MMGSDI_IS_IOT_DEVICE

DESCRIPTION
  This utility function return the TRUE or FALSE,
  if the target is IOE and NV is not set or set to MTC then return true
  if the target is IOE and NV is set to non MTC then return false
  if the target is not IOE and NV is not set then return false
  if the target is not IOE and NV is set to MTC then return true
  if the target is not IOE and NV is set to non MTC then return false 

DEPENDENCIES
  None

LIMITATIONS
  none.

RETURN VALUE
  MTC :   TRUE or FALSE

SIDE EFFECTS
  None

============================================================================*/
boolean mmgsdi_is_iot_device(
  void
)
{
  boolean                 is_iot_device = FALSE;
  uint8                   nv_data       = 0;
  
#ifdef FEATURE_UIM_IOE
  is_iot_device = TRUE;
#else 
  is_iot_device = FALSE;
#endif /* FEATURE_UIM_IOT */

  if (uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_MTC,
                          UIM_COMMON_EFS_ITEM_FILE_TYPE,
                          UIM_COMMON_EFS_DEVICE,
                          &nv_data,
                          sizeof(nv_data)) == UIM_COMMON_EFS_SUCCESS)
  {
    if (nv_data == 1)
    {
      is_iot_device = TRUE;
    }
    else if (nv_data == 2)
    {
      is_iot_device = FALSE;
    }
  }
  return is_iot_device;
}/* mmgsdi_is_iot_device */


/* ============================================================================
FUNCTION MMGSDI_GET_IOT_CATEGORY

DESCRIPTION
  This utility function return the category 

DEPENDENCIES
  None

LIMITATIONS
  none.

RETURN VALUE
  CATEGORY:   category 

SIDE EFFECTS
  None

============================================================================*/
mmgsdi_iot_category_enum mmgsdi_get_iot_category(
  void
)
{
#ifdef FEATURE_LTE
  mmgsdi_iot_category_enum           iot_category     = MMGSDI_IOT_UNKNWON;
  uint8                              lte_iot_category = 0;
  uint16                             i                = 0;

  lte_iot_category = lte_get_ue_category();
  UIM_MSG_LOW_1("LTE category is: 0x%x", lte_iot_category);

  for (i = 0; i < sizeof(mmgsdi_lte_category_iot_table) / sizeof(mmgsdi_lte_category_iot_table[0]); i++)
  {
    if (mmgsdi_lte_category_iot_table[i].lte_category == lte_iot_category)
    {
      iot_category = mmgsdi_lte_category_iot_table[i].category;
      break;
    }
  }
  return iot_category;
#else
  return MMGSDI_IOT_UNKNWON;
#endif /* FEATURE_LTE */
}/* mmgsdi_get_iot_category */

