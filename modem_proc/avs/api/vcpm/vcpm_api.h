#ifndef _VCPM_API_H_
#define _VCPM_API_H_

/**<  @file vcpm_api.h
      @brief This file contains Voice Call Proxy Manager Commands and Data Structures */

/*==============================================================================
  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*==============================================================================
                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $header:
 
==============================================================================*/

/*------------------------------------------------------------------------------
 *  Header Includes
 *----------------------------------------------------------------------------*/
#include "mmdefs.h"

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

/*------------------------------------------------------------------------------
 *  Module ID Definitions
 *----------------------------------------------------------------------------*/

 /**
    The instance ID of VCPM static module.
 */
#define VCPM_MODULE_INSTANCE_ID     ( 0x00000004 )

/**
    The instance ID of Voice Services static module.
 */
#define VOICE_SERVICES_MODULE_INSTANCE_ID     ( 0x00000005 )

/** ID of the parameter used to set the TTY mode */
#define VCPM_PARAM_ID_TTY_MODE 0x080011E4

/** @h2xmlp_parameter   {"VCPM_PARAM_ID_TTY_MODE", VCPM_PARAM_ID_TTY_MODE}
    @h2xmlp_description {Parameter for setting the TTY mode of operation}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "gk_begin_pack.h"
struct vcpm_param_id_tty_mode_t
{
   uint32_t vsid;
   /**< @h2xmle_description {vsid}*/

   uint32_t mode;
   /**< @h2xmle_description {TTY mode to be set}
        @h2xmle_rangeList   {"VOICE_TTY_MODE_OFF"=0,
                             "VOICE_TTY_MODE_HCO"=1,
                             "VOICE_TTY_MODE_VCO"=2,
                             "VOICE_TTY_MODE_FULL"=3}
        @h2xmle_policy      {Basic} */

}
#include "gk_end_pack.h"
;
typedef struct vcpm_param_id_tty_mode_t vcpm_param_id_tty_mode_t;


