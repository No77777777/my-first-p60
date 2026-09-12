
/*===========================================================================


      R F C  C o m m o n  G S M  c o n t r o l   A P I

                            S o u r c e  F i l e

DESCRIPTION
  This file contains common functions for the RFC.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2011 - 2015 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rfc/common/src/rfc_common.cpp#1 $
  $DateTime: 2021/03/09 05:11:02 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/16   pk      Added check for grfc in the generate and init GRFC API
11/25/15   jr      NV support to enable/disable LPM feature
10/13/15   Saul    GNSS Blanking
09/20/15   fhuo    Add support for grfc switch for antenna sharing
07/24/15   zg      Fixed ADC cal
07/14/15   Saul    Debug messages.
06/22/15   Saul    Allow Multiple GPIO Sig Definitions
06/08/15   Saul    RFC Dynamic Properties
06/01/15   zhw     Added support for RFC preferred device query
04/30/15   Saul    CRAT Updates
02/24/15   aks     Device driver updates for ADC Vcm Cal 
02/20/15   px      Support for sending rffe scan efs dump to rf apps task
01/06/15   vv      Added API support to configure rfc for adc dc cal
03/03/15   vv      Added support for ADC VCM cal feature
11/07/14   ag      Removed redundant message in rfc_common_generate_grfc_script()
10/20/14   vb      ASDiv immediate script support
10/13/14   dr      Adding Featurization for Tabasco
08/15/14   kg      Framework to read RFC EEPROM and populate local structure.
07/18/14   Saul    Convert to using GPIO protection APIs.
05/23/14   vrb     Calling TLMM API to set TLMM_RFFE_CTL Slew Rates as prescribed
12/13/13   cc      Unify gpio type to rfc_sig_cfg_type for FDD and TDD tech
11/26/13   nrk     Fixed compiler warnings 
06/24/13   sar     Replaced memcpy calls with safe memscpy. 
05/29/13   spa     Added delay param to APIs for getting GRFC scripts
04/24/13   APU     If delay is of the special value of 0x7FFF, then just 
                   append the current GRFC transaction to the previous 
                   GRFC transaction with 0 relative delay.
04/02/13   cri     Remove unnecessary error messages
02/27/13   sr      GNSS concurrency fix with critical section lock for sleep and wakeup.
01/11/13   APU     If GRFCs are in config-only mode, skip appending the GRFC
                   scripts.
12/21/12   sr      Put the GRFC/GPIOs to rf-card init_state during wakeup.
12/11/12   hdz     Enabled timing support in rfc_common_get_grfc_overwritten_buffer 
12/09/12   zhw     Make GRFC overwritten API take RFC logic instead of boolean
11/07/12   sbm     API to extract timing information.
10/30/12   sr      Warnings fix
10/24/12   zhw     Support for generating GRFC script with init state
10/16/12   zhw     RF On/Off grfc scripting using overwritten rfc signal
10/11/12   dw      SVVZ bringup updates
10/02/12   vss     Add support for getting grfc script buffer based on signal type 
09/05/12   sr      fixed compiler warnings.
07/21/12   sr      removed the grfc rf engine references.
07/11/12   shb     Updated script_buffer to be of rf_buffer_intf* type 
                   instead of void ptr
07/09/12   sr      changes to use the rfc_data object instead of rfc_ag function ptr table.
07/05/12   sr      Added Err message if the signal type is GPIO while generating scripts
06/29/12   sr      RFC changes : use rf_hal_buffer to generate scripts 
06/29/12   sr      rfc api name change.
06/28/12   sr      dime rfc changes.
05/18/12   sb      made APIs to get grfc number from msm signal and signal type
05/16/12   zw      Refactored grfc script appending API
05/15/12   zw      Added API for appending grfc scripts. 
                   The API will memcopy the entries of source_script into dest_script
                   starting from the first empty slot. GRFC counts in dest_script
                   will also be updated
04/24/12   ac      Added support for rfc binary data
04/05/12   sr      made changes to put all the GPIOs into low power mode during sleep.
04/01/12   lcl     Removed empty mdsp interface header file 
03/26/12   vss     Add API to retrive GRFC engine output type 
03/16/12   aak     Change to base voltage scaling on WTR1605 process  
03/17/12   jfc     Remove deprecated mdsp_intf_v.h include
12/15/11   hdz     Configure GFRC in rfccommon_ini()
11/17/11   aak     Critical section lock for RFC 
11/17/11   sr      Add rfc_common_get_grfc_info()
10/17/11    sr     changed to initialize the grfc signal to default state based 
                   on the rfc-card init state setting info.
09/19/11   aro     Added callback function for NV write
08/26/11   sr      RFC V1.3.35 SS release changes. SS format has changed.
08/20/09   sr      added function to check for the rfc common init status.
07/29/11   Saul    IRAT update. Skip setting GPIOs during IRAT.
07/26/11   Saul    Changed common GRFC script gen API to skip processing GPIOs.
07/15/11   whc     IRAT GRFC script support for autogen RFC  
07/08/11   sr      Added new API to get the GRFC number from list of signals
06/23/11   vb      Fix the infinite loop in rfc_common_init_default()
03/25/11   sr      Initial version
============================================================================*/


/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include <stringl/stringl.h>

extern "C" {
#include "rflm_diag.h"
#include "comdef.h"
#include "rfc_common.h" 
#include "msg.h"
#include "rfcommon_msg.h"
#include "rf_hal_grfc.h"
#include "rf_buffer_intf.h"
#include "rfdevice_cmn_intf.h"
#include "rf_apps_task.h"
#include "rfc_class.h"
#include "rfm_umid_list.h"
#include "rfcommon_nv_mm.h"

/*Includes to read and populate 
the eeprom structure from smem*/
#include "smem.h"
//#include "rfc_class.h"
#include "rfc_wlan_data.h"
#include "rfdevice_physical_device.h"
#include "rfcommon_data.h"
#include "modem_mem.h"

}

extern "C" {

static DalDeviceHandle * rfc_tlmm_dal_handle = NULL;

/* rfc Signal info table for the specific RF CARD get initialized once during the init. */
rfc_signal_info_type *rfc_signal_info_table = NULL;
uint32 rfc_signals_num = 0;

/* MSM Signal info table for the Specific MSM, get initializd once during init */ 
rfc_msm_signal_info_type *rfc_msm_signal_info_table = NULL; 
uint32 rfc_msm_signals_num = 0;

} /* extern "C" */

/* Device process */
uint8 device_process[2] = {0,0};

/* this is to keep track of which tech's are active and which rf_path. 
 if all the techs in sleep state, we put all the rf signals into low power state.
 if any of the tech becomes active we need to put all the signals into enable state.
**/ 

typedef struct 
{
  int8 active_techs; /* this is to keep track of how many techs are active */ 
  uint32 vote_mask[RF_PATH_MAX];
} rfc_common_vote_type;

static rfc_common_vote_type rfc_common_vote_info = { 0, {0} };

static rf_rfc_eeprom_data_type rf_rfc_eeprom_data;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Maintains the information on the Common RFC Lock.
 
  @details
  This variable will be referenced through RFC code to acquire and release the locks.
  */ 
static rf_lock_data_type rfc_lock_data;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Callback function for NV Write

  @param wait_sig
  Signal to Wait
*/
void
rfc_common_write_nv_cb
(
  rex_sigs_type wait_sig
)
{
  /* Do rex_wait() on a signal */
  (void) rex_wait(wait_sig);

  return;
} /* rfc_common_write_nv_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function initializes the DAL handle.

  @details
  This function initializes the DAL handle.

  @param None
*/
DALResult rfc_common_init_dal_handle(void)
{
  DALResult dal_status = DAL_SUCCESS;

  /* Replacing bio with DAL equivalent*/
  dal_status = DAL_DeviceAttach(DALDEVICEID_TLMM, &rfc_tlmm_dal_handle);
  if(dal_status == DAL_SUCCESS)
  {
    (void) DalDevice_Open(rfc_tlmm_dal_handle,DAL_OPEN_SHARED);
  }

  return dal_status;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function actiavtes/de-activates the specified GRFC based on the specified signal logic.

  @details
  This function actiavtes/de-activates the specified GRFC based on the specified signal logic.

  @param grfc:  GRFC to be activated/de-activated.
  @param logic: Signal logic ( HIGH or LOW)
*/
boolean rfc_common_set_grfc(uint8 grfc, rfc_logic_type logic)
{
  rf_hal_bus_result_type result = RF_HAL_BUS_SUCCESS;
  boolean ret_status = TRUE;
#ifndef FEATURE_RFA_ATLAS_MODEM
  if(RFC_HIGH == logic)
  {
    result = rfhal_grfc_write_single(grfc, TRUE, NULL);
  }
  else if(RFC_LOW == logic)
  {
    result = rfhal_grfc_write_single(grfc, FALSE, NULL);
  }
  else if(logic != RFC_CONFIG_ONLY)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_common_set_grfc(%d, %d) Invalid GRFC logic!", grfc, logic);
    ret_status = FALSE;
  }
#endif
  if (result != RF_HAL_BUS_SUCCESS)
  {
    MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_common_set_grfc(%d, %d) failed!, result = %d", grfc, logic, result);
    ret_status = FALSE;
  }

  return ret_status;
}

#if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function sets the specified GPIO signals based on the specified signal logic. 

  @details
  This function sets the specified GPIO signals based on the specified signal logic. 

  @param dal_signal:  DAL Signal (GPIO) to be set.
  @param logic: Signal logic ( HIGH or LOW)
*/
boolean rfc_common_set_gpio(DALGpioSignalType dal_signal, rfc_logic_type logic)
{
  boolean ret_status = FALSE;

  if (rfc_tlmm_dal_handle == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_common_tlmm_dal_handle==NULL", 0);
    return FALSE;
  }

  if(RFC_HIGH == logic)
  {
    (void) DalTlmm_GpioOut(rfc_tlmm_dal_handle, dal_signal, DAL_GPIO_HIGH_VALUE);
    ret_status = TRUE;
  }
  else if(RFC_LOW == logic)
  {
    (void) DalTlmm_GpioOut(rfc_tlmm_dal_handle, dal_signal, DAL_GPIO_LOW_VALUE);
    ret_status = TRUE;
  }

  return ret_status;
}
#else
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function sets the specified GPIO signals based on the specified signal logic. 

  @details
  This function sets the specified GPIO signals based on the specified signal logic. 

  @param gpio_id:  GPIO ID.
  @param logic: Signal logic ( HIGH or LOW)
