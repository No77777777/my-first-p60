#ifndef COEX_SERVICE_02_H
#define COEX_SERVICE_02_H
/**
  @file coexistence_service_v02.h

  @brief This is the public header file which defines the coex service Data structures.

  This header file defines the types and structures that were defined in
  coex. It contains the constant values defined, enums, structures,
  messages, and service message IDs (in that order) Structures that were
  defined in the IDL as messages contain mandatory elements, optional
  elements, a combination of mandatory and optional elements (mandatory
  always come before optionals in the structure), or nothing (null message)

  An optional element in a message is preceded by a uint8_t value that must be
  set to true if the element is going to be included. When decoding a received
  message, the uint8_t values will be set to true or false by the decode
  routine, and should be checked before accessing the values that they
  correspond to.

  Variable sized arrays are defined as static sized arrays with an unsigned
  integer (32 bit) preceding it that must be set to the number of elements
  in the array that are valid. For Example:

  uint32_t test_opaque_len;
  uint8_t test_opaque[16];

  If only 4 elements are added to test_opaque[] then test_opaque_len must be
  set to 4 before sending the message.  When decoding, the _len value is set
  by the decode routine and should be checked so that the correct number of
  elements in the array will be accessed.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013-2019 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.


  $Header: //components/rel/qmimsgs.mpss/4.6.9/coex/api/coexistence_service_v02.h#2 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.7 
   It was generated on: Thu Jul 25 2019 (Spin 0)
   From IDL File: coexistence_service_v02.idl */

/** @defgroup coex_qmi_consts Constant values defined in the IDL */
/** @defgroup coex_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup coex_qmi_enums Enumerated types used in QMI messages */
/** @defgroup coex_qmi_messages Structures sent as QMI messages */
/** @defgroup coex_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup coex_qmi_accessor Accessor for QMI service object */
/** @defgroup coex_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"
#include "common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup coex_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define COEX_V02_IDL_MAJOR_VERS 0x02
/** Revision Number of the IDL used to generate this file */
#define COEX_V02_IDL_MINOR_VERS 0x10
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define COEX_V02_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define COEX_V02_MAX_MESSAGE_ID 0x0056
/**
    @}
  */


/** @addtogroup coex_qmi_consts
    @{
  */

/**  Maximum number of supported unique radio access technologies (RATs).  */
#define COEX_MAX_TECHS_SUPPORTED_V02 24

/**  Maximum number of unique operating bands and channels supported for WWAN GSM RAT.  */
#define COEX_WWAN_GSM_MAX_BANDS_SUPPORTED_V02 16

/**  Maximum number of unique operating bands and channels supported for all other WWAN RATs.  */
#define COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02 8

/**  Maximum number of unique events supported for WLAN Wi-Fi RAT.  */
#define COEX_WLAN_WIFI_MAX_BANDS_SUPPORTED_V02 4

/**  Maximum number of unique WLAN Wi-Fi high-priority events supported.  */
#define COEX_WLAN_WIFI_MAX_EVENTS_SUPPORTED_V02 8

/**  Maximum number of unique WLAN Wi-Fi active connections supported.  */
#define COEX_WLAN_WIFI_MAX_CONNS_SUPPORTED_V02 4

/**  Maximum number of conflict entries in the victim table shared with each
     response.  */
#define COEX_NV_MSG_VICTIM_TABLE_MAX_ENTRIES_V02 2

/**  Maximum number of simultaneous active WLAN/WWAN conflicts from the victim table.  */
#define COEX_MAX_ACTIVE_CONFLICTS_V02 8

/**  Maximum number of active LTE component carriers that are supported for WWAN LTE RAT.  */
#define COEX_WWAN_LTE_STATE_MAX_CARRIERS_V02 12

/**  Maximum number of simultaneous potential conflicts, for WCN to try to avoid.  */
#define COEX_MAX_CHANNEL_AVOID_CONFLICTS_V02 12

/**  Maximum number of simultaneous active RAT ID mappings.  */
#define COEX_MAX_RAT_ID_MAPPINGS_V02 16

/**  Maximum number of bytes in BT Channel mapping info.  */
#define COEX_MAX_BT_CHANNEL_MAP_SIZE_V02 10

/**  Maximum number of concurrent operation modes for 2.4GHz WLAN  */
#define COEX_MAX_WLAN_CONCURRENT_MODES_V02 8

/**  Maximum number of WLAN transmitters supported for Smart transmit  */
#define COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02 2

/**  Default WLAN power limit (in dbm10) for Smart transmit  */
#define COEX_RTSAR_WLAN_DEFAULT_PWR_LMT_V02 -32768
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_TECH_TYPE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_TECH_TYPE_UNINIT_V02 = 0, /**<  RAT type is uninitialized  */
  COEX_TECH_TYPE_WWAN_LTE_V02 = 1, /**<  RAT type is WWAN LTE  */
  COEX_TECH_TYPE_WWAN_TDSCDMA_V02 = 2, /**<  RAT type is WWAN TD-SCDMA  */
  COEX_TECH_TYPE_WWAN_GSM_V02 = 3, /**<  RAT type is WWAN GSM [Instance 1]  */
  COEX_TECH_TYPE_WWAN_ONEX_V02 = 4, /**<  RAT type is WWAN CDMA2000\textsuperscript{\textregistered} 1X  */
  COEX_TECH_TYPE_WWAN_HDR_V02 = 5, /**<  RAT type is WWAN HDR  */
  COEX_TECH_TYPE_WWAN_WCDMA_V02 = 6, /**<  RAT type is WWAN WCDMA [Instance 1]  */
  COEX_TECH_TYPE_WLAN_WIFI_V02 = 7, /**<  RAT type is WLAN Wi-Fi  */
  COEX_TECH_TYPE_WLAN_BT_V02 = 8, /**<  RAT type is WLAN BlueTooth  */
  COEX_TECH_TYPE_SPECIAL_DIVERSITY_ANTENNA_V02 = 9, /**<  Special type to represent WWAN (MDM) diversity antenna chain  */
  COEX_TECH_TYPE_WWAN_GSM2_V02 = 10, /**<  RAT type is WWAN GSM [Instance 2]  */
  COEX_TECH_TYPE_WWAN_GSM3_V02 = 11, /**<  RAT type is WWAN GSM [Instance 3]  */
  COEX_TECH_TYPE_WWAN_WCDMA2_V02 = 12, /**<  RAT type is WWAN WCDMA [Instance 2]  */
  COEX_TECH_TYPE_WWAN_LTE2_V02 = 13, /**<  RAT type is WWAN LTE [Instance 2]  */
  COEX_TECH_TYPE_WWAN_NR5G_V02 = 14, /**<  RAT type is WWAN NR5G  */
  COEX_TECH_TYPE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_tech_type_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_CARRIER_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_CARRIER_PRIMARY_V02 = 0, /**<  Primary component carrier ID  */
  COEX_CARRIER_SECONDARY_0_V02 = 1, /**<  Component carrier ID for first secondary carrier  */
  COEX_CARRIER_SECONDARY_1_V02 = 2, /**<  Component carrier ID for second secondary carrier  */
  COEX_CARRIER_SECONDARY_2_V02 = 3, /**<  Component carrier ID for third secondary carrier  */
  COEX_CARRIER_SECONDARY_3_V02 = 4, /**<  Component carrier ID for fourth secondary carrier  */
  COEX_CARRIER_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_carrier_v02;
/**
    @}
  */

typedef uint8_t coex_tech_state_v02;
#define COEX_TECH_STATE_OFF_V02 ((coex_tech_state_v02)0x00) /**<  Whether the technology's state is off  */
#define COEX_TECH_STATE_IDLE_V02 ((coex_tech_state_v02)0x01) /**<  Whether the technology's state is idle  */
#define COEX_TECH_STATE_CONN_V02 ((coex_tech_state_v02)0x02) /**<  Whether the technology's state is connected  */
#define COEX_TECH_STATE_ACTIVE_V02 ((coex_tech_state_v02)0x04) /**<  Whether the technology's state is active  */
/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_TECH_OPERATING_DIMENSION_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_TECH_OPERATING_DIMENSION_FDD_V02 = 0x0, /**<  Technology is operating in the Frequency Division Duplex dimension  */
  COEX_TECH_OPERATING_DIMENSION_TDD_V02 = 0x1, /**<  Technology is operating in the Time Division Duplex dimension  */
  COEX_TECH_OPERATING_DIMENSION_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_tech_operating_dimension_v02;
/**
    @}
  */

typedef uint8_t coex_tech_band_direction_v02;
#define COEX_TECH_BAND_DIRECTION_DONT_CARE_V02 ((coex_tech_band_direction_v02)0x00) /**<  Band direction is don't care.  */
#define COEX_TECH_BAND_DIRECTION_DOWNLINK_V02 ((coex_tech_band_direction_v02)0x01) /**<  Band information is for a downlink.  */
#define COEX_TECH_BAND_DIRECTION_UPLINK_V02 ((coex_tech_band_direction_v02)0x02) /**<  Band information is for an uplink.  */
/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint16_t frequency;
  /**<   Frequency; radio access technology's operating center frequency in MHz
       (times 10). This value is represented in x10 to provide the fractional
       precision of one decimal point. Valid range: 0.0 MHz to 6553.5 MHz.
       Values: 0 to 65535 (2\textsuperscript{16}-1).
  */

  uint16_t bandwidth_and_direction;
  /**<   Bandwidth and direction; radio access technology's operating bandwidth
       in MHz (times 10) bit-packed with the link direction.\n
       - bit[15:14] -- coex_tech_band_direction: Uplink, Downlink, Both, Don't Care.
                      Note that a band can be specified as both uplink (UL) and
                      downlink (DL) by combining the mask values of UL and DL.\n
       - bit[13:0]  -- Indicates the bandwidth in x10 to provide fractional precision
                      of one decimal point. Valid range: 0.0 MHz to 1638.3 MHz.
                      Values: 0 to 16383 (2\textsuperscript{14}-1).
  */
}coex_tech_band_type_v02;  /* Type */
/**
    @}
  */

