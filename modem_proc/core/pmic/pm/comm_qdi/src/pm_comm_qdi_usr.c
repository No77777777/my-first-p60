/*! \file   pm_comm_qdi_usr.c
*  
*  \brief   This file contains PMIC QDI driver user initialization
*  \details PMIC Modem Proc initialization to initialize driver resources
*  
*  &copy; Copyright 2019-2020 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

#include "pm_comm_qdi.h"
#include "dll_global_def.h"
#include "pm_ulog.h"

int pm_comm_qdi_client_handle = -1;

DLL_API_GLOBAL int pm_comm_qdi_reg_read(uint32 slave_id, uint16 addr, uint8 *data)
{
   if(pm_comm_qdi_client_handle < 0)
   { 
     return -1;
   }
   return qurt_qdi_handle_invoke( pm_comm_qdi_client_handle, PM_COMM_QDI_REGISTER_READ, slave_id, addr, data);
}


DLL_API_GLOBAL void pm_comm_qdi_usr_init()
{

 pm_comm_qdi_client_handle = qurt_qdi_open(PM_COMM_QDI_DRV_NAME);

 if(pm_comm_qdi_client_handle < 0)
 {
   return;
 }

}
