
/*!
  @file
  ds_umts_bearer_flow_manager.h

  @brief
  Manages Flow specific information related to bearer. provides interface to 
  the bearer and applications to extract and configure EPS flow information.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/umtsps/inc/ds_umts_bearer_flow_manager.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/11   ttv     Added support for get session params.
02/04/10   vd      Changed function naming format.
11/07/09   vd      Initial version.
===========================================================================*/

#ifndef DSUMTSBEARERFLOWMANAGER_H
#define DSUMTSBEARERFLOWMANAGER_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#if defined(FEATURE_DATA_WCDMA_PS) || defined(FEATURE_GSM_GPRS)

#include "ds_3gpp_flow_manager.h"
#include "ds_3gpp_bearer_flow_manager.h"
#include "ds_3gpp_qos.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*-------------------------------------------------------------------------
Cookie to ensure that dynamic memory allocated is valid
---------------------------------------------------------------------------*/
#define UMTS_BEARER_FLOW_MANAGER_VALID_COOKIE  0x1bef1bef

#define DS_UMTS_BEARER_FLOW_BIT_RATE_10GBPS      10000000
#define DS_UMTS_BEARER_FLOW_BIT_RATE_256MBS      256000
#define DS_UMTS_BEARER_FLOW_BIT_RATE_8640_KBPS   8640

/*-----------------------------------------------------------------------------
  This structure contains UMTS specific info for the Bearer Flow manager.
-----------------------------------------------------------------------------*/
typedef struct
{ 

    /*! Cookie to ensure dyn mem is valid */
  uint32                           cookie;  


  ds_umts_umts_qos_params_type app_sps_min_qos; /* App requested Min QOS,  */
                                                /* Converted to Min QoS    */ 
                                                /* (to UMTS)               */
  
  ds_umts_umts_qos_params_type app_sps_req_qos; /* App req Requested QoS,  */
                                                /* Converted to Requested  */
                                                /* QOS (to UMTS)           */

  ds_umts_umts_qos_params_type umts_netw_alloc_qos;  /* Network granted QoS */
                                                /* Converted to Granted */
                                                /* QoS (to UMTS)*/

  ds_umts_gprs_qos_params_type gprs_app_min_qos; /* App requested Min QOS,  */
                                                /* Converted to Min QoS    */ 
                                                /* (to UMTS)               */
  
  ds_umts_gprs_qos_params_type gprs_app_req_qos; /* App req Requested QoS,  */
                                                /* Converted to Requested  */
                                                /* QOS (to UMTS)           */

  ds_umts_gprs_qos_params_type gprs_netw_alloc_qos;  /* Network granted QoS */
                                                /* Converted to Granted */
                                                /* QoS (to UMTS)*/

  qos_T                 req_qos;               /* requested QOS NW format  */
  qos_T                 min_qos;               /* req Min QOS in NW format */

  uint32                       src_stat_desc_req;
                                                /*Source Statistics Descriptor*/
                                                /* for App req Req QOS */

  uint32                       src_stat_desc_min;
                                                /*Source Statistics Descriptor*/
                                                /* for App req Min QOS */

  uint32                       src_stat_desc_netw_alloc;
                                                /*Source Statistics Descriptor*/
                                                /* for N/W allocated QOS*/
}ds_umts_bearer_flow_manager_dynamic_s;


typedef struct
{
 ds_umts_bearer_flow_manager_dynamic_s* ds_umts_bearer_flow_manager_dynamic_p;    
} ds_umts_bearer_flow_manager_s;

/*-----------------------------------------------------------------------------
  UMTS bearer Flow manager table exposed to other files through header
-----------------------------------------------------------------------------*/
extern ds_umts_bearer_flow_manager_s ds_umts_bflow_manager_tbl[DS_3GPP_MAX_BEARER_CONTEXT];

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/
/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_INIT

DESCRIPTION
  Register UMTS specific functions

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None.

===========================================================================*/
void ds_umts_bearer_flow_manager_init( void );

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_CONVERT_APP_DATA_RATE_TO_NW
DESCRIPTION
  Converts the APP passed data rate to UMTS NW data rate.

PARAMETERS
app_qos         -  Application qos to be converted
nw_qos          -  UMTS network qos
DEPENDENCIES
  None.

