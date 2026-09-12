/*!
  @file
  lte_mac_prose_api.h

  @brief
  This file contains all the common define structures/function prototypes 
  interface between LTE-D client and other modules


*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/lte.mpss/6.6.1/api/lte_mac_prose_api.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/08/16   el      CR992330: UTC time interface and logging fix
07/13/15   el      Initial version  
===========================================================================*/

#ifndef LTE_MAC_PROSE_API_H
#define LTE_MAC_PROSE_API_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <comdef.h>
#include <msgr.h>
#include <msgr_lte.h>
#include <a2_dl_phy.h>
#include <lte_as.h>

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief defined maximal instance number per deletion request
*/
#define LTE_MAC_MAX_INSTANCE_PER_DEL_REQ                        5

/*! @brief defined maximal instance number per filter add request
*/
#define LTE_MAC_MAX_INSTANCE_PER_FILTER_ADD_REQ                 3

/*! @brief defined maximal number of mask associated with a PAC in a filter
*/
#define LTE_MAC_MAX_NUM_MASK_PER_FILTER                         2

/*! @brief defined maximal instance number per transmit status report ind
*/
#define LTE_MAC_MAX_INSTANCE_PER_TX_STATUS_RPT_IND              5

/*! @brief defined maximal number of matched filter PAC in a match report per received PAC
*/
#define LTE_MAC_MAX_NUM_FILTER_PER_MATCH_RPT                    7

/*! @brief defined pac code length in bytes which is 184 bits
*/
#define LTE_MAC_PAC_CODE_LEN                                    23

/*! @brief defined MIC key length in bytes which is 128 bits
*/
#define LTE_MAC_MIC_KEY_LEN                                     16

/*! @brief this enum defines class type which is the priority level with each PAC
*/
typedef enum
{
  LTE_MAC_PAC_CLASS_TYPE_0   =  0,    /* PAC class type 0, which is the highest priority */
  LTE_MAC_PAC_CLASS_TYPE_1,           /* PAC class type 1 */
  LTE_MAC_PAC_CLASS_TYPE_2,           /* PAC class type 2 */
  LTE_MAC_PAC_CLASS_TYPE_3,           /* PAC class type 3 */
  LTE_MAC_PAC_CLASS_TYPE_MAX          /* Total maximal number of class type  */
} lte_mac_pac_class_type_e;


/*! @brief PAC related information parameters structure, this structure will be filled
when prose client send add PAC code info to MAC */
typedef struct
{
  /*! Priority level associate with this PAC */
  lte_mac_pac_class_type_e priority;
  /*! PAC code value */
  uint8 pac[LTE_MAC_PAC_CODE_LEN];
  /*! MIC key value */
  uint8 key[LTE_MAC_MIC_KEY_LEN];
} lte_mac_pac_info_s;


/*! @brief MAC PAC add request from LTE-D to MAC, for now, due to the real user case,
it only add a APPS at a time, so only one PAC is added at one time */
typedef struct 
{
  msgr_hdr_struct_type        msg_hdr;  /*!< Message header */  
  /*! the message payload is the pac info to add */
  /*! Session Id at client side */
  uint32                      session_id;
  /*! PAC code information */
  lte_mac_pac_info_s          pac_info;
} lte_mac_pac_add_req_s;


/*! @brief MAC PAC add confirmation from MAC to LTE-D client */
typedef struct 
{
  msgr_hdr_struct_type              msg_hdr;  /*!< Message header */  
  /*! the message payload */
  /*! Echo back the session Id at client side */
  uint32                            session_id;
  /* if mac handler id is NULL, that means it is not successfully added in MAC 
  due to the maximal PAC limit */
  uint32                            mac_handler_id;
} lte_mac_pac_add_cnf_s;

/*! @brief MAC PAC delete request from LTE-D to MAC, it contains an array of mac handler id,
and it allows to delete a list of PAC with maximal number as LTE_MAC_MAX_INSTANCE_NUM_PER_DEL_REQ */
typedef struct 
{
  msgr_hdr_struct_type          msg_hdr;  /*!< Message header */  
  /*! the message payload */
  /*! number of active pac req */
  uint8                         active_num;  
  /*! Array of mac handler id that need to be deleted */
  uint32                        mac_handler_id_arr[LTE_MAC_MAX_INSTANCE_PER_DEL_REQ];
} lte_mac_pac_del_req_s;

