/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.

 * rfs_gateway_errno.h
=============================================================================*/

#ifndef __GATEWAY_ERRNO_H__
#define __GATEWAY_ERRNO_H__


#define FS_GATEWAY_SUCESS    			     (0)    /*<No error*/
#define FS_GATEWAY_SECURITY_SUCCESS          (0)    /*<gateway secuirty auth sucessful*/
#define FS_GATEWAY_HASH_SEGMENT_NOT_FOUND    (-17)  /*<No hash segment found in the file*/
#define FS_GATEWAY_AUTH_FUSE_DISABLED        (-18)  /*<Secboot auth fuses disabled*/
#define FS_GATEWAY_CERT_CHAIN_VERF_FAIL      (-19)  /*<CSMS certificate chain verification failed*/
#define FS_GATEWAY_HASH_VERF_FAIL            (-20)  /*<CSMS certificate chain verification failed*/
#define FS_GATEWAY_MALLOC_FAIL    	         (-21)  /*<malloc failed*/
#define FS_GATEWAY_ZERO_FILESZ    	         (-22)  /*<bad file*/
#define FS_GATEWAY_MAP_FAIL      	         (-23)  /*<memory map op fail*/
#define FS_GATEWAY_PERM_CHG_FAIL      	     (-24)  /*<memory map op fail*/
#define FS_GATEWAY_BAD_ELF      	         (-25)  /*<bad elf*/
#define GATEWAY_HASH_SEG_NOT_FOUND      	 (-26)  /*<non-secure ELF*/
#define FS_GATEWAY_HDR_COPY_FAIL         	 (-27)  /*<header copy failed*/
#define FS_GATEWAY_MAX_NUM_FILES_OPEN        (-28)  /*<gateway has opened max num of files it can*/
#define FS_GATWAY_RAMFS_INSTALL_FAIL         (-29)  /*<gateway failed to instll file in RAMFS*/
#define FS_GATEWAY_BAD_HASH_SEG              (-30)  /*<Bad hash segment*/
#define FS_GATEWAY_AUTH_FAIL                 (-31)  /*<MBN auth failed*/
#define FS_GATEWAY_STR_CONCAT_FAIL           (-32)  /*<String conacat failed*/
#define FS_GATEWAY_BAD_PATH                  (-33)  /*<String conacat failed*/
#define FS_GATEWAY_FSTAT_FAIL                (-34)  /*<Cannot fstat file*/
#define FS_GATEWAY_INIT_FAIL                 (-35)  /*<Gateway not initialized*/
#define FS_GATEWAY_SECURITY_FAIL_BAD_PARAM   (-36)  /*<Gateway security bad prarmeter*/
#define FS_GATEWAY_SECURITY_FAIL_THERAD      (-37)  /*<Gateway security cant spawn thread*/
#define FS_GATEWAY_SECURITY_FAIL_THERAD_JN   (-38)  /*<Gateway security cant join thread*/
  

#endif

