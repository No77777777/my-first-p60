/*!
  @file 
  wlan_md.c

  @brief

*/

/*==============================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     -------------------------------------------------------------
            
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include "wlan_md.h"
#include "qurt.h"
#include "atomic_ops.h"
//#include "qurt_qdi_driver.h"
//#include "qurt_qdi.h"
//#include "qurt_restricted.h"

extern wlan_md_ctx_t g_wlan_md_ctx;
uint32 wlan_md_qdi_open_status = 0;

/*! \brief wlan_md_qdi_release
*
* @param[in] QURT QDI object
* \return    NA
*
* WLAN MD QDI release function for relinquishing system
* resources
*
*/
static void wlan_md_qdi_release(qurt_qdi_obj_t *qdiobj)
{
	if (g_wlan_md_ctx.mtx.count != 0)
	{
		qurt_rmutex_destroy(&g_wlan_md_ctx.mtx);
	}
	wlan_md_qdi_open_status = 0;
}

/*! \brief wlan_md_qdi_invocation
*
* @param[in] QDI client handle, QURT QDI object
* @param[in] method, arguments
* \return    NA
*
* WLAN MD QDI invocation function for sending request 
* from use to root PD
*
*/
static int wlan_md_qdi_invocation
(
	int client_handle,
	qurt_qdi_obj_t *obj,
	int method,
	qurt_qdi_arg_t a1,
	qurt_qdi_arg_t a2,
	qurt_qdi_arg_t a3,
	qurt_qdi_arg_t a4,
	qurt_qdi_arg_t a5,
	qurt_qdi_arg_t a6,
	qurt_qdi_arg_t a7,
	qurt_qdi_arg_t a8,
	qurt_qdi_arg_t a9
)
{
    int result=0;	

    switch (method)
    {
	case QDI_OPEN:
             if (!qurt_atomic_compare_and_set((void *)&wlan_md_qdi_open_status, 0, 1))
	     {
		/* Unsuccessful */
		result = -1;
		break;
	     }
	     g_wlan_md_ctx.qdiobj.invoke = wlan_md_qdi_invocation;
             g_wlan_md_ctx.qdiobj.refcnt = QDI_REFCNT_INIT;
             g_wlan_md_ctx.qdiobj.release = wlan_md_qdi_release;

             qurt_rmutex_init(&g_wlan_md_ctx.mtx);
             result = qurt_qdi_new_handle_from_obj_t(client_handle, &g_wlan_md_ctx.qdiobj);
	     /* Set qdi open status to 0 if the new object creation fails */
	     if(result < 0)
	     {
		wlan_md_qdi_open_status = 0;
	     }
     	     break;
	case WLAN_MD_REGISTER_REGION:
	     result = wlan_md_register_region(client_handle, a1.ptr);
	     break;
	case WLAN_MD_CONFIG:
		 wlan_md_config_callback(*(boolean *)a1.ptr);
		 break;
	case QDI_CLOSE:
	default:
	     result = qurt_qdi_method_default(client_handle, obj, method,
			a1, a2, a3, a4, a5, a6, a7, a8, a9);
   }
   return result;
}

/*! \brief wlan_md_qdi_init
* \return    NA
* WLAN MD QDI init function to create server
*
*/

void wlan_md_qdi_init(void)
{
	int ret = 0;

	wlan_md_qdi_opener.invoke = wlan_md_qdi_invocation;
	wlan_md_qdi_opener.refcnt = QDI_REFCNT_PERM;
	wlan_md_qdi_opener.release = NULL;

	ret = qurt_qdi_devname_register(WLAN_MD_QDI_DRV_NAME, &wlan_md_qdi_opener);

	if (ret != 0)
	{
	}
	else
	{
	}
}


