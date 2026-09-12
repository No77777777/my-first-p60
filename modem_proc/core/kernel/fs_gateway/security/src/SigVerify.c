/*=============================================================================

FILE:       SigVerify.c

SERVICES:   API for authenticating dynamic modules using RSA signatures and
            an X509 certificate chain embedded in the file.

==============================================================================
        Copyright (c) 2016 Qualcomm Technologies Incorporated.
                   All Rights Reserved.
            QUALCOMM Proprietary and Confidential
==============================================================================*/
#include "HAP_farf.h"

#include "secapi.h"
#include "secboot_util.h"
#include "secboot.h"
#include "secboot_hw.h"

#include "verify.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "SigVerify.h"
#include "parseelf.h"

#include "secboot.h"
#include "secboot_util.h"
#include "platform_libs.h"
#include "util.h"
#include "miprogressive.h"

#define MAX_DIGEST_SIZE 32
#define SIGVERIFY_DEBUG printf
#define SIGVERIFY_ERROR printf
//#define SIGVERIFY_DEBUG (...)
//#define SIGVERIFY_ERROR (...)
/*This structure stores the auth info for the given ELF*/
typedef struct {
   char* so_name;
   void* pHash; //Start of hash segment
   void* pHash_table_start; //Start of table of hash values
   int nDigestSize;
   int mode;
} SigVerify_inst;


#ifndef SIGVERIFY_SIGNED_IMG_ROOT_HASH
  #define SIGVERIFY_SIGNED_IMG_ROOT_HASH \
   {0xb5, 0x3f, 0xb2, 0x3d, 0x19, 0x53, 0xde, 0xcb, \
    0x95, 0x92, 0x8f, 0xe6, 0x57, 0x55, 0x6c, 0xea, \
    0x6e, 0xda, 0xb3, 0x44, 0x4d, 0xc7, 0x08, 0xc0, \
    0x19, 0x05, 0x7c, 0xba, 0xf8, 0xc6, 0x2d, 0x4a}
#endif


   
#ifndef SIGVERIFY_SIGNED_IMG_ROOT_HASH_2
  #define SIGVERIFY_SIGNED_IMG_ROOT_HASH_2 \
   {0xf8, 0xab, 0x20, 0x52, 0x63, 0x58, 0xc4, 0xfa, \
    0x4c, 0xef, 0x96, 0xd7, 0x8c, 0x45, 0x18, 0x0d, \
    0xc3, 0xdb, 0x75, 0xe8, 0xf2, 0x40, 0x51, 0xad, \
    0x62, 0x44, 0x48, 0xc1, 0x34, 0xb4, 0xe8, 0x61} 
#endif

static uint8 sigverify_root_hash[] = SIGVERIFY_SIGNED_IMG_ROOT_HASH_2;

uint8* sigverify_get_root_hash (uint32 *hash_len)
{
  *hash_len = sizeof (sigverify_root_hash);
  return &sigverify_root_hash[0];
}
/** \details
 * Generate hash for the data within given buffer.
 *
 * @param[in] pSeg
 *    Pointer to segment start.
 *
 * @param[in] cbSeg
 *    Size of the segment
 *
 * @param[in] digest_out
 *    Output buffer to store generated hash value.
 *
 * @param[in] nDigestSize
 *    Algorithm used to generate hash. (SHA256/SHA1)
 *
 * @return 
 *      SIGVERIFY_EOK      - Successful hash generation.
 *      SIGVERIFY_FAIL_XXX - Failure during hash generation with error code.
 *
 */
secapi_handle_type          sh;
int sh_init_done;

