/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_qurt_cb.c
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
Copyright (c) 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_qurt_cb.c#1 $
$DateTime: 2019/11/07 04:24:43 $
$Change: 21281462 $
$Author: pwbldsvc $
===============================================================================*/

#include "stdlib.h"
#include "stringl/stringl.h"
#include "pd_dump_interface_qdi.h"
#include "qurt.h"
#include "qurt_thread_context.h"
#include "qurt_restricted.h"
#include "qurt_process.h"
#include "queue.h"
#include "err.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"

#include "pd_dump.h"
#include "pd_mon_qurt.h"
#include "pd_mon_dump.h"

extern int pd_dump_open(struct pd_dump_device_s** device_pp, pd_dump_process_info_t *pn, void *unRePtr , int unReSz, void *ptcbDumpSz, int tcbDataDumpMethod, boolean is_registered);
extern int pd_dump_collect_level_1(struct pd_dump_generic_s* device_p, void *memblock_ptr, int memblock_num, void *tcb_ptr, int tcb_num);
extern void pd_dump_device_close(struct pd_dump_device_s* device_p_del);
extern struct pd_dump_device_s* pd_dump_get_device_p(pd_dump_process_info_t *pn);
extern int pd_dump_enqueue_note_section(struct pd_dump_generic_s* device_p, char *name, char *desc, uint32 descsz, uint32 type);
extern int pd_dump_ext_cb(int client_handle, void *tcb_ptr, int tcb_num);
extern boolean is_pd_dump_enabled(void);
/////////////////////////////////////////////////////////////////////
// Localized Type Declarations
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Implementation Details
/////////////////////////////////////////////////////////////////////

void pd_dump_callback(unsigned int pid)
{
   qurt_process_attr_t attr;
   struct pd_dump_device_s* device_p = NULL;
   boolean is_registered;
   pd_dump_process_info_t pinfo;
   int rc;
   int memblock_num=0;
   unsigned int *memblock_ptr = NULL;
   int tcb_num=0;
   unsigned int *tcb_ptr = NULL;
   unsigned int rtld_debug_addr;

   TMS_MSG_HIGH_1("PID %d",pid);

   qurt_process_attr_get(pid,&attr);

   pinfo.pid = pid;
   strlcpy(pinfo.name,attr.name,PROCESS_MAX_NAME_LEN);

   if ( attr.flags & QURT_PROCESS_UNTRUSTED )
   {
     pinfo.is_untrusted = TRUE;
   }
   else
   {
     pinfo.is_untrusted = FALSE;
   }

   TMS_MSG_SPRINTF_HIGH_3("Process_info %s, %d , %d", pinfo.name,pinfo.pid,pinfo.is_untrusted );

   device_p = pd_dump_get_device_p(&pinfo);
   if(NULL == device_p)
   {
      TMS_MSG_HIGH("PD is not registered with TMS");
      is_registered = FALSE;
   }
   else
   {
     TMS_MSG_ERROR_1(" PD is registered with TMS 0x%x", device_p);
     is_registered = TRUE;
   }

   /* Collect TCBs */
   tcb_num = qurt_process_get_thread_count(pid);
   if (0 == tcb_num)
   {
      TMS_MSG_ERROR_1("Qurt passed 0 tcb for %d ", pid);
      goto clear ;
   }

   TMS_MSG_HIGH_2("Qurt passed %d tcb for %d ",tcb_num, pid);

   tcb_ptr = (unsigned int *)calloc(sizeof(unsigned int),tcb_num); //  4 bytes for address
   if (tcb_ptr == NULL)
   {
      TMS_MSG_ERROR_1("memory allocation failed for, %d ",pid);
      goto clear ;
   }

   (void)qurt_process_get_thread_ids(pid, tcb_ptr, tcb_num);

   if (TRUE == is_pd_dump_enabled())
   {
      if( FALSE == is_registered )
      {
         /* allocate a temp device_p and free that after completion */
         rc = pd_dump_open(&device_p, &pinfo, NULL, 0, NULL, PDDUMP_TCB_DATA_NOTE_SECTION, is_registered);
         if(PD_DUMP_STATUS_SUCCESS == rc)
         {
            TMS_MSG_HIGH_3("PD Dump open %x: %d: %d", device_p->generic_data.hash, pinfo.pid, rc);
         }
         else
         {
           TMS_MSG_ERROR_2("PD Dump open failed for %d %d",pinfo.pid,rc);
           goto clear ;
         }
      }

      /* Collect Memory blocks */
      memblock_num = qurt_process_dump_get_mem_mappings_count(pid);
      if (0 == memblock_num)
      {
         TMS_MSG_ERROR_2("Qurt passed 0 blocks for 0x%x , %d ",device_p, pid);
         goto ext_cb ;
      }

      TMS_MSG_HIGH_3("Qurt passed %d blocks for 0x%x , %d ",memblock_num, device_p, pid);

      /* QuRT passes 64 bit TLB entry for each mapping */
      memblock_ptr = (unsigned int *)calloc(sizeof(qurt_mmu_entry_t),memblock_num); 
      if (memblock_ptr == NULL)
      {
         TMS_MSG_ERROR_2("memory allocation failed for 0x%x , %d ",device_p, pid);
         goto ext_cb ;
      }

      memblock_num = qurt_process_dump_get_mappings(pid, memblock_ptr, memblock_num);
      TMS_MSG_HIGH_3("Qurt Dumped %d blocks for 0x%x , %d ",memblock_num, device_p, pid);

      (void)qurt_process_get_rtld_debug(pid, &rtld_debug_addr);
      pd_dump_enqueue_note_section(&device_p->generic_data,"QC_Hexagon",(char *)&rtld_debug_addr, sizeof(unsigned int), NT_HEXAGON_RTLD_LINKMAP);

      (void)pd_dump_collect_level_1(&device_p->generic_data, memblock_ptr, memblock_num, tcb_ptr, tcb_num);
   }
   else
   {
      TMS_MSG_HIGH("PD dumps are disabled");
   }

ext_cb:
   if( TRUE == is_registered && NULL != device_p )
   {
     (void)pd_dump_ext_cb(device_p->generic_data.client_handle, tcb_ptr, tcb_num);
   }

clear:
   if (NULL != memblock_ptr)
      free(memblock_ptr);
   if (NULL != tcb_ptr)
      free(tcb_ptr);
   if (FALSE == is_registered && NULL != device_p )
   {
      pd_dump_device_close(device_p);
   }
   return ;
}

/**
@return
None.
*/
void pd_dump_register_qurt_cb(void)
{
   qurt_cb_data_t cb_data;
   cb_data.cb_func = &pd_dump_callback;
   cb_data.cb_arg = 0;
   (void)qurt_process_dump_register_cb(&cb_data,QURT_PROCESS_DUMP_CB_ROOT,0); /*0-255 priority. Lower number, higher priority*/
}