*/
boolean rfc_common_set_gpio( DALGpioIdType gpio_id, rfc_logic_type logic)
{
  boolean ret_status = FALSE;
  DALResult dal_status = DAL_SUCCESS;
  DALGpioValueType dal_gpio_io_val;

  if (rfc_tlmm_dal_handle == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_common_tlmm_dal_handle==NULL", 0);
    return FALSE;
  }

  if(RFC_HIGH == logic)
  {
    dal_gpio_io_val = DAL_GPIO_HIGH_VALUE;
    ret_status = TRUE;
  }
  else if(RFC_LOW == logic)
  {
    dal_gpio_io_val = DAL_GPIO_LOW_VALUE;
    ret_status = TRUE;
  }

  /* Config GPIO output state */
  if (ret_status == TRUE)
  {
    dal_status = DalTlmm_GpioIdOut( rfc_tlmm_dal_handle, gpio_id, dal_gpio_io_val );
    if(dal_status != DAL_SUCCESS)
    {
      ERR_FATAL( "rfc_common_set_gpio - DalTlmm_GpioIdOut api failed for GPIO id %d.",
                 gpio_id, 0, 0 );
      ret_status &= FALSE;
    }
  }

  return ret_status;
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function initilizes the requested rf signal into its default state. 

  @details
  This function initilizes the requested rf signal into its default state.

  @param rf_sig_name:  rfc signal to be set to default state. 
*/
void rfc_common_init_rfc_signal(int32 rf_sig_name)
{
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  rfc_logic_type init_state = RFC_LOGIC_NUM;

  if (rf_sig_name >= (int32)rfc_signals_num)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid RF signal (%d, %d)", rf_sig_name, rfc_signals_num);
    return;
  }

  /* make sure that the RF-CARD Signal info table has been initialized. which is done during init. */
  if ( !rfc_common_get_init_status() ) 
  {
    return;
  }

  msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */
  init_state = rfc_signal_info_table[rf_sig_name].init_state;

  if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GRFC)
  {
    rfc_common_set_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num, init_state);
  }
  else if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GPIO)
  {
    #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
    rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].dal_signal, init_state);
    #else
    rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].gpio_id, init_state);
    #endif
  }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  returns the GRFC Engine/RF Engine/GPIO signal number  for the corresponding rf_signal and signal
  type.

  @details
  returns the GRFC Engine/RF Engine/GPIO signal number  for the corresponding rf_signal and signal
  type.

  @param rf_sig_name: RF Signal for which GRFC Engine/RF Engine/GPIO number is requested.
  @param sig_type: Signal type for which GRFC Engine/RF Engine/GPIO number should be returned.
*/
int32 rfc_common_get_signal_num(int32 rf_sig_name, rfc_signal_type sig_type)
{
  boolean status = TRUE;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  int32 signal_num = -1;

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    return signal_num;
  }

  msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */

  if (rfc_msm_signal_info_table[msm_signal].signal_type == sig_type)
  {
    signal_num = rfc_common_get_grfc_num(rf_sig_name);
  }

  return signal_num;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  returns the grfc number from msm signal

  @details
  returns the grfc number from msm signal

  @param rfc_msm_signal_type: MSM Signal for which GRFC/GPIO number is requested.
*/
int32 rfc_common_get_grfc_num_of_msm_sig (rfc_msm_signal_type msm_signal) 
{

  int32 signal_num = -1;
  switch (rfc_msm_signal_info_table[msm_signal].output_type)
  {
  case RFC_GRFC:
    signal_num = rfc_msm_signal_info_table[msm_signal].grfc_num;
    break;

  case RFC_GPIO:
    /* configure the GPIO to the specified logic */
    signal_num = rfc_msm_signal_info_table[msm_signal].msm_gpio;
    break;

  default:
    break;
  }

  return signal_num;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  returns the grfc number from signal type

  @details
  returns the grfc number from signal type

  @param rfc_signal_type: RFC Signal type for which GRFC Engine/RF Engine/GPIO number is requested.
*/

int32 rfc_common_get_grfc_num_of_output_type(rfc_signal_type sig_type)
{
  int32 grfc_num = -1;
  int32 msm_sig_name;
  int8 success = 0;
  for ( msm_sig_name = 0; msm_sig_name < (int32)RFC_MSM_SIG_NUM; msm_sig_name++)
  {
    if (rfc_msm_signal_info_table[msm_sig_name].signal_type == sig_type )
    {
      success = 1;
      grfc_num = rfc_common_get_grfc_num_of_msm_sig ( (rfc_msm_signal_type) msm_sig_name );
    }
  }

  if (success == 0)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC Signal type: %d not found ", sig_type);
  }


  return grfc_num;  //get_grfc_num_of_msm_sig (sig_name)
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  returns the GRFC Engine/RF Engine/GPIO signal number  for the corresponding rf_signal.
  @details
  returns the GRFC Engine/RF Engine/GPIO signal number  for the corresponding rf_signal.

  @param rf_sig_name: RF Signal for which GRFC Engine/RF Engine/GPIO number is requested.

*/
int32 rfc_common_get_grfc_num(int32 rf_sig_name)
{
  boolean status = FALSE;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  int32 signal_num = -1;

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    return signal_num;
  }

  msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */
  signal_num = rfc_common_get_grfc_num_of_msm_sig (msm_signal);
  return signal_num;
}
/*----------------------------------------------------------------------------*/

/*!
  @brief
  returns the GRFC Engine output type  for the corresponding rf_signal.
  @details
  returns the GRFC Engine output type  for the corresponding rf_signal.

  @param rf_sig_name: RF Signal for which GRFC Engine/RF Engine/GPIO output type is requested.

*/
rfc_gpio_grfc_type rfc_common_get_grfc_output_type(int32 rf_sig_name)
{
  boolean status = FALSE;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    return RFC_GPIO_GRFC_INVALID;
  }

  msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */

  return rfc_msm_signal_info_table[msm_signal].output_type;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function checks whether the rfc common has ben initialized or not. If initialized returns
  TRUE.

  @param None.
