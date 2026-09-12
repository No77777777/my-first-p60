#ifndef _DS_RMNET_SHIM_H
#define _DS_RMNET_SHIM_H
/*===========================================================================

                 D S _ R M N E T _ S H I M . H

DESCRIPTION

  RMNET physical devices - Definitions header file.

EXTERNALIZED FUNCTIONS

Copyright (c) 2005-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/rmifacectls/inc/ds_rmnet_shim.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/24/13    wc     Initial file
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ds_rmnet_xport.h"

/*---------------------------------------------------------------------------
  enum for RmNet ipa error
---------------------------------------------------------------------------*/
typedef enum
{
  RMNET_IPA_SUCCESS           = 0,
  RMNET_ERR_IPA_FAIL          = 1,
  RMNET_ERR_MODEM_SSR         = 2,
  RMNET_ERR_MAX               = 0xFF
} rmnet_err_enum_type;


/*===========================================================================

                                 DEFINITIONS

===========================================================================*/


/*===========================================================================

                           EXTERNAL FUNCTIONS 

===========================================================================*/

/*===========================================================================
  FUNCTION RMNET_SHIM_GET_PHYS_SIO_PORT()

  DESCRIPTION
    This function assigns a physical SIO port

  PARAMETERS
    None

  RETURN VALUE
    Physical SIO port

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
sio_port_id_type rmnet_shim_get_phys_sio_port
(
  void
);

/*===========================================================================
  FUNCTION RMNET_SHIM_FREE_PHYS_SIO_PORT()

  DESCRIPTION
    This function frees a physical SIO port

  PARAMETERS
    sio_port: physical SIO port id 

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void rmnet_shim_free_phys_sio_port
(
  sio_port_id_type  sio_port
);

/*===========================================================================
  FUNCTION RMNET_SHIM_GET_LOGICAL_SIO_PORT()

  DESCRIPTION
    This function assigns a logical SIO port

  PARAMETERS
    None

  RETURN VALUE
    Logical SIO port

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
sio_port_id_type rmnet_shim_get_logical_sio_port
(
  void
);

/*===========================================================================
  FUNCTION RMNET_SHIM_FREE_LOGICAL_SIO_PORT()

  DESCRIPTION
    This function frees a logical SIO port

  PARAMETERS
    sio_port: logical SIO port id 

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void rmnet_shim_free_logical_sio_port
(
  sio_port_id_type  sio_port
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_PER_MAPPING()

  DESCRIPTION
    This function configs driver of SIO to peripheral mapping

  PARAMETERS
    phys_xport:   rmnet phys xport structure

  RETURN VALUE
    RmNet error

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
rmnet_err_enum_type rmnet_shim_set_per_mapping
(
  rmnet_phys_xport_type   * phys_xport
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_DATA_FORMAT()

  DESCRIPTION
    This function configs driver's data format

  PARAMETERS
    phys_xport:   rmnet phys xport structure
    data_format:  pointer to data format

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_set_data_format
(
  rmnet_phys_xport_type   * phys_xport,
  rmnet_data_format_type  * data_format
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_PORT_MAPPING()

  DESCRIPTION
    This function configs driver of logical to physical port mapping

  PARAMETERS
    xport_info:   rmnet xport structure

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_set_port_mapping
(
  rmnet_xport_type        * xport_info
);

/*===========================================================================
  FUNCTION RMNET_SHIM_UPDATE_ETH_HEADER()

  DESCRIPTION
    This function updates Ethenet header.

  PARAMETERS
    xport_info:   rmnet xport structure
    eth_hdr:      Ethenet header
    eth_hdr_len:  Ethenet header length
    is_v6:        Is v6 call

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_update_eth_header
(
  rmnet_xport_type        * xport_info,
  void                    * eth_hdr,
  uint8                     eth_hdr_len,
  boolean                   is_v6
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_PORT_MAPPING()

  DESCRIPTION
    This function registers EP config done callback with the driver

  PARAMETERS
    cb_fn:    callback function
    cb_data:  callback data

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_register_ep_cfg_done_cb
(
  void (* cb_fn)(sio_stream_id_type, uint32),
  uint32  cb_data
);

/*===========================================================================
  FUNCTION RMNET_SHIM_REGISTER_QMAP_CMD_CB()

  DESCRIPTION
    This function registers QMAP cmd callbacks

  PARAMETERS
    None

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_register_qmap_cmd_cb
(
  void
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SEND_QMAP_CMD()

  DESCRIPTION
    This function sends a QMAP cmd

  PARAMETERS
    None

  RETURN VALUE
    TRUE: Success
    FALSE: Failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_send_qmap_cmd
(
  rmnet_xport_type  * xport_info,
  uint8               cmd,
  uint8               cmd_type,
  dsm_item_type     * pkt,
  uint32            * tx_id,
  boolean             is_retx
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_PHYS_RX_FLOW()

  DESCRIPTION
    This function sets phys port UL flow control state

  PARAMETERS
    phys_xport: phys xport info
    disable:    is flow disable

  RETURN VALUE
    TRUE if success

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_set_phys_rx_flow
(
  rmnet_phys_xport_type  * phys_xport,
  boolean                  disable
);

/*===========================================================================
  FUNCTION RMNET_SHIM_GET_PIPE_STATS()

  DESCRIPTION
    This function query hw pipe statistics

  PARAMETERS
    pipe_num:   hw pipe num
    num_pkts:   ptr to store number of packets

  RETURN VALUE
    TRUE if success

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean rmnet_shim_get_pipe_stats
(
  uint32    pipe_num,
  uint32  * num_pkts
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_OFFLOAD_FC()

  DESCRIPTION
    Flow control the offloaded traffic.

  PARAMETERS
    xport_info       : ptr to rmnet xport
    flow_disable     : TRUE -- disable flow
                       FALSE -- enable flow

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void rmnet_shim_set_offload_fc
(
  rmnet_xport_type  * xport_info,
  boolean             flow_disable
);
#ifdef FEATURE_DATA_FLOW_MGMT
/*===========================================================================
  FUNCTION RMNET_SHIM_IPA_TO_DS_QMAP_CMD()

  DESCRIPTION
    Convert IPA flow control command into DS flow control cmd

  PARAMETERS
    ipa_cmd_name       : ipa flow control command name

  RETURN VALUE
    DS flow control command name

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

uint32 rmnet_shim_ipa_to_ds_qmap_cmd
(
  uint32 ipa_cmd_name
);
#endif/* FEATURE_DATA_FLOW_MGMT*/ 

/*===========================================================================
  FUNCTION RMNET_SHIM_IPA_SET_DL_BURST_MARKER()

  DESCRIPTION
    This function will invoke IPA API to enable/disable downlink marker

  PARAMETERS
    is_enabled : TRUE if DL marker is needed to be enabled, otherwise FALSE

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void rmnet_shim_ipa_set_dl_burst_marker
(
  boolean is_enabled
);

/*===========================================================================
  FUNCTION RMNET_SHIM_SET_LOW_LAT_RULE()

  DESCRIPTION
    Sets the Low Latency Rule at IPA

  PARAMETERS
    rule_enable      : TRUE -- Enable Rule
                       FALSE -- Disable Rule

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void rmnet_shim_set_low_latency_rule
(
  boolean             rule_enable
);

#endif /* _DS_RMNET_SHIM_H */

