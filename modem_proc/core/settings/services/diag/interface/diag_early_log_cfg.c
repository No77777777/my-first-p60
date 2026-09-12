
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      DIAG Bootup Log Configuration

GENERAL DESCRIPTION
  DAL Device Configuration support for enabling diag boot up logs

Copyright (c) 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*==========================================================================

                        EDIT HISTORY FOR MODULE
$Header: //components/rel/core.mpss/10.0/settings/services/diag/interface/diag_early_log_cfg.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/29/18   ph      Enable diag boot up logging.
04/29/18   ph      Created the new file.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "com_dtypes.h"
#include "diag_early_log_cfg.h"


typedef struct DiagLogTableSize_t DiagLogTableSize;

struct DiagLogTableSize_t {
   uint32 size;
};

/*
This array consists of MSG SSID and MSG Level pairs that need to be enabled during
DIAG boot up. This is extracted from DMC file that is used for loading the masks on
to the tool.
*/
const diag_early_msg_config diag_early_msg_array[]=
{
  {7,01}, {7,02}, {7,03}, {7,04}
};

/*
This array consists of Log codes that need to be enabled during DIAG boot up.
This is extracted from DMC file that is used for loading the masks on to the tool.
*/
const uint32 diag_early_log_arr[]={
  0x115F
};

/*
This array consists of Event IDs that need to be enabled during DIAG boot up.
This is extracted from DMC file that is used for loading the masks on to the tool.
*/
const uint32 diag_early_event_arr[]={
  0,414,486,487,1752
  };


#ifndef DIAG_EARLY_LOG_CONFIG_TABLE_SIZE
#define DIAG_EARLY_LOG_CONFIG_TABLE_SIZE sizeof(diag_early_log_arr)/sizeof(uint32)
#endif

#ifndef DIAG_EARLY_EVENT_CONFIG_TABLE_SIZE
#define DIAG_EARLY_EVENT_CONFIG_TABLE_SIZE sizeof(diag_early_event_arr)/sizeof(uint32)
#endif

#ifndef DIAG_EARLY_MSG_CONFIG_TABLE_SIZE
#define DIAG_EARLY_MSG_CONFIG_TABLE_SIZE sizeof(diag_early_msg_array)/sizeof(diag_early_msg_config)
#endif

const DiagLogTableSize diag_log_table_size_array[]={
   {DIAG_EARLY_LOG_CONFIG_TABLE_SIZE},
 };

 const DiagLogTableSize diag_event_table_size_array[]={
   {DIAG_EARLY_EVENT_CONFIG_TABLE_SIZE},
 };

  const DiagLogTableSize diag_msg_table_size_array[]={
   {DIAG_EARLY_MSG_CONFIG_TABLE_SIZE},
 };