*/
boolean rfc_common_get_init_status( void )
{
  boolean api_status = TRUE;

  /* make sure that the RF-CARD Signal info table has been initialized. which is done during init. */
  if ((rfc_signal_info_table == NULL) || (rfc_signals_num == 0)) 
  {
    RF_MSG( RF_ERROR, "rfc_common_init_status RFC ERROR CARD NOT INSTANTIATED. RF CANNOT RUN." );
    api_status &= FALSE;
  }

  /* make sure that the MSM Signal info table has been initialized. which is done during init. */ 
  if ((rfc_msm_signal_info_table == NULL) || (rfc_msm_signals_num == 0))
  {
    RF_MSG_1( RF_ERROR, "rfc_common_init_status RFC ERROR empty rfc_msm_signal_info_table msm num sig %d. RF CANNOT RUN.", 
              rfc_msm_signals_num);
    api_status &= FALSE;
  }

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Initializes the RFC common module. 

  @details
  This function configures the specified GRFC engines, GRFC RF engines and GPIO signals to their
  default state. Enables the specified GPIOs using DAL interface. This function will be called once
  per power-up.

  @param rfc_sig_table: RFC Signal info table, which will be used for the specific RF CARD.
  @param num_signals: Number of RF Signals used in the specified RF CARD.
*/
#if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
boolean rfc_common_init(rfc_signal_info_type *rfc_sig_table, int32 num_signals)
{
  DALResult dal_status = DAL_SUCCESS;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  uint16 i = 0;

  rfc_signal_info_table = rfc_sig_table; /* initialize the rfc_card_table and which will be used later */ 
  rfc_signals_num = num_signals;

  rfc_msm_signals_num = rfc_msm_sig_info_table_get(&rfc_msm_signal_info_table);

  if ( !rfc_common_get_init_status() )
  {
    return FALSE;
  }

  dal_status = rfc_common_init_dal_handle();
  if(dal_status != DAL_SUCCESS)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"DAL Device failed to attach!!!", 0);
    return FALSE;
  }

  //DalTlmm_SetPort(rfc_tlmm_dal_handle, DAL_TLMM_RFFE_CTL, 0x1);

  for (i = 0; i < rfc_signals_num ; i++)
  {

    msm_signal = rfc_signal_info_table[i].msm_sig_name;

    if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GRFC)
    {
      rfc_common_set_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num,
                                 rfc_signal_info_table[i].init_state);
    }

    if (rfc_msm_signal_info_table[msm_signal].msm_gpio != RFC_GPIO_INVALID)
    {
      /* construct the DAL Signal */
      rfc_signal_info_table[i].dal_signal = DAL_GPIO_CFG(rfc_msm_signal_info_table[msm_signal].msm_gpio, \
                                                         rfc_msm_signal_info_table[msm_signal].function_select, \
                                                         rfc_msm_signal_info_table[msm_signal].direction, \
                                                         rfc_signal_info_table[i].gpio_pull, \
                                                         rfc_signal_info_table[i].gpio_drive);
      /* Disable the DAL Signal  */
      (void) DalTlmm_ConfigGpio(rfc_tlmm_dal_handle, rfc_signal_info_table[i].dal_signal, DAL_TLMM_GPIO_DISABLE);
      }
  } /* end of for() */

  return TRUE;
}
#else
boolean rfc_common_init(rfc_signal_info_type *rfc_sig_table, int32 num_signals)
{
  boolean api_status = TRUE;
  DALResult api_dal_status = DAL_SUCCESS;
  DALResult loop_dal_status = DAL_SUCCESS;
  DALResult dal_status_temp = DAL_SUCCESS;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  rfc_msm_signal_type msm_signal_temp = RFC_MSM_SIG_NUM;
  uint16 i = 0;
  uint16 j = 0;
  DalTlmm_GpioConfigIdType dal_gpio_settings;
  boolean found = FALSE;

  rfc_signal_info_table = rfc_sig_table; /* initialize the rfc_card_table and which will be used later */ 
  rfc_signals_num = num_signals;

  rfc_msm_signals_num = rfc_msm_sig_info_table_get(&rfc_msm_signal_info_table);

  if ( !rfc_common_get_init_status() )
  {
    return FALSE;
  }

  api_dal_status = rfc_common_init_dal_handle();
  if(api_dal_status != DAL_SUCCESS)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC ERROR - DAL Device failed to attach!!!", 0);
    return FALSE;
  }

  //Removing from here; already called in rfcommon_msm_init
  //DalTlmm_SetPort(rfc_tlmm_dal_handle, DAL_TLMM_RFFE_CTL, 0x1);

  for (i = 0; i < rfc_signals_num; i++)
  {
    loop_dal_status = DAL_SUCCESS; //Using new status for each loop iter
    msm_signal = rfc_signal_info_table[i].msm_sig_name;

    if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GRFC)
    {
      rfc_common_set_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num,
                                 rfc_signal_info_table[i].init_state);
    }

    if (rfc_msm_signal_info_table[msm_signal].msm_gpio != RFC_GPIO_INVALID)
    {
      /* Get GPIO ID. Store GPIO ID. Also locks GPIO */
      if (loop_dal_status == DAL_SUCCESS)
      {
        dal_status_temp = DalTlmm_GetGpioId( rfc_tlmm_dal_handle, 
                                             rfc_msm_signal_info_table[msm_signal].tlmm_gpio_name, 
                                             &rfc_signal_info_table[i].gpio_id );
        if(dal_status_temp != DAL_SUCCESS)
        {
          MSG_3( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC GPIO[num %d/id %d] Get ID DAL error code %d. DUP check.",
                 rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id, dal_status_temp );
          // Since item failed, check for duplicate entry, if found, just init to already obtained ID
          found = FALSE;
          for (j = 0; j < i; j++) // Stop loop at num signals already processed
          {
            msm_signal_temp = rfc_signal_info_table[j].msm_sig_name;

            // A signal to be considered the same must be same GPIO # AND function select
            if( 
                (rfc_msm_signal_info_table[msm_signal].msm_gpio == rfc_msm_signal_info_table[msm_signal_temp].msm_gpio) 
                && (rfc_msm_signal_info_table[msm_signal].function_select == rfc_msm_signal_info_table[msm_signal_temp].function_select)
              )
            {
              // Set ID to already obtained ID
              rfc_signal_info_table[i].gpio_id = rfc_signal_info_table[j].gpio_id;
              found = TRUE;
              break;
            }
          } // for(j)

          // If duplicate item was not actually found, then mark as error
          if (found)
          {
            MSG_2( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC GPIO[num %d/id %d] DUP check OK. Setting to same ID.",
                   rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id);
          }
          else
          {
            loop_dal_status = dal_status_temp;
            api_status &= FALSE;
            api_dal_status = DAL_ERROR;
            MSG_2( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC GPIO[num %d/id %d] ERROR - DUP NOT found. Cannot continue.",
                   rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id);
          }
        }
      }

      /* Config GPIO Settings */
      if (loop_dal_status == DAL_SUCCESS)
      {
        dal_gpio_settings.eDirection = rfc_msm_signal_info_table[msm_signal].direction;
        dal_gpio_settings.eDriveStrength = rfc_signal_info_table[i].gpio_drive;
        dal_gpio_settings.ePull = rfc_signal_info_table[i].gpio_pull;

        loop_dal_status = DalTlmm_ConfigGpioId( rfc_tlmm_dal_handle, rfc_signal_info_table[i].gpio_id, 
                                                &dal_gpio_settings );
        if(loop_dal_status != DAL_SUCCESS)
        {
          MSG_3( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC GPIO[num %d/id %d] Config DAL Error code %d. Check GPIO settings.",
                 rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id, loop_dal_status);
          api_status &= FALSE;
          api_dal_status = DAL_ERROR;
        }
      }

      /* Config GPIO to Disabled state */
      if (loop_dal_status == DAL_SUCCESS)
      {
        loop_dal_status = DalTlmm_ConfigGpioIdInactive( rfc_tlmm_dal_handle, rfc_signal_info_table[i].gpio_id );
        if(loop_dal_status != DAL_SUCCESS)
        {
          MSG_3( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC GPIO[num %d/id %d] Inactive DAL Error code %d.",
                 rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id, loop_dal_status);
          api_status &= FALSE;
          api_dal_status = DAL_ERROR;
        }
      }
    } // if (rfc_msm_signal_info_table[msm_signal].msm_gpio != RFC_GPIO_INVALID)
  } /* end of for(i) */

  if (api_dal_status != DAL_SUCCESS)
  {
    RF_MSG( RF_ERROR,
            "RFC common_init - DAL GPIO API error(s) detected. Check immediately preceeding messages for details. Options: "
               " 1 - Check DAL database for missing GPIO num/function/name combo."
               " 2 - Check GPIO num/function/name combo validity in RFC."
               " 3 - Ensure the RF Card is not re-using this same GPIO for multiple signals since not allowed."
            " 4 - Ensure no other client has reserved this GPIO already.");
    ERR_FATAL( "RFC common_init - DAL GPIO API error(s) detected. Check immediately preceeding messages for details.", 0, 0, 0 );
  }

  return api_status;
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function Initializes the specified RF signals into their default state.

  @details
  This function Initializes the specified RF signals into their default state.

  @param rfc_signals: List of RF Signals which need to placed into their default state.
*/
boolean rfc_common_init_default(rfc_sig_cfg_type *rfc_signals)
{
  boolean status = TRUE;
  uint16 i = 0;
  int32 rf_sig_name = RFC_SIG_LIST_END;

  if (rfc_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_signals==NULL", 0);
    return FALSE;
  }

  /* loop through the signal list and configure them to specified default state */
  while (rfc_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rfc_signals[i].sig_name; /* get the RF-CARD Signal name */

    rfc_common_init_rfc_signal(rf_sig_name);

    i++;
  }

  return status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function configuers the specified RF signals to the specified state(HIGH/LOW).

  @details
  This function configuers the specified RF signals to the specified state(HIGH/LOW). It also setup
  the RFD routing table for the GRFC and RF GRFC engines.

  @param rf_signals: List of RF signals and the logic information to be configured.
*/
boolean
rfc_common_config_signal
(
  rfc_sig_cfg_type *rf_signals,
  boolean gnss_blanking_enabled
)
{
  uint16 i = 0;
  boolean status = TRUE;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
  int32 rf_sig_name = RFC_SIG_LIST_END;

  if (rf_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_signals==NULL", 0);
    return FALSE;
  }

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    return status;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (rf_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rf_signals[i].sig_name; /* get the RF-CARD Signal name */
    msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */

    if( (gnss_blanking_enabled == FALSE) && (msm_signal == RFC_MSM_INTERNAL_GNSS_BLANK) &&
         (rf_signals[i].start.logic == RFC_HIGH) )
    {
      i++;
      continue;
    }

//    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH,"rf_signals[i].sig_name i:%d signal Name:%d", i, rf_signals[i].sig_name);
    switch(rfc_msm_signal_info_table[msm_signal].output_type)
    {
    case RFC_GRFC:

      /* Set the specified GRFC Engine logic */
      status &= rfc_common_set_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num, 
                                           rf_signals[i].start.logic);
      break;

    case RFC_GPIO:
      /* configure the GPIO to the specified logic */
      #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
      status &= rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].dal_signal, rf_signals[i].start.logic);
      #else
      status &= rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].gpio_id, rf_signals[i].start.logic);
      #endif
      break;

    case RFC_SSBI: /* do nothing for now */
      break;

    default:
      break;
    }

    i++;
  }

  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function finds the particular signal in the list of signals and return the correspoding
  signal number (GRFC ENGINE NUM, GRFC RF _ENGINE NUM or GPIO NUM).

  @details
  This function finds the particular signal in the list of signals and return the correspoding
  signal number (GRFC ENGINE NUM, GRFC RF _ENGINE NUM or GPIO NUM).

  @param
  rf_signals: List of RF signals.
  sig_type : the Signal type for which the Signal number need to be returned.
*/
int32 rfc_common_get_grfc_engine_num(rfc_sig_cfg_type *rf_signals, rfc_signal_type sig_type)
{
  uint16 i = 0;
  int32 rf_sig_name = RFC_SIG_LIST_END;
  int32 signal_num = -1;

  if (rf_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_signals==NULL", 0);
    return signal_num;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (rf_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rf_signals[i].sig_name; /* get the RF-CARD Signal name */

    signal_num = rfc_common_get_signal_num(rf_sig_name, sig_type);
    if (signal_num >= 0)
    {
      break;
    }

    i++;
  }

  return signal_num;
}


/*! 
  @brief
  Maintains the information on the Common RFC Lock.
 
  @details
  Referenced through RFC code to acquire and release the locks.
  */ 
rf_lock_data_type* rfc_get_lock_data(void)
{
  return &rfc_lock_data;
} /* rfc_get_lock_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function finds the particular signal in the list of signals and return the correspoding
  signal information (GRFC ENGINE NUM, GRFC RF _ENGINE NUM or GPIO NUM).

  @details
  This function finds the particular signal in the list of signals and return the correspoding
  signal information (GRFC ENGINE NUM, GRFC RF _ENGINE NUM or GPIO NUM).

  @param
  rf_signals: List of RF signals.
  sig_type : the Signal type for which the Signal number need to be returned.
  rfc_grfc_list : signal information like GRFC Number, start/stop delta and logic, grfc output type.
*/
int32 rfc_common_get_grfc_info(rfc_sig_cfg_type *rf_signals, rfc_signal_type sig_type,
                               rfc_sig_cfg_type *rfc_grfc_list)
{
  uint16 i = 0;
  int32 rf_sig_name = RFC_SIG_LIST_END;
  int32 grfc_num = -1;
  int32 num_signals = 0;
  rfc_gpio_grfc_type rf_grfc_out_type;

  if (rf_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_signals==NULL", 0);
    return num_signals;
  }

  if (rfc_grfc_list == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfc_grfc_list==NULL", 0);
    return num_signals;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (rf_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rf_signals[i].sig_name; /* get the RF-CARD Signal name */

    grfc_num = rfc_common_get_signal_num(rf_sig_name, sig_type);

    /*get GRFC signal output type*/
    rf_grfc_out_type = rfc_common_get_grfc_output_type(rf_sig_name);

    if(rf_grfc_out_type >= RFC_GPIO_GRFC_INVALID)
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_LOW, 
            "Invalid GPIO_GRFC type: %d",rf_grfc_out_type);
    } 

    if (grfc_num >= 0)
    {
      rfc_grfc_list[num_signals].sig_name = grfc_num;
      rfc_grfc_list[num_signals].start.logic = rf_signals[i].start.logic;
      num_signals++;

      if(num_signals >= RFC_MAX_NUM_OF_GRFC)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
              "GRFC script exceed MAX count. count: %d, MAX: %d",num_signals,RFC_MAX_NUM_OF_GRFC);
        break;
      }

    }

    i++;
  }

  return num_signals;
}

