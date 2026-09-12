/*!
   @file
   rf_power_reporting.h

   @brief
   defines power logging structure and logging APIs

   @details

*/
/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $
$Header: //components/rel/rfa.mpss/4.11/rfc/vreg_mgr/wtr1605_sv/inc/rf_power_reporting.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
03/12/15   rc     Initial revision.

===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "log.h" /* defines log header */
#include "rflm_diag_power_rfa.h"
#include "rfdevice_type_defs.h" /* defines bitmask token */
#include "rfm_device_types.h" /* defines rfm devices */

#ifdef __cplusplus
extern "C"
{
#endif

rfdevice_wtr_config_bitmask_type*
rf_power_reporting_get_bitmask_ptr
(
  rfm_device_enum_type rfm_device  
);

void 
rf_power_reporting_update_channel
( 
  rfm_device_enum_type rfm_device, 
  uint16 channel
);

typedef enum
{
  TECH_CDMA,
  TECH_WCDMA,
  TECH_GSM,
  TECH_LTE,
  TECH_TDSCDMA
} technology_enum_type;

typedef struct
{
  uint32  time_delta_us;
  uint8   transmit_device_number;
  int16   power; /* For PA, TBD - are the units dbm10 or dbm100? */
  uint8   rgi; /* PDM field, RF gain index */
  uint8   lut; /* lut_enum_type */
  uint8   pa_state; /* eg. 0-8 */
  uint8   pa_gain_range; /* e.g. 0 corresponds to pa_states 0, 1 and 2 */
  /* TBD whether RF can find and fill in this value correctly */
  uint8   modulation;
  uint8   rb;
  uint8   padding[2];
} tx_agc_event_data;

typedef struct
{
  uint32 time_delta_us;
  uint8  receive_device_number;
  uint8  lna_state;
  uint8  reserved;
} rx_agc_event_data;

typedef struct
{
  uint32 time_delta_us;
  uint8  transmit_device_number;
  uint8  state; /* 0=off, 1=dtx, 2=on */
  uint8  reserved;
} tx_dynamic_state_event_data;

typedef struct
{
  uint32 time_delta_us;
  uint8  receive_device_number;
  uint8  state; /* 0=off, 1=dtx, 2=on */
  uint8  path; /* path_config_enum_type */
  /* Diversity state and carrier aggregation is determined by examining
     the active receive paths.  If PRX and DRX are active then one spreadsheet
     entry gives the total current of both rx paths.  For SCA, one spreadsheet
     entry gives the total corrent of all 4 rx paths */
} rx_dynamic_state_event_data;

typedef struct
{
  uint32  time_delta_us;
  uint8   transmit_device_number;
  uint8   tech; /* technology_enum_type */
  uint8   band; /* Implies TDD vs. FDD for LTE */
  uint8   bandwidth;
  uint8   wtr_chip_type; /* wtr_chip_enum_type */
  uint16  data_revision; /* WTR spreadsheet revision */
  uint32  freq_khz;
  uint8   pa_chip_type; /* pa_chip_enum_type */
  uint8   reserved; /* Resource blocks allocated for LTE (for PA) */
  uint16  pa_data_revision;
} tx_config_event_data;

typedef struct
{
  uint32  time_delta_us;
  uint8   receive_device_number;
  uint8   tech; /* technology_enum_type */
  uint8   band;
  uint8   bandwidth;
  uint8   wtr_chip_type; /* wtr_chip_enum_type */
  uint8   reserved;
  uint16  data_revision; /* WTR spreadsheet revision */
  uint32  freq_khz;
} rx_config_event_data;

typedef struct
{
  uint32 time_delta_us;
  uint32 duration; /* 0=off, 1=on */
  uint8  tx_device_number; /* TX device that fbrx is associated with */
  uint8  gain;
  uint16 reserved;
} fbrx_state_event_data;

typedef PACK(struct)
{
  uint32 time_delta_us;
  uint8  gps; /* 0=off, 1=on */
  int16  temp_c;
  int16  voltage_mv;
  uint32  reserved[2];
} external_conditions_event_data;


typedef PACK(struct)
{
  log_hdr_type  log_header;
  uint8   log_pkt_version;
  uint8   padding[7];
  uint64  ustmr;      /* Time when first sample in packet was collected */
  uint32  num_events;
  uint8   num_tx_agc_data_events;
  uint8   num_rx_agc_data_events;
  uint8   num_tx_dynamic_state_events;
  uint8   num_rx_dynamic_state_events;
  uint8   num_tx_config_events;
  uint8   num_rx_config_events;
  uint8   num_fbrx_state_events;
  uint8   num_external_conditions_events;
} rf_power_reporting_log_pkt;

void
process_tx_agc_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  tx_agc_event_data* tx_agc_storage,
  uint8* arr_cur_active_tx_device, 
  uint8* cur_num_of_active_tx_device
);

