/*!
   @file
   rfc_vreg_mgr_wtr1605_sv.h

   @brief
   

   @details

*/
/*===========================================================================

Copyright (c) 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rfc/vreg_mgr/wtr1605_sv/src/rf_power_reporting.cpp#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
03/02/15   rc     Initial revision.

===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rf_power_reporting.h"
#include "rfcommon_msg.h"  /* Define RF MSG */
extern "C"{
#include "rfcommon_core_device_manager.h"
enum tracer_event_id_enum
{
  event_0,
  event_1
};


#if !defined(TEST_FRAMEWORK) && (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
#include "tracer.h"
tracer_client_handle_t rfc_power_tracer_handle = NULL;
#endif

#define MAX_GAIN_STATE 10
#define RF_POWER_REPORTING_TIMER_MS 20
#define CONVERSION_RATIO 19202 //converts us to ms time
#define convert_to_ms(timestamp) (timestamp/CONVERSION_RATIO)
#define DATA_REVISION 2
#define PA_DATA_REVISION 2

static timer_type power_reporting_timer;
static rfdevice_wtr_config_bitmask_type bitmask_arr[RFM_MAX_WAN_DEVICES][MAX_GAIN_STATE];
static uint8 num_of_active_rx_device = 0;
static uint8 num_of_active_tx_device = 0;
static uint8 arr_active_rx_device[RFM_MAX_WAN_DEVICES] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
static uint8 arr_active_tx_device[RFM_MAX_WAN_DEVICES] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
static tx_agc_event_data tx_agc_record[RFM_MAX_WAN_DEVICES];
static rx_agc_event_data rx_agc_record[RFM_MAX_WAN_DEVICES];
static rx_dynamic_state_event_data rx_dynamic_record[RFM_MAX_WAN_DEVICES];
static tx_dynamic_state_event_data tx_dynamic_record[RFM_MAX_WAN_DEVICES];
static uint16 arr_channel[RFM_MAX_WAN_DEVICES]; /* stores device channel number */
static uint16 prev_channel[RFM_MAX_WAN_DEVICES]; /* stores previous channel number */
const uint16  wcdma_band[RFCOM_BAND_INVALID] = {1,1900,3,4,800,8,9,11,19};
const uint8  lte_bw[RFCOM_BW_LTE_INVALID] = {1,3,5,10,15,20,40};
const uint8  wcdma_bw[RFCOM_BW_WCDMA_INVALID] = {3,5,10,15};
const uint8  cdma_bw[RFCOM_BW_WCDMA_INVALID] = {1,3,5,10};
const uint16 gsm_band[RFCOM_NUM_GSM_BANDS] = {850, 900, 1800, 1900, 1800};
const uint8 tds_band[RFCOM_NUM_TDSCDMA_BANDS] = {34,39,40,40,34,39};
static uint32 local_ustmr = 0; /* timestamp of earliest event */
static uint64 prev_sys_time = 0;
static uint64 prev_ustmr = 0;

void rfc_power_reporting_qdss_tracer(uint32* rbuf_ptr, uint32 num_of_rd);

inline uint8 get_tech(rfdevice_wtr_config_bitmask_type bitmask) { return ((bitmask>>RFDEVICE_WTR_CFG_TECH_BSHFT) & ((1<<RFDEVICE_WTR_CFG_TECH_BIT_SIZE)-1));}
inline uint8 get_band(rfdevice_wtr_config_bitmask_type bitmask) {return ((bitmask>>RFDEVICE_WTR_CFG_BAND_BSHFT) & ((1<<RFDEVICE_WTR_CFG_BAND_BIT_SIZE)-1));}
inline uint8 get_bandwidth(rfdevice_wtr_config_bitmask_type bitmask) {return ((bitmask>>RFDEVICE_WTR_CFG_BW_BSHFT) & ((1<<RFDEVICE_WTR_CFG_BW_BIT_SIZE)-1));}
inline uint8 get_lut(rfdevice_wtr_config_bitmask_type bitmask) {return ((bitmask>>RFDEVICE_WTR_CFG_LUT_BSHFT) & ((1<<RFDEVICE_WTR_CFG_LUT_BIT_SIZE)-1));}
inline rfdevice_wtr_config_bitmask_type 
mask_lut_mode(rfdevice_wtr_config_bitmask_type bitmask)
{
   uint32 mask = ((1<<(RFDEVICE_WTR_CFG_MODE_BSHFT+RFDEVICE_WTR_CFG_MODE_BIT_SIZE))-1)&(~((1<<RFDEVICE_WTR_CFG_LUT_BSHFT)-1)); 
   bitmask &= ~mask;
   #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
   RF_MSG_1(RF_MED,"rf_power_reporting.c -> mask_lut_mode -> mask after clear bits %d", bitmask);
   #endif
   return bitmask;
}

rfdevice_wtr_config_bitmask_type*
rf_power_reporting_get_bitmask_ptr
(
  rfm_device_enum_type rfm_device  
)
{
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_1(RF_MED,"rf_power_reporting.c -> rf_power_reporting_get_bitmask_ptr -> device%d", rfm_device);
  #endif
  if (rfm_device < RFM_DEVICE_0 || rfm_device > RFM_MAX_WAN_DEVICES) 
  {
    return NULL;
  }
  else
  {
    return bitmask_arr[rfm_device];
  }
}

void 
rf_power_reporting_update_channel
( 
  rfm_device_enum_type rfm_device, 
  uint16 channel
)
{
  if (rfm_device > RFM_MAX_WAN_DEVICES || rfm_device < RFM_DEVICE_0 ) 
  {
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_1(RF_MED, "rf_power_reporting - rf_power_reporting_update_channel invalid device - device number %d", 
             rfm_device );
    #endif
    return;
  }
  arr_channel[rfm_device] = channel;
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_2(RF_MED, "rf_power_reporting - rf_power_reporting_update_channel channel: %d, device number %d", 
           channel,
           rfm_device );
  #endif
}

inline uint8 get_path_config(rfdevice_wtr_config_bitmask_type bitmask) {return ((bitmask>>RFDEVICE_WTR_CFG_MODE_BSHFT) & ((1<<RFDEVICE_WTR_CFG_MODE_BIT_SIZE)-1));}
inline uint8 get_chip_number(rfdevice_wtr_config_bitmask_type bitmask) {return ((bitmask>>RFDEVICE_WTR_CFG_CHIP_BSHFT) & ((1<<RFDEVICE_WTR_CFG_CHIP_BIT_SIZE)-1));}

void init_power_reporting_timer(){
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"POW_REP:RF init power timer %d", 1);
  #endif
  timer_def( &power_reporting_timer,
               NULL, 
               NULL, 
               0,
               rfc_power_reporting_timer_cb, 
               NULL );

  rflm_diag_power_set_read_callback(rfc_power_reporting_logging_cb);
}

void 
start_power_reporting_timer()
{
	//Start 20ms timer for power reporting
    timer_set(&power_reporting_timer, RF_POWER_REPORTING_TIMER_MS, RF_POWER_REPORTING_TIMER_MS, T_MSEC);
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"POW_REP:RF timer started %d", 1);

