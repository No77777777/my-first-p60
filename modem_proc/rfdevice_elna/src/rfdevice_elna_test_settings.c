/*!
   @file
   rfdevice_elna.cpp

   @brief
   eLNA  device driver

*/

/*===========================================================================

Copyright (c) 2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_elna_test_settings.c#1 $

when       who    what, where, why
--------   ---   ---------------------------------------------------------------
02/17/15   vv    eLNA sleep/wakeup PM trigger support
12/03/14   vv    Initial file

============================================================================*/


/*! 
============================================================================================
 This file is used for internal testing purposes only
============================================================================================
!*/


/*======================================================================
                           INCLUDE FILES
========================================================================*/

#include "rfdevice_elna_test_settings.h"


/*! 
========================================================================
 COMMON SETTINGS 
======================================================================== 
!*/ 
 
/*=====================WAKEUP==========================================*/  
const uint16 test_elna_wakeup_reg_list[TEST_ELNA_WAKEUP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     wakeup_reg_0_addr , wakeup_reg_1_addr , ... wakeup_reg_n_addr
  */
  RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 test_elna_wakeup_data_list[TEST_ELNA_WAKEUP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     wakeup_data_0_data , wakeup_data_1_data , ... wakeup_data_n_data
  */
  RFDEVICE_ELNA_REG_DATA_INVALID,
};
/*====================================================================*/



/*=====================SLEEP==========================================*/  
const uint16 test_elna_sleep_reg_list[TEST_ELNA_SLEEP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     sleep_reg_0_addr , sleep_reg_1_addr , ... sleep_reg_n_addr
  */
 RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 test_elna_sleep_data_list[TEST_ELNA_SLEEP_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     sleep_reg_0_data , sleep_reg_1_data , ... sleep_reg_n_data
  */
  RFDEVICE_ELNA_REG_DATA_INVALID,
};
/*====================================================================*/



/*=====================TRIGGER========================================*/  
const uint16 test_elna_trigger_reg_list[TEST_ELNA_TRIGGER_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     trigger_reg_0_addr , trigger_reg_1_addr , ... trigger_reg_n_addr
  */
  RFDEVICE_ELNA_REG_ADDR_INVALID,
};
const int16 test_elna_trigger_data_list[TEST_ELNA_TRIGGER_SCRIPT_SIZE] = 
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
const uint16 test_elna_gain_reg_list[TEST_ELNA_GAIN_SCRIPT_SIZE] = 
{
  /*!Populate this table as below*/
  /* 
     gain_reg_0_addr , gain_reg_1_addr , ... gain_reg_n_addr
  */
  0x00,0x01
};

const int16 test_elna_gain_data_list[TEST_ELNA_NUM_PORTS][TEST_ELNA_NUM_GAIN_STATES][TEST_ELNA_GAIN_SCRIPT_SIZE] = 
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
// },

/*!-------------------------------------PORT NUM: 0 -----------------------------------*/
   {

     { 0x00 , 0x01  },  /* LNA Gain: 0 */
     { 0x02 , 0x03  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 1 -----------------------------------*/
   {

     { 0x04 , 0x05  },  /* LNA Gain: 0 */
     { 0x06 , 0x07  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 2 -----------------------------------*/
   {

     { 0x08 , 0x09  },  /* LNA Gain: 0 */
     { 0x0A , 0x0B  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 3 -----------------------------------*/
   {

     { 0x0C , 0x0D  },  /* LNA Gain: 0 */
     { 0x0E , 0x0F  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 4 -----------------------------------*/
   {

     { 0x10 , 0x11  },  /* LNA Gain: 0 */
     { 0x12 , 0x13  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 5 -----------------------------------*/
   {

     { 0x14 , 0x15  },  /* LNA Gain: 0 */
     { 0x16 , 0x17  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 6 -----------------------------------*/
   {

     { 0x18 , 0x19  },  /* LNA Gain: 0 */
     { 0x1A , 0x1B  },  /* LNA Gain: 1 */

   },

/*!-------------------------------------PORT NUM: 7 -----------------------------------*/
   {

     { 0x1C , 0x1D  },  /* LNA Gain: 0 */
     { 0x1E , 0x1F  },  /* LNA Gain: 1 */

   },

};


boolean get_elna_internal_test_settings(rfdevice_elna_settings_type *test_elna_settings)
{
  boolean status = TRUE;

  /* NULL Pointer Check*/
   if(test_elna_settings == NULL)
   {
      return FALSE;
   }

  /*==========================================================================*/
  /* BUS Config settings */
  /*==========================================================================*/

   test_elna_settings->elna_device_bus_cfg.extended_cmd = FALSE;
   test_elna_settings->elna_device_bus_cfg.settings.rd_delay=FALSE;
   test_elna_settings->elna_device_bus_cfg.settings.half_rate=FALSE;

  /*==========================================================================*/
  /* Low power mode trigger for sleep/wakeup */
  /*==========================================================================*/
   test_elna_settings->lpm_pm_trig_flag = (boolean)TEST_ELNA_LPM_PM_TRIGGER;

  /*==========================================================================*/
  /* COMMON SETTINGS */
  /*==========================================================================*/

   /*WAKEUP*/
   test_elna_settings->common_settings.elna_wakeup_script_size  = TEST_ELNA_WAKEUP_SCRIPT_SIZE;
   test_elna_settings->common_settings.elna_wakeup.addr         = (const uint16*)test_elna_wakeup_reg_list;
   test_elna_settings->common_settings.elna_wakeup.data         = (const int16*)test_elna_wakeup_data_list;

  /*SLEEP*/
   test_elna_settings->common_settings.elna_sleep_script_size   = TEST_ELNA_SLEEP_SCRIPT_SIZE;
   test_elna_settings->common_settings.elna_sleep.addr          = (const uint16*)test_elna_sleep_reg_list;
   test_elna_settings->common_settings.elna_sleep.data          = (const int16*)test_elna_sleep_data_list;

   /*TRIGGER*/
   test_elna_settings->common_settings.elna_trigger_script_size  = TEST_ELNA_TRIGGER_SCRIPT_SIZE;
   test_elna_settings->common_settings.elna_trigger.addr         = (const uint16*)test_elna_trigger_reg_list;
   test_elna_settings->common_settings.elna_trigger.data         = (const int16*)test_elna_trigger_data_list;


  /*==========================================================================*/
  /* PORT SETTINGS */
  /*==========================================================================*/
   test_elna_settings->num_ports = TEST_ELNA_NUM_PORTS;

   /*eLNA gain*/
   test_elna_settings->port_settings.elna_gain_script_size    = TEST_ELNA_GAIN_SCRIPT_SIZE;
   test_elna_settings->port_settings.elna_num_gain_states     = TEST_ELNA_NUM_GAIN_STATES;
   test_elna_settings->port_settings.elna_gain.addr           = (const uint16*)test_elna_gain_reg_list;
   test_elna_settings->port_settings.elna_gain.data           = (const int16*)(&(test_elna_gain_data_list[0][0][0]));     

  return status;  

}
