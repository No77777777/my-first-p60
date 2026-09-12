#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include "rfs_api.h"
#include "rfs_errno.h"

#include "rfs_gateway.h"
#include "gateway_errno.h"
#include "gateway_memory.h"
#include "gateway_verify.h"
#include "gateway_security.h"
#include "gateway_elf_utils.h"
#include "qurt_atomic_ops.h"
#include "qurt.h"
#include "qurt_restricted.h"

#include "qurt_printf.h"
#include "SigVerify.h"
#include "miprogressive.h"
#include "root_init.h"

//rfs gateway file descriptor
typedef struct rfs_gateway_file {
	unsigned int			used;		/*Marker to detect if spot is free ot in use*/
	const char* 			path;
	unsigned int  			pathlen;
	unsigned int 			filesz;
	void*					file_start;
	void*					hash_segment_start;
	uint32					hash_seg_size;
} rfs_gateway_file_t;
/* Need to access global variable for PD to register RFS callback into QURT  */
extern eloader_pd_info_t pd_info_carry[N_MAX_NUM_USERPD];

fs_gateway_debug_info_t FS_GATEWAY_DEBUG_TABLE[FS_GATEWAY_TABLE_MAX_INDEX+1];
unsigned volatile fs_gateway_debug_index;

#define MAX_RFS_GATEWAY_FD 10 //Max num of files that can be opened by rfs_gateway
#define PAGESZ 0x1000
/* TLB entry alignment size, this needs to map to the linker alignment size; for example, if image is mapped to 256KB, this ALIGMENT size needs to be 256KB*/ 
#define ALIGNMENT_SZ 0x40000 
#define RFS_GET_RETRY_COUNT 10

//This list will hold file descriptors for all files opened by rfs gateway
static rfs_gateway_file_t rfs_gateway_file_q[MAX_RFS_GATEWAY_FD] = {{0}};
static int rfs_gateway_init_done = 0; // This is qdi handle we get when we register call back. 
									  // Doing qdi_close on this will de-register callback from RAMFS.
//temp debugging
int rfs_gateway_errcode = -1;
#ifdef USERPD_PROFILE_DEBUG
unsigned long curr_time_in_micro_sec = 0;
#endif 

//ToDo: use better logic
static 
int rfs_gateway_add_new_file(void){

	int index=0;
	for(index=0; index<MAX_RFS_GATEWAY_FD; index++)
	{
		if(rfs_gateway_file_q[index].used == 0)
		{
			rfs_gateway_file_q[index].used = qurt_atomic_add_return(&(rfs_gateway_file_q[index].used), 1);
			if(rfs_gateway_file_q[index].used > 1) continue; //someone else added to index
			return index;
		}
	}
	return -1; //max num of files already opened.
}

//See if rfs_gateway already opened this file.
static 
int rfs_gateway_search(const char* path, unsigned int pathlen)
{
	int index=0;
	for(index=0; index<MAX_RFS_GATEWAY_FD; index++)
	{
		if(rfs_gateway_file_q[index].used > 0 && rfs_gateway_file_q[index].pathlen == pathlen)
		{
			if(!strcmp(rfs_gateway_file_q[index].path, path)) return index;
		}

	}
	return -1;
}

//Must register rfs_gateway with RAMFS at rcinit
//Add check to ensure init is called before rfs_gateway is used.
void rfs_gateway_init(void)
{

#ifdef USERPD_PROFILE_DEBUG
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("WLANPD times starts from ROOT RCINIT:%lu\n",curr_time_in_micro_sec);
#endif /*USERPD_PROFILE_DEBUG */
	rfs_gateway_init_done = qurt_fs_notfound_callback_add(&rfs_gateway, &pd_info_carry);
}

