/*===========================================================================

FILE:  secdbg_fuse.c

DESCRIPTION:
  Update the fuse information for EFS-tar and MCFG authentication

EXTERNALIZED FUNCTIONS


Copyright (c) 2013-2014, 2019 - 2020 by Qualcomm Technologies, Inc. All Rights Reserved.
===========================================================================*/

/*==========================================================================

           Include Files for Module

==========================================================================*/
#include "secboot_util.h"
#include <stringl.h>
#include "secboot_hw.h"

#define SOC_VERS_FAMILY_DEVICE_NUM_SHIFT (0x10) // Use family and device number for soc hw version

#define AGATTI_TCSR_SOC_HW_DEVICE_NUM	0x9003

#define SIGNED_IMG_RSA_256_ROOT_HASH \
 {0xbd, 0xaf, 0x51, 0xb5, 0x9b, 0xa2, 0x1d, 0x8a, \
  0x24, 0x37, 0x92, 0xc0, 0xe1, 0x83, 0xe8, 0x8b, \
  0xdd, 0xd3, 0x69, 0xcc, 0xca, 0x58, 0xbc, 0x79, \
  0x2a, 0x3e, 0x4c, 0x22, 0xef, 0xf3, 0x29, 0xe8, \
  0xa8, 0xc7, 0x2d, 0x44, 0x95, 0x59, 0xcd, 0x5f, \
  0x09, 0xeb, 0xfa, 0x5c, 0x7b, 0xf3, 0x98, 0xc0}
	
#define SIGNED_IMG_ECC_384_ROOT_HASH \
 {0xd9, 0x35, 0x7d, 0xb8, 0x87, 0x95, 0xb5, 0xa8, \
  0xaf, 0xae, 0xbf, 0xd9, 0xab, 0x08, 0xa5, 0x69, \
  0xcc, 0x8e, 0x51, 0x9f, 0x6c, 0x68, 0x97, 0x23, \
  0x75, 0x9f, 0x4e, 0x69, 0x15, 0xca, 0x34, 0x66, \
  0xe9, 0x8b, 0x5a, 0x32, 0x82, 0x67, 0x8b, 0xdf, \
  0x63, 0x67, 0x3d, 0x85, 0x17, 0xbb, 0x0c, 0x5b}

#define SIGNED_IMG_MSM_HW_ID    0x0000000000000000
#define FEATURE_EFS_DISABLE_AUTH_IN_NON_SECURE	0		

static uint8 fs_rsa_256_root_hash[] = SIGNED_IMG_RSA_256_ROOT_HASH;
static uint8 fs_ecc_384_root_hash[] = SIGNED_IMG_ECC_384_ROOT_HASH;

uint32 secboot_util_get_feature_efs_auth_disable_in_non_secure( void )
{
	return 	FEATURE_EFS_DISABLE_AUTH_IN_NON_SECURE;
}
secboot_util_error_type secboot_util_update_fuse_info(secboot_util_fuse_info_type * fuse_info)
{
	uint32 soc_hw_ver_family_device_num = 0;
	uint32 soc_hw_version = 0;

	if (fuse_info == NULL)
		return E_SECBOOT_UTIL_INVALID_PARAM;
	
	if (E_SECBOOT_HW_SUCCESS != secboot_hw_get_soc_hw_version(&soc_hw_ver_family_device_num))
	{
		return E_SECBOOT_UTIL_HW_FAIL;
	}
	soc_hw_version = (soc_hw_ver_family_device_num >> SOC_VERS_FAMILY_DEVICE_NUM_SHIFT);
	
	/* Agatti uses ECC SHA384 and rest all targets supported in this build uses RSA SHA256 for Root Hash */
	if (AGATTI_TCSR_SOC_HW_DEVICE_NUM == soc_hw_version)
	{
        memscpy(fuse_info->root_of_trust,sizeof(fuse_info->root_of_trust),&fs_ecc_384_root_hash[0],sizeof(fs_ecc_384_root_hash));
	}
	else 
	{
        memscpy(fuse_info->root_of_trust,sizeof(fuse_info->root_of_trust),&fs_rsa_256_root_hash[0],sizeof(fs_rsa_256_root_hash));
	}
	fuse_info->msm_hw_id = SIGNED_IMG_MSM_HW_ID;
	fuse_info->auth_use_serial_num = 0;
	fuse_info->serial_num = 0x0;
	fuse_info->use_root_of_trust_only = FALSE;
	fuse_info->mrc_fuse_info.is_root_sel_enabled = 0;	
	fuse_info->mrc_fuse_info.num_root_certs = 0;
	fuse_info->mrc_fuse_info.activation_list = 0;
	fuse_info->mrc_fuse_info.revocation_list = 0;
	
	return E_SECBOOT_UTIL_SUCCESS;
}