/*----------------------------------------------------------------------------*/


/*!
  @brief
  This function finds the particular timing information from the timing table.

  @details
  This function finds the particular timing information from the timing table.

  @param
  device: device type.
  rf_band : Rf band 
  timing_type : tming signa
  rfc_timing : pointer to store the timing informatin for specific timing_type.
*/
boolean rfc_common_get_timing_info(rfc_timing_info_type *timing_info_tbl,
                                   rfc_timing_param_type timing_type,int32 *rfc_timing)
{
  uint16 i = 0;
 
  if (timing_type >= RFC_TIMING_PARAMS_NUM)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"timing_type Invalid", timing_type);
    return FALSE;
  }

  if (timing_info_tbl == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR,"timing info tbl is NULL");
    return FALSE;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (timing_info_tbl[i].timing_param != RFC_TIMING_PARAMS_NUM)
  {
    if (timing_info_tbl[i].timing_param == timing_type)
    {
      *rfc_timing = timing_info_tbl[i].timing;
      break;
    }
    i++;
  }

  return TRUE;
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  This function populates the rf_buffer with the specified rfc signal list.

  @details
  This function populates the rf_buffer with the specified rfc signal list.

  @param rfc_signal_list: List of RF signals.
  rf_buf_ptr : ptr to rf_buffer to store the rfc signals.
*/
boolean rfc_common_generate_grfc_script(rfc_sig_cfg_type *rfc_signal_list, 
                                        rf_buffer_intf *rf_buffer, int16 delay ) 
{
    uint8 sig_idx = 0;
    rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
    int32 rf_sig_name = RFC_SIG_LIST_END;

    if(rfc_signal_list == NULL || rf_buffer == NULL)
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
            "rfc_common_generate_grfc_script got NULL ptr. rfc_signal_list= %x, rf_buffer = %x",rfc_signal_list,rf_buffer);
      return FALSE;
    }

    /* For each signal in rfc_signal_list, look up GRFC engine number and then fill in grfc_buf_ptr.  */ 
    for(sig_idx = 0; rfc_signal_list[sig_idx].sig_name != RFC_SIG_LIST_END; sig_idx++ )
    {
      rf_sig_name = rfc_signal_list[sig_idx].sig_name; /* get the RF-CARD Signal name */      
      msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */
    
      /* MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," rfc_common_generate_grfc_script for loop entered",0); */ 
      
      if (rfc_signal_list[sig_idx].start.logic == RFC_LOW ||  rfc_signal_list[sig_idx].start.logic == RFC_HIGH)
      {
        if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GPIO)
        {
          /* configure the GPIO to the specified logic */
          #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
          rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].dal_signal, rfc_signal_list[sig_idx].start.logic);
          #else
          rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].gpio_id, rfc_signal_list[sig_idx].start.logic);
          #endif
          MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Found a GPIO signal. GPIO signals are expected when creating GRFC scripts!: Configuring the GPIO(%d) to %d", 
                rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_list[sig_idx].start.logic);
        }
        else if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GRFC)
        { /* for GRFCs put into to the script buffer */
          rf_buffer->append_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num, 
                                 rfc_signal_list[sig_idx].start.logic, delay ); 
        /*  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," rfc_common_generate_grfc_script append GRFC is called",0);  */
        }
	else
	{
	  /* For GRFC\GPIO_INVALID signals print error message */
          MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"Signal %d|%d is neither GPIO nor GRFC %d", 
                rf_sig_name, msm_signal, rfc_msm_signal_info_table[msm_signal].output_type);
	}
      }
    }

    return TRUE;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function populates the rf_buffer with the specified rfc signal list,
  in addition, the signals are all at default state.

  @details
  This function populates the rf_buffer with the specified rfc signal list.
  In addition, the signals are all at default state.

  @param rfc_signal_list
  List of RF signals.  
 
  @param rf_buf_ptr
  Ptr to rf_buffer to store the rfc signals.
*/
boolean rfc_common_generate_init_grfc_script(rfc_sig_cfg_type *rfc_signal_list, 
                                             rf_buffer_intf *rf_buffer, int16 delay) 
{
    uint8 sig_idx = 0;
    rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;
    int32 rf_sig_name = RFC_SIG_LIST_END;
    rfc_logic_type init_state = RFC_LOGIC_NUM;

    if(rfc_signal_list == NULL || rf_buffer == NULL)
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
            "rfc_common_generate_init_grfc_script got NULL ptr. rfc_signal_list= %x, rf_buffer = %x",rfc_signal_list,rf_buffer);
      return FALSE;
    }

    /* For each signal in rfc_signal_list, look up GRFC engine number and then fill in grfc_buf_ptr.  */ 
    for(sig_idx = 0; rfc_signal_list[sig_idx].sig_name != RFC_SIG_LIST_END; sig_idx++ )
    {
      rf_sig_name = rfc_signal_list[sig_idx].sig_name; /* get the RF-CARD Signal name */      
      msm_signal = rfc_signal_info_table[rf_sig_name].msm_sig_name; /* get the MSM Signal name */
      init_state = rfc_signal_info_table[rf_sig_name].init_state;

      if (rfc_signal_list[sig_idx].start.logic == RFC_LOW ||  rfc_signal_list[sig_idx].start.logic == RFC_HIGH)
      {
        if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GPIO)
        {
          /* configure the GPIO to the specified logic */
          #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
          rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].dal_signal, init_state);
          #else
          rfc_common_set_gpio(rfc_signal_info_table[rf_sig_name].gpio_id, init_state);
          #endif
          MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Found a GPIO signal. GPIO signals are expected when creating GRFC scripts!: Configuring the GPIO(%d) to %d",
                rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_list[sig_idx].start.logic);
        }
        else if (rfc_msm_signal_info_table[msm_signal].output_type == RFC_GRFC)
        { /* for GRFCs put into to the script buffer */
          if (init_state == RFC_LOW || init_state == RFC_HIGH)
          {
            rf_buffer->append_grfc(rfc_msm_signal_info_table[msm_signal].grfc_num, 
                                   init_state, delay ); 
          }
        }
	else
	{
	  MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"Signal %d|%d is neither GPIO nor GRFC %d", 
               rf_sig_name, msm_signal, rfc_msm_signal_info_table[msm_signal].output_type);
	}
		
      }
    }

    return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function keep track of the rfc voting based on the path and technologies.

  @details
  This function keep track of the votes based on path and techs. This voting information will be
  used to decide whethere to put all the RF Signals into low power mode or to be enabled (active
  mode).

  @param
  path: RF_PATH for which voting need to be done.
  mode : technology which is voting.
  vote : TRUE - cast vote; FALSE - decast vote.
  active_techs : number of techs active.
