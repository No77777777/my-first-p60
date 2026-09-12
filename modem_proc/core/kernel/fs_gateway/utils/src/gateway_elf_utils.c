/*=============================================================================

FILE:       gateway_elf_utils.c

==============================================================================
        Copyright (c) 2013 Qualcomm Technologies Incorporated.
                   All Rights Reserved.
            QUALCOMM Proprietary and Confidential
==============================================================================*/
#ifndef FARF_ERROR
#define FARF_ERROR 1
#endif
#ifndef VERIFY_PRINT_ERROR
#define VERIFY_PRINT_ERROR
#endif 

#include "gateway_elf_utils.h"
#include <string.h>



static
int GetCertLen(byte *pCert)
{
   // discover start of each certificate
   if (*pCert == 0x30 && *(pCert+1) == 0x82) {
      int16 len;
      // convert to little endian-
      len = *(pCert+2) << 8;
      len += *(pCert+3);
      return (int)len;
   }
   return 0;
}


static
int FindHashPh(Elf32_Ehdr* pEh)
{
   Elf32_Phdr *pPh;

   int i;

   for (i = 0, 
        pPh = (Elf32_Phdr*)((byte*)pEh + pEh->e_phoff);
        i < pEh->e_phnum; 
        i++, 
        pPh++) {
      if (GET_SEGMENT_TYPE(pPh->p_flags) == HASH_SEGMENT) {
         if (i != 1) {
            // hash segment must be 2nd
            return 0;
         }
         return i;
      }
   }
   return -1;
}

//Verify basic ELF header sanity
int gateway_elf_verify_sanity(void* pFilePtr)
{
    Elf32_Ehdr* pEh = (Elf32_Ehdr*)pFilePtr;
    int nErr=0;

    // verify basic ELF properties
   VERIFY(pEh->e_ident[0] == 0x7f && // magic number
          pEh->e_ident[1] == 'E' && // magic number
          pEh->e_ident[2] == 'L' && // magic number
          pEh->e_ident[3] == 'F' && // magic number
          pEh->e_ident[4] == 1 && // ELF32 class
          pEh->e_ident[5] == 1 && // little endian
          pEh->e_ident[6] == 1); // current version
   VERIFY(pEh->e_ehsize == sizeof(Elf32_Ehdr));
   VERIFY(pEh->e_phentsize == sizeof(Elf32_Phdr));

   bail:
   return nErr;
}

int gateway_elf_utils_get_mbn_info(Elf32_Ehdr* pHdrs, Elf32_Phdr* pHash,
            ELFHashInfo *pei)
{
   Elf32_Ehdr* pEh = (Elf32_Ehdr*)pHdrs;
   int nErr = 0;

   if (0 == pHash) {
      THROW("dynamic module is unsigned");
   }

   pei->pHh = (HashMBNHeader*)pHash;
   pei->cbHh = sizeof(HashMBNHeader);
   pei->pHashes = (byte*)pHash + sizeof(HashMBNHeader);
   pei->cbHashes = pei->pHh->code_size;
   pei->pSig = (byte*)pei->pHashes + pei->pHh->code_size;
   pei->cbSig = pei->pHh->signature_size;
   pei->pCertChain = pei->pSig + pei->cbSig;
   pei->cbCertChain = pei->pHh->cert_chain_size;
   pei->pPhs = (Elf32_Phdr*)(pHdrs + pEh->e_phoff);
   pei->nPhs = (int)pEh->e_phnum;

   // get start and length of each certificate
   {  int i;
      byte *pc = pei->pCertChain;
      for (i = 0, pei->nCerts = 0; i < 3 && pc < (pei->pCertChain + pei->cbCertChain); i++) {
         int cb = GetCertLen(pc);
         if (cb == 0) {
            break;
         }
         pei->pCert[i] = pc;
         pei->cbCert[i] = cb + 4;  // 4 bytes for tag/len
         pc += cb + 4;
         pei->nCerts++;
      }
   }

bail:
   return nErr;
}

int gateway_elf_get_hash_segment(Elf32_Ehdr* pEh, int* nHashSegIdx)
{
   Elf32_Phdr *pPh = 0;//hash segment's pheader
   int nErr = 0;

   VERIFY(0 != (*nHashSegIdx = FindHashPh(pEh)));

bail:
   return nErr;
}

// assumes serial number is sizeof(uint32), original code assumption it was
// <= sizeof(uint32).
// TODO verify what serial number size should be
int gateway_elf_get_serial(const unsigned char* pHdrs, int cbHdrs,
                       unsigned char* pHash, int cbHash, uint32* serialnum)
{
   Elf32_Ehdr* pEh = (Elf32_Ehdr*)pHdrs;
   byte* pHashes = 0;
   int cbHashes;
   byte* pserialnum = 0;
   int nErr = 0;

   pHashes = pHash + sizeof(HashMBNHeader);
   cbHashes = ((HashMBNHeader*)pHash)->code_size;

   VERIFY(pEh->e_phnum > 0);  // sanity check for bounds overflow
   pserialnum = pHashes + (cbHashes / pEh->e_phnum); // index to hashEntry[1]
   VERIFY(pserialnum < pHashes + cbHashes);  // sanity check for bounds overflow

   memmove(serialnum,  pserialnum, sizeof(uint32));

bail:
   return nErr;
}




