/*==============================================================================

      M O D E M   C O N F I G   F E A T U R E T A G   T L V   U T I L S

GENERAL DESCRIPTION
  Provides an interface to help decode the TLV format of feature tags

Copyright (c) 2016-2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_proc/src/mcfg_tlv.c#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
10/5/2016   as     Initial Creation.

==============================================================================*/

#include "comdef.h"

#include "mcfg_common.h"
#include "mcfg_int.h"
#include "mcfg_osal.h" /* for memscpy */

#ifdef FEATURE_MCFG_FEATURETAG
#include "mcfg_feat_tag.h"
/*==============================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/

#define INVALID_OFFSET 0xFFFFFFFF
/*------------------------------------------------------------------------------
  TLV info flags
------------------------------------------------------------------------------*/

#define MCFG_FEAT_TLV_INFO_FLAG_SIZE_BIT (1 << 0)

/* TLV size is fixed; size field in TLV info struct is the expected size */
#define MCFG_FEAT_TLV_INFO_FLAG_SIZE_FIXED (0)

/* TLV size is variable; size field in TLV info struct is minimum data size
   (0 is allowed for no minimum) */
#define MCFG_FEAT_TLV_INFO_FLAG_SIZE_VARIABLE (1 << 0)

/*------------------------------------------------------------------------------
  TLV sizes
------------------------------------------------------------------------------*/

#define MCFG_FEAT_TLV_SIZE_CONDITION       (1)
#define MCFG_FEAT_TLV_SIZE_FEAT_NAME       (2)
#define MCFG_FEAT_TLV_SIZE_COND_EXPRESSION (4)

/*==============================================================================
  Typedefs
==============================================================================*/

typedef PACK(struct) {
  uint8  type;
  uint16 length;
} mcfg_feat_tlv_header_s_type;

typedef PACKED struct 
{ 
  mcfg_feat_tlv_header_s_type hdr;
  uint8                       *value;
} mcfg_feat_tlv;


typedef boolean (mcfg_feat_tlv_decode_fn_type)
(
  uint8                         *ram_cust_data_ptr,
  uint8                          tlv_id,
  uint32                         offset,
  void                          *decode_buffer
);

typedef struct {
  /* Flags describing the TLV */
  uint32 flags;

  /* Expected size of the TLV data (bytes) */
  uint32 size;

  /* Size of the data structure used for the decoded TLV data (bytes, always
     fixed length) */
  uint32 decoded_size;

  /* Function to decode TLV data from the packed format into its associated
     C structure defined in mcfg_feat_tag.h. Set to NULL if no decode function
     implemented. */
  mcfg_feat_tlv_decode_fn_type *decode_fn;
} mcfg_feat_tlv_info_s_type;

/*------------------------------------------------------------------------------
  Packed TLV structures
------------------------------------------------------------------------------*/

/* NONE */

/*==============================================================================
  Function prototypes
==============================================================================*/

static boolean mcfg_feat_decode_generic_tlv
(
  uint8                         *featData,
  uint8                          tlv_id,
  uint32                         offset,
  void                          *decode_buffer
);

static uint32 mcfg_feat_find_tlv
(
  uint8                          *ram_cust_data_ptr,
  uint8                          tlv_id,
  uint32                         feat_data_size
);

boolean mcfg_read_feat_tlv(mcfg_feat_tlv* dest, uint8* src);

/*==============================================================================
  Private Variables
==============================================================================*/

/* Table of TLV information, including decode handler. Indexed by TLV ID. */
static const mcfg_feat_tlv_info_s_type mcfg_feat_tlv_info[] =
{
  /* MCFG_FEAT_TLV_ID_CONDITION */
  {
    MCFG_FEAT_TLV_INFO_FLAG_SIZE_FIXED,
    MCFG_FEAT_TLV_SIZE_CONDITION,
    sizeof(mcfg_cond_type_e),
    mcfg_feat_decode_generic_tlv
  },

  /* MCFG_FEAT_TLV_ID_FEATURE_NAME */
  {
    MCFG_FEAT_TLV_INFO_FLAG_SIZE_FIXED,
    MCFG_FEAT_TLV_SIZE_FEAT_NAME,
    sizeof(mcfg_supported_features_e),
    mcfg_feat_decode_generic_tlv
  },

  /* MCFG_FEAT_TLV_ID_EXPRESSION */
  {
    MCFG_FEAT_TLV_INFO_FLAG_SIZE_FIXED,
    MCFG_FEAT_TLV_SIZE_COND_EXPRESSION,
    sizeof(mcfg_expr_type_s),
    mcfg_feat_decode_generic_tlv
  },

};

