#ifndef RFDEVICE_ELNA_CUSTOMER_SETTINGS_H
#define RFDEVICE_ELNA_CUSTOMER_SETTINGS_H

/*=============================================================================

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S.
  Government. Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //source/qcom/qct/modem/rfdevice/qelna/main/latest/api/rfdevice_elna_customer_settings.h
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   ---------------------------------------------------------------
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/03/14   vv    Initial version

=============================================================================*/

/*=============================================================================
                            INCLUDE FILES FOR MODULE
=============================================================================*/
#include "rfc_common.h"
#include "rfdevice_elna_typedef.h"


/*==========================================================================*/
/* DEFINES and ENUMS  */
/*==========================================================================*/

/*! 
==================================================================================================
 This below enums needs to be populated with the eLNA device configuration settings by the customer
================================================================================================== 
*/ 
#define CUST_MFG_ID               0x00   /*Customer manufacture ID*/    
#define CUST_PRD_ID               0x00   /*Customer product ID*/
#define CUST_PRD_REV_ID           0x00   /*Customer product rev ID*/
#define ELNA_WAKEUP_SCRIPT_SIZE   1      /*Total number of register writes in wakeup script*/
#define ELNA_SLEEP_SCRIPT_SIZE    1      /*Total number of register writes in sleep script*/
#define ELNA_TRIGGER_SCRIPT_SIZE  1      /*Total number of register writes in trigger script*/
#define ELNA_GAIN_SCRIPT_SIZE     1      /*Total number of register writes per GAIN script*/
#define ELNA_NUM_GAIN_STATES      1      /*Total number of GAIN states supported by the eLNA device*/
#define ELNA_NUM_PORTS            1      /*Total software ports supported by the eLNA device*/
#define ELNA_LPM_PM_TRIGGER       0      /*Specify the Low power mode trigger for sleep/wakeup */

/*==========================================================================*/
/* Function Definitions  */
/*==========================================================================*/
/*!
    @brief
    return the customer elna device settings to the eLNA device-driver

    @details
    get_elna_customer_settings()
   
    @param elna_settings
    pointer to an elna configuration settings structure which will be populated and returned to the eLNA driver during device construction.
   
    @return
    boolean indicating pass or fail
*/
boolean get_elna_customer_settings(rfdevice_elna_settings_type *elna_settings);

#endif /*RFDEVICE_ELNA_CUSTOMER_SETTINGS_H */

