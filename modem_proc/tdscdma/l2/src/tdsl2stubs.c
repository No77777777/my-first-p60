/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

        L A Y E R  2  STUBS FILE

GENERAL DESCRIPTION
  This file contains the Layer 2 STUBS and API dispatcher implementation.

Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

=============================================================================================*/
/*=============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/L2/vcs/l2ultask.c_v   1.18   10 Jul 2002 10:07:34   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/l2/src/tdsl2stubs.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------------------- 
07/18/14  sarao   Created the stubs file to provide support for No TDS builds using TDS APIs in Audio side



=============================================================================================*/

/*=============================================================================================

                      I N C L U D E    F I L E S

=============================================================================================*/
#ifndef FEATURE_TDSCDMA
#include  "rex.h"
#include  "task.h"
#include  "msg.h"
#include  "err.h"
#include  "tdsl2external.h"


/*==============================================================================================
 FUNCTION NAME  tds_l2_api_call

 DESCRIPTION  This function implements the wrapper for all public APIs. This wrapper function can be used by external modules to use 
 TDSCDMA APIs. Currently this function supports the following  L2 external APIs tdsl2_ul_service_register(), tdsl2_dl_service_register()
 and  tdscdma_register_vfr_notification().
 
 Instead of calling the APIs directly, MVS/Audio or other modules can call the generic tdscdma_l2_api_call() and provide the arguments and
 cmd_id suggesting which API they are referring. The wrapper function will internally call the APIs and provide the result back.
 
 The return value is defined in enum tds_l2_api_ret_value_t. Users can use this enum to find out the error cause.
 
 DEPENDENCIES:
  None.

RETURN VALUE:
  tds_l2_api_ret_value_t.

SIDE EFFECTS:
None.
===============================================================================================*/
tds_l2_api_ret_value_t tdscdma_l2_api_call 
(
  uint32   cmd_id,   /*This will tell us which API needs to be called */
  void*    params,   /* This will provide the API arguments */
  uint32   size      
)
{

	tds_l2_api_ret_value_t rc = TDS_EOK;
	boolean ret_value = FALSE;

	/* Perform common parameter checks */

	if( size >0) 
	{
		if(params == NULL)
		{
			//TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected NULL params received cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		}
		
	}
	
	else
	{
			//TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected Zero size params received cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
	}
	
	 /* Command parser. */
	 switch (cmd_id)
	 {
	 
	 default:
	 {
		//TDSL2_MSG2(MSG_LEGACY_ERROR,"Unsupported command cmd_id %d and size %d ",cmd_id,size);
		rc = TDS_EUNSUPPORTED;
		
	 
	 }  break;
	 
	} 
	 if ( rc != TDS_EOK ) 
	 {
		//TDSL2_MSG1(MSG_LEGACY_ERROR,"An error %d occurred processing command  %d",cmd_id);
	 }

	return rc;
	 
}

#endif //FEATURE_TDSCDMA

