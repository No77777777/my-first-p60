/** vi: tw=128 ts=3 sw=3 et
@file pd_dump.c
@brief This file contains the API details for the Protection Domain Monitor, API 1.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2014 - 2020,2022 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump.c#6 $
$DateTime: 2022/06/22 22:40:36 $
$Change: 38006310 $
$Author: pwbldsvc $
===============================================================================*/

#include "stdlib.h"
#include "stringl/stringl.h"
#include "pd_dump_interface_qdi.h"
#include "qurt.h"
#include "qurt_printf.h"
#include "qurt_thread_context.h"
#include "qurt_restricted.h"
#include "queue.h"
#include "pd_dump_fs_al.h"
#include "err.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"

#include "pd_dump.h"
#include "pd_elf.h"
#include "pd_elf_hexagon.h"
#include "pd_mon_qurt.h"
#include "pd_dump_security.h"
#include "pd_mon_dump.h"

#if defined(PD_MON_TRACER_SWEVT)
#include "tracer.h"
#include "pd_mon_tracer_swe.h"
#endif

extern void dog_force_kick(void);
extern int pd_dump_copy_to_user(int client_handle, void *dest, const void *src, unsigned len);
extern int pd_dump_copy_from_user(int client_handle, void *dest, const void *src, unsigned len);
extern void pd_dump_interface_data(struct pd_dump_device_s* device_p);

/* Size should be 4 byte aligned as per the ABI spec */
#define PD_DUMP_NOTE_SECTION_NAMESZ    32
#define PD_DUMP_NOTE_SECTION_DESCSZ    1024

#if (PD_DUMP_NOTE_SECTION_NAMESZ % 4 != 0)
#error invalid configuration
#endif

#if (PD_DUMP_NOTE_SECTION_DESCSZ % 4 != 0)
#error invalid configuration
#endif


#if defined(ADD_ROOT_PD_IMAGE_VER)
extern char QC_IMAGE_VERSION_STRING_AUTO_UPDATED[];
extern char IMAGE_VARIANT_STRING_AUTO_UPDATED[];
extern char OEM_IMAGE_VERSION_STRING_AUTO_UPDATED[];
extern char OEM_IMAGE_UUID_STRING_AUTO_UPDATED[];


#define PD_DUMP_IMAGE_VERSION_SIZE        128
#endif

/////////////////////////////////////////////////////////////////////
// Localized Type Declarations
/////////////////////////////////////////////////////////////////////



struct pd_dump_note_section_s
{
  char    name[PD_DUMP_NOTE_SECTION_NAMESZ];  /* Readelf owner, must be NULL terminated */
  char    desc[PD_DUMP_NOTE_SECTION_DESCSZ];  /* Readelf data, may not be NULL terminated, but is descsz bound*/
  uint32  type;                               /* Readelf description */
  uint32  descsz;
};

struct pd_dump_queue_note_section_s
{
  /* q_link_type - Must be the first element */
  q_link_type                      node;
  struct pd_dump_note_section_s    note;
};

struct Thread_s /**< qurt supplied with QDI_OS_DUMP_PROCESS_EXIT */
{
   qurt_thread_t tid;
};

const char* pd_mon_internal_prefix = "pd_dump_"; /**< RFSA Volume */

/////////////////////////////////////////////////////////////////////
// Localized Storage
/////////////////////////////////////////////////////////////////////

static struct pd_dump_internal_s
{
   struct pd_dump_device_s* head;

   qurt_mutex_t mutex;

   boolean dump_enabled;

   pd_dump_cb_info_t pd_dump_cb; /** PD dump external callback */

} pd_dump_internal;

typedef struct pd_dump_mapped_addr_s
{
   unsigned long mapped_addr_va;
   unsigned int len;
   struct pd_dump_mapped_addr_s *next;
} pd_dump_mapped_addr_t;

pd_dump_mapped_addr_t *pd_dump_mapped_addr_list;

/////////////////////////////////////////////////////////////////////
// Implementation Details
/////////////////////////////////////////////////////////////////////

static void pd_dump_free_mappings(void)
{
  pd_dump_mapped_addr_t *temp = pd_dump_mapped_addr_list;
  while(NULL != temp)
  {
    if(0 != qurt_mem_munmap((void *)temp->mapped_addr_va , temp->len))
    {
      TMS_MSG_ERROR_2("unmapping failed for VA 0x%x, len 0x%x", temp->mapped_addr_va,temp->len);
    }
    pd_dump_mapped_addr_list = pd_dump_mapped_addr_list->next;
    free(temp);
    temp = pd_dump_mapped_addr_list;
  }
}

static unsigned long long tms_mmu_entry_get_paddr(qurt_mmu_entry_t entry)
{
  unsigned long long phy_addr = (entry & 0XFFFFFF)>>1;
  unsigned long size_bits = (entry & 0X7F);
  unsigned long size = 0x1;

  phy_addr |= (entry >> 38) & 0x800000;     //This takes care of EP bit from the TLB entry

  while ((size_bits & 0x1)==0)
  {
   size=4*size;
   size_bits=size_bits>>1;
  }

  /*if size > 4K, remove the last bit set in the phy_addr*/
  if (size>0x1) 
     phy_addr=phy_addr&(phy_addr-1ull);

  phy_addr = phy_addr << 12;
  return phy_addr;
}

