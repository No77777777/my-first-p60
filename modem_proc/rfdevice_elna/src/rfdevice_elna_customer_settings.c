/*!
   @file
   rfdevice_elna_customer_settings.c

   @brief
   eLNA   customer settings file.
   This file needs to be populated with the eLNA device configuration settings by the customer

*/

/*===========================================================================

Copyright (c) 2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_elna_customer_settings.c#1 $

when       who    what, where, why
--------   ---   ---------------------------------------------------------------
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/03/14   vv    Initial file

============================================================================*/

/*======================================================================
                           INCLUDE FILES
========================================================================*/

#include "rfdevice_elna_customer_settings.h"


/*! 
============================================================================================
 This file needs to be populated with the eLNA device configuration settings by the customer
============================================================================================
!*/ 


/*! 
========================================================================
 COMMON SETTINGS 
======================================================================== 
!*/ 
 
/*=====================WAKEUP==========================================*/  
const uint16 elna_wakeup_reg_list[ELNA_WAKEUP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     wakeup_reg_0_addr , wakeup_reg_1_addr , ... wakeup_reg_n_addr
  */
  RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 elna_wakeup_data_list[ELNA_WAKEUP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     wakeup_data_0_data , wakeup_data_1_data , ... wakeup_data_n_data
  */
  RFDEVICE_ELNA_REG_DATA_INVALID,
};
/*====================================================================*/



/*=====================SLEEP==========================================*/  
const uint16 elna_sleep_reg_list[ELNA_SLEEP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     sleep_reg_0_addr , sleep_reg_1_addr , ... sleep_reg_n_addr
  */
 RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 elna_sleep_data_list[ELNA_SLEEP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     sleep_reg_0_data , sleep_reg_1_data , ... sleep_reg_n_data
  */
  RFDEVICE_ELNA_REG_DATA_INVALID,
};
/*====================================================================*/



/*=====================TRIGGER========================================*/  
const uint16 elna_trigger_reg_list[ELNA_TRIGGER_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     trigger_reg_0_addr , trigger_reg_1_addr , ... trigger_reg_n_addr
  */
  RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 elna_trigger_data_list[ELNA_TRIGGER_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     trigger_reg_0_data , trigger_reg_1_data , ... trigger_reg_n_data
  */
 RFDEVICE_ELNA_REG_DATA_INVALID,
};                               
/*=====================================================================*/



/*! 
==========================================================================
 PORT SPECIFIC SETTINGS 
========================================================================== 
!*/ 

/*=====================LNA GAIN SETTINGS ========================================*/  
const uint16 elna_gain_reg_list[ELNA_GAIN_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     gain_reg_0_addr , gain_reg_1_addr , ... gain_reg_n_addr
  */
  RFDEVICE_ELNA_REG_ADDR_INVALID,
};

const int16 elna_gain_data_list[ELNA_NUM_PORTS][ELNA_NUM_GAIN_STATES][ELNA_GAIN_SCRIPT_SIZE] = 
{
 /*!Populate this table as below*/
/*!-------------------------------------PORT NUM: 0 -----------------------------------*/
// {
//   { gain_0_reg_0_data , gain_0_reg_1_data , ... gain_0_reg_0_data },  /* LNA Gain: 0 */
//   { gain_1_reg_0_data , gain_1_reg_1_data , ... gain_1_reg_0_data },  /* LNA Gain: 1 */
//             .                    .                        .
//             .                    .                        .
//             .                    .                        .
//   { gain_n_reg_0_data , gain_n_reg_1_data , ... gain_n_reg_0_data },  /* LNA Gain: n */
// },
/*!-------------------------------------PORT NUM: 1 -----------------------------------*/
// {
// 
//   { gain_0_reg_0_data , gain_0_reg_1_data , ... gain_0_reg_0_data },  /* LNA Gain: 0 */
//   { gain_1_reg_0_data , gain_1_reg_1_data , ... gain_1_reg_0_data },  /* LNA Gain: 1 */
//             .                    .                        .
//             .                    .                        .
//             .                    .                        .
//   { gain_n_reg_0_data , gain_n_reg_1_data , ... gain_n_reg_0_data },  /* LNA Gain: n */
// 
// },
//  .
//  .
//  .
//  .
/*!-------------------------------------PORT NUM: N -----------------------------------*/
// {
// 
//   { gain_0_reg_0_data , gain_0_reg_1_data , ... gain_0_reg_0_data },  /* LNA Gain: 0 */
//   { gain_1_reg_0_data , gain_1_reg_1_data , ... gain_1_reg_0_data },  /* LNA Gain: 1 */
//             .                    .                        .
//             .                    .                        .
//             .                    .                        .
//   { gain_n_reg_0_data , gain_n_reg_1_data , ... gain_n_reg_0_data },  /* LNA Gain: n */
// 
// }
// {
//   { gain_0_reg_0_data , gain_0_reg_1_data , ... gain_0_reg_0_data },  /* LNA Gain: 0 */
//   { gain_1_reg_0_data , gain_1_reg_1_data , ... gain_1_reg_0_data },  /* LNA Gain: 1 */
//             .                    .                        .
//             .                    .                        .
//             .                    .                        .
//   { gain_n_reg_0_data , gain_n_reg_1_data , ... gain_n_reg_0_data },  /* LNA Gain: n */
// },
  RFDEVICE_ELNA_REG_DATA_INVALID,
};


boolean get_elna_customer_settings(rfdevice_elna_settings_type *elna_settings)
{
  boolean status = TRUE;

  /* NULL Pointer Check*/
   if(elna_settings == NULL)
   {
      return FALSE;
   }

  /*==========================================================================*/
  /* BUS Config settings */
  /*==========================================================================*/

   elna_settings->elna_device_bus_cfg.extended_cmd = FALSE;
   elna_settings->elna_device_bus_cfg.settings.rd_delay=FALSE;
   elna_settings->elna_device_bus_cfg.settings.half_rate=FALSE;

  /*==========================================================================*/
  /* Low power mode trigger for sleep/wakeup */
  /*==========================================================================*/
   elna_settings->lpm_pm_trig_flag = (boolean)ELNA_LPM_PM_TRIGGER;

  /*==========================================================================*/
  /* COMMON SETTINGS */
  /*==========================================================================*/

   /*WAKEUP*/
   elna_settings->common_settings.elna_wakeup_script_size  = ELNA_WAKEUP_SCRIPT_SIZE;
   elna_settings->common_settings.elna_wakeup.addr         = (const uint16*)elna_wakeup_reg_list;
   elna_settings->common_settings.elna_wakeup.data         = (const int16*)elna_wakeup_data_list;

  /*SLEEP*/
   elna_settings->common_settings.elna_sleep_script_size   = ELNA_SLEEP_SCRIPT_SIZE;
   elna_settings->common_settings.elna_sleep.addr          = (const uint16*)elna_sleep_reg_list;
   elna_settings->common_settings.elna_sleep.data          = (const int16*)elna_sleep_data_list;

   /*TRIGGER*/
   elna_settings->common_settings.elna_trigger_script_size  = ELNA_TRIGGER_SCRIPT_SIZE;
   elna_settings->common_settings.elna_trigger.addr         = (const uint16*)elna_trigger_reg_list;
   elna_settings->common_settings.elna_trigger.data         = (const int16*)elna_trigger_data_list;


  /*==========================================================================*/
  /* PORT SETTINGS */
  /*==========================================================================*/
   elna_settings->num_ports = ELNA_NUM_PORTS;

   /*eLNA gain*/
   elna_settings->port_settings.elna_gain_script_size    = ELNA_GAIN_SCRIPT_SIZE;
   elna_settings->port_settings.elna_num_gain_states     = ELNA_NUM_GAIN_STATES;
   elna_settings->port_settings.elna_gain.addr           = (const uint16*)elna_gain_reg_list;
   elna_settings->port_settings.elna_gain.data           = (const int16*)(&(elna_gain_data_list[0][0][0]));     

  return status;  

}