static int SigVerify_GetSegmentHash(void* pSeg, int cbSeg, void* digest_out, int nDigestSize)
{

    /*Crypto variables*/
    secapi_algo_enum_type       hash_algo;
    secerrno_enum_type          stat;
    secapi_hsh_param_data_type  hash_param;

    /*Input sanity*/
    if(NULL == pSeg || NULL == digest_out || 0 == cbSeg)
    {
      return SIGVERIFY_FAIL_BAD_INPUTS;
    }

    /*Digest size validity*/
    if(nDigestSize == 20) 
    {
        hash_algo = SECAPI_SHA;
    }
    else if (nDigestSize == 32) 
    {
        hash_algo = SECAPI_SHA256;
    }
    else if (nDigestSize == 48) 
    {
        hash_algo = SECAPI_SHA384;
    }
    else
    {
      return SIGVERIFY_FAIL_BAD_DIGEST_SIZE;
    }

    /*init crypto engine*/
    if(sh_init_done != 1)
    {
    	stat = secapi_new(&sh, hash_algo);
    	if (stat != E_SUCCESS )
    	{ 
      		return SIGVERIFY_FAIL_CYRPTO_INIT_FAIL;
    	}
    	sh_init_done =1;
    }

    /*Set Hash param to use hardware based hash generation*/
    hash_param.common.exec_platform = SECAPI_EXEC_PLATFORM_HW;  

    stat = secapi_hsh_set_param(  sh,
                                  hash_algo,
                                  SECAPI_EXEC_PLATFORM,
                                  &hash_param );
    if(stat != E_SUCCESS)
    {
      SIGVERIFY_ERROR("secapi_hsh_set_param1 error: %d\n", stat);
      SIGVERIFY_DEBUG("sh: %u, hash_algo: %u, hasparam: %u\n", (unsigned int)&sh, hash_algo, (unsigned int)&hash_param);
      return SIGVERIFY_FAIL_CYRPTO_PARAM_SET_FAIL;
    }

    {
        //Set mode
        hash_param.mode = SECAPI_HSH_INPUT_MODE_ATOMIC;
        stat = secapi_hsh_set_param( sh,
                                     hash_algo,
                                     SECAPI_HSH_INPUT_MODE,
                                     &hash_param );

        //Set mode was success, Generate digest
        if ( stat == E_SUCCESS )
        {
            stat = secapi_hsh_create_digest(  sh,
                                              hash_algo,
                                              (uint8*)pSeg,
                                              cbSeg,
                                              digest_out);
         }else{

          SIGVERIFY_ERROR("secapi_hsh_set_param2 error: %d\n", stat);
          return SIGVERIFY_FAIL_CYRPTO_CREATE_DIGEST_FAILED;
         }

        if(stat != E_SUCCESS) //Failed to generate hash
        {
          SIGVERIFY_ERROR("secapi_hsh_create_digest error: %d\n", stat);
          return SIGVERIFY_FAIL_CYRPTO_PARAM_SET_FAIL; 
        }
        else
        {
          printf("Done creating digest\n");
          return SIGVERIFY_EOK;
        }

    }

    return SIGVERIFY_EOK;
}


/** \details
 * Authenticate the MBN hash segment.
 *
 * @param[in] so_name
 *    Name of the MBN file to authenticate
 *
 * @param[in] pHdrs
 *    Pointer to the start ot ELF header
 *    Program headers aer followed by Elf header at offset specified by e_phoff field
 *
 * @param[in] cbHdrs
 *    Length of Elf headers in bytes.
 *
 * @param[in] pHash
 *    Pointer to hash segment.
 *
 * @param[in] cbHash
 *    Size of hash segment in bytes.
 *
 * @param[in] ppv
 *    Elf info to be returned back to loader.
 * 
 * @return 
 *      SIGVERIFY_EOK      - Successful authentication.
 *      SIGVERIFY_FAIL_XXX - Failure during authentication with error code.
 *
 */
