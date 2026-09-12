#ifndef RFM_DEVICE_CONFIG_TYPE_H
#define RFM_DEVICE_CONFIG_TYPE_H

/*
   @file
   rfm_device_config_type.h

   @brief
   RF Driver's common external interface file for device configuration
   information.

   @details

*/

/*===========================================================================
Copyright (c) 2011-2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfa_api/common/rfm_device_config_type.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/26/15   fhuo    Add ignore_same_band_restriction in rfm_concurrency_restriction_type
10/11/15   mah     TRM iRAT restriction band mask support
10/01/15   kg      Generalize the rx_op enum to account for tx rfm_devices
07/08/15   Saul    RF Device Max Bandwidth Interface
05/26/15   Saul    RFC Dynamic Properties API
05/19/15   Saul    Featurize DGLNA With FEATURE_CUST_1
04/30/15   Saul    CRAT Updates
10/17/14   krg     Updated the rf-trm interface structures for prx/drx and sub id
10/07/14   tks     Updated the rf-trm interface structures
09/02/14   Saul    Renamed var.
08/08/14   Saul    MULTI-SIM Capability Type For TRM
07/25/14   tks     Added new enum and strucut for reconfig status indication
05/28/14   tks     Added trm interface api's for concurrency management
03/13/14   sbm     3 mode support for SLTE.
02/18/14   kg      Update struct for shdr
01/03/13   vrb     Update TRM intf for EFS RFC compatibility
09/05/12   vrb     Deprecated the tech_supported and ant_supported elements
08/17/12   vrb     Further Updates to the RF interface to TRM
07/26/12   vrb     Updated interface to TRM
05/18/12   shb     Added gps_has_independent_receive_chain to 
                   rfm_devices_configuration_type
10/19/11   bmg     Added device/band concurrency restrictions data to the
                   rfm_devices_configuration_type data structure.
06/29/11   cd      Initial revision

============================================================================*/
#include "rfm_mode_types.h"
#include "rfm_device_types.h"
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define rx_operation operation

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines a bit mask of bands

  @details
  This data structure contains an array of n elements to represent the bands
  supported, based on a bit mask of type sys_band_class_e_type
*/
typedef struct 
{
  uint64 mask[3];
}rfm_bands_bitmask;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines a combination of devices, technologies, and bands
  which are incompatible in the current configuration.

  @details
  This data structure is used to return a table of mode combinations which
  are not compatible with the RF driver front end configuration in the
  target.  Most simultaneous configurations are expected to be valid, so
  only the disallowed configurations will be listed.

  As an example, assume an SVDO configuration where 1x and DO have no
  restrictions on single-chain operation, but if both 1x and HDR are
  on in BC0, neither technology can enable diversity.  This would be
  the case if there are only two BC0 antennas in the system, such that
  1x is using one antenna for its single-chain BC0 operation, and DO
  is using the other antenna for its single-chain BC0 operation.  With
  no antennas left, neither 1x nor DO can enable diversity.  Assume:
  - RFM_DEVICE_0: HDR Rx and Tx
  - RFM_DEVICE_1: HDR Rx diversity
  - RFM_DEVICE_2: 1x Rx and Tx
  - RFM_DEVICE_3: GPS
  - RFM_DEVICE_4: 1x Rx diversity

  A table with these rows:
  - { RFM_DEVICE_0, RFM_1XEVDO_MODE, RFM_CDMA_BC0, RFM_DEVICE_4, RFM_1X_MODE, RFM_CDMA_BC0 },
  - { RFM_DEVICE_2, RFM_1X_MODE, RFM_CDMA_BC0, RFM_DEVICE_1, RFM_1XEVDO_MODE, RFM_CDMA_BC0 }

  would mean:
  - "If HDR is on Device 0 on BC0 => 1x cannot use Device 4 for BC0 (diversity) AND
    If 1x is on Device 4 on BC0 (diversity) => HDR cannot use Device 0 for BC0"
  - "If 1x is on Device 2 on BC0 => HDR cannot use Device 1 for BC0 (diversity) AND
    If HDR is on Device 1 on BC0 (diversity) => 1x cannot use Device 2 for BC0"
