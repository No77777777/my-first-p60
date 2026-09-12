#ifndef SERVREG_LOCALDB_H
#define SERVREG_LOCALDB_H
/*
#============================================================================
#  Name:                                                                     
#    servreg_localdb.h 
#
#  Description:                                                              
#    None 
#                                                                            
# Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.        
#============================================================================
*/
#include "stdlib.h"
#include "servreg_common.h"
#include "servreg_internal.h"
#include "servreg_locator.h"

#if defined(__cplusplus)
extern "C"
{
#endif

struct servreg_entry_s
{
   char * service_name;
   unsigned int service_data_valid;
   unsigned int service_data;
};
typedef struct servreg_entry_s servreg_entry_t, * servreg_entry_p;

#if defined(__cplusplus)
}
#endif

#endif