//rfs_gateway implementation.
void rfs_gateway(void *arg, int handle, const char *path){

	struct rfs_stat_buf				   *fstat_buf = NULL;
	int 							    nErr =0,
										rfs_err,
										retVal,
										nPathLen,
										nFileIdx,
										i=0;
	uint32								nBAlignedFileSz = 0x0;
	void 								*addr= GATEWAY_MAP_FAILED;
	Elf32_Ehdr							*pEh = NULL;
	
	Elf32_Phdr 							*pPhdr = NULL;
	user_pd_name_t                      index;
    eloader_pd_info_t *local_pd_info_carry;
    local_pd_info_carry = (eloader_pd_info_t *) arg;
    char *latest_file_loc = NULL;
	// find out which PD is calling this function.
	for (index=N_WLAN_PD; index < N_MAX_NUM_USERPD; index++) 
	{
	  if (!(strncmp(local_pd_info_carry[index].img_name,path,32)))  	
	   break;	
	}
	VERIFYC(nErr, index < N_MAX_NUM_USERPD, index);
	VERIFYC(nErr, 0 <= rfs_gateway_init_done, FS_GATEWAY_INIT_FAIL);
	VERIFYC(nErr, NULL != path, FS_GATEWAY_BAD_PATH);
	VERIFYC(nErr, 0 < (nPathLen = strlen(path)), FS_GATEWAY_BAD_PATH);

	//See if we have this file already opened by the rfs_gateway
	nFileIdx = rfs_gateway_search(path, nPathLen);
	if(-1 != nFileIdx)
	{
		//ToDO: Do a RW data fetching.
		//Install file in RAMFS, log success.
		retVal = qurt_fs_extrafile_add(path, 
									   rfs_gateway_file_q[nFileIdx].file_start, 
									   rfs_gateway_file_q[nFileIdx].filesz);

		VERIFYC(nErr, -1 != retVal, FS_GATWAY_RAMFS_INSTALL_FAIL);
		return;
	}
	//If file was not alreay open, lets do a fresh fetch
	fstat_buf = (struct rfs_stat_buf*)malloc(sizeof(struct rfs_stat_buf));	
	VERIFYC(nErr, NULL != fstat_buf, FS_GATEWAY_MALLOC_FAIL);
#ifdef USERPD_PROFILE_DEBUG
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("USERPD RFS query time start:%lu\n",curr_time_in_micro_sec);
#endif 

	//Check vendor path first
	latest_file_loc = local_pd_info_carry[index].rfs_path_vendor;
	rfs_err = rfs_stat(local_pd_info_carry[index].rfs_path_vendor, fstat_buf);
	
	//check alternative vendor path if default path doesn't exist.
	if (rfs_err == RFS_ENOENT)
    {
       rfs_err = rfs_stat(pd_info_carry[index].rfs_path_vendor_2, fstat_buf);
       latest_file_loc = local_pd_info_carry[index].rfs_path_vendor_2;
	}
	
	//check alternative path if default path doesn't exist.
	if (rfs_err == RFS_ENOENT)
    {
       rfs_err = rfs_stat(pd_info_carry[index].rfs_path, fstat_buf);
       latest_file_loc = local_pd_info_carry[index].rfs_path;
	}
	

	//found the matching image
	qurt_printf("RFS FILE PATH:%s\n",latest_file_loc);
	VERIFYC(nErr, 0 == rfs_err, rfs_err);
	
	VERIFYC(nErr, 0 < fstat_buf->st_size, FS_GATEWAY_ZERO_FILESZ);

	//Align filesize to alignment boundary
	nBAlignedFileSz = fstat_buf->st_size;
	nBAlignedFileSz = ((ALIGNMENT_SZ-1)&nBAlignedFileSz) ? ((nBAlignedFileSz+ALIGNMENT_SZ) & ~(ALIGNMENT_SZ-1)):nBAlignedFileSz;

	addr = gateway_memory_map(nBAlignedFileSz);
	VERIFYC(nErr, GATEWAY_MAP_FAILED != addr, FS_GATEWAY_MAP_FAIL);
#ifdef USERPD_PROFILE_DEBUG
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("USERPD RFS GET time:%lu\n",curr_time_in_micro_sec);
#endif

	for(i=0; i<RFS_GET_RETRY_COUNT; i++)
	{
		rfs_err = rfs_get(latest_file_loc, addr, fstat_buf->st_size);
	VERIFYC(nErr, fstat_buf->st_size == rfs_err, rfs_err);

	
	retVal = gateway_memory_change_prot(addr, 
		 									nBAlignedFileSz, 
		 								GATEWAY_MAP_PROT_READ);

	VERIFYC(nErr, 0 == retVal, FS_GATEWAY_PERM_CHG_FAIL);

		/*Check if file size is less than size of a 32bit ELF header*/
		VERIFYC(nErr, sizeof(Elf32_Ehdr) < fstat_buf->st_size, FS_GATEWAY_BAD_ELF);

		//Get Ehdr
		pEh = (Elf32_Ehdr*)addr;

		/*Is the program header table within the bounds of this file?*/
		/*Note that e_phoff is offset into file (from start) where program header table starts, i.e location of first program header entry*/
		VERIFYC(nErr, pEh->e_phoff < fstat_buf->st_size , FS_GATEWAY_BAD_ELF); //program header offset is ok
		VERIFYC(nErr, (pEh->e_phnum * sizeof(Elf32_Phdr)) < (fstat_buf->st_size - pEh->e_phoff), FS_GATEWAY_BAD_ELF); //program header size is ok		
		
		//Start of program header table
		pPhdr = (Elf32_Phdr*)((unsigned int)pEh + pEh->e_phoff);

		/*Find hash segment, ensure that it lies within bounds of the file*/
		int index =0, iHashSegIdx =-1;
		for(index = 0; index < pEh->e_phnum; index++)
		{
			/*look up hash segment*/
			if( MI_PBT_SEGMENT_TYPE_VALUE(pPhdr[index].p_flags) == MI_PBT_HASH_SEGMENT)
			{
				/*found hash segment, now check if its within the file bounds*/
				VERIFYC(nErr, pPhdr[index].p_offset  < fstat_buf->st_size, FS_GATEWAY_BAD_HASH_SEG); //offset of hash seg falls within file		
				VERIFYC(nErr, pPhdr[index].p_filesz < (fstat_buf->st_size - pPhdr[index].p_offset), FS_GATEWAY_BAD_HASH_SEG); //segment is contained within file		
				iHashSegIdx = index;
				break; //we found the hash segment now stop.
			}
		}

		/*If there is no hash segment, the file is not signed or corrupted, abort*/
		VERIFYC(nErr, (iHashSegIdx != -1) , FS_GATEWAY_HASH_SEGMENT_NOT_FOUND);		

		//Do auth here
		retVal = gateway_security_do_auth(latest_file_loc, pEh);

		//If auth succesful continue using fetched file 
		if(FS_GATEWAY_SECURITY_SUCCESS == retVal) break;

		//We must fail if we fail to authenticate after exausting all retry attempts
		if(i == RFS_GET_RETRY_COUNT-1)
	VERIFYC(nErr, FS_GATEWAY_SECURITY_SUCCESS == retVal, FS_GATEWAY_AUTH_FAIL);

		//If auth failed, open the buffer and fetch file again
		retVal = gateway_memory_change_prot(addr, 
		 									nBAlignedFileSz, 
		 									GATEWAY_MAP_PROT_WRITE);
	
		VERIFYC(nErr, 0 == retVal, FS_GATEWAY_PERM_CHG_FAIL);

		qurt_printf("rfs_gateway retry (auth fail) : %d\n", i+2);
	}

#ifdef USERPD_PROFILE_DEBUG
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("USERPD RFS GET DONE time:%lu\n",curr_time_in_micro_sec);
#endif 
	//If auth goood, Add file to our records.
	nFileIdx = rfs_gateway_add_new_file();
	VERIFYC(nErr, -1 != nFileIdx, FS_GATEWAY_MAX_NUM_FILES_OPEN);
	rfs_gateway_file_q[nFileIdx].path = path;
	rfs_gateway_file_q[nFileIdx].pathlen = strlen(path);
	rfs_gateway_file_q[nFileIdx].filesz = fstat_buf->st_size;
	rfs_gateway_file_q[nFileIdx].file_start = addr;
	
	//Install file in RAMFS
	retVal = qurt_fs_extrafile_add(path, addr, nBAlignedFileSz);
	VERIFYC(nErr, -1 != retVal, FS_GATWAY_RAMFS_INSTALL_FAIL);
#ifdef USERPD_PROFILE_DEBUG
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("WLANPD RFS GATEWAY COMPLETE:%lu\n",curr_time_in_micro_sec);
#endif 

	{
		//Clean-up, success logging
		if(fstat_buf) free(fstat_buf);
		rfs_gateway_errcode = 0;
		return;
	}

	//Catch errors, release buffers and log error codess
	CATCH(nErr)
	{
		if(fstat_buf) free(fstat_buf);
		if(addr != GATEWAY_MAP_FAILED) gateway_memory_unmap(addr);
		rfs_gateway_errcode = nErr;
		return;
	}
}


int rfs_gateway_error(void)
{
	return rfs_gateway_errcode;
}