boolean is_pd_dump_enabled(void)
{
   /* Check if PD Dumps is allowed according to security policy */
   if ( TRUE == pd_dump_security_debug_enabled() 
        /* Check if PD Dumps is disabled through dalcfg\servreg */
       && TRUE == pd_dump_internal.dump_enabled)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

int pd_dump_enqueue_note_section
(
  struct pd_dump_generic_s* device_p,
  char   * name,
  char   * desc,
  uint32 descsz,
  uint32  type
)
{
 struct pd_dump_queue_note_section_s * note_section = calloc(1, sizeof(struct pd_dump_queue_note_section_s));

  if ( note_section == NULL )
     return PD_DUMP_STATUS_ERROR;

  note_section->note.descsz = descsz > PD_DUMP_NOTE_SECTION_DESCSZ ? PD_DUMP_NOTE_SECTION_DESCSZ : descsz;
  /* Name cannot be NULL and should be greater than 1 character */
  /* strlcpy will ensure NULL termination */
  if ( 1 >= strlcpy(note_section->note.name, name, PD_DUMP_NOTE_SECTION_NAMESZ))
  {
    free(note_section);
    return PD_DUMP_STATUS_ERROR;
  }
  /* Description can be NULL */
  /* desc cannot use strlcpy */
  if (desc != NULL && descsz > 0 )
  {
    memscpy(note_section->note.desc, PD_DUMP_NOTE_SECTION_DESCSZ, desc, descsz );
  }
  else
  {
    note_section->note.descsz = 0;
  }
  note_section->note.type = type;

  (void) q_link( note_section , &(note_section->node) );
  q_put( &(device_p->q_of_pd_dump_queue_note_section_s), &(note_section->node) );

  return PD_DUMP_STATUS_SUCCESS;

}

static PD_ELF_STATUS pd_dump_dequeue_write_note_section
(
  struct pd_dump_generic_s* device_p,
  boolean                  clean_only,
  unsigned int           * num_note_section
)
{
  struct pd_dump_queue_note_section_s * note_section;
  
  if ( num_note_section != NULL )
    *num_note_section = q_cnt(&(device_p->q_of_pd_dump_queue_note_section_s));

  note_section = (struct pd_dump_queue_note_section_s *)q_get(&(device_p->q_of_pd_dump_queue_note_section_s));

  while ( note_section != NULL )
  {
     if ( clean_only == FALSE )
     {
        if (PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_alloc(&device_p->elf_hdr, &device_p->elf_prgtab_head, PT_NOTE, note_section->note.name, note_section->note.type, note_section->note.desc, note_section->note.descsz , NULL))
        {
#if defined(PD_MON_TRACER_SWEVT)
          tracer_event_simple_vargs(PD_DUMP_SWE_HPE, 2, device_p->hash, device_p->pn.pid);
#endif
          TMS_MSG_ERROR_1("PD dump headers alloc error in prgtab .process %d", device_p->pn.pid);

          /* Flush the queue */
          free(note_section); //last node extracted from queue
          note_section = (struct pd_dump_queue_note_section_s *)q_get(&(device_p->q_of_pd_dump_queue_note_section_s));
          while ( NULL != note_section )
          {
            free(note_section);
            note_section = (struct pd_dump_queue_note_section_s *)q_get(&(device_p->q_of_pd_dump_queue_note_section_s));
          }
          return PD_ELF_STATUS_ERROR;
        }
     }
     free(note_section);
     note_section = (struct pd_dump_queue_note_section_s *)q_get(&(device_p->q_of_pd_dump_queue_note_section_s));
  }
  return PD_ELF_STATUS_SUCCESS;
}

static PD_ELF_STATUS pd_dump_wrapper_prgtab_alloc( struct pd_dump_generic_s* device_p, void *BlockAddr_orig, unsigned int Blocklen, unsigned int * num_included_blocks, void *mapped_addr)
{
   if (PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_alloc(&device_p->elf_hdr, &device_p->elf_prgtab_head, PT_LOAD, NULL, NT_NONE, BlockAddr_orig, Blocklen, mapped_addr))
   {
#if defined(PD_MON_TRACER_SWEVT)
     tracer_event_simple_vargs(PD_DUMP_SWE_HPE, 2, device_p->hash, device_p->pn.pid);
#endif
     TMS_MSG_ERROR_1("PD dump headers alloc error in prgtab .data %d", device_p->pn.pid);
     return PD_ELF_STATUS_ERROR;
   }
   (*num_included_blocks)++;

   return PD_ELF_STATUS_SUCCESS;
}
static PD_ELF_STATUS pd_dump_mem_blocks_untrusted(struct pd_dump_generic_s* device_p, qurt_mmu_entry_t * MemBlock, unsigned int num_blocks, unsigned int * num_included_blocks)
{
  unsigned int i;
  unsigned long BlockLen = 0, BlockAddr =0;
  unsigned long long BlockPhyAddr =0;
  unsigned short BlockCCCC = 0;
  pd_dump_mapped_addr_t *temp =  NULL;

  for (i = 0; i < num_blocks; ++i)
  {
    BlockLen = qurt_mmu_entry_get_size(MemBlock[i]);
    BlockAddr = qurt_mmu_entry_get_vaddr(MemBlock[i]);
    BlockCCCC = (unsigned short )QURT_MMU_EXTRACT_CCCC(MemBlock[i]);
    BlockPhyAddr = tms_mmu_entry_get_paddr(MemBlock[i]);
    //Add it for debug
    //TMS_MSG_HIGH_3("PD dump:BlockAddr 0x%x BlockLen 0x%x BlockCCCC 0x%x",BlockAddr, BlockLen,(unsigned long)BlockCCCC);

    temp = (pd_dump_mapped_addr_t *)calloc(sizeof(pd_dump_mapped_addr_t),1);
    if (NULL == temp)
    {
      TMS_MSG_ERROR("memory allocation failed");
      return PD_ELF_STATUS_ERROR;
    }

    temp->mapped_addr_va = (unsigned long)qurt_mem_mmap(-1,  /*Mapping will be created for rootPD*/
                                          -1,                /* We don’t need it from any particular physpool. We will provide PA*/
                                          NULL,              /*Ignore*/
                                          0,                 /*API will allocate VA and return it as part of API return value.*/
                                          BlockLen,          /*Size of the mapping*/
                                          QURT_PROT_CACHE_MODE(BlockCCCC)|QURT_PERM_READ, /* cacheability with permissions. Set cacheability using QURT_PROT_CACHE_MODE*/
                                          QURT_MAP_PHYSADDR, /*This flag tells QuRT to take the PA that you have passed instead of allocating from pool.*/
                                          -1,                /* Ignore */
                                          BlockPhyAddr);     /*PA that you want to map*/

    if((unsigned long)QURT_MAP_FAILED == temp->mapped_addr_va)
    {
      TMS_MSG_ERROR_2("qurt_mem_mmap failed for BlockPhyAddr 0x%x , BlockLen 0x%x",(unsigned long)BlockPhyAddr, BlockLen);
      free(temp);
      return PD_ELF_STATUS_ERROR;
    }

    temp->len = BlockLen;
    temp->next = pd_dump_mapped_addr_list;
    pd_dump_mapped_addr_list = temp;

    if (PD_ELF_STATUS_SUCCESS != pd_dump_wrapper_prgtab_alloc(device_p, (void *)BlockAddr, BlockLen, num_included_blocks, (void *)temp->mapped_addr_va))
    {
      return PD_ELF_STATUS_ERROR;
    }
  }
  return PD_ELF_STATUS_SUCCESS;
}
static PD_ELF_STATUS pd_dump_mem_blocks(struct pd_dump_generic_s* device_p, qurt_mmu_entry_t * MemBlock, unsigned int num_blocks, unsigned int * num_included_blocks)
{
  unsigned int i;
  unsigned long BlockLen, BlockAddr;// Block;
  unsigned long catBlocklen =0, catBlockAddr =0;// Block;
  pd_dump_mapped_addr_list = NULL;

  if( TRUE == device_p->pn.is_untrusted)
  {
    /* Since untrusted/unsigned PDs has their own memory space , root PD cannot directly access it
       so we need to map each block and dump it block by block */
    TMS_MSG_HIGH("PD is untrusted");
    return pd_dump_mem_blocks_untrusted(device_p,MemBlock,num_blocks,num_included_blocks);
  }
  else
  {
    /* Try to filter and coalesce contiguous region */
    /* Logic :
    1) Find out the first block that can be dumped and record it in catBlockAddr and catBlocklen
    1.1) If this is the only block, allocate it in elf structure
    2) Start from the next index until a non contiguous block or a block 
       that cannot be dumped is reached or end of list is reached
    2.1) If the next block is contiguous, adjust lengths to grow catBlocklen over the contiguous blocks
    3) Dump if end of list if reached
    4) If a non contiguous block is found or a block that cannot be dumped is found, 
       Dump the previous block
    5) Find out the next block(starting from current index ) that can be dumped 
       and record it in catBlockAddr and catBlocklen
    5.1) If this is last block, allocate it in elf structure */

    /********************************************************************************************/

    /* Dump the memory blocks as program table LOAD sections */
    /* Find out first block that can be dumped */
    // Walk through each block.  It is either: start of region, extension of region, or not a region
    for (i = 0; i < num_blocks; ++i)
    {
      BlockLen = qurt_mmu_entry_get_size(MemBlock[i]);
      BlockAddr = (unsigned long ) qurt_mmu_entry_get_vaddr(MemBlock[i]);

      //Add it for debug
      //TMS_MSG_HIGH_3("PD dump: MemBlock[i] 0x%x BlockAddr 0x%x BlockLen 0x%x", MemBlock[i], BlockAddr, BlockLen);

      //find a start of region
      catBlockAddr = (unsigned int)BlockAddr;
      catBlocklen  = (unsigned int)BlockLen;

      //concat all extensions of this region
      for (++i; i < num_blocks; ++i)
      {
        BlockLen = qurt_mmu_entry_get_size(MemBlock[i]);
        BlockAddr = (unsigned long) qurt_mmu_entry_get_vaddr(MemBlock[i]);

        // if not a region, break to find next start block
        if ( BlockAddr != (unsigned long)catBlockAddr + catBlocklen)
        {
          --i;  //we pay a penalty of filtering this again when we know it is good, alternative requires goto
          break;
        }
        else
        {
          catBlocklen = BlockLen + catBlocklen;
          continue;
        }
      } //Inner for()

      //catblockAddr is complete, record it
      if (PD_ELF_STATUS_SUCCESS != pd_dump_wrapper_prgtab_alloc(device_p, (void *)catBlockAddr, catBlocklen, num_included_blocks, NULL))
      {
        return PD_ELF_STATUS_ERROR;
      }
    } //Outer for()
    return PD_ELF_STATUS_SUCCESS;
  }//is_untrusted
}

static PD_ELF_STATUS pd_dump_tcb_data_into_unmapped_region(struct pd_dump_generic_s* device_p, struct Thread_s* Thread, unsigned int num_threads)
{
  // for unsigned/unregistered PDs, it is expected that this function should not be called.
  unsigned int i=0;
  void* mbuf_head_p = NULL;
  uint32 mbuf_sz, malloc_sz;
  boolean try_optimization = TRUE;
  unsigned int tcb_index, remaining_tcb_data;
  char *temp = NULL, *temp1 = NULL;
  void **temp_free, **link;

  //we are going to dynamically build our own structures
  // { 
  //    void * next_ptr;
  //    qurt_debug_thread_info data;
  // } one_block;

  // prep for thread info dump
  mbuf_sz = qurt_system_tcb_dump_get_size();

  // ensure we have an area to write the data
  if ((NULL==device_p->unmapped_region_ptr) || (mbuf_sz > device_p->unmapped_region_sz))
  {
    TMS_MSG_ERROR("PD dump no user allocation for qurt_debug_thread_info region");
    return PD_ELF_STATUS_ERROR;
  }

  /* Check for overflow */
  if ( mbuf_sz > UINT32_MAX / num_threads )
  {
    TMS_MSG_ERROR("TCB Data exceeds size limit");
    return PD_ELF_STATUS_ERROR;
  }

  remaining_tcb_data = num_threads * mbuf_sz;

  if ( -1 ==  pd_dump_copy_to_user(device_p->client_handle, device_p->ptcb_dump_sz, &remaining_tcb_data, sizeof(uint32_t)))
  {
    TMS_MSG_ERROR("Failed to update tcb dump size to user PD");
    // Don't return .. Continue
   // for unsigned/unregistered PDs, it is expected that this function should not be called.
  }

  // build linked list of qurt_debug_thread_info structs
  for (i = 0; i < num_threads; )
  {
    tcb_index = i;
    // Get the space
    /* Check if optimization can be done */
    if ( try_optimization == TRUE )
    {
      /* Get Max malloc size */
      malloc_sz = mbuf_sz * PD_DUMP_MAX_BLOCK_NUM;
      malloc_sz = malloc_sz > remaining_tcb_data ? remaining_tcb_data:malloc_sz;

      temp = malloc(malloc_sz + sizeof(void*) + sizeof(unsigned  int));
      if (NULL == temp)
      {
        try_optimization = FALSE;

        /* Try without opimization */
        temp = malloc(mbuf_sz + sizeof(void*) + sizeof(unsigned  int));

        if ( NULL == temp )
        {
          TMS_MSG_ERROR("PD dump could not malloc for qurt_debug_thread_info region");
          return PD_ELF_STATUS_ERROR;
        }

        malloc_sz = mbuf_sz;
        ++i;
        secure_memset(temp, 0, mbuf_sz + sizeof(void*) + sizeof(unsigned  int));
        remaining_tcb_data -= mbuf_sz;
      }
      else /* if (NULL == temp)*/
      {
        if ( malloc_sz == remaining_tcb_data)
          i += remaining_tcb_data/mbuf_sz;
        else
          i += PD_DUMP_MAX_BLOCK_NUM;

         secure_memset(temp, 0, malloc_sz + sizeof(void*) + sizeof(unsigned  int));
         remaining_tcb_data -= malloc_sz;
      }
    }
    else /* if ( try_optimization == TRUE ) */
    {
      /* Try without opimization */
      temp = malloc(mbuf_sz + sizeof(void*) + sizeof(unsigned  int));

      if ( NULL == temp )
      {
        TMS_MSG_ERROR("PD dump could not malloc for qurt_debug_thread_info region");
        return PD_ELF_STATUS_ERROR;
      }
      malloc_sz = mbuf_sz;

      ++i;
      secure_memset(temp, 0, mbuf_sz + sizeof(void*) + sizeof(unsigned  int));
      remaining_tcb_data -= mbuf_sz;
    }

    *(unsigned int*)(temp + sizeof(void*)) = malloc_sz;
    // link the list
    *(void**)temp = mbuf_head_p;

    // fill the data
    temp1 = temp + sizeof(void*) + sizeof(unsigned  int);
    for  (; tcb_index <i && QURT_EOK == qurt_system_tcb_dump_get(Thread[tcb_index].tid, temp1, mbuf_sz); tcb_index++, temp1= temp1 + mbuf_sz );

    if ( tcb_index != i )
    {
      TMS_MSG_ERROR("PD dump could not fill qurt_debug_thread_info region");
      free(temp); // free it separately since it is not part of list yet. 
      /* We should free here memory for list prepared till now */
      for (link = mbuf_head_p; NULL != link; /*inline*/)
      {
        temp_free = link;
        link = (void**)(*link);
        free(temp_free);
      }
      return PD_ELF_STATUS_ERROR;
    }
    mbuf_head_p = temp;
  } // For()

  // store for later consumption
  if (PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_alloc_list(&device_p->elf_hdr, &device_p->elf_prgtab_head, num_threads * mbuf_sz, mbuf_head_p, malloc_sz, device_p->unmapped_region_ptr, device_p->unmapped_region_sz))
  {
#if defined(PD_MON_TRACER_SWEVT)
    tracer_event_simple_vargs(PD_DUMP_SWE_HPE, 2, device_p->hash, device_p->pn.pid);
#endif
    TMS_MSG_ERROR_1("PD dump headers alloc error in thread list %d", device_p->pn.pid);
    /* We should free here memory for complete list */
    for (link = mbuf_head_p; NULL != link; /*inline*/)
    {
      temp_free = link;
      link = (void**)(*link);
      free(temp_free);
    }
    return PD_ELF_STATUS_ERROR;
  }

  return PD_ELF_STATUS_SUCCESS;

} /* pd_dump_tcb_data_into_unmapped_region*/

static PD_ELF_STATUS pd_dump_tcb_data_allocate_note_segment(struct pd_dump_generic_s* device_p, struct Thread_s* Thread, unsigned num_threads, unsigned int * num_note_segment )
{
  unsigned int composite_note_section_size =0;
  unsigned int i=0;
  *num_note_segment =0;

  /* Check if TCB data needs to go into Note Segment, If so just allocate the header now. This elf prgtab allocation should be the first one, 
     it will ensure that the entry is last in the linked list. Tt will facilitate writes in multiple parts */
  if ( device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_NOTE_SECTION || device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_UNMAPPED_REGION_NOTE_SECTION )   
  {
    /* Elf Header format 
       typedef struct Elf32_Nhdr_s
       {
         Elf32_Word n_namesz;
         Elf32_Word n_descsz;
         Elf32_Word n_type;
       } Elf32_Nhdr; */
    composite_note_section_size = num_threads * ( qurt_system_tcb_dump_get_size() + PD_DUMP_NOTE_SECTION_HEADER_SIZE );

    /* Calculate total HVX data and add to Note segment size */
    for ( i=0; i< num_threads; i++ )
    {
       int size;
       if ( 0 != (size =qurt_system_hvx_regs_get_size(Thread[i].tid) ) )
       {
         composite_note_section_size += (size + PD_DUMP_NOTE_SECTION_HEADER_SIZE);
       }
    }

    if (PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_alloc_note_segment(&device_p->elf_hdr, &device_p->elf_prgtab_head, PT_NOTE, composite_note_section_size))
    {
#if defined(PD_MON_TRACER_SWEVT)
      tracer_event_simple_vargs(PD_DUMP_SWE_HPE, 2, device_p->hash, device_p->pn.pid);
#endif
      TMS_MSG_ERROR_1("PD dump headers alloc error in prgtab .process %d", device_p->pn.pid);
      return PD_ELF_STATUS_ERROR;
    }
    *num_note_segment=1;
  }
  return PD_ELF_STATUS_SUCCESS;
} /* pd_dump_tcb_data_allocate_note_segment*/

static PD_ELF_STATUS pd_dump_tcb_data_into_note_sections(int fd, struct pd_dump_generic_s* device_p, struct Thread_s* Thread, unsigned num_threads )
{
  unsigned int i=0, size =0;
  char *temp = NULL;
  unsigned int mbuf_sz = qurt_system_tcb_dump_get_size();
  unsigned int hvx_buf_sz;
  unsigned char * hvx_buf, * hvx_buf_128byte_aligned;

  temp = calloc(1, PD_DUMP_MAX_BUFFER_SIZE);
  if ( temp == NULL )
  {
    TMS_MSG_ERROR("Couldn't allocate buffer for TCB Data");
    return PD_ELF_STATUS_ERROR;
  }

  /* Need 128 bytes aligned buffer, hence adding 128 to calloc size */
  hvx_buf = calloc(1, qurt_system_hvx_regs_get_maxsize() + PD_DUMP_HVX_BUF_ALIGN_IN_BYTES );
  if ( hvx_buf == NULL )
  {
    TMS_MSG_ERROR("Couldn't allocate buffer for HVX Data");
    /* Free temp only on an error */
    free(temp);
    return PD_ELF_STATUS_ERROR;
  }

  /* HVX buffer must be 128 byte aligned.. Also account for additional 4 bytes for version before 128 byte aligned data for HVX */
  hvx_buf_128byte_aligned = hvx_buf + ( PD_DUMP_HVX_BUF_ALIGN_IN_BYTES - 4 - ((unsigned int)hvx_buf%PD_DUMP_HVX_BUF_ALIGN_IN_BYTES));

  // build linked list of qurt_debug_thread_info structs
  for (i = 0; i < num_threads; i++ )
  {    
    if ( PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_populate_note_section_header((temp + size), PD_DUMP_NOTE_NAME_SECTION, NT_HEXAGON_PRSTATUS, mbuf_sz))
    {
      TMS_MSG_ERROR("Couldn't populate note section header for TCB Data");
      /* Free temp only on an error */
      free(temp);
      free(hvx_buf);
      return PD_ELF_STATUS_ERROR;
    }
    size += PD_DUMP_NOTE_SECTION_HEADER_SIZE;
    // fill the data
    if ( QURT_EOK == qurt_system_tcb_dump_get(Thread[i].tid, temp + size, mbuf_sz))
    {
      size += mbuf_sz;
    }

    if ( 0 != (hvx_buf_sz = qurt_system_hvx_regs_get_size(Thread[i].tid) ) )
    {
      qurt_system_hvx_regs_get(Thread[i].tid, hvx_buf_128byte_aligned, hvx_buf_sz);

      if ( size + PD_DUMP_NOTE_SECTION_HEADER_SIZE + hvx_buf_sz >= PD_DUMP_MAX_BUFFER_SIZE )
      {
        if ( PD_ELF_STATUS_SUCCESS != pd_elf_write_composite_note_segment(fd, temp, size))
        {
          TMS_MSG_ERROR("Couldn't write note segment");
          free(temp);
          free(hvx_buf);
          return PD_ELF_STATUS_ERROR;
        }

        temp = calloc(1, PD_DUMP_MAX_BUFFER_SIZE);
        if ( temp == NULL )
        {
          TMS_MSG_ERROR("Couldn't allocate buffer for HVX Data");
          free(hvx_buf);
          return PD_ELF_STATUS_ERROR;
        }
        size =0;
      }

      if ( PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_populate_note_section_header((temp + size), PD_DUMP_NOTE_NAME_SECTION, NT_HEXAGON_PRHVXREG, hvx_buf_sz))
      {
        TMS_MSG_ERROR("Couldn't populate note section header for HVX Data");
        /* Free temp only on an error */
        free(temp);
        free(hvx_buf);
        return PD_ELF_STATUS_ERROR;
      }

      size += PD_DUMP_NOTE_SECTION_HEADER_SIZE;
      memsmove( temp + size, hvx_buf_sz, hvx_buf_128byte_aligned, hvx_buf_sz);
      size += hvx_buf_sz;
    }

    /* Write it out if the buffer is insufficient or have reached the end of TCBs */
    if ( size + mbuf_sz + PD_DUMP_NOTE_SECTION_HEADER_SIZE >= PD_DUMP_MAX_BUFFER_SIZE || i == num_threads -1 )
    {
       if ( PD_ELF_STATUS_SUCCESS != pd_elf_write_composite_note_segment(fd, temp, size))
       {
         TMS_MSG_ERROR("Couldn't write note segment");
         free(temp);
         free(hvx_buf);
         return PD_ELF_STATUS_ERROR;
       }

       /* Allocate new buffer only if TCB data needs to be written */
       if ( i != num_threads -1 )
       {
          temp = calloc(1, PD_DUMP_MAX_BUFFER_SIZE);
          if ( temp == NULL )
          {
            TMS_MSG_ERROR("Couldn't allocate buffer for TCB Data");
            free(hvx_buf);
            return PD_ELF_STATUS_ERROR;
          }
          size =0;
       }
    }
  } // For()

  free(hvx_buf);
  // temp will be freed in pd_elf_write_composite_note_segment() in success cases
  //free(temp);

  return PD_ELF_STATUS_SUCCESS;

} /* pd_dump_tcb_data_into_note_sections*/

static void pd_dump_collect_level_2(struct pd_dump_generic_s* device_p, void *memblock_ptr, int memblock_num, void *tcb_ptr, int tcb_num)
{
   struct Thread_s* Thread = (struct Thread_s*)tcb_ptr;
   qurt_mmu_entry_t * MemBlock =  (unsigned long long *)memblock_ptr;
   unsigned int num_included_blocks = 0, num_note_segment =0, num_note_section =0, num_included_memblocks =0;
   boolean elf_write_success = FALSE;
   int fd_ret;

#if defined(PD_MON_TRACER_SWEVT)
   tracer_event_simple_vargs(PD_DUMP_SWE_EDB, 2, device_p->hash, device_p->pn.pid);
#endif
   TMS_MSG_SPRINTF_HIGH_3("PD dump begin %s %lx %d", device_p->pn.name, device_p->hash, device_p->pn.pid);

   dog_force_kick();

   qurt_pimutex_lock(&pd_dump_internal.mutex); /**< serialize driver access */

   /* Here are the sequence of operations 
   * Open Elf format, If it fails, break do_while{}.
   * Initialize Elf. If it fails, break do_while{}.
   * Update user PD data for total TCB size. 
   * Calculate Size for Composite Note Segment and Allocate it. If it fails, free allocated memories and break do_while{}. 
   * Allocate other remaining note sections ( user programmed, process info, image version ). If it fails, free allocated memories and break do_while{}.
   * Allocate headers for dumping TCB data into unmapped region if required.If it fails, free allocated memories and break do_while{}.
   * Allocate headers for dumping all Memblocks ( after filtering out non dumpables regions).If it fails, free allocated memories and break do_while{}.
   * Prepare Prgtab Header to have one single write ( Optimization ). If it fails, free allocated memories and break do_while{}.
   * Allocate section Tab. If it fails, free allocated memories and break do_while{}.
   * Program offsets in the ELF header, Prgtab header and Sectab Header.If it fails, free allocated memories and break do_while{}.
   * Write All headers, memblocks and other note sections ( user programmed, process info, image version ). If it fails, free allocated memories and break do_while{}.
     If it is pass then free Up headers and other data structures.
   * Write TCB ( and HVX Data if available) data as note sections ( club multiple notes with max size PD_DUMP_MAX_BUFFER_SIZE).
     If it fails, free allocated memories and break do_while{}. This may have several writes since only 7K are allocated.
     If it is pass then free buffers.
   * Write Sectab If it fails, free allocated memories and break do_while{}.
   * if it is pass ,free up sectab.
   */

   do
   {
      if (sizeof(device_p->filename) <= tms_utils_fmt(device_p->filename, sizeof(device_p->filename), PD_DUMP_FILE_COUNT_TEMPLATE, pd_mon_internal_prefix, device_p->pn.name))
      {
#if defined(PD_MON_TRACER_SWEVT)
         tracer_event_simple_vargs(PD_DUMP_SWE_RC, 2, device_p->hash, device_p->pn.pid);
#endif
         TMS_MSG_ERROR_1("PD dump count read filename too long %d", device_p->pn.pid);

         break; /**< do while () */
      }

      fd_ret = pd_dump_fs_al_open_file_for_read(device_p->filename, &device_p->fd); /**< open .count file (read) */
      TMS_MSG_HIGH_2("Open .count file: %d %d", device_p->fd, fd_ret);

      if (0 > fd_ret || sizeof(device_p->counter) != pd_dump_fs_al_read(device_p->fd, (char*)&device_p->counter, sizeof(device_p->counter)))
      {
         device_p->counter = 0;

#if defined(PD_MON_TRACER_SWEVT)
         tracer_event_simple_vargs(PD_DUMP_SWE_RC, 2, device_p->hash, device_p->pn.pid);
#endif
         TMS_MSG_ERROR_1("PD dump count read file not available %d", device_p->pn.pid);
      }
      else
      {
#if defined(PD_MON_TRACER_SWEVT)
         tracer_event_simple_vargs(PD_DUMP_SWE_RC, 2, device_p->hash, device_p->pn.pid);
#endif
         TMS_MSG_ERROR_2("PD dump count read %d %d", device_p->counter, device_p->pn.pid);
      }

      if (0 <= fd_ret)
         pd_dump_fs_al_close(device_p->fd); /**< close .count file (read) */

      /**< BEGIN ALLOCATIONS : BE SENSITIVE NOT TO LEAK MEMORY */

      do
      {
         /* Delete the existing old PD dump file. Need to keep only recent MAX_PD_DUMP_FILES pd dump files */
         if (MAX_PD_DUMP_FILES <= device_p->counter)
         {
            if (sizeof(device_p->filename) <= tms_utils_fmt(device_p->filename, sizeof(device_p->filename), PD_DUMP_FILE_ELF_TEMPLATE, pd_mon_internal_prefix, device_p->pn.name, device_p->counter-MAX_PD_DUMP_FILES))
            {
               TMS_MSG_ERROR_1("PD dump elf filename too long %d", device_p->pn.pid);
               break; /**< do while () */
            }

            fd_ret = pd_dump_fs_al_delete(device_p->filename); /**< delete .elf file*/
            TMS_MSG_HIGH_1("delete .elf file: %d", fd_ret);
         }

         device_p->elf_prgtab_head = NULL; /**< prepare ELF headers */
         device_p->elf_sectab_head = NULL; /**< prepare ELF headers */
         device_p->prgtab_phdr     = NULL; /**< prepare ELF headers */

         if (sizeof(device_p->filename) <= tms_utils_fmt(device_p->filename, sizeof(device_p->filename), PD_DUMP_FILE_ELF_TEMPLATE, pd_mon_internal_prefix, device_p->pn.name, device_p->counter))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_OE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump elf filename too long %d", device_p->pn.pid);
            break; /**< do while () */
         }

         fd_ret = pd_dump_fs_al_open_file_for_create_write(device_p->filename, &device_p->fd); /**< open .elf file (write) */
         TMS_MSG_HIGH_2("Open .elf file: %d %d", device_p->fd, fd_ret);

         if (0 > fd_ret)
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_OE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump elf file not available %d", device_p->pn.pid);
            break; /**< do while () */
         }

         if (PD_ELF_STATUS_SUCCESS != pd_elf_init(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->elf_sectab_head))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HIE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump headers init error %d", device_p->pn.pid);
            break; /**< do while () */
         }

         if (PD_ELF_STATUS_SUCCESS != pd_dump_tcb_data_allocate_note_segment(device_p, Thread, tcb_num, &num_note_segment))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HIE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump note segment failure %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            break; /**< do while () */
         }

         //Allocate program table Note section
         if (PD_ELF_STATUS_SUCCESS != pd_dump_dequeue_write_note_section(device_p, FALSE, &num_note_section) ) 
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HIE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump note section failure %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            break; /**< do while () */
         }

         num_note_section += num_note_segment;

         if ( device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_UNMAPPED_REGION || device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_UNMAPPED_REGION_NOTE_SECTION )  
         {
           /* num_included_blocks is incremented by 1 if TCB is dumped in unmapped region */
           if (PD_ELF_STATUS_SUCCESS != pd_dump_tcb_data_into_unmapped_region(device_p, Thread, tcb_num) )
           {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HIE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump TCB dump into unmapped region failed %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            break; /**< do while () */
           }
           /* num_included_blocks is incremented by 1 if TCB is dumped in unmapped region */
           ++num_included_blocks;
         }

         //Allocate LOADABLE memory blocks
         if (PD_ELF_STATUS_SUCCESS != pd_dump_mem_blocks(device_p, MemBlock, memblock_num, &num_included_memblocks))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HIE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump LOADABLE memory blocks failure %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            pd_dump_free_mappings();
            break; /**< do while () */
         }

         num_included_blocks += num_included_memblocks;

         if (PD_ELF_STATUS_SUCCESS != pd_elf_prgtab_prepare_header(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr, num_included_blocks + num_note_section ))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HSE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump headers alloc error in prepare header %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            pd_dump_free_mappings();
            break; /**< do while () */
         }

         // required NULL section table entry, ELF specification

         if (PD_ELF_STATUS_SUCCESS != pd_elf_sectab_alloc(&device_p->elf_hdr, &device_p->elf_sectab_head, SHT_NULL, SHN_UNDEF))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HSE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump headers alloc error in sectab %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            pd_dump_free_mappings();
            break; /**< do while () */
         }

         if (PD_ELF_STATUS_SUCCESS != pd_elf_prepare_offsets(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->elf_sectab_head))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_HOE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump headers offsets error %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            pd_elf_term_sectab(&device_p->elf_sectab_head);
            pd_dump_free_mappings();
            break; /**< do while () */
         }

         if (PD_ELF_STATUS_SUCCESS != pd_elf_write_header_prgtab(device_p->fd, &device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump elf write header prgtab failed %d", device_p->pn.pid);
            pd_elf_term_header_prgtab(&device_p->elf_hdr, &device_p->elf_prgtab_head, &device_p->prgtab_phdr);
            pd_elf_term_sectab(&device_p->elf_sectab_head);
            pd_dump_free_mappings();
            break; /**< do while () */
         }
         pd_dump_free_mappings();

         /* Check again, if TCB data needs to go into Note section, We collect TCB data and write now*/
         if ( device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_NOTE_SECTION || device_p->tcb_data_dump_method == PDDUMP_TCB_DATA_UNMAPPED_REGION_NOTE_SECTION )
         {
           if ( PD_ELF_STATUS_SUCCESS != pd_dump_tcb_data_into_note_sections(device_p->fd, device_p, Thread, tcb_num) )
           {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump TCB Dump note segment not written %d", device_p->pn.pid);
            pd_elf_term_sectab(&device_p->elf_sectab_head);
            break; /**< do while () */
           }
         }

         if (PD_ELF_STATUS_SUCCESS != pd_elf_write_sectab(device_p->fd, &device_p->elf_sectab_head))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WE, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump write sectab failed %d", device_p->pn.pid);
            pd_elf_term_sectab(&device_p->elf_sectab_head);
            break; /**< do while () */
         }

         elf_write_success = TRUE;

      } while (0); /**< memory potentially still allocated */

      TMS_MSG_HIGH_1("Write .elf file: %d completed", device_p->fd);

      if (0 <= fd_ret)
         pd_dump_fs_al_close(device_p->fd); /**< close .elf file (write) */

      if ( FALSE == elf_write_success )
      {
         fd_ret = pd_dump_fs_al_open_file_for_create_write(device_p->filename, &device_p->fd); /**< open .elf file (write) */

         if (0 > fd_ret || sizeof(device_p->counter) != pd_dump_fs_al_write(device_p->fd, (const byte*)&device_p->counter, sizeof(device_p->counter)))
         {
           TMS_MSG_ERROR_2("PD dump elf file truncated, no valid content %d %d ", device_p->pn.pid, device_p->fd);
         }
         else
         {
           TMS_MSG_ERROR_2("PD dump elf file truncated, invalid content %d %d ", device_p->pn.pid, device_p->fd);
         }

         if (0 <= fd_ret)
           pd_dump_fs_al_close(device_p->fd); /**< close .elf file (write) */
      }

      /**< END ALLOCATIONS : BE SENSITIVE NOT TO LEAK MEMORY */

      if(TRUE == elf_write_success)
      {
         device_p->counter++;

         if (sizeof(device_p->filename) <= tms_utils_fmt(device_p->filename, sizeof(device_p->filename), PD_DUMP_FILE_COUNT_TEMPLATE, pd_mon_internal_prefix, device_p->pn.name))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WC, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_1("PD dump count write filename too long %d", device_p->pn.pid);
            break; /**< do while () */
         }

         fd_ret = pd_dump_fs_al_open_file_for_create_write(device_p->filename, &device_p->fd); /**< open .count file (write) */
         TMS_MSG_HIGH_2("Open .count file: %d %d again", device_p->fd, fd_ret);

         if (0 > fd_ret || sizeof(device_p->counter) != pd_dump_fs_al_write(device_p->fd, (const byte*)&device_p->counter, sizeof(device_p->counter)))
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WC, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_2("PD dump count write file not available %d %d", device_p->pn.pid,device_p->fd);
         }
         else
         {
#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_WC, 2, device_p->hash, device_p->pn.pid);
#endif
            TMS_MSG_ERROR_2("PD dump count write %d %d", device_p->counter, device_p->pn.pid);
         }

         if (0 <= fd_ret)
            pd_dump_fs_al_close(device_p->fd); /**< close .count file (write) */
      }
   } while (0);

   qurt_pimutex_unlock(&pd_dump_internal.mutex); /**< serialize driver access */

