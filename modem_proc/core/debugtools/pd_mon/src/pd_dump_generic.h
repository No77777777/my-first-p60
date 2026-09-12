/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_generic.h
@brief This file contains common data of pd dumps across interfaces.
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2019,2022 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_generic.h#2 $
$DateTime: 2022/06/22 22:40:36 $
$Change: 38006310 $
$Author: pwbldsvc $
===============================================================================*/
#include "qurt.h"
#include "qurt_printf.h"
#include "qurt_thread_context.h"
#include "qurt_restricted.h"
#include "queue.h"
#include "pd_dump_fs_al.h"

#include "pd_dump.h"
#include "pd_elf.h"
#include "pd_elf_hexagon.h"
#include "pd_mon_qurt.h"
#include "pd_mon_dump.h"


#define PD_DUMP_FILE_COUNT_TEMPLATE     "%s%s.count"
#define PD_DUMP_FILE_ELF_TEMPLATE       "%s%s.%02d.elf"
#define PD_DUMP_FILE_NAME_MAX           128
#define PD_DUMP_MAX_BLOCK_NUM           7
#define PD_DUMP_MAX_BUFFER_SIZE         7168
#define PD_DUMP_NOTE_NAME_SECTION       "QC_Hexagon"
#define PD_DUMP_NOTE_SECTION_HEADER_SIZE ( sizeof(Elf32_Nhdr)  + strlen(PD_DUMP_NOTE_NAME_SECTION) + sizeof(Elf32_Word) - strlen(PD_DUMP_NOTE_NAME_SECTION) % sizeof(Elf32_Word) )

#define PD_DUMP_HVX_BUF_ALIGN_IN_BYTES   128

#define PROCESS_MAX_NAME_LEN QURT_MAX_NAME_LEN

#define MAX_PD_DUMP_FILES 2

typedef struct pd_dump_process_info_s
{
  unsigned int pid;
  char name[QURT_MAX_NAME_LEN]; /*process name*/
  boolean is_untrusted;
}pd_dump_process_info_t;

struct pd_dump_generic_s /**< localized driver storage area */
{
   pd_dump_process_info_t pn; /**< client procname on open */

   uint32_t hash; /**< name hash */

   Elf32_Ehdr elf_hdr; /**< ELF header */

   pd_elf_prgtab_p elf_prgtab_head; /**< linked list builds prgtab */

   void * prgtab_phdr;

   pd_elf_sectab_p elf_sectab_head; /**< linked list builds sectab */

   unsigned int counter; /**< read elf file counter */

   char filename[PD_DUMP_FILE_NAME_MAX]; /**< temporary filename construction buffer */

   int fd; /**< file descriptor fs */

   void * unmapped_region_ptr; /**< virtual buffer addr for non-process data (tcb's) */

   uint32_t unmapped_region_sz; /**< virtual buffer size for non-process data (tcb's) */

   uint32_t * ptcb_dump_sz; /** Points to user PD memory, Populated during dump collection, TCB Dump size */

   pd_dump_tcb_data_dump_method_t  tcb_data_dump_method; /* Determines the TCB data Dumping method */

   q_type q_of_pd_dump_queue_note_section_s;

   int client_handle;
};