typedef uint8_t coex_wwan_conflict_frequency_range_v02;
#define COEX_WWAN_CONFLICT_FREQUENCY_RANGE_DONT_CARE_V02 ((coex_wwan_conflict_frequency_range_v02)0x00) /**<  Frequency range is "do not care," or is less than 2300 MHz.  */
#define COEX_WWAN_CONFLICT_FREQUENCY_RANGE_2300_TO_2400_V02 ((coex_wwan_conflict_frequency_range_v02)0x01) /**<  Frequency range is 2300 to 2400 MHz.  */
#define COEX_WWAN_CONFLICT_FREQUENCY_RANGE_GTE_2400_V02 ((coex_wwan_conflict_frequency_range_v02)0x02) /**<  Frequency range is greater than or equal to 2400 MHz.  */
/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_ANTENNA_CHAIN_STATE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_ANTENNA_CHAIN_STATE_INACTIVE_V02 = 0, /**<  Antenna chain is inactive (not in use by the modem)  */
  COEX_ANTENNA_CHAIN_STATE_ACTIVE_V02 = 1, /**<  Antenna chain is active (in use by the modem).  */
  COEX_ANTENNA_CHAIN_STATE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_antenna_chain_state_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint16_t adv_notice;
  /**<    Advance notice; delta time (in the future), in milliseconds, when
        the specified coex_antenna_chain_state takes effect. */

  uint16_t duration;
  /**<    Duration; delta time (in the future), in milliseconds, beyond the
        advance notice that the coex_antenna_chain_state remains in effect. */
}coex_antenna_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t id;
  /**<   ID; bit-packed to represent the following fields:\n
       - bit[7:6] -- Reserved for future use \n
       - bit[5:0] -- coex_tech_type; COEX technology type
  */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state\n
       - bit[4:0] -- 5-bit revolving token that indicates if the state changed;
                     valid range: 0 to 31
  */
}coex_tech_sync_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Requests synchronization of COEX states between modules. */
typedef struct {

  /* Optional */
  /*  WWAN/WLAN Technologies Sync State */
  uint8_t tech_valid;  /**< Must be set to true if tech is being passed */
  uint32_t tech_len;  /**< Must be set to # of elements in tech */
  coex_tech_sync_state_v02 tech[COEX_MAX_TECHS_SUPPORTED_V02];
  /**<   \n
     (Shares the current COEX synchronization state for all valid/current
     technologies.)
   */

  /* Optional */
  /*  Sync state token for the VOLTE/CDRX state */
  uint8_t volte_cdrx_sync_token_valid;  /**< Must be set to true if volte_cdrx_sync_token is being passed */
  uint8_t volte_cdrx_sync_token;
  /**<   \n
     (Shares the current COEX synchronization state of Voice over LTE or
     connected discontinuous receiption between MDM and WCN)
 
       - bit[7:5] -- reserved for future use \n
       - bit[4:0] -- 5-bit revolving token that indicates if the state
                     changed; valid range: 0 to 31
  */
}qmi_coex_tech_sync_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Response Message; Responds to synchronize the COEX states between modules. */
typedef struct {

  /* Optional */
  /*  WWAN/WLAN Technologies Sync State */
  uint8_t tech_valid;  /**< Must be set to true if tech is being passed */
  uint32_t tech_len;  /**< Must be set to # of elements in tech */
  coex_tech_sync_state_v02 tech[COEX_MAX_TECHS_SUPPORTED_V02];
  /**<   \n
       (Used to share the current COEX synchronization state for all valid/current
       technologies.)*/
}qmi_coex_tech_sync_resp_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Updates a specific COEX state between modules. */
typedef struct {

  /* Optional */
  /*  WWAN/WLAN Technologies Sync State */
  uint8_t tech_valid;  /**< Must be set to true if tech is being passed */
  coex_tech_sync_state_v02 tech;
  /**<   \n
       (Used to share the current COEX synchronization state for the current technology.)*/
}qmi_coex_tech_sync_update_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t id;
  /**<   ID; bit-packed to represent the following fields:\n
       - bit[7:6] -- Reserved for future use\n
       - bit[5:0] -- coex_tech_type; the COEX technology type */
}coex_tech_state_update_req_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Requests an update of the COEX state of various technologies. */
typedef struct {

  /* Optional */
  /*  WWAN/WLAN Technologies Sync State */
  uint8_t tech_ids_valid;  /**< Must be set to true if tech_ids is being passed */
  uint32_t tech_ids_len;  /**< Must be set to # of elements in tech_ids */
  coex_tech_state_update_req_v02 tech_ids[COEX_MAX_TECHS_SUPPORTED_V02];
  /**<   \n
       (Requests the current COEX state for selected technologies.)
  */
}qmi_coex_tech_state_update_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_LTE_TDD_CONFIG_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_LTE_TDD_CONFIG_UNDEF_V02 = 0, /**<  Uninitialized LTE TDD configuration  */
  COEX_LTE_TDD_CONFIG_0_V02 = 1, /**<  LTE TDD configuration 0  */
  COEX_LTE_TDD_CONFIG_1_V02 = 2, /**<  LTE TDD configuration 1  */
  COEX_LTE_TDD_CONFIG_2_V02 = 3, /**<  LTE TDD configuration 2  */
  COEX_LTE_TDD_CONFIG_3_V02 = 4, /**<  LTE TDD configuration 3  */
  COEX_LTE_TDD_CONFIG_4_V02 = 5, /**<  LTE TDD configuration 4  */
  COEX_LTE_TDD_CONFIG_5_V02 = 6, /**<  LTE TDD configuration 5  */
  COEX_LTE_TDD_CONFIG_6_V02 = 7, /**<  LTE TDD configuration 6  */
  COEX_LTE_TDD_CONFIG_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_lte_tdd_config_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_LTE_TDD_SUBFRAME_CONFIG_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_LTE_TDD_SUBFRAME_CONFIG_UNDEF_V02 = 0, /**<  Uninitialized LTE TDD configuration  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_0_V02 = 1, /**<  LTE TDD special subframe configuration 0  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_1_V02 = 2, /**<  LTE TDD special subframe configuration 1  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_2_V02 = 3, /**<  LTE TDD special subframe configuration 2  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_3_V02 = 4, /**<  LTE TDD special subframe configuration 3  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_4_V02 = 5, /**<  LTE TDD special subframe configuration 4  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_5_V02 = 6, /**<  LTE TDD special subframe configuration 5  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_6_V02 = 7, /**<  LTE TDD special subframe configuration 6  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_7_V02 = 8, /**<  LTE TDD special subframe configuration 7  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_8_V02 = 9, /**<  LTE TDD special subframe configuration 8  */
  COEX_LTE_TDD_SUBFRAME_CONFIG_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_lte_tdd_subframe_config_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_LTE_TDD_LINK_CONFIG_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_LTE_TDD_LINK_UNDEF_V02 = 0, /**<  Uninitialized LTE TDD link  */
  COEX_LTE_TDD_LINK_NORMAL_V02 = 1, /**<  Normal cyclic prefix  */
  COEX_LTE_TDD_LINK_EXTENDED_V02 = 2, /**<   Extended cyclic prefix  */
  COEX_LTE_TDD_LINK_CONFIG_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_lte_tdd_link_config_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band info; WWAN LTE's current operating frequencies */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */

  uint16_t data;
  /**<   Data; bit-packed to represent the following fields:\n
       - bit[15]    -- coex_tech_operating_dimension; LTE's dimension\n
       - bit[14:10] -- Reserved for future use\n
       - bit[9:0]   -- Frame offset (timing advance) in microseconds;
                       valid range: 0 to 1023 microseconds
  */

  uint16_t tdd_info;
  /**<   LTE Time Division Duplex (TDD) information; contains the TDD parameters
       and is bit-packed to represent the following fields:\n
       - bit[15:12] -- Reserved for future use\n
       - bit[11:10] -- Uplink coex_lte_tdd_link_config\n
       - bit[9:8]   -- Downlink coex_lte_tdd_link_config\n
       - bit[7:4]   -- coex_lte_tdd_config\n
       - bit[3:0]   -- coex_lte_tdd_link_config
  */
}coex_wwan_lte_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t bands;
  /**<   Bands; identifies the bands that belong to this component carrier, with an
       index into the band_info array.
       An index of 0xFF indicates an invalid band.\n
       - bit[7:4] -- UL band index\n
       - bit[3:0] -- DL band index
  */

  uint16_t data;
  /**<   Data; bit-packed data that represents the following fields:\n
       - bit[15]    -- coex_tech_operating_dimension; LTE's dimension\n
       - bit[14:10] -- Reserved for future use\n
       - bit[9:0]   -- Frame offset (timing advance) in microseconds;
                       valid range: 0 to 1023 microseconds
  */

  uint16_t tdd_info;
  /**<   LTE Time Division Duplex (TDD) information; contains the TDD parameters
       and is bit-packed to represent the following fields:\n
       - bit[15:12] -- Reserved for future use\n
       - bit[11:10] -- Uplink coex_lte_tdd_link_config\n
       - bit[9:8]   -- Downlink coex_lte_tdd_link_config\n
       - bit[7:4]   -- coex_lte_tdd_config\n
       - bit[3:0]   -- coex_lte_tdd_subframe_config
  */
}coex_wwan_lte_carrier_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band info; WWAN LTE current operating frequencies. */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */

  uint32_t cc_info_len;  /**< Must be set to # of elements in cc_info */
  coex_wwan_lte_carrier_type_v02 cc_info[COEX_WWAN_LTE_STATE_MAX_CARRIERS_V02];
  /**<   LTE carrier info; for each component carrier, these are the bands
       that belong to it and its operating parameters.\n
  */
}coex_wwan_lte_ca_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN LTE technology. */
typedef struct {

  /* Optional */
  /*  WWAN LTE Technology's State Information for COEX */
  uint8_t lte_band_info_valid;  /**< Must be set to true if lte_band_info is being passed */
  coex_wwan_lte_state_v02 lte_band_info;
  /**<   \n
       (Contains the LTE state information needed for COEX.)
      */

  /* Optional */
  /*  WWAN LTE Technology with Carrier Aggregation State Information for COEX */
  uint8_t lte_ca_band_info_valid;  /**< Must be set to true if lte_ca_band_info is being passed */
  coex_wwan_lte_ca_state_v02 lte_ca_band_info;
  /**<   \n
       (Contains the LTE component carrier state information needed for COEX.)
       Replaces the optional WWAN LTE Technology's State Information for COEX
       TLV when on a target that supports LTE carrier aggregation.
      */
}qmi_coex_wwan_lte_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the WWAN TD-SCDMA's current operating frequencies. */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wwan_tdscdma_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN TD-SCDMA technology. */
typedef struct {

  /* Optional */
  /*  WWAN TD-SCDMA Technology's State Information for COEX */
  uint8_t tdscdma_band_info_valid;  /**< Must be set to true if tdscdma_band_info is being passed */
  coex_wwan_tdscdma_state_v02 tdscdma_band_info;
  /**<   \n
       (Contains the TD-SCDMA state information needed for COEX.)
       */
}qmi_coex_wwan_tdscdma_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_GSM_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; the WWAN GSM's current operating frequencies */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                    valid range: 0 to 31
  */
}coex_wwan_gsm_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN GSM [Instance 1]
                     technology. */
typedef struct {

  /* Optional */
  /*  WWAN GSM [Instance 1] Technology's State Information for COEX */
  uint8_t gsm_band_info_valid;  /**< Must be set to true if gsm_band_info is being passed */
  coex_wwan_gsm_state_v02 gsm_band_info;
  /**<   \n
       (Contains the GSM [Instance 1] state information needed for COEX.)
  */
}qmi_coex_wwan_gsm_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN GSM [Instance 2]
                     technology. */
typedef struct {

  /* Optional */
  /*  WWAN GSM [Instance 2] Technology's State Information for COEX */
  uint8_t gsm2_band_info_valid;  /**< Must be set to true if gsm2_band_info is being passed */
  coex_wwan_gsm_state_v02 gsm2_band_info;
  /**<   \n
        (Contains the GSM [Instance 2] state information needed for COEX.)
   */
}qmi_coex_wwan_gsm2_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN GSM [Instance 3]
                     technology. */