*/
int8 rfc_common_vote(rf_path_enum_type path, rfm_mode_enum_type mode, boolean vote, int8 active_techs)
{ 
  uint32 vote_mask = 0x0;

  vote_mask = rfc_common_vote_info.vote_mask[path];
  if (vote)
  {
    rfc_common_vote_info.vote_mask[path] |= (1U << mode);
    if (vote_mask != rfc_common_vote_info.vote_mask[path])
    {
      active_techs++;
    }
  }
  else
  {
    rfc_common_vote_info.vote_mask[path] &= ~(1U << mode);
    if (vote_mask != rfc_common_vote_info.vote_mask[path])
    {
      active_techs--;
    }
  }
#if 0
  MSG_4(MSG_SSID_RF, MSG_LEGACY_HIGH," RFC vote : path=%d mode=%d vote=%d active_techs=%d", path, mode, vote, active_techs);
#endif
  return active_techs;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function puts the rf signals(GPIOs) into active mode of operation if the signals are in low
  power mode.

  @details
  This function puts the rf signals(GPIOs) into active mode of operation if the signals are in low
  power mode. If the signals are already in active mode of operation then nothing will be done.
  It will also cast the RFC Vote for the requested path and tech.
 
  @param
  path: RF_PATH which is requesting the signals to be enabled.
  mode : technology which is requesting the signals to be enabled.
*/
#if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
boolean rfc_common_wakeup(rf_path_enum_type path, rfm_mode_enum_type mode)
{
  uint16 i = 0;
  boolean status = TRUE;
  DALResult result = DAL_SUCCESS;
  uint32 msm_gpio = 0;
  rfc_msm_signal_type msm_signal;
  int8 new_active_techs = 0;

  /* Protect the rfc sleep/wakeup functions with critical section */
  rf_common_enter_critical_section(rfc_get_lock_data());

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* cast the rfc vote */
  new_active_techs = rfc_common_vote(path, mode, TRUE, rfc_common_vote_info.active_techs);

  if (rfc_common_vote_info.active_techs != 0)
  {
    /* one of the tech is already active so, there is no need to enable the rf signals, so just
       account the new tech's vote and return */
    rfc_common_vote_info.active_techs = new_active_techs;
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* no tech was active before, so, we have to enable all the rf signals. */
  rfc_common_vote_info.active_techs = new_active_techs; /* active techs shoule be ONE here */

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Enabling the rf signals from low power mode (%d)", rfc_common_vote_info.active_techs);

  /* configure all the rf signals into active mode of operation */
  for (i = 0; i < rfc_signals_num ; i++)
  {

    msm_signal = rfc_signal_info_table[i].msm_sig_name;
    msm_gpio = rfc_msm_signal_info_table[msm_signal].msm_gpio;

    if ( msm_gpio != RFC_GPIO_INVALID )
    {
      /* Disable the DAL Signal, which will put the signal into inactive state.  */
      result = DalTlmm_ConfigGpio(rfc_tlmm_dal_handle, rfc_signal_info_table[i].dal_signal, DAL_TLMM_GPIO_ENABLE);
      if (result != DAL_SUCCESS)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Error while configuring dal_signal(0x%x) to wakeup: error %d", rfc_signal_info_table[i].dal_signal, result);
      }
      else
      {
        /* initialize the GRFC/GPIO state according to the default init_state*/
        rfc_common_init_rfc_signal((int32)i);
      }
    }
  }

  rf_common_leave_critical_section(rfc_get_lock_data());
  return status;
}
#else
boolean rfc_common_wakeup(rf_path_enum_type path, rfm_mode_enum_type mode)
{
  uint16 i = 0;
  boolean status = TRUE;
  DALResult result = DAL_SUCCESS;
  uint32 msm_gpio = 0;
  rfc_msm_signal_type msm_signal;
  int8 new_active_techs = 0;
  DalTlmm_GpioConfigIdType dal_gpio_settings;

  /* Protect the rfc sleep/wakeup functions with critical section */
  rf_common_enter_critical_section(rfc_get_lock_data());

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* cast the rfc vote */
  new_active_techs = rfc_common_vote(path, mode, TRUE, rfc_common_vote_info.active_techs);

  if (rfc_common_vote_info.active_techs != 0)
  {
    /* one of the tech is already active so, there is no need to enable the rf signals, so just
       account the new tech's vote and return */
    rfc_common_vote_info.active_techs = new_active_techs;
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* no tech was active before, so, we have to enable all the rf signals. */
  rfc_common_vote_info.active_techs = new_active_techs; /* active techs shoule be ONE here */

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Enabling the rf signals from low power mode (%d)", rfc_common_vote_info.active_techs);

  /* configure all the rf signals into active mode of operation */
  for (i = 0; i < rfc_signals_num ; i++)
  {

    msm_signal = rfc_signal_info_table[i].msm_sig_name;
    msm_gpio = rfc_msm_signal_info_table[msm_signal].msm_gpio;

    if ( msm_gpio != RFC_GPIO_INVALID )
    {
      /* Config GPIO Settings */
      if (status == TRUE)
      {
        dal_gpio_settings.eDirection = rfc_msm_signal_info_table[msm_signal].direction;
        dal_gpio_settings.eDriveStrength = rfc_signal_info_table[i].gpio_drive;
        dal_gpio_settings.ePull = rfc_signal_info_table[i].gpio_pull;

        result = DalTlmm_ConfigGpioId( rfc_tlmm_dal_handle, rfc_signal_info_table[i].gpio_id, &dal_gpio_settings );
        if(result != DAL_SUCCESS)
        {
          ERR_FATAL( "rfc_common_wakeup - DalTlmm_ConfigGpioId api failed for GPIO  [num %d, id %d]. "
                     "Check GPIO settings.",
                     rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id, 0);
          status &= FALSE;
        }
      }

      if (result != DAL_SUCCESS)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Error while configuring gpio_id(0x%x) to wakeup: error %d", rfc_signal_info_table[i].gpio_id, result);
      }
      else
      {
        /* initialize the GRFC/GPIO state according to the default init_state*/
        rfc_common_init_rfc_signal((int32)i);
      }
    }
  }

  rf_common_leave_critical_section(rfc_get_lock_data());
  return status;
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function wakes up the WLAN signals(GPIOs) and set the grfc logic used in WLAN to the initial state

  @details
  This function wakes up the WLAN signals(GPIOs) and set the grfc logic used in WLAN to the initial state

  @param
*/

boolean rfc_set_wlan_grfc_init_state(void)
{
  boolean status = TRUE;
  DALResult result = DAL_SUCCESS;
  DalTlmm_GpioConfigIdType dal_gpio_settings;
  int rfc_sig_name;
  uint32 msm_gpio;
  rfc_msm_signal_type msm_signal;
  rfc_cfg_params_type wlan_cfg; 
  rfc_sig_cfg_type *wlan_sig_ptr =NULL;
  rfc_wlan_data *rfc_wlan_data = rfc_wlan_data::get_instance();
  int i = 0;

  if (NULL != rfc_wlan_data) 
  {
    wlan_cfg.alternate_path = 0;
    wlan_cfg.req = RFC_REQ_DEFAULT_GET_DATA;
    rfc_wlan_data->sig_cfg_data_get(&wlan_cfg, &wlan_sig_ptr);
  }
  else
  {
    RF_MSG( RF_MED, "WLAN data ptr is null, will skip the WLAN grfc reset to initial state" );
    return status;
  }

  if (NULL != wlan_sig_ptr)
  {
    while ((rfc_sig_name = wlan_sig_ptr[i++].sig_name) != RFC_SIG_LIST_END) 
    { 
      msm_signal = rfc_signal_info_table[rfc_sig_name].msm_sig_name;
      msm_gpio = rfc_msm_signal_info_table[msm_signal].msm_gpio;
      if ( msm_gpio != RFC_GPIO_INVALID )
      {      
        #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
        result = DalTlmm_ConfigGpio(rfc_tlmm_dal_handle, rfc_signal_info_table[rfc_sig_name].dal_signal, DAL_TLMM_GPIO_ENABLE);
        if (result != DAL_SUCCESS)
        {
          ERR_FATAL("Error while configuring dal_signal(0x%x) to wakeup: error %d", rfc_signal_info_table[rfc_sig_name].dal_signal, result,0);
          status &=FALSE;
        }
        #else
        /* Config GPIO Settings */
        dal_gpio_settings.eDirection = rfc_msm_signal_info_table[msm_signal].direction;
        dal_gpio_settings.eDriveStrength = rfc_signal_info_table[rfc_sig_name].gpio_drive;
        dal_gpio_settings.ePull = rfc_signal_info_table[rfc_sig_name].gpio_pull;
        result = DalTlmm_ConfigGpioId( rfc_tlmm_dal_handle, rfc_signal_info_table[rfc_sig_name].gpio_id, &dal_gpio_settings );
        if(result != DAL_SUCCESS)
        {
          ERR_FATAL( "rfc_wlan_wakeup - DalTlmm_ConfigGpioId api failed for GPIO  [num %d, id %d]. "
                     "Check GPIO settings.",
                     rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[rfc_sig_name].gpio_id, 0);
          status &=FALSE;
        }
        #endif
      }
    }
    status &=rfc_common_config_signal(wlan_sig_ptr, TRUE);
  }
  else
  {
    RF_MSG( RF_MED, "WLAN signal ptr is null, no WLAN grfc signal is reset to initial state" );   
  }
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function puts the rf signals(GPIOs) into low power mode of operation if the signals are in
  active mode and none of the other techs are active.

  @details
  This function puts the rf signals(GPIOs) into low power mode of operation if the signals are in
  active mode and no other techs are active, then all the rf signals will be configured to low power
  mode of operation. If the signals are already in low power mode of operation then nothing will be
  done. It will also de-cast the RFC Vote for the requested path and tech.
 
  @param
  path: RF_PATH which is requesting the signals to be put into active mode.
  mode : technology which is requesting the signals to be put into active mode.
*/
boolean rfc_common_sleep(rf_path_enum_type path, rfm_mode_enum_type mode)
{
  boolean status = TRUE;
  uint16 i = 0;
  DALResult result = DAL_SUCCESS;
  uint32 msm_gpio;
  rfc_msm_signal_type msm_signal;
  int8 new_active_techs = 0;

  /* Protect the rfc sleep/wakeup functions with critical section */
  rf_common_enter_critical_section(rfc_get_lock_data());

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* de-cast the rfc-vote */
  new_active_techs = rfc_common_vote(path, mode, FALSE, rfc_common_vote_info.active_techs);


  if (rfc_common_vote_info.active_techs == new_active_techs || new_active_techs != 0)
  {
    // if there is no change in state,  nothing todo. OR, new_active_techs is NOT ZERO,
    // means another tech is still active so, don't disable rf signals.
    rfc_common_vote_info.active_techs = new_active_techs;
    rf_common_leave_critical_section(rfc_get_lock_data());
    return status;
  }

  /* if we are here means that none of the other techs are active so, it is safe to put all the rf
     signals into low power mode */

  rfc_common_vote_info.active_techs = new_active_techs; /* the active techs should be ZERO here */

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Putting rf signals to low power mode (%d)", rfc_common_vote_info.active_techs);

  #if defined(FEATURE_DIMEPM_MODEM) || defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM)
  for (i = 0; i < rfc_signals_num ; i++)
  {
    msm_signal = rfc_signal_info_table[i].msm_sig_name;
    msm_gpio = rfc_msm_signal_info_table[msm_signal].msm_gpio;

    if ( msm_gpio != RFC_GPIO_INVALID)
    {
      /* Disable the DAL Signal, which will put the signal into inactive state.  */
      result = DalTlmm_ConfigGpio(rfc_tlmm_dal_handle, rfc_signal_info_table[i].dal_signal, DAL_TLMM_GPIO_DISABLE);
      if (result != DAL_SUCCESS)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Error while configuring dal_signal(0x%x) into sleep: error %d", rfc_signal_info_table[i].dal_signal, result);
      }
    }
  }
  #else
  /* put all the rf signals into low power mode */
  for (i = 0; i < rfc_signals_num ; i++)
  {
    msm_signal = rfc_signal_info_table[i].msm_sig_name;
    msm_gpio = rfc_msm_signal_info_table[msm_signal].msm_gpio;

    if ( msm_gpio != RFC_GPIO_INVALID)
    {
      /* Disable the DAL Signal, which will put the signal into inactive state.  */

      /* Config GPIO to Disabled state */
      if (status == TRUE)
      {
        result = DalTlmm_ConfigGpioIdInactive( rfc_tlmm_dal_handle, rfc_signal_info_table[i].gpio_id );
        if(result != DAL_SUCCESS)
        {
          ERR_FATAL( "rfc_common_sleep - DalTlmm_ConfigGpioIdInactive api failed for GPIO [num %d, id %d].",
                     rfc_msm_signal_info_table[msm_signal].msm_gpio, rfc_signal_info_table[i].gpio_id, 0);
          status &= FALSE;
        }
      }

      if (result != DAL_SUCCESS)
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Error while configuring gpio_id(0x%x) into sleep: error %d", rfc_signal_info_table[i].gpio_id, result);
      }
    }
  }
  /*Put WLAN grfc signals to the init state*/
  status &= rfc_set_wlan_grfc_init_state();
  #endif

  rf_common_leave_critical_section(rfc_get_lock_data());
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Append dest_script to source_script if there is enough space available in
  source_script
  
  @param *dest_script
  Destination script that stores the final appending result
  @param *source_script
  Source script that all the contents are copied from

  @return
  TRUE if append operation is successful. Otherwise return FALSE.
*/
boolean
rfc_common_append_grfc_scripts
(
  rfc_grfc_script_type *dest_script,
  rfc_grfc_script_type *source_script
)
{
  boolean success;
  if( source_script == NULL || dest_script == NULL )
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_common_append_grfc_scripts: "
         "Null pointer passed in" );
    success = FALSE;
  }
  else if( (dest_script->num_grfc + source_script->num_grfc) > RFC_MAX_NUM_OF_GRFC )
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_common_append_grfc_scripts: "
           "Exceeded max GRFC num: ", dest_script->num_grfc + source_script->num_grfc );
    success = FALSE;
  }
  else
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_LOW, "rfc_common_append_grfc_scripts: "
           "GRFC num in script: ", dest_script->num_grfc + source_script->num_grfc );
    memscpy( &(dest_script->grfc_buffer[ dest_script->num_grfc ]),
            source_script->num_grfc * sizeof( rfc_grfc_script_entry_type ),
            &(source_script->grfc_buffer[0]),
            source_script->num_grfc * sizeof( rfc_grfc_script_entry_type ) );

    dest_script->num_grfc += source_script->num_grfc;
    success = TRUE;
  }

  return success;
} /* rfc_common_append_grfc_scripts() */