/*! @brief filter related information parameters structure, this will be filled when send add filter request to MAC */
typedef struct
{
  /*! PAC code value */
  uint8                    pac[LTE_MAC_PAC_CODE_LEN];
  /*! number of active mask */
  uint8                    active_mask_num;  
  /*! List of pac mask */
  uint8                    mask_list_arr[LTE_MAC_MAX_NUM_MASK_PER_FILTER][LTE_MAC_PAC_CODE_LEN];
} lte_mac_filter_info_s;

/*! @brief Prose PAC filter add request from LTE-D to MAC*/
typedef struct 
{
  msgr_hdr_struct_type                msg_hdr;  /*!< Message header */  
  /*! the message payload */
  /*! Session ID at client side */
  uint32                              session_id;
  /*! number of active mask in the below array */
  uint8                               num_active_filter;  
  /*! array of pac filter info */
  lte_mac_filter_info_s               filter_arr[LTE_MAC_MAX_INSTANCE_PER_FILTER_ADD_REQ];
} lte_mac_filter_add_req_s;


/*! @brief MAC filter add confirmation msg from MAC to LTE-D, it will contains a list of hanlder id mapping info 
with max number as LTE_MAC_MAX_INSTANCE_NUM_PER_FILTER_ADD_REQ */
typedef struct 
{
  msgr_hdr_struct_type                       msg_hdr;  /*!< Message header */  
  /*! the message payload */
  /*! Echo back the Session ID at client side */
  uint32                                     session_id;
  /*! number of active id mapping info, if the number is 0, that means none is added */
  uint8                                      active_num;  
  /*! Array of handler id map info, if mac handler id is NULL, then it means that prose id is not succesffully added 
  due to MAC maximal filter number limit */
  uint32                                     mac_handler_id_arr[LTE_MAC_MAX_INSTANCE_PER_FILTER_ADD_REQ];
} lte_mac_filter_add_cnf_s;

/*! @brief MAC PAC filter delete request msg from LTE-D to MAC
*/
typedef struct 
{
  msgr_hdr_struct_type              msg_hdr;  /*!< Message header */  
  /*! the message payload is the filter del request */
  /*! number of active mac handler id in the below array */
  uint8                             active_num;  
  /*! Array of mac handler id in uint32 */
  uint32                            mac_handler_id_arr[LTE_MAC_MAX_INSTANCE_PER_DEL_REQ];
} lte_mac_filter_del_req_s;

/*! @brief PAC match report related information parameters structure, this will be used when MAC report match result to 
client */
typedef struct
{
  /*! DL PAC code value received, LTE_MAC_PAC_CODE_LEN is 23 bytes */
  uint8           pac[LTE_MAC_PAC_CODE_LEN];  
  /*! number of active handler id in the above array, put here for word 
  alignment purpose with below pac structure */
  uint8           active_handler_id_num; 
  /*! Matched mac_handler_id associated with matched filter PAC */
  uint32          mac_handler_id_arr[LTE_MAC_MAX_NUM_FILTER_PER_MATCH_RPT]; 
  /*! MIC value */
  uint32          mic;
  /*! UTC system time in seconds */
  uint32          utc_time; 
  /*! The frequency of the cell on which the transport block was received */
  uint32          freq : 20;
  /*! time_lsb */
  uint32          time_lsb : 4;
  /*! reserved bits */
  uint32          reserve_bits : 8;
} lte_mac_pac_match_rpt_s;

/*! @brief MAC PAC match report request from MAC to LTE-D
*/
typedef struct 
{
  msgr_hdr_struct_type                   msg_hdr;  /*!< Message header */  
  /*! the message payload is the filter match report */
  lte_mac_pac_match_rpt_s               pac_match_rpt;
} lte_mac_pac_match_rpt_ind_s;

/*! @brief MAC PAC status report ind from MAC to LTE-D, it contains a 
list of MAC handler id that was successfully txed at the first time after the corresponding PAC 
get installed in the MAC. */
typedef struct 
{
  msgr_hdr_struct_type        msg_hdr;  /*!< Message header */  
  /*! the message payload is the list of MAC PACs which are successfully txed at
  the first time after they get installed in the MAC */
  /*! number of active client handler id to report in below array */
  uint8                      active_num;
  /*! Array of handler id in MAC side */
  uint32                     mac_handler_id_arr[LTE_MAC_MAX_INSTANCE_PER_TX_STATUS_RPT_IND];
} lte_mac_pac_tx_status_rpt_ind_s;

#endif /* LTE_MAC_PROSE_API_H */