*/
typedef struct
{
  uint32               device_group1;
  rfm_bands_bitmask    band_group1;

  uint32               device_group2;
  rfm_bands_bitmask    band_group2;
  
  boolean              ignore_same_band_restriction;
  
} rfm_concurrency_restriction_type;

#ifdef FEATURE_CUST_1
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines pairs of devices/modes/bands that can operate
  concurrently with dglna

  @details
  This data structure contains device-sys_band_class_e_type pairs to represent the bands
  supported concurrently for dglna
*/
typedef struct
{
  rfm_device_enum_type device1;
  uint32               band1;

  rfm_device_enum_type device2;
  uint32               band2;
} rfm_dglna_concurrency_type;
#endif //#ifdef FEATURE_CUST_1
#if 0
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines devices that are associated and restrict operation to a 
  singe tech as well as defince VCO COUPLING needs
*/
typedef struct
{
  rfm_device_enum_type *associated_devices;
} rfm_associated_devices_type;
#endif
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines a bit mask for device capabilities

  @details
  RFM_DEVICE_INVALID_SUPPORT represents neither Rx nor Tx is supported on
  a logical device.
  RFM_DEVICE_RX_SUPPORTED represents whether Rx functionality is supported on
  a logical device.
  RFM_DEVICE_TX_SUPPORTED represents whether Tx functionality is supported on
  a logical device,
  RFM_DEVICE_PREFERRED_SUBS_2 represents whether the logical device should be
  the preferred choice for a second subscription under DSDA context
*/
typedef enum
{
  RFM_DEVICE_INVALID_SUPPORT = 0x0,
  RFM_DEVICE_RX_SUPPORTED = 0x01,
  RFM_DEVICE_TX_SUPPORTED = 0x02,
} rfm_device_capability_type;


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure defines a bit mask for concurrency features

  @details
  RFM_SINGLE_RX_DSDS_SUPPORTED - In this dual-sim mode, only one technology is
    active at a given time, other technology is suspended when new tech comes
    up. Also referred to as tune away DSDS.
  RFM_DUAL_RX_DSDS_SUPPORTED  - Two receivers can be active simultaneously. For
    eg. LTE B1 TX+RX in connected state and GSM in paging. Some transceivers
    support DR-DSDS with a single WTR, some bands can be supported without
    diversity sharing (i.e. without giving up diversity). Some transceivers can
    only do DR-DSDS with diversity sharing since WTR3905 can only have two LNAs
    ON at any given time.
  RFM_DUAL_TX_DSDA_SUPPORTED  - This is supported on HW that has two independent
    TX chains available for DSDA, going out of two independent antennas. This is
    legacy DSDA implementation.
  RFM_SINGLE_TX_DSDA_SUPPORTED - One TX chain is time multi-plexed between two
    technologies. For eg, with some transceivers, LTE and GSM can be in
    connected mode simultaneously. LTE RX on CA1, GSM RX on CA2, and TX time
    shared between LTE and GSM.

*/

typedef enum                           
{                                      
  RFM_NO_CONCURRENCY_SUPPORTED = 0x00000000,
  RFM_SINGLE_RX_DSDS_SUPPORTED = 0x00000001, 
  RFM_DUAL_RX_DSDS_SUPPORTED   = 0x00000002,   
  RFM_DUAL_TX_DSDA_SUPPORTED   = 0x00000004,   
  RFM_SINGLE_TX_DSDA_SUPPORTED = 0x00000008,
  RFM_SGLTE_SUPPORTED          = 0x00000010,
  RFM_SVLTE_SUPPORTED          = 0x00000020,
  RFM_SLTE_SUPPORTED           = 0x00000040,
  RFM_SRLTE_SUPPORTED          = 0x00000080,
  RFM_SHDR_SUPPORTED           = 0x00000100,
  RFM_SVDO_SUPPORTED           = 0x00000200,
  RFM_SGTDS_SUPPORTED          = 0x00000400
} rfm_concurrency_feature_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfm_devices_configuration_type
  This structure explains the features that are supported by all devices.
