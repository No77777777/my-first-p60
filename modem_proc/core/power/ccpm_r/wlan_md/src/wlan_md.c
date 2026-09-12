/*!
  @file 
  wlan_md.c

  @brief

*/

/*==============================================================================

  Copyright (c)  QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     -------------------------------------------------------------
            
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "wlan_md.h"
#include "wlan_md_elf.h"
#include "wlan_md_cipher.h"
#include "qurt.h"
#include "qurt_thread_context_restricted.h"
#include "pd_mon_qurt.h"
#include <string.h>
#include "tms_utils.h"
#include "qurt_qdi_driver.h"
#include "qurt_qdi.h"
#include "fs_lib.h"
#include "DDITimer.h"
#include "DDITimetick.h"
#include "DALFramework.h"
#include "DALSys.h"
#include "DalDevice.h"
#include "pd_dump.h"
#include "HALhwio.h"
#include "time_svc.h"

wlan_md_ctx_t g_wlan_md_ctx;

#define WLAN_MD_ENCRYPT (g_wlan_md_ctx.encrypt_enable == TRUE)

/*CBSP will allocate 1 MB buffer later*/
uint8 g_tcb_buffer[1024*100];
uint8 g_image_ver_buffer[1024];

#ifdef WLAN_MD_IMAGE_VER
extern char QC_IMAGE_VERSION_STRING_AUTO_UPDATED[];
extern char IMAGE_VARIANT_STRING_AUTO_UPDATED[];
extern char OEM_IMAGE_VERSION_STRING_AUTO_UPDATED[];
extern char OEM_IMAGE_UUID_STRING_AUTO_UPDATED[];
#define MD_IMAGE_VERSION_SIZE  128
#endif

#define WLAN_MD_FILE_NAME_MAX       128
#define WLAN_MD_FILE_ELF_TEMPLATE      "%s.%04d%02d%02d_%02d%02d.%02d.elf"

/*define bit for region to include in PDR or SSR or both*/
#define WLAN_MD_REGION_PDR_INC				0x1 /*bit 1 set*/
#define WLAN_MD_REGION_SSR_INC				0x2 /*bit 2 set*/

static wlan_md_ss_pvt_toc_type * wlan_md_toc_pvt_ptr;
static wlan_md_global_toc_type * wlan_md_toc_ptr ;
	
static uint8 wlan_rsa_encr_aes_key[WLAN_RSA_KEY_LEN];
static uint8 wlan_aes_key_iv[WLAN_MD_NUM_SMEM_ENTRIES*WLAN_SW_AES_IV_SIZE];
static uint8 wlan_aes_key_mac[WLAN_MD_NUM_SMEM_ENTRIES*WLAN_SW_AES_MAC_SIZE];

wlan_md_blob_entry wlan_md_blob[WLAN_MD_NUM_SMEM_ENTRIES];
wlan_md_ss_region wlan_md_base[WLAN_MD_NUM_SMEM_ENTRIES + WLAN_MD_NUM_ENC_ENTRIES];

uint32 wlan_md_blob_entries=0;
wlan_md_blob_entry wlan_md_non_encrypt_blob[WLAN_MD_NUM_ENC_ENTRIES];
uint32 wlan_md_non_encrypt_blob_entries=0;
extern void wlan_md_qdi_init(void);

#define WLAN_MINIDUMP_COUNTER_FILENAME "/ramdumps/wlan_minidump.count"

/* This function is called to add/update WLAN minidump region entry in SMEM */
void wlan_md_update_smem_entry(uint32 paddr , char* name, uint32 size, uint32 entry_index)
{
  wlan_md_ss_region *entry = NULL;

  if (entry_index >= (WLAN_MD_NUM_SMEM_ENTRIES + WLAN_MD_NUM_ENC_ENTRIES))
       return;

  entry = &wlan_md_base[entry_index];

  strlcpy((char *)entry->region_name, name , WLAN_MD_REGION_NAME_LENGTH);
  entry->region_base_address = paddr; // Need Physical address of this memory
  entry->region_size = size;
  entry->md_valid = WLAN_MD_REGION_VALID;
  wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].ss_region_count++;
  wlan_md_toc_pvt_ptr->ss_region_count++;
}