#if 1
    #if !defined(TEST_FRAMEWORK) && (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
    {
      tracer_return_enum_t result;
      if (rflm_diag_power_qdss_log_enabled())
      {
        result = tracer_register((tracer_client_handle_t *)&rfc_power_tracer_handle, 
                                (tracer_ost_entity_id_enum_t)TRACER_ENTITY_DEFAULT, 
                                (tracer_ost_protocol_id_t)TRACER_PROTOCOL_DEFAULT);
         if (result == TRACER_RETURN_SUCCESS)
         {
            RF_MSG_1(RF_MED,"POW_REP:RFC power tracer handle is registered.", 1);
         }
         else
         {
           rfc_power_tracer_handle = NULL;
           RF_MSG_1(RF_MED,"POW_REP:RFC power tracer handle failed to register.", 0);
}
      }
    }
    #endif
#endif
}

void 
stop_power_reporting_timer()
{
	// Stop 20ms timer for power reporting
    timer_clr( &power_reporting_timer, T_NONE ); 
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"POW_REP:RF timer cleared %d", 1);

#if 1
  #if !defined(TEST_FRAMEWORK) && (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
  if (rfc_power_tracer_handle!= NULL) 
  {
    tracer_unregister((tracer_client_handle_t *)&rfc_power_tracer_handle);
  }
  #endif
#endif
}

void 
rfc_power_reporting_timer_cb (timer_cb_data_type data)
{
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"POW_REP:Timer callback called %d", 1);
  #endif
  if (log_status(LOG_WTR_POWER_C)) 
  {
    rflm_diag_power_enable_qxdm_log();
  }
  rflm_diag_power_switch_notify();
}

