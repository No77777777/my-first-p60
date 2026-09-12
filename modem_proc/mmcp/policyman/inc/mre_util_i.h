#ifndef _MRE_UTIL_I_H_
#define _MRE_UTIL_I_H_

/**
  @file mre_util_i.h

  @brief
*/

/*
    Copyright (c) 2017 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/mre_util_i.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/


#include "comdef.h"
#include <sys/types.h>

#include "mre_namedobj.h"
#include "mre_util.h"


/*-------- sys_get_num_of_mnc_digits --------*/
boolean sys_get_num_of_mnc_digits(
  mmgsdi_data_type * imsi_ptr,
  mmgsdi_data_type * ad_ptr,
  uint8            * num_mnc_digits_ptr
);


/*=============================================================================
    Memory management functions.
=============================================================================*/

/*-------- mre_simple_dtor --------*/
/**
@brief  A destructor to be used when all that is required is to free
        the memory block that is the object.

@param[in]  pObj  Pointer to the object that is being destroyed.

@return
  Nothing.
*/
void mre_simple_dtor(
  void  *pObj
);



/*=============================================================================
  Other APIs
=============================================================================*/

typedef enum
{
  MRE_SEPARATOR_HYPHEN,         /* use '-' */
  MRE_SEPARATOR_STAR,           /* use '*' */
  MRE_SEPARATOR_UNDER,          /* use '_' */
  MRE_SEPARATOR_PLUS,           /* use '+' */
  MRE_SEPARATOR_EQUAL,          /* use '=' */
} mre_msg_separator_t;



/*-------- mre_util_msg_separator --------*/
/**
@brief  Log a separator message.

@param[in]  which   The sort of separator to use.

@return

*/
void mre_util_msg_separator(
  mre_msg_separator_t  which
);

/*-------- mre_util_get_time --------*/
/**
@brief  Returns a millisecond resolution timestamp as a 32-bit quantity.
        This has about 1.5 days of resolution -- enough for logging purposes.

@return 32-bit ms timestamp

*/
uint32 mre_util_get_time(
  void
);


/*-------- mre_str_to_boolean --------*/
mre_status_t mre_str_to_boolean(
  char const  *pStr,
  boolean     *pBoolean
);


/*-------- mre_plmn_get_mcc --------*/
uint32 mre_plmn_get_mcc(
  sys_plmn_id_s_type  *pPlmn
);


/*-------- mre_util_plmn_set_has_mcc --------*/
boolean mre_util_plmn_set_has_mcc(
  mre_set_t     *pPlmnSet,
  sys_mcc_type   mcc
);

#endif /* _MRE_UTIL_H_ */
