/*=============================================================================

FILE:       sigverify.h

SERVICES:   API for authenticating dynamic modules using RSA signatures and
            an X509 certificate chain embedded in the dynmod.

==============================================================================
        Copyright (c) 2013-2014 Qualcomm Technologies Incorporated.
                   All Rights Reserved.
            QUALCOMM Proprietary and Confidential
==============================================================================*/
#ifndef __SIGVERIFY_H__
#define __SIGVERIFY_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PL_SHA256_DIGEST_LENGTH   32
#define PL_SHA256_BLOCK_LENGTH    64

#define SIGVERIFY_START 		0x0 + 128
#define SIGVERIFY_VERIFYSEG     0x1 + 128
#define SIGVERIFY_STOP  		0x2 + 128


/*Error codes*/
#define SIGVERIFY_EOK                            0
#define SIGVERIFY_FAIL_BAD_ELF_HEADERS          -1
#define SIGVERIFY_FAIL_NO_HASH_FOUND            -2
#define SIGVERIFY_FAIL_BAD_PROGRAM_HDR          -3
#define SIGVERIFY_FAIL_SECBOOT_INIT_FAIL        -4
#define SIGVERIFY_FAIL_MALLOC                   -5
#define SIGVERIFY_FAIL_SECBOOT_AUTH_FAIL        -6
#define SIGVERIFY_VERIFY_SEG_BAD_INPUT          -7
#define SIGVERIFY_FAIL_HASH_GEN_FAILED          -8
#define SIGVERIFY_FAIL_HASH_MATCH_FAIL          -9
#define SIGVERIFY_FAIL_BAD_DIGEST_SIZE          -11
#define SIGVERIFY_FAIL_BAD_INPUTS               -12
#define SIGVERIFY_FAIL_CYRPTO_INIT_FAIL         -13
#define SIGVERIFY_FAIL_CYRPTO_PARAM_SET_FAIL    -14
#define SIGVERIFY_FAIL_INPUT_MODE_FAIL          -15
#define SIGVERIFY_FAIL_ATOMIC_MODE_FAIL         -16
#define SIGVERIFY_FAIL_FINAL_MODE_FAIL          -17
#define SIGVERIFY_EFAIL  			            -18
#define SIGVERIFY_HASH_SEG_LEN_ZERO             -19
#define SIGVERIFY_FAIL_CYRPTO_CREATE_DIGEST_FAILED -20

/*Mode for authentication*/
#define MODE_SIGVERIFY  1                       //Enable signature verification on a secure device
#define MODE_HASHVERIFY 1					 	//Enable segment hash verification on secure or non-secure device
#define MODE_SIGVERIFY_NON_SECURE_DEV  0        //Enable signature verification on a non-secure device for testing purposes

int SigVerify_start(const char* so_name, const unsigned char* pHdrs, int cbHdrs, const unsigned char* pHash, int cbHash, void** ppv);
int SigVerify_verifyseg(void* pv, int segnum, const unsigned char* pSeg, int cbSeg);
int SigVerify_stop(void* pv);

#ifdef __cplusplus
}
#endif

#endif   // __SIGVERIFY_H__