void
rfc_power_reporting_logging_cb
(
  uint32* rbuf_ptr, 
  uint32  num_of_rd
)
{
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_3(RF_MED, "POW_REP:logging cb-qdss enable=%d,qxdm enable=%d,num_of_rd=%d", 
           rflm_diag_power_qdss_log_enabled(),
           rflm_diag_power_qxdm_log_enabled(), num_of_rd);
  #endif
  if( rflm_diag_power_qxdm_log_enabled() && num_of_rd )
   {
      rflm_diag_power_buffer_item_t  *buf_ptr  = (rflm_diag_power_buffer_item_t  *)rbuf_ptr;
      rf_power_reporting_log_pkt *rf_pwr_rpt_log_pkt = NULL;
      /*initializes local variables*/
      uint8 num_tx_agc_events = 0;
      uint8 num_rx_agc_events = 0;
      uint8 num_tx_dynamic_events = 0;
      uint8 num_rx_dynamic_events = 0;
      uint8 num_tx_config_events = 0;
      uint8 num_rx_config_events = 0;
      uint8 num_fbrx_state_events = 0;
      uint8 num_external_conditions_events = 1;
      uint8 array_tx_agc_event_index[num_of_rd];
      uint8 array_rx_agc_event_index[num_of_rd];
      uint8 array_tx_config_event_index[num_of_rd];
      uint8 array_rx_config_event_index[num_of_rd];
      uint8 array_tx_dynamic_event_index[num_of_rd];
      uint8 array_rx_dynamic_event_index[num_of_rd];
      uint8 array_fbrx_state_event_index[num_of_rd];
      uint32 static_variable_length = 0 ;
      uint32 tx_agc_length = 0;
      uint32 rx_agc_length = 0;
      uint32 tx_dynamic_length = 0;
      uint32 rx_dynamic_length = 0;
      uint32 tx_config_length = 0;
      uint32 rx_config_length = 0;
      uint32 fbrx_length = 0;
      uint32 external_condition_length = 0;
      uint32 length = 0;
    uint8  tx_agc_update = 0;
    uint8  rx_agc_update = 0;
    uint8  tx_dynamic_update = 0;
    uint8  rx_dynamic_update = 0;
    tx_agc_event_data tx_agc_storage[num_of_rd];
    rx_agc_event_data rx_agc_storage[num_of_rd];
    tx_dynamic_state_event_data tx_dynamic_storage[num_of_rd];
    rx_dynamic_state_event_data rx_dynamic_storage[num_of_rd];
    uint8 cur_num_of_active_tx_device = 0; /* number of current TX device */
    uint8 arr_cur_active_rx_device[RFM_MAX_WAN_DEVICES]; /* current active RX device */
    uint8 arr_cur_active_tx_device[RFM_MAX_WAN_DEVICES]; /* current active TX device */
    uint8 cur_num_of_active_rx_device = 0; /* number of current RX device */
    uint32 pre_tx_agc_timestamp = 0; /* store previous TXAGC timestamp read */
    uint8 tx_update;
    uint8 rx_update;
    uint8 missing_num_of_rx_device = 0;
    uint8 arr_missing_active_rx_device[RFM_MAX_WAN_DEVICES];
    uint8 num_logged_rx_agc_events = 0;
    uint8 num_logged_rx_dynamic_events = 0;  

    memset(arr_cur_active_rx_device, -1, sizeof(arr_cur_active_rx_device));
    memset(tx_agc_storage,0,sizeof(tx_agc_storage));
    memset(rx_agc_storage,0,sizeof(rx_agc_storage));
    memset(tx_dynamic_storage,0,sizeof(tx_dynamic_storage));
    memset(rx_dynamic_storage,0,sizeof(rx_dynamic_storage));

    uint8 i;

      for (i=0; i<num_of_rd; i++)
      {
            local_ustmr = buf_ptr[0].timestamp;
      switch (buf_ptr[i].payload[0].chg_type) 
      {
         case RFLM_DIAG_POWER_TYPE_TX_AGC:{
             uint32 tx_agc_timestamp = buf_ptr[i].timestamp;
        if (convert_to_ms(tx_agc_timestamp) != convert_to_ms(pre_tx_agc_timestamp)) 
        {
                array_tx_agc_event_index[num_tx_agc_events] = i;
          process_tx_agc_data(&buf_ptr[i],
                              &tx_agc_storage[num_tx_agc_events], 
                              arr_cur_active_tx_device, 
                              &cur_num_of_active_tx_device );
                num_tx_agc_events++;
                pre_tx_agc_timestamp = tx_agc_timestamp;
          tx_agc_update = 1;
             }
             break;}
         case RFLM_DIAG_POWER_TYPE_RX_AGC:{
             array_rx_agc_event_index[num_rx_agc_events] = i;
        process_rx_agc_data( &buf_ptr[i], 
                             &rx_agc_storage[num_rx_agc_events], 
                             arr_cur_active_rx_device, 
                             &cur_num_of_active_rx_device );
             num_rx_agc_events++;
        rx_agc_update = 1;
             break;}
         case RFLM_DIAG_POWER_TYPE_TX_DYNAMIC:{
             array_tx_dynamic_event_index[num_tx_dynamic_events] = i;
        process_tx_dynamic_data(&buf_ptr[i],
                                &tx_dynamic_storage[num_tx_dynamic_events], 
                                &num_tx_dynamic_events, 
                                &tx_dynamic_update, 
                                arr_cur_active_tx_device, 
                                &cur_num_of_active_tx_device );
             break;}
         case RFLM_DIAG_POWER_TYPE_RX_DYNAMIC:{
             array_rx_dynamic_event_index[num_rx_dynamic_events] = i;
        process_rx_dynamic_data(&buf_ptr[i],
                                &rx_dynamic_storage[num_rx_dynamic_events], 
                                arr_cur_active_rx_device, 
                                &cur_num_of_active_rx_device );
             num_rx_dynamic_events++;
        rx_dynamic_update = 1;
             break;}
         case RFLM_DIAG_POWER_TYPE_FBRX:{
             array_fbrx_state_event_index[num_fbrx_state_events] = i;
             num_fbrx_state_events++;
             break;}
      default:
        break;
      } /* for (i=0; i<num_of_rd; i++) */
    }

    tx_update = (tx_agc_update || tx_dynamic_update);
    rx_update = (rx_agc_update || rx_dynamic_update);
             
    if (rx_update) 
    {
      process_missing_rx_device
      (
        &cur_num_of_active_rx_device,
        &missing_num_of_rx_device,
        arr_missing_active_rx_device,
        arr_cur_active_rx_device
      );
    }  

    if (!tx_update){
      num_tx_agc_events = num_of_active_tx_device;
      num_tx_dynamic_events = num_of_active_tx_device;
      num_tx_config_events = num_of_active_tx_device;
         }
    else
    {
      if (!tx_agc_update) 
      {
        num_tx_agc_events = cur_num_of_active_tx_device;
      }
      else if (!tx_dynamic_update) 
      {
        num_tx_dynamic_events = cur_num_of_active_tx_device;
      }
      num_tx_config_events = cur_num_of_active_tx_device; 
      }

    if (!rx_update) 
    {
      num_rx_agc_events = num_of_active_rx_device;
      num_rx_dynamic_events = num_of_active_rx_device;
      num_rx_config_events = num_of_active_rx_device;
      }
    else
    {
      if (rx_agc_update) 
      {
        num_logged_rx_agc_events = num_rx_agc_events;
        num_rx_agc_events = num_rx_agc_events + missing_num_of_rx_device;
      }
      else
      {
        num_rx_agc_events = cur_num_of_active_rx_device;
      }

      if ( rx_dynamic_update ) {
        num_logged_rx_dynamic_events = num_rx_dynamic_events;
        num_rx_dynamic_events = num_rx_dynamic_events + missing_num_of_rx_device;
      }
      else
      {
        num_rx_dynamic_events = cur_num_of_active_rx_device;
      }

      num_rx_config_events = cur_num_of_active_rx_device;
      }

    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_5(RF_MED, "num_of_active_tx_device %d, cur_num_of_active_tx_device %d, num_of_active_rx_device %d, cur_num_of_active_rx_device %d, missing_num_of_rx_device %d", 
             num_of_active_tx_device,
             cur_num_of_active_tx_device,
             num_of_active_rx_device,
             cur_num_of_active_rx_device,
             missing_num_of_rx_device);
    #endif

     static_variable_length = sizeof(rf_power_reporting_log_pkt);
     tx_agc_length = sizeof(tx_agc_event_data)*num_tx_agc_events;
     rx_agc_length = sizeof(rx_agc_event_data)*num_rx_agc_events;
     tx_dynamic_length = sizeof(tx_dynamic_state_event_data)*num_tx_dynamic_events;
     rx_dynamic_length = sizeof(rx_dynamic_state_event_data)*num_rx_dynamic_events;
     tx_config_length = sizeof(tx_config_event_data)*num_tx_config_events;
     rx_config_length = sizeof(rx_config_event_data)*num_rx_config_events;
     fbrx_length = sizeof(fbrx_state_event_data)*num_fbrx_state_events;
     external_condition_length = sizeof(external_conditions_event_data)*num_external_conditions_events; 

     length =  static_variable_length + tx_agc_length + rx_agc_length+tx_dynamic_length+rx_dynamic_length+tx_config_length+rx_config_length+fbrx_length+external_condition_length;   
      
     /* Allocate memory */
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_1(RF_MED, "packet size %d", length); 
    RF_MSG_1(RF_MED, "static_variable_length %d", static_variable_length); 
    RF_MSG_1(RF_MED, "tx_agc_length %d", tx_agc_length); 
    RF_MSG_1(RF_MED, "rx_agc_length %d", rx_agc_length); 
    RF_MSG_1(RF_MED, "tx_dynamic_length %d", tx_dynamic_length); 
    RF_MSG_1(RF_MED, "rx_dynamic_length %d", rx_dynamic_length); 
    RF_MSG_1(RF_MED, "tx_config_length %d", tx_config_length);
    RF_MSG_1(RF_MED, "rx_config_length %d", rx_config_length); 
    RF_MSG_1(RF_MED, "fbrx_length %d", fbrx_length);
    RF_MSG_1(RF_MED, "external_condition_length %d", external_condition_length);
    #endif

     rf_pwr_rpt_log_pkt = (rf_power_reporting_log_pkt*) log_alloc(LOG_WTR_POWER_C, length);
     if( rf_pwr_rpt_log_pkt != NULL )
     {
        tx_agc_event_data*                tx_agc_event;
        rx_agc_event_data*                rx_agc_event;             
        tx_dynamic_state_event_data*      tx_dynamic_state_event;   
        rx_dynamic_state_event_data*      rx_dynamic_state_event;   
        tx_config_event_data*             tx_config_event;          
        rx_config_event_data*             rx_config_event;          
        fbrx_state_event_data*            fbrx_state_event;         
        external_conditions_event_data*   external_conditions_event;

      rf_pwr_rpt_log_pkt->log_pkt_version = 2;
        memset(rf_pwr_rpt_log_pkt->padding,0,sizeof(rf_pwr_rpt_log_pkt->padding)); // zero padding
      uint32 cur_lower_sys_time = (rf_time_get_tick().fine_timetick) & (((uint64)1 << 32) - 1);
      uint32 prev_lower_sys_time = prev_sys_time & (((uint64)1 << 32) - 1);
      if (cur_lower_sys_time < prev_lower_sys_time) {
         rf_pwr_rpt_log_pkt->ustmr = ( prev_sys_time & (~(((uint64)1<<32)-1)) ) | local_ustmr; 
      }
      else{
         rf_pwr_rpt_log_pkt->ustmr = ( rf_time_get_tick().fine_timetick & (~(((uint64)1<<32)-1)) ) | local_ustmr; 
      }

      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      if ( (rf_pwr_rpt_log_pkt->ustmr - prev_ustmr) > 4000000 ) {

         RF_MSG_4(RF_MED, "rf_power_reporting - possible ustmr error: cur_lower_sys_time %d, prev_lower_sys_time %d,prev_sys_time %d, rf_time_get_tick().fine_timetick %d ", 
                  cur_lower_sys_time,
                  prev_lower_sys_time,
                  prev_sys_time,
                  rf_time_get_tick().fine_timetick
                  ); 
      }
      #endif

      prev_ustmr = rf_pwr_rpt_log_pkt->ustmr;
      prev_sys_time = rf_time_get_tick().fine_timetick;
        rf_pwr_rpt_log_pkt->num_events = num_tx_agc_events+num_rx_dynamic_events+num_tx_dynamic_events+num_rx_dynamic_events+num_tx_config_events+num_rx_config_events+num_fbrx_state_events+num_external_conditions_events;
        rf_pwr_rpt_log_pkt->num_tx_agc_data_events = num_tx_agc_events;
        rf_pwr_rpt_log_pkt->num_rx_agc_data_events = num_rx_agc_events;
        rf_pwr_rpt_log_pkt->num_tx_dynamic_state_events = num_tx_dynamic_events;
        rf_pwr_rpt_log_pkt->num_rx_dynamic_state_events = num_rx_dynamic_events;
        rf_pwr_rpt_log_pkt->num_tx_config_events = num_tx_config_events;
        rf_pwr_rpt_log_pkt->num_rx_config_events = num_rx_config_events;
        rf_pwr_rpt_log_pkt->num_fbrx_state_events = num_fbrx_state_events;
        rf_pwr_rpt_log_pkt->num_external_conditions_events = num_external_conditions_events;

      /* copy tx agc data */
        tx_agc_event = (tx_agc_event_data*)((uint8*)rf_pwr_rpt_log_pkt + static_variable_length);
      populate_tx_agc_data
      (
        tx_agc_event,
        num_tx_agc_events,
        tx_agc_update,
        tx_update,
        array_tx_agc_event_index,
        buf_ptr,
        tx_agc_storage,
        arr_cur_active_tx_device
      );
      /* copy tx agc data */

      /* ------------------------- */
      /* copy rx agc  */
        rx_agc_event = (rx_agc_event_data*)((uint8*)tx_agc_event + tx_agc_length );
      populate_rx_agc_data
      (
        rx_agc_event,
        rx_agc_update,
        num_logged_rx_agc_events,
        buf_ptr,
        rx_agc_storage,
        missing_num_of_rx_device,
        arr_missing_active_rx_device,
        rx_dynamic_update,
        cur_num_of_active_rx_device,
        arr_cur_active_rx_device,
        array_rx_agc_event_index
      ); 
      /* end copy rx agc */
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy rx agc" );
      #endif 
      /* copy tx dynamic data */
        tx_dynamic_state_event = (tx_dynamic_state_event_data*)((uint8*)rx_agc_event+rx_agc_length);
      populate_tx_dynamic_data
      (
        tx_dynamic_state_event,
        num_tx_dynamic_events,
        tx_dynamic_update,
        array_tx_dynamic_event_index,
        tx_dynamic_storage,
        arr_cur_active_tx_device,
        buf_ptr,
        tx_update
      );
      /* end copy tx dynamic data */
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy tx dynamic data" );
      #endif 
      /* copy rx dynamic */
        rx_dynamic_state_event = (rx_dynamic_state_event_data*)((uint8*)tx_dynamic_state_event+tx_dynamic_length);

      populate_rx_dynamic_data
      (
        rx_dynamic_state_event,
        rx_update,
        rx_dynamic_update,
        num_logged_rx_dynamic_events,
        num_rx_dynamic_events,
        array_rx_dynamic_event_index,
        rx_dynamic_storage,
        buf_ptr,
        arr_cur_active_rx_device,
        missing_num_of_rx_device,
        arr_missing_active_rx_device        
      );
      /* end copy rx dynamic */
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy rx dynamic " );
      #endif 
     /* copy tx config */
        tx_config_event = (tx_config_event_data*)((uint8*)rx_dynamic_state_event+rx_dynamic_length);
      populate_tx_config_data
      (
        tx_config_event,
        num_tx_config_events,
        tx_update,
        arr_cur_active_tx_device
      );
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy tx config " );
      #endif 
      /* copy rx config data */
        rx_config_event = (rx_config_event_data*)((uint8*)tx_config_event+tx_config_length);
      populate_rx_config_data
      (
        rx_config_event,
        num_rx_config_events,
        rx_update,
        arr_cur_active_rx_device
      );
      /* end copy rx config data */
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy rx config " );
      #endif 
      /* populating fbrx event data */
        fbrx_state_event = (fbrx_state_event_data*)((uint8*)rx_config_event+rx_config_length);
      populate_fbrx_data
      (
        fbrx_state_event,
        num_fbrx_state_events,
        array_fbrx_state_event_index,
        buf_ptr
      );
      /* finish populating fbrx event data */
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG( RF_MED, "end copy fbrx " );
      #endif 

      /*populate external event data*/ 
      external_conditions_event = (external_conditions_event_data*)((uint8*)fbrx_state_event+fbrx_length);
      external_conditions_event[0].time_delta_us = 0;
      external_conditions_event[0].temp_c = 0 ;
      external_conditions_event[0].gps = rfcommon_get_dev_power_voting_status(RFM_DEVICE_GPS);
      int32 volt = 0;
      //AdcReadVphPwrExample(&volt);
      external_conditions_event[0].voltage_mv = volt;
      external_conditions_event[0].reserved[0] = 0;
      external_conditions_event[0].reserved[1] = 0;         
      /*finish populate external event data*/

      /*Commit Log packet*/
      log_commit( rf_pwr_rpt_log_pkt );
      rf_pwr_rpt_log_pkt = NULL;
      RF_MSG_1(RF_MED, "POW_REP:RF logging comitted %d", 1);
    } /* if( rf_pwr_rpt_log_pkt != NULL ) */
    else 
    {
      RF_MSG_1( RF_ERROR, "RF_Power_reporting: Unable to allocate Log memory: %d", 20 );
    }
    
  } /* if( log_status(LOG_WTR_POWER_C) && num_of_rd) */ 

  if (rflm_diag_power_qdss_log_enabled() && (num_of_rd > 0))
  {
     rfc_power_reporting_qdss_tracer(rbuf_ptr,num_of_rd);
  }
}


