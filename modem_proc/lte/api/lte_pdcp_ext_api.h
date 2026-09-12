/*!
  @file
  lte_pdcp_ext_api.h

  @brief
  REQUIRED brief one-sentence description of this C header file.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies, Inc. All Rights Reserved

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
  written permission of Qualcomm Technologies, Inc.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/api/lte_pdcp_ext_api.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/08/17   el      CR2084937: Porting DSDS L+L mac layer changes
05/21/15   sb      CR803880: modified PDCP interface for UDC feature
11/19/13   sb      CR535830: PDCP/RLC Re-arch and support of REL-11 features
11/18/13   sb      CR576794: New PDCP API prototype for DS to tag UL packet
10/02/13   mg      CR489345: Get num of PDCP PDU bytes transmitted for an EPSID
12/03/12   sb      CR427147: instead of sending RoHC feedback pkt as a separate message,
                         it will be copied to PDCP-OFFLOAD circular buffer just like data PDU
11/07/12   ax      CR383618: L2 modem heap use reduction phase I
10/22/12   sb      CR411260: Handle the case properly when UE moves from non-ciphering to 
                         ciphering enabled case during RoHC scenario
05/16/12   sb      CR361321: Added L2 indication optimization structure to reduce
                         frequency between PDCP to OFFLOAD notification.
01/04/10   ax      move what is needed by FTM under API dir
05/18/10   ax      consolidate pdcp external APIs and place them under lte/api/
==============================================================================*/

#ifndef LTE_PDCP_EXT_API_H
#define LTE_PDCP_EXT_API_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <lte.h>
#include <queue.h>
#include <dsm_item.h>
#include <dsm_queue.h>
#include <mutils_circ_q.h>
#include <lte_security.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*! @brief PDCPDL->PDCP OFFLOAD circular queue size (number of elements)
     10ms*1 entry per TTI per bearer*total number of active bearers
*/
#define	LTE_MAX_ACTIVE_ROHC_RB 32

#define PDCP_DL_SEC_LTE_NUM_KEYS_MAX 16
#define PDCP_DL_SEC_KEY_SIZE 16

/*! @brief DL data activity statistics
*/
typedef struct
{
  uint32  num_bytes;        /*!< data pdu bytes recieved*/
  uint32  num_pdus;         /*!< number of data PDUs received */
  uint32  num_pdus_dropped; /*!< number of data pdus dropped */
} lte_pdcpdl_activity_stats_s;


/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/


/*==============================================================================

  FUNCTION:  lte_pdcpdl_get_activity_statistics

==============================================================================*/
/*!
    @brief
    access function for DL data statistics that can be called from other module.

    @detail
    Caller needs to be aware of the possibility of statistics overflow, and 
    handle it appropriately.

    @return
    TRUE if the statistics returned is valid. FALSE if the statistics for the 
    eps_id is not available (bearer removed, etc)
*/
/*============================================================================*/
extern boolean lte_pdcpdl_get_activity_statistics
(
  lte_eps_id_t                  eps_id,   /*!< eps ID */
  lte_pdcpdl_activity_stats_s*  stats_ptr, /*!< pointer to caller provided 
                                               staticstics struct */ 
  uint8                         as_id
);

/*==============================================================================

  FUNCTION:  lte_pdcpul_get_pdu_bytes_tx

==============================================================================*/
/*!
    @brief
    Get the number of PDCP PDU bytes transmitted per epsid

    @detail
    Gives the total number of PDCP PDU bytes transmitted so far per epsid.
    The values are reset during the connection release.

    @return
    Number of PDCP PDU bytes transmitted per epsid if it is valid. 
    0 Otherwise

*/
/*============================================================================*/
extern uint64 lte_pdcpul_get_pdu_bytes_tx( lte_eps_id_t epsid, lte_l2_common_as_id_e as_id);

/*==============================================================================

  FUNCTION:  lte_pdcpul_tag_each_pkt

==============================================================================*/
/*!
    @brief
    This function is called to tag eack packet with discard tick and populate
    kind fill as "NEW"

    @detail

    @return

    @note

    @see

*/
/*============================================================================*/

extern void lte_pdcpul_tag_each_pkt
(
  dsm_item_type* pkt_ptr,      /*packet to be enqueued to PS-PDCP WM*/
  uint8          as_id
);

#endif /* LTE_PDCP_EXT_API_H */