/*==============================================================================

                     INTERNAL FUNCTION DECLARATIONS

==============================================================================*/

/*===========================================================================

  FUNCTION mcfg_feat_decode_generic_tlv

  DESCRIPTION
    Decodes the carrier MCC+MNC TLV data.

  DEPENDENCIES
    Assumes the trailer record, TLV length, and decode buffer length have
    already been validated. Also assumes decode_buffer has been zero-init.

  PARAMETERS
    config_info    [in]
    offset         [in] Byte index of the start of the format version TLV
    decode_buffer [out] Output buffer for decoded data

  RETURN VALUE
    TRUE on successful decode, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_read_feat_tlv(mcfg_feat_tlv* dest, uint8* src)
{
  boolean success = TRUE; // here for future use in case we want to indicate errors

  memscpy(&(dest->hdr.type), sizeof(uint8), src, sizeof(uint8));
  src += sizeof(uint8);
  memscpy(&(dest->hdr.length), sizeof(uint16), src, sizeof(uint16));
  src += sizeof(uint16);
  dest->value = src;

  return success;
}

/*===========================================================================

  FUNCTION mcfg_feat_decode_generic_tlv

  DESCRIPTION
    Generic TLV decoder for TLV data

  DEPENDENCIES
    TLV length, and decode buffer length have already been validated.
    Also assumes decode_buffer has been zero-init.

  PARAMETERS
    *featData        [in] Pointer to beginning of TLVs listed in featureTag data
     tlv_id          [in] TLV identifier
     offset          [in] Offset from *featData to beginning of needed TLV data
    *decode_buffer   [out] Output buffer for decoded data

  RETURN VALUE
    TRUE on successful decode, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_feat_decode_generic_tlv
(
  uint8                         *featData,
  uint8                          tlv_id,
  uint32                         offset,
  void                          *decode_buffer
)
{
  mcfg_feat_tlv tlv;
  uint32 decode_buffer_len;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(featData);
  MCFG_CHECK_NULL_PTR_RET_FALSE(decode_buffer);

  decode_buffer_len = mcfg_feat_tlv_info[tlv_id].size;
  
  if (!mcfg_read_feat_tlv(&tlv, featData + offset))
  {
    MCFG_MSG_ERROR_1("Failure reading generic TLV data for TLV ID %d", tlv_id);
    return FALSE;
  }

  if (tlv.hdr.length > decode_buffer_len)
  {
    MCFG_MSG_MED_2("FeatureTag TLV decode: dest buffer smaller than src buffer - src_buf: %d, dest_buf: %d", tlv.hdr.length, decode_buffer_len);
  }
  // Lengths already validated during find tlv
  memscpy(decode_buffer, decode_buffer_len, tlv.value, tlv.hdr.length);

  return TRUE;
} /* mcfg_feat_decode_generic_tlv() */