void rfc_power_reporting_qdss_tracer(uint32* rbuf_ptr, uint32 num_of_rd)
{
#if !defined(TEST_FRAMEWORK) && (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
  tracer_return_enum_t result;
  char buf[100];
  const uint16 buf_len = 100;
  uint32 idx           = 0;
  rflm_diag_power_buffer_item_t * buffer_item_ptr    = NULL;
  rflm_diag_power_buffer_item_header_t  * header_ptr = NULL;

  static const char * const rfc_power_qdss_args_format[] = {
      "RFSW_%d@0x%x:[%x-%x]\0",
      "RFSW_%d@0x%x:[%x-%x,%x-%x]\0",
      "RFSW_%d@0x%x:[%x-%x,%x-%x,%x-%x]\0",
      "RFSW_%d@0x%x:[%x-%x,%x-%x,%x-%x,%x-%x]\0",
      "RFSW_%d@0x%x:[%x-%x,%x-%x,%x-%x,%x-%x,%x-%x]\0",
      "RFSW_%d@0x%x:[%x-%x,%x-%x,%x-%x,%x-%x,%x-%x,%x-%x]\0" 
  };

  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_1(RF_MED, "POW_REP:qdss logging called,handle=0x%x", (uint32)rfc_power_tracer_handle);
  #endif

  if (rfc_power_tracer_handle == NULL) 
  {
    RF_MSG_1( RF_ERROR, "POW_REP - tracer handle is NULL and can not log data!",0); 
    return; 
  }

  for (idx = 0; idx < num_of_rd; idx++) 
  {
    buffer_item_ptr = (rflm_diag_power_buffer_item_t *)(rbuf_ptr +  (sizeof(rflm_diag_power_buffer_item_t)/sizeof(uint32))*idx);
    header_ptr = &buffer_item_ptr->header;

    (void) snprintf(buf, buf_len,                   
                    rfc_power_qdss_args_format[header_ptr->length - 1],
                    header_ptr->tech,
                    buffer_item_ptr->timestamp,
                    buffer_item_ptr->payload[0].chg_type,
                    buffer_item_ptr->payload[0].chg_data,
                    buffer_item_ptr->payload[1].chg_type,
                    buffer_item_ptr->payload[1].chg_data,
                    buffer_item_ptr->payload[2].chg_type,
                    buffer_item_ptr->payload[2].chg_data,
                    buffer_item_ptr->payload[3].chg_type,
                    buffer_item_ptr->payload[3].chg_data,
                    buffer_item_ptr->payload[4].chg_type,
                    buffer_item_ptr->payload[4].chg_data,
                    buffer_item_ptr->payload[5].chg_type,
                    buffer_item_ptr->payload[5].chg_data);
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_1(RF_MED,"POW_REP - tracer_data %s", *buf);
    #endif
    #if 1
    result = tracer_data(rfc_power_tracer_handle,TRACER_OPT_NONE,strlen(buf), buf);
    if(result != TRACER_RETURN_SUCCESS)
    {
    RF_MSG_1( RF_ERROR, "rflm_diag_power - tracer_data failed to log power data!",0);
  }
    #endif
  }
#endif
}