/* This function handles USER PD QDI call to register region, it will save VA/PA in static global as well as add entry in SMEM */
int wlan_md_register_region(int client_handle, uint32 *buffer)
{
   uint32 num_entry=0, index=0, non_encrypt_index=0, smem_index=0;
   uint32 *temp;
   uint32 flag;
   if (!buffer)
      return -1;
 
   wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].ss_region_count=0;
   wlan_md_toc_pvt_ptr->ss_region_count=0;
   /*update the 3 entries of encryption in smem*/ 
   wlan_md_non_encrypt_blob[non_encrypt_index].region_va = (uint32)&wlan_rsa_encr_aes_key;
   wlan_md_non_encrypt_blob[non_encrypt_index].region_pa = (uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_rsa_encr_aes_key);
   wlan_md_non_encrypt_blob[non_encrypt_index].region_size = sizeof(wlan_rsa_encr_aes_key);
   non_encrypt_index++;
   wlan_md_update_smem_entry((uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_rsa_encr_aes_key),"wlan_aes_key", WLAN_RSA_KEY_LEN, smem_index++);
   
   wlan_md_non_encrypt_blob[non_encrypt_index].region_va = (uint32)&wlan_aes_key_iv;
   wlan_md_non_encrypt_blob[non_encrypt_index].region_pa = (uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_aes_key_iv);
   wlan_md_non_encrypt_blob[non_encrypt_index].region_size = sizeof(wlan_aes_key_iv);
   non_encrypt_index++;
   wlan_md_update_smem_entry((uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_aes_key_iv),"wlan_iv_key", (WLAN_SW_AES_IV_SIZE * WLAN_MD_NUM_SMEM_ENTRIES), smem_index++);
   
   wlan_md_non_encrypt_blob[non_encrypt_index].region_va = (uint32)&wlan_aes_key_mac;
   wlan_md_non_encrypt_blob[non_encrypt_index].region_pa = (uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_aes_key_mac);
   wlan_md_non_encrypt_blob[non_encrypt_index].region_size = sizeof(wlan_aes_key_mac);
   non_encrypt_index++;
   wlan_md_update_smem_entry((uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_aes_key_mac),"wlan_mac_key", (WLAN_SW_AES_MAC_SIZE * WLAN_MD_NUM_SMEM_ENTRIES), smem_index++);

   wlan_md_non_encrypt_blob_entries = non_encrypt_index;

   /* add all the wlan va_pa_mapping*/
   wlan_md_update_smem_entry((uint32)qurt_lookup_physaddr((qurt_addr_t)&wlan_md_blob), "wlan_va_pa_map", sizeof(wlan_md_blob), smem_index++); 

   temp = buffer;
   num_entry = *temp++;

   if (num_entry > WLAN_MD_NUM_SMEM_ENTRIES)
	   num_entry = WLAN_MD_NUM_SMEM_ENTRIES;
  
   while (index<num_entry)
   {
      wlan_md_blob[index].region_va = *temp++;
      wlan_md_blob[index].region_pa = *temp++;
      wlan_md_blob[index].region_size = *temp++;
      strlcpy(wlan_md_blob[index].region_name, (const char *)temp , WLAN_MD_REGION_NAME_LENGTH);
      temp = (uint32 *)(((char *)temp) + WLAN_MD_REGION_NAME_LENGTH);
	  flag = *temp++;
	  if(flag & WLAN_MD_REGION_SSR_INC) /*include blob to smem if bit_1 is set, else skip */
	  {
		wlan_md_update_smem_entry(wlan_md_blob[index].region_pa, wlan_md_blob[index].region_name, wlan_md_blob[index].region_size, smem_index++);
	  }
      index++;
   }
   wlan_md_blob_entries=index;

   
   return 1;
}

/* This function is registered with PD MON and will be invoked when PD goes down
 * This will create an elf and encrypt and write the data into file system */