int SigVerify_start(const char* so_name, const unsigned char* pHdrs, int cbHdrs,
                    const unsigned char* pHash, int cbHash, void** ppv)
{
    /*Sigverify structures*/
    SigVerify_inst              *inst = NULL;
    uint32                      auth_enabled = 0;

    /*Elf structures*/
    Elf32_Ehdr                  *Ehdr = (Elf32_Ehdr*)pHdrs;
    Elf32_Phdr                  *Phdr = (Elf32_Phdr*)((unsigned int)pHdrs + Ehdr->e_phoff);
    sec_img_mbn_header_t        *auth_seg_hdr = NULL; //ToDo: change name to image_info

    /*Verify that ELF has a hash segment*/
    if(NULL == pHash)
    {
      return SIGVERIFY_FAIL_NO_HASH_FOUND;
    } 

    /*verify ELF header sanity*/
    if(!(Ehdr->e_ident[ELFINFO_MAG0_INDEX] == ELFINFO_MAG0 && 
      Ehdr->e_ident[ELFINFO_MAG1_INDEX] == ELFINFO_MAG1 &&
      Ehdr->e_ident[ELFINFO_MAG2_INDEX] == ELFINFO_MAG2 &&
      Ehdr->e_ident[ELFINFO_MAG3_INDEX] == ELFINFO_MAG3 &&
      Ehdr->e_ident[ELFINFO_CLASS_INDEX] == ELF_CLASS_32 &&
      Ehdr->e_ident[ELFINFO_VERSION_INDEX] == ELF_VERSION_CURRENT &&
      Ehdr->e_ehsize == sizeof(Elf32_Ehdr) &&
      Ehdr->e_phentsize == sizeof(Elf32_Phdr)))
    {
      return SIGVERIFY_FAIL_BAD_ELF_HEADERS;
    }
  
    /*verify valid program header*/
    if(NULL == Phdr)
    {
      return SIGVERIFY_FAIL_BAD_PROGRAM_HDR;
    }

    /*Find auth header or mbn header*/
    auth_seg_hdr = (sec_img_mbn_header_t*)pHash;

    /*Store MBN information*/    
    inst = (SigVerify_inst*)malloc(sizeof(SigVerify_inst));

    if(NULL == inst)
    {
      return SIGVERIFY_FAIL_MALLOC;
    }

    /*NOTE : sec_img_mbn_header_t == dl_qc_auth_seg*/
    inst->so_name = (char*)so_name;
    inst->pHash = (void*)pHash;
    inst->pHash_table_start = (void*)((unsigned int)pHash + sizeof(sec_img_mbn_header_t) + auth_seg_hdr->oem_metadata_size);
    inst->nDigestSize = (auth_seg_hdr->code_size)/Ehdr->e_phnum;
    inst->mode = MODE_SIGVERIFY;

    /*Info returned to loader for further use*/
    *ppv = inst;
  
    /*Decide if secureboot fuses are blown, i.e. if secureboot auth is enabled or not*/
    if(E_SUCCESS == secboot_util_hw_is_auth_enabled(SECBOOT_UTIL_HW_MSS_CODE_SEGMENT, 
                                                    &auth_enabled))
    {

        secboot_util_handle_type         *sec_boot_handle;
        boolean                           valid_signature = FALSE;
        secboot_util_verified_info_type   verified_info;
        secboot_util_fuse_info_type       fuse_info;
        secboot_util_error_type           ret;

        sec_boot_handle = (secboot_util_handle_type *) malloc(sizeof(secboot_util_handle_type));
        if(sec_boot_handle == NULL)
        {
          FARF(ALWAYS, "Secboot handle malloc failed \n");
          free(inst);
          *ppv = NULL;
          return SIGVERIFY_FAIL_MALLOC;
        }

        memset (sec_boot_handle, 0,  sizeof (secboot_util_handle_type));
        memset (&verified_info, 0,  sizeof (verified_info));
        memset (&fuse_info, 0,  sizeof (fuse_info));

        /*Skip signature auth if secureboot fuses are not blown, We will still do hash verification*/
        /*If device is not secure, but internal testing flag is enabled (MODE_SIGVERIFY_NON_SECURE_DEV), we use hard-coded key to verify signature*/
        if(auth_enabled && MODE_SIGVERIFY)
        {
            FARF(ALWAYS, "ELF SigVerify secboot_util_hw_is_auth_enabled: Secureboot Auth is enabled \n");
            secboot_util_update_fuse_info(&fuse_info);
            
            ret = secboot_util_auth_img_verify
                              (
                              sec_boot_handle,
                              pHash,
                              cbHash,
                              SECBOOT_UTIL_HW_MSS_CODE_SEGMENT,
                              0x0000000000000012, //WLAN sw_id assigned by secboot
                              NULL,
                              &verified_info
                              );
             if(ret == E_SECBOOT_UTIL_SUCCESS)
                  valid_signature = TRUE;
         }
         else if(!auth_enabled && MODE_SIGVERIFY_NON_SECURE_DEV) //Internal testing on a non-secure device only
         {
            FARF(ALWAYS, "ELF SigVerify secboot_util_hw_is_auth_enabled: Secureboot Auth is disabled, using hardcoded key\n");
            //Init fuses

	         secboot_util_update_fuse_info(&fuse_info);
			
            ret = secboot_util_auth_img_verify
                              (
                              sec_boot_handle,
                              pHash,
                              cbHash,
                              SECBOOT_UTIL_HW_MSS_CODE_SEGMENT,
                              0x0000000000000012, //WLAN sw_id assigned by secboot
                              &fuse_info,
                              &verified_info
                              );
            if(ret == E_SECBOOT_UTIL_SUCCESS)
                  valid_signature = TRUE;

         }
		    else
		    {
      		 	FARF(ALWAYS, "ELF SigVerify secboot_util_hw_is_auth_enabled: Secureboot Auth is disabled, skipping Signature Verification\n");
      		 	//If device is non-secure (secure fuses not blown) and For internal 
      			//builds auth is disabled we skip signature verification
      		 	valid_signature= TRUE;
      	}
		 
        free(sec_boot_handle);

        if(TRUE == valid_signature)
        {
          FARF(ALWAYS, "Signature authentication success\n", so_name);
          return SIGVERIFY_EOK;
        } 
        else
        {
          FARF(ALWAYS, "Signature Authentication failed\n", so_name);
          free(inst);
          *ppv = NULL;
          return SIGVERIFY_FAIL_SECBOOT_AUTH_FAIL;
        }
                
    }//Auth done
  
    //Failure to check if device is secure or not leads to auth failure.
    FARF(ALWAYS, "Failed to check if device is secure or not\n");
    free(inst);
    *ppv = NULL;
    return SIGVERIFY_FAIL_SECBOOT_AUTH_FAIL; 
}