*/
typedef struct 
{
  uint32 hwid; 

  struct
  {
    rfm_bands_bitmask bands_supported;  
    /*!<
    @brief
    192 Bits of the Bands Supported Bit Mask

    @details
    Bits 0-191 of the Bit mask indicating the bands supported by this 
    logical device. 
    This uses the type sys_band_class_e_type to construct the bitmask
    */

    rfm_bands_bitmask preferred_bands_supported;  
    /*!<
    @brief
    192 Bits of the Preferred Bands Supported Bit Mask

    @details
    Bits 0-191 of the Bit mask indicating the preferred bands supported by this 
    logical device. 
    This uses the type sys_band_class_e_type to construct the bitmask
    */

    rfm_bands_bitmask ho_rxd_bands_supported;
    /*!<
    @brief
    192 Bits of the Bands Supported Bit Mask for HORxD

    @details
    Bits 0-191 of the Bit mask indicating the bands supported by this 
    logical device. 
    This uses the type sys_band_class_e_type to construct the bitmask
    */

    uint32 capability;
    /*!<
    @brief
    A bit mask to identify the RF capabilities (RX or TX) of this RFM device

    @details
    The bits of the bitmask is defined by rfm_device_capability_type
    */ 

    rfm_device_enum_type preferred_associated_rx_device;
    /*!<
    @brief
    VCO Based Preferred Associated Rx Device 

    @details
    Used to indentify the preferred associated Rx device on same tranceiver.
    Indicates the logical device that is associated with this device. 
    Typically, RFM_DEVICE_1 would operate as the RxD device to RFM_DEVICE_0 
    In this case, the associated_device for RFM_DEVICE_1 would be 0 and vice 
    versa. 
    */ 

    rfm_device_enum_type preferred_associated_tx_device;
    /*!<
    @brief
    Tranceiver Based Preferred Associated Tx Device 

    @details
    Captures the preferred associated tx device on same tranceiver for corresponding main Rx device.
    This item is purely for RF. TRM is not expected to make use of this item.
    */
  } device_support[RFM_MAX_WAN_DEVICES];
  /*!< This describes the functionality that each RF Device supports */ 

  /*!
    @brief
    This structure refers to an array of concurrency restrictions in the
    current RF configuration.

    @details
    The concurrency restrictions list modes and bands of operation that are
    incompatible with the current RF hardware configuration.  This will
    commonly be due to inter-antenna band restrictions, where two RF devices
    cannot simultaneously use the same antenna in certain bands.

    This incompatiblity table only describes restrictions due to concurrency,
    it does not provide any information about non-concurrent operation of
    a device on a given band or technology.  The basic capabilities of
    a device in a technology and band must be validated with the 
    rfm_is_band_chan_supported(), rfm_is_band_chan_supported_v2(), and
    rfm_is_tunable_on_antenna() APIs.
  */
  struct
  {
    /*!
      @brief
      The number of entries in the array pointed to by the restriction_table
      member of this structure.

      @details
      If this number is zero, the restriction_table member may be NULL, and
      there are no concurrency restrictions listed.
    */
    int32 num_restrictions;

    /*!
      @brief
      A pointer to the beginning of table of concurrency restrictions.

      @details
      If the num_restrictions member of this structure is zero, this
      pointer may be NULL, and should never be accessed.  Otherwise,
      this pointer will point to the beginning of an array of
      N rfm_concurrency_restriction_type entries, where N is
      num_restrictions.
    */
    const rfm_concurrency_restriction_type* restriction_table;
  } concurrency_restrictions;

  #ifdef FEATURE_CUST_1
  /* DGLNA Concurrency Info */
  struct
  {
    /*!
      @brief
      The number of entries in the array pointed to by the dglna_concurrency_table
      member of this structure.

      @details
      If this number is zero, the table may be NULL, and
      there are no DGLNA concurrencies listed.
    */
    uint32 num_dglna_concurrencies;

    /*!
      @brief
      A pointer to the beginning of table of DGLNA concurrencies.
    */
    const rfm_dglna_concurrency_type* dglna_concurrency_table;
  } dglna_concurrency_info;
  #endif //FEATURE_CUST_1

  boolean antenna_swap_supported;
  /* Bitmask, indexed by enums of rfm_concurrency_feature_type */
  uint32 concurrency_features;
  
} rfm_devices_configuration_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  enum to differentiate the logical device reservation 
 
  @details
  RFM_DEVICE_ASSIGNED means that a particular logical device has been
  reserved for requesting tech
 
  RFM_DEVICE_EXPECTED means that a particular logical device is expected
  to have reservation for a new tech
 
  RFM_DEVICE_ALLOCATION_INVALID means that for a particular logical device
  there is no allocation 
 