#if defined(PD_MON_TRACER_SWEVT)
   tracer_event_simple_vargs(PD_DUMP_SWE_EDE, 2, device_p->hash, device_p->pn.pid);
#endif
   TMS_MSG_HIGH("PD dump process exit");
} /* pd_dump_collect_level_2 */

/**
API, Initialization of service prior to use
@return
None.
*/
void pd_dump_host_init(void)
{
   qurt_pimutex_init(&pd_dump_internal.mutex); /**< serialize driver access */

   pd_mon_pd_dump_config(&pd_dump_internal.dump_enabled);
}

/** pd_dump_cb_enable
 * This will allow clients to register for callback to be invoke in path
 * of PD dump handling post-crash.
 *
 * @param cb_info pd_dump_cb_info_t structure having required info.
 *
 * @return PDDUMP_E_SUCCESS in case of successful registration else
 *         pd_dump_cb_error_t error codes.
 */
pd_dump_cb_error_t pd_dump_cb_enable(pd_dump_cb_info_t * cb_info)
{
  /* input validation */
  if(cb_info->cb_fn == NULL)
    return PDDUMP_E_INVALID_PARAM;

  /* register callback */
  if(pd_dump_internal.pd_dump_cb.cb_fn == NULL)
  {
    pd_dump_internal.pd_dump_cb.cb_fn = cb_info->cb_fn;
    pd_dump_internal.pd_dump_cb.user_data = cb_info->user_data;
  }
  else
    return PDDUMP_E_NOT_ALLOWED;
  
  return PDDUMP_E_SUCCESS;
}

