#ifndef FTM_GSM_RFTEST_H
#define FTM_GSM_RFTEST_H

/*===========================================================================

        R F  C O N T R O L  D E F I N I T I O N S  H E A D E R  F I L E

DESCRIPTION
   This file contains headers for the new RF test framework supported by G.

 
   Copyright (c) 2001, 2002 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  Initially, comments will be entered manually. Later, as the module is
  placed under version control, the version control tool will automatic-
  ally insert change comments into this section. [Delete this comment].

  
$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/20   mpa     To support MiniDump Feature
08/14/15   zc      Rx Meas/Tx Control Interface update
07/27/15   zc      Initial Revision

=============================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#include "target.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"
#include "rex.h"

#ifdef FTM_HAS_UMTS

#ifdef FEATURE_GSM

#include "ftm.h"
#include "ftm_gsm_ctl.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm.h"
#include "ftm_gsm_device.h"
#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_rx_measure.h"
#include "ftm_rf_test_tx_control.h"
#include "rfgsm_core_util.h"
#include "rfgsm_mc.h"
#include "rfgsm_nv_cmn.h"
#include "rfgsm_mdsp_cal.h"
#include "rfcommon_math.h"
#include "ftm_msg.h"
#include "rfm_subscriber_types.h"

/* For reference to SYS_BAND macros. */
#include "sys_type.h"


#ifdef FEATURE_GSM_RFLM
#include "rxlm_intf.h"
#include "txlm_intf.h"
#endif

#include "rfcommon_nv_mm.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_device_manager.h"


#ifdef FEATURE_QSH_MDUMP 
#include "qsh.h"
#endif


/*Helper Macros*/
#define UNDEFINED_SUB_ID 2
#define RFTEST_GET_SUB(x) (((x) > 0) && ((x) < RFM_SUBSCRIBER_NUM)) ? ((x)-1) :UNDEFINED_SUB_ID
#define SUB_ID_BOUNDS_FAIL(x) (((x) >= UNDEFINED_SUB_ID) || ((x) < 0))
#define IS_DEVICE_VALID(x) ((x) < RFM_MAX_DEVICES)


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for  */

typedef enum
{
  GRFTEST_DEVCONFIG_DROP = 0,
  GRFTEST_DEVCONFIG_ADD,
  GRFTEST_DEVCONFIG_RECONFIG
} ftm_gsm_rftest_dev_config_t;

typedef enum
{
  GRFTEST_CLEAR_ASSOC_TX = 0,
  GRFTEST_CLEAR_ASSOC_RX,
  GRFTEST_CLEAR_MAX
} ftm_gsm_rftest_fh_action_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for device config type function pointer */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_gsm_rftest_dev_config_fp_t)
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data,
  rfm_device_enum_type device
);


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define G rftest config data*/

typedef struct 
{
  rfm_device_enum_type tx_device;
  rfm_device_enum_type rx_device;
  rfm_device_enum_type drx_device;
  ftm_mode_id_type gsm_mode;
  uint16 arfcn;
  ftm_gsm_rftest_dev_config_fp_t ftm_gsm_rftest_dev_config_fp;
  uint8 * chain_to_sub_map;
} ftm_gsm_rftest_radio_config_t;


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for G rftest rx_meas and tx_control data*/

typedef struct
{
  ftm_tx_burst_type tx_burst;
  ftm_frame_matrix_type frame_matrix[8];
  ftm_rx_burst_type rx_burst;
  int16 rx_burst_params[2];
} ftm_gsm_rftest_radio_control_t;


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Global Rf Test Data*/

typedef struct
{
  /*Storage structure for ftm_gsm_rftest_radio_data*/
  ftm_gsm_rftest_radio_config_t ftm_gsm_rftest_radio_data[UNDEFINED_SUB_ID];

  /*Storage structure for Rx Meas and Tx Control*/
  ftm_gsm_rftest_radio_control_t ftm_gsm_rftest_radio_control[UNDEFINED_SUB_ID];

  /*Dynamic mapping of chain to sub*/
  uint8 ftm_gsm_rftest_chain_to_sub_map[FTM_GSM_INVALID_CHAIN];
  
  /*Global Switch for the new RF test interface*/
  boolean ftm_gsm_rftest_enabled;

  /*Critical Section for changing devices on the fly*/
  rex_crit_sect_type ftm_gsm_rftest_device_lock;
} ftm_gsm_rftest_global_t;


/*===========================================================================
FUNCTION DEFINITIONS
===========================================================================*/