/**
   This Param ID contains the information of Module Instace IDs of specific modules
    present in a stream sub graph that is part of a voice call use case graph. 
   This Param is to be set to VCPM Instance ID by HLOS at the time of graph open.
*/
#define VCPM_PARAM_ID_VOICE_CONFIG              0x08001162

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_VOICE_STREAM_MOD_INFO'.
   Immediately following this structure is an array of structures 
     of type 'vcpm_voice_subgraph_tag_info_t'. 
   Array length is equal to 'num_voice_subgraph_info_objects'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_voice_config_payload_t
{
   uint32_t num_sub_graphs;
   /**< Number of vcpm_cfg_subgraph_properties_t structures and their payloads
        that follow immediately after this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_voice_config_payload_t vcpm_param_voice_config_payload_t;

#include "gk_begin_pack.h"

struct vcpm_cfg_subgraph_properties_t
{
   uint32_t sub_graph_id;
   /**< Sub Graph ID for which the information is provided. */

   uint32_t num_vcpm_properties;
   /**< Number of "vcpm_voice_stream_mod_iid_tag_t" structures that follow this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_cfg_subgraph_properties_t vcpm_cfg_subgraph_properties_t;

#include "gk_begin_pack.h"

struct vcpm_property_config_struct_t
{
   uint32_t property_id;
   /**< VCPM propertyID. */

   uint32_t property_size;
   /**< Size of the property payload that immediately follows this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_property_config_struct_t vcpm_property_config_struct_t;


/**
   This Property ID contains the information of Module Instace IDs of specific modules present
   in different sub graphs that are part of a voice call use case graph.
   This Property is to be set to VCPM as part of 'VCPM_PARAM_ID_VOICE_CONFIG's payload.
*/
#define VCPM_PROPERTY_ID_TAG_INFO              0x080011B2

#include "gk_begin_pack.h"

struct vcpm_property_id_tag_info_payload_t
{
   uint32_t num_tag_info;
   /* Number of vcpm_tag_miid_info_t structures that immediately follow after this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_property_id_tag_info_payload_t vcpm_property_id_tag_info_payload_t;


#include "gk_begin_pack.h"

struct vcpm_tag_miid_info_t
{
   uint32_t tag_id;
   /* a Tag ID published by VCPM.
        Supported Tag IDs:
         VOICE_MOD_TAG_ID_ENCODER
         VOICE_MOD_TAG_ID_TX_MAILBOX
         VOICE_MOD_TAG_ID_TX_STREAM_HPCM
         VOICE_MOD_TAG_ID_DECODER
         VOICE_MOD_TAG_ID_RX_MAILBOX
         VOICE_MOD_TAG_ID_RX_STREAM_HPCM
         VOICE_MOD_TAG_ID_CTMTTY_TX
         VOICE_MOD_TAG_ID_LTETTY_TX
         VOICE_MOD_TAG_ID_CTMTTY_RX
         VOICE_MOD_TAG_ID_LTETTY_RX
         VOICE_MOD_TAG_ID_1XTTY_RX
         VOICE_MOD_TAG_ID_TX_HW_EP
         VOICE_MOD_TAG_ID_RX_HW_EP
       VOICE_MOD_TAG_ID_RX_TRM
       VOICE_MOD_TAG_ID_TX_SMART_SYNC
   */

   uint32_t module_iid;
   /**< Instance ID of the module corresponding to the tag_id.*/
}

#include "gk_end_pack.h"
;

typedef struct vcpm_tag_miid_info_t vcpm_tag_miid_info_t;


/**< Tag IDs for the voice graph modules. */

#define VOICE_MOD_TAG_ID_ENCODER 0x08001177

#define VOICE_MOD_TAG_ID_TX_MAILBOX   0x08001178

#define VOICE_MOD_TAG_ID_TX_STREAM_HPCM   0x08001179

#define VOICE_MOD_TAG_ID_DECODER  0x0800117B

#define VOICE_MOD_TAG_ID_RX_MAILBOX    0x0800117C

#define VOICE_MOD_TAG_ID_RX_STREAM_HPCM   0x0800117D

#define VOICE_MOD_TAG_ID_RX_TRM      0x080011AC

#define VOICE_MOD_TAG_ID_TX_SMART_SYNC   0x080011AD

#define VOICE_MOD_TAG_ID_LTETTY_TX   0x0800117A

#define VOICE_MOD_TAG_ID_CTMTTY_TX   0x080011E1

#define VOICE_MOD_TAG_ID_LTETTY_RX   0x0800117E

#define VOICE_MOD_TAG_ID_CTMTTY_RX   0x080011E2

#define VOICE_MOD_TAG_ID_1XTTY_RX   0x080011E3


/**
   Param ID to register the calibration table(s) per Sub Graph.
*/
#define VCPM_PARAM_ID_CAL_TABLE              0x08001163

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_CAL_TABLE'.
   Immediately following this structure is an array of structures 
      of type 'vcpm_sgid_cal_table_t'. 
   Array length is equal to 'num_sub_graphs'.

*/
#include "gk_begin_pack.h"

struct vcpm_param_cal_table_payload_t
{
   uint32_t num_sub_graphs;
   /**< Number of voice sub graphs for which calibration tables are provided. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_cal_table_payload_t vcpm_param_cal_table_payload_t;

#include "gk_begin_pack.h"

struct vcpm_sgid_cal_table_t
{
   uint32_t sub_graph_id;
   /**< Sub graph ID for the table that follows this structure. */

   uint32_t cal_table_size;
   /**< Size of the calibration table that follows this structure.*/

   /*uint8_t cal_table[cal_table_size] */
   /**< immediately following this structure is the calibration table of size 'cal_table_size'. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_sgid_cal_table_t vcpm_sgid_cal_table_t;

/** Calibration Key ID to identify the Vocoder class.*/
#define VCPM_CAL_KEY_ID_VOCODER_CLASS  0x08001164

/** Calibration Key ID to identify the Sampling rate.*/
#define VCPM_CAL_KEY_ID_SAMPLING_RATE  0x08001165

/** Calibration Key ID to identify the Volume level.*/
#define VCPM_CAL_KEY_ID_VOLUME_LEVEL   0x08001166

/** Calibration Key ID to identify the bandwidth extension mode. */
#define VCPM_CAL_KEY_ID_BWE            0x08001167

/** Calibration Key ID to identify the Network. */
#define VCPM_CAL_KEY_ID_NETWORK        0x080011B5

/** Calibration Key ID to identify the Volume boost mode. */
#define VCPM_CAL_KEY_ID_VOL_BOOST      0x080011B6

/**< Values supported for the key ID 'VCPM_CAL_KEY_ID_VOCODER_CLASS'*/
#define VCPM_CAL_KEY_VOCODER_CLASS_AMR  0x08001168
#define VCPM_CAL_KEY_VOCODER_CLASS_EVRC 0x08001169
#define VCPM_CAL_KEY_VOCODER_CLASS_EVS  0x0800116A

/**< Values supported for the key ID 'VCPM_CAL_KEY_ID_NETWORK'*/
#define VCPM_CAL_KEY_NETWORK_CDMA        0x080011B7
#define VCPM_CAL_KEY_NETWORK_GSM         0x080011B8
#define VCPM_CAL_KEY_NETWORK_WCDMA       0x080011B9
#define VCPM_CAL_KEY_NETWORK_LTE         0x080011BA
#define VCPM_CAL_KEY_NETWORK_TDSCDMA     0x080011BB

/**
   Param ID to set the calibration keys to VCPM.
*/
#define VCPM_PARAM_ID_CAL_KEYS              0x0800116B

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_CAL_KEYS'.
   Immediately following this structure is an array of structures 
      of type 'vcpm_ckv_pair_t'. 
   Array length is equal to 'num_sub_graphs'.

*/
#include "gk_begin_pack.h"

struct vcpm_param_cal_keys_payload_t
{
   uint32_t vsid;
   /**< vsid for which the param payload is provided.. */

   uint32_t num_ckv_pairs;
   /**< Number of vcpm_ckv_pair_t structures that
        follow immediately after this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_cal_keys_payload_t vcpm_param_cal_keys_payload_t;

#include "gk_begin_pack.h"

struct vcpm_ckv_pair_t
{
   uint32_t cal_key_id;
   /**< Calibration key ID. */

   uint32_t value;
   /**< value of the key. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_ckv_pair_t vcpm_ckv_pair_t;

/**
   Param ID to set the calibration keys to VCPM.
*/
#define VCPM_PARAM_ID_ACTIVE_CAL_KEYS         0x080011C1

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_ACTIVE_CAL_KEYS'.
   Immediately following this structure is an array of structures 
      of type 'vcpm_ckv_pair_t'. 
   Array length is equal to 'num_ckv_pairs'.

   This param supports only GET_CFG cmd from client. SET_CFG is not supported.

*/
#include "gk_begin_pack.h"

struct vcpm_param_active_cal_keys_payload_t
{
   uint32_t sg_id;
   /**< Sub Graph id for which the param payload is provided.. */

   uint32_t num_max_ckv_pairs;
   /**< Number of maximum ckv_pairs of the given sg id. */
   /** Client fills this value when APM_CMD_GET_CFG is called for this param_id. */

   uint32_t num_ckv_pairs;
   /**< Number of vcpm_ckv_pair_t structures that
        follow immediately after this structure. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_active_cal_keys_payload_t vcpm_param_active_cal_keys_payload_t;

/**
   Param ID to set the number of volume steps to VCPM.
*/
#define VCPM_PARAM_ID_NUM_VOL_STEPS              0x0800116C

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_NUM_VOL_STEPS'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_num_vol_steps_payload_t
{
   uint32_t vsid;
   /**< vsid for which the param payload is provided.. */

   uint32_t num_vol_steps;
   /**< Total number of volume steps for the vsid session.*/

}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_num_vol_steps_payload_t vcpm_param_num_vol_steps_payload_t;

#define VCPM_PARAM_ID_CLK_CTRL 0x080011AE

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_CLK_CTRL'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_clk_ctrl_payload_t
{
   uint32_t kpps_scale_factor;
   /**< Scale factor to be applied on aggregated kpps of subgraph. */

   uint32_t bus_bw_scale_factor;
   /**< Scale factor to be applied on aggregated bus bw of subgraph.*/

}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_clk_ctrl_payload_t vcpm_param_clk_ctrl_payload_t;

/**
   Event to indicate that a voice call graph is opened.
*/
#define VCPM_EVENT_ID_GRAPH_OPEN   0x080011AF

/**
   Payload structure for the event 'VCPM_EVENT_ID_GRAPH_OPEN'.

*/
#include "gk_begin_pack.h"

struct vcpm_event_graph_open_payload_t
{
   uint32_t vsid;
   /**< VSID of the graph that is being opened.*/
   
   uint32_t num_tag_info;
   /* Number of vcpm_vs_tag_miid_info_t structures that immediately follow after this. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_event_graph_open_payload_t vcpm_event_graph_open_payload_t;

#include "gk_begin_pack.h"

/* Payload struct for the tag info that is sent to VS.*/
struct vcpm_vs_tag_miid_info_t
{
   uint32_t sg_id;
   /* Subgraph ID corresponding to the module_iid.*/

   uint32_t tag_id;
   /* a Tag ID published by VCPM.
        Supported Tag IDs:
         VOICE_MOD_TAG_ID_ENCODER
         VOICE_MOD_TAG_ID_TX_MAILBOX
         VOICE_MOD_TAG_ID_TX_STREAM_HPCM
         VOICE_MOD_TAG_ID_DECODER
         VOICE_MOD_TAG_ID_RX_MAILBOX
         VOICE_MOD_TAG_ID_RX_STREAM_HPCM
         VOICE_MOD_TAG_ID_CTMTTY_TX
         VOICE_MOD_TAG_ID_LTETTY_TX
         VOICE_MOD_TAG_ID_CTMTTY_RX
         VOICE_MOD_TAG_ID_LTETTY_RX
         VOICE_MOD_TAG_ID_1XTTY_RX
         VOICE_MOD_TAG_ID_TX_HW_EP
         VOICE_MOD_TAG_ID_RX_HW_EP
       VOICE_MOD_TAG_ID_RX_TRM
       VOICE_MOD_TAG_ID_TX_SMART_SYNC
   */

   uint32_t module_iid;
   /**< Instance ID of the module corresponding to the tag_id.*/
}
#include "gk_end_pack.h"
;

typedef struct vcpm_vs_tag_miid_info_t vcpm_vs_tag_miid_info_t;

/**
   Event to indicate that a voice call graph is closed.
*/

#define VCPM_EVENT_ID_GRAPH_CLOSE   0x080011B0

/**
   Payload structure for the event 'VCPM_EVENT_ID_GRAPH_CLOSE'.
*/
#include "gk_begin_pack.h"

struct vcpm_event_graph_close_payload_t
{
   uint32_t vsid;
   /**< VSID of the graph that is closed. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_event_graph_close_payload_t vcpm_event_graph_close_payload_t;


/**
   Event to indicate client Resync cmd is processed.
*/
#define VCPM_EVENT_ID_RESYNC_DONE   0x0800135C

/**
   Payload structure for the event 'VCPM_EVENT_ID_RESYNC_DONE'.

*/
#include "gk_begin_pack.h"

struct vcpm_event_resync_done_t
{
   uint32_t status;
   /* Status of the resync cmd. */

   uint32_t token;
   /* Token capturing the client info. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_event_resync_done_t vcpm_event_resync_done_t;

/**
	Event to report Tx/Rx path delays to the VS.
*/
#define VCPM_EVENT_ID_PATH_DELAY   0x0800135D


/** @ingroup spf_vpcm_events
    Payload for #VCPM_EVENT_ID_PATH_DELAY.
 */
#include "gk_begin_pack.h"
struct vcpm_event_path_delay_payload_t
{
   uint32_t tx_path_delay_us;     /**< Tx voice path delay in microseconds. */
   uint32_t rx_path_delay_us;     /**< Rx voice path delay in microseconds. */
}
#include "gk_end_pack.h"
;
typedef struct vcpm_event_path_delay_payload_t vcpm_event_path_delay_payload_t;

/* Parameter id to be used to set timing params on VCPM.
 */
#define VCPM_PARAM_ID_TIMING_PARAMS  0x0800116E  

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_TIMING_PARAMS'.
*/
#include "gk_begin_pack.h"
/** @h2xmlp_parameter   {"VCPM_PARAM_ID_TIMING_PARAMS", VCPM_PARAM_ID_TIMING_PARAMS}
    @h2xmlp_description {Parameter id to be used to set timing
                         params on VCPM. }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

struct vcpm_param_timing_params_t
{
  uint32_t vfr_mode;
  /**< @h2xmle_description {Vocoder frame synchronization mode.} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0, 1 }
        @h2xmle_policy      {Basic} */             

  uint32_t vsid;
  /**< @h2xmle_description {Voice System ID as defined @80-NF774-2,80-NA610-2.}
          @h2xmle_default     {0} 
          @h2xmle_range       { 0x11C05000, 0x11DC5000 }
          @h2xmle_policy      {Basic} */             

  uint32_t tx_delivery_offset_us;
  /**< @h2xmle_description { This represents the offset in microseconds 
                            from the VFR to deliver a Tx vocoder packet.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */       
 
  uint32_t rx_start_offset_us;
  /**< @h2xmle_description { This represents the offset in microseconds 
                            from the VFR to indicate the deadline to receive an Rx vocoder packet.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */      

  uint32_t vfr_cycle_us;
  /**< @h2xmle_description {VFR cycle duration for the voice call.}
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */       

}
#include "gk_end_pack.h"
;

typedef struct vcpm_param_timing_params_t vcpm_param_timing_params_t;



/* Parameter id to be used to set timing references on VCPM.
 */
#define VCPM_PARAM_ID_TIMING_REFERENCES  0x080010A5

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_TIMING_REFERENCES'.
*/
#include "gk_begin_pack.h"
/** @h2xmlp_parameter   {"VCPM_PARAM_ID_TIMING_REFERENCES", VCPM_PARAM_ID_TIMING_REFERENCES}
    @h2xmlp_description {Parameter id to be used to set timing
                         references on VCPM. }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

struct vcpm_param_timing_references_t
{
  uint32_t vfr_mode;
  /**< @h2xmle_description {Vocoder frame synchronization mode.} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0, 1 }
        @h2xmle_policy      {Basic} */             

  uint32_t vsid;
  /**< @h2xmle_description {Voice System ID as defined @80-NF774-2,80-NA610-2.}
          @h2xmle_default     {0} 
          @h2xmle_range       { 0x11C05000, 0x11DC5000 }
          @h2xmle_policy      {Basic} */             

  uint64_t tx_ref_timstamp_us;
  /**< @h2xmle_description { Client's encoder packet exchange time reference (in microseconds).
                             The time reference is used to align the encoder's timeline to the
                             client timeline at the beginning of the vocoder's packet exchange
                             operation. The clock source to be used for interpreting the timestamp is Qtimer.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */       
 
  uint64_t rx_ref_timstamp_us;
  /**< @h2xmle_description { Client's decoder packet exchange time reference (in microseconds).
                             The time reference is used to align the decoder's timeline to the
                             client timeline at the beginning of the vocoder's packet exchange
                             operation. The clock source to be used for interpreting the timestamp is Qtimer.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */      

  uint32_t vfr_cycle_us;
  /**< @h2xmle_description {VFR cycle duration for the voice call.}
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */       

}
#include "gk_end_pack.h"
;

typedef struct vcpm_param_timing_references_t vcpm_param_timing_references_t;


/* Parameter id to be used to set timing references v2 on VCPM.
 */
#define VCPM_PARAM_ID_TIMING_REFERENCES_V2  0x08001256

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_TIMING_REFERENCES_V2'.
*/
#include "gk_begin_pack.h"
/** @h2xmlp_parameter   {"VCPM_PARAM_ID_TIMING_REFERENCES_V2", VCPM_PARAM_ID_TIMING_REFERENCES_V2}
    @h2xmlp_description {Parameter id to be used to set timing
                         references v2 on VCPM. }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

struct vcpm_param_timing_references_v2_t
{
  uint32_t vfr_mode;
  /**< @h2xmle_description {Vocoder frame synchronization mode.} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0, 1 }
        @h2xmle_policy      {Basic} */             

  uint32_t vsid;
  /**< @h2xmle_description {Voice System ID as defined @80-NF774-2,80-NA610-2.}
          @h2xmle_default     {0} 
          @h2xmle_range       { 0x11C05000, 0x11DC5000 }
          @h2xmle_policy      {Basic} */             

  uint64_t tx_ref_timstamp_us;
  /**< @h2xmle_description { Client's encoder packet exchange time reference (in microseconds).
                             The time reference is used to align the encoder's timeline to the
                             client timeline at the beginning of the vocoder's packet exchange
                             operation. The clock source to be used for interpreting the timestamp is Qtimer.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */       
 
  uint64_t rx_ref_timstamp_us;
  /**< @h2xmle_description { Client's decoder packet exchange time reference (in microseconds).
                             The time reference is used to align the decoder's timeline to the
                             client timeline at the beginning of the vocoder's packet exchange
                             operation. The clock source to be used for interpreting the timestamp is Qtimer.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic} */      

  uint32_t is_frame_type_event_enabled;
  /**< @h2xmle_description { Enables the event driven mailbox packet exchange mode in Tx path for NO_DATA packets.}
           @h2xmle_default     {0} 
           @h2xmle_range       { 0..1 }
           @h2xmle_policy      {Basic}
    */
    
  uint64_t frame_type_ref_timestamp_us;
   /**< @h2xmle_description { Client's time reference to receive the frame type of uplink packet ( absolute time in microseconds).
                              Indication of NO_DATA frame for the next CDRx cycle should 
                              be sent prior to this reference.
                              The clock source to be used for interpreting the timestamp is
                              product-specific. Currently, the QTimer is used as the timing reference
                              for all products.                           
                              frame_type_ref_timestamp_us, read_ref_timestamp_us and enc_packet_ready_margin_us will be 
                              used together to determine the uplink packet delivery timeline for speech packets.
                              Timestamp of speech packet delivery = minimum ( (frame_type_ref_timestamp_us + enc_proc_time*(vfr_cycle_duration_us/20000_us)),
                                                                             (read_ref_timestamp_us - enc_packet_ready_margin_us))
                              This value equal to 0 implies mailbox enhancements feature is disabled. Events for NO_DATA frame type will 
                              not be sent to client.}
           @h2xmle_default     {0} 
           @h2xmle_range       { 0..0xFFFFFFFF }
           @h2xmle_policy      {Basic}
    */
    
    uint32_t enc_packet_ready_margin_us;
    /**< 
        @h2xmle_description { Minimum allowed delay margin between the timestamp of encoder packet
                              being ready and read_ref_timestamp_us.
                              
                              Encoder packet would be ready and available in the mailbox buffer atleast enc_packet_ready_margin_us
                              ahead of read_ref_timestamp_us.
                            
                              Value is specified in microseconds. 
                              Default value is 0us.}
       @h2xmle_default     {0} 
       @h2xmle_range       { 0..0xFFFFFFFF }
       @h2xmle_policy      {Basic}

       */

  uint32_t vfr_cycle_us;
  /**< @h2xmle_description {VFR cycle duration for the voice call.}
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */       

}
#include "gk_end_pack.h"
;

typedef struct vcpm_param_timing_references_v2_t vcpm_param_timing_references_v2_t;


/**
   Param ID to set the VSID to VCPM.
*/
#define VCPM_PARAM_ID_VSID              0x080011BC

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_VSID'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_vsid_payload_t
{
   uint32_t vsid;
   /**< vsid to be set on the graph created with dont care value earlier. */
}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_vsid_payload_t vcpm_param_vsid_payload_t;

#define VCPM_PARAM_ID_MAILBOX_MEMORY_CONFIG    0x080011E7  
 
/* Type definition for vcpm_param_id_mailbox_memory_config_t. */
typedef struct vcpm_param_id_mailbox_memory_config_t vcpm_param_id_mailbox_memory_config_t;
 
#include "gk_begin_pack.h"
 
/* Structure for specifying mailbox memory config.
*/
struct vcpm_param_id_mailbox_memory_config_t
{
  uint32_t mailbox_mem_address_adsp_lsw;
  /**< Lower 32 bits of IO virtual address(understandable to ADSP) of Mailbox Memory carved out from APQ DDR 
       specifically for mailbox packet exchange between ADSP and MDM. */
 
  uint32_t mailbox_mem_address_adsp_msw;
  /**< Upper 32 bits of IO virtual address(understandable to ADSP) of Mailbox Memory carved out from APQ DDR 
       specifically for mailbox packet exchange between ADSP and MDM. */
 
  uint32_t mailbox_mem_address_pcie_lsw;
  /**< Lower 32 bits of IO virtual address(understandable to PCIe) of Mailbox Memory carved out from APQ DDR 
       specifically for mailbox packet exchange between ADSP and MDM. */
 
  uint32_t mailbox_mem_address_pcie_msw;
  /**< Upper 32 bits of IO virtual address(understandable to PCIe) of Mailbox Memory carved out from APQ DDR 
       specifically for mailbox packet exchange between ADSP and MDM. */
 
  uint32_t mem_size;
  /**< Size(in bytes) of the Mailbox Memory carved out from APQ DDR.
   */
}
#include "gk_end_pack.h"
;


/**
  * This Param ID is exposed by VCPM to HLOS to set the Tx Channel info processed by the Tx Sub Graph(hosting ECNS module) 
  * for a given Voice call usecase. 
  * It needs be set after the valid VSID configuration is SET and before issuing Prepare command to the Voice Subgraphs. 
  * It needs to be set during Voice call setup and Device switch scenarios. 
  */
#define VCPM_PARAM_ID_TX_DEV_PP_CHANNEL_INFO              0x08001310

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_TX_DEV_PP_CHANNEL_INFO'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_id_tx_dev_pp_channel_info_t
{
   uint32_t vsid;
   /**< Vsid for which the param payload is provided.> */

   uint32_t num_channels;
   /**< Number of channels processed by Tx Vocproc SG .>*/

}

#include "gk_end_pack.h"
;

typedef struct vcpm_param_id_tx_dev_pp_channel_info_t vcpm_param_id_tx_dev_pp_channel_info_t;

/**
  * This Param ID is exposed by VCPM to VS to set the Timeline Resync cmd for a given Voice call usecase. 
  * It needs be set after the valid VSID configuration is SET and before issuing Prepare command to the Voice Subgraphs. 
  * It needs to be set while Voice call is in Steady state. 
  */
#define VCPM_PARAM_ID_TIMELINE_RESYNC              0x0800134C

/* None resync type indicates this is not a timeline resync. 
 */
#define VCPM_RESYNC_NONE (0x00000000)

/* Tx resync type indicates this a timeline resync in Tx direction. This is expected only during Steady state. 
 */
#define VCPM_RESYNC_TX  (0x00000001)

/* Rx resync type indicates this a timeline resync in Rx direction. This is expected only during Steady state. 
 */
#define VCPM_RESYNC_RX  (0x00000002)

/* Tx_Rx resync type indicates timeline resync is occurring in both Tx & Rx direction together. This is expected only during Steady state. 
 */
#define VCPM_RESYNC_TX_RX  ( VCPM_RESYNC_TX | VCPM_RESYNC_RX )

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_TIMELINE_RESYNC'.
*/
#include "gk_begin_pack.h"

struct vcpm_param_id_timeline_resync_t
{

   uint32_t resync_type;
  /**<  Type of resync that is being performed.
    *  Supported Values: 
    *   VCPM_RESYNC_TX
    *   VCPM_RESYNC_RX
    *   VCPM_RESYNC_TX_RX
    */

   uint32_t network_id;
  /**<  Network ID. */
    
   uint64_t tx_ref_timestamp_us;
    /**< Client's encoder packet exchange time reference (absolute time in microseconds).
      *
      *  The time reference is used to align the encoder's timeline to the
      *  client's timeline.
      *
      *  The clock source to be used for interpreting the timestamp is
      *  product-specific. Currently, the QTimer is used as the timing reference
      *  for all products.
      */
      
  uint64_t rx_ref_timestamp_us;
    /**< Client's decoder packet exchange time reference (absolute time in microseconds).
      *
      *  The time reference is used to align the encoder's timeline to the
      *  client's timeline.
      *
      *  The clock source to be used for interpreting the timestamp is
      *  product-specific. Currently, the QTimer is used as the timing reference
      *  for all products.
      */
     
  uint64_t frame_type_ref_timestamp_us;
    /**< Client's time reference to receive the frame type of uplink packet ( absolute time in microseconds).
      *
      *  Indication of NO_DATA frame for the next CDRx cycle should 
      *  be sent prior to this reference.
      *
      *  The clock source to be used for interpreting the timestamp is
      *  product-specific. Currently, the QTimer is used as the timing reference
      *  for all products.
      *
      *  frame_type_ref_timestamp_us, read_ref_timestamp_us and enc_packet_ready_margin_us will be 
      *  used together to determine the uplink packet delivery timeline for speech packets.
      *
      *  Timestamp of speech packet delivery = minimum ( (frame_type_ref_timestamp_us + enc_proc_time*(vfr_cycle_duration_us/20000_us)),
      *                                                   (read_ref_timestamp_us - enc_packet_ready_margin_us))
      *
      *  Value 0 implies mailbox enhancements feature is disabled. Events for NO_DATA frame type will 
      *  not be sent to client.
      */
      
   uint32_t is_frame_type_event_enabled;
    /**< Enables the event driven mailbox packet exchange mode in Tx path for NO_DATA packets.
      */
      
   uint32_t vsid;
  /**< Voice System ID as defined @80-NF774-2,80-NA610-2.}*/
  
  uint32_t vfr_mode;
    /**< Vocoder frame synchronization mode.
      *
      *  @values
      *   - 0: SOFT_VFR - VFR sourced by vfr_cycle_duration_ms software interupt.
      *   - 1: HARD_VFR - VFR sourced by hardware interupt.
      *     This is not expected to change during runtime reconfig. 
      */
      
  uint32_t vfr_cycle_us;
    /**< Vocoder frame reference duration in microseconds. 
      *  This is not expected to change during runtime reconfig. 
      *
      *  @values N*20, where N is an integer.
      */

  uint32_t enc_packet_ready_margin_us;
    /**< 
      *  Minimum allowed delay margin between the timestamp of encoder packet
      *  being ready and read_ref_timestamp_us.
      *  
      *  Encoder packet would be ready and available in the mailbox buffer atleast enc_packet_ready_margin_us
      *  ahead of read_ref_timestamp_us.
      *  Value is specified in microseconds. 
      *  Default value is 0us.
      */
}
#include "gk_end_pack.h"
;

typedef struct vcpm_param_id_timeline_resync_t vcpm_param_id_timeline_resync_t;

/* Parameter id for setting up the vocoder packet loopback delay in ms.
 */
#define VCPM_PARAM_ID_VOC_PKT_LOOPBACK_DELAY 0x08001311

/**
   Payload structure for the Parameter 'VCPM_PARAM_ID_VOC_PKT_LOOPBACK_DELAY'.
*/
#include "gk_begin_pack.h"

/** @h2xmlp_parameter   {"VCPM_PARAM_ID_VOC_PKT_LOOPBACK_DELAY", VCPM_PARAM_ID_VOC_PKT_LOOPBACK_DELAY}
    @h2xmlp_description {Parameter id for setting up vocoder packet loopback delay in ms. }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

struct vcpm_param_id_voc_pkt_loopback_delay_t
{
   uint32_t vsid;
   /**< @h2xmle_description    {VSID for vocoder packet loopback}
           @h2xmle_default     {0}
           @h2xmle_range       { 0x12006000, 0x121C6000 }
           @h2xmle_policy      {Basic} */

   uint32_t delay_ms;
   /**< @h2xmle_description { Vocoder packet Loopback delay in milliseconds. 
                              Recommended to increment in steps of 20 ms
                              it will be rounded off to next 20 ms multiple.}
        @h2xmle_default     {0}
        @h2xmle_range       { 0..5000 }
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
typedef struct vcpm_param_id_voc_pkt_loopback_delay_t vcpm_param_id_voc_pkt_loopback_delay_t;

/*------------------------------------------------------------------------------
 *  H2XML annotations
 *----------------------------------------------------------------------------*/
enum platforms_gecko 
{
   PLATFORM_GECKO = 0xFFFFFFFE     /**< @h2xmle_name {GECKO} */
};

/**
   @h2xml_platforms{PLATFORM_GECKO}
*/
 
enum vcpm_key_ids
{
    voice_vocoder_class = VCPM_CAL_KEY_ID_VOCODER_CLASS,
    voice_sample_rate = VCPM_CAL_KEY_ID_SAMPLING_RATE,
    voice_vol_level = VCPM_CAL_KEY_ID_VOLUME_LEVEL,
    voice_bandwidth_extension = VCPM_CAL_KEY_ID_BWE,
    voice_network = VCPM_CAL_KEY_ID_NETWORK,
    voice_vol_boost = VCPM_CAL_KEY_ID_VOL_BOOST
};

/**
   @h2xmlk_key {voice_vocoder_class}
   @h2xmlk_isVoice {true} 
   @h2xmlk_description { Calibration key for vocoder class }
*/
enum vcpm_key_vocoder_class 
{
   voc_cal_key_amr = VCPM_CAL_KEY_VOCODER_CLASS_AMR, /**< @h2xmle_description { Vocoder class AMR } */
   voc_cal_key_evrc = VCPM_CAL_KEY_VOCODER_CLASS_EVRC, /**< @h2xmle_description { Vocoder class EVRC } */
   voc_cal_key_evs = VCPM_CAL_KEY_VOCODER_CLASS_EVS /**< @h2xmle_description { Vocoder class EVS } */
};

/**
   @h2xmlk_key {voice_sample_rate }
   @h2xmlk_isVoice {true} 
   @h2xmlk_description { Calibration key for sampling rate }
*/
enum vcpm_key_sample_rate 
{
   SR_NB = 0x1F40, /**< @h2xmle_description {Narrow Band Sampling rate}*/
   SR_WB = 0x3E80, /**< @h2xmle_description {Wide Band Sampling rate}*/
   SR_SWB = 0x7D00, /**< @h2xmle_description {Super Wide Band Sampling rate}*/
   SR_FB = 0xBB80, /**< @h2xmle_description {Full Band Sampling rate}*/
};

/**
   @h2xmlk_key {voice_vol_level} 
   @h2xmlk_isVoice {true} 
   @h2xmlk_isDynamic {true} 
   @h2xmlk_description { Calibration key for volume level}
*/
enum vcpm_key_volume_level 
{
   vol_level_1 = 0x00000001, /**< @h2xmle_description {Volume Level 1}*/
   vol_level_2 = 0x00000002, /**< @h2xmle_description {Volume Level 2}*/
   vol_level_3 = 0x00000003, /**< @h2xmle_description {Volume Level 3}*/
   vol_level_4 = 0x00000004, /**< @h2xmle_description {Volume Level 4}*/
   vol_level_5 = 0x00000005, /**< @h2xmle_description {Volume Level 5}*/
   vol_level_6 = 0x00000006, /**< @h2xmle_description {Volume Level 6}*/
};


/**
   @h2xmlk_key {voice_bandwidth_extension}
   @h2xmlk_isVoice {true}
   @h2xmlk_description {Calibration key for BWE}
*/
enum vcpm_key_bandwidth_extension
{
   enable = 0x00000001, /**< @h2xmle_description {BWE enable}*/
   disable = 0          /**< @h2xmle_description {BWE disable}*/
};


/**
   @h2xmlk_key {voice_network} 
   @h2xmlk_isVoice {true} 
   @h2xmlk_description { Calibration key for vocoder network }
*/
enum vcpm_key_vocoder_network 
{
   voc_cal_key_cdma = VCPM_CAL_KEY_NETWORK_CDMA, /**< @h2xmle_description { Vocoder network CDMA } */
   voc_cal_key_gsm = VCPM_CAL_KEY_NETWORK_GSM, /**< @h2xmle_description { Vocoder network GSM } */
   voc_cal_key_wcdma = VCPM_CAL_KEY_NETWORK_WCDMA, /**< @h2xmle_description { Vocoder network WCDMA } */
   voc_cal_key_lte = VCPM_CAL_KEY_NETWORK_LTE, /**< @h2xmle_description { Vocoder network LTE } */
   voc_cal_key_TDSCDMA = VCPM_CAL_KEY_NETWORK_TDSCDMA /**< @h2xmle_description { Vocoder network TDSCDMA } */
};

/**
   @h2xmlk_key {voice_vol_boost} 
   @h2xmlk_isVoice {true} 
   @h2xmlk_description {Calibration key for vol_boost}
*/
enum vcpm_key_vol_boost 
{
   vol_boost_enable = 0x00000001, /**< @h2xmle_description {vol_boost enable}*/
   vol_boost_disable = 0          /**< @h2xmle_description {vol_boost disable}*/
};

/**
   @h2xmlk_ckeys
   @h2xmlk_isVoice {true} 
   @h2xmlk_description {Calibration Keys for voice call usecase}
*/
enum vcpm_cal_Keys 
{
   ck_vocoder_class = voice_vocoder_class,
   ck_sampling_rate = voice_sample_rate,  
   ck_volume_level = voice_vol_level,
   ck_WV2 = voice_bandwidth_extension,
   ck_voice_network = voice_network,
   ck_vol_boost = voice_vol_boost
};

/**
   @h2xmlk_modTagList 
   @h2xmlk_description {Module Tag Info} 
*/
typedef enum  
{
   voice_mod_tag_id_encoder = VOICE_MOD_TAG_ID_ENCODER,                
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Voice Encoder} */

   voice_mod_tag_id_tx_mailbox = VOICE_MOD_TAG_ID_TX_MAILBOX,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Tx Mailbox} */

   voice_mod_tag_id_tx_stream_hpcm = VOICE_MOD_TAG_ID_TX_STREAM_HPCM,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Tx Stream Host PCM} */

   voice_mod_tag_id_tx_smart_sync = VOICE_MOD_TAG_ID_TX_SMART_SYNC,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Tx Smart Sync} */

   voice_mod_tag_id_decoder = VOICE_MOD_TAG_ID_DECODER,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Voice Decoder} */

   voice_mod_tag_id_rx_mailbox = VOICE_MOD_TAG_ID_RX_MAILBOX,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Rx Mailbox} */

   voice_mod_tag_id_rx_stream_hpcm = VOICE_MOD_TAG_ID_RX_STREAM_HPCM,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {Rx Stream Host PCM} */

   voice_mod_tag_id_1x_rx_tty = VOICE_MOD_TAG_ID_1XTTY_RX,
   /**< @h2xmle_isVoice {true}
        @h2xmle_description {CTMTTY Rx} */

   voice_mod_tag_id_ctm_rx_tty = VOICE_MOD_TAG_ID_CTMTTY_RX,
   /**< @h2xmle_isVoice {true} 
        @h2xmle_description {CTMTTY Rx} */

   voice_mod_tag_id_ctm_tx_tty = VOICE_MOD_TAG_ID_CTMTTY_TX,
   /**< @h2xmle_isVoice {true}
        @h2xmle_description {CTMTTY Tx} */

   voice_mod_tag_id_lte_rx_tty = VOICE_MOD_TAG_ID_LTETTY_RX,
   /**< @h2xmle_isVoice {true}
        @h2xmle_description {LTETTY Rx} */

   voice_mod_tag_id_lte_tx_tty = VOICE_MOD_TAG_ID_LTETTY_TX,
   /**< @h2xmle_isVoice {true}
        @h2xmle_description {LTETTY Tx} */

}vcpm_graph_tag_Keys;


#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* _VCPM_API_H_ */
