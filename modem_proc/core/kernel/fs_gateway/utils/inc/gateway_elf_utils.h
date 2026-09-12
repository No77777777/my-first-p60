/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.

 * rfs_gateway_elf_utils.h
=============================================================================*/

#ifndef __GATEWAY_ELF_UTILS_H__
#define __GATEWAY_ELF_UTILS_H__

#include "comdef.h"
#include "gateway_verify.h"

#define CATCH(exception) exception##bail: if (exception != 0)
#define THROW(m)  do{ nErr=-1; goto bail; } while(0)

// The segment type is stored in bits 24-26 of the p_flags member of the
// Program Header (Elf32_Phdr).  The value 2 of this field identfies the
// segment as the hash segment.
#define SEGMENT_TYPE_MASK 0x07000000
#define GET_SEGMENT_TYPE(x) ((x) & SEGMENT_TYPE_MASK)
#define HASH_SEGMENT 0x02000000
#define ELFINFO_MAGIC_SIZE (16)

#endif