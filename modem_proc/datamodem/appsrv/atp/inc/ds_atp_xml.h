/******************************************************************************
  @file    ds_atp_xml.h
  @brief   XML parser header file

  DESCRIPTION
  XML parser header file

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2009-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/atp/inc/ds_atp_xml.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/18/13   jz      Created
===========================================================================*/
#ifndef DS_ATP_XML_H
#define DS_ATP_XML_H

#include "datamodem_variation.h"

#ifdef FEATURE_DATA_ATP

#include "ds_atp_internal.h"


#ifdef __cplusplus
extern "C"
{
#endif

boolean ds_atp_xml_parsing(const char* xml_string_ptr, ds_atp_policy_info_internal_type* policy_ptr);

#ifdef __cplusplus
}
#endif

#endif  /*FEATURE_DATA_ATP*/

#endif