void
process_tx_agc_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  tx_agc_event_data* tx_agc_storage,
  uint8* arr_cur_active_tx_device, 
  uint8* cur_num_of_active_tx_device
)
{
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_1(RF_MED, "rf_power_reporting - process_tx_agc_data %d" , 0);
  #endif
  uint8 payload_len = buf_ptr->header.length;
            uint8 pa_state = 0;
  rfm_device_enum_type tx_device;
  uint8 i;
  for (i = 0; i < payload_len; i++) 
  {
    switch (buf_ptr->payload[i].chg_type) 
    {
    case RFLM_DIAG_POWER_TYPE_RGI:                                                                                                                                                
      tx_agc_storage->rgi = buf_ptr->payload[i].chg_data;   
      break;
    case RFLM_DIAG_POWER_TYPE_PA:
      pa_state = buf_ptr->payload[i].chg_data;                                                                                                                                                  
      tx_agc_storage->pa_state = pa_state;
      break;
    case RFLM_DIAG_POWER_TYPE_POWER:
      tx_agc_storage->power = buf_ptr->payload[i].chg_data/10;
      break;
    case RFLM_DIAG_POWER_TYPE_MOD:
      tx_agc_storage->modulation = buf_ptr->payload[i].chg_data;
      break;
    case RFLM_DIAG_POWER_TYPE_RB:
      tx_agc_storage->rb = buf_ptr->payload[i].chg_data;
    default:                                                                                                                                                                       
      break;                                                                                                                                                                                                                                                                                                                                                                                     
    } 
  }
  
  tx_device = (rfm_device_enum_type) buf_ptr->header.device_id;
  rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[tx_device][pa_state];
  tx_agc_storage->time_delta_us = 0;
  tx_agc_storage->transmit_device_number = tx_device;                            
  tx_agc_storage->lut = get_lut(bitmask);
  if (get_lut(bitmask) == RFDEVICE_WTR_CFG_LUT_HP) 
  {
    tx_agc_storage->lut = 2;
  }
  else if (get_lut(bitmask) == RFDEVICE_WTR_CFG_LUT_LP)  
  {
    tx_agc_storage->lut = 0;
  }

  tx_agc_storage->pa_gain_range = 0;
  memset(tx_agc_storage->padding,0,sizeof(tx_agc_storage->padding));

  tx_agc_record[tx_device] =  *tx_agc_storage;

  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_3(RF_MED, "rf_power_reporting - process_tx_agc_data bitmask %d, tx device %d, pa_state %d", 
           bitmask, 
           tx_device, 
           pa_state);
  #endif
                                                                                                                                                                                                  
  /* add tx device to cur active device list */
  uint8 add_cur_tx_device = 1;
  for (i = 0; i < *cur_num_of_active_tx_device && i < RFM_MAX_WAN_DEVICES; i++) 
  {
    if (arr_cur_active_tx_device[i] == tx_device) 
    {
      add_cur_tx_device = 0;
    }
  }

  if ( add_cur_tx_device ) 
  {
    arr_cur_active_tx_device[(*cur_num_of_active_tx_device)] = tx_device;
    (*cur_num_of_active_tx_device)++;
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_1(RF_MED, "process tx agc add cur device %d ", tx_device);
    #endif
                       }

}

void
process_rx_agc_data
(
  rflm_diag_power_buffer_item_t* buf_ptr, 
  rx_agc_event_data* rx_agc_storage,
  uint8* arr_cur_active_rx_device, 
  uint8* cur_num_of_active_rx_device
)
{
  rfm_device_enum_type rx_device = RFM_DEVICE_0;
  uint8 lna = 0;
  uint8 payload_len = buf_ptr->header.length;

  rx_device = (rfm_device_enum_type) buf_ptr->header.device_id;

  uint8 i;
  for (i = 0;i<payload_len;i++) 
  {                                                                                                                                                                             
    switch (buf_ptr->payload[i].chg_type) 
    {
     case RFLM_DIAG_POWER_TYPE_LNA:
       lna = buf_ptr->payload[i].chg_data;                                                                                                                                              
       break;                                                                                                                                                                  
    default:                                                                                                                                                                        
       break;                                                                                                                                                                       
    } /* switch (buf_ptr->payload[i].chg_type) */                                                                                                                                                                                                     
  } /* for (i = 0;i<payload_len;i++)  */

  rx_agc_storage->lna_state = lna;
  rx_agc_storage->receive_device_number = rx_device;
  rx_agc_storage->time_delta_us = 0;
  rx_agc_storage->reserved = 0;

  rx_agc_record[rx_device] = *rx_agc_storage; //copy storage to record indexed by device

  rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[rx_device][lna];
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_3(RF_MED, "rf_power_reporting - process_rx_agc_data bitmask %d, rx device %d, lna %d", 
            bitmask, 
            rx_device,  
            lna);
  #endif

  uint8 j;
  /* add to cur rx device list */
  uint8 add_cur_device = 1;
  for (j = 0; j < *cur_num_of_active_rx_device && j < RFM_MAX_WAN_DEVICES; j++) 
  {
     if(arr_cur_active_rx_device[j] == rx_device)
     {
       add_cur_device = 0;
     }
  }

  if (add_cur_device) 
  {
    arr_cur_active_rx_device[(*cur_num_of_active_rx_device)] = rx_device;
    (*cur_num_of_active_rx_device)++;
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_1(RF_MED,"process_rx_agc_data - add current RX device %d", rx_device);
    #endif
  }
}