typedef struct {

  /* Optional */
  /*  WWAN GSM [Instance 3] Technology's State Information for COEX */
  uint8_t gsm3_band_info_valid;  /**< Must be set to true if gsm3_band_info is being passed */
  coex_wwan_gsm_state_v02 gsm3_band_info;
  /**<   \n
          (Contains the GSM [Instance 3] state information needed for COEX.)
     */
}qmi_coex_wwan_gsm3_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the WWAN CDMA2000 1X's (ONEX) current
       operating frequencies
  */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wwan_onex_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN ONEX technology. */
typedef struct {

  /* Optional */
  /*  WWAN ONEX Technology's State Information for COEX */
  uint8_t onex_band_info_valid;  /**< Must be set to true if onex_band_info is being passed */
  coex_wwan_onex_state_v02 onex_band_info;
  /**<   \n
       (Contains the ONEX state information needed for COEX.)
       */
}qmi_coex_wwan_onex_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the WWAN HDR's current operating frequencies */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wwan_hdr_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN HDR technology. */
typedef struct {

  /* Optional */
  /*  WWAN HDR Technology's State Information for COEX */
  uint8_t hdr_band_info_valid;  /**< Must be set to true if hdr_band_info is being passed */
  coex_wwan_hdr_state_v02 hdr_band_info;
  /**<   \n
      (Contains the HDR state information needed for COEX.)
    */
}qmi_coex_wwan_hdr_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the WWAN WCDMA's current operating frequencies */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wwan_wcdma_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN WCDMA [Instance 1]
                     technology. */
typedef struct {

  /* Optional */
  /*  WWAN WCDMA [Instance 1] Technology's State Information for COEX */
  uint8_t wcdma_band_info_valid;  /**< Must be set to true if wcdma_band_info is being passed */
  coex_wwan_wcdma_state_v02 wcdma_band_info;
  /**<   \n
       (Contains the WCDMA [Instance 1] state information needed for COEX.)
       */
}qmi_coex_wwan_wcdma_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN WCDMA [Instance 2] technology. */
typedef struct {

  /* Optional */
  /*  WWAN WCDMA [Instance 2] Technology's State Information for COEX */
  uint8_t wcdma2_band_info_valid;  /**< Must be set to true if wcdma2_band_info is being passed */
  coex_wwan_wcdma_state_v02 wcdma2_band_info;
  /**<   \n
       (Contains the WCDMA [Instance 2] state information needed for COEX.)
       */
}qmi_coex_wwan_wcdma2_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WWAN_TECH_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the WWAN NR5G's current operating frequencies. */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wwan_nr5g_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WWAN NR5G technology. */
typedef struct {

  /* Optional */
  /*  WWAN NR5G Technology's State Information for COEX */
  uint8_t nr5g_band_info_valid;  /**< Must be set to true if nr5g_band_info is being passed */
  coex_wwan_nr5g_state_v02 nr5g_band_info;
  /**<   \n
       (Contains the NR5G state information needed for COEX.)
       */
}qmi_coex_wwan_nr5g_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t band_info_len;  /**< Must be set to # of elements in band_info */
  coex_tech_band_type_v02 band_info[COEX_WLAN_WIFI_MAX_BANDS_SUPPORTED_V02];
  /**<   Band information; represents the current WLAN Wi-Fi's connection
       operating frequencies */
}coex_wlan_wifi_connection_info_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t high_priority_events_list_len;  /**< Must be set to # of elements in high_priority_events_list */
  coex_tech_band_type_v02 high_priority_events_list[COEX_WLAN_WIFI_MAX_EVENTS_SUPPORTED_V02];
  /**<   High priority events list; list of active WLAN Wi-Fi frequencies with
    on-going high priority events */

  uint32_t connections_list_len;  /**< Must be set to # of elements in connections_list */
  coex_wlan_wifi_connection_info_v02 connections_list[COEX_WLAN_WIFI_MAX_CONNS_SUPPORTED_V02];
  /**<   Connections list; list of the active WLAN Wi-Fi on-going connections */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_wlan_wifi_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for the WLAN Wi-Fi\reg technology. */
typedef struct {

  /* Optional */
  /*  WLAN Wi-Fi Technology's State Information for COEX */
  uint8_t wifi_state_info_valid;  /**< Must be set to true if wifi_state_info is being passed */
  coex_wlan_wifi_state_v02 wifi_state_info;
  /**<   \n
       (Contains the WLAN Wi-Fi state information needed for COEX.)
        */
}qmi_coex_wlan_wifi_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the COEX state for WLAN Bluetooth\reg technology. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}qmi_coex_wlan_bt_state_ind_msg_v02;

  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_antenna_state_v02 data;
  /**<   Data; represents the state of the diversity antenna.
       It comprises the following fields:\n
        - adv_notice -- Time (in the future), in milliseconds, when the
                      coex_antenna_chain_state takes effect\n
        - duration -- Time, in milliseconds, beyond the adv_notice that the
                    coex_antenna_chain_state remains in effect
  */

  uint8_t sync_state_token;
  /**<   Sync state token; bit-packed to represent the following fields:\n
       - bit[7:5] -- coex_tech_state; the technology's current state; note
                     that only bit[7] applies and is represented by
                     coex_antenna_chain_state; bit[6:5] is don't care\n
       - bit[4:0] -- 5-bit revolving token to indicate if the state changed;
                     valid range: 0 to 31
  */
}coex_diversity_antenna_state_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the state of the diversity antenna chain. */
typedef struct {

  /* Optional */
  /*  Antenna Chain State Information for COEX */
  uint8_t state_valid;  /**< Must be set to true if state is being passed */
  coex_diversity_antenna_state_v02 state;
  /**<   \n
        (Contains the current state of the diversity antenna chain needed for
        COEX.) */
}qmi_coex_diversity_antenna_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t filter_alpha;
  /**<   Filter alpha; the "history" parameter for the first-order LTE SINR
       metrics filter. Valid range: 0 to 1, with 1/100th precision; represented
       as x100 (times 100), e.g., 0.1 is 10, 0.89 is 89.
  */
}coex_metrics_lte_sinr_params_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Starts collection and collation of the LTE Signal-to-Interface
              Plus Noise Ratio (SINR) metric. */
typedef struct {

  /* Optional */
  /*  Filter Parameters */
  uint8_t data_valid;  /**< Must be set to true if data is being passed */
  coex_metrics_lte_sinr_params_v02 data;
  /**<   \n
      (Filter parameters for the LTE SINR metric.)
  */

  /* Optional */
  /*  Component Carrier ID Mask */
  uint8_t cc_mask_valid;  /**< Must be set to true if cc_mask is being passed */
  uint8_t cc_mask;
  /**<   Mask of coex_carriers; the WWAN component carriers to which this message
       applies. Each position in the enum corresponds to a bit position
       in the mask. If that bit is set to 1, this message
       applies to that carrier. If not present, the primary component carrier
       is assumed.
  */
}qmi_coex_metrics_lte_sinr_start_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Reads the current filter output for the LTE SINR metric. */
typedef struct {

  /* Optional */
  /*  Component Carrier ID Mask */
  uint8_t cc_mask_valid;  /**< Must be set to true if cc_mask is being passed */
  uint8_t cc_mask;
  /**<   Mask of coex_carriers -- WWAN component carriers to which this message
        applies. Each position in the enum corresponds to a bit position
        in the mask. If that bit is set to 1, this message
        applies to that carrier. If not present, the primary component carrier
        is assumed.
   */
}qmi_coex_metrics_lte_sinr_read_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  int8_t sinr_system;
  /**<   Filter output for the total (overall system);
       LTE SINR metrics, in dBM. Likely range: -10 to +30
       (with 1 dBM resolution)
  */

  int8_t sinr_bt_only;
  /**<   Filter output for the LTE SINR metrics, in dBM,
       when Bluetooth is active.
       Likely range: -10 to +30 (with 1 dBM resolution)
  */

  int8_t sinr_wifi_only;
  /**<   Filter output for the LTE SINR metrics, in dBM,
       when Wi-Fi is active.
       Likely range: -10 to +30 (with 1 dBM resolution)
  */

  int8_t sinr_bt_and_wifi;
  /**<   Filter output for the LTE SINR metrics, in dBM,
       when both Bluetooth & Wi-Fi are active.
       Likely range: -10 to +30 (with 1 dBM resolution)
  */

  int8_t sinr_lte_only;
  /**<   Filter output for the LTE SINR metrics, in dBM,
       when only LTE is active (both Bluetooth and Wi-Fi are inactive).
       Likely range: -10 to +30 (with 1 dBM resolution)
  */
}coex_metrics_lte_sinr_stats_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_METRICS_ESTATUS_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_METRICS_E_SUCCESS_V02 = 0x00, /**<  Metrics were collected and reported successfully  */
  COEX_METRICS_E_NOT_STARTED_V02 = 0x01, /**<  Metric collection was not started; the request was never made  */
  COEX_METRICS_E_TECH_NOT_ACTIVE_V02 = 0x02, /**<  Metrics technology is not present or active  */
  COEX_METRICS_ESTATUS_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_metrics_estatus_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t estatus;
  /**<   Validates the enclosed LTE SINR metrics collected
  */

  uint8_t collection_duration;
  /**<   Collection duration; how long the metric collection
       has been running, in tens of milliseconds. Duration range:
       0x00 - 0xFE (0 to 254*10 = 2540 ms). \n
       Special values:\n
       - 0x00 -- Just started or not started (see estatus) \n
       - 0xFF -- More than 2.54 seconds has passed
  */

  coex_metrics_lte_sinr_stats_v02 stats;
  /**<   Summary report of all the LTE SINR metrics collected so far
  */
}coex_metrics_lte_sinr_report_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Response Message; Provides the requested filter output for the LTE SINR metric. */
typedef struct {

  /* Optional */
  /*  LTE SINR Metrics Report */
  uint8_t report_valid;  /**< Must be set to true if report is being passed */
  coex_metrics_lte_sinr_report_v02 report;
  /**<   \n
      (Current report of LTE SINR statistics, as requested.)
  */

  /* Optional */
  /*  Component Carrier ID */
  uint8_t cc_id_valid;  /**< Must be set to true if cc_id is being passed */
  uint8_t cc_id;
  /**<   coex_carrier; the WWAN component carrier that this message applies to.
       If not present, the primary component carrier is assumed.
  */
}qmi_coex_metrics_lte_sinr_read_resp_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Stops collection and collation of the LTE SINR metric. */
typedef struct {

  /* Optional */
  /*  Component Carrier ID Mask */
  uint8_t cc_mask_valid;  /**< Must be set to true if cc_mask is being passed */
  uint8_t cc_mask;
  /**<   Mask of coex_carriers; the WWAN component carriers to which this message
        applies. Each position in the enum corresponds to a bit position
        in the mask. If that bit is set to 1, this message
        applies to that carrier. If not present, the primary component carrier
        is assumed.
   */
}qmi_coex_metrics_lte_sinr_stop_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Starts collection and collation of the LTE Block error rate
             (BLER) metric
Starts collection and collation of the LTE Block-error-rate
              (BLER) metric. */
