/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_qdi_client.c
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
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_qdi_client.c#1 $
$DateTime: 2019/11/07 04:24:43 $
$Change: 21281462 $
$Author: pwbldsvc $
===============================================================================*/

#include "pd_mon_qurt.h"
#include "pd_mon_internal.h"
#include "pd_dump.h"
#include "qurt_memory.h"
#include "qurt_thread_context.h"

PD_DUMP_HANDLE pd_dump_register_client(struct qurt_debug_thread_info * addr, int region_len, unsigned int *size)
{
   return  qurt_qdi_open(PD_DUMP_DEVICE, addr, region_len, size, PDDUMP_TCB_DATA_UNMAPPED_REGION_NOTE_SECTION);
}

PD_DUMP_STATUS pd_dump_add_note_section_interface(int handle, char * name, char * desc, uint32 descsz, uint32 type )
{
   return  qurt_qdi_handle_invoke(handle, PD_DUMP_ADD_NOTE_SECTION, name, desc, descsz, type);
}

PD_DUMP_HANDLE pd_dump_close_client(int handle)
{
   return qurt_qdi_close(handle);
}