*/
typedef enum
{
  RFM_DEVICE_ASSIGNED,
  RFM_DEVICE_EXPECTED,
  RFM_DEVICE_ALLOCATION_INVALID
}rfm_device_assignment_type;


/*----------------------------------------------------------------------------*/
/*! 
  @brief
  enumeration to indicate prx/drx for the concurrency manager
*/
typedef enum
{
  RFM_PRX_OPERATION = 0,
  RFM_DRX_OPERATION,
  RFM_PTX_OPERATION,
  RFM_INVALID_OPERATION = 0xFF 
}rfm_device_operation_type; 

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  enumeration to indicate sub ID for the concurrency manager
*/
typedef enum
{
  RFM_SUB_ID_1 = 0,               /* Multi-mode subscription*/
  RFM_SUB_ID_2,                   /* Typically GSM only subscription*/
  RFM_INVALID_SUB_ID = 0xFF 
}rfm_device_sub_id_type; 

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Structure to hold the tech, band and logical device allocation information
 
  @details
  The structure is used primarily for TRM interface. The structure is populated
  by TRM and used to determine the concurrency scenario. 
 
*/
typedef struct
{
  struct
  {
    rfm_mode_enum_type tech;
    rfm_device_assignment_type allocation_type;
    sys_band_class_e_type band;
    uint8 alt_path; 
    rfm_device_operation_type operation;
    rfm_device_sub_id_type sub_id;
  }device_allocation[RFM_MAX_DEVICES];
}rfm_device_allocation_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Structure to hold the tech, band and logical device allocation information
 
  @details
  The structure is used primarily for TRM interface. The structure is populated
  by TRM and used to determine the concurrency scenario. 
 
*/
typedef struct
{
  rfm_device_enum_type rfm_dev;
  rfm_mode_enum_type tech;
  sys_band_class_e_type band;
  rfm_device_operation_type operation;
  rfm_device_sub_id_type sub_id;
}rfm_device_client_info_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  enumeration to indicate re-config status to trm
*/
typedef enum
{
  RFM_DEVICE_RECONFIG_REQUIRED = 0,
  RFM_DEVICE_RECONFIG_NOT_REQD = 1, 
  RFM_DEVICE_CURRENTLY_INACTIVE = 2, 
  RFM_DEVICE_STATUS_NUM,
  RFM_DEVICE_STATUS_INVALID
}rfm_device_reconfig_status_type; 

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Structure to hold the reconfiguration status for all logical devices
 
  @details
  The structure is used primarily for TRM interface. It is populated by RF
  concurrency mananger as part of the validate scenario check. RF response
  is used by TRM to decide on whether a chain should be granted or not. 
 