typedef struct {

  /* Mandatory */
  /*  Transport Block Count */
  uint32_t tb_cnt;
  /**<   Window/count of LTE transport blocks over which the
       Block Error Rate (BLER) must be calculated.
  */

  /* Mandatory */
  /*  Error Threshold Transport Block Count */
  uint32_t threshold_err_tb_cnt;
  /**<   Error threshold for the LTE transport block over which the
       service reports the BLER statistics.
  */
}qmi_coex_metrics_lte_bler_start_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Provides the transport block parameters for the LTE BLER metric.
Provides the requested transport block parameters for the LTE
              BLER metric. */
typedef struct {

  /* Optional */
  /*  Transport Block Count */
  uint8_t tb_cnt_valid;  /**< Must be set to true if tb_cnt is being passed */
  uint32_t tb_cnt;
  /**<   Current count of LTE transport blocks over which the
       BLER is collected.
  */

  /* Optional */
  /*  Errored Transport Block Count */
  uint8_t errored_tb_cnt_valid;  /**< Must be set to true if errored_tb_cnt is being passed */
  uint32_t errored_tb_cnt;
  /**<   Current count of errored LTE transport blocks over the
       total count, used by the client to compute the LTE BLER metric.
  */

  /* Optional */
  /*  Transport Block Count with Only Bluetooth Active */
  uint8_t tb_cnt_bt_only_valid;  /**< Must be set to true if tb_cnt_bt_only is being passed */
  uint32_t tb_cnt_bt_only;
  /**<   Current count of LTE transport blocks over which the
       BLER is collected while Bluetooth is active
       and Wi-Fi\reg is inactive.
  */

  /* Optional */
  /*  Errored Transport Block Count with Only Bluetooth Active */
  uint8_t errored_tb_cnt_bt_only_valid;  /**< Must be set to true if errored_tb_cnt_bt_only is being passed */
  uint32_t errored_tb_cnt_bt_only;
  /**<   Current count of errored LTE transport blocks over the
       total count, used by the client to compute the LTE BLER metric.
       This is collected while Bluetooth is active and Wi-Fi is inactive.
  */

  /* Optional */
  /*  Transport Block Count with Only Wi-Fi Active */
  uint8_t tb_cnt_wifi_only_valid;  /**< Must be set to true if tb_cnt_wifi_only is being passed */
  uint32_t tb_cnt_wifi_only;
  /**<   Current count of LTE transport blocks over which the
       BLER is collected while Wi-Fi is active
       and Bluetooth is inactive.
  */

  /* Optional */
  /*  Errored Transport Block Count with Only Wi-Fi Active */
  uint8_t errored_tb_cnt_wifi_only_valid;  /**< Must be set to true if errored_tb_cnt_wifi_only is being passed */
  uint32_t errored_tb_cnt_wifi_only;
  /**<   Current count of errored LTE transport blocks over the
       total count, used by the client to compute the LTE BLER metric.
       This is collected while Wi-Fi is active and Bluetooth is inactive.
  */

  /* Optional */
  /*  Transport Block Count with Bluetooth and Wi-Fi Active */
  uint8_t tb_cnt_bt_wifi_valid;  /**< Must be set to true if tb_cnt_bt_wifi is being passed */
  uint32_t tb_cnt_bt_wifi;
  /**<   Current count of LTE transport blocks over which the
       BLER is collected while both Wi-Fi and 
       Bluetooth are active.
  */

  /* Optional */
  /*  Errored Transport Block Count with Bluetooth and Wi-Fi Active */
  uint8_t errored_tb_cnt_bt_wifi_valid;  /**< Must be set to true if errored_tb_cnt_bt_wifi is being passed */
  uint32_t errored_tb_cnt_bt_wifi;
  /**<   Current count of errored LTE transport blocks over the
       total count, used by the client to compute the LTE BLER metric.
       This is collected while both Wi-Fi and Bluetooth are active.
  */

  /* Optional */
  /*  Transport Block Count with Only LTE Active */
  uint8_t tb_cnt_lte_only_valid;  /**< Must be set to true if tb_cnt_lte_only is being passed */
  uint32_t tb_cnt_lte_only;
  /**<   Current count of LTE transport blocks over which the
       BLER is collected while both Wi-Fi and 
       Bluetooth are inactive.
  */

  /* Optional */
  /*  Errored Transport Block Count with Only LTE Active */
  uint8_t errored_tb_cnt_lte_only_valid;  /**< Must be set to true if errored_tb_cnt_lte_only is being passed */
  uint32_t errored_tb_cnt_lte_only;
  /**<   Current count of errored LTE transport blocks over the
       total count, used by the client to compute the LTE BLER metric.
       This is collected while both Wi-Fi and Bluetooth are inactive.
  */
}qmi_coex_metrics_lte_bler_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Reads the current BLER metrics for the transport block parameters.
Reads the current BLER metric for the TB parameters sent in the
              BLER start request. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}qmi_coex_metrics_lte_bler_read_req_msg_v02;

  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Stops collection and collation of the LTE BLER metric.
Stops collection and collation of the LTE BLER metric. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}qmi_coex_metrics_lte_bler_stop_req_msg_v02;

  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Gets the parameters related to processing conflicting WCN and
              WWAN bands. */
typedef struct {

  /* Optional */
  /*  Victim Table Offset */
  uint8_t victim_tbl_offset_valid;  /**< Must be set to true if victim_tbl_offset is being passed */
  uint8_t victim_tbl_offset;
  /**<   Offset in the complete victim table from which to extract the sub-table
       with a maximum size of COEX_NV_MSG_ VICTIM_TABLE_MAX_ENTRIES in the response.

       \hangindent1.1cm \textbf{Note:} The offset is zero-based.
  */
}qmi_coex_conflict_params_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint16_t tech_type;
  /**<   WWAN technology type; bit-packed to represent the following fields:\n
       - bit[15:10] -- Reserved for future use \n
       - bit[9:8]   -- coex_tech_operating_ dimension; the RAT's dimension \n
       - bit[7:6]  -- coex_tech_band_direction: Uplink, Downlink, Both, or
                      Don't Care. Note that a band can be specified as both
                      uplink (UL) and downlink (DL) by combining the mask
                      values of UL and DL. \n
       - bit[5:0]  -- coex_tech_type; the COEX technology type
  */

  uint16_t frequency_range_start;
  /**<   WWAN lower bound for the RAT's operating frequency range,
      in MHz times 10; it is done in x10 to provide the fractional precision of
      1 decimal point. Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
    (2\textsuperscript{16}-1).
  */

  uint16_t frequency_range_stop;
  /**<   WWAN upper bound of RAT's operating frequency range, in MHz times 10; it
       is done in x10 to provide fractional precision of 1 decimal point.
       Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
    (2\textsuperscript{16}-1).
  */
}coex_wwan_conflict_band_range_type_v02;  /* Type */
/**
    @}
  */

typedef uint8_t coex_wlan_tech_operating_mode_v02;
#define COEX_WLAN_UNDEF_MODE_V02 ((coex_wlan_tech_operating_mode_v02)0x00) /**<  WLAN technology mode is undefined or unintialized  */
#define COEX_WLAN_CONN_MODE_V02 ((coex_wlan_tech_operating_mode_v02)0x01) /**<  WLAN technology is connected  */
#define COEX_WLAN_HIGH_PRIO_MODE_V02 ((coex_wlan_tech_operating_mode_v02)0x02) /**<  WLAN technology is in high priority mode  */
/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint16_t tech_type;
  /**<   WLAN technology type; bit-packed to represent the following fields:\n
       - bit[15:8] -- Reserved for future use \n
       - bit[7:6]  -- coex_wlan_tech_operating_ mode: connected or high_priority.
                      Note that the mode can be specified as both connected and
                      high_priority by combining the mask values of each in
                      these two bits\n
       - bit[5:0]  -- coex_tech_type; the COEX technology type
  */

  uint16_t frequency_range_start;
  /**<   WLAN lower bound of RAT's operating frequency range, in MHz times 10;
       this is represented in x10 to provide fractional precision of 1 decimal
       point. Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
    (2\textsuperscript{16}-1).
  */

  uint16_t frequency_range_stop;
  /**<   WLAN upper bound of RAT's operating frequency range, in MHz times 10;
       this is represented in x10 to provide fractional precision of 1 decimal
       point. Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
    (2\textsuperscript{16}-1).
  */
}coex_wlan_conflict_band_range_type_v02;  /* Type */
/**
    @}
  */

typedef uint16_t coex_wlan_policy_config_mask_v02;
#define COEX_WLAN_PCM_UNDEF_V02 ((coex_wlan_policy_config_mask_v02)0x0000) /**<  Undefined policy configuration mask for WLAN
   */
#define COEX_WLAN_PCM_ENFORCE_WWAN_RX_IMD_PROTECTION_V02 ((coex_wlan_policy_config_mask_v02)0x0001) /**<  Protect WWAN receive; apply the static power back-off or block
       the WLAN transmit based on WLAN RSSI
   */
#define COEX_WLAN_PCM_ENFORCE_DYNAMIC_WLAN_POWER_BACKOFF_V02 ((coex_wlan_policy_config_mask_v02)0x0002) /**<  Dynamically adjust the WLAN transmit power based on the modem Rx
       desensing due to the WLAN transmit
   */
#define COEX_WLAN_PCM_ENFORCE_BT_AFH_UPDATE_V02 ((coex_wlan_policy_config_mask_v02)0x0004) /**<  Update the Bluetooth AFH channels range
   */
#define COEX_WLAN_PCM_ENFORCE_WLAN_CHANNEL_AVOIDANCE_V02 ((coex_wlan_policy_config_mask_v02)0x0008) /**<  WLAN is to carry out channel avoidance
   */
#define COEX_WLAN_PCM_ENFORCE_TDM_V02 ((coex_wlan_policy_config_mask_v02)0x0010) /**<  Run TDM, which is a mechanism to avoid WLAN transmit and receive
       when the WWAN transmit is active
   */
#define COEX_WLAN_PCM_ENFORCE_MAX_WWAN_PROTECTION_V02 ((coex_wlan_policy_config_mask_v02)0x0020) /**<  Assume WWAN is extra sensitive to desensing due to the WLAN
       transmit, such as when using a suboptimal RF filter; apply aggressive
       power back-off or TDM
   */
#define COEX_PCM_ENFORCE_TDM_STICKY_V02 ((coex_wlan_policy_config_mask_v02)0x0040) /**<   Run sticky TDM, which is similar to COEX_PCM_ENFORCE_TDM except, after TDM
       is triggered, it stays triggered when the WLAN RX rate recovers to high
   */
#define COEX_PCM_ENFORCE_STATIC_WLAN_POWER_BACKOFF_V02 ((coex_wlan_policy_config_mask_v02)0x0080) /**<  Update Static Power Backoff
   */