boolean rfc_read_eeprom()
{
  uint32 rf_eeprom_data_size = 0;
  void* rf_eeprom_info_smem_ptr = NULL; 
  boolean rf_eeprom_data_integrity_success = FALSE;
  
  rf_eeprom_info_smem_ptr = (void *)smem_get_addr( SMEM_RF_EEPROM_DATA, 
                                                    &rf_eeprom_data_size );
  if( rf_eeprom_info_smem_ptr != NULL)
  {
     //We are ignoring rf_eeprom_data_size since we want to copy only upto struct size
     memscpy(&rf_rfc_eeprom_data, sizeof(rf_rfc_eeprom_data_type),
              rf_eeprom_info_smem_ptr, sizeof(rf_rfc_eeprom_data_type));

     if (rf_rfc_eeprom_data.rf_rfc_eeprom_magic_num == RFC_EEPROM_MAGIC_NUM) {
        rf_eeprom_data_integrity_success = TRUE;
     } else {
        MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"EEPROM integrity check failed", 0);
     }

  } else {
     MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_eeprom_info_smem_ptr==NULL. EEPROM not present or couldn't read", 0);
  }

  return rf_eeprom_data_integrity_success;
} /* rfc_read_eeprom()*/


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function finds the particular signal in the list of signals and populates corresponding
  signals in the script_buffer

  @details
  This function finds the particular signal in the list of signals and populates corresponding
  signals in the script_buffer
 
  @param
  rf_signals: List of RF signals
  sig_type : the Signal type for which the Signal number need to be returned
  script : buffer to populate the grfc signals
 
  @return
  num_signals: number of signals of the selected type present in the sig list
*/
int32 
rfc_common_get_grfc_buffer
(
  rfc_sig_cfg_type *rf_signals, 
  rfc_signal_type sig_type,
  rf_buffer_intf* script
)
{
  uint16 i = 0;
  int32 rf_sig_name = RFC_SIG_LIST_END;
  int32 grfc_num = -1;
  int32 num_signals = 0;
  rfc_gpio_grfc_type rf_grfc_out_type;

  if (rf_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_signals==NULL", 0);
    return num_signals;
  }

  if (script == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," grfc buffer script==NULL", 0);
    return num_signals;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (rf_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rf_signals[i].sig_name; /* get the RF-CARD Signal name */

    grfc_num = rfc_common_get_signal_num(rf_sig_name, sig_type);

    /*get GRFC signal output type*/
    rf_grfc_out_type = rfc_common_get_grfc_output_type(rf_sig_name);

    if(rf_grfc_out_type != RFC_GRFC)
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR," Expecting signal of GRFC output type. "
                    "found output type: %d, signal name: %d ", rf_grfc_out_type, rf_sig_name);
    }
    else
    {
      if (grfc_num >= 0 )
      {
        if (script->append_grfc(grfc_num, rf_signals[i].start.logic, 0, FALSE))
        {
          num_signals++;
        }
        else
        {
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"populate script buffer failed:"
                                               " sig_type: %d", sig_type);
        }        
      }
    }
    i++;
  }

  return num_signals;
}
rfc_signal_type rfc_common_get_signal_type(int rfc_signal_name)
{
  boolean status = FALSE;
  rfc_msm_signal_type msm_signal = RFC_MSM_SIG_NUM;

  /* make sure that the rfc common has been initialized */
  status = rfc_common_get_init_status();
  if (!status) 
  {
    return RFC_SIG_TYPE_INVALID;
  }

  msm_signal = rfc_signal_info_table[rfc_signal_name].msm_sig_name; /* get the MSM Signal name */

  return rfc_msm_signal_info_table[msm_signal].signal_type;
}

/*----------------------------------------------------------------------------*/
/*!
  @details
  This function accepts a table of signals (rf_signals), a signal_type 
  (sig_type), the logic for the signal type (HIGH / LOW), HAL buffer (script)  
  and a time delay in micro-secs.
 
  The functionality of this API is to update the HAL buffer with entries 
  (GRFC_number) from the rf_signals table that match a given signal_type. In 
  additon, the time field for the first signal that matches the sig_type will be 
  updated in the HAL buffer (with time set to "delay" arg)
 
  For Ex:
  if rf_signals[] = {
                     PA_ON           , HIGH,
                     TX_ON           , LOW ,
                     Ant_sel_00      , HIGH,
                     Ant_sel_01      , HIGH,
                     Ant_sel_02      , LOW ,
                     RFC_SIG_LIST_END, LOW
                    }
 
  if the arg "sig_type" is Ant_sel, and arg "delay" is -90us
 
  This API looks for the GRFC numbers associated with Ant_sel_xx - say 
  GRFC_num(ant_sel_xx)

  The HAL buffer will be updated as follows: << GRFC area of the buffer >>
  {
    GRFC_num (Ant_sel_00), -90
    GRFC_num (Ant_sel_00),   0 <<--- note that the "delay" needs to be added
    GRFC_num (Ant_sel_00),   0       only for the first occurence of sig_type..
    GRFC_num (Ant_sel_00),   0       the timings for the rest are relative to 
                                     the first one
  }
 
  @param rf_signals
  List of RF signals
 
  @param sig_type
  The Signal type for which the Signal number need to be returned (Ant_sel, 
  TX_ON, PA_ON, etc)
 
  @param logic
  The logic to be overwritten (HIGH/LOW)
 
  @param script
  HAL Buffer to populate the grfc signals and the timings associated with them
 
  @param delay
  time delay in mirosecs
 
  @return
  num_signals: number of signals of the selected type present in the sig list
*/
int32 
rfc_common_get_grfc_overwritten_buffer
(
  rfc_sig_cfg_type *rf_signals, 
  rfc_signal_type sig_type,
  rfc_logic_type logic,
  rf_buffer_intf* script,
  int16 delay
)
{
  uint16 i = 0;
  int32 rf_sig_name = RFC_SIG_LIST_END;
  int32 grfc_num = -1;
  int32 num_signals = 0;
  rfc_gpio_grfc_type rf_grfc_out_type;

  if (rf_signals == NULL) /* tech specific rf signals to be configured */
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," rf_signals==NULL", 0);
    return num_signals;
  }

  if (script == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," grfc buffer script==NULL", 0);
    return num_signals;
  }

  /* loop through the signal list and configure them based on their type to specified logic  */
  while (rf_signals[i].sig_name != RFC_SIG_LIST_END)
  {
    rf_sig_name = rf_signals[i].sig_name; /* get the RF-CARD Signal name */

    grfc_num = rfc_common_get_signal_num(rf_sig_name, sig_type);

    /*get GRFC signal output type*/
    rf_grfc_out_type = rfc_common_get_grfc_output_type(rf_sig_name);

    if(rf_grfc_out_type != RFC_GRFC)
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR," Expecting signal of GRFC output type. "
                    "found output type: %d, signal name: %d ", rf_grfc_out_type, rf_sig_name);
    }
    else
    {
      if (grfc_num >= 0 )
      {
        if ( ( num_signals == 0 ) &&                /* The first transaction appends non-zero delay as new time ref*/   
             ( delay != RFDEVICE_NO_TIME_OFFSET ) ) /* AND if delay is not RFDEVICE_NO_TIME_OFFSET */
        {
          if (script->append_grfc(grfc_num, logic, delay, TRUE))
          {
            num_signals++;
          }
          else
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"populate script buffer failed:"
                  " sig_type: %d", sig_type);
          }      
        }
        else  /* The rest of transactions append zero delay relative to previous transaction*/
        {
          if (script->append_grfc(grfc_num, logic, 0, FALSE))
          {
            num_signals++;
          }
          else
          {
            MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"populate script buffer failed:"
                                                 " sig_type: %d", sig_type);
          }               
        }
      } /* if (grfc_num >= 0 ) */
                 
    } /* if(rf_grfc_out_type != RFC_GRFC) */
    i++;

  } /* while (rf_signals[i].sig_name != RFC_SIG_LIST_END) */

  return num_signals;
}

