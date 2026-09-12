/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 M M G S D I   U I M   L O G G I N G   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains utility functions to support masking of sensitive information.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2017, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE



when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/02/20   sch     Added support for log masking 
08/28/17   tq      Hiding Keys data irrespective of is it valid or not
01/19/17   nc      Initial revision 
==============================================================================*/ 

#include "uim_msg.h"
#include "mmgsdilib.h"
#include "mmgsdilib_p.h"
#include "mmgsdi_logging_mask.h"
#include "mmgsdi_uim_uicc.h"
#include "mmgsdiutil.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* Length of the EF-EXT2 and EF-EXT4 files. */
#define MMGSDI_EXT2_EXT4_LEN             13
/* Length of the mandatory fields of the EF-FDN and EF-ICE_DN files. */
#define MMGSDI_FDN_ICE_DN_MIN_LEN        14
/* Length of the mandatory fields of the EF-BDN file. */
#define MMGSDI_BDN_MIN_LEN               15

/* Type for holding log masking information */
typedef struct
{
  uim_path_type              path;    /* Path to the EF */
  boolean                    (*masking_fn_ptr)(const uint8 *input_ptr, uint8 *output_ptr, 
                                               uint32 len); /* The function pointer to mask the log information */
}mmgsdi_masking_path_type;

/* masking function declarations*/
static boolean mmgsdi_log_masking_fn_keys(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
);

static boolean mmgsdi_log_masking_fn_all(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
);

static boolean mmgsdi_log_masking_fn_auth_resp(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
);

static boolean mmgsdi_log_masking_fn_mbms_auth_resp(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
);

static boolean mmgsdi_log_masking_fn_ext2_ext4(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
);

static boolean mmgsdi_log_masking_fn_fdn_ice_dn(
  const uint8              *input_ptr,
  uint8                    *output_ptr,
  uint32                    length
);

static boolean mmgsdi_log_masking_fn_loci(
  const uint8                       *input_ptr,
  uint8                             *output_ptr,
  uint32                             length
);

static boolean mmgsdi_log_masking_fn_bdn(
  const uint8                     *input_ptr,
  uint8                           *output_ptr,
  uint32                           length
);

static boolean mmgsdi_log_masking_fn_epsloci(
  const uint8                           *input_ptr,
  uint8                                 *output_ptr,
  uint32                                 length
);

static boolean mmgsdi_log_masking_fn_pseudo(
  const uint8                          *input_ptr,
  uint8                                *output_ptr,
  uint32                                length
);