/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_wwan_conflict_band_range_type_v02 wwan;
  /**<   WWAN band information for this conflict
  */

  coex_wlan_conflict_band_range_type_v02 wlan;
  /**<   WLAN band information for this conflict
  */

  coex_wlan_policy_config_mask_v02 wlan_coex_policy;
  /**<   Action to be taken by WLAN when this conflict is found. Values: \n
      - COEX_WLAN_PCM_UNDEF (0x0000) --  Undefined policy configuration mask for WLAN
  
      - COEX_WLAN_PCM_ENFORCE_WWAN_RX_IMD_PROTECTION (0x0001) --  Protect WWAN receive; apply the static power back-off or block
       the WLAN transmit based on WLAN RSSI
  
      - COEX_WLAN_PCM_ENFORCE_DYNAMIC_WLAN_POWER_BACKOFF (0x0002) --  Dynamically adjust the WLAN transmit power based on the modem Rx
       desensing due to the WLAN transmit
  
      - COEX_WLAN_PCM_ENFORCE_BT_AFH_UPDATE (0x0004) --  Update the Bluetooth AFH channels range
  
      - COEX_WLAN_PCM_ENFORCE_WLAN_CHANNEL_AVOIDANCE (0x0008) --  WLAN is to carry out channel avoidance
  
      - COEX_WLAN_PCM_ENFORCE_TDM (0x0010) --  Run TDM, which is a mechanism to avoid WLAN transmit and receive
       when the WWAN transmit is active
  
      - COEX_WLAN_PCM_ENFORCE_MAX_WWAN_PROTECTION (0x0020) --  Assume WWAN is extra sensitive to desensing due to the WLAN
       transmit, such as when using a suboptimal RF filter; apply aggressive
       power back-off or TDM
  
      - COEX_PCM_ENFORCE_TDM_STICKY (0x0040) --   Run sticky TDM, which is similar to COEX_PCM_ENFORCE_TDM except, after TDM
       is triggered, it stays triggered when the WLAN RX rate recovers to high
  
      - COEX_PCM_ENFORCE_STATIC_WLAN_POWER_BACKOFF (0x0080) --  Update Static Power Backoff
  
 */

  uint8_t wlan_policy_coex_wwan_rx_imd_protection_params;
  /**<   WLAN's WWAN receive IMD protection policy parameters.
       Parameters applying to COEX_WLAN_PCM_ ENFORCE_WWAN_RX_IMD_ PROTECTION policy;
       bit-packed to represent the following fields: \n
         - bit[7]   -- Reserved for future use \n
         - bit[6]   -- Boolean value to enable or disable blocking the WLAN transmit
                       when RSSI is too low \n
         - bit[5:0] -- Static WLAN transmit power limit, in dBm;
                       valid range: 0 dBm to approximately 25 dBm
  */
}coex_conflict_definition_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_CONFLICT_PARAMS_ESTATUS_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_CONFLICT_PARAMS_REQ_E_SUCCESS_V02 = 0x00, /**<   Conflict parameters request was successful  */
  COEX_CONFLICT_PARAMS_REQ_E_INVALID_ARG_V02 = 0x01, /**<   Victim table offset in the request is out of bounds  */
  COEX_CONFLICT_PARAMS_ESTATUS_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_conflict_params_estatus_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t request_estatus;
  /**<    Whether the request made was successful; coex_conflict_params_estatus
        gives the error status. Error status values: \n
        - COEX_CONFLICT_PARAMS_REQ_ E_SUCCESS (0x00) --
          Conflict parameters request was successful\n
        - COEX_CONFLICT_PARAMS_REQ_ E_INVALID_ARG (0x01) --
          Victim table offset in the request is out of bounds
  */

  uint8_t victim_tbl_offset;
  /**<   Offset in the complete victim table from which the provided table in this
        message was extracted; this number is equal to the offset in the
        request message. Offset is zero-based.
  */

  uint8_t victim_tbl_complete_size;
  /**<   Size of the complete victim table; used to determine when the
        complete victim table has been sent. If this value is greater than the size of the
        provided victim table upon request, the client can make more requests with a
        larger offset to get more pieces of the complete victim table.
  */

  uint32_t partial_victim_table_len;  /**< Must be set to # of elements in partial_victim_table */
  coex_conflict_definition_type_v02 partial_victim_table[COEX_NV_MSG_VICTIM_TABLE_MAX_ENTRIES_V02];
  /**<    An array of conflicts where each row contains:\n
        - WWAN band information \n
        - WLAN band information \n
        - Action (policy) to be taken by the WLAN when this conflict is found \n
        - WLAN's WWAN receive IMD protection policy parameters
  */
}coex_conflict_params_report_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Response Message; Provides a set of parameters related to processing
              conflicting WCN and WWAN bands. */
typedef struct {

  /* Optional */
  /*  Conflict Parameters Report */
  uint8_t report_valid;  /**< Must be set to true if report is being passed */
  coex_conflict_params_report_type_v02 report;
  /**<   \n
       (Current conflict parameter(s) report.)
  */
}qmi_coex_conflict_params_resp_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t index;
  /**<    Index into the victim table, indicates the active conflict.
  */

  uint8_t wan_carrier;
  /**<   Component carrier that belongs to the WWAN technology.
  */
}coex_active_conflict_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_wlan_policy_config_mask_v02 wlan_coex_policy;
  /**<   Action to be taken by WLAN when this conflict is found. Values: \n
      - COEX_WLAN_PCM_UNDEF (0x0000) --  Undefined policy configuration mask for WLAN
  
      - COEX_WLAN_PCM_ENFORCE_WWAN_RX_IMD_PROTECTION (0x0001) --  Protect WWAN receive; apply the static power back-off or block
       the WLAN transmit based on WLAN RSSI
  
      - COEX_WLAN_PCM_ENFORCE_DYNAMIC_WLAN_POWER_BACKOFF (0x0002) --  Dynamically adjust the WLAN transmit power based on the modem Rx
       desensing due to the WLAN transmit
  
      - COEX_WLAN_PCM_ENFORCE_BT_AFH_UPDATE (0x0004) --  Update the Bluetooth AFH channels range
  
      - COEX_WLAN_PCM_ENFORCE_WLAN_CHANNEL_AVOIDANCE (0x0008) --  WLAN is to carry out channel avoidance
  
      - COEX_WLAN_PCM_ENFORCE_TDM (0x0010) --  Run TDM, which is a mechanism to avoid WLAN transmit and receive
       when the WWAN transmit is active
  
      - COEX_WLAN_PCM_ENFORCE_MAX_WWAN_PROTECTION (0x0020) --  Assume WWAN is extra sensitive to desensing due to the WLAN
       transmit, such as when using a suboptimal RF filter; apply aggressive
       power back-off or TDM
  
      - COEX_PCM_ENFORCE_TDM_STICKY (0x0040) --   Run sticky TDM, which is similar to COEX_PCM_ENFORCE_TDM except, after TDM
       is triggered, it stays triggered when the WLAN RX rate recovers to high
  
      - COEX_PCM_ENFORCE_STATIC_WLAN_POWER_BACKOFF (0x0080) --  Update Static Power Backoff
  
 */

  uint8_t wwan_tech_and_carrier;
  /**<   Component carrier that belongs to the WWAN technology component of the
       conflict; bit-packed to represent the following fields:\n
       - bit[7:6] -- coex_carrier; the component carrier for the WWAN technology
       - bit[5:0] -- coex_tech_type; the WWAN COEX technology type
  */
}coex_rt_wlan_active_conflict_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_wlan_policy_config_mask_v02 wlan_coex_policy;
  /**<   Action to be taken by WLAN when this conflict is found. Values: \n
      - COEX_WLAN_PCM_UNDEF (0x0000) --  Undefined policy configuration mask for WLAN
  
      - COEX_WLAN_PCM_ENFORCE_WWAN_RX_IMD_PROTECTION (0x0001) --  Protect WWAN receive; apply the static power back-off or block
       the WLAN transmit based on WLAN RSSI
  
      - COEX_WLAN_PCM_ENFORCE_DYNAMIC_WLAN_POWER_BACKOFF (0x0002) --  Dynamically adjust the WLAN transmit power based on the modem Rx
       desensing due to the WLAN transmit
  
      - COEX_WLAN_PCM_ENFORCE_BT_AFH_UPDATE (0x0004) --  Update the Bluetooth AFH channels range
  
      - COEX_WLAN_PCM_ENFORCE_WLAN_CHANNEL_AVOIDANCE (0x0008) --  WLAN is to carry out channel avoidance
  
      - COEX_WLAN_PCM_ENFORCE_TDM (0x0010) --  Run TDM, which is a mechanism to avoid WLAN transmit and receive
       when the WWAN transmit is active
  
      - COEX_WLAN_PCM_ENFORCE_MAX_WWAN_PROTECTION (0x0020) --  Assume WWAN is extra sensitive to desensing due to the WLAN
       transmit, such as when using a suboptimal RF filter; apply aggressive
       power back-off or TDM
  
      - COEX_PCM_ENFORCE_TDM_STICKY (0x0040) --   Run sticky TDM, which is similar to COEX_PCM_ENFORCE_TDM except, after TDM
       is triggered, it stays triggered when the WLAN RX rate recovers to high
  
      - COEX_PCM_ENFORCE_STATIC_WLAN_POWER_BACKOFF (0x0080) --  Update Static Power Backoff
  
 */

  uint8_t wwan_tech_and_carrier;
  /**<   Component carrier that belongs to the WWAN technology component of the
       conflict; bit-packed to represent the following fields:\n
       - bit[7:6] -- coex_carrier; the component carrier for the WWAN technology
       - bit[5:0] -- coex_tech_type; the WWAN COEX technology type
  */

  uint16_t wlan_frequency;
  /**<   Frequency; radio access technology's operating center frequency in MHz
       (times 10). This value is represented in x10 to provide the fractional
       precision of one decimal point. Valid range: 0.0 MHz to 6553.5 MHz.
       Values: 0 to 65535 (2\textsuperscript{16}-1).
  */
}coex_rt_wlan_active_conflict_type_ex_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_wwan_conflict_frequency_range_v02 wan_frequency_range;
  /**<   WWAN frequency range that triggered this potential conflict. Because
       factors such as LTE carrier aggregation or multiple WAN technologies can
       create COEX conflicts where none exist on their own, this range is
       implemented as a mask. If multiple WAN frequency ranges cause an
       inter-modulation conflict, multiple simultaneous ranges can be set in
       the mask.
  */

  uint16_t wlan_frequency_range_start;
  /**<   WLAN lower bound of RAT's operating frequency range, in MHz times 10;
       this value is represented as x10 to provide fractional precision of 1
       decimal point. Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
       (2\textsuperscript{16}-1).
  */

  uint16_t wlan_frequency_range_stop;
  /**<   WLAN upper bound of RAT's operating frequency range, in MHz times 10;
       this value is represented in x10 to provide fractional precision of 1
       decimal point. Valid range: 0.0 MHz to 65535 MHz. Values: 0 to 65535
       (2\textsuperscript{16}-1).
  */
}coex_rt_wlan_channel_avoidance_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates the active victim table conflict indices. */
typedef struct {

  /* Optional */
  /*  Active Conflicts for COEX */
  uint8_t active_conflicts_valid;  /**< Must be set to true if active_conflicts is being passed */
  uint32_t active_conflicts_len;  /**< Must be set to # of elements in active_conflicts */
  coex_active_conflict_type_v02 active_conflicts[COEX_MAX_ACTIVE_CONFLICTS_V02];

  /* Optional */
  /*  Active Conflicts Between WWAN and WLAN */
  uint8_t wlan_active_conflicts_valid;  /**< Must be set to true if wlan_active_conflicts is being passed */
  uint32_t wlan_active_conflicts_len;  /**< Must be set to # of elements in wlan_active_conflicts */
  coex_rt_wlan_active_conflict_type_v02 wlan_active_conflicts[COEX_MAX_ACTIVE_CONFLICTS_V02];

  /* Optional */
  /*  Active Conflicts Between WWAN and WLAN */
  uint8_t wlan_active_conflicts_ex_valid;  /**< Must be set to true if wlan_active_conflicts_ex is being passed */
  uint32_t wlan_active_conflicts_ex_len;  /**< Must be set to # of elements in wlan_active_conflicts_ex */
  coex_rt_wlan_active_conflict_type_ex_v02 wlan_active_conflicts_ex[COEX_MAX_ACTIVE_CONFLICTS_V02];
}qmi_coex_active_conflict_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Reads the current coexistence conflicts. */
typedef struct {

  /* Optional */
  /*  Send Latest Active Conflicts */
  uint8_t resend_active_conflict_ind_valid;  /**< Must be set to true if resend_active_conflict_ind is being passed */
  uint8_t resend_active_conflict_ind;
  /**<    Indicates whether or not the MDM should send the latest 
        QMI_COEX_ACTIVE_CONFLICT_IND. If not present, the MDM sends the
        message.
  */

  /* Optional */
  /*  Send Latest Potential Conflicts */
  uint8_t resend_potential_conflict_ind_valid;  /**< Must be set to true if resend_potential_conflict_ind is being passed */
  uint8_t resend_potential_conflict_ind;
  /**<    Indicates whether or not the MDM should send the latest
        QMI_COEX_POTENTIAL_CONFLICT_IND. If not present, the MDM sends the
        message.
  */
}qmi_coex_active_conflict_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Indicates any potential conflicts based on the current WWAN
                     frequency. */