void
process_rx_agc_data
(
  rflm_diag_power_buffer_item_t *buf_ptr, 
  rx_agc_event_data* rx_agc_storage, 
  uint8* arr_cur_active_rx_device, 
  uint8* cur_num_of_active_rx_device
);

void
process_tx_dynamic_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  tx_dynamic_state_event_data* tx_dynamic_storage, 
  uint8* num_tx_dynamic_events, 
  uint8* tx_dynamic_update,
  uint8* arr_cur_active_tx_device, 
  uint8* cur_num_of_active_tx_device
);

void 
process_rx_dynamic_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  rx_dynamic_state_event_data* rx_dynamic_storage,
  uint8* arr_cur_active_rx_device, 
  uint8* cur_num_of_active_rx_device
);

void
process_missing_rx_device
(
  uint8* cur_num_of_active_rx_device_ptr,
  uint8* missing_num_of_rx_device_ptr,
  uint8* arr_missing_active_rx_device,
  uint8* arr_cur_active_rx_device
);

void
populate_tx_agc_data
(
  tx_agc_event_data* tx_agc_event,
  uint8 num_tx_agc_events,
  uint8 tx_agc_update,
  uint8 tx_update,
  uint8* array_tx_agc_event_index,
  rflm_diag_power_buffer_item_t *buf_ptr,
  tx_agc_event_data* tx_agc_storage,
  uint8* arr_cur_active_tx_device
);

void
populate_rx_agc_data
(
  rx_agc_event_data* rx_agc_event,
  uint8 rx_agc_update,
  uint8 num_logged_rx_agc_events,
  rflm_diag_power_buffer_item_t  *buf_ptr,
  rx_agc_event_data* rx_agc_storage,
  uint8 missing_num_of_rx_device,
  uint8* arr_missing_active_rx_device,
  uint8 rx_dynamic_update,
  uint8 cur_num_of_active_rx_device,
  uint8* arr_cur_active_rx_device,
  uint8* array_rx_agc_event_index
);

void
populate_tx_dynamic_data
(
  tx_dynamic_state_event_data* tx_dynamic_state_event,
  uint8 num_tx_dynamic_events,
  uint8 tx_dynamic_update,
  uint8* array_tx_dynamic_event_index,
  tx_dynamic_state_event_data* tx_dynamic_storage,
  uint8* arr_cur_active_tx_device,
  rflm_diag_power_buffer_item_t  *buf_ptr,
  uint8 tx_update
);

void
populate_rx_dynamic_data
(
  rx_dynamic_state_event_data* rx_dynamic_state_event,
  uint8 rx_update,
  uint8 rx_dynamic_update,
  uint8 num_logged_rx_dynamic_events,
  uint8 num_rx_dynamic_events,
  uint8* array_rx_dynamic_event_index,
  rx_dynamic_state_event_data* rx_dynamic_storage,
  rflm_diag_power_buffer_item_t* buf_ptr,
  uint8* arr_cur_active_rx_device,
  uint8 missing_num_of_rx_device,
  uint8* arr_missing_active_rx_device        
);

void
populate_tx_config_data
(
  tx_config_event_data* tx_config_event,
  uint8 num_tx_config_events,
  uint8 tx_update,
  uint8* arr_cur_active_tx_device
);

void
populate_rx_config_data
(
  rx_config_event_data* rx_config_event,
  uint8 num_rx_config_events,
  uint8 rx_update,
  uint8* arr_cur_active_rx_device
);

void 
populate_fbrx_data
(
  fbrx_state_event_data* fbrx_state_event,
  uint8 num_fbrx_state_events,
  uint8* array_fbrx_state_event_index,
  rflm_diag_power_buffer_item_t  *buf_ptr
);

uint8 convert_band_enum(uint8 tech, uint8 band);
uint8 convert_bw_enum(uint8 tech, uint8 bw);

void init_power_reporting_timer();
void start_power_reporting_timer();
void stop_power_reporting_timer();
void rfc_power_reporting_timer_cb(timer_cb_data_type data);
void rfc_power_reporting_logging_cb(uint32* rbuf_ptr, uint32 num_of_rd);

#ifdef __cplusplus
}
#endif





