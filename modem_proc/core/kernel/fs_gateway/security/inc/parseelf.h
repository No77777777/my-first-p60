/*=============================================================================

FILE:       parseelf.h

SERVICES:   

==============================================================================
        Copyright (c) 2013-2014 Qualcomm Technologies Incorporated.
                   All Rights Reserved.
            QUALCOMM Proprietary and Confidential
==============================================================================*/
#ifndef __PARSE_ELF_H__
#define __PARSE_ELF_H__

#include "comdef.h"

typedef struct {
  uint32 image_id; /* ignored */
  uint32 header_vsn_num; /* ignored */
  uint32 image_src; /* ignored */
  uint8* image_dest_ptr; /* ignored */
  uint32 image_size; /* Size of complete image in bytes */
  uint32 code_size; /* Size of hashes region in the hash table segment */
  uint8* signature_ptr; /* ignored */
  uint32 signature_size; /* Size of the attestation signature in
                             bytes */
  uint8* cert_chain_ptr; /* ignored */
  uint32 cert_chain_size; /* Size of the attestation chain in bytes */
} HashMBNHeader;

#endif   //__PARSE_ELF_H__

