/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               PDCOMMSNV Items using EFS API Interface Header File

GENERAL DESCRIPTION
  This file contains the structure definitions and function prototypes for 
  the PDCOMMS Non-Volatile Memory interface.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2016-18 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/pd_comms/inc/pd_comms_nv_efs.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $ 

=============================================================================

                           EDIT HISTORY FOR FILE

when       who         what, where, why
--------   ---         ----------------------------------------------------------
09/30/19   skm          Support for 911 nDDS on DSDS targets  
04/10/18   py           variable to keep track of current DD sub and its get and set function
09/13/10   ssheshad     Initial Release.

=============================================================================

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef PD_COMMS_NV_EFS_H
#define PD_COMMS_NV_EFS_H

/*
 * Include files
*/
#include "comdef.h"
#include "customer.h"
#include "gps_nv_efs.h"
#include "nv.h"
#include "sys.h"

/* All the NV EFS values are initialzed here */
/* default value for the RSA key length */
#define CGPS_NV_EFS_PDCOMM_SSL_RSA_CERTIFICATE_KEY_LENGTH_DEFAULT (256)
#define PD_COMMS_CONFIG_MIN_SUB_ID SYS_MODEM_AS_ID_1
#define PD_COMMS_CONFIG_MAX_SUB_ID SYS_MODEM_AS_ID_3
#define PD_COMMS_CONFIG_MAX_SUBS   (PD_COMMS_CONFIG_MAX_SUB_ID+1) /*Max number of subscriptions */


/* add all the default values for NV items here */
/*
 * Structure definitions
*/
/*===========================================================================

FUNCTION sm_nv_efs_init

DESCRIPTION
  This function is used to initialize internal PDCOMM-NV data efs structure.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void pd_comms_nv_efs_init (void);

/*===========================================================================

FUNCTION pd_comms_nv_efs_reg_item_read

DESCRIPTION
  This function is used to read NV item. If the read fails, it sets it to
  default value.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
boolean pd_comms_nv_efs_reg_item_read(cgps_nv_efs_reg_item_type const item, 
                                      uint32 data_size,
                                      void *data_ptr,
                                      boolean force_read_from_hw,
                                      sys_modem_as_id_e_type sub
                                      );

/*===========================================================================

FUNCTION pd_comms_nv_efs_reg_item_write

DESCRIPTION
  This function is used to write an NV item. 
 
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
boolean pd_comms_nv_efs_reg_item_write(cgps_nv_efs_reg_item_type const item, 
                                       uint32 data_size,
                                       void *data_ptr
                                      );

/*===========================================================================

FUNCTION pd_comms_nv_efs_set_current_dd_sub

DESCRIPTION
     This function is used to set the current default subscription ID(usually Dedicated-Data sub). When EFS NV 
  read/write is performed without giving specific Subs ID, this current Subs ID is will be used.
 
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void pd_comms_nv_efs_set_current_dd_sub(sys_modem_as_id_e_type e_sub_id);

/*===========================================================================

FUNCTION pd_comms_nv_efs_get_current_dd_sub

DESCRIPTION
     This function is used to set the current default subscription ID(usually Dedicated-Data sub). When EFS NV 
  read/write is performed without giving specific Subs ID, this current Subs ID is will be used.
 
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
sys_modem_as_id_e_type pd_comms_nv_efs_get_current_dd_sub(void);

#endif /* PD_COMMS_NV_EFS_H */

