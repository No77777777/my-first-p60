/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_interface_qdi.h
@brief This file contains qdi interface specific data.
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
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_interface_qdi.h#1 $
$DateTime: 2019/11/07 04:24:43 $
$Change: 21281462 $
$Author: pwbldsvc $
===============================================================================*/
#include "pd_dump_generic.h"

struct pd_dump_device_s /**< localized driver storage area */
{
   qurt_qdi_obj_t qdiobj; /**< required to be first */

   struct pd_dump_generic_s generic_data; /**< common data across interfaces */

   struct pd_dump_device_s* next; /**< next device (one per user domain) */

};


