/*! \file   pm_comm_qdi_srv.c
*  
*  \brief   This file contains PMIC QDI driver initialization
*  \details PMIC Modem Proc initialization to initialize driver resources
*  
*  &copy; Copyright 2019-2020 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

#include "comdef.h"
//#include <sys.h>
#include "qurt_atomic_ops.h"
#include "pm_comm_qdi.h"
#include "pm_comm.h"
#include "qurt_qdi_constants.h"
#include "qurt_rmutex.h"
#include "pm_ulog.h"

typedef struct pm_comm_ctx_t {
    qurt_qdi_obj_t qdiobj;
    qurt_mutex_t mtx;
}pm_comm_ctx_t;

qurt_qdi_obj_t pm_comm_qdi_opener;
pm_comm_ctx_t g_pm_comm_ctx;
uint32 pm_comm_qdi_open_status = 0;

/*! \brief pm_comm_qdi_release
*
* @param[in] QURT QDI object
* \return    NA
*
* PM COMM QDI release function for relinquishing system
* resources
*
*/
static void pm_comm_qdi_release(qurt_qdi_obj_t *qdiobj)
{
	if (g_pm_comm_ctx.mtx.count != 0)
	{
		qurt_rmutex_destroy(&g_pm_comm_ctx.mtx);
	}
	pm_comm_qdi_open_status = 0;
}

/*! \brief pm_comm_qdi_invocation
*
* @param[in] QDI client handle, QURT QDI object
* @param[in] method, arguments
* \return    NA
*
* PM COMM QDI invocation function for sending request 
* from use to root PD
*
*/
static int pm_comm_qdi_invocation
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
    int result;
    unsigned char data;    

    switch (method)
    {
	case QDI_OPEN:
         if (!qurt_atomic_compare_and_set((void *)&pm_comm_qdi_open_status, 0, 1))
	     {
			/* Unsuccessful */
			result = -1;
			break;
		 }
	     g_pm_comm_ctx.qdiobj.invoke = pm_comm_qdi_invocation;
         g_pm_comm_ctx.qdiobj.refcnt = QDI_REFCNT_INIT;
         g_pm_comm_ctx.qdiobj.release = pm_comm_qdi_release;

         qurt_rmutex_init(&g_pm_comm_ctx.mtx);
         result = qurt_qdi_new_handle_from_obj_t(client_handle, &g_pm_comm_ctx.qdiobj);
	     /* Set qdi open status to 0 if the new object creation fails */
	     if(result < 0)
	     {
			pm_comm_qdi_open_status = 0;
	     }
     	 break;
	
	case PM_COMM_QDI_REGISTER_READ:
	     result = pm_comm_read_byte(a1.num, a2.num, &data, 0);
	     result = qurt_qdi_copy_to_user(client_handle, a3.ptr, &data, sizeof(unsigned char));
	     break;
	case QDI_CLOSE:
	default:
	     result = qurt_qdi_method_default(client_handle, obj, method,
			a1, a2, a3, a4, a5, a6, a7, a8, a9);
   }
   return result;
}

/*! \brief pm_comm_qdi_init
* \return    NA
* PM COMM QDI init function to create server
*
*/

void pm_comm_qdi_init(void)
{
	int ret = 0;

	pm_comm_qdi_opener.invoke = pm_comm_qdi_invocation;
	pm_comm_qdi_opener.refcnt = QDI_REFCNT_PERM;
	pm_comm_qdi_opener.release = NULL;

	ret = qurt_qdi_devname_register(PM_COMM_QDI_DRV_NAME, &pm_comm_qdi_opener);

	if (ret != 0)
	{
		PM_LOG_MSG_INFO("PM QDI init fail");
	}
	else
	{
		PM_LOG_MSG_INFO("PM QDI init success");
	}
}