void
process_tx_dynamic_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  tx_dynamic_state_event_data* tx_dynamic_storage, 
  uint8* num_tx_dynamic_events, 
  uint8* tx_dynamic_update,
  uint8* arr_cur_active_tx_device, 
  uint8* cur_num_of_active_tx_device
)
{
  uint8 payload_len = buf_ptr->header.length;  
  uint8 state = 0;
  uint8 i;
  rfm_device_enum_type tx_device;     
  for (i = 0;i < payload_len;i++) {              

   switch (buf_ptr->payload[i].chg_type) {                                                               
   case RFLM_DIAG_POWER_TYPE_STATE:{
      state = buf_ptr->payload[i].chg_data;
      tx_dynamic_storage->state = state+1;
                          break;
                       }
                       default:                                                                                                                                                                       
                          break;                                                                                                                                                                      
                    }
  }

  if(state == RFLM_DIAG_POWER_RFA_STATE_ENABLE || state == RFLM_DIAG_POWER_RFA_STATE_DISABLE){
     *tx_dynamic_update = 0;
     return;
  }

  *tx_dynamic_update = 1;
  (*num_tx_dynamic_events)++;

  tx_device = (rfm_device_enum_type) buf_ptr->header.device_id;
  rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[tx_device][0]; 
  tx_dynamic_storage->transmit_device_number = tx_device; 
                            
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG                          
  RF_MSG_2(RF_MED, "process tx dynamic data - bitmask %d, transmit device %d ", 
           bitmask,
           tx_device); 
  #endif

  tx_dynamic_storage->time_delta_us = 0;
  tx_dynamic_storage->reserved = 0;

  tx_dynamic_record[tx_device] = *tx_dynamic_storage; //copy temp storage to record indexed by device num
                                                                                                                                                                                        
  uint8 add_tx_device = 1;

  if (state == RFLM_DIAG_POWER_RFA_STATE_DISABLE) {
     add_tx_device = 0;
  }

  for (i = 0; i < num_of_active_tx_device; i++) {
     if (arr_active_tx_device[i] ==  tx_device) {
        if (state != RFLM_DIAG_POWER_RFA_STATE_DISABLE) {
           add_tx_device = 0;
        }
        else if(state == RFLM_DIAG_POWER_RFA_STATE_DISABLE){      
           uint8 j;
           for (j = i; j < (num_of_active_tx_device - 1); j++) {
               arr_active_tx_device[j] = arr_active_tx_device[j+1];
            }
           num_of_active_tx_device--;
           #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
           RF_MSG_1(RF_MED, "tx dynamic reduce device %d ", tx_device);
           #endif
       }                                                                                                                                                                                    
     }
  }

  if (add_tx_device) {
     arr_active_tx_device[num_of_active_tx_device] = tx_device;
     num_of_active_tx_device++;
     #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
     RF_MSG_1(RF_MED, "tx dynamic add device  %d", tx_device);
     #endif
  }

  /* add tx device to cur active device list */
  uint8 add_cur_tx_device = 1;
  for (i = 0; i < *cur_num_of_active_tx_device && i < RFM_MAX_WAN_DEVICES; i++) {
     if (arr_cur_active_tx_device[i] == tx_device) {
         add_cur_tx_device = 0;
     }
  }

  if (add_cur_tx_device) {
     arr_cur_active_tx_device[(*cur_num_of_active_tx_device)] = tx_device;
     (*cur_num_of_active_tx_device)++;
     #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
     RF_MSG_1(RF_MED, "tx dynamic add cur device %d ", tx_device);
     #endif
  }
}

void 
process_rx_dynamic_data
(
  rflm_diag_power_buffer_item_t  *buf_ptr, 
  rx_dynamic_state_event_data* rx_dynamic_storage,
  uint8* arr_cur_active_rx_device, 
  uint8* cur_num_of_active_rx_device
)
{
   #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
   RF_MSG(RF_MED, "rf_power_reporting - process_rx_dynamic_data");
   #endif
   uint8 payload_len = buf_ptr->header.length;
   uint8 handle_id = 0;
   uint8 state = 0;
   rfm_device_enum_type rx_device = RFM_DEVICE_0;

   uint8 i;       
   for (i = 0; i<payload_len; i++) {              

     switch (buf_ptr->payload[i].chg_type) 
     {
     case RFLM_DIAG_POWER_TYPE_STATE:
        state = buf_ptr->payload[i].chg_data;
        if (state == 1) 
        {
           rx_dynamic_storage->state = 2;
                    }
        else
        {
           rx_dynamic_storage->state = 0;
        }        
                          break;
                    default:                                                                                                                                                                        
                       break;                                                                                                                                                                       
     }
   }
                                                                                                                                                                                                 
   rx_device = (rfm_device_enum_type) buf_ptr->header.device_id;
   rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[rx_device][0]; 
   rx_dynamic_storage->path = get_path_config(bitmask);
   rx_dynamic_storage->time_delta_us = 0;
   rx_dynamic_storage->receive_device_number = rx_device;

   #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
   RF_MSG_5(RF_MED, "rf_power_reporting - process_rx_dynamic_data handle id %d, bitmask %d, rx device %d, state %d, path %d", 
            handle_id,
            bitmask,
            rx_device,
            state,
            rx_dynamic_storage->path
            );
   #endif

   rx_dynamic_record[rx_device] = *rx_dynamic_storage;

   uint8 j;
   uint8 add_device = 1;

   if (state != 1) {
      add_device = 0;
   }

   for (j = 0; j < num_of_active_rx_device; j++) {
      if(arr_active_rx_device[j] == rx_device){
         if (state != 1) {
            uint8 m;
            for (m = j; m < (num_of_active_rx_device - 1); m++) {
               arr_active_rx_device[m] = arr_active_rx_device[m+1];
           }
            num_of_active_rx_device--;
            #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
            RF_MSG_1(RF_MED,"process_rx_dynamic_data - delete RX device %d", rx_device);  
            #endif
        }
        else{
            add_device = 0;
         }
      }
   }

   if (add_device) {
      arr_active_rx_device[num_of_active_rx_device] = rx_device;
      num_of_active_rx_device++;
      RF_MSG_1(RF_MED,"process_rx_dynamic_data - add RX device %d", rx_device);
   }

   /* add to cur rx device list */
   uint8 add_cur_device = 1;
   for (j = 0; j < *cur_num_of_active_rx_device && j < RFM_MAX_WAN_DEVICES; j++) {
      if(arr_cur_active_rx_device[j] == rx_device){
           add_cur_device = 0;
      }
   }

   if (add_cur_device) {
      arr_cur_active_rx_device[(*cur_num_of_active_rx_device)] = rx_device;
      (*cur_num_of_active_rx_device)++;
      #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
      RF_MSG_1(RF_MED,"process_rx_dynamic_data - add current RX device %d", rx_device);
      #endif
   }

}

void
process_missing_rx_device
(
  uint8* cur_num_of_active_rx_device_ptr,
  uint8* missing_num_of_rx_device_ptr,
  uint8* arr_missing_active_rx_device,
  uint8* arr_cur_active_rx_device
)
{
  if ( (*cur_num_of_active_rx_device_ptr) < num_of_active_rx_device ) 
  {

    (*missing_num_of_rx_device_ptr) = num_of_active_rx_device - (*cur_num_of_active_rx_device_ptr) ;
    /* add missing device */
    uint8 i;
    for (i = 0; i < num_of_active_rx_device; i ++) 
    {
      uint8 j;
      uint8 add_device = TRUE;
      for (j = 0; j < (*cur_num_of_active_rx_device_ptr); j ++ ) 
      {
        if (arr_cur_active_rx_device[j] == arr_active_rx_device[i]) 
        {
          add_device = FALSE;
          break;
        }
      }

      if (add_device == TRUE) 
      {
        arr_cur_active_rx_device[ (*cur_num_of_active_rx_device_ptr) ] = arr_active_rx_device[i];
        (*cur_num_of_active_rx_device_ptr)++;
        arr_missing_active_rx_device[ (*missing_num_of_rx_device_ptr) ] = arr_active_rx_device[i];
        (*missing_num_of_rx_device_ptr)++; 
        #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
        RF_MSG_1(RF_MED, "processing missing rx device rx device %d", arr_active_rx_device[i]);    
        #endif
      }
    }
    /* add missing device */
        }        

}

