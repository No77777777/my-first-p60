/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_qdi.c
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
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_qdi.c#1 $
$DateTime: 2019/11/07 04:24:43 $
$Change: 21281462 $
$Author: pwbldsvc $
===============================================================================*/

#include "stdlib.h"
#include "stringl/stringl.h"
#include "pd_dump_interface_qdi.h"
#include "qurt.h"
#include "qurt_printf.h"
#include "qurt_qdi_driver.h"
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

extern int pd_dump_open(struct pd_dump_device_s** device_pp, pd_dump_process_info_t *pn, void *unRePtr , int unReSz, void *ptcbDumpSz, int tcbDataDumpMethod, boolean is_registered);
extern void pd_dump_device_close(struct pd_dump_device_s* device_p_del);
extern int pd_dump_add_note_sec(struct pd_dump_generic_s* device_p, void *note_name, void *note_desc, int descsz, int note_type);
extern void pd_dump_register_qurt_cb(void);
/////////////////////////////////////////////////////////////////////
// Localized Type Declarations
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Implementation Details
/////////////////////////////////////////////////////////////////////

static void pd_dump_device_release(qurt_qdi_obj_t* qdiobj)
{
   struct pd_dump_device_s* device_p_del = (struct pd_dump_device_s*)qdiobj;

   pd_dump_device_close(device_p_del);

}

