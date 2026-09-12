/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 MMGSDI - MEMORY CLEANUP RELATED FUNCTIONS


GENERAL DESCRIPTION

  This source file contains functions for freeing up the memory allocated
  and held by MMGSDI during its normal operation in off-target environment.
  This is called only after all MMGSDI tasks have stopped

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2016, 2019 QUALCOMM Techologies, Inc (QTI) and its licensors.  All Rights
Reserved.  QUALCOMM Techologies Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_memory_cleanup.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/16/19    sg     Fallback to file system/cache for missing files
04/06/16    sp     Add DSDS onchip support
02/29/16    bcho   Delete attr cache while deleting cache of EPSLOCI and EPSNSC
01/22/16    bcho   Send Card and session events based on SFS data in PSM mode
01/07/16    sp     Protect prl cache write with critical section 
12/10/15    bcho   Usage of mmgsdi_refresh_data_info global removed
11/02/15    yt     Remove cmd_rsp_cb_ptr from mmgsdi_client_id_reg_table_type
10/27/15    vdc    Send profile switching evt when profile has switched in card
08/13/15    sp     merging mmgsdiutil_mem_free macros into one
06/18/15    ar     Validate PRL at diff steps if SD task not ready
05/28/15    nr     Fixing memoory leak during card powerdown and task stop
05/18/15    nr     Fix to distiguish reads on different apps in single slot
04/29/15    bcho   Support for App specific cache
02/19/15    nr     Phonebook record read optimization
11/24/14    kr     Fixed Compilation warnings
11/15/14    ar     Reduce usage of MMGSDI_TASK_ENTER/LEAVE_CRIT_SECT
10/16/14    am     Flush high pri queues without clien_id/session_id reference 
09/29/14    tkl    Added support for persistent cache
09/27/14    ar     Add support for multiple slots in BT-SAP
09/21/14    ar     Add check for task_state before accessing its globals
08/14/14    tl     Add support for silent PIN verification
08/06/14    am     Clear refresh retry info 
07/15/14    yt     Support for NV refresh
07/14/14    tl     Remove always ON feature FEATURE_MODEM_RCINIT
07/14/14    am     Free the pointers in response queues causing leaks
07/14/14    am     Correctly pass slot index 
06/11/14    vv     Fix for a possible memory leak
04/06/14    am     Initial version
===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
