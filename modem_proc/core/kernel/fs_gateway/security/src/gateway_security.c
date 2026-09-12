#include "gateway_security.h"
#include "SigVerify.h"
#include "stdio.h"
#include "pthread.h"
#include "SigVerify.c"

typedef struct mbn_info
{
	char* 			so_name;
	Elf32_Ehdr* 	pEhdr;

}mbn_info;

void* _sec_worker(void* param)
{

	char* 			so_name;
	Elf32_Ehdr* 	pEhdr;
	void* 			return_val = (void*)SIGVERIFY_EFAIL;
	char* 			pSeg; //pointer to offset in file to segment start
	int 			cbSeg; //file size of segment


	mbn_info *info = (mbn_info*)param;
	if(info == NULL) return (void*)(-1);
	
	so_name = info->so_name;
	pEhdr = (Elf32_Ehdr*)info->pEhdr;
 	SIGVERIFY_DEBUG("pEhdr: ​%u\n", (unsigned int)pEhdr);
	void *ppv=NULL, *pHashSeg=NULL;
	int nHashSegLen =0,  nErr = FS_GATEWAY_SECURITY_SUCCESS, index=0, iHashSegIdx = -1;
	Elf32_Phdr 							*pPhdr = NULL;

	//Sanity check input params
	if(pEhdr == NULL)
	{
		return_val = (void*)SIGVERIFY_FAIL_BAD_ELF_HEADERS;
		return return_val;
	}

	//Find program header table
	pPhdr = (Elf32_Phdr*)((unsigned int)pEhdr + pEhdr->e_phoff);
	if(pPhdr == NULL)
	{
		return_val = (void*)SIGVERIFY_FAIL_BAD_PROGRAM_HDR;
		return return_val;
	}
	/*First program header should be ELF metdata (ELF headers + Program header table), make sure it looks ok*/
	/*We assume that ELF headers start at offset 0 in the file and are non-zero in size*/
	/*We will verify this segment's hash value during hash verification.*/
	if(pPhdr[0].p_offset != 0 || pPhdr[0].p_filesz == 0)
		return (void*)SIGVERIFY_FAIL_BAD_PROGRAM_HDR;

	//Find hash segment
	for(index = 0; index < pEhdr->e_phnum; index++)
	{
		//Hash segment
		if( MI_PBT_SEGMENT_TYPE_VALUE(pPhdr[index].p_flags) == MI_PBT_HASH_SEGMENT)
		{
			iHashSegIdx = index;
			nHashSegLen = pPhdr[index].p_filesz;
			if(nHashSegLen == 0)
			{
				return_val = (void*)SIGVERIFY_HASH_SEG_LEN_ZERO;
				return return_val;
			} 
			
			//Hash segment is located at starting vaddr of ELF + offset described by hash segment's program header.
			pHashSeg = (void*)((unsigned int)pEhdr + pPhdr[index].p_offset);
			break; //we found the hash segment now stop.
		}
	}

	/*If there is no hash segment, the file is not signed or corrupted, abort*/
	if(iHashSegIdx == -1)
	{
		return_val = (void*)FS_GATEWAY_HASH_SEGMENT_NOT_FOUND;
		return return_val;
	} 

	nErr = SigVerify_start(so_name, (const unsigned char *)pEhdr, 0x1000, pHashSeg, nHashSegLen, &ppv);
	if(nErr != SIGVERIFY_EOK)
	{
		return_val = (void*)nErr;
		return return_val;
	} 

	//Verify all segment hashes
	for(index = 0; index < pEhdr->e_phnum; index++)
	{
		//Skip hash seg verification of hash segment itself
		if (index == iHashSegIdx)
		{
			continue;
		}

		pSeg = (char*)((unsigned int)pEhdr + pPhdr[index].p_offset);
		cbSeg = pPhdr[index].p_filesz;

		//printf("pSeg %x cbSeg %x segment number %d\n", (unsigned int)pSeg, (unsigned int)cbSeg,index);

		nErr = SigVerify_verifyseg_impl(ppv, index, (void*)pSeg, cbSeg);
		if (nErr != SIGVERIFY_EOK)
		{
			printf("Hash verification failed for segment: %d\n", index);
			return_val = (void*)nErr;
			return return_val;
		}

	}

	return_val = SIGVERIFY_EOK;
	return return_val;
}

/** \details
 * Authenticate a given ELF bianry
 *
 * @param[in] so_name
 *    Name of the ELF binary to authenticate
 *
 * @param[in] pEhdr
 *    Address of ELF headers of the binary.
 *
 * @return 
 *      SIGVERIFY_EOK      - Successful authentication generation.
 *      SIGVERIFY_FAIL_XXX - Failure during hash generation with error code from sigverify.h
 *
 */
int gateway_security_do_auth(const char* so_name, Elf32_Ehdr* pEhdr)
{

	pthread_t 			sec_worker_thread;
	void 				*arg = NULL, 
						*ret_val_thread = (void*)SIGVERIFY_EFAIL;
	pthread_attr_t 		thread_attr;
	int 				error = -1;

	mbn_info info;

	info.so_name = (char*)so_name;
	info.pEhdr = pEhdr;
	arg = (void*)&info;

	error = pthread_attr_init(&thread_attr);
	error = pthread_attr_setthreadname(&thread_attr, "SigVerify");
	error = pthread_attr_setstacksize(&thread_attr, 0x2000);

	if(error != 0) return FS_GATEWAY_SECURITY_FAIL_THERAD;

	error = pthread_create(&sec_worker_thread, &thread_attr, _sec_worker, arg);

	if(error == 0)
	{
		error = pthread_join(sec_worker_thread, &ret_val_thread);
		if(error == 0)
		{
			if(ret_val_thread == (void*)SIGVERIFY_EOK)
			{
				printf("Gateway Authentication success!!\n");
				return FS_GATEWAY_SUCESS;
			}
			else
			{
				printf("Gateway Authentication fail, error code %d\n", (int)ret_val_thread);	
				return (int)ret_val_thread;
			}
		}
		else
		{
			printf("Failed to join thread for Sigverify\n");
			return FS_GATEWAY_SECURITY_FAIL_THERAD_JN;
		}

	}
	else
	{
		printf("Failed to launch thread for Sigverify\n");
		return FS_GATEWAY_SECURITY_FAIL_THERAD;
	}

	return SIGVERIFY_EOK;

}