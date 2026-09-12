/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.

 * rfs_gateway_errno.h
=============================================================================*/

#ifndef __GATEWAY_SECURITY_H__
#define __GATEWAY_SECURITY_H__

#include "gateway_elf_utils.h"
#include "gateway_errno.h" 
#include "SigVerify.h"
#include "miprogressive.h"

/**
 A gaetway driver should call this function to authenticate a file from RFS.
 The file to be authenticated must be a valid signed ELF file.

 This function -
 1. looks up for a hash segment.
 2. If secureboot fuses are blown, authenricate the hash segment with secureMSM APIs.
 3. Generate, compare and match hashes of every segement within ELF.

 @param so_name
    name of the file to authenticate

 @param pEhdr
 	Address of ELF header for the given ELF file.

*/
int gateway_security_do_auth(const char* so_name, Elf32_Ehdr* pEhdr);

#endif