void wlan_md_err_fatal_cb(int client_handle, uint32 thread_info, uint32 n_tcb, uint32 user_data)
{
   uint32 i=0, offset=0, total_bytes=0, elfcore_size=0;
   int written=0, read=0, counter=0;
   qurt_thread_t *thread = (qurt_thread_t *)thread_info;
   char *elfcore_buf;
   Elf32_Ehdr *ehdr;
   Elf32_Phdr *phdr;
   void *buffer;
   uint32 tcb_size = qurt_system_tcb_dump_get_size();
   uint8 *tcb_buffer = &g_tcb_buffer[0];
#ifdef WLAN_MD_IMAGE_VER   
   uint8 *ver_buffer = &g_image_ver_buffer[0];
#endif
   int fd, counter_fd;
   char filename[WLAN_MD_FILE_NAME_MAX];
   time_julian_type sys_time;

   if (!wlan_md_blob_entries)
      return;

   /*dog_force_kick();*/

   /*Open counter file to read mini dump counter*/
   counter_fd = rfs_open(WLAN_MINIDUMP_COUNTER_FILENAME, RFS_O_RDWR, 755); 
   if (counter_fd < 0)
   {
     counter_fd = rfs_open(WLAN_MINIDUMP_COUNTER_FILENAME, RFS_O_WRONLY | RFS_O_CREAT, 755);
     counter = 0; 
     if (counter_fd >= 0)
     {
        written = rfs_write(counter_fd, (char*)&counter, sizeof(counter));
        rfs_close(counter_fd);
     }
   }
   else
   {
      read = rfs_read(counter_fd, (char*)&counter, sizeof(counter));
      if (read != sizeof(counter))
      {
	 counter=0;   
      }
      rfs_close(counter_fd);
   }

	time_get_julian(&sys_time);
   /*Generate WLAN minidump file using counter*/
   written = tms_utils_fmt(filename, sizeof(filename), WLAN_MD_FILE_ELF_TEMPLATE, "/ramdumps/wlan_minidump",sys_time.year,sys_time.month,sys_time.day,sys_time.hour,sys_time.minute,counter);
   if (written < sizeof(filename))
   {
      fd = rfs_open(filename, RFS_O_WRONLY | RFS_O_CREAT, 755);
      if (fd < 0)
        return;       
   }
   else
   {
      return;
   }

#ifdef WLAN_MD_IMAGE_VER
   /*GET IMAGE VERSION STRING*/
   strlcpy((char *)ver_buffer, QC_IMAGE_VERSION_STRING_AUTO_UPDATED, MD_IMAGE_VERSION_SIZE);
   ver_buffer = (uint8 *)((uint32)ver_buffer + (uint32)MD_IMAGE_VERSION_SIZE);
   strlcpy((char *)ver_buffer, IMAGE_VARIANT_STRING_AUTO_UPDATED, MD_IMAGE_VERSION_SIZE);
   ver_buffer = (uint8 *)((uint32)ver_buffer + (uint32)MD_IMAGE_VERSION_SIZE);
   strlcpy((char *)ver_buffer, OEM_IMAGE_VERSION_STRING_AUTO_UPDATED, MD_IMAGE_VERSION_SIZE);
   ver_buffer = (uint8 *)((uint32)ver_buffer + (uint32)MD_IMAGE_VERSION_SIZE);
   strlcpy((char *)ver_buffer, OEM_IMAGE_UUID_STRING_AUTO_UPDATED, MD_IMAGE_VERSION_SIZE);
   ver_buffer = (uint8 *)((uint32)ver_buffer + (uint32)MD_IMAGE_VERSION_SIZE);
#endif

   /*GET USER PD TCB INFO*/
   for (i=0; i<n_tcb; i++)
   {
      qurt_system_tcb_dump_get(thread[i], (void *)tcb_buffer, tcb_size);
      tcb_buffer = (uint8 *)((uint32)tcb_buffer + tcb_size);
   }

   /*Iterate through blobs, exncrypt and invoke RFS API to save*/
   elfcore_size = sizeof(*ehdr)+(sizeof(*phdr)*(wlan_md_blob_entries+wlan_md_non_encrypt_blob_entries+2/*TCB+IMAGE VESION*/));
   ehdr = malloc(elfcore_size);
   elfcore_buf = (char *)ehdr;
   if (!elfcore_buf)
	return;

   memcpy(ehdr->e_ident, ELFMAG, SELFMAG);
   ehdr->e_ident[EI_CLASS] = ELFCLASS32;
   ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
   ehdr->e_ident[EI_VERSION] = EV_CURRENT;
   ehdr->e_ident[EI_OSABI] = ELFOSABI_SYSV;
   ehdr->e_type = ET_CORE;
   ehdr->e_version = EV_CURRENT;
   ehdr->e_phoff = sizeof(*ehdr);
   ehdr->e_ehsize = sizeof(*ehdr);
   ehdr->e_phentsize = sizeof(*phdr);
   ehdr->e_phnum = (wlan_md_blob_entries + wlan_md_non_encrypt_blob_entries + 2/*TCB+IMAGE VESION*/);
   offset = elfcore_size;
   phdr = (Elf32_Phdr *)(ehdr + 1);

   /*IMAGE VERSION STRING REGION*/
   phdr->p_type = PT_LOAD;
   phdr->p_offset = offset;
   phdr->p_vaddr = (uint32)&g_image_ver_buffer;
   phdr->p_paddr = (uint32)qurt_lookup_physaddr((qurt_addr_t)&g_image_ver_buffer);
   phdr->p_filesz = phdr->p_memsz = sizeof(g_image_ver_buffer); 
   phdr->p_flags = PF_R | PF_W | PF_X;
   offset += phdr->p_filesz;
   phdr++;

   /*TCB REGION*/
   phdr->p_type = PT_LOAD;
   phdr->p_offset = offset;
   phdr->p_vaddr = (uint32)&g_tcb_buffer;
   phdr->p_paddr = (uint32)qurt_lookup_physaddr((qurt_addr_t)&g_tcb_buffer);
   phdr->p_filesz = phdr->p_memsz = sizeof(g_tcb_buffer);
   phdr->p_flags = PF_R | PF_W | PF_X;
   offset += phdr->p_filesz;
   phdr++;

   for (i = 0; i < wlan_md_blob_entries; i++, phdr++) {
       phdr->p_type = PT_LOAD;
       phdr->p_offset = offset;
       phdr->p_vaddr = wlan_md_blob[i].region_va;
       phdr->p_paddr = wlan_md_blob[i].region_pa;
       phdr->p_filesz = phdr->p_memsz = wlan_md_blob[i].region_size;
       phdr->p_flags = PF_R | PF_W | PF_X;
       offset += phdr->p_filesz;
   }

   for (i = 0; i < wlan_md_non_encrypt_blob_entries; i++, phdr++) {
       phdr->p_type = PT_LOAD;
       phdr->p_offset = offset;
       phdr->p_vaddr = wlan_md_non_encrypt_blob[i].region_va;
       phdr->p_paddr = wlan_md_non_encrypt_blob[i].region_pa;
       phdr->p_filesz = phdr->p_memsz = wlan_md_non_encrypt_blob[i].region_size;
       phdr->p_flags = PF_R | PF_W | PF_X;
       offset += phdr->p_filesz;
   }

   /*Encrypt regions*/
   //wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].encryption_status  = WLAN_MD_IN_PROGRESS_ENCRYPTION;

   /* Do any init, and generate the RSA key required */
   if (WLAN_MD_ENCRYPT) {
	   if (!g_wlan_md_ctx.crypto_init_done) {
		  wlan_md_crypto_init();
		  g_wlan_md_ctx.crypto_init_done = 1;
	   }
   }

   /*Write ELF header*/
   buffer = (void *)(ehdr);
   total_bytes = elfcore_size;
   written=0;
   while(total_bytes !=0)
   {
       written = rfs_write(fd, (void *)buffer, total_bytes);
       if (written < 0)
          goto error;  
       buffer = (void *)((uint32)buffer + written);
       total_bytes = total_bytes - written;
   }

   /*Encrypt/Write IMAGE VERSION STRING REGION*/
   buffer = (void *)&g_image_ver_buffer;
   total_bytes = sizeof(g_image_ver_buffer);
   if (WLAN_MD_ENCRYPT) {
	   wlan_md_encrypt_region((uint64)(buffer), 
						  (uint64)(total_bytes), 0);
   }
   written=0;

   while(total_bytes !=0)
   {
       written = rfs_write(fd, (void *)buffer, total_bytes);
       if (written < 0)
          goto error;  
       buffer = (void *)((uint32)buffer + written);
       total_bytes = total_bytes - written;
   }

   /*Encrypt/Write TCB INFO REGION*/
   buffer = (void *)&g_tcb_buffer;
   total_bytes = sizeof(g_tcb_buffer);
   if (WLAN_MD_ENCRYPT) {
	   wlan_md_encrypt_region((uint64)(buffer), 
						  (uint64)(total_bytes), 1);
   }
   written=0;

   while(total_bytes !=0)
   {
       written = rfs_write(fd, (void *)buffer, total_bytes);
       if (written < 0)
          goto error;  
       buffer = (void *)((uint32)buffer + written);
       total_bytes = total_bytes - written;
   }

   /*Encrypt/Write registered blobs*/
   i=0;
   while (i < wlan_md_blob_entries)
   {
      buffer = (void *)(wlan_md_blob[i].region_va);
      total_bytes = wlan_md_blob[i].region_size;

      if (WLAN_MD_ENCRYPT) {
		  wlan_md_encrypt_region((uint64)(buffer), 
						  (uint64)(total_bytes), i+2);
      }
      written=0;
      if (!buffer)
	    goto error;

      while(total_bytes !=0)
      {
    	  written = rfs_write(fd, (void *)buffer, total_bytes);
  	  if (written < 0)
		goto error;  
  	  buffer = (void *)((uint32)buffer + written);
  	  total_bytes = total_bytes - written;
      }
      i++;
   }

   wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].encryption_status  = WLAN_MD_SS_ENCR_DONE;

   /*Write encryption artifact blobs*/
   i=0;
   while (i < wlan_md_non_encrypt_blob_entries)
   {
      buffer = (void *)(wlan_md_non_encrypt_blob[i].region_va);
      total_bytes = wlan_md_non_encrypt_blob[i].region_size;
      written=0;
      if (!buffer)
	 goto error;

      while(total_bytes !=0)
      {
    	  written = rfs_write(fd, (void *)buffer, total_bytes);
  	  if (written < 0)
		goto error;  
  	  buffer = (void *)((uint32)buffer + written);
  	  total_bytes = total_bytes - written;
      }
      i++;
   }

   /*Update the minidump counter*/
   counter_fd = rfs_open(WLAN_MINIDUMP_COUNTER_FILENAME, RFS_O_WRONLY | RFS_O_CREAT, 755);
   if (counter_fd >= 0)
   {
     counter++;
     written = rfs_write(counter_fd, (char*)&counter, sizeof(counter));
     rfs_close(counter_fd);
   }

