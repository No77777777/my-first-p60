#ifndef _DS_QMAP_H
#define _DS_QMAP_H
/*===========================================================================

                         D S _ Q M A P . H

DESCRIPTION

  Qualcomm Mux and Aggregation - Definitions header file.

EXTERNALIZED FUNCTIONS

Copyright (c) 2005-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/rmifacectls/inc/ds_qmap.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/27/13    wc     Support dynamic port configuration
05/24/13    wc     Initial file
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "sio.h"
#include "timer.h"
#include "ds_rmnet_xport.h"

/*===========================================================================

                                 DEFINITIONS

===========================================================================*/

#define DS_QMAP_CMD_OFFSET         (4)
#define DS_QMAP_CMD_TYPE_OFFSET    (5)
#define DS_QMAP_CMD_PAYLOAD_OFFSET (12)

/*---------------------------------------------------------------------------
 * QMAP commands
---------------------------------------------------------------------------*/
enum ds_qmap_cmd_enum
{
  DS_QMAP_CMD_FLOW_DISABLE      = 1,
  DS_QMAP_CMD_FLOW_ENABLE       = 2,
  DS_QMAP_CMD_UL_DATA_SENT_CNF  = 3
};

enum ds_qmap_cmd_type_enum
{
  DS_QMAP_CMD_TYPE_REQUEST      = 0,
  DS_QMAP_CMD_TYPE_ACK          = 1,
  DS_QMAP_CMD_TYPE_UNSUPPORTED  = 2,
  DS_QMAP_CMD_TYPE_INVALID      = 3
};

enum ds_qmap_ip_family_enum
{
  DS_QMAP_ADDR_FAMILY_ANY = 0,
  DS_QMAP_ADDR_FAMILY_V4  = 1,
  DS_QMAP_ADDR_FAMILY_V6  = 2,
};
#ifdef FEATURE_DATA_FLOW_MGMT
/*---------------------------------------------------------------------------
 * Type can be used to parse different command payload types
---------------------------------------------------------------------------*/
enum ds_qmap_cmd_ul_data_sent_conf_type_enum
{
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_UL_BURST            = 0x1,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_UL_BURST_LAST_BYTE  = DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_UL_BURST,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_UL_BURST_THRESHOLD  = 0x2,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_RESERVED_1          = 0x4,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_RESERVED_2          = 0x8,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_RESERVED_3          = 0x10,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_RESERVED_4          = 0x20,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_RESERVED_5          = 0x40,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_TYPE_ACK_MAX             = 0x80
};

/*---------------------------------------------------------------------------
 * Version of the commmand used for communication 
---------------------------------------------------------------------------*/
enum ds_qmap_cmd_ul_data_sent_conf_version_enum
{
  DS_QMAP_CMD_UL_DATA_SENT_CONF_VERSION            = 1,
  DS_QMAP_CMD_UL_DATA_SENT_CONF_THRESHOLD_VERSION  = 2,
};
#endif/* FEATURE_DATA_FLOW_MGMT*/ 

typedef PACKED struct PACKED_POST
{
  uint8               cd_pad;
  uint8               mux_id;
  uint16              length;
} ds_qmap_hdr_type;

typedef PACKED struct PACKED_POST
{
  uint8               cmd;         // ds_qmap_cmd_enum
  uint8               cmd_type;    // ds_qmap_cmd_type_enum
  uint16              reserved;
  uint32              tx_id;
} ds_qmap_cmd_hdr_type;

typedef PACKED struct PACKED_POST
{
  ds_qmap_hdr_type       hdr;
  ds_qmap_cmd_hdr_type   cmd_hdr;
  uint8                  reserved;
  uint8                  ip_family;
  uint16                 seq_num;
  uint32                 qos_id;
} ds_qmap_cmd_flow_ctrl_type;


#ifdef FEATURE_DATA_FLOW_MGMT
typedef PACKED struct PACKED_POST
{
  ds_qmap_hdr_type             hdr;
  ds_qmap_cmd_hdr_type         cmd_hdr;
  uint8                        conf_version; // ds_qmap_cmd_ul_data_sent_conf_version_enum
  uint8                        conf_type;    // ds_qmap_cmd_ul_data_sent_conf_type_enum
  uint16                       seq_num;
  uint8                        reserved;
  uint16                       reserved2;
  uint8                        bearer_id;
} ds_qmap_cmd_ul_data_sent_conf_type;  
#endif/* FEATURE_DATA_FLOW_MGMT*/ 