/** \details
 * Verify hash value of the given segment with the one in hash table.
 *
 * @param[in] ppv
 *    Elf information as parsed by the SigVerify before.
 *
 * @param[in] segnum
 *    Segment number to be authenticated.
 *
 * @param[in] cbSeg
 *    Length of the segment
 * 
 * @return 
 *      SIGVERIFY_EOK      - Successful authentication.
 *      SIGVERIFY_FAIL_XXX - Failure during authentication, error code from SigVerify.h.
 *
 */


int SigVerify_verifyseg_impl(void* pv, int segnum, const unsigned char* pSeg, int cbSeg)
{

    unsigned int digest_out[MAX_DIGEST_SIZE];
    int          nErr = -1;
    int          nDigestSize;

    /*Input sanity*/
    if( NULL == pv || NULL == pSeg)
    {
      return SIGVERIFY_VERIFY_SEG_BAD_INPUT;
    }

    /*Find the expected hash value for the underlying segment*/
    SigVerify_inst* inst = (SigVerify_inst*)pv;
    void* pHash_table_start = inst->pHash_table_start;
    nDigestSize = inst->nDigestSize;
    void* pHashValCurrent = (void*)((unsigned int)pHash_table_start + segnum*nDigestSize);

    /*zero-out digest_out*/
    memset(digest_out, 0, sizeof(digest_out)); //zero out digest_out

    /*If segment size is zero, we skip hash generation*/
    /*It is expected that for such a segment the hash entry is filled with zeros*/
    /*We will confirm this by matching it in the hash table.*/
    if(cbSeg == 0)
    {
      if(segnum == 0) //ELF header segment must be of non-zero size
        return SIGVERIFY_FAIL_HASH_MATCH_FAIL;
    } 
    else
    {
      /*Calculate hash value of the current segment*/
      nErr = SigVerify_GetSegmentHash((void*)pSeg, cbSeg, &digest_out, nDigestSize);
      if(SIGVERIFY_EOK != nErr)
      {
        return SIGVERIFY_FAIL_HASH_GEN_FAILED;
      }
    }    
    
    /*Compare the hash values and return*/
    nErr = memcmp((void*)&digest_out, pHashValCurrent, nDigestSize);
    if(SIGVERIFY_EOK != nErr)
    {
      return SIGVERIFY_FAIL_HASH_MATCH_FAIL;
    }
    
    printf("Hash matched for segment %d\n", segnum);

    return SIGVERIFY_EOK;

}

//Future enhancements, do segment verification in separate threads.
int SigVerify_verifyseg(void* pv, int segnum, const unsigned char* pSeg, int cbSeg)
{
   return SIGVERIFY_EOK;
}

int SigVerify_stop(void* pv)
{
   if(NULL != pv) free(pv);
   return SIGVERIFY_EOK;
}


int SigVerify_Init(void)
{
  return SIGVERIFY_EOK;
}