uint8 convert_band_enum(uint8 tech, uint8 band){
   uint8 result = band;
   switch (tech) {
   case TECH_CDMA: //same as enum
                    break;
   case TECH_LTE:
        if (band <= RFCOM_BAND_LTE_B44) {
           result = band+1; 
        }
        else if (band == 44 ) {
           result = 28;
        }
        else if (band == 45) {
           result = 40;
        }
        else if (band == 46 || band == 47) {
           result = 41;
        }
        else if (band == 48) {
           result = 49;
                 }
        else if (band >= 49) {
           result = 125 + band - 49;
        }
       break;
   case TECH_WCDMA: 
       if ( band >= RFCOM_BAND_INVALID ) 
       {
         band = 0;
       }
       result = wcdma_band[band]; 
       break;
   case TECH_GSM:
       if ( band >= RFCOM_NUM_GSM_BANDS ) 
       {
         band = 0;
       }
       result = gsm_band[band]; 
       break;
   case TECH_TDSCDMA:
       if ( band >= RFCOM_NUM_TDSCDMA_BANDS ) 
       {
         band = 0;
       }
       result = tds_band[band];
       break;
   default:                                                                                                                                                                        
       result = band;
       break;                                                                                                                                                                    
   }
   return result;
}

uint8 convert_bw_enum(uint8 tech, uint8 bw){
   uint8 result = 0;
   switch (tech) {
   case TECH_LTE:
      if ( bw >= RFCOM_BW_LTE_INVALID ) 
      {
        bw = 0;
      }
      result = lte_bw[bw];
      break;
   case TECH_WCDMA:
      if ( bw >= RFCOM_BW_WCDMA_INVALID ) 
      {
        bw = 0;
      }
      result = wcdma_bw[bw];
      break;
   case TECH_CDMA:
      if ( bw >= RFCOM_BW_WCDMA_INVALID ) 
      {
        bw = 0;
      }
      result = cdma_bw[bw];
      break;
   case TECH_GSM:
      result = 1;
      break;
   case TECH_TDSCDMA:
      result = 1;
      break;
   default: 
      result = bw; 
      break;
   }
   return result;
}

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
)
{
  uint8 i;
  for (i = 0; i < num_tx_agc_events; i++) {
     if (tx_agc_update) {
         uint8 j =  array_tx_agc_event_index[i];
         uint8 roll = 0;
         uint32 prev_timestamp = local_ustmr;                                                                                                                                                                                                                                
         if (buf_ptr[j].timestamp < prev_timestamp) {
            roll++;
            tx_agc_storage[i].time_delta_us = (((uint64)roll << 32) | buf_ptr[j].timestamp) - local_ustmr;
         }
         else{
            tx_agc_storage[i].time_delta_us = buf_ptr[j].timestamp - local_ustmr;
         }
         prev_timestamp = buf_ptr[j].timestamp;
         tx_agc_event[i] = tx_agc_storage[i];
     }
     else{  
        uint8 tx_device;
        if (!tx_update) {
           tx_device = arr_active_tx_device[i];
        }
        else{
          if (i >= RFM_MAX_WAN_DEVICES) {
            i = 0;
          }
          tx_device = arr_cur_active_tx_device[i];
        }
        tx_agc_event[i] = tx_agc_record[tx_device];
        tx_agc_event[i].transmit_device_number = tx_device;
        #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
        RF_MSG_1(RF_MED, "tx agc update false -> tx device %d", tx_device); 
        #endif
     }
                 }   
}

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
)
{
  uint8 i ;
  if (rx_agc_update) {
    for (i = 0; i < num_logged_rx_agc_events; i++) {
        uint8 j = array_rx_agc_event_index[i];
        uint8 roll = 0;
        uint32 prev_timestamp = local_ustmr;
        if (buf_ptr[j].timestamp < prev_timestamp) {
            roll++;
            rx_agc_storage[i].time_delta_us = (((uint64)roll << 32) | buf_ptr[j].timestamp) - local_ustmr;
        }
        else{
            rx_agc_storage[i].time_delta_us = buf_ptr[j].timestamp - local_ustmr;
        }
        prev_timestamp = buf_ptr[j].timestamp;
        rx_agc_event[i] = rx_agc_storage[i];
    }

    for (i = 0; i < missing_num_of_rx_device && i < RFM_MAX_WAN_DEVICES; i++) {
        uint8 rx_device = arr_missing_active_rx_device[i];
        rx_agc_event[num_logged_rx_agc_events + i] = rx_agc_record[rx_device];
    }
  } /* if (rx_agc_update) */
  else{
   #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
   RF_MSG_1(RF_MED, "rx_agc_update = %d", 0);
   #endif
   if ( rx_dynamic_update ) {
     for (i = 0; i < cur_num_of_active_rx_device && i < RFM_MAX_WAN_DEVICES; i++) {
       uint8 rx_device = arr_cur_active_rx_device[i];
       rx_agc_event[i] = rx_agc_record[rx_device];
       rx_agc_event[i].receive_device_number = rx_device;
     }
   } /* if ( rx_dynamic_update ) */
   else{
     for (i = 0; i < num_of_active_rx_device; i++) {
       uint8 rx_device = arr_active_rx_device[i];
       rx_agc_event[i] = rx_agc_record[rx_device];
       rx_agc_event[i].receive_device_number = rx_device;
     }
   } /* else if ( rx_dynamic_update ) */        
  } /* else if (rx_agc_update) */
}

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
)
{
  uint8 i;
  for (i = 0; i < num_tx_dynamic_events; i++) {
     if (tx_dynamic_update) {
        uint8 j = array_tx_dynamic_event_index[i];
        uint8 roll = 0;
        uint32 prev_timestamp = local_ustmr;
        if (buf_ptr[j].timestamp < prev_timestamp) {
           roll++;
           tx_dynamic_storage[i].time_delta_us = (((uint64)roll << 32) | buf_ptr[j].timestamp) - local_ustmr;
        }
        else{
            tx_dynamic_storage[i].time_delta_us = buf_ptr[j].timestamp - local_ustmr;
               }
        prev_timestamp = buf_ptr[j].timestamp;
        tx_dynamic_state_event[i] = tx_dynamic_storage[i];
           }
     else{
        uint8 tx_device;
        if (tx_update) {
           if (i >= RFM_MAX_WAN_DEVICES) {
             i = 0;
           }
           tx_device = arr_cur_active_tx_device[i];
        }
        else{
           tx_device = arr_active_tx_device[i];
        }
        tx_device = arr_active_tx_device[i];
        tx_dynamic_state_event[i] = tx_dynamic_record[tx_device];
        tx_dynamic_state_event[i].transmit_device_number = tx_device;
        #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
        RF_MSG_1(RF_MED, "tx_dynamic_update false - tx_device %d", tx_device); 
        #endif
        }
  }
}
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
  uint8  missing_num_of_rx_device,
  uint8* arr_missing_active_rx_device        
)
{
  uint8 i;
  for (i = 0; i < num_rx_dynamic_events; i++) {
     if (rx_dynamic_update) {
        if ( i == num_logged_rx_dynamic_events) {
                    break;
                 }
        uint8 j = array_rx_dynamic_event_index[i];
        uint8 roll = 0;
        uint32 prev_timestamp = local_ustmr ;
        if (buf_ptr[j].timestamp < prev_timestamp) {
           roll++;
           rx_dynamic_storage[i].time_delta_us = (((uint64)roll << 32) | buf_ptr[j].timestamp) - local_ustmr;
        }
        else{
            rx_dynamic_storage[i].time_delta_us = buf_ptr[j].timestamp - local_ustmr; 
        }
        prev_timestamp = buf_ptr[j].timestamp;
        rx_dynamic_state_event[i] = rx_dynamic_storage[i];
     }
     else{
         uint8 rx_device_num;
         if(!rx_update){
           rx_device_num = arr_active_rx_device[i];
         }
         else
         {
           if ( i >= RFM_MAX_WAN_DEVICES ) 
           {
             i = 0;
           }
           rx_device_num = arr_cur_active_rx_device[i];
         }
         rx_dynamic_state_event[i] = rx_dynamic_record[rx_device_num];
         rx_dynamic_state_event[i].receive_device_number = rx_device_num;
         #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
         RF_MSG_1(RF_MED, "rx_dynamic_update false - rx_device %d", rx_device_num); 
         #endif
     }
                                                                                                                                                                                
               }

  for (i = 0; i < missing_num_of_rx_device && i < RFM_MAX_WAN_DEVICES; i++) {
    uint8 rx_device = arr_missing_active_rx_device[i];
    rx_dynamic_state_event[num_logged_rx_dynamic_events + i] = rx_dynamic_record[rx_device];
           }
}

