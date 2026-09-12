/*!
  @file
  ds_3gpp_pco_epco_hdlr.h

  @brief
  This software unit handles the PCO/EPCO handling.

*/

/*===========================================================================

  Copyright (c) 2008 - 2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_pco_epco_hdlr.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/04/17   ag      Initial version.
===========================================================================*/

#ifndef DS_3GPP_PCO_EPCO_HDLR_H
#define DS_3GPP_PCO_EPCO_HDLR_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "cm_gw.h"
#include "cm.h"
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/* Max number of protocol ID's */ 
/* Increament this num everytime a new protocol is added to EPCO */

#define MAX_NUM_EPCO_PROTOCOL_ID 20

#define PROTOCOL_ID_LEN 2 


/* Structure for individual protocol information */

/** @brief Individual protocol information.
*/
typedef struct
{
  byte protocol_id[PROTOCOL_ID_LEN];
  /**< Protocol IDs. */
  byte protocol_len;
  /**< Protocol ID content length. */
  byte protocol_contents[MAX_PROTOCOL_ID_CONTENT];
  /**< Protocol content. */
} ds_3gpp_protocol_id_s;
 
/*~ FIELD protocol_id_s.protocol_contents VARRAY MAX_PROTOCOL_ID_CONTENT
    LENGTH protocol_id_s.protocol_len */
 
/** @brief Protocol configuration options.
*/
typedef struct
{
  boolean                 valid;
  /**< Whether protocol configuration options are present. */
  byte                    config_protocol;
  /**< Configuration protocol. */
  word                    num_protocol_ids;
  /**< Number of protocol IDs. */
  
  ds_3gpp_protocol_id_s   *protocol_info_p[MAX_NUM_EPCO_PROTOCOL_ID];

  /**< Protocol information. */
} ds_3gpp_protocol_cfg_options_s;

#ifdef FEATURE_ePCO_UNSUPP

#define MIN_EPCO_CONTENT_LEN 1
#define MAX_EPCO_CONTENT_LEN 65535

typedef struct ext_protocol_cfg_options
{
  boolean         valid; /**valid IE or not*/
  word            len;  /**length of epco contents*/
  byte            *content_ptr;/**epco contents.*/
  void            *dsm_ptr;
}ext_protocol_cfg_options_T;
#endif /* FEATURE_ePCO_UNSUPP */


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
  FUNCTION DS_3GPP_PCO_EPCO_HDLR_CONVERT_LTE_NAS_EPCO_TO_DS
  
  DESCRIPTION
    This function converts the EPCO from NAS to DS format so that it can be
    decoded by the current PCO decode functions.
  
  PARAMETERS
    [in]   ext_protocol_config_options_p
    [out]  proto_config_option_p
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    boolean
  
  SIDE EFFECTS
    None.
  
  ===========================================================================*/
  
  boolean ds_3gpp_pco_epco_hdlr_convert_lte_nas_epco_to_ds
  (
    ext_protocol_cfg_options_T      *ext_protocol_config_options_p,
    ds_3gpp_protocol_cfg_options_s  *proto_config_option_p
  );
  
  /*===========================================================================
  FUNCTION DS_3GPP_PCO_EPCO_HDLR_CONVERT_DS_EPCO_TO_LTE_NAS
  
  DESCRIPTION
    This function converts the EPCO from DS to NAS format before it is sent
    out to NAS
  
  PARAMETERS
    [in]   proto_config_option_p
    [out]  ext_protocol_config_options_p
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    boolean
  
  SIDE EFFECTS
    None.
  
  ===========================================================================*/
  
  boolean ds_3gpp_pco_epco_hdlr_convert_ds_epco_to_lte_nas
  (
    ds_3gpp_protocol_cfg_options_s  *proto_config_option_p,
    ext_protocol_cfg_options_T      *ext_protocol_config_options_p
  );
  
  /*===========================================================================
  FUNCTION  DS_3GPP_PCO_EPCO_HDLR_CONVERT_NAS_PCO_TO_DS
  
  DESCRIPTION
    This function converts the PCO from NAS format to DS format before it is decoded.
  
  PARAMETERS
    [in]   nas_proto_config_option_p
    [out]  ds_proto_config_option_p
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    boolean
  
  SIDE EFFECTS
    None.
  
  ===========================================================================*/
  
  boolean ds_3gpp_pco_epco_hdlr_convert_nas_pco_to_ds
  (
    protocol_cfg_options_T          *nas_proto_config_option_p,
    ds_3gpp_protocol_cfg_options_s  *ds_proto_config_option_p
  );
  
  /*===========================================================================
  FUNCTION  DS_3GPP_PCO_EPCO_HDLR_CONVERT_DS_PCO_TO_NAS
  
  DESCRIPTION
    This function converts the PCO from NAS format to DS format before it is decoded.
  
  PARAMETERS
    [in]   ds_proto_config_option_p
    [out]  nas_proto_config_option_p
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    boolean
  
  SIDE EFFECTS
    None.
  
  ===========================================================================*/
  
  boolean ds_3gpp_pco_epco_hdlr_convert_ds_pco_to_nas
  (
    ds_3gpp_protocol_cfg_options_s  *ds_proto_config_option_p,
    protocol_cfg_options_T          *nas_proto_config_option_p
  );