/** pd_dump_cb_disable
 * This will allow clients to de-register callback, which was register via 
 * pd_dump_cb_enable.
 *
 * @param cb_info pd_dump_cb_info_t structure having required info.
 *
 * @return PDDUMP_E_SUCCESS in case of successful de-registration else
 *         pd_dump_cb_error_t error codes.
 */
pd_dump_cb_error_t pd_dump_cb_disable(pd_dump_cb_info_t * cb_info)
{
  /* input validation */
  if(cb_info->cb_fn == NULL)
    return PDDUMP_E_INVALID_PARAM;

  /* de-register callback */
  if(pd_dump_internal.pd_dump_cb.cb_fn == cb_info->cb_fn)
  {
    pd_dump_internal.pd_dump_cb.cb_fn = NULL;
    pd_dump_internal.pd_dump_cb.user_data = 0;
  }
  else
    return PDDUMP_E_NOT_ALLOWED;

  return PDDUMP_E_SUCCESS;
}

int pd_dump_open(struct pd_dump_device_s** device_p, pd_dump_process_info_t *pn, void *unRePtr , int unReSz, void *ptcbDumpSz, int tcbDataDumpMethod, boolean is_registered)
{
   int len1 = 0,len2 = 0;
   int rc = PD_DUMP_STATUS_SUCCESS;

   qurt_pimutex_lock(&pd_dump_internal.mutex); /**< serialize driver access */

   len1 = strnlen(pn->name,PROCESS_MAX_NAME_LEN);

   do
   {
      for (*device_p = pd_dump_internal.head; NULL != *device_p; *device_p = (*device_p)->next)
      {
         len2 = strnlen((*device_p)->generic_data.pn.name,PROCESS_MAX_NAME_LEN);
         if (len1 == len2 && 0 == tms_utils_chr_compare((*device_p)->generic_data.pn.name, len2, pn->name, len1))
         {
            break; /**< for () */
         }
      }

      /** operate on new */

      if (NULL == *device_p)
      {
         /** allocate new */

         if (NULL == (*device_p = (struct pd_dump_device_s*)calloc(sizeof(struct pd_dump_device_s),1)))
         {
            break; /**< do while () */
         }

         if(TRUE == is_registered)
            pd_dump_interface_data(*device_p);

         /** device object init */

         (*device_p)->generic_data.unmapped_region_ptr = unRePtr; //validate when consumed
         (*device_p)->generic_data.unmapped_region_sz = unReSz;  //validate when consumed
         (*device_p)->generic_data.ptcb_dump_sz = ptcbDumpSz;  //validate when consumed
         if ( tcbDataDumpMethod == 0 )
            (*device_p)->generic_data.tcb_data_dump_method = PDDUMP_TCB_DATA_UNMAPPED_REGION; 
         else if ( tcbDataDumpMethod == 1 )
            (*device_p)->generic_data.tcb_data_dump_method = PDDUMP_TCB_DATA_NOTE_SECTION;  
         else if ( tcbDataDumpMethod == 2 )
            (*device_p)->generic_data.tcb_data_dump_method = PDDUMP_TCB_DATA_UNMAPPED_REGION_NOTE_SECTION;  

         (*device_p)->generic_data.pn.pid = pn->pid;
         strlcpy((*device_p)->generic_data.pn.name,pn->name,PROCESS_MAX_NAME_LEN);
         (*device_p)->generic_data.pn.is_untrusted = pn->is_untrusted;

         (*device_p)->generic_data.hash = tms_utils_hash((*device_p)->generic_data.pn.name, sizeof((*device_p)->generic_data.pn.name));

         /** link into device list */
         (*device_p)->next = pd_dump_internal.head;
         pd_dump_internal.head = *device_p;

         q_init( &((*device_p)->generic_data.q_of_pd_dump_queue_note_section_s));

         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, (char *)(*device_p)->generic_data.pn.name, sizeof((*device_p)->generic_data.pn.name), NT_HEXAGON_DUMP_PROCESS);
         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, (char *)&(*device_p)->generic_data.pn.pid, sizeof((*device_p)->generic_data.pn.pid), NT_HEXAGON_DUMP_PID);

         #if defined(ADD_ROOT_PD_IMAGE_VER)
         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, QC_IMAGE_VERSION_STRING_AUTO_UPDATED, strnlen(QC_IMAGE_VERSION_STRING_AUTO_UPDATED,PD_DUMP_IMAGE_VERSION_SIZE), NT_HEXAGON_DUMP_VERSION); 
         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, IMAGE_VARIANT_STRING_AUTO_UPDATED,  strnlen(IMAGE_VARIANT_STRING_AUTO_UPDATED,PD_DUMP_IMAGE_VERSION_SIZE), NT_HEXAGON_DUMP_VERSION); 
         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, OEM_IMAGE_VERSION_STRING_AUTO_UPDATED, strnlen(OEM_IMAGE_VERSION_STRING_AUTO_UPDATED,PD_DUMP_IMAGE_VERSION_SIZE), NT_HEXAGON_DUMP_VERSION); 
         pd_dump_enqueue_note_section(&(*device_p)->generic_data, PD_DUMP_NOTE_NAME_SECTION, OEM_IMAGE_UUID_STRING_AUTO_UPDATED, strnlen(OEM_IMAGE_UUID_STRING_AUTO_UPDATED,PD_DUMP_IMAGE_VERSION_SIZE), NT_HEXAGON_DUMP_VERSION); 
         #endif
      }

      else
      {
         /** operate on existing */
      }

   } while (0);

   qurt_pimutex_unlock(&pd_dump_internal.mutex); /**< serialize driver access */

   if (NULL == *device_p)
   {
      rc = PD_DUMP_STATUS_ERROR; /**< failure */

#if defined(PD_MON_TRACER_SWEVT)
      tracer_event_simple_vargs(PD_DUMP_SWE_I, 2, 0, pn->pid);
#endif
      TMS_MSG_ERROR_2("Interface open NULL NA %d, %d", pn->pid, rc);
   }
   return rc;
}

