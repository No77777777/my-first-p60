/*===========================================================================

                    BOOT EXTERN ENCRYPTION DEFINITIONS

DESCRIPTION
  Contains wrapper definition for external crypto drivers

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright 2017 by QUALCOMM Technologies Incorporated.  All Rights Reserved.
============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
    
    
when       who     what, where, why
--------   ---     ----------------------------------------------------------
25/04/17   bd      Initial Creation.

===========================================================================*/

/*==========================================================================
                               INCLUDE FILES
===========================================================================*/
#include "PrngML.h"
#include "CeML.h"
#include "secmath.h"
#include "secrsa.h"
#include "wlan_md.h"
#include "wlan_md_cipher.h"
#include "wlan_md_oem_public_key.h"
#include "stringl.h"

static uint8 wlan_md_rsa_encr_key[WLAN_RSA_KEY_LEN];
static uint8 wlan_md_aes_key[WLAN_SW_AES_KEY_SIZE];
static uint8 wlan_md_hmac_key[WLAN_SW_HMAC_KEY_SIZE];
static uint8 wlan_pd_minidump_version[WLAN_MD_VERSION_SIZE] = 
                                         {WLAN_MD_VERSION_MSB, WLAN_MD_VERSION_LSB}; 
static uint8 wlan_md_aes_and_hmac_key_and_version[WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE];
static wlan_md_raw_key_t raw_key = {0};

extern wlan_md_blob_entry wlan_md_blob[WLAN_MD_NUM_SMEM_ENTRIES];
extern uint32 wlan_md_blob_entries;
extern wlan_md_blob_entry wlan_md_non_encrypt_blob[WLAN_MD_NUM_ENC_ENTRIES];
extern uint32 wlan_md_non_encrypt_blob_entries;

/*===========================================================================
                      FUNCTION DEFINITIONS
===========================================================================*/ 

void wlan_md_oem_get_pub_key(char** pub_mod_ptr, uint32* pub_mod_len, char** pub_exp_ptr, uint32* pub_exp_len)
{
  *pub_mod_ptr = wlan_md_oem_public_key.n;
  *pub_exp_ptr = wlan_md_oem_public_key.e;
  *pub_mod_len = wlan_md_oem_public_key.bitLength / 8;
  *pub_exp_len = wlan_md_oem_public_key.bitLength / 8;
}