#if (defined (FEATURE_THOR_MODEM))
boolean rfc_set_fbrx_mapping( rfc_cfg_params_type       *cfg,
                              rfc_device_info_type *device_info_ptr
                             )
{
  boolean status = TRUE;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  uint8 i =0;
  if (NULL == cfg || NULL == device_info_ptr)
  {
    RF_MSG(RF_ERROR,"rfc_set_fbrx_mapping(): Invalid parameters passed.returning FALSE");
    return FALSE;
  }

  if (rfc_cmn == NULL)
  {
    RF_MSG(RF_ERROR,"rfc_set_fbrx_mapping(): RFC CMN Object is NOT Created");
    return FALSE;
  }

  while (i < device_info_ptr->num_devices)
  {
    if ((device_info_ptr->rf_asic_info[i].device_type == RFDEVICE_TRANSCEIVER) && 
        (device_info_ptr->rf_asic_info[i].device_id != WTR4905) && 
        (device_info_ptr->rf_asic_info[i].device_id != WTR4905V100))
    {
      if(!rfc_cmn->set_fbrx_phy_path_adc_mapping(
                                                device_info_ptr->modem_chain,
                                                device_info_ptr->rf_asic_info[i].phy_path_num,
                                                device_info_ptr->rf_asic_info[i].instance
                                                )
         )
      {
               RF_MSG_3( RF_ERROR, "rfc_set_fbrx_mapping():rfc_cmn->set_fbrx_phy_path_adc_mapping() failed. check rfc"
                                    "modem_chain=%d,phy_path_num=%d, trx logical instance=%d  ",
                                    device_info_ptr->modem_chain, 
                                    device_info_ptr->rf_asic_info[i].phy_path_num,
                                    device_info_ptr->rf_asic_info[i].instance
                                   );
               status &= FALSE;
      }
    }

    i++;

  }

  return status;
}
#endif


void rfc_common_rffe_scan_efs_dump_cmd_put(void)
{
  /*Holds details about rf apps task command*/
  rf_apps_cmd_type* rffe_scan_cmd = NULL;

  /* Add command to RF Apps task to dump device info to EFS */
  /* Get the pointer to the free buffer of RF command queue. */
  if ( ( rffe_scan_cmd = rf_apps_get_buf() ) == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_rffe_bus_scan: Cmd queue is full" );
  }
  else
  {
    rffe_scan_cmd->msgr_hdr.id = RFA_RF_COMMON_MSG_ID_RFFE_SCAN_EFS_DUMP;
    rffe_scan_cmd->rex_hdr.cmd_hdr.tech_id = RF_APPS_COMMON_ID;
    

    /* place RFC_APPS_TASK_RFFE_SCAN_CMD to RF command queue */
    rf_apps_cmd_put( rffe_scan_cmd );
  }
}

void rfc_common_rffe_scan_efs_dump(void* cmd_ptr, rf_dispatch_cid_info_type* cid_info, void* cb_data)
{
  rfc_intf* rfc_ptr = rfc_intf::get_instance();
  
  if ( rfc_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "rfc_apps_task_cmd_dispatcher: NULL rfc ptr!");
    return ;
  }

  rfc_ptr->rfc_rffe_bus_scan();
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is used to enable/disable  trx rx chain mapped to the requested modem chain(ADC)

  @details
  This function is used to enable/disable  wtr rx chain mapped to the requested modem chain(ADC)
 
  @param rx_chain_enable
  TRUE to enable and FALSE to disable the rx chain
 
  @param rx_modem_chain
  to enable the requested modem chain
 
  @return
  TRUE for success and FALSE for failure