error:
   free(ehdr);
   rfs_close(fd);
   return;
}

/* This function is used to register callback with PD MON */
void wlan_md_register_callback()
{
   pd_dump_cb_info_t cb_info;
   cb_info.cb_fn = wlan_md_err_fatal_cb;
   cb_info.user_data=0;
   /*Register error callback with PD MON*/
   pd_dump_cb_enable(&cb_info);
   return;
}

/* This function is used to deregister callback with PD MON */
void wlan_md_deregister_callback()
{
   pd_dump_cb_info_t cb_info;
   cb_info.cb_fn = wlan_md_err_fatal_cb;
   cb_info.user_data=0;
   /*Register error callback with PD MON*/
   pd_dump_cb_disable(&cb_info);
   return;
}


/* This function is used to register callback with PD MON */
void wlan_md_config_callback(boolean enable)
{
	if (enable == TRUE)
		{
			wlan_md_register_callback();
		}
	else
		{
			wlan_md_deregister_callback();
		}
   return;
}


/* This function checks if minidump file is present in file system*/
uint32 wlan_md_check_efs()
{
  uint32 minidump_mask;

  if (!(efs_get(WLAN_MD_EFS_FILE, &minidump_mask, sizeof(minidump_mask))==sizeof(minidump_mask)))
  {
    minidump_mask = WLAN_MD_DEFAULT_EFS_MASK;
  }
  
  return minidump_mask;
}

