/*#============================================================================
#  Name:                                                                     
#    root_init.h 
#
#  Description:                                                              
#    PD related info used by elf loader/dynamic loader to spwan a new User pD. 
#                                                                            
# Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.        
#============================================================================
#-------------------------------------------------------------------------------
#
#  $Header: //components/rel/core.mpss/10.0/kernel/elf_loader/root_init/inc/root_init.h#1 $
#  $DateTime: 2019/04/24 00:03:26 $
#  $Change: 18983508 $
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when       who     what, where, why
# --------   ---     ---------------------------------------------------------
# 01/09/17    tkuo    Created.
#-------------------------------------------------------------------------------
*/

#include "qurt.h"
#include "pthread_types.h"
/* typedefs */
typedef struct qurt_eloader_thread_list_t
{
  void *stack; //allocated stack pointer
  qurt_anysignal_t *sig;
  pthread_t tid; //tid 
} eloader_thread_list_t;

typedef struct eloader_pd_info
{
  const char * domain_name;
  const char * process_name;
  const char * img_name;
  char * rfs_path;
  char * rfs_path_vendor; /* this field is for vendor file system path */
  char * rfs_path_vendor_2; /* this field is for second/ optional vendor file system path */
  eloader_thread_list_t eloader_thread;
} eloader_pd_info_t;

typedef enum user_pd_name
{
 N_WLAN_PD=0x0000,
 N_TEST_PD=0x0001,
 N_MAX_NUM_USERPD=0x0002
}user_pd_name_t;