void
populate_tx_config_data
(
  tx_config_event_data* tx_config_event,
  uint8 num_tx_config_events,
  uint8 tx_update,
  uint8* arr_cur_active_tx_device
)
{
  uint8 i;
  for (i = 0; i < num_tx_config_events; i++) {
    uint8 tx_device;
    if (tx_update) {
       if (i >= RFM_MAX_WAN_DEVICES) {
         i = 0;
       }
       tx_device = arr_cur_active_tx_device[i];
    }
    else{
       tx_device = arr_active_tx_device[i];
    }
    rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[tx_device][0];

    tx_config_event[i].time_delta_us = 0; 
    tx_config_event[i].transmit_device_number = tx_device;
    tx_config_event[i].tech = get_tech(bitmask);

    switch (get_tech(bitmask)) {
     case RFDEVICE_WTR_CFG_TECH_TDSCDMA:{
        tx_config_event[i].tech = TECH_TDSCDMA;
        break;
     }
     case RFDEVICE_WTR_CFG_TECH_GSM:{
        tx_config_event[i].tech = TECH_GSM;
        break;
     }
     case RFDEVICE_WTR_CFG_TECH_LTEFDD:{
        tx_config_event[i].tech = TECH_LTE;
        break;
     }
     case RFDEVICE_WTR_CFG_TECH_LTETDD:{
        tx_config_event[i].tech = TECH_LTE;
        break;
     }
     default: break;
    }
                                                                                                                                                                                              
    tx_config_event[i].band = convert_band_enum(tx_config_event[i].tech, get_band(bitmask));
    tx_config_event[i].bandwidth = convert_bw_enum(tx_config_event[i].tech, get_bandwidth(bitmask));             
    tx_config_event[i].wtr_chip_type = get_chip_number(bitmask);
    tx_config_event[i].data_revision = DATA_REVISION; // ??
    tx_config_event[i].freq_khz = arr_channel[tx_device];
    tx_config_event[i].pa_chip_type = 0; //???
    tx_config_event[i].reserved = 0;
    tx_config_event[i].pa_data_revision = PA_DATA_REVISION;
  }
}

void
populate_rx_config_data
(
  rx_config_event_data* rx_config_event,
  uint8 num_rx_config_events,
  uint8 rx_update,
  uint8* arr_cur_active_rx_device
)
{
  uint8 i;
  for (i = 0; i < num_rx_config_events; i++) {
     uint8 rx_device;
     if(!rx_update){
       rx_device = arr_active_rx_device[i];
        }
     else
     {
       if ( i >= RFM_MAX_WAN_DEVICES ) 
       {
         i = 0;
       }
       rx_device = arr_cur_active_rx_device[i]; 
     }
    
     rfdevice_wtr_config_bitmask_type bitmask = bitmask_arr[rx_device][0];

     rx_config_event[i].data_revision = DATA_REVISION;
     rx_config_event[i].freq_khz = arr_channel[rx_device];
     rx_config_event[i].receive_device_number = rx_device;
     rx_config_event[i].reserved = 0;
     rx_config_event[i].tech = get_tech(bitmask);
     switch (get_tech(bitmask)) {
        case RFDEVICE_WTR_CFG_TECH_TDSCDMA:{
           rx_config_event[i].tech = TECH_TDSCDMA;
           break;
        }
        case RFDEVICE_WTR_CFG_TECH_GSM:{
           rx_config_event[i].tech = TECH_GSM;
           break;
        }
        case RFDEVICE_WTR_CFG_TECH_LTEFDD:{
           rx_config_event[i].tech = TECH_LTE;
           break;
        }
        case RFDEVICE_WTR_CFG_TECH_LTETDD:{
           rx_config_event[i].tech = TECH_LTE;
                    break;
                 }

                 }

     rx_config_event[i].band = convert_band_enum(rx_config_event[i].tech, get_band(bitmask));
     rx_config_event[i].bandwidth = convert_bw_enum(rx_config_event[i].tech, get_bandwidth(bitmask));
     rx_config_event[i].time_delta_us = 0;
     rx_config_event[i].wtr_chip_type = get_chip_number(bitmask); 
        }
}

void 
populate_fbrx_data
(
  fbrx_state_event_data* fbrx_state_event,
  uint8 num_fbrx_state_events,
  uint8* array_fbrx_state_event_index,
  rflm_diag_power_buffer_item_t  *buf_ptr
)
{
  uint8 temp_fbrx_event_index;
  rfm_device_enum_type tx_device;  
        for(temp_fbrx_event_index = 0;temp_fbrx_event_index<num_fbrx_state_events;temp_fbrx_event_index++) {   
                                                                                         
               uint8 buffer_fbrx_event_index =  array_fbrx_state_event_index[temp_fbrx_event_index];                                                                                                                                                                                                  
               uint8 temp_fbrx_event_payload_index; 
               uint8 payload_len = buf_ptr[buffer_fbrx_event_index].header.length; 
     uint8 handle = 0;
     for (temp_fbrx_event_payload_index = 0;temp_fbrx_event_payload_index < payload_len;temp_fbrx_event_payload_index++) {              
                                                                                                                                                                              
                 switch (buf_ptr[buffer_fbrx_event_index].payload[temp_fbrx_event_payload_index].chg_type) {                                                               
       case RFLM_DIAG_POWER_TYPE_FBRX_DURATION:{
          fbrx_state_event[temp_fbrx_event_index].duration = buf_ptr[buffer_fbrx_event_index].payload[temp_fbrx_event_payload_index].chg_data;                                                                                                                                                  
                    break;
                 }
                 case RFLM_DIAG_POWER_TYPE_FBRX_GAIN_STATE:{
                    fbrx_state_event[temp_fbrx_event_index].gain = buf_ptr[buffer_fbrx_event_index].payload[temp_fbrx_event_payload_index].chg_data;
                    break;
       }
                 default:                                                                                                                                                                        
                    break;                                                                                                                                                                       
                 }   
                 
                                                                                                                                                                                
               }
     tx_device = (rfm_device_enum_type) buf_ptr->header.device_id;
     fbrx_state_event[temp_fbrx_event_index].time_delta_us = buf_ptr[buffer_fbrx_event_index].timestamp - local_ustmr;
     fbrx_state_event[temp_fbrx_event_index].tx_device_number =  tx_device;
  }
}
}