/* This function to checks if minidump is enabled or not.*/
static uint32 wlan_md_check_non_legacy_enable(void)
{
	smem_alloc_params_type non_legacy_params;
	//Get Non-Legacy SMEM region
	non_legacy_params.remote_host = SMEM_TZ  ;
	non_legacy_params.smem_type		= WLAN_MD_SMEM_PRIVATE_ID;
	non_legacy_params.size 	   = 0;
	non_legacy_params.buffer      = NULL;
	non_legacy_params.flags       = 0  ;
	
	/*First check for new SMEM ID as per new TZ implementation if failed, check for global SMEM ID to support backward compatibility*/
	if(!((smem_get_addr_ex(&non_legacy_params) == SMEM_STATUS_SUCCESS) && (non_legacy_params.size != 0) && (non_legacy_params.buffer != NULL))) {
		//Seprate toc private structure is not allocated for CNSS by QSEE, Now check for global SMEM private toc structure
		non_legacy_params.smem_type   = WLAN_MD_SMEM_GLOBAL_ID;
		
		if(!((smem_get_addr_ex(&non_legacy_params) == SMEM_STATUS_SUCCESS) && (non_legacy_params.size != 0) && (non_legacy_params.buffer != NULL))) {
			//SMEM non legacy region is not allocated for MPSS by QSEE
			return WLAN_MD_SS_DISABLED;
		}
		else {
			//Non-Legacy init    			
			wlan_md_toc_pvt_ptr = (wlan_md_ss_pvt_toc_type*)(non_legacy_params.buffer+sizeof(wlan_md_ss_pvt_toc_type));
		}
	}
	else {
		//Non-Legacy init    			
		wlan_md_toc_pvt_ptr = (wlan_md_ss_pvt_toc_type*)(non_legacy_params.buffer);
	}
	
	return (wlan_md_toc_pvt_ptr->md_ss_enable_status);
}