/*===========================================================================

  FUNCTION mcfg_feat_find_tlv

  DESCRIPTION
    Searches within specified size bytes for a TLV with matching ID in starting from data ptr. Also
    verifies that the length field of the TLV is valid (is correct per the
    TLV info and does not exceed the length of the specified data size).

  DEPENDENCIES
    The TLV ID must already be sanity checked to be
    within the supported bounds of mcfg_feat_tlv_info[].

  PARAMETERS
    config_info [in]
    tlv_id      [in]

  RETURN VALUE
    Byte offset to start of TLV structure with matching TLV ID, or zero
    if the TLV was not found

  SIDE EFFECTS
    None

===========================================================================*/
static uint32 mcfg_feat_find_tlv
(
  uint8                          *ram_cust_data_ptr,
  uint8                          tlv_id,
  uint32                         feat_data_size
)
{
  boolean found = FALSE;
  uint32 offset = 0;
  const mcfg_feat_tlv_header_s_type *tlv_header;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_ZERO(ram_cust_data_ptr);

#if 0
#ifdef MCFG_DEBUG
    MCFG_MSG_MED_3("feat_data_size: %d, feat_hdr_sz: %d, diff: %d",
                    feat_data_size, sizeof(mcfg_feat_tlv_header_s_type),
                    feat_data_size - sizeof(mcfg_feat_tlv_header_s_type));

#endif /* MCFG_DEBUG */
#endif 

  while (!found && offset <= (feat_data_size -
                                sizeof(mcfg_feat_tlv_header_s_type)))
  {
#ifdef MCFG_DEBUG
    // MCFG_MSG_MED_3("found: %d, offset: %d, diff: %d",
    //               found, offset, feat_data_size - sizeof(mcfg_feat_tlv_header_s_type));
#endif /* MCFG_DEBUG */

    tlv_header = (const mcfg_feat_tlv_header_s_type *)
      &ram_cust_data_ptr[offset];
    if (tlv_header->type == tlv_id)
    {
      MCFG_MSG_HIGH_1("found succeeded for tlv id: %d", tlv_id);
      found = TRUE;
    }
    else
    {
#ifdef MCFG_DEBUG
    // MCFG_MSG_MED_1("adding tlv_header->length to offset: %d", tlv_header->length);
#endif /* MCFG_DEBUG */
      offset += sizeof(mcfg_feat_tlv_header_s_type) + tlv_header->length;
    }
  }

  if (!found)
  {
    offset = INVALID_OFFSET;
    MCFG_MSG_HIGH_1("TLV 0x%02x not found", tlv_id);
  }
  else if ((offset + sizeof(mcfg_feat_tlv_header_s_type) + tlv_header->length) >
             feat_data_size)
  {
    MCFG_MSG_ERROR_4("Bad size for TLV 0x%02x: length %hu at offset %lu past "
                     "feat size %lu", tlv_id, tlv_header->length, offset,
                     feat_data_size);
    offset = INVALID_OFFSET;
  }
  else if (((mcfg_feat_tlv_info[tlv_id].flags & MCFG_FEAT_TLV_INFO_FLAG_SIZE_BIT) ==
              MCFG_FEAT_TLV_INFO_FLAG_SIZE_VARIABLE &&
            tlv_header->length < mcfg_feat_tlv_info[tlv_id].size) ||
           ((mcfg_feat_tlv_info[tlv_id].flags & MCFG_FEAT_TLV_INFO_FLAG_SIZE_BIT) ==
              MCFG_FEAT_TLV_INFO_FLAG_SIZE_FIXED &&
            tlv_header->length != mcfg_feat_tlv_info[tlv_id].size))
  {
    offset = INVALID_OFFSET;
    MCFG_MSG_ERROR_3("Bad length for TLV 0x%02x: got %hu, expected/min %lu",
                     tlv_id, tlv_header->length, mcfg_feat_tlv_info[tlv_id].size);
  }

  return offset;
} /* mcfg_feat_find_tlv() */

/*==============================================================================
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
==============================================================================*/

/*===========================================================================

  FUNCTION mcfg_feat_decode_tlv

  DESCRIPTION
    Finds a TLV and decodes it into the appropriate data
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
  uint8                         *ram_cust_data_ptr,
  uint8                          tlv_id,
  void                          *decode_buffer,
  uint32                         decode_buffer_len, 
  uint32                         feat_tlv_size
)
{
  boolean success = FALSE;
  uint32 offset;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(ram_cust_data_ptr);
  MCFG_CHECK_NULL_PTR_RET_FALSE(decode_buffer);

  if (decode_buffer_len != mcfg_feat_tlv_info[tlv_id].decoded_size)
  {
    MCFG_MSG_ERROR_2("Invalid decode buffer size %lu, expected %lu",
                     decode_buffer_len, mcfg_feat_tlv_info[tlv_id].decoded_size);
  }
  else
  {
    offset = mcfg_feat_find_tlv(ram_cust_data_ptr, tlv_id, feat_tlv_size);
    if (offset == INVALID_OFFSET)
    {
      MCFG_MSG_ERROR_1("TLV ID 0x%02x not found or has invalid length", tlv_id);
    }
    else
    {
      MCFG_MSG_MED_2("Offset for TLV ID 0x%02x: %d", tlv_id, offset);
      memset(decode_buffer, 0, decode_buffer_len);
      success = mcfg_feat_tlv_info[tlv_id].decode_fn(
        ram_cust_data_ptr, tlv_id, offset, decode_buffer);
    }
  }


  MCFG_MSG_MED_2("FeatDecode: Decode status for TLV ID 0x%02x: %d", tlv_id, success);
  return success;
} /* mcfg_feat_decode_tlv() */

#endif  /* FEATURE_MCFG_FEATURETAG */
