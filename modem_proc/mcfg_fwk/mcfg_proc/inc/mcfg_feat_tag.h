/*===========================================================================
                        mcfg_feat_tag.h

DESCRIPTION

   External header file for mcfg_feat_tag
 
Copyright (c) 2016-2017 by QUALCOMM Technologies Incorporated.
 
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_proc/inc/mcfg_feat_tag.h#1 $ $DateTime: 2021/04/08 00:04:19 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
10/5/2016   as     Initial Creation.

===========================================================================*/

#ifndef _MCFG_FEAT_H_
#define _MCFG_FEAT_H_

#include "mcfg_common.h"

/*==============================================================================
  Constants and Macros
==============================================================================*/

/* Defines related to processing status */
#define PROCESSING_OFF    FALSE
#define PROCESSING_ON     TRUE

/*==============================================================================
  Typedefs
==============================================================================*/

/* Tags the different circumstances
   in which feature processing is invoked*/
typedef enum mcfg_feat_context_e {
  MCFG_FEAT_CONTEXT_MIN = 0, 
  MCFG_FEAT_CONTEXT_GENERIC,
  MCFG_FEAT_CONTEXT_DEACTIVATION,
  MCFG_FEAT_CONTEXT_ACTIVATION,
  MCFG_FEAT_CONTEXT_MAX
} mcfg_feat_context_e_type;

/* Adding new features is conrolled via the enum below */
typedef enum mcfg_supported_features_e
{
  MCFG_FEAT_FIRST = 0,
  MCFG_FEAT_MULTISIM = 0,
  MCFG_FEAT_OS,
  MCFG_FEAT_MARKET,
  MCFG_FEAT_DEPLOYMENT,
  MCFG_FEAT_OPENMARKET,
  MCFG_FEAT_IMS_FEAT,
  MCFG_FEAT_COUNTRY,
  MCFG_FEAT_VARIANT,
  MCFG_FEAT_CARRIER,
  MCFG_NUM_OF_SUPPORTED_FEAT,
  MCFG_MAXIMUM_FEAT = 0x7FFF
  /* Added FEAT_MAX to force enum to be 2B */
}mcfg_supported_features_e;

typedef enum mcfg_cond_type_e
{
  MCFG_COND_TYPE_FIRST = 0,
  MCFG_COND_TYPE_IF = 0,
  MCFG_COND_TYPE_ELIF,
  MCFG_COND_TYPE_ELSE,
  MCFG_COND_TYPE_ENDIF,
  MCFG_NUM_OF_COND_TYPES
}mcfg_cond_type_e;

typedef enum mcfg_bool_compare_e
{
  MCFG_EQUAL = 0,
  MCFG_NOT_EQUAL = 1,
  MCFG_MAXIMUM_COMPARE = 0x7FFF
  /* Added MAX_COMPARE to force enum to be 2B */
}mcfg_bool_compare_e;

typedef struct 
{
  mcfg_bool_compare_e comparison;
  uint16              value;
}mcfg_expr_type_s;


/*===========================================================================
  Functions
=============================================================================*/

/*===========================================================================

FUNCTION  mcfg_process_feature() 

DESCRIPTION
  Processes an MCFG FeatureTag

DEPENDENCIES

RETURN VALUE
  boolean - 0: SUCCESS - FeatureTag successfully processed
            1: FAILURE - FeatureTag unsuccessfully processed

SIDE EFFECTS
  None

===========================================================================*/
boolean mcfg_process_feature(uint8, uint8*, uint32, mcfg_feat_context_e_type );

/*===========================================================================

FUNCTION  mcfg_feat_init() 

DESCRIPTION
  Initializes the feature list

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mcfg_feat_init();

/*===========================================================================

FUNCTION  mcfg_processing_status_get() 

DESCRIPTION
  Determines whether the current config processing status is active

DEPENDENCIES

RETURN VALUE
  boolean - TRUE / active (processing occurs normally),
    FALSE / inactive (storage items treated as SW_ONLY).

SIDE EFFECTS
  None

===========================================================================*/
boolean mcfg_processing_status_get();

/*===========================================================================

FUNCTION  mcfg_feature_tag_walk_through() 

DESCRIPTION
  Performs a preliminary scan through MCFG data segment to verify sequence of
  feature tags listed create logical scopes that can be activated/deactivated
  within the configuration.

DEPENDENCIES

RETURN VALUE
  boolean - 0: SUCCESS - FeatureTags in config match intended usage
    1: FAILURE - FeatureTag unsuccessfully processed

SIDE EFFECTS
  None

===========================================================================*/
mcfg_error_e_type mcfg_feature_tag_walk_through(mcfg_config_type_e_type type, mcfg_config_s_type* config);

#endif /* MCFG_FEAT_H */