*/
typedef struct
{
  rfm_device_reconfig_status_type curr_status[RFM_MAX_WAN_DEVICES]; 
  uint8 alt_path[RFM_MAX_WAN_DEVICES]; 
}rfm_device_allocation_status_type; 

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure captures the association of RFM_DEVICE + BAND_GROUP -> Antenna Num
*/
typedef struct
{
  rfm_device_enum_type rfm_device;
  rfm_bands_bitmask band_group;
  uint32 antenna_num;  
} rfm_antenna_mapping_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Defines max size of antenna mapping table.
  With 4 antennas and 10 RFM_DEVICEs, max should be 40
*/
#define MAX_RFM_ANTENNA_MAPPING_ENTRIES 50

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure captures a table of antenna mappings.
*/
typedef struct
{
  uint32 num_antenna_mapping_entries;
  rfm_antenna_mapping_type antenna_mapping[MAX_RFM_ANTENNA_MAPPING_ENTRIES];  
} rfm_device_antenna_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Defines max size of device groups.
  With 4 antennas and 10 RFM_DEVICEs, max should be 40
*/
#define MAX_RFM_DEVICE_GROUPS 5

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure defines a table of device group bitmask
*/
typedef struct
{
  uint32 num_rfm_device_group_entries;
  /*! Bitmask indexed on rfm_device_enum_type */
  uint32 rfm_device_group_bitmask[MAX_RFM_DEVICE_GROUPS];
} rfm_device_groups_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure defines a single table item for device max bandwidth info
 
  @details
  Defines a group of devices and sys_bands that belog with a specific bandwidth
*/
typedef struct
{
  /*! Mask of devices based on rfm_device_enum_type */
  uint32 device_mask;
  /*! Band mask based on sys_band */
  rfm_bands_bitmask band_group;
  /*! Max bandwidth in raw KHz */
  uint32 max_bw_khz;
} rfm_device_max_bw_info_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Defines max size of device bandwidth table.
  Num elements is dominated by the different bandwidths
  LTE has the most bandwidths (rfcom_lte_bw_type).
  Other techs have bw (rfcom_xyz_bw_type) with same vals as LTE
*/
#define RFM_MAX_DEVICE_BW_GROUPS 15

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure defines a table of max bandwidth information
*/
typedef struct
{
  /*! The number of entries available in bw info table */
  uint32 num_max_bw_info_entries;
  /*! The bw info table */
  rfm_device_max_bw_info_type device_max_bw_info[ RFM_MAX_DEVICE_BW_GROUPS ];
} rfm_device_max_bw_groups_type;

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  This structure consolidates dynamic properties of the rf card.
*/
typedef struct
{
  rfm_device_groups_type single_rat_restriction_groups;
  rfm_device_antenna_type antenna_device_association;
  rfm_device_max_bw_groups_type device_max_bw_groups;
  /*! Band mask for which irat is restricted to preferred device only */ 
  rfm_bands_bitmask irat_restriction_band_group;
} rfm_rfcard_dynamic_properties_type;

/*----------------------------------------------------------------------------*/
boolean
rfm_get_devices_configuration
(
  rfm_devices_configuration_type *dev_cfg
);

/*----------------------------------------------------------------------------*/
boolean
rfm_inform_device_allocation
(
  rfm_device_allocation_type *dev_allocation
);

/*----------------------------------------------------------------------------*/
boolean
rfm_validate_device_concurrency
(
  rfm_device_client_info_type *dev_client 
);

/*----------------------------------------------------------------------------*/
rfm_device_allocation_status_type* rfm_verify_rf_concurrency
(
  rfm_device_client_info_type *dev_client
);

/*----------------------------------------------------------------------------*/
boolean
rfm_get_rfcard_dynamic_properties
(
  rfm_rfcard_dynamic_properties_type *rfc_cfg
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* RFM_DEVICE_CONFIG_TYPE_H */

