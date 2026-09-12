/*!
  @file
  ipa_dl_rlc.h

  @brief
  This file contain declarations of IPA RLC DL data path processing

*/
/*===========================================================================

  Copyright (c) 2013-14 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$DateTime: 2019/07/05 03:48:36 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/06/17   pgm      HFN resync support
02/16/17   pgm      New version
08/15/16   pgm      Support for ciph cfg
03/24/16   pgm      Initial version
===========================================================================*/

#ifndef IPA_DL_OPT_LTE_API_H
#define IPA_DL_OPT_LTE_API_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include "ipa_api.h"


/*!===========================================================================
                              MACROS
===========================================================================*/

/*!Max no of iovecs per pkt expected = MAX_RLC_IOVEC(5) + METADATA_IOVEC(1)*/
#define IPA_DL_OPT_LTE_IOVEC_ARR_SZ                              (6)

/*! No op Tag value passed in tag CB to L2. This should be ignored and is an
    indication of new iovec space available.threshold check() needs to invoked
    to query new space.*/
#define IPA_DL_OPT_LTE_TAG_NO_OP_VAL                             (0xDEADDEFE)

/*!===========================================================================
                            Interface Data structures 
===========================================================================*/

/*! @brief IPA-RLC Iovec interface structure*/
typedef struct
{
  /*!pkt size in this IOVEC buffer*/
  uint32 size : 16;
  /*!Reserved fields*/
  uint32 rsvd : 16;
  /*!Phy addr of IOVEC buffer*/
  uint32 addr;
} ipa_dl_opt_lte_iovec_s;

/*!IPA-RLC iovec union structure*/
typedef union
{
  /*!iovec fields*/
  ipa_dl_opt_lte_iovec_s fields;
  /*!iovec 64b value*/
  uint64 val;
} ipa_dl_opt_lte_iovec_type;

/*! @brief enum: bearer configuration events */
typedef enum
{
  /*Bearer reg/dereg evts*/
  IPA_DL_OPT_LTE_BEARER_CFG_EVT_REG_FLOW_ENABLED = 0,
  IPA_DL_OPT_LTE_BEARER_CFG_EVT_DEREG_FLOW_DISABLED = 1,

  /*Bearer flow enable/disable(or suspend) evts*/
  IPA_DL_OPT_LTE_BEARER_CFG_EVT_FLOW_ENABLED = 2,
  IPA_DL_OPT_LTE_BEARER_CFG_EVT_FLOW_DISABLED = 3,
  IPA_DL_OPT_LTE_BEARER_CFG_EVT_MAX
} ipa_dl_opt_lte_bearer_cfg_evt_e;

/*!===========================================================================
                            CB Fn ptr prototypes 
===========================================================================*/

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_IOVEC_WRITE_FPTR_TYPE

===========================================================================*/
/*!
   Function prototype of iovec write function that IPA registers with
   RLC. It is invoked by RLC to submit IOVECs for IPA processing.
   
   @param
    iovec_arr, Array of IOVECs for the current IP pkt
    num_iovec, number of IOVECs in the array
    total_pkt_len, Complete IP pkt length.
  
   @return
    None
  
*/ 
/*=========================================================================*/
typedef uint32 (*ipa_dl_opt_lte_iovec_write_fptr_type)
(
  /*!Array of IOVECs for a IP pkt*/
  ipa_dl_opt_lte_iovec_type iovec_arr[], 
  /*! Number of IOVECs*/
  uint32 num_iovec,
  /*! Total packet len */
  uint32 total_pkt_len
);

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_TAG_CMPLT_IND_CB_TYPE

===========================================================================*/
/*! @brief Function prototype of CB fn to be triggered by IPA to indicate
    completion of pkt processing.
*/
/*=========================================================================*/
typedef void (*ipa_dl_opt_lte_tag_cmplt_ind_cb_type)
(
  /*! Unique tag identifier for the stream of pkts processed*/
  uint32 tag
);

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_BEARER_CFG_IND_CFG_TYPE

===========================================================================*/
/*! @brief Function prototype of CB fn to be triggered by IPA to indicate 
    bearer configuration change.
*/
/*=========================================================================*/
typedef void (*ipa_dl_opt_lte_bearer_cfg_ind_cb_type)
(
  /*! Bearer change event type*/
  ipa_dl_opt_lte_bearer_cfg_evt_e bearer_evt,
  /*! EPS bearer id*/
  uint8 eps_bearer_id,
  /*! Subscription id for this bearer*/
  uint32 subs_id,
  /*! bearer specific user data*/
  uint32 usr_data,
  /*! Write IOVEC function pointer */
  ipa_dl_opt_lte_iovec_write_fptr_type iovec_write_fptr
);

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_CTRL_TAG_IND_CB_TYPE

===========================================================================*/
/*! @brief Function prototype of CB fn to be triggered by IPA to indicate
    completion of control tag processing - Ex - HFN ciph cnt tag commit
*/
/*=========================================================================*/
typedef void (*ipa_dl_opt_lte_ctrl_tag_ind_cb_type)
(
  /*! Unique tag identifier for the stream of pkts processed*/
  uint32 tag,
  /*! sub id */
  uint8 subs_id
);