static void wlan_md_encr_aes_key (uint8 *in, uint32 inlen, uint8 *out, uint32 *outlen)
{
  CE_RSA_KEY               key;
  CE_RSA_OAEP_PAD_INFO     oaep_pad_info = {0};
  CE_RSA_PADDING_TYPE      padding_type  = CE_RSA_PAD_PKCS1_OAEP;
  void                    *padding_info  = NULL;
  char                   *mod = NULL;
  char                   *exp = NULL;
  uint32                   mod_len = 0;
  uint32                   exp_len = 0;  
  size_t                   keyByteLength;
  uint8                    key_e[sizeof(S_BIGINT)];
  uint8                    key_N[sizeof(S_BIGINT)];

  wlan_md_oem_get_pub_key(&mod,&mod_len,&exp,&exp_len);

  raw_key.n = (char*)mod;
  raw_key.e = (char*)exp;

  oaep_pad_info.hashidx  = CE_HASH_IDX_SHA256;   
  oaep_pad_info.label    = NULL;
  oaep_pad_info.labellen = 0;
  padding_info = (void*)&oaep_pad_info;

  memset(&key, 0, sizeof(CE_RSA_KEY));
  raw_key.bitLength =2048;
  keyByteLength = raw_key.bitLength/8;

  key.e = (S_BIGINT *)key_e;

  if( secmath_BIGINT_read_radix(&(key.e)->bi, raw_key.e, 16) != 0)
  {
    memset(out, 0, *outlen);
    return;
  }

  (key.e)->bi.n = keyByteLength/sizeof(BLONG);
  (key.e)->sign = S_BIGINT_POS; // Positive

  key.N = ( S_BIGINT *)key_N;

  if( secmath_BIGINT_read_radix(&(key.N)->bi, raw_key.n, 16) != 0)
  {
    return;
  }

  (key.N)->bi.n = keyByteLength / sizeof(BLONG);
  (key.N)->sign = S_BIGINT_POS; // Positive

  key.type = CE_RSA_KEY_PUBLIC;
  key.bitLength = raw_key.bitLength;
  ce_rsa_encrypt(&key, padding_type, padding_info, in, inlen, out, (INT32*)outlen);  

}
static int wlan_md_sw_cipher
(   
   void   *in, 
   size_t  in_len, 
   void   *out, 
   size_t  out_len,
   void   *key_aes, 
   size_t  key_aes_len,
   void   *key_hmac, 
   size_t  key_hmac_len,
   void   *nonce, 
   size_t  nonce_len,
   void   *tag,
   size_t  tag_len,
   boolean    dir                    //false: encrypt, true: decrypt
)
{
  CeMLErrorType                    ret = CEML_ERROR_FAILURE;
  CeMLCntxHandle*        cntx = NULL;
  CEMLIovecListType          ioVecIn;
  CEMLIovecListType          ioVecOut;
  CEMLIovecType              IovecIn;
  CEMLIovecType              IovecOut;
  CeMLCipherDir    sw_dir;
  CeMLCipherAlgType     alg  = CEML_CIPHER_ALG_AES256;
  CeMLCipherModeType      mode = CEML_CIPHER_MODE_CTR;
   
  do
  {
    if( !in  || !out  || !key_aes || !key_hmac || !nonce  || !tag )
    {
      ret = CEML_ERROR_FAILURE;
      break;
    }     
 
    if((ret = CeMLInit()) != 0)
    {
      ret = CEML_ERROR_FAILURE;
      break;
    }    

    if((ret = CeMLCipherInit(&cntx, alg)) != 0)
    {
      ret = CEML_ERROR_FAILURE;
      break;
    }

    if( dir == FALSE)
       sw_dir = CEML_CIPHER_ENCRYPT;
    else
       sw_dir = CEML_CIPHER_DECRYPT;
    
     //Set parameters so we can do the encrypt
    if( (ret = CeMLCipherSetParam(cntx, CEML_CIPHER_PARAM_MODE,       &mode,   sizeof(CeMLCipherModeType)))   != 0  ||
        (ret = CeMLCipherSetParam(cntx, CEML_CIPHER_PARAM_DIRECTION,  &sw_dir, sizeof(CeMLCipherDir))) != 0  ||
        (ret = CeMLCipherSetParam(cntx, CEML_CIPHER_PARAM_KEY,        key_aes,     key_aes_len))                     != 0  ||
        (ret = CeMLCipherSetParam(cntx, CEML_CIPHER_PARAM_IV,         nonce,   nonce_len))                    != 0 /*TODO: memory for this shall be of 16bytes */
      )
    {  
      ret = CEML_ERROR_FAILURE;
      goto DEINIT;
    }
    // Input IOVEC 
    ioVecIn.size = 1;
    ioVecIn.iov = &IovecIn;
    ioVecIn.iov[0].dwLen  = in_len;
    ioVecIn.iov[0].pvBase = in; 
    // Output IOVEC
    ioVecOut.size = 1;
    ioVecOut.iov = &IovecOut;
    ioVecOut.iov[0].dwLen  = out_len;
    ioVecOut.iov[0].pvBase = out;

    //Now encrypt the data
    if((ret = CeMLCipherData(cntx, ioVecIn, &ioVecOut)) != 0)
    {
      ret = CEML_ERROR_FAILURE;
      goto DEINIT;
    }
  
     // Input IOVEC 
    ioVecIn.size = 1;
    ioVecIn.iov = &IovecIn;
    ioVecIn.iov[0].dwLen  = out_len;
    ioVecIn.iov[0].pvBase = out; 
	
    // Output IOVEC
    ioVecOut.size = 1;
    ioVecOut.iov = &IovecOut;
    ioVecOut.iov[0].dwLen  = tag_len;
    ioVecOut.iov[0].pvBase = tag;

    CeMLHmac(key_hmac, key_hmac_len, ioVecIn, &ioVecOut, CEML_HASH_ALGO_SHA256);

DEINIT:
    if(CeMLCipherDeInit(&cntx) != 0)
    {
      ret = CEML_ERROR_FAILURE;
    }
    CeMLDeInit();
    
  }while(0);

  return (int)ret;
}
static void wlan_md_generate_encr_aeskey(void)
{

  uint32  cipherlen = WLAN_RSA_KEY_LEN;
  uint32  offset = 0;
  
  PrngML_getdata(wlan_md_aes_key, WLAN_SW_AES_KEY_SIZE);
  PrngML_getdata(wlan_md_hmac_key, WLAN_SW_HMAC_KEY_SIZE);
  
  offset+=memscpy(wlan_md_aes_and_hmac_key_and_version, WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE,
                  wlan_md_aes_key,(size_t)WLAN_SW_AES_KEY_SIZE);
  offset+=memscpy(wlan_md_aes_and_hmac_key_and_version + offset, 
                 (WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE-offset), wlan_md_hmac_key, WLAN_SW_HMAC_KEY_SIZE);
  offset+=memscpy(wlan_md_aes_and_hmac_key_and_version + offset,
                 (WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE-offset), wlan_pd_minidump_version, WLAN_MD_VERSION_SIZE);
   
  wlan_md_encr_aes_key(wlan_md_aes_and_hmac_key_and_version, WLAN_SW_AES_PLUS_HMAC_KEY_PLUS_VERSION_SIZE,
                  wlan_md_rsa_encr_key, &cipherlen);
}

