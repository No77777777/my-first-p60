/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_user.c
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
Copyright (c) 2014 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_user_rtld_debug.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

typedef char* caddr_t;

#include <stddef.h>
#include "link_elf.h"
#include "pd_mon_qurt.h"
#include "pd_mon_internal.h"
#include "pd_dump.h"
#include "pd_elf_hexagon.h"
#include "tms_utils_msg.h"
#include "tms_utils.h"


extern struct r_debug _rtld_debug;
extern PD_DUMP_STATUS pd_dump_add_note_section(char * name, char * desc, uint32 descsz, uint32 type );
/**
API, pd_dump_user_rtld_debug_init
@param[in] NULL
@return
PD_DUMP_HANDLE Opaque Device Handle
*/
void pd_dump_user_rtld_debug_init(void)
{
   unsigned int addr;

   addr = (unsigned int)&_rtld_debug;

   pd_dump_add_note_section("QC_Hexagon",(char *)&addr, sizeof(unsigned int), NT_HEXAGON_RTLD_LINKMAP);
}