/* Table for file paths USIM file paths under secure logging category */
const mmgsdi_masking_path_type mmgsdi_masking_path_usim_tbl_secure[] =
{
  /* Path to KEYS */
  {{0x03, {0x3F00, 0x7FFF, 0x6F08, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_keys},
  /* Path to KEYSPS */
  {{0x03, {0x3F00, 0x7FFF, 0x6F09, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_keys},
  /* Path to KC */
  {{0x04, {0x3F00, 0x7FFF, 0x5F3B, 0x4F20, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to KCGPRS */
  {{0x04, {0x3F00, 0x7FFF, 0x5F3B, 0x4F52, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to EPSNSC */
  {{0x03, {0x3F00, 0x7FFF, 0x6FE4, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to START-HFN */
  {{0x03, {0x3F00, 0x7FFF, 0x6F5B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to Hidden Key */
  {{0x03, {0x3F00, 0x7FFF, 0x6FC3, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths USIM file paths under private logging category */
const mmgsdi_masking_path_type mmgsdi_masking_path_usim_tbl_private[] =
{
  /* Path to EXT2 */
  {{0x03, {0x3F00, 0x7FFF, 0x6F4B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_ext2_ext4},
  /* Path to EXT4 */
  {{0x03, {0x3F00, 0x7FFF, 0x6F55, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_ext2_ext4},
  /* Path to FDN */
  {{0x03, {0x3F00, 0x7FFF, 0x6F3B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_fdn_ice_dn},
  /* Path to ICE_DN */
  {{0x03, {0x3F00, 0x7F10, 0x6FE0, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_fdn_ice_dn},
  /* Path to LOCI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F7E, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_loci},
  /* Path to BDN */
  {{0x03, {0x3F00, 0x7FFF, 0x6F4D, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_bdn},
  /* Path to EPSLOCI */
  {{0x03, {0x3F00, 0x7FFF, 0x6FE3, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_epsloci},
  /* Path to PSEUDO */
  {{0x04, {0x3F00, 0x7FFF, 0x5F40, 0x4F41, 0xFFFF}}, mmgsdi_log_masking_fn_pseudo},
    /* Path to SMS */
  {{0x03, {0x3F00, 0x7FFF, 0x6F3C, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to LI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F05, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CBMI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F45, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CBMIR */
  {{0x03, {0x3F00, 0x7FFF, 0x6F50, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PSLOCI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F73, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ICI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F80, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to OCI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F81, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ACL */
  {{0x03, {0x3F00, 0x7FFF, 0x6F57, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to EXT5 */
  {{0x03, {0x3F00, 0x7FFF, 0x6F4E, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to FDNURI */
  {{0x03, {0x3F00, 0x7FFF, 0x6FED, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to BDNURI */
  {{0x03, {0x3F00, 0x7FFF, 0x6FEE, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MBDN */
  {{0x03, {0x3F00, 0x7FFF, 0x6FC7, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to EXT6 */
  {{0x03, {0x3F00, 0x7FFF, 0x6FC8, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MBI */
  {{0x03, {0x3F00, 0x7FFF, 0x6FC9, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MWIS */
  {{0x03, {0x3F00, 0x7FFF, 0x6FCA, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MMSUP */
  {{0x03, {0x3F00, 0x7FFF, 0x6FD1, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to GBABP */
  {{0x03, {0x3F00, 0x7FFF, 0x6FD6, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to WRI */
  {{0x04, {0x3F00, 0x7FFF, 0x5F40, 0x4F46, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PROSE_GC */
  {{0x04, {0x3F00, 0x7FFF, 0x5F90, 0x4F09, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ICE_graphics */
  {{0x04, {0x3F00, 0x7F10, 0x5F50, 0x4F21, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MMDF */
  {{0x04, {0x3F00, 0x7F10, 0x5F3B, 0x4F48, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MML */
  {{0x04, {0x3F00, 0x7F10, 0x5F3B, 0x4F47, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ACSGL */
  {{0x04, {0x3F00, 0x7FFF, 0x5F50, 0x4F81, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to HNBN */
  {{0x04, {0x3F00, 0x7FFF, 0x5F50, 0x4F83, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ICE_FF */
  {{0x03, {0x3F00, 0x7F10, 0x6FE1, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PSC */
  {{0x04, {0x3F00, 0x7FFF, 0x5F3A, 0x4F22, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CC */
  {{0x04, {0x3F00, 0x7FFF, 0x5F3A, 0x4F23, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PUID */
  {{0x04, {0x3F00, 0x7FFF, 0x5F3A, 0x4F24, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths GSM file paths under secure logging category*/
const mmgsdi_masking_path_type mmgsdi_masking_path_sim_tbl_secure[] =
{
  /* Path to KC */
  {{0x03, {0x3F00, 0x7F20, 0x6F20, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to KCGPRS */
  {{0x03, {0x3F00, 0x7F20, 0x6F52, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths GSM file paths under private logging category*/
const mmgsdi_masking_path_type mmgsdi_masking_path_sim_tbl_private[] =
{
  /* Path to LOCI */
  {{0x03, {0x3F00, 0x7F20, 0x6F7E, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_loci},
  /* Path to FDN */
  {{0x03, {0x3F00, 0x7F10, 0x6F3B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_fdn_ice_dn},
  /* Path to BDN */
  {{0x03, {0x3F00, 0x7F10, 0x6F4D, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_bdn},
  /* Path to EXT2 */
  {{0x03, {0x3F00, 0x7F10, 0x6F4B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_ext2_ext4},
  /* Path to EXT4 */
  {{0x03, {0x3F00, 0x7F10, 0x6F4E, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_ext2_ext4},
  /* Path to LP */
  {{0x03, {0x3F00, 0x7F20, 0x6F05, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CBMI */
  {{0x03, {0x3F00, 0x7F20, 0x6F45, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CBMIR */
  {{0x03, {0x3F00, 0x7F20, 0x6F50, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MBDN */
  {{0x03, {0x3F00, 0x7F20, 0x6FC7, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MBI */
  {{0x03, {0x3F00, 0x7F20, 0x6FC9, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MWIS */
  {{0x03, {0x3F00, 0x7F20, 0x6FCA, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to EXT6 */
  {{0x03, {0x3F00, 0x7F20, 0x6FC8, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to SMS */
  {{0x03, {0x3F00, 0x7F10, 0x6F3C, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths CSIM file paths under secure logging category*/
const mmgsdi_masking_path_type mmgsdi_masking_path_csim_tbl_secure[] =
{
  /* Path to 3GCIK */
  {{0x03, {0x3F00, 0x7FFF, 0x6F6B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to Hidden Key */
  {{0x03, {0x3F00, 0x7FFF, 0x6F76, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to RC */
  {{0x03, {0x3F00, 0x7FFF, 0x6F82, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths CSIM file paths under private logging category */
const mmgsdi_masking_path_type mmgsdi_masking_path_csim_tbl_private[] =
{
  /* Path to EXT2 */
  {{0x03, {0x3F00, 0x7FFF, 0x6F7A, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_ext2_ext4},
  /* Path to FDN */
  {{0x03, {0x3F00, 0x7FFF, 0x6F3B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_fdn_ice_dn},
  /* Path to LI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F3A, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to SMS */
  {{0x03, {0x3F00, 0x7FFF, 0x6F3C, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to ICI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F7C, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to OCI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F7D, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to EXT5 */
  {{0x03, {0x3F00, 0x7FFF, 0x6F7E, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MMSUP */
  {{0x03, {0x3F00, 0x7FFF, 0x6F68, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to TMSI */
  {{0x03, {0x3F00, 0x7FFF, 0x6F24, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PSC */
  {{0x04, {0x3F00, 0x7F10, 0x5F3A, 0x4F22, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to CC */
  {{0x04, {0x3F00, 0x7F10, 0x5F3A, 0x4F23, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to PUID */
  {{0x04, {0x3F00, 0x7F10, 0x5F3A, 0x4F24, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths RUIM file paths under secure logging category*/
const mmgsdi_masking_path_type mmgsdi_masking_path_ruim_tbl_secure[] =
{
  /* Path to 3GCIK */
  {{0x03, {0x3F00, 0x7F25, 0x6F6B, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to RC */
  {{0x03, {0x3F00, 0x7F25, 0x6F91, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};

/* Table for file paths RUIM file paths under private logging category*/
const mmgsdi_masking_path_type mmgsdi_masking_path_ruim_tbl_private[] =
{
  /* Path to SMS */
  {{0x03, {0x3F00, 0x7F25, 0x6F3C, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all},
  /* Path to MMSUP */
  {{0x03, {0x3F00, 0x7F25, 0x6F68, 0xFFFF, 0xFFFF}}, mmgsdi_log_masking_fn_all}
};


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_KEYS

   DESCRIPTION
      This function sets the masking function for KEYS

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_keys(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
)
{

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* The first byte of the file contains the KSI and we want to
     display it correctly, as it can be useful, while
     the rest of the value needs to be masked. Content
     needs to be masked also when KSI indicates that
     valid key is not available, as the file might contain
     previous values used in the past */
  memset(output_ptr, 0xFF, length);
  output_ptr[0] = input_ptr[0];

  return TRUE;
} /* mmgsdi_log_masking_fn_keys */


/* ----------------------------------------------------------------------------
   FUNCTION: MMGSDI_LOG_MASKING_FN_ALL
 
   DESCRIPTION
      This function masks all bytes of APDU
 
   DEPENDENCIES:
   None
 
   LIMITATIONS:
   None
 
   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set
 
   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_all(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
)
{

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  memset(output_ptr, 0xFF, length);

  return TRUE;
} /* mmgsdi_log_masking_fn_all */


/* ----------------------------------------------------------------------------
   FUNCTION: MMGSDI_LOG_MASKING_FN_AUTH_RESP
 
   DESCRIPTION
      This function sets the masking function for AUTH response
 
   DEPENDENCIES:
   None
 
   LIMITATIONS:
   None
 
   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set
 
   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_auth_resp(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
)
{
  uint8  resp_tag = 0;

  if (output_ptr == NULL || input_ptr == NULL || length == 0) 
  {
    return FALSE;
  }

  resp_tag = input_ptr[0];

  /* Check for successful response tag. If the
     response is a failure, just return*/
  if(resp_tag != 0xDB)
  {
    return FALSE;
  }

  memset(output_ptr, 0xFF, length);
  output_ptr[0] = resp_tag;

  return TRUE;
} /* mmgsdi_log_masking_fn_auth_resp */


/* ----------------------------------------------------------------------------
   FUNCTION: MMGSDI_LOG_MASKING_FN_MBMS_AUTH_RESP 
 
   DESCRIPTION
      This function sets the masking function for MBMS AUTH response
 
   DEPENDENCIES:
   None
 
   LIMITATIONS:
   None
 
   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set
 
   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_mbms_auth_resp(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
)
{
  uint8          mbms_encoding_first_byte   = 0xFF;
  uint8          mbms_resp_bytes            = 0x00;
  
  /* Length check needed to check the byte at second position in
     input_ptr and get the information about following bytes*/
  if (input_ptr == NULL || output_ptr == NULL || length < 2)
  {
    return FALSE;
  }

  /*---------------------------------------------------------------
    Tag for BER TLV according to 102 331 Section 11.3 -
    First byte      Following bytes            Encoded length value
    1. '00' to '7F'      none                     0     to 127
    2. '81'             '80' to 'FF'              128   to 255
    3. '82'             '01 00' to 'FF FF'        256   to 65 535
    4. '83'             '01 00 00' to 'FF FF FF'  65536 to 16777215
    ---------------------------------------------------------------*/

  mbms_encoding_first_byte = input_ptr[1];

  if (mbms_encoding_first_byte >= ((0x00)) &&
      mbms_encoding_first_byte <= 0x7F) 
  {
    /* Bytes to copy are, data Tag, response length and  operation tag */
    mbms_resp_bytes = 0x03;
  }
  else if (mbms_encoding_first_byte == 0x81) 
  {
    /* Bytes to copy are, data tag, response length and operation tag = 1+2+1*/
    mbms_resp_bytes = 0x04;
  }
  else if (mbms_encoding_first_byte == 0x82) 
  {
    /* Bytes to copy are, data tag, response length and operation tag = 1+3+1*/
    mbms_resp_bytes = 0x05;
  }
  else if (mbms_encoding_first_byte == 0x83) 
  {
    /* Bytes to copy are, data tag, response length and operation tag = 1+4+1*/
    mbms_resp_bytes = 0x06;
  }

  /* Determine the number of bytes we can copy based on the response */
  if(mbms_resp_bytes > 0 && mbms_resp_bytes < length) 
  {
    mmgsdi_memscpy(output_ptr, length, input_ptr, mbms_resp_bytes);
    mmgsdi_memscpy(&output_ptr[mbms_resp_bytes - 1], length, &input_ptr[mbms_resp_bytes - 1], 0x01);
    memset(output_ptr + mbms_resp_bytes, 0xFF, (length - mbms_resp_bytes));
  }
    
  return TRUE;
} /* mmgsdi_log_masking_fn_mbms_auth_resp */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_EXT2_EXT4

   DESCRIPTION
      This function sets the masking function for EXT2 AND EXT4

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_ext2_ext4(
  const uint8                      *input_ptr,
  uint8                            *output_ptr,
  uint32                            length
)
{
  if (input_ptr == NULL || output_ptr == NULL || length < MMGSDI_EXT2_EXT4_LEN)
  {
    return FALSE;
  }

  /* Mask everything but first and last byte */
  output_ptr[0] = input_ptr[0];
  memset(output_ptr + 1, 0xFF, length - 2);
  output_ptr[length - 1] = input_ptr[length - 1];

  return TRUE;
} /* mmgsdi_log_masking_fn_ext2_ext4 */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_FDN_ICE_DN

   DESCRIPTION
      This function sets the masking function for FDN and ICE_DN

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_fdn_ice_dn(
  const uint8              *input_ptr,
  uint8                    *output_ptr,
  uint32                    length
)
{
  /* Length of (optional field) alpha identifier is X in spec, there are 14 bytes after alpha */
  uint8  alpha_len = 0;

  if (input_ptr == NULL || output_ptr == NULL || length < MMGSDI_FDN_ICE_DN_MIN_LEN)
  {
    return FALSE;
  }

  memset(output_ptr, 0xFF, length);
  if (length > MMGSDI_FDN_ICE_DN_MIN_LEN)
  {
    alpha_len = length - MMGSDI_FDN_ICE_DN_MIN_LEN;
  }
  /* leave X+1 and X+2 unmasked */
  mmgsdi_memscpy(output_ptr + alpha_len, MMGSDI_FDN_ICE_DN_MIN_LEN, input_ptr + alpha_len, 0x02);
  /* leave X+13 and X+14 unmasked */
  mmgsdi_memscpy(output_ptr + (length - 2), 0x02, input_ptr + (length - 2), 0x02);

  return TRUE;
}  /* mmgsdi_log_masking_fn_fdn_ice_dn */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_LOCI

   DESCRIPTION
      This function sets the masking function for LOCI

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_loci(
  const uint8                       *input_ptr,
  uint8                             *output_ptr,
  uint32                             length
)
{
  if (input_ptr == NULL || output_ptr == NULL || length < MMGSDI_LOCI_LEN)
  {
    return FALSE;
  }

  /* Mask TMSI and LAI */
  memset(output_ptr, 0xFF, 9);
  mmgsdi_memscpy(output_ptr + 9, length - 9, input_ptr + 9, length - 9);

  return TRUE;
} /* mmgsdi_log_masking_fn_loci */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_BDN

   DESCRIPTION
      This function sets the masking function for BDN

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_bdn(
  const uint8                     *input_ptr,
  uint8                           *output_ptr,
  uint32                           length
)
{
  /* Length of (optional field) alpha identifier is X in spec, there are 15 bytes after alpha */
  uint8  alpha_len = 0;

  if (input_ptr == NULL || output_ptr == NULL || length < MMGSDI_BDN_MIN_LEN)
  {
    return FALSE;
  }

  memset(output_ptr, 0xFF, length);
  if (length > MMGSDI_BDN_MIN_LEN)
  {
    alpha_len = length - MMGSDI_BDN_MIN_LEN;
  }
  /* leave X+1 and X+2 unmasked */
  mmgsdi_memscpy(output_ptr + alpha_len, MMGSDI_BDN_MIN_LEN, input_ptr + alpha_len, 0x02);
  /* leave X+13, X+14, X+15 unmasked */
  mmgsdi_memscpy(output_ptr + (length - 3), 0x03, input_ptr + (length - 3), 0x02);

  return TRUE;
} /* mmgsdi_log_masking_fn_bdn */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_EPSLOCI

   DESCRIPTION
      This function sets the masking function for EPSLOCI

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_epsloci(
  const uint8                           *input_ptr,
  uint8                                 *output_ptr,
  uint32                                 length
)
{
  if (input_ptr == NULL || output_ptr == NULL || length < MMGSDI_EPSLOCI_LEN)
  {
    return FALSE;
  }

  /* Mask GUTI and TAI */
  memset(output_ptr, 0xFF, length - 1);
  output_ptr[length - 1] = input_ptr[length - 1];

  return TRUE;
} /* mmgsdi_log_masking_fn_epsloci */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_MASKING_FN_PSEUDO

   DESCRIPTION
      This function sets the masking function for PSEUDO

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static boolean mmgsdi_log_masking_fn_pseudo(
  const uint8                          *input_ptr,
  uint8                                *output_ptr,
  uint32                                length
)
{
  if (input_ptr == NULL || output_ptr == NULL || length < 2)
  {
    return FALSE;
  }

  mmgsdi_memscpy(output_ptr, length, input_ptr, 0x02);
  memset(output_ptr + 2, 0xFF, length - 2);

  return TRUE;
} /* mmgsdi_log_masking_fn_pseudo */

/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOG_SET_EF_MASKING_FN

   DESCRIPTION
      This function sets the EF masking function

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
static void mmgsdi_log_set_ef_masking_fn(
  uim_cmd_type                    *uim_cmd_ptr,
  const uim_path_type             *uim_path_ptr,
  const mmgsdi_masking_path_type  *masking_path_tbl_ptr,
  uint16                           table_size
)
{
  uint16             i              = 0;
  uim_cmd_name_type  cmd_name_type  = UIM_NO_SUCH_COMMAND_F;

  if (uim_cmd_ptr == NULL)
  {
    return;
  }

  cmd_name_type  = uim_cmd_ptr->hdr.command;

  for (i = 0; i < table_size; i++)
  {
    if ((masking_path_tbl_ptr[i].path.len == uim_path_ptr->len) &&
        (memcmp(&uim_path_ptr->path,
                &masking_path_tbl_ptr[i].path.path,
                sizeof(masking_path_tbl_ptr[i].path.path)) == 0))
    {
      if (cmd_name_type == UIM_READ_BIN_CMD_F ||
          cmd_name_type == UIM_READ_REC_CMD_F )
      {
        /* For read command, only the response has to be masked */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = masking_path_tbl_ptr[i].masking_fn_ptr;
      }
      else if (cmd_name_type == UIM_WRITE_BIN_CMD_F ||
               cmd_name_type == UIM_WRITE_REC_CMD_F )
      {
        /* For write command, only the request has to be masked */
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = masking_path_tbl_ptr[i].masking_fn_ptr;
      }
      break;
    }
  }
} /* mmgsdi_log_set_ef_masking_fn */


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOGGING_POPULATE_MASKING_FN_PTR

   DESCRIPTION
     This function populates the appropriate function pointer based on the
     command and file type

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
-------------------------------------------------------------------------------*/
void mmgsdi_logging_populate_masking_fn_ptr(
  uim_cmd_type                        *uim_cmd_ptr,
  mmgsdi_app_enum_type                 app_type
)
{
  uint16                             secure_table_size                = 0;
  uint16                             private_table_size               = 0;
  uim_path_type                     *uim_path_ptr                     = NULL;
  const mmgsdi_masking_path_type    *secure_masking_path_tbl_ptr      = NULL;
  const mmgsdi_masking_path_type    *private_masking_path_tbl_ptr     = NULL;
  mmgsdi_slot_id_enum_type           slot                             = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_return_enum_type            mmgsdi_status                    = MMGSDI_ERROR;
  uim_cmd_name_type                  cmd_name_type;
  boolean                            is_test_mode_secure_log_enabled  = FALSE;
  boolean                            is_test_mode_private_log_enabled = FALSE;

  if (uim_cmd_ptr == NULL)
  {
    return;
  }

  mmgsdi_status = mmgsdi_util_convert_from_uim_slot(uim_cmd_ptr->hdr.slot, &slot);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return;
  }

  /* If it is a test card, no need to populate the function pointers,
     because there is no need to mask some bytes of the APDUs */
  is_test_mode_secure_log_enabled = mmgsdi_is_test_mode_logging_enabled_for_slot(slot);
  is_test_mode_private_log_enabled = mmgsdi_is_private_logging_enabled_for_slot(slot);
  if(is_test_mode_secure_log_enabled && is_test_mode_private_log_enabled)
  {
    return;
  }

  cmd_name_type = uim_cmd_ptr->hdr.command;

  switch (cmd_name_type)
  {
    case UIM_STREAM_APDU_F:
    case UIM_STREAM_ISO7816_APDU_F:
      if (is_test_mode_private_log_enabled == FALSE)
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_READ_BIN_CMD_F:
      uim_path_ptr = &uim_cmd_ptr->read_bin.path;
      break;

    case UIM_READ_REC_CMD_F:
      uim_path_ptr = &uim_cmd_ptr->read_rec.path;
      break;

    case UIM_WRITE_BIN_CMD_F:
      uim_path_ptr = &uim_cmd_ptr->write_bin.path;
      break;

    case UIM_WRITE_REC_CMD_F:
      uim_path_ptr = &uim_cmd_ptr->write_rec.path;
      break;

    case UIM_INC_CMD_F:
      uim_path_ptr = &uim_cmd_ptr->inc.path;
      break;

    case UIM_RUN_GSM_ALGO_F:
      uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      return;

    case UIM_VERIFY_CHV_F:
      if (is_test_mode_private_log_enabled == FALSE &&
          strnlen((const char *)uim_cmd_ptr->verify_chv.chv_digits, UIM_MAX_CHV_DIGITS) > 0)
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_CHANGE_CHV_F:
      if (is_test_mode_private_log_enabled == FALSE &&
          (strnlen((const char *)uim_cmd_ptr->change_chv.chv_digits, UIM_MAX_CHV_DIGITS) > 0 ||
           strnlen((const char *)uim_cmd_ptr->change_chv.new_chv_digits, UIM_MAX_CHV_DIGITS) > 0))
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_ENABLE_CHV_F:
      if (is_test_mode_private_log_enabled == FALSE &&
          strnlen((const char *)uim_cmd_ptr->enable_chv.chv_digits, UIM_MAX_CHV_DIGITS) > 0)
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_DISABLE_CHV_F:
      if (is_test_mode_private_log_enabled == FALSE &&
          strnlen((const char *)uim_cmd_ptr->disable_chv.chv_digits, UIM_MAX_CHV_DIGITS) > 0)
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_UNBLOCK_CHV_F:
      if (is_test_mode_private_log_enabled == FALSE  &&
          (strnlen((const char *)uim_cmd_ptr->unblk_chv.chv_digits, UIM_MAX_CHV_DIGITS) > 0 ||
           strnlen((const char *)uim_cmd_ptr->unblk_chv.new_chv_digits, UIM_MAX_CHV_DIGITS) > 0))
      {
        uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      return;

    case UIM_AUTHENTICATE_F:
      if (uim_cmd_ptr->autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
          uim_cmd_ptr->autn.ref_data_number == 0x00)
      {
        /* GSM context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      }
      else if (uim_cmd_ptr->autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
               uim_cmd_ptr->autn.ref_data_number == 0x01)
      {
        /* 3G context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      else if (uim_cmd_ptr->autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
                uim_cmd_ptr->autn.ref_data_number == 0x02)
      {
        /* VGCS/VBS context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      else if (uim_cmd_ptr->autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
               uim_cmd_ptr->autn.ref_data_number == 0x04)
      {
        /* GBA context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      else if (uim_cmd_ptr->autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
               uim_cmd_ptr->autn.ref_data_number == 0x05)
      {
        /* MBMS context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_mbms_auth_resp;
      }
      return;

    case UIM_AUTHENTICATE_ODD_INS_F:
      if (uim_cmd_ptr->autn_odd_ins.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
          uim_cmd_ptr->autn_odd_ins.ref_data_number == 0x05)
      {
        /* MBMS context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_mbms_auth_resp;
        return;
      }
      break;

    case UIM_ISIM_AUTHENTICATE_F:
      if (uim_cmd_ptr->isim_autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
          uim_cmd_ptr->isim_autn.ref_data_number == 0x01)
      {
        /* IMS AKA context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      else if (uim_cmd_ptr->isim_autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
               uim_cmd_ptr->isim_autn.ref_data_number == 0x02)
      {
        /* HTTP Digest context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      else if (uim_cmd_ptr->isim_autn.ref_data_type == UIM_AUTN_SPECIFIC_REF_DATA &&
               uim_cmd_ptr->isim_autn.ref_data_number == 0x04)
      {
        /* GBA context */
        uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_auth_resp;
      }
      return;

    case UIM_RUN_CAVE_F:
      uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = mmgsdi_log_masking_fn_all;
      return;

    default:
     /* Other commands do not need log masking */
     return;
  }

  /* Assign masking table ptrs based on app and secure/private logging groups*/
  switch (app_type)
  {
    case MMGSDI_APP_USIM:
      secure_masking_path_tbl_ptr = mmgsdi_masking_path_usim_tbl_secure;
      secure_table_size = sizeof(mmgsdi_masking_path_usim_tbl_secure) / sizeof(mmgsdi_masking_path_type);
      private_masking_path_tbl_ptr = mmgsdi_masking_path_usim_tbl_private;
      private_table_size = sizeof(mmgsdi_masking_path_usim_tbl_private) / sizeof(mmgsdi_masking_path_type);
      break;

    case MMGSDI_APP_CSIM:
      secure_masking_path_tbl_ptr = mmgsdi_masking_path_csim_tbl_secure;
      secure_table_size = sizeof(mmgsdi_masking_path_csim_tbl_secure) / sizeof(mmgsdi_masking_path_type);
      private_masking_path_tbl_ptr = mmgsdi_masking_path_csim_tbl_private;
      private_table_size = sizeof(mmgsdi_masking_path_csim_tbl_private) / sizeof(mmgsdi_masking_path_type);
      break;

    case MMGSDI_APP_SIM:
      secure_masking_path_tbl_ptr = mmgsdi_masking_path_sim_tbl_secure;
      secure_table_size = sizeof(mmgsdi_masking_path_sim_tbl_secure) / sizeof(mmgsdi_masking_path_type);
      private_masking_path_tbl_ptr = mmgsdi_masking_path_sim_tbl_private;
      private_table_size = sizeof(mmgsdi_masking_path_sim_tbl_private) / sizeof(mmgsdi_masking_path_type);
      break;

    case MMGSDI_APP_RUIM:
      secure_masking_path_tbl_ptr = mmgsdi_masking_path_ruim_tbl_secure;
      secure_table_size = sizeof(mmgsdi_masking_path_ruim_tbl_secure) / sizeof(mmgsdi_masking_path_type);
      private_masking_path_tbl_ptr = mmgsdi_masking_path_ruim_tbl_private;
      private_table_size = sizeof(mmgsdi_masking_path_ruim_tbl_private) / sizeof(mmgsdi_masking_path_type);
      break;

    default:
      break;
  }

  if (uim_path_ptr == NULL || secure_masking_path_tbl_ptr == NULL || private_masking_path_tbl_ptr == NULL)
  {
    return;
  }

  /* Set the masking functions for EFs in secure and private logging groups*/
  if (is_test_mode_private_log_enabled == FALSE)
  {
    mmgsdi_log_set_ef_masking_fn(uim_cmd_ptr, uim_path_ptr, private_masking_path_tbl_ptr, private_table_size);
  }
  if (is_test_mode_secure_log_enabled == FALSE)
  {
    mmgsdi_log_set_ef_masking_fn(uim_cmd_ptr, uim_path_ptr, secure_masking_path_tbl_ptr, secure_table_size);
  }
} /* mmgsdi_logging_populate_masking_fn_ptr */