/*
*/
void wlan_md_crypto_init(void)
{
  wlan_md_blob_entry *wlan_md_region;
  
  wlan_md_region = (wlan_md_blob_entry*)wlan_md_non_encrypt_blob;

  wlan_md_generate_encr_aeskey();
  
  /* store the key to first region for encryption artifacts */
  memscpy((uint8*)wlan_md_region->region_va, WLAN_RSA_KEY_LEN, 
          wlan_md_rsa_encr_key, WLAN_RSA_KEY_LEN);

  wlan_md_region->region_size = WLAN_RSA_KEY_LEN;
  
  /* Initialize the sizes of IV and MAC regions */
  wlan_md_region++;
  wlan_md_region->region_size = 0;
  wlan_md_region++;
  wlan_md_region->region_size = 0;
}

static void wlan_md_store_iv_mac(uint8* iv, uint8* mac, uint32 r_num)
{
  wlan_md_blob_entry *wlan_md_region;
  
  wlan_md_region = (wlan_md_blob_entry*)wlan_md_non_encrypt_blob;
  wlan_md_region++;
  
  /* Store IV in this region */
  memscpy((uint8*)(wlan_md_region->region_va) + (r_num * WLAN_SW_AES_IV_SIZE), 
           WLAN_SW_AES_IV_SIZE, iv, WLAN_SW_AES_IV_SIZE);
  wlan_md_region->region_size += WLAN_SW_AES_IV_SIZE;
  
  /* Next Region, store the mac */
  wlan_md_region++;
  memscpy((uint8*)wlan_md_region->region_va + r_num * WLAN_SW_AES_MAC_SIZE,
           WLAN_SW_AES_MAC_SIZE, mac, WLAN_SW_AES_MAC_SIZE);
  wlan_md_region->region_size += WLAN_SW_AES_MAC_SIZE;
}

/*
*/
void wlan_md_encrypt_region(uint64 r_base, uint64 r_len, uint32 r_num)
{
  uint8 wlan_md_aes_key_iv[WLAN_SW_AES_IV_SIZE];
  uint8 wlan_md_aes_key_mac[WLAN_SW_AES_MAC_SIZE];
  
  memset(&wlan_md_aes_key_iv, 0, sizeof(wlan_md_aes_key_iv));
  memset(&wlan_md_aes_key_mac, 0, sizeof(wlan_md_aes_key_mac));
  
  /* Generate AES IV */
  PrngML_getdata(wlan_md_aes_key_iv, WLAN_SW_AES_IV_SIZE);

  wlan_md_sw_cipher((void*)(r_base), r_len,
                 (void*)(r_base), r_len, 
                 (void*)(&wlan_md_aes_key[0]), WLAN_SW_AES_KEY_SIZE,
				 (void*)(&wlan_md_hmac_key[0]), WLAN_SW_HMAC_KEY_SIZE, 
                 (void*)(wlan_md_aes_key_iv), WLAN_SW_AES_IV_SIZE,
                 (void*)(wlan_md_aes_key_mac), WLAN_SW_AES_MAC_SIZE,
                 FALSE);
  wlan_md_store_iv_mac(wlan_md_aes_key_iv, wlan_md_aes_key_mac, r_num);
}