int pd_dump_collect_level_1(struct pd_dump_generic_s* device_p, void *memblock_ptr, int memblock_num, void *tcb_ptr, int tcb_num)
{
   uint64 start_tick, end_tick;

   tms_get_timetick(&start_tick);
   pd_dump_collect_level_2(device_p, memblock_ptr, memblock_num, tcb_ptr, tcb_num);
   tms_get_timetick(&end_tick);
   TMS_MSG_HIGH_1_64("Time(ms) spent in PD Dumps %llu", TMS_TIME_DIFF_IN_MSEC(end_tick, start_tick));

   return PD_DUMP_STATUS_SUCCESS; /**< this code path always reports success; check F3 logs for information */
}

int pd_dump_ext_cb(int client_handle, void *tcb_ptr, int tcb_num)
{
   // Call for external callback of pd dump
   if(NULL != pd_dump_internal.pd_dump_cb.cb_fn)
   {
     TMS_MSG_HIGH("PD dump external handler called.");
     pd_dump_internal.pd_dump_cb.cb_fn(client_handle, (uint32)(tcb_ptr), (uint32)(tcb_num), pd_dump_internal.pd_dump_cb.user_data);
     TMS_MSG_HIGH("PD dump external handler returned.");
   }
   else
     TMS_MSG_HIGH("No PD dump external handler.");

   return PD_DUMP_STATUS_SUCCESS; /**< this code path always reports success; check F3 logs for information */
}