/*===========================================================================
FUNCTION  DS_3GPP_PCO_EPCO_HDLR_GET_MAX_NUMBER_OF_PROTOCOL_IDS

DESCRIPTION
  This function returns the max number of protocols ID's used for PCO/EPCO

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  max number of protocol ID's

SIDE EFFECTS
  None.

===========================================================================*/
uint8 ds_3gpp_pco_epco_hdlr_get_max_number_of_protocol_ids
(
  void
);


/*===========================================================================
FUNCTION DS_3GPP_PCO_EPCO_HDLR_CONVERT_DS_EPCO_TO_UMTS_NAS

DESCRIPTION
  This function converts the EPCO from DS to 3G NAS format before it is sent
  out to 3G NAS

PARAMETERS
  [in]   proto_config_option_p
  [out]  ext_protocol_config_options_p

DEPENDENCIES
  Caller must set the valid flag as a result of the return value of this 
  function.  This function will NOT set the valid flag for them. 

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_pco_epco_hdlr_convert_ds_epco_to_umts_nas
(
  ds_3gpp_protocol_cfg_options_s  *proto_config_option_p,
  ext_protocol_cfg_options_T      *ext_protocol_config_options_p,
  sys_modem_as_id_e_type           subs_id  
);

/*===========================================================================
FUNCTION DS_3GPP_PCO_EPCO_HDLR_GET_LEN_OF_CONFIG_OPTION

DESCRIPTION
  This function converts the EPCO from 3G NAS to DS format so that it can be
  decoded by the current PCO decode functions.

PARAMETERS
  [in]   proto_config_option_p

DEPENDENCIES
  None.

RETURN VALUE
  lenght - lenght of the contents of proto_config_option_p in bytes

SIDE EFFECTS
  None.

===========================================================================*/
uint16 ds_3gpp_pco_epco_hdlr_get_len_of_config_option
(
  ds_3gpp_protocol_cfg_options_s  *proto_config_option_p,
  sys_modem_as_id_e_type           subs_id  
);
/*===========================================================================
FUNCTION DS_3GPP_PCO_EPCO_HDLR_CONVERT_UMTS_NAS_EPCO_TO_DS

DESCRIPTION
  This function converts the EPCO from 3G NAS to DS format so that it can be
  decoded by the current PCO decode functions.

PARAMETERS
  [in]   ext_protocol_config_options_p
  [out]  proto_config_option_p

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_pco_epco_hdlr_convert_umts_nas_epco_to_ds
(
  ext_protocol_cfg_options_T      *ext_protocol_config_options_p,
  ds_3gpp_protocol_cfg_options_s  *proto_config_option_p
);

/*===========================================================================
FUNCTION DS_3GPP_PCO_EPCO_HDLR_GET_CONFIG_FROM_LTE

DESCRIPTION
  This function checks if PCO or ePCO is provided by 4G NAS and converts
  accordingly

PARAMETERS
  [in]   lte_info_p
  [out]  proto_config_option_p
  [out]  epco_enabled flag for pdn context

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if successfully getting protocol config from 4g
  FALSE - if failed to get protocol config from 4g

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_pco_epco_hdlr_get_config_from_lte
(
  cm_lte_call_info_s_type         *lte_info_p,
  ds_3gpp_protocol_cfg_options_s  *proto_config_option_p,
  boolean                         *epco_enabled_flag_p
);

/*===========================================================================
FUNCTION DS_3GPP_PCO_EPCO_HDLR_GET_CONFIG_FROM_UMTS

DESCRIPTION
  This function checks if PCO or ePCO is provided by 3G NAS and converts
  accordingly

PARAMETERS
  [in]   gw_ps_call_info_p
  [out]  proto_config_option_p
  [out]  epco_enabled flag for pdn context
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if successfully getting protocol config from 3g
  FALSE - if failed to get protocol config from 3g

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_pco_epco_hdlr_get_config_from_umts
(
  cm_gw_ps_call_info_s_type       *gw_ps_call_info_p,
  ds_3gpp_protocol_cfg_options_s  *proto_config_option_p,
  boolean                         *epco_enabled_flag_p
);

#endif /* DS_3GPP_PCO_EPCO_HDLR_H */

