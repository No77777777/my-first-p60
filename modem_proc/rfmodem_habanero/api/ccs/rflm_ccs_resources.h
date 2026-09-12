#ifndef RFLM_CCS_RESOURCES_H
#define RFLM_CCS_RESOURCES_H


/*
   @file
   rflm_resources.h

   @brief
   RFLM CCS Driver's interface for resources request from other clients.

   @details

*/

/*===========================================================================
Copyright (c) 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfmodem_habanero.mpss/1.11/api/ccs/rflm_ccs_resources.h#1 $
$DateTime: 2021/03/09 05:29:45 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/10/14   ra      Support CCS memory retention voting on Tabasco 
08/10/14   ra      Support CCS memory retention voting for Thor
04/21/14   ra      Support M3 bit flip with no image download via MCPM
10/01/13   ra      Added support for CCS memory collapse 

==========================================================================*/

typedef enum{
  RFC_RETAIN_MEMORY,
  RFC_DONT_RETAIN_MEMORY,
  RFC_MCPM_MAX
} rfm_sleep_cfg_e;

rfm_sleep_cfg_e  rfm_get_mcpm_rfi_sleep_control( void );


void rfm_ccs_sleep(void);
void rfm_ccs_wakeup(void);

#endif  /* RFLM_CCS_RESOURCES.H */