/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_HFN_ERR_IND_CB_TYPE

===========================================================================*/
/*! @brief Function prototype of CB fn to be triggered by IPA to indicate
    decipher errors 
*/
/*=========================================================================*/
typedef void (*ipa_dl_opt_lte_hfn_err_ind_cb_type)
(
  /*!EPS bearer id*/
  uint8 eps_bearer_id,
  /*!subscription id*/
  uint8 subs_id
);


/*!CB function reg structure*/
typedef struct
{
  /*! Tag indication CB function*/
  ipa_dl_opt_lte_tag_cmplt_ind_cb_type tag_ind_cb_fn;
  /*! Bearer config indication CB function*/
  ipa_dl_opt_lte_bearer_cfg_ind_cb_type bearer_cfg_cb_fn;
  /*! Ctrl indication CB function*/
  ipa_dl_opt_lte_ctrl_tag_ind_cb_type ctrl_tag_ind_cb_fn;
  /*! HFN err indication CB function*/
  ipa_dl_opt_lte_hfn_err_ind_cb_type hfn_err_ind_cb_fn;
} ipa_dl_opt_lte_register_cb_type_s;


/*!===========================================================================
                            Interface APIS
===========================================================================*/

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_REQUEST_TAG

===========================================================================*/
/*!
   This function triggeres a request to IPA for iovec processing and return a
   tag for completion indication. Expectation is that this API will be called
   for every TTI.
   
   @param
   tag - Tag value to mark the end of current processing of pkts
   num_total_pkts - Total num pkts programmed
   num_total_bytes - Total num bytes programmed
   @return max num commit iovec tags
*/ 
/*=========================================================================*/
uint32 ipa_dl_opt_lte_req_tag
(
  /*! Tag value to mark the end of current processing of pkts*/
  uint32 tag,
  /*! Total num pkts programmed*/
  uint32 num_total_pkts,
  /*! Total num bytes programmed*/
  uint32 num_total_bytes
);
	
/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_CTL_REQUEST_TAG

===========================================================================*/
/*!
   This function triggeres a control tag to be processed to indicate 
   completion of control event.
   
   @param
   tag - Tag value to mark the end of current processing of pkts
   @return max num commit iovec tags
*/ 
/*=========================================================================*/
uint32 ipa_dl_opt_lte_ctl_req_tag
(
  /*! Tag value to mark the commit of ctrl tags*/
  uint32 tag
);

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_THRESHOLD_CHECK

===========================================================================*/
/*!
   This function checks if free space is available for num_iovecs.
   
   @param
   num_iovec - number of iovecs for which space needs to be checked.
   @return 
    Always returns the number of free ioves that can be supported currently
	for processing
*/ 
/*=========================================================================*/
uint32 ipa_dl_opt_lte_threshold_check
(
  uint32 num_iovec
);


/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_REGISTER_EVENT_IND_CB

===========================================================================*/
/*!
   This function registers a set of callback functions with IPA driver to indicate
   various events
   1. tag completion for the IOVECs programmed.
   2. bearer configuration change events to RLC.
   
   @param
   reg_cb - set of fn ptrs
 
   @return 
*/ 
/*=========================================================================*/
void ipa_dl_opt_lte_register_event_ind_cb
(
  ipa_dl_opt_lte_register_cb_type_s      *reg_cb
);


/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LOOPBACK_REGISTER_EVENT_IND_CB

===========================================================================*/

void ipa_dl_opt_loopback_register_event_ind_cb
(
  ipa_dl_opt_lte_register_cb_type_s      *reg_cb
);


/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_PROC_BEARER_CFG_CHANGE

===========================================================================*/
/*!
   This function is invoked by RLC for
    - notifying bearer changes
    - initing new cipher info.
    - cipher count changes.
    
   @param
   ciph_param, Cipher algo, key idx, bearer id (0..31)
   ciph_count, per pkt Cipher count if algo is not NONE
   usr_data, bearer specific user data 
  
   @return
  
*/ 
/*=========================================================================*/
uint32 ipa_dl_opt_lte_proc_bearer_cfg_change
(
  /*! Cipher algo, key idx, bearer id 0..31. (refer ipa_api.h)*/
  ipa_wan_cipher_param_u ciph_param, 
  /*! per pkt Cipher count if algo is not NONE*/
  uint32 ciph_count,
  /*! bearer specific user data*/
  uint32 usr_data
);

/*===========================================================================

  FUNCTION:  IPA_DL_OPT_LTE_PROC_RB_REGISTERED_EVENT

===========================================================================*/
/*!
   This function is invoked by L2 to indicate a new rb that was registered.
    
   @param
   EPS bearer id - eps_bearer_id
   subs id       - subscription id
 
   @return
  
*/ 
/*=========================================================================*/
void ipa_dl_opt_lte_proc_rb_registered_event
(
  /*! EPS bearer id*/
  uint8 eps_bearer_id,
  /*! Subscription id for this bearer*/
  uint32 subs_id
);

#endif /* IPA_DL_OPT_LTE_API_H */