*/
boolean rfc_config_adc_dc_cal(
                                 boolean rx_chain_enable,       /*Enable/disable trx rx chain*/
                                 uint32  rx_modem_chain        /*modem chain to map the ADC*/                        
                               )
{
  uint32 trx_logical_instance = 0xFF;
  uint32 phy_path_num = 0xFF;
  rfdevice_class* trx_obj_p = NULL;
  rfc_intf *rfc_cmn = rfc_intf::get_instance();
  rfdevice_rx_vco_path_enum_type vco_path = WTR_RX_INVALID_VCO_PATH;

  if (rfc_cmn == NULL)
  {
    RF_MSG(RF_ERROR,"rfc_config_adc_dc_cal(): RFC CMN Object is NOT Created");
    return FALSE;
  }

  /*parameter validity check*/
  if (rx_modem_chain >= RXLM_CHAIN_MAX)
  {
    RF_MSG_2(RF_ERROR, "rfc_config_adc_dc_cal(): invalid parameters passed.  rx_modem_chain=%d, rx_chain_enable= %d", 
                        rx_modem_chain,rx_chain_enable);
    return FALSE;  
  }
 
  /*get the phy path and trx logical instance for the requested modem_chain*/
  if(!rfc_cmn->get_trx_phy_path_adc_mapping(
                                            rx_modem_chain, 
                                            &phy_path_num,
                                            &trx_logical_instance,
                                            &vco_path
                                           )
    )
  {

    RF_MSG(RF_ERROR,"rfc_config_adc_dc_cal(): failed to get trx to phy_path,vco, adc mapping ");
    return FALSE;

  }

  /*Get the device object*/
  trx_obj_p = rfc_cmn->get_cmn_rf_device_object(RFDEVICE_TRANSCEIVER, trx_logical_instance);

  if (trx_obj_p == NULL)
  {
    RF_MSG_2(RF_ERROR,"rfc_config_adc_dc_cal(): returned NULL for device type = %d, trx_logical instance = %d",RFDEVICE_TRANSCEIVER,trx_logical_instance);
    return FALSE;
  }

  if (rx_chain_enable)
  {  /*!Turn ON the WTR*/

     if(rfdevice_cmn_power_up ((rfdevice_rxtx_common_class *)trx_obj_p,
                       RFDEV_VOTE_ON,
                       RFDEV_CMN_NORMAL_VOTING) != RFDEV_ON)
     {
       RF_MSG_2(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to turn ON the WTR "
                         "device type = %d, trx_logical instance = %d",RFDEVICE_TRANSCEIVER,trx_logical_instance);
       return FALSE;
     }

 if(rfcommon_nv_get_lpm_feature_enabled())
 {
     if(rfdevice_cmn_power_lpm (
        (rfdevice_rxtx_common_class *)trx_obj_p, TRUE)!= RFDEV_ON)
     {
       RF_MSG_1(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to power on from LPM"
                         "trx_logical instance = %d", trx_logical_instance);
       return FALSE;
     }
 }


#if (defined (FEATURE_THOR_MODEM))
     if (rx_modem_chain == RXLM_CHAIN_FBRX)
     {
       if(!rfdevice_cmn_fbrx_adc_vcm_cal_setup((rfdevice_rxtx_common_class*)trx_obj_p))/*Configures  WTR Rx chain for BBRx DC Cal for FBRx*/
     {
         RF_MSG_2(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to setup Rx chain for FBRx DC cal "
                         "device type = %d, trx_logical instance = %d",RFDEVICE_TRANSCEIVER,trx_logical_instance);
       return FALSE;
     }
     }
     else
     {
       if(!rfdevice_cmn_rx_adc_vcm_cal_setup((rfdevice_rxtx_common_class*)trx_obj_p,vco_path ))/*Configures  WTR Rx chain for BBRx DC Cal */
       {
         RF_MSG_3(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to setup Rx chain for BBrx DC cal "
                           "device type = %d, trx_logical instance = %d, vco=%d",RFDEVICE_TRANSCEIVER,trx_logical_instance,vco_path);
         return FALSE;
       }
     }
#else
     if(!rfdevice_cmn_rx_adc_vcm_cal_setup((rfdevice_rxtx_common_class*)trx_obj_p,vco_path ))/*Configures  WTR Rx chain for BBRx DC Cal */
     {
       RF_MSG_3(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to setup Rx chain for BBrx DC cal "
                           "device type = %d, trx_logical instance = %d, vco=%d",RFDEVICE_TRANSCEIVER,trx_logical_instance,vco_path);
       return FALSE;
     }
#endif

  } 
  else
  {  /*!Turn OFF the WTR*/

if(rfcommon_nv_get_lpm_feature_enabled())
{
     if(rfdevice_cmn_power_lpm (
        (rfdevice_rxtx_common_class *)trx_obj_p, FALSE)!= RFDEV_ON)
     {
       RF_MSG_1(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to power off from LPM"
                         "trx_logical instance = %d", trx_logical_instance);
       return FALSE;
     }
}   

     if(rfdevice_cmn_power_up ((rfdevice_rxtx_common_class *)trx_obj_p,
                       RFDEV_VOTE_OFF,
                       RFDEV_CMN_NORMAL_VOTING) != RFDEV_OFF)
     {
       RF_MSG_2(RF_ERROR,"rfc_config_adc_dc_cal(): Failed to turn OFF the WTR "
                         "device type = %d, trx_logical instance = %d",RFDEVICE_TRANSCEIVER,trx_logical_instance);
       return FALSE;
     }
   }


  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is used to get the list of valid ADCs

  @details
  This function is used to get the list of valid ADCs
 
  @param rx_chain_enable
  TRUE to enable and FALSE to disable the rx chain
 
  @param rx_modem_chain
  to enable the requested modem chain
 
  @return
  TRUE for success and FALSE for failure
*/
boolean rfc_get_valid_adc_list( uint32* adc_list )
{

  rfc_intf *rfc_cmn = rfc_intf::get_instance();

  if (adc_list == NULL)
  {
    RF_MSG(RF_ERROR,"rfc_get_valid_adc_list(): adc_list pointer is NULL. returning FALSE");
    return FALSE;
  }

  if (rfc_cmn == NULL)
  {
    RF_MSG(RF_ERROR,"rfc_get_valid_adc_list(): RFC CMN Object is NOT Created");
    return FALSE;
  }

  /*get the valid adc list mapping*/
  if(!rfc_cmn->get_valid_adc_list(adc_list))
  {

    RF_MSG(RF_ERROR,"rfc_get_valid_adc_list(): failed to get valid adc list mapping ");
    return FALSE;

  }
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief

  This function returns the preferred associated rx device for the given device.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.

  @retval
  Associated rx device.
*/
rfm_device_enum_type
rfc_common_get_preferred_associated_rx_device
(
  rfm_device_enum_type device
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_preferred_associated_rx_device: NULL rfc cmn intf." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_intf_ptr->rfc_get_preferred_associated_device( device, RFC_CONFIG_RX );
}/* rfc_common_get_preferred_associated_rx_device */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred associated tx device for the given device.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.

  @retval
  Associated tx device.
*/
rfm_device_enum_type
rfc_common_get_preferred_associated_tx_device
(
  rfm_device_enum_type device
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_preferred_associated_tx_device: NULL rfc cmn intf." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_intf_ptr->rfc_get_preferred_associated_device( device, RFC_CONFIG_TX );
}/* rfc_common_get_preferred_associated_tx_device */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred rx device listed for the given sys_band.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.

  @retval
  Associated tx device.
*/
rfm_device_enum_type
rfc_common_get_preferred_rx_device
(
  sys_band_class_e_type sys_band
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_preferred_rx_device: NULL rfc cmn intf." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_intf_ptr->rfc_get_preferred_device( sys_band, RFC_CONFIG_RX );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred tx device listed for the given sys_band.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.

  @retval
  Associated tx device.
*/
rfm_device_enum_type
rfc_common_get_preferred_tx_device
(
  sys_band_class_e_type sys_band
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_preferred_tx_device: NULL rfc cmn intf." );
    return RFM_INVALID_DEVICE;
  }

  return rfc_intf_ptr->rfc_get_preferred_device( sys_band, RFC_CONFIG_TX );
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function checks whether a sys_band is listed in the band_supported
  fields in RFC

  @details
  If an invalid input is provided, it returns FALSE.
 
  @param sys_band
  Sys Band enum for a given tech/band
 
  @param device
  RFM Device to check against

  @retval
  TRUE if supported. FALSE if otherwise
*/
boolean
rfc_common_check_band_supported
(
  sys_band_class_e_type sys_band,
  rfm_device_enum_type device
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_check_band_supported: NULL rfc cmn intf." );
    return FALSE;
  }

  return rfc_intf_ptr->rfc_check_band_supported( sys_band, device );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function remaps the old device represenations in ftm to the new physical device scheme.

  @details
  This API is to be used only in FTM to get the equivalent device mapping from
  the new physical device mappings.
 
  @param source_device_info
  Contains the old device information for which to get equivalent new re-mapping
 
  @param remapped_device_info
  Contains the re-mapped device information

  @retval
  TRUE means provided remapped info should be deemed usable/valid.
  FALSE means an internal API failure. Provided remapped info should be deemed invalid.
*/
boolean
rfc_common_get_remapped_device_info
(
   rfc_cal_device_remap_info_type *source_device_info,
   rfc_cal_device_remap_info_type *remapped_device_info
)
{
  boolean api_status = TRUE;
  rfc_intf *rfc_intf_ptr = NULL;

  if ( (source_device_info == NULL) || (remapped_device_info == NULL) )
  {
    RF_MSG( RF_ERROR, "rfc_common_get_remapped_device_info: NULL params provided." );
    return FALSE;
  }

  /* Copy original contents */
  memscpy( remapped_device_info, sizeof(*remapped_device_info), source_device_info, sizeof(*source_device_info) );

  rfc_intf_ptr = rfc_intf::get_instance();

  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_remapped_device_info: NULL rfc cmn intf." );
    return FALSE;
  }

  /* Get remapping info */
  api_status &= rfc_intf_ptr->rfc_get_remapped_device_info( source_device_info, remapped_device_info );

  return api_status;
} /* rfc_common_get_remapped_device_info */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retuns the capability(RX or TX) of the rfm device passed

  @details
  This function retuns the capability(RX or TX) of the rfm device passed
 
  @param device
  Contains the rfm device whose capability needs to be queried

  @retval
  RFM_DEVICE_INVALID_SUPPORT(0x00) for Invalid support(No Rx/Tx) on that device
  RFM_DEVICE_RX_SUPPORTED(0x01) for RX device
  RFM_DEVICE_TX_SUPPORTED(0x02) for TX device
*/
rfm_device_capability_type
rfc_common_get_device_capability
(
   rfm_device_enum_type device
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();    
  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_common_get_device_capability: NULL rfc cmn intf." );
    return RFM_DEVICE_INVALID_SUPPORT;
  }
  
  return rfc_intf_ptr->rfc_get_device_capability( device );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API to obtain RFC dynamic properties

  @param rfc_cfg
  RFC dynamic properties

  @return
  TRUE means data is usable. FALSE otherwise
*/
boolean
rfc_get_rfcard_dynamic_properties
(
  rfm_rfcard_dynamic_properties_type *rfc_cfg
)
{
  rfc_intf *rfc_intf_ptr = rfc_intf::get_instance();    
  if (rfc_intf_ptr == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_get_rfcard_dynamic_properties: NULL rfc cmn intf." );
    return FALSE;
  }
  
  return rfc_intf_ptr->get_rfcard_dynamic_properties( rfc_cfg );
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Initializes RF device snapshot data structure. 
 
  return
  ptr to corresponding 
*/
/*----------------------------------------------------------------------------*/
#define BUS_INDEX 0
#define DEV_INSTANCE 0

void 
rfc_common_snapshot_init
(
   void
)
{
  rfc_physical_device_struct_type* phys_devices_array = NULL;
  rfcommon_device_snapshot_type* snapshot_data = rfcommon_get_snapshot_data();
  // Get pointer to rfc instance (rfc_p)
  rfc_intf *rfc_ptr = rfc_intf::get_instance();
  int invalid_reg_count=0;
  int phys_devices_count=0;
  if (NULL != rfc_ptr && snapshot_data != NULL )
  {    
    // Initialize all fields to 0
    memset(snapshot_data, 0x0, sizeof(rfcommon_device_snapshot_type));
    // Get physical devices array from rfc
    phys_devices_array = rfc_ptr->get_phys_devices_array();
    phys_devices_count = rfc_ptr->get_phys_devices_count();
    snapshot_data->hw_id = rfc_ptr->get_rfc_hardware_id();
    if ((NULL != phys_devices_array) && (phys_devices_count > 0)) 
    { 

      uint8 device_index = 0;
      for (device_index = 0; device_index < phys_devices_count; device_index++ )
      { 
        rfc_phy_device_info_type* phy_dev_cfg = phys_devices_array[device_index].phy_dev_cfg;
		
        rfdevice_physical_device* phy_device = phys_devices_array[device_index].device_obj;

        if ( phy_device != NULL && phy_dev_cfg != NULL && 
             phy_dev_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_RFFE)
        {
          if( snapshot_data->reg_list_phy_rffe_device_count < RFA_SNAPSHOT_MAX_CUSTOM_DEVICE_NUM )
          {
            
            uint16 reg_list_size = (uint16)(phy_device->rfdevice_phy_dev_get_reg_list_size());   
			
            uint16 reg_list_device_idx = snapshot_data->reg_list_phy_rffe_device_count;
            /* double check reg_list_size, if invalid size, do not allocate */
            if ( reg_list_size > 0 )
            {
              snapshot_data->reg_list_phy_rffe_device_count++; 
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx] = 
              (rfcommon_rffe_snapshot_addr_list_data_type*)modem_mem_alloc(
                        sizeof(rfcommon_rffe_snapshot_addr_list_data_type), MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx], 0,sizeof(rfcommon_rffe_snapshot_addr_list_data_type) );

              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->bus = phy_dev_cfg->bus[BUS_INDEX];              
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->slave_id = phy_dev_cfg->assigned_usid;
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump_size = reg_list_size;
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->addr_list =  
                     (uint16*)modem_mem_alloc( sizeof(uint16)*reg_list_size, MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->addr_list, 0, sizeof(uint16)*reg_list_size );
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump = 
                     (uint32*)modem_mem_alloc( sizeof(uint32)*reg_list_size, MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump, 0, sizeof(uint32)*reg_list_size );
			 
              phy_device->rfdevice_phy_dev_get_reg_list(snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->addr_list, reg_list_size);

            }
            else
            {
              uint8 max_reg_list_size=255;
              snapshot_data->reg_list_phy_rffe_device_count++; 
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx] = 
                    (rfcommon_rffe_snapshot_addr_list_data_type*)modem_mem_alloc(
                       sizeof(rfcommon_rffe_snapshot_addr_list_data_type), MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx], 0, sizeof(rfcommon_rffe_snapshot_addr_list_data_type) );
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->bus = phy_dev_cfg->bus[BUS_INDEX];              
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->slave_id = phy_dev_cfg->assigned_usid;
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump_size = max_reg_list_size;
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->addr_list =  
                     (uint16*)modem_mem_alloc( sizeof(uint16)*max_reg_list_size, MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->addr_list, 0, sizeof(uint16)*max_reg_list_size );
              snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump = 
                     (uint32*)modem_mem_alloc( sizeof(uint32)*max_reg_list_size, MODEM_MEM_CLIENT_RFA);
              memset(snapshot_data->reg_list_rffe_dump[reg_list_device_idx]->data_dump, 0, sizeof(uint32)*max_reg_list_size );
           }
           phy_device->rfdevice_phy_dev_free_reg_addr_buffer();

        }  
		  
      }/* if ( phy_dev_cfg != NULL && phy_dev_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_RFFE ) */  
    }/* for (device_index = 0; device_idx < phys_devices_count; device_idx++ ) */

    snapshot_data->initialized = TRUE;
  }/* if ((NULL != phys_devices_array) && (phys_devices_count > 0))  */
  else
  {
    RF_MSG(RF_ERROR, "rfc_common_rffe_logging_init: RFC physical devices array not available.");
  }
  }
  else
  {
    RF_MSG(RF_ERROR, "rfc_common_rffe_logging_init: rfc_p was not created." );
    //ERR_FATAL("RFC not initialized!!",0,0,0);
  }
}