static int pd_dump_device_invocation(int client_handle, qurt_qdi_obj_t* obj, int method, qurt_qdi_arg_t a1, qurt_qdi_arg_t a2, qurt_qdi_arg_t a3, qurt_qdi_arg_t a4, qurt_qdi_arg_t a5, qurt_qdi_arg_t a6, qurt_qdi_arg_t a7, qurt_qdi_arg_t a8, qurt_qdi_arg_t a9)
{
   int rc;
   struct pd_dump_device_s* device_p = (struct pd_dump_device_s*)obj;
   qurt_sysenv_procname_t pn;

   /* client_handle means that we're asking this particular client handle to answer the question about its process */
   /* QDI_OS_SYSENV is the standard method for requesting this type of information */
   /* QDI_HANDLE_LOCAL_CLIENT means that this request originated locally */
   /* QURT_SYSENV_PROCNAME_TYPE is the specific piece of data being requested -- to fill in a qurt_sysenv_procname_t structure */
   /* &pn is the structure we want filled in */

   if (0 > (rc = qurt_qdi_handle_invoke(client_handle, QDI_OS_SYSENV, QDI_HANDLE_LOCAL_CLIENT, QURT_SYSENV_PROCNAME_TYPE, &pn)))
   {
#if defined(PD_MON_TRACER_SWEVT)
      tracer_event_simple_vargs(PD_DUMP_SWE_E, 2, 0, rc);
#endif
      TMS_MSG_ERROR_1("Cannot obtain client asid %d", rc);

      return rc;
   }

   rc = PD_DUMP_STATUS_ERROR; /**< failure */

   if (NULL == device_p)
   {
      return rc;
   }

   /* Driver Methods */

   switch (method)
   {
      /**
      INTERNAL, Default Method Handler
      */
      default:
      {
         rc = qurt_qdi_method_default(client_handle, obj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9); /**< pass along */

         break; /**< switch () */
      }

      /**
      API, pd_dump_init 
      a1.ptr is redundant device name 
      a2.ptr is the address of unmapped virtual memory region 
      a3.num is the size of unmapped virtual memory region
      a4.ptr is the address in user process where PD Dump driver updates the 
             size of total tcb contents 
      a5.num is used to indicate if TCB data needs to be dumped in 
             unmapped virtual memory region(0) or in note section(1)
             or in both(2)
      */
      case QDI_OPEN:
      {
         pd_dump_process_info_t pinfo;
         pinfo.pid = pn.asid;
         strlcpy(pinfo.name,pn.name,PROCESS_MAX_NAME_LEN);
         pinfo.is_untrusted = FALSE;
         rc = pd_dump_open(&device_p, &pinfo, a2.ptr, a3.num, a4.ptr, a5.num, TRUE);

         if(PD_DUMP_STATUS_SUCCESS == rc)
         {
            device_p->generic_data.client_handle = client_handle;
            rc = qurt_qdi_handle_create_from_obj_t(client_handle, (qurt_qdi_obj_t*)device_p); /**< return handle */

#if defined(PD_MON_TRACER_SWEVT)
            tracer_event_simple_vargs(PD_DUMP_SWE_I, 2, device_p->generic_data.hash, pn.asid);
#endif
            TMS_MSG_HIGH_3("QDI open %x: %d: %d", device_p->generic_data.hash, pn.asid, rc);
         }
         else
         {
           TMS_MSG_HIGH_2("QDI open failed for %d %d",pn.asid,rc);
         }

         break; /**< switch () */
      }

      /**
      API, pd_dump_destroy
      */
      case QDI_CLOSE:
      {
         rc = PD_DUMP_STATUS_ERROR; /**< handle does not close */

#if defined(PD_MON_TRACER_SWEVT)
         tracer_event_simple_vargs(PD_DUMP_SWE_D, 2, device_p->generic_data.hash, pn.asid);
#endif
         TMS_MSG_HIGH_3("QDI close %x %d, %d", device_p->generic_data.hash, pn.asid, rc);

         rc = qurt_qdi_method_default(client_handle, obj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9); /**< pass along */

         break; /**< switch () */
      }

      /* INTERNAL, PD_DUMP_ADD_NOTE_SECTION Handler
      a1.ptr is a pointer to the name of note section
      a2.ptr is a pointer to the desc of note section
      a3.num is the descsz of note section
      a4.num is the type of note section
      */
      case PD_DUMP_ADD_NOTE_SECTION:
      {
         rc = pd_dump_add_note_sec(&device_p->generic_data, a1.ptr, a2.ptr, a3.num, a4.num);
         break;
      }

      /**
      INTERNAL, OS_DUMP_PROCESS_EXIT Handler
      a1.ptr is a pointer to a list of (address, size) pairs
      a2.num is the number of entries pointed to by a1.ptr
      a3.ptr is a pointer to a list of process Thread IDs 
      a4.num is the number of entries pointed to by a3.ptr 
      a5.num is deprecated
      */
      //case QDI_OS_DUMP_PROCESS_EXIT:
      //{
      //  (void)pd_dump_collect_level_1(&device_p->generic_data, a1.ptr, a2.num, a3.ptr, a4.num, TRUE);
       // rc = PD_DUMP_STATUS_SUCCESS; /**< this code path always reports success; check F3 logs for information */
       // break; /**< switch () */
      //}
   }

   return rc;
}

/**
INTERNAL, QDI Opener Invocation Object
*/
static struct
{
   qurt_qdi_obj_t qdiobj;

} opener = { { pd_dump_device_invocation, QDI_REFCNT_PERM, 0 } };

/**
API, Initialization of service prior to use
@return
None.
*/
void pd_dump_interface_init(void)
{
   qurt_qdi_devname_register(PD_DUMP_DEVICE, &opener.qdiobj);  /**< register driver */

   pd_dump_register_qurt_cb();
}

int pd_dump_copy_to_user(int client_handle, void *dest, const void *src, unsigned len)
{
  return qurt_qdi_copy_to_user(client_handle, dest, src, len);
}

int pd_dump_copy_from_user(int client_handle, void *dest, const void *src, unsigned len)
{
  return qurt_qdi_copy_from_user(client_handle, dest, src, len);
}

void pd_dump_interface_data(struct pd_dump_device_s* device_p)
{
   device_p->qdiobj.invoke = pd_dump_device_invocation;
   device_p->qdiobj.refcnt = QDI_REFCNT_INIT;
   device_p->qdiobj.release = pd_dump_device_release;
   return;
}