void pd_dump_device_close(struct pd_dump_device_s* device_p_del)
{
   struct pd_dump_device_s* device_p_curr = NULL;
   struct pd_dump_device_s* device_p_prev = NULL;
   int len1 = 0,len2 = 0;

   if(NULL == device_p_del)
   {
     TMS_MSG_HIGH("NULL in PD dump release");
     return;
   }

   qurt_pimutex_lock(&pd_dump_internal.mutex); /**< serialize driver access */

   len2 = strnlen(device_p_del->generic_data.pn.name,PROCESS_MAX_NAME_LEN);

   for (device_p_curr = pd_dump_internal.head; NULL != device_p_curr; device_p_curr = device_p_curr->next)
   {
      len1 = strnlen(device_p_curr->generic_data.pn.name,PROCESS_MAX_NAME_LEN);
      if (len1 == len2 && 0 == tms_utils_chr_compare(device_p_curr->generic_data.pn.name, len1,
                                                        device_p_del->generic_data.pn.name, len2))
      {
         if (NULL == device_p_prev) /**< head check */
         {
            if ( pd_dump_internal.head->next == NULL)
            {
              pd_dump_internal.head = NULL;
            }
            else
            {
              pd_dump_internal.head = device_p_curr->next;
              device_p_curr->next = NULL;
            }
         }
         else
         {
            device_p_prev->next = device_p_curr->next;
            device_p_curr->next = NULL;
         }

#if defined(PD_MON_TRACER_SWEVT)
         tracer_event_simple_vargs(PD_DUMP_SWE_R, 2, device_p_curr->generic_data.hash, device_p_curr->generic_data.pn.pid);
#endif
         TMS_MSG_HIGH_2("PD dump release %lx %d", device_p_curr->generic_data.hash, device_p_curr->generic_data.pn.pid);

         /* Clean up the note_section queue if any data is still there */
         pd_dump_dequeue_write_note_section(&device_p_curr->generic_data, TRUE, NULL);

         q_destroy(&(device_p_curr->generic_data.q_of_pd_dump_queue_note_section_s));

         free(device_p_curr);

         break;
      }

      device_p_prev = device_p_curr;
   }

   qurt_pimutex_unlock(&pd_dump_internal.mutex); /**< serialize driver access */

   return ;
}

