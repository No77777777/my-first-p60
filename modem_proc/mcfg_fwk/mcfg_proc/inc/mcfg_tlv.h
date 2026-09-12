#ifndef MCFG_FEAT_H
#define MCFG_FEAT_H
/*==============================================================================

      M O D E M   C O N F I G   F E A T   T L V   U T I L S

GENERAL DESCRIPTION
  Provides an interface to help decode feat TLV formats

Copyright (c) 2016-2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_proc/inc/mcfg_tlv.h#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
10/5/2016   as     Initial Creation.

==============================================================================*/

#include "comdef.h"
#include "mcfg_common.h"

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/

/*------------------------------------------------------------------------------
  FEAT TLV IDs
------------------------------------------------------------------------------*/

#define MCFG_FEAT_TLV_ID_CONDITION               (0x00)
#define MCFG_FEAT_TLV_ID_FEATURE_NAME            (0x01)
#define MCFG_FEAT_TLV_ID_CONDITIONAL_EXPRESSION  (0x02)

/*------------------------------------------------------------------------------
  Constants used in TLV data
------------------------------------------------------------------------------*/

/* Conditional Expression: maximum number of characters in the conditional expression string */
#define MCFG_FEAT_COND_EXPR_STRING_MAX_LEN (127)

/*==============================================================================
  Typedefs
==============================================================================*/

/*------------------------------------------------------------------------------
  TLV data structures
------------------------------------------------------------------------------*/

/* MCFG_FEAT_TLV_ID_CONDITION */
typedef struct {
  uint8 feat_condition;
} mcfg_feat_condition_s_type;

/* MCFG_FEAT_TLV_ID_FEATURE_NAME */
typedef PACKED struct {
  /* Feature Name */
  uint8 feat_name;
} mcfg_feat_name_s_type;

/* MCFG_FEAT_TLV_ID_CONDITIONAL_EXPRESSION */
typedef struct {
  /* For now treat this as an integer. Future enhancements
     will convert this to a NULL-terminated ASCII string. */
  // char carrier_name[MCFG_FEAT_COND_EXPR_STRING_MAX_LEN + 1];
  uint8 feat_cond_expr;
} mcfg_feat_carrier_name_s_type;

/*==============================================================================

                    PUBLIC FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/

/*===========================================================================

  FUNCTION mcfg_feat_decode_tlv

  DESCRIPTION
    Finds a feature TLV and decodes it into the appropriate data
    structure.

  DEPENDENCIES
    None

  PARAMETERS
    config_info       [in]
    tlv_id            [in]
    decoded_data     [out] Pointer to decoded data structure for this TLV
    decoded_data_len [out] Size of the decoded data structure

  RETURN VALUE
    TRUE if TLV was successfully found and decoded, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_feat_decode_tlv
(
  uint8          *ram_cust_data_ptr,
  uint8           tlv_id,
  void           *decoded_data,
  uint32          decoded_data_len,
  uint32          feat_tlv_size
);

#endif /* MCFG_FEAT_H */