/*intialize toc pointers*/
static void wlan_md_memset_toc_ptr(void)
{
	/*intializing non-lagecy private toc pointer*/
	wlan_md_toc_pvt_ptr->md_ss_smem_regions_baseptr = qurt_lookup_physaddr((qurt_addr_t)wlan_md_base);
	wlan_md_toc_pvt_ptr->ss_region_count      		= 0;
	wlan_md_toc_pvt_ptr->encryption_status    		= WLAN_MD_SS_ENCR_NOT_DONE;
	wlan_md_toc_pvt_ptr->encryption_required  		= WLAN_MD_SS_ENCR_REQ;
	
	/*intializing global toc pointer*/
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].md_ss_smem_regions_baseptr = qurt_lookup_physaddr((qurt_addr_t)wlan_md_base); 
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].ss_region_count      = 0;
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].encryption_status    = WLAN_MD_SS_ENCR_NOT_DONE;
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].encryption_required  = WLAN_MD_SS_ENCR_NOTREQ;
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].md_ss_enable_status 	= WLAN_MD_SS_ENABLED;
	wlan_md_toc_ptr->md_ss_toc[MD_SS_CNSS].md_ss_toc_init      	= 1;	
}


/* This function is invoked as part of rc init sequence and initializes WLAN minidump, open QDI channel etc.*/
void wlan_md_init(void)
{
	uint32 md_ss_enable_status;
	uint32 minidump_enable=FALSE;
	uint32 smem_legacy_size = WLAN_MD_LEGACY_SMEM_SIZE; 
	wlan_md_toc_ptr = NULL;
	wlan_md_toc_pvt_ptr = NULL;
	
	/*Check minidump enable/disable flag set by TZ*/
	md_ss_enable_status = wlan_md_check_non_legacy_enable();

	if(md_ss_enable_status == WLAN_MD_SS_IMPL_DEFINE || md_ss_enable_status == WLAN_MD_SS_IMPL_DEFINE_NEW) {
		/*Non secure device. Check EFS*/
		if (wlan_md_check_efs() != WLAN_MD_DEFAULT_EFS_MASK) {
		  minidump_enable = TRUE;
		}
		g_wlan_md_ctx.encrypt_enable = FALSE;
	}
	else if(md_ss_enable_status == WLAN_MD_SS_ENABLED )  {
		/*Secure device and minidump is enabled*/
		minidump_enable = TRUE;
		g_wlan_md_ctx.encrypt_enable = TRUE;
	}
	else
	{
		/*Secure device and minidump is disabled*/
		minidump_enable = FALSE;
	}


	if (minidump_enable) {
		wlan_md_toc_ptr = (wlan_md_global_toc_type *)smem_get_addr(WLAN_MD_SMEM_GLOBAL_ID, &smem_legacy_size);
		if (wlan_md_toc_ptr != NULL && wlan_md_toc_pvt_ptr != NULL) {
			wlan_md_memset_toc_ptr();
			/*Register callback to run in last bucket with order NORMAL*/
			wlan_md_qdi_init();
			wlan_md_register_callback();	
		}
	}
	return;
}