typedef struct {

  /* Optional */
  /*  Potential Conflicts for COEX */
  uint8_t wlan_potential_conflicts_valid;  /**< Must be set to true if wlan_potential_conflicts is being passed */
  uint32_t wlan_potential_conflicts_len;  /**< Must be set to # of elements in wlan_potential_conflicts */
  coex_rt_wlan_channel_avoidance_type_v02 wlan_potential_conflicts[COEX_MAX_CHANNEL_AVOID_CONFLICTS_V02];
}qmi_coex_potential_conflict_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_RAT_ID_BEHAVIOR_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_RAT_ID_DISABLED_V02 = 0, /**<  WCI-2 RAT ID is disabled (legacy behavior)  */
  COEX_RAT_ID_ENABLED_SINGLE_V02 = 1, /**<  WCI-2 RAT ID is enabled, but the modem dictates to which RAT ID the WCN
       reacts. In this case, only a single mapping is sent.  */
  COEX_RAT_ID_ENABLED_DYNAMIC_MAP_V02 = 2, /**<  WCI-2 RAT ID is enabled, and a mapping of all RAT IDs currently in use
       by MDM is sent  */
  COEX_RAT_ID_BEHAVIOR_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_rat_id_behavior_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t tech_and_carrier;
  /**<   Component carrier and technology ID, corresponding to the RAT ID below;
       bit-packed to represent the following fields:\n
       - bit[7:6] -- coex_carrier; the component carrier for a WWAN technology
       - bit[5:0] -- coex_tech_type; the technology type
  */

  uint8_t rat_id;
  /**<   RAT ID for the above tech_id. If RAT ID is enabled on the WCI-2 
       interface, this field is used to identify which technology is sending
       the WCI-2 real-time data in a preceding WCI-2 type 7 byte.
       Valid range for the RAT ID is 0x00 to 0x0F.
  */
}coex_rat_id_mapping_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Dictates the current WCI-2 MDM RAT ID to technology mappings. */
typedef struct {

  /* Optional */
  /*  RAT ID Mapping Behavior */
  uint8_t behavior_valid;  /**< Must be set to true if behavior is being passed */
  uint8_t behavior;
  /**<   coex_rat_id_behavior; identifies the expected operating mode of the 
       RAT ID feature. If not present, assume RAT ID is disabled. */

  /* Optional */
  /*  COEX RAT ID Mapping */
  uint8_t rat_ids_valid;  /**< Must be set to true if rat_ids is being passed */
  uint32_t rat_ids_len;  /**< Must be set to # of elements in rat_ids */
  coex_rat_id_mapping_v02 rat_ids[COEX_MAX_RAT_ID_MAPPINGS_V02];
}qmi_coex_rat_id_update_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Reads the MDM's current WCI-2 RAT ID configuration. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}qmi_coex_rat_id_update_req_msg_v02;

  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Bluetooth sends its channel mapping info to CXM. */
typedef struct {

  /* Optional */
  /*  BT Channel mapping info */
  uint8_t bt_channel_map_valid;  /**< Must be set to true if bt_channel_map is being passed */
  uint32_t bt_channel_map_len;  /**< Must be set to # of elements in bt_channel_map */
  uint8_t bt_channel_map[COEX_MAX_BT_CHANNEL_MAP_SIZE_V02];
  /**<   Channel mapping information of Bluetooth channels.
   */
}qmi_coex_bt_conn_info_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_GNSS_CRIT_DEMOD_ESTATUS_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_GNSS_CRIT_DEMOD_E_OFF_V02 = 0x00, /**<  GNSS critical demod is off  */
  COEX_GNSS_CRIT_DEMOD_E_ON_V02 = 0x01, /**<  GNSS critical demod is on  */
  COEX_GNSS_CRIT_DEMOD_ESTATUS_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_gnss_crit_demod_estatus_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the GNSS critical demod state to WLAN. */
typedef struct {

  /* Mandatory */
  /*  GNSS critical demod state */
  uint8_t gnss_demod_state;
  /**<   \n
      (Current state of GNSS critical demod, as requested.)
  */
}qmi_coex_gnss_crit_demod_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_TECH_CRIT_PAGE_ESTATUS_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_TECH_CRIT_E_NONE_V02 = 0x00, /**<  GNSS critical demod is off  */
  COEX_TECH_CRIT_E_PAGE_OK_V02 = 0x01, /**<  Critical state denoting page was not missed in the last cycle  */
  COEX_TECH_CRIT_E_PAGE_MISS_V02 = 0x02, /**<  Critical state denoting page was missed in the last cycle  */
  COEX_TECH_CRIT_PAGE_ESTATUS_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_tech_crit_page_estatus_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_WLAN_BAND_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_WLAN_BAND_E_INVALID_V02 = 0x00, /**<  GNSS critical demod is off  */
  COEX_WLAN_BAND_E_2G_V02 = 0x01, /**<  Critical state denoting page was not missed in the last cycle  */
  COEX_WLAN_BAND_E_5G_V02 = 0x02, /**<  Critical state denoting page was missed in the last cycle  */
  COEX_WLAN_BAND_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_wlan_band_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_tech_type_v02 tech;
  /**<   RAT reporting sleep/wakeup
  */

  uint8_t going_to_sleep;
  /**<   TRUE if going to sleep, FALSE if waking up
  */

  uint32_t duration;
  /**<   Time (ms) RAT will sleep/ remaining sleep durn for wakeup
  */

  coex_tech_crit_page_estatus_v02 tech_critical;
  /**<   Status of paging in the last cycle
  */

  int32_t rssi;
  /**<   RSSI value for the past paging cycle
  */

  coex_wlan_band_v02 wlan_band;
  /**<   Conflicting WLAN band type. Reserved for future use
  */
}coex_tech_sleep_wakeup_msg_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the sleep/wakeup state of the RAT. */
typedef struct {

  /* Optional */
  /*  Sleep/wakeup msg */
  uint8_t sleep_wakeup_valid;  /**< Must be set to true if sleep_wakeup is being passed */
  coex_tech_sleep_wakeup_msg_v02 sleep_wakeup;
  /**<   \n
      (Current state of the RAT sleep/wakeup, as requested.)
  */
}qmi_coex_tech_sleep_wakeup_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_2G_WLAN_MODE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_2G_WLAN_MODE_E_NONE_V02 = 0x00, /**<  Default/unused 2.4GHz WLAN mode  */
  COEX_2G_WLAN_MODE_E_SCAN_V02 = 0x01, /**<  2.4 GHz WLAN is in Scan mode  */
  COEX_2G_WLAN_MODE_E_STA_V02 = 0x02, /**<  2.4 GHz WLAN is in Infrastructure station mode  */
  COEX_2G_WLAN_MODE_E_SAP_V02 = 0x03, /**<  2.4 GHz WLAN is in Infrastructure AP mode  */
  COEX_2G_WLAN_MODE_E_P2P_V02 = 0x04, /**<  2.4 GHz WLAN is in P2P (Client or GO mode) mode  */
  COEX_2G_WLAN_MODE_E_IBSS_V02 = 0x05, /**<  2.4 GHz WLAN is in IBSS (Adhoc) station mode  */
  COEX_2G_WLAN_MODE_E_NAN_V02 = 0x06, /**<  2.4 GHz WLAN is in Near Area Network mode  */
  COEX_2G_WLAN_MODE_E_RTT_V02 = 0x07, /**<  2.4 GHz WLAN is in RTT mode  */
  COEX_2G_WLAN_MODE_E_MONITOR_V02 = 0x08, /**<  2.4 GHz WLAN is in Monitor mode  */
  COEX_2G_WLAN_MODE_E_OCB_V02 = 0x09, /**<  2.4 GHz WLAN is in Outside the context of BSS (11p) mode  */
  COEX_2G_WLAN_MODE_E_NDI_V02 = 0x0A, /**<  2.4 GHz WLAN is in Network Device Interface mode  */
  COEX_2G_WLAN_MODE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_2g_wlan_mode_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Sends a map for 2.4GHz WLAN channels for different modes. */
