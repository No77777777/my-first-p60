/*============================================================================
  @file core_user_info.h

  Copyright (c) 2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

============================================================================*/
#ifndef _CORE_USER_INFO_
#define _CORE_USER_INFO_

#include "comdef.h"

#define MAX_NUM_RW_SECTION 5

typedef enum core_user_result_enum
{
   CORE_USER_FAILURE = -1,
   CORE_USER_SUCCESS,
}core_user_eResult;

typedef struct core_info {
	uint32 start_rw_addr[MAX_NUM_RW_SECTION];
	uint32 size[MAX_NUM_RW_SECTION];
	uint32 base_address;
	uint32 n_region;
}core_info;

core_user_eResult core_user_info(core_info *info);

#endif /* _CORE_USER_INFO_ */