#define DS_QMAP_IS_FC_CMD(cmd) \
    ((cmd) == DS_QMAP_CMD_FLOW_DISABLE || (cmd) == DS_QMAP_CMD_FLOW_ENABLE)

#define DS_QMAP_FC_CMD_PAYLOAD_SIZE  (8)

/*---------------------------------------------------------------------------
  Data type used to qmap flow functions
---------------------------------------------------------------------------*/
typedef struct
{
  q_link_type         link; 
  rmnet_xport_type  * xport_info;
  uint16              cmd_size;
  void              * cmd_ptr;
  uint32              tx_id;
  uint8               num_retx;
  sio_stream_id_type  stream_id;
  uint32              qmap_cmd_id;
  timer_type          qmap_cmd_timer;
  uint32              timeout;
} ds_qmap_cmd_info_type;

/*===========================================================================

                           EXTERNAL FUNCTIONS 

===========================================================================*/

/*===========================================================================
  FUNCTION DS_QMAP_SEND_CMD()

  DESCRIPTION
    This function sends a QMAP command.

  PARAMETERS
    port_id:               SIO port id
    mux_id:                QMAP mux_id
    cmd_size:              QMAP command size
    cmd_ptr:               QMAP command pointer
    flow_info_ptr:         Ptr to the retransmitted pkt in queue/
                           NULL if not a retransmitted pkt
    

  RETURN VALUE
    TRUE/FALSE 

  DEPENDENCIES
    SIO port must be opened and QMAP data format is enabled.

  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_qmap_send_cmd
(
  rmnet_xport_type       * xport_info,
  uint16                   cmd_size,
  void                   * cmd_ptr,
  ds_qmap_cmd_info_type  * cmd_info_ptr
);

/*===========================================================================
FUNCTION   DS_QMAP_CMD_CB()

DESCRIPTION
  This callback function is called when a response for a
   QMAP command is received on the UL

PARAMETERS:
    stream_id :  Stream ID on which response was received
    cmd_name  :  Command name as indicated in the QMAP control command spec
    cmd_type  :  Command type as indicated in the QMAP control command spec
    tx_id     :  QMAP command Transaction ID
    dsm_ptr   :  Pointer to the DSM item which contains the command payload. 
                 The QMAP control header is stripped out of the dsm item and passed
                 to DS/client. DS is expected to free this item

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_qmap_cmd_cb
(
  sio_stream_id_type              stream_id,
  uint8                           cmd_name,   
  uint8                           cmd_type,
  uint32                          tx_id,
  dsm_item_type                 * dsm_ptr
);

/*===========================================================================
  FUNCTION DS_QMAP_BUILD_FLOW_ENABLE_CMD()

  DESCRIPTION
    This function builds a QMAP flow enable command.

  PARAMETERS
    qmap_fc_cmd:   QMAP command ptr
    ip_family:     IP family
    seq_num:       sequence number
    qos_id:        qos_id 

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void ds_qmap_build_flow_enable_cmd
(
  ds_qmap_cmd_flow_ctrl_type   * qmap_fc_cmd,
  uint8                          ip_family,
  uint16                         seq_num,
  uint32                         qos_id
);

/*===========================================================================
  FUNCTION DS_QMAP_BUILD_FLOW_DISABLE_CMD()

  DESCRIPTION
    This function builds a QMAP flow disable command.

  PARAMETERS
    qmap_fc_cmd:   QMAP command ptr
    ip_family:     IP family
    seq_num:       sequence number
    qos_id:        qos_id 

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void ds_qmap_build_flow_disable_cmd
(
  ds_qmap_cmd_flow_ctrl_type   * qmap_fc_cmd,
  uint8                          ip_family,
  uint16                         seq_num,
  uint32                         qos_id
);

/*===========================================================================
  FUNCTION DS_QMAP_INIT()

  DESCRIPTION
    This function initializes the QMAP related functions.
                                                                                        
  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    Should be called once at powerup

  SIDE EFFECTS
    None
===========================================================================*/
void ds_qmap_init
(
  void
);

/*===========================================================================
FUNCTION DS_QMAP_CLEAN_CMD

DESCRIPTION
  Clean all the pending commands for a specific call

PARAMETERS
  xport_info: call xport_info

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_qmap_clean_cmd
(
  rmnet_xport_type * xport_info
);

#endif /* _DS_QMAP_H */