typedef struct {

  /* Mandatory */
  /*  WLAN Channel mapping list for different 2.4GHz operation modes */
  uint32_t wlan_mode_map;
  /**<   Bitmap of WLAN modes for which channel maps are being reported for.
           Bits[3:0]   -- WLAN mode for first entry in channel_map
           Bits[7:4]   -- WLAN mode for second entry in channel_map
           Bits[11:8]  -- WLAN mode for third entry in channel_map
           Bits[15:12] -- WLAN mode for fourth entry in channel_map
           Bits[31:16] -- Reserved for future use
   */

  /* Optional */
  /*  WLAN Channel mapping list for different 2.4GHz operation modes */
  uint8_t channel_map_valid;  /**< Must be set to true if channel_map is being passed */
  uint32_t channel_map_len;  /**< Must be set to # of elements in channel_map */
  uint16_t channel_map[COEX_MAX_WLAN_CONCURRENT_MODES_V02];
  /**<   Bitmap of channel mapping for 2.4GHz WLAN 
           Bits 0 to 13 represent WLAN 2.4Ghz Channels 1 to 14.
           Bit15/bit14 represent bandwidth. 00: 20MHz; 01: 40MHz
           10 and 11 are reserved for future. 10: 80 MHz; 11: 160 MHz
   */
}qmi_coex_2g_wlan_mode_chnl_map_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_WLAN_RTSAR_MSG_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_WLAN_RTSAR_ENABLE_CMD_V02 = 0x00, /**<  RTSAR enable command  */
  COEX_WLAN_RTSAR_DISABLE_CMD_V02 = 0x01, /**<  RTSAR disable command  */
  COEX_WLAN_RTSAR_ENABLE_ACK_V02 = 0x02, /**<  RTSAR enable acknowledgement  */
  COEX_WLAN_RTSAR_DISABLE_ACK_V02 = 0x03, /**<  RTSAR disable acknowledgement  */
  COEX_WLAN_RTSAR_ENABLE_REQ_V02 = 0x04, /**<  RTSAR enable request from WLAN  */
  COEX_WLAN_RTSAR_MSG_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_wlan_rtsar_msg_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_WLAN_MIMO_MODE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_WLAN_MIMO_MODE_NONE_V02 = 0x00, /**<  MIMO not supported by WLAN  */
  COEX_WLAN_MIMO_MODE_2X2_V02 = 0x01, /**<  WLAN in 2X2 MIMO mode  */
  COEX_WLAN_MIMO_MODE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_wlan_mimo_mode_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  int16_t tx_pwr_2_4_ghz_dbm10;
  /**<   Tx power (dbm10) for 2.4 GHz band
  */

  int16_t tx_pwr_5_0_ghz_dbm10;
  /**<   Tx power (dbm10) for 5.0 GHz band
  */
}coex_wlan_rtsar_pwr_report_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  int16_t sar_plimit_2_4_ghz_dbm10;
  /**<   Tx power limit (dbm10) for 2.4 GHz band
  */

  int16_t sar_plimit_5_0_ghz_dbm10;
  /**<   Tx power limit (dbm10) for 5.0 GHz band
  */
}coex_wlan_rtsar_pwr_limit_type_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Sends the RT-SAR intital configuration message to WLAN. */
typedef struct {

  /* Mandatory */
  /*  WLAN Config */
  coex_wlan_rtsar_msg_v02 msg_type;
  /**<   \n
      (WLAN Configuration request type)
  */

  /* Optional */
  /*  Reporting period */
  uint8_t reporting_period_msec_valid;  /**< Must be set to true if reporting_period_msec is being passed */
  uint16_t reporting_period_msec;
  /**<   \n
      (Reporting period in milliseconds. WLAN should report their TX
       power at this frequency.)
  */

  /* Optional */
  /*  Fallback period */
  uint8_t fallback_period_msec_valid;  /**< Must be set to true if fallback_period_msec is being passed */
  uint16_t fallback_period_msec;
  /**<   \n
      (Fallback period in milliseconds. If WLAN doesn't receive TX limits
       within this period, they can "fall back" their TX power to the
       fallback limits given below.)
  */

  /* Optional */
  /*  Fallback WLAN Pwr Limit for non-MIMO */
  uint8_t fallback_non_mimo_tx_limits_valid;  /**< Must be set to true if fallback_non_mimo_tx_limits is being passed */
  uint32_t fallback_non_mimo_tx_limits_len;  /**< Must be set to # of elements in fallback_non_mimo_tx_limits */
  coex_wlan_rtsar_pwr_limit_type_v02 fallback_non_mimo_tx_limits[COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02];
  /**<   \n
      (WLAN TX power limit for 2.4GHz and 5GHz on non-MIMO chains. These
       are fallback limits when WLAN has not received a new limit from
       modem after fallback_period_msec period of time)
  */

  /* Optional */
  /*  Fallback WLAN Pwr Limit for MIMO */
  uint8_t fallback_mimo_tx_limits_valid;  /**< Must be set to true if fallback_mimo_tx_limits is being passed */
  uint32_t fallback_mimo_tx_limits_len;  /**< Must be set to # of elements in fallback_mimo_tx_limits */
  coex_wlan_rtsar_pwr_limit_type_v02 fallback_mimo_tx_limits[COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02];
  /**<   \n
      (WLAN TX power limit for 2.4GHz and 5GHz on MIMO chains. These are
       fallback limits when WLAN has not received a new limit from modem
       after fallback_period_msec period of time)
  */
}qmi_coex_rtsar_wlan_cfg_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; WLAN sends its Smart transmit acknowledgement/request msg to CXM. */
typedef struct {

  /* Mandatory */
  /*  WLAN Config ack/req state */
  coex_wlan_rtsar_msg_v02 msg_type;
  /**<   \n
      (WLAN Configuration acknowledgement/request type)
  */

  /* Optional */
  /*  WLAN MIMO mode for 2.4GHz */
  uint8_t mimo_mode_2g_valid;  /**< Must be set to true if mimo_mode_2g is being passed */
  coex_wlan_mimo_mode_v02 mimo_mode_2g;
  /**<   \n
      (MIMO mode supported by WLAN HW on 2.4GHz chains)
  */

  /* Optional */
  /*  WLAN MIMO mode for 5GHz */
  uint8_t mimo_mode_5g_valid;  /**< Must be set to true if mimo_mode_5g is being passed */
  coex_wlan_mimo_mode_v02 mimo_mode_5g;
  /**<   \n
      (MIMO mode supported by WLAN HW on 5GHz chains)
  */

  /* Optional */
  /*  Number of WLAN TX chains */
  uint8_t num_tx_chains_valid;  /**< Must be set to true if num_tx_chains is being passed */
  uint8_t num_tx_chains;
  /**<   \n
      (Maximum number of TX chains supported by WLAN HW)
  */
}qmi_coex_rtsar_wlan_info_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the WLAN transmit powers on 2.4GHz and 5GHz bands. */
typedef struct {

  /* Mandatory */
  /*  WLAN Pwr Report */
  uint32_t tx_chain_len;  /**< Must be set to # of elements in tx_chain */
  coex_wlan_rtsar_pwr_report_type_v02 tx_chain[COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02];
  /**<   \n
      (WLAN power report for 2.4GHz and 5GHz WLAN on different TX chains.)
  */

  /* Optional */
  /*  Sequence number */
  uint8_t seq_num_valid;  /**< Must be set to true if seq_num is being passed */
  uint16_t seq_num;
  /**<   \n
      (Sequence number for WLAN power report.)
  */

  /* Optional */
  /*  Duration */
  uint8_t duration_valid;  /**< Must be set to true if duration is being passed */
  uint32_t duration;
  /**<   \n
      (Duration in msec for which WLAN is reporting its power.)
  */
}qmi_coex_rtsar_wlan_pwr_report_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the MIMO and non-MIMO SAR limits for 2.4G and 5G to WLAN. */
typedef struct {

  /* Mandatory */
  /*  WLAN Pwr Limit for non-MIMO */
  uint32_t non_mimo_tx_limits_len;  /**< Must be set to # of elements in non_mimo_tx_limits */
  coex_wlan_rtsar_pwr_limit_type_v02 non_mimo_tx_limits[COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02];
  /**<   \n
      (WLAN TX power limit for 2.4GHz and 5GHz on non-MIMO chains.)
  */

  /* Optional */
  /*  WLAN Pwr Limit for MIMO */
  uint8_t mimo_tx_limits_valid;  /**< Must be set to true if mimo_tx_limits is being passed */
  uint32_t mimo_tx_limits_len;  /**< Must be set to # of elements in mimo_tx_limits */
  coex_wlan_rtsar_pwr_limit_type_v02 mimo_tx_limits[COEX_RTSAR_MAX_WLAN_TX_CHAINS_V02];
  /**<   \n
      (WLAN TX power limit for 2.4GHz and 5GHz on MIMO chains.)
  */
}qmi_coex_rtsar_wlan_pwr_limit_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_VOLTE_STATE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_VOLTE_STATE_INACTIVE_V02 = 0x00, /**<  Voice over LTE is inactive  */
  COEX_VOLTE_STATE_ACTIVE_V02 = 0x01, /**<  Voice over LTE is active  */
  COEX_VOLTE_STATE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_volte_state_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_CDRX_STATE_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_CDRX_STATE_DISABLED_V02 = 0x00, /**<  CDRX is disabled for the LTE tech  */
  COEX_CDRX_STATE_ENABLED_V02 = 0x01, /**<  CDRX is enabled for the LTE tech  */
  COEX_CDRX_STATE_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_cdrx_state_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_aggregates
    @{
  */
typedef struct {

  coex_cdrx_state_v02 cdrx_state;
  /**<   CDRX state of the WWAN tech
  */

  uint16_t on_duration_timer;
  /**<   CDRX on duration timer in msec
  */

  uint16_t short_drx_timer;
  /**<   Short DRX timer duration in msec
  */

  uint16_t long_drx_cycle;
  /**<   Long DRX cycle duration in msec
  */

  uint16_t drx_inactivity_timer;
  /**<   DRX inactivity timer duration in msec
  */

  uint16_t drx_short_cycle_timer;
  /**<   DRX short cycle timer duration in msec
  */

  uint16_t drx_start_offset;
  /**<   DRX start offset duration in msec
  */
}coex_tech_cdrx_state_info_v02;  /* Type */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the VOLTE and CDRX sleep state of the WWAN RAT. */
typedef struct {

  /* Mandatory */
  /*  WWAN technology type */
  coex_tech_type_v02 tech;
  /**<   \n
      RAT reporting VOLTE and/or CDRX state
  */

  /* Mandatory */
  /*  Sync state token for the VOLTE/CDRX state */
  uint8_t volte_cdrx_sync_token;
  /**<   \n
     (Shares the current COEX synchronization state of Voice over LTE or
     connected discontinuous receiption between MDM and WCN)

       - bit[7:5] -- reserved for future use \n
       - bit[4:0] -- 5-bit revolving token that indicates if the state
                     changed; valid range: 0 to 31
  */

  /* Optional */
  /*  VOLTE state */
  uint8_t volte_state_valid;  /**< Must be set to true if volte_state is being passed */
  coex_volte_state_v02 volte_state;
  /**<   \n
      Voice over LTE state of the WWAN tech
  */

  /* Optional */
  /*  CDRX state and durations */
  uint8_t cdrx_info_valid;  /**< Must be set to true if cdrx_info is being passed */
  coex_tech_cdrx_state_info_v02 cdrx_info;
  /**<   \n
      CDRX state and durations for the WWAN tech
  */
}qmi_coex_tech_volte_cdrx_state_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Requests the WLAN power report for smart transmit. */
typedef struct {

  /* Optional */
  /*  Sequence number */
  uint8_t seq_num_valid;  /**< Must be set to true if seq_num is being passed */
  uint16_t seq_num;
  /**<   Sequence number for the WLAN power report request.
  */
}qmi_coex_rtsar_wlan_pwr_report_req_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Request Message; Acknowledges the WLAN power report for smart transmit. */
typedef struct {

  /* Optional */
  /*  Sequence number */
  uint8_t seq_num_valid;  /**< Must be set to true if seq_num is being passed */
  uint16_t seq_num;
  /**<   Sequence number for the acknowledgement of WLAN power report.
  */
}qmi_coex_rtsar_wlan_pwr_report_ack_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_enums
    @{
  */
typedef enum {
  COEX_BT_RTSAR_MSG_MIN_ENUM_VAL_V02 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  COEX_BT_RTSAR_ENABLE_CMD_V02 = 0x00, /**<  RTSAR enable command  */
  COEX_BT_RTSAR_DISABLE_CMD_V02 = 0x01, /**<  RTSAR disable command  */
  COEX_BT_RTSAR_MSG_MAX_ENUM_VAL_V02 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}coex_bt_rtsar_msg_v02;
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Sends the RT-SAR intital configuration message to BT. */
typedef struct {

  /* Mandatory */
  /*  BT Config */
  coex_bt_rtsar_msg_v02 msg_type;
  /**<   \n
      (BT Configuration request type)
  */

  /* Optional */
  /*  Reporting period */
  uint8_t reporting_period_msec_valid;  /**< Must be set to true if reporting_period_msec is being passed */
  uint16_t reporting_period_msec;
  /**<   \n
      (Reporting period in milliseconds. BT should report their TX
       power at this frequency.)
  */

  /* Optional */
  /*  Fallback period */
  uint8_t fallback_period_msec_valid;  /**< Must be set to true if fallback_period_msec is being passed */
  uint16_t fallback_period_msec;
  /**<   \n
      (Fallback period in milliseconds. If BT doesn't receive TX limits
       within this period, they can "fall back" their TX power to the
       fallback limit corresponding to the index given below.)
  */

  /* Optional */
  /*  Fallback BT Pwr Limit index */
  uint8_t fallback_limit_idx_valid;  /**< Must be set to true if fallback_limit_idx is being passed */
  uint8_t fallback_limit_idx;
  /**<   \n
      (Index in the TX power map. This is the fallback limit when BT has not
       received a new limit from modem for fallback_period_msec)
  */
}qmi_coex_rtsar_bt_cfg_ind_msg_v02;  /* Message */
/**
    @}
  */

/** @addtogroup coex_qmi_messages
    @{
  */
/** Indication Message; Reports the smart transmit power limit for BT. */
typedef struct {

  /* Optional */
  /*  BT power limit index */
  uint8_t power_limit_idx_valid;  /**< Must be set to true if power_limit_idx is being passed */
  uint8_t power_limit_idx;
  /**<   \n
      (Index in the TX power map indicating BT TX power limit)
  */

  /* Optional */
  /*  BT power limit */
  uint8_t sar_plimit_dbm10_valid;  /**< Must be set to true if sar_plimit_dbm10 is being passed */
  int16_t sar_plimit_dbm10;
  /**<   \n
      (Numerical value of BT-RTSAR TX power limit in dBm10)
  */
}qmi_coex_rtsar_bt_pwr_limit_ind_msg_v02;  /* Message */
/**
    @}
  */

/* Conditional compilation tags for message removal */ 
//#define REMOVE_QMI_COEX_2G_WLAN_MODE_CHNL_MAP_IND_V02 
//#define REMOVE_QMI_COEX_ACTIVE_CONFLICT_IND_V02 
//#define REMOVE_QMI_COEX_ACTIVE_CONFLICT_REQ_V02 
//#define REMOVE_QMI_COEX_BT_CONN_INFO_IND_V02 
//#define REMOVE_QMI_COEX_CONFLICT_PARAMS_REQ_V02 
//#define REMOVE_QMI_COEX_CONFLICT_PARAMS_RESP_V02 
//#define REMOVE_QMI_COEX_DIVERSITY_ANTENNA_STATE_IND_V02 
//#define REMOVE_QMI_COEX_GNSS_CRIT_DEMOD_STATE_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_BLER_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_BLER_READ_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_BLER_START_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_BLER_STOP_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_SINR_READ_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_SINR_READ_RESP_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_SINR_START_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE2_SINR_STOP_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_BLER_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_BLER_READ_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_BLER_START_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_BLER_STOP_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_SINR_READ_REQ_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_SINR_READ_RESP_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_SINR_START_IND_V02 
//#define REMOVE_QMI_COEX_METRICS_LTE_SINR_STOP_IND_V02 
//#define REMOVE_QMI_COEX_POTENTIAL_CONFLICT_IND_V02 
//#define REMOVE_QMI_COEX_RAT_ID_UPDATE_IND_V02 
//#define REMOVE_QMI_COEX_RAT_ID_UPDATE_REQ_V02 
//#define REMOVE_QMI_COEX_RTSAR_BT_CFG_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_BT_PWR_LIMIT_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_CFG_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_INFO_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_PWR_LIMIT_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_PWR_REPORT_ACK_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_PWR_REPORT_IND_V02 
//#define REMOVE_QMI_COEX_RTSAR_WLAN_PWR_REPORT_REQ_V02 
//#define REMOVE_QMI_COEX_TECH_SLEEP_WAKEUP_IND_V02 
//#define REMOVE_QMI_COEX_TECH_STATE_UPDATE_REQ_V02 
//#define REMOVE_QMI_COEX_TECH_SYNC_REQ_V02 
//#define REMOVE_QMI_COEX_TECH_SYNC_RESP_V02 
//#define REMOVE_QMI_COEX_TECH_SYNC_UPDATE_IND_V02 
//#define REMOVE_QMI_COEX_TECH_VOLTE_CDRX_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WLAN_BT_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WLAN_WIFI_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_GSM2_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_GSM3_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_GSM_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_HDR_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_LTE_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_NR5G2_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_NR5G_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_ONEX_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_TDSCDMA_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_WCDMA2_STATE_IND_V02 
//#define REMOVE_QMI_COEX_WWAN_WCDMA_STATE_IND_V02 

/*Service Message Definition*/
/** @addtogroup coex_qmi_msg_ids
    @{
  */
#define QMI_COEX_TECH_SYNC_REQ_V02 0x0020
#define QMI_COEX_TECH_SYNC_RESP_V02 0x0021
#define QMI_COEX_TECH_SYNC_UPDATE_IND_V02 0x0022
#define QMI_COEX_TECH_STATE_UPDATE_REQ_V02 0x0023
#define QMI_COEX_WWAN_LTE_STATE_IND_V02 0x0024
#define QMI_COEX_WWAN_TDSCDMA_STATE_IND_V02 0x0025
#define QMI_COEX_WWAN_GSM_STATE_IND_V02 0x0026
#define QMI_COEX_WWAN_ONEX_STATE_IND_V02 0x0027
#define QMI_COEX_WWAN_HDR_STATE_IND_V02 0x0028
#define QMI_COEX_WWAN_WCDMA_STATE_IND_V02 0x0029
#define QMI_COEX_WLAN_WIFI_STATE_IND_V02 0x002A
#define QMI_COEX_WLAN_BT_STATE_IND_V02 0x002B
#define QMI_COEX_DIVERSITY_ANTENNA_STATE_IND_V02 0x002C
#define QMI_COEX_METRICS_LTE_SINR_START_IND_V02 0x002D
#define QMI_COEX_METRICS_LTE_SINR_READ_REQ_V02 0x002E
#define QMI_COEX_METRICS_LTE_SINR_READ_RESP_V02 0x002F
#define QMI_COEX_METRICS_LTE_SINR_STOP_IND_V02 0x0030
#define QMI_COEX_WWAN_GSM2_STATE_IND_V02 0x0031
#define QMI_COEX_WWAN_GSM3_STATE_IND_V02 0x0032
#define QMI_COEX_CONFLICT_PARAMS_REQ_V02 0x0033
#define QMI_COEX_CONFLICT_PARAMS_RESP_V02 0x0034
#define QMI_COEX_WWAN_WCDMA2_STATE_IND_V02 0x0035
#define QMI_COEX_ACTIVE_CONFLICT_IND_V02 0x0036
#define QMI_COEX_ACTIVE_CONFLICT_REQ_V02 0x0037
#define QMI_COEX_POTENTIAL_CONFLICT_IND_V02 0x0038
#define QMI_COEX_RAT_ID_UPDATE_IND_V02 0x0039
#define QMI_COEX_RAT_ID_UPDATE_REQ_V02 0x003A
#define QMI_COEX_WWAN_LTE2_STATE_IND_V02 0x003B
#define QMI_COEX_METRICS_LTE2_SINR_START_IND_V02 0x003C
#define QMI_COEX_METRICS_LTE2_SINR_READ_REQ_V02 0x003D
#define QMI_COEX_METRICS_LTE2_SINR_READ_RESP_V02 0x003E
#define QMI_COEX_METRICS_LTE2_SINR_STOP_IND_V02 0x003F
#define QMI_COEX_METRICS_LTE_BLER_START_REQ_V02 0x0040
#define QMI_COEX_METRICS_LTE_BLER_IND_V02 0x0041
#define QMI_COEX_METRICS_LTE_BLER_READ_REQ_V02 0x0042
#define QMI_COEX_METRICS_LTE_BLER_STOP_REQ_V02 0x0043
#define QMI_COEX_METRICS_LTE2_BLER_START_REQ_V02 0x0044
#define QMI_COEX_METRICS_LTE2_BLER_IND_V02 0x0045
#define QMI_COEX_METRICS_LTE2_BLER_READ_REQ_V02 0x0046
#define QMI_COEX_METRICS_LTE2_BLER_STOP_REQ_V02 0x0047
#define QMI_COEX_WWAN_NR5G_STATE_IND_V02 0x0048
#define QMI_COEX_BT_CONN_INFO_IND_V02 0x0049
#define QMI_COEX_WWAN_NR5G2_STATE_IND_V02 0x004A
#define QMI_COEX_GNSS_CRIT_DEMOD_STATE_IND_V02 0x004B
#define QMI_COEX_TECH_SLEEP_WAKEUP_IND_V02 0x004C
#define QMI_COEX_2G_WLAN_MODE_CHNL_MAP_IND_V02 0x004D
#define QMI_COEX_RTSAR_WLAN_CFG_IND_V02 0x004E
#define QMI_COEX_RTSAR_WLAN_INFO_IND_V02 0x004F
#define QMI_COEX_RTSAR_WLAN_PWR_REPORT_IND_V02 0x0050
#define QMI_COEX_RTSAR_WLAN_PWR_LIMIT_IND_V02 0x0051
#define QMI_COEX_TECH_VOLTE_CDRX_STATE_IND_V02 0x0052
#define QMI_COEX_RTSAR_WLAN_PWR_REPORT_REQ_V02 0x0053
#define QMI_COEX_RTSAR_WLAN_PWR_REPORT_ACK_IND_V02 0x0054
#define QMI_COEX_RTSAR_BT_CFG_IND_V02 0x0055
#define QMI_COEX_RTSAR_BT_PWR_LIMIT_IND_V02 0x0056
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro coex_get_service_object_v02( ) that takes in no arguments. */
qmi_idl_service_object_type coex_get_service_object_internal_v02
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define coex_get_service_object_v02( ) \
          coex_get_service_object_internal_v02( \
            COEX_V02_IDL_MAJOR_VERS, COEX_V02_IDL_MINOR_VERS, \
            COEX_V02_IDL_TOOL_VERS )
/**
    @}
  */


#ifdef __cplusplus
}
#endif
#endif