/*===========================================================================

FUNCTION ftm_gsm_rftest_lock_device/unlock_device

DESCRIPTION
   Lock/unlock rftest device mutex
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_lock_device(void);

void ftm_gsm_rftest_unlock_device(void);



/*===========================================================================

FUNCTION ftm_gsm_rftest_tx_control

DESCRIPTION
   Function handles common rx_measure command from host machine. 
   Command is broken down per device
   Carrier_X:
   Device_Y
  .Tx Action
   Tx Power
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_tx_control
(
  ftm_rf_test_tx_control_unpacked_data_t *query_data,
  uint32 num_actions
);


/*===========================================================================

FUNCTION ftm_gsm_rftest_rx_measure

DESCRIPTION
   Function handles common rx_measure command from host machine. 
   Command is broken down per device
   Rx_Carrier_X:
   Device_Y
   Expected AGC
   LNA Gain State
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
);


/*===========================================================================

FUNCTION ftm_gsm_rftest_init

DESCRIPTION
   Function inits gsm rftest
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_init( void );


/*===========================================================================

FUNCTION ftm_gsm_rftest_set_sub_from_device

DESCRIPTION
   Function takes the device and sub_id passed in, and sets the sub_id for the mapped chain
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_set_sub_from_device(rfm_device_enum_type dev, uint8 sub_id);



/*===========================================================================

FUNCTION ftm_gsm_rftest_get_sub_from_device

DESCRIPTION
   Function returns the sub from the device passed in
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ftm_gsm_rftest_get_sub_from_device(rfm_device_enum_type dev);



/*===========================================================================

FUNCTION ftm_gsm_rftest_get_sub_from_chain

DESCRIPTION
   Function returns the sub from the chain passed in
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ftm_gsm_rftest_get_sub_from_chain(ftm_gsm_chains_enum_type chain);



/*===========================================================================

FUNCTIONs:  ftm_gsm_rftest_get_remapped_device, ftm_gsm_rftest_get_associated_tx_device, ftm_gsm_rftest_get_associated_rx_device

DESCRIPTION
   If the new rftest_dev_config is enabled by the host, the functions:
   ftm_gsm_rftest_get_associated_rx_device, ftm_gsm_rftest_get_associated_rx_device, ftm_gsm_rftest_get_associated_rx_device
   need to return the below corresponding functions, instead of the static value stored in RFC.
   
DEPENDENCIES
   None.

RETURN VALUE
   RFM Device associated with the request

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_rftest_get_remapped_device(uint8 sub_id);
rfm_device_enum_type ftm_gsm_rftest_get_associated_tx_device(uint8 sub_id);
rfm_device_enum_type ftm_gsm_rftest_get_associated_rx_device(uint8 sub_id);



/*===========================================================================

FUNCTION ftm_gsm_rftest_get/set_rftest_enabled

DESCRIPTION
   Get/set rf_test_enabled flag
   
DEPENDENCIES
   None.

RETURN VALUE
   Boolean (TRUE if new rftest interface is enabled, false if otherwise)

SIDE EFFECTS
  None

===========================================================================*/
boolean ftm_gsm_rftest_get_rftest_enabled(void);

void ftm_gsm_rftest_set_rftest_enabled(boolean enabled);


/*===========================================================================

FUNCTION ftm_gsm_rftest_get_rftest_data_ptr

DESCRIPTION
   Function handles common radio_config command from host machine
   
DEPENDENCIES
   None.

RETURN VALUE
   Pointer to ftm_gsm_rftest_radio_data

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_rftest_radio_config_t * ftm_gsm_rftest_get_rftest_data_ptr(uint8 sub_id);



/*===========================================================================

FUNCTION ftm_gsm_rftest_get_rf_control_data_ptr

DESCRIPTION
   Get pointer to RF control data (for Rx measure and Tx pwr control)
   
DEPENDENCIES
   None.

RETURN VALUE
   Pointer to ftm_gsm_rftest_radio_control

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_rftest_radio_control_t * ftm_gsm_rftest_get_rf_control_data_ptr(uint8 sub_id);



/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config

DESCRIPTION
   Function handles common radio_config command from host machine
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config( ftm_rf_test_radio_config_unpack_data_t *command_unpack_data );
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================

FUNCTION ftm_gsm_get_rftest_globals_address

DESCRIPTION
   returns the address of ftm_gsm_rftest_globals
   
DEPENDENCIES
   None.

RETURN VALUE
   None

SIDE EFFECTS
   None

===========================================================================*/

QSH_MDUMP_FN_ATTR ftm_gsm_rftest_global_t * ftm_gsm_get_rftest_globals_address( void );
#endif /* FEATURE_QSH_MDUMP */



#endif //FEATURE_GSM
#endif // FTM_HAS_UMTS
#endif //FEATURE_FACTORY_TESTMODE


#endif /* #ifdef FTM_GSM_RFTEST_H */