int pd_dump_add_note_sec(struct pd_dump_generic_s* device_p, void *note_name , 
                             void *note_desc, int descsz, int note_type)
{
   /*for unsigned/unregistered PDs, No use of this API. we can directly use pd_dump_enqueue_note_section()
      since we need to fetch required info from process attrs or some API directly, if available*/
   char name[PD_DUMP_NOTE_SECTION_NAMESZ];
   char *desc;
   int result;
   int rc = PD_DUMP_STATUS_SUCCESS;

   /* Name cannot be NULL or '\0' according to spec */
   if  (note_name == NULL ) 
   {
     rc = PD_DUMP_STATUS_ERROR;
     return rc;
   }

   result = pd_dump_copy_from_user(device_p->client_handle, name, note_name, PD_DUMP_NOTE_SECTION_NAMESZ );
   if (result || name[0] == '\0' )
   {
     rc = PD_DUMP_STATUS_ERROR;
     return rc;
   }

   /* Desc can be NULL according to spec */
   if ( note_desc == NULL )
   {
      rc = pd_dump_enqueue_note_section( device_p, name, NULL, PD_DUMP_NOTE_SECTION_DESCSZ, note_type );
   }
   else
   {
      desc = calloc(1, PD_DUMP_NOTE_SECTION_DESCSZ );
      if ( desc == NULL )
         return PD_DUMP_STATUS_ERROR;
      result = pd_dump_copy_from_user(device_p->client_handle, desc, note_desc, PD_DUMP_NOTE_SECTION_DESCSZ );
      if (result)
      {
        free(desc);
        rc = PD_DUMP_STATUS_ERROR;
        return rc;
      }
   
      /* descsz is range checked in the enqueue function */
      rc = pd_dump_enqueue_note_section( device_p, name, desc, descsz, note_type ); 

      free(desc);
   }

   return rc;
}

struct pd_dump_device_s* pd_dump_get_device_p(pd_dump_process_info_t *pn)
{
   struct pd_dump_device_s* device_p = NULL ;
   int len1 = 0,len2 = 0;

   qurt_pimutex_lock(&pd_dump_internal.mutex);

   len1 = strnlen(pn->name,PROCESS_MAX_NAME_LEN);
   device_p = pd_dump_internal.head ;

   while (NULL != device_p)
   {
     len2 = strnlen(device_p->generic_data.pn.name,PROCESS_MAX_NAME_LEN);
     if (len1 == len2 && 0 == tms_utils_chr_compare(device_p->generic_data.pn.name, len2, pn->name, len1))
     {
       qurt_pimutex_unlock(&pd_dump_internal.mutex);
       return device_p;
     }
     device_p = device_p->next;
   }
   qurt_pimutex_unlock(&pd_dump_internal.mutex);
   return device_p;
}