RETURN VALUE
boolean         - return DS3G_SUCCESS or DS3G_FAILURE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_umts_bearer_flow_manager_convert_app_data_rate_to_nw
(
   qos_spec_type                *app_qos,
   void                         *nw_qos,
   void                         *extended_nw_qos_ptr,
   sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_CONVERT_NW_DATA_RATE_TO_APP
DESCRIPTION
  Converts the UMTS NW passed data rate to App data rate.

PARAMETERS
nw_qos          -  UMTS network qos to be converted
app_qos         -  Application qos structure
DEPENDENCIES
  None.

RETURN VALUE
boolean         - return DS3G_SUCCESS or DS3G_FAILURE
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_umts_bearer_flow_manager_convert_nw_data_rate_to_app
(
  ds_bearer_flow_manager_s *bflow_manager_p,
  void                     *nw_qos,
  qos_spec_type            *app_qos,
  ds_3gpp_internal_cause_code_enum_type *cause_code_p
);

/*===========================================================================
FUNCTION DSUMTSBEARERFLOWMANAGER_CONVERTAPPBITRATETONW

DESCRIPTION
  Converts the NW passed bit rate to uint32.

PARAMETERS
  input         -  network octet to be converted

DEPENDENCIES
  None.

RETURN VALUE
uint32          - return converted bit rate
  
SIDE EFFECTS
  None.

===========================================================================*/
void dsEpsBearerFlowmanager_convertAppBitRateToNw
(
  uint32 app_rate,
  byte   *nw_rate,
  byte   *nw_rate_ex
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_FLOW_VALIDATION

DESCRIPTION
  Verify the app did not pass invalid flow parameters.

PARAMETERS
 input_qos           - Pointer to App passed Qos.

DEPENDENCIES
  None.

RETURN VALUE
 boolean              - Indicates true or false.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_umts_bearer_flow_manager_flow_validation
( 
  qos_spec_type                *input_qos,
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_GET_UMTS_NETW_ALLOC_QOS

DESCRIPTION
  Returns the pointer to umts_netw_alloc_qos from ds_umts_bflow_manager_tbl
  based on the call instance value.

PARAMETERS
 call_instance                - index of ds_umts_bflow_manager_tbl.

DEPENDENCIES
  None.

RETURN VALUE
 ds_umts_umts_qos_params_type - pointer to umts_netw_alloc_qos.
  
SIDE EFFECTS
  None.

===========================================================================*/
ds_umts_umts_qos_params_type*
ds_umts_bearer_flow_manager_get_umts_netw_alloc_qos
(
  uint32 call_instance
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_GET_GPRS_NETW_ALLOC_QOS

DESCRIPTION
  Returns the pointer to gprs_netw_alloc_qos from ds_umts_bflow_manager_tbl
  based on the call instance value.

PARAMETERS
 call_instance                - index of ds_umts_bflow_manager_tbl.

DEPENDENCIES
  None.

RETURN VALUE
 ds_umts_umts_qos_params_type - pointer to gprs_netw_alloc_qos.
  
SIDE EFFECTS
  None.

===========================================================================*/
ds_umts_gprs_qos_params_type*
ds_umts_bearer_flow_manager_get_gprs_netw_alloc_qos
(
  uint32 call_instance
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_UPDATE_FLOW_ON_RESUME

DESCRIPTION
  Updates flow from L->W handoff with W params

PARAMETERS
  bflow_manager_p   - bearer flow manager ptr.

DEPENDENCIES
  None.

RETURN VALUE
  boolean - True if success, false otherwise.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_umts_bearer_flow_manager_update_flow_on_resume
(
    ds_bearer_context_s* bearer_context_p
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_STORE_NW_DATA_RATE
DESCRIPTION
  Converts the APP passed data rate to UMTS data rate and store in bearer
  flow manager
  orig_call_hdlr converts the UMTS values to NW format.
  In UMTS, except data rates and guaranteed data rates, other parameters
  are common across UL and DL. 

PARAMETERS
nw_qos          -  NW qos to be converted
bflow_manager_p -  UMTS bearer flow manager 

DEPENDENCIES
  None.

RETURN VALUE
boolean         - return DS3G_SUCCESS or DS3G_FAILURE
  
SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_umts_bearer_flow_manager_store_nw_data_rate
(
   const cm_gw_ps_call_info_s_type 	*gw_ps_call_p,	
   void            *bflow_manager_p
);

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_UPDATE_FILTER_ID

DESCRIPTION
  Assign unique filter id to filters if none are filled
  in by app.

PARAMETERS
 nw_tft - tft spec

DEPENDENCIES
  None.

RETURN VALUE
 boolean              - Indicates true or false.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_umts_bearer_flow_manager_update_filter_id
( 
  tft_type_T                    *nw_tft,
  sys_modem_as_id_e_type         subs_id     
);

/*===========================================================================
FUNCTION ds_umts_bearer_flow_manager_allocate_umts_bearer_flow_manager

DESCRIPTION
  This function allocates dynamic memory for the index in the umts bearer flow manager table

PARAMETERS
int loop_index
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE 
  FALSE

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_umts_bearer_flow_manager_allocate_umts_bearer_flow_manager
(
   int loop_index
);

/*===========================================================================
FUNCTION ds_umts_bearer_flow_manager_allocate_umts_bearer_flow_manager

DESCRIPTION
  This function free's the umts bearer flow manager

PARAMETERS
ds_umts_bearer_flow_manager_s *umts_bearer_flow_manager_p 
    
DEPENDENCIES
  

RETURN VALUE
  TRUE 
  FALSE

SIDE EFFECTS
  None

===========================================================================*/


boolean ds_umts_bearer_flow_manager_free_umts_bearer_flow_manager
(
   int loop_index
);
/*===========================================================================
FUNCTION ds_umts_bearer_flow_manager_validate

DESCRIPTION
  This function checks if the umts bearer flow manager has been allocated dynamic memory

PARAMETERS
  bflow_manager_p  -bflow_manager_p to be validated
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Non null
  FALSE- Otherwise

SIDE EFFECTS
  None

===========================================================================*/


inline boolean ds_umts_bearer_flow_manager_validate
(
  ds_umts_bearer_flow_manager_s *bflow_manager_p
);

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_MANAGER_CONVERT_APP_FLOW_TO_NW_FLOW

DESCRIPTION
  This function converts app data rate to corresponding network data rate

PARAMETERS
  app_rate         - Rate provided by the application
  nw_rate          - nw rate corresponding to octet 8 or 9 and 12 or 13
  nw_rate_ex       - Extended nw rate corresponding to octet 15 through 18
  nw_rate_ex2      - Extended 2 nw rate corresponding to octet 19 through 22
  nw_ext_qos_rate  - Ext QoS nw rate specified in the new extended QoS
                     Structure
  multiplier_unit  - unit for mbr or gbr
  subs_id          - Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_umts_bearer_flow_manager_convert_app_flow_to_nw_flow
(
  unsigned long                       app_rate,
  byte                               *nw_rate,
  byte                               *nw_rate_ex,
  byte                               *nw_rate_ex2,
  uint16                             *nw_ext_qos_rate,
  byte                               *multiplier_unit,
  sys_modem_as_id_e_type              subs_id
);
#endif /* FEATURE_LAPP */

/*===========================================================================
FUNCTION DS_UMTS_BEARER_FLOW_CONVERT_DATA_RATE_TO_R7_EXT2_ENCODE_VAL

DESCRIPTION
  This function converts the given App data rate from an unsigned integer
  to a one byte value as specified in TS24.008 (10.5.6.5)

  NOTE: The return value is a 'byte' so that it can be used as-is to
  encode the QOS parameters in the message.

PARAMETERS
  data_rate : data rate in kbps

DEPENDENCIES
  None.

RETURN VALUE
  one byte value indicating the data rate as per encoding rules

SIDE EFFECTS
  None.

===========================================================================*/
uint8 ds_umts_bearer_flow_convert_data_rate_to_r7_ext2_encode_val
(
  uint32  data_rate
);

#endif /* if defined(FEATURE_DATA_WCDMA_PS) || defined(FEATURE_GSM_GPRS) */
#endif /* DSUMTSBEARERFLOWMANAGER_H */

