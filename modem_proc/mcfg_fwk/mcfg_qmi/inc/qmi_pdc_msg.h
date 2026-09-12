#ifndef QMI_PDC_MSG_H
#define QMI_PDC_MSG_H
/*==============================================================================

    Q M I   P E R S I S T E N T   D E V I C E   C O N F I G   M E S S A G E
 
GENERAL DESCRIPTION
  Header for the Persistent Device Configuration (PDC) QMI service message
  library

Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_qmi/inc/qmi_pdc_msg.h#1 $ 
$DateTime: 2021/04/08 00:04:19 $ 
$Author: pwbldsvc $ 
$Change: 30003610 $ 

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
04/30/10 mj   Initial creation

==============================================================================*/

#include "persistent_device_configuration_v01.h"

#include "mcfg_common.h"
#include "mcfg_client.h"
#include "mcfg_svc.h"

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/
/* Macro to validate config type */
#define IS_VALID_PDC_CONFIG_TYPE(type) ((type) == PDC_CONFIG_TYPE_MODEM_PLATFORM_V01 || \
                                        (type) == PDC_CONFIG_TYPE_MODEM_SW_V01)

/* Macro to validate config id len */
#define IS_VALID_PDC_CONFIG_ID_LEN(len) ((len) != 0 && (len) <= PDC_CONFIG_ID_SIZE_MAX_V01)

/* Macro to validate subscription ID */
#define IS_VALID_MCFG_SUBSCRIPTION_ID(sub) (MCFG_SUB_ID_FIRST <= (sub) && (sub) < MCFG_NUM_OF_SUB_ID)

/* Macro to validate Slot ID */
#define IS_VALID_MCFG_SLOT_ID(slot) (MCFG_SLOT_INDEX_FIRST <= (slot) && (slot) < MCFG_NUM_OF_UIM_SLOT)

/* Macro to validate config storage type */
#define IS_VALID_PDC_STORAGE_TYPE(fs) ((fs) == PDC_STORAGE_LOCAL_V01 || \
                                       (fs) == PDC_STORAGE_REMOTE_V01)          

/*==============================================================================
  Typedefs
==============================================================================*/

/*==============================================================================
  Variables
==============================================================================*/


/*============================================================================== 
 
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
 
==============================================================================*/

/*===========================================================================

  FUNCTION pdc_qmi_compose_config_change_ind

  DESCRIPTION
    This function maps the QMI config type to MCFG MEM enum value

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    qmi_ind          [out] QMI IND
    qmi_ind_size     [out] Size of composed QMI IND
    mcfg_config_type [in]  type of config file
    mcfg_config_id   [in]  ID of new config file

  RETURN VALUE
    QMI_CSI_NO_ERR if success, otherwise error

  SIDE EFFECTS
    None

===========================================================================*/
qmi_csi_error pdc_qmi_compose_config_change_ind
(
  pdc_config_change_ind_msg_v01    *qmi_ind,
  uint32                           *qmi_ind_size,
  mcfg_config_type_e_type           config_type,
  mcfg_config_id_s_type            *config_id
);

/*===========================================================================

  FUNCTION pdc_qmi_compose_refresh_event_ind

  DESCRIPTION
    This function maps the QMI config type to MCFG MEM enum value

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    qmi_ind          [out] QMI IND
    qmi_ind_size     [out] Size of composed QMI IND
    mcfg_config_type [in]  type of config file
    mcfg_config_id   [in]  ID of new config file

  RETURN VALUE
    QMI_CSI_NO_ERR if success, otherwise error

  SIDE EFFECTS
    None

===========================================================================*/
qmi_csi_error pdc_qmi_compose_refresh_event_ind
(
  pdc_refresh_ind_msg_v01          *qmi_ind,
  uint32                           *qmi_ind_size,
  mcfg_client_event_e_type         event_reg_type,
  mcfg_refresh_index_type          slot_index,
  mcfg_refresh_index_type          sub_index
);

/*===========================================================================

  FUNCTION pdc_send_resp

  DESCRIPTION
    This function sends QMI_PDC response after a req is processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    None 
 
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void pdc_send_resp
( 
  qmi_req_handle     req_handle,
  unsigned int       msg_id,
  void               *response,
  unsigned int       size
);

/*===========================================================================

  FUNCTION pdc_send_ind

  DESCRIPTION
    This function sends QMI_PDC indications after a req/response is processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    None 
 
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void pdc_send_ind
( 
  qmi_client_handle   qmi_client_handle,
  uint16_t            message_id,
  void                *p_src,
  uint32_t            src_len
);

#endif /* QMI_PDC_MSG_H */

