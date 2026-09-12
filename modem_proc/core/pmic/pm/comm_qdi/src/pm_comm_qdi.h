#ifndef PM_COMM_QDI_H
#define PM_COMM_QDI_H

/*! \file pm_comm_qdi.h
 *  \n
 *  \brief
 *  \n  
 *  &copy; Copyright 2019-2020 QUALCOMM Technologies Incorporated, All Rights Reserved
 */
#include "comdef.h"
#include "qurt_qdi_constants.h"
#include "qurt_qdi_driver.h"
#include "qurt_qdi.h"
#define PM_COMM_QDI_DRV_NAME       "/dev/pmcomm"
#define PM_COMM_QDI_REGISTER_READ  QDI_PRIVATE

int pm_comm_qdi_reg_read(uint32 slave_id, uint16 addr, uint8 *data);
void pm_comm_qdi_init(void);
#endif /*PM_COMM_QDI_H*/
