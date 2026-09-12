/*!
  @file
  rfwcdma_core_log.c

  @brief
  This file contains all declarations and definitions pertaining to WCDMA
  RF logging
*/

/*==============================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_log.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/22/18   nik     KW error fix
02/01/18   nik     TxAGC logging fix
06/03/16   ag      Remove unnecessary msgs to avoid F3 flooding
05/24/16   ag      W+W logging changes for inside the W2W gap
01/28/16   sd      Feature differentiation for WTRs TA2.3
11/30/15   ak      AGC logging Fixes
10/07/15   rmb     Fix KW Error.
10/06/15   ac/rmb  AGC logging change
08/26/15   ac      backingout the agc logging change for W plus W, will re-submit along with WL1 code
08/25/15   ac      additional checks, fix to support the older api
08/20/15   ac      agc logging changes for w plus w
08/13/15   asn     Fix errors in logging
07/22/15   asn     Changes to call new RxAGC read API
07/06/15   ak      Moving to the new W plus W feature
06/08/15   vbh     Log valid AGC only 
05/21/15   sd      Dispatch clk voting in Rf task only when enabled
04/21/15   ac       w plus w rxagc bringup changes
4/10/15    vbh     [Rx/Tx split] Updates for WCDMA data routine
02/18/15   kr      replace FEATURE_JOLOKIA_MODEM by FEATURE_TABASCO_MODEM
01/08/15   sd      Replace deprecated API rfwcdma_mdsp_get_lna_range() usage
11/26/14   zhh    add max carrier num check before call rfwcdma_core_util_get_rxagc()
11/19/14   vs      Featurize the AGC logging as per the modem support
03/19/14   kr      ASDiv: Logging issue with Ant Pos
01/28/14   vs      Fix incorrect LNA state query for path1
06/30/14   kai     Fix immediate AGC logging
06/18/14   kai     Add AGC Subpacket Version 2
05/01/14   rmb     Add support for new version of AGC log packet 0x4176 v2.
04/11/14   dw      Add Tx ON logging from RFLM
02/04/14   kai     Add xpt logging support
12/17/13   kr      added car_id support to rfwcdma_mdsp_is_tx_accum_frozen()
12/05/13   bn      Add IQ gain logging support
12/04/13   dw      Consolidate all get RxAGC core functions. Move all RxAGC logging through single API
12/04/13   kr      set txagc_frozen_ch0ch1 to 1 
10/28/13   dw      Add LNA state logging support
09/10/13   dw      Remove DBDC featurization and code clean up.
08/23/13   vs      Added support for 3 carrier
08/13/13   aro     Extended log packet definition to support subpacket
07/31/13   aro     Fixed antenna switch position query function call
07/30/13   aro     Doxygen grouping
07/30/13   aro     Added debug mode to print F3 during logging
07/30/13   aro     Fixed the invalid NULL pointer check
07/30/13   aro     Added function to log AGC once
07/29/13   aro     Implementation to populate AGC param
07/29/13   aro     Converted AGC chunk to bitwise field in the structure
07/29/13   aro     Updated log packet payload
07/29/13   aro     Documentation of log packet fields
07/24/13   aro     Consolidated all data structure to one
07/24/13   aro     Added initial framework to support AGC logging for WCDMA
07/24/13   aro     Initial version

==============================================================================*/

#include "comdef.h"
#include "rfwcdma_core_log.h"
#include "rfwcdma_core_txplim.h"
#include "rfwcdma_core_util.h"
#include "rfwcdma_mdsp.h"
#include "rfcommon_msg.h"
#include "rfcommon_log.h"
#include "rfcommon_asdiv_manager.h"
#include "log.h"
#include "seq.h"
#include "log_codes_wcdma_int.h"
#include "rfwcdma_data.h"
#include "modem_mem.h"
#include "rfm_device_types.h"

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the maximum number of AGC samples that can be 
part of each log packet */
#define RFWCDMA_LOG_AGC_SAMPLES 50

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the number of AGC samples in immediate AGC 
log packet */
#define RFWCDMA_LOG_AGC_ONE_INSTANCE_SAMPLES 1

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the TxAGC equivalent of -80 dbm in AGC 
log packet */
#define RFWCDMA_LOG_TX_AGC_THRESHOLD -612

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the AGC version */
typedef enum
{
  RFWCDMA_LOG_AGC_VER_1 = 1, /*!< AGC Version 1 */
  RFWCDMA_LOG_AGC_VER_2 = 2, /*!< AGC Version 2 */
  RFWCDMA_LOG_AGC_VER_3 = 3  /*!< AGC Version 3 */
} rfwcdma_log_agc_version_type;

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the AGC version */
typedef enum
{
  RFWCDMA_LOG_AGC_SUBPACKET_VER_1 = 1, /*!< AGC Subpacket Version 1 */
  RFWCDMA_LOG_AGC_SUBPACKET_VER_2 = 2, /*!< AGC Subpacket Version 2 */
} rfwcdma_log_agc_subpacket_version_type;

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the AGC version */
typedef enum
{
  RFWCDMA_LOG_AGC_SUBPACKET_ID = 1 /*!< Subpacket holding all AGC data */
} rfwcdma_log_agc_subpacket_id_type;

/*============================================================================*/
/*!
  @addtogroup RFWCDMA_LOG_AGC_PACKET_DEFN
 
  @brief
  This module contains the definition of AGC log packet. This will be the
  interface between embedded side and tools to transfer AGC log parameter
  @{
*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Header structure for RF log sub-packets.

  @details
  This structure is the header for any RF sub-packet, contains information about 
  the payload that follows
*/
typedef PACK(struct)
{
  uint8 pkt_id;
  /*!< Sub-packet ID, a unique sub-packet identifier*/ 

  uint8 pkt_ver;
  /*!< Sub-packet version number, if the version is unknown to the parsing tool 
 the sub-packet will be skipped */ 

  uint16 pkt_size;
  /*!< Sub-packet size, size in bytes of the sub-packet, including the header. 
  If the version of the sub-packet is unknown this size can be used to skip the 
  subpacket and decode the next one */ 

} rfwcdma_log_sub_pkt_header_type;

/*----------------------------------------------------------------------------*/
/*! Structure to hold the AGC parameter */
typedef PACK(struct)
{
  PACK(struct)
  {
    uint16 agc_status_ch0cr0:1; 
    /*!< Chain 0 Carrier 0 AGC status */

    uint16 agc_status_ch0cr1:1;
    /*!< Chain 0 Carrier 1 AGC status */

    uint16 agc_status_ch0cr2:1;
    /*!< Chain 0 Carrier 2 AGC status */

    uint16 agc_status_ch0cr3:1;
    /*!< Chain 0 Carrier 3 AGC status */

    uint16 agc_status_ch1cr0:1;
    /*!< Chain 1 Carrier 0 AGC status */

    uint16 agc_status_ch1cr1:1;
    /*!< Chain 1 Carrier 1 AGC status */

    uint16 agc_status_ch1cr2:1;
    /*!< Chain 1 Carrier 2 AGC status */

    uint16 agc_status_ch1cr3:1;
    /*!< Chain 1 Carrier 3 AGC status */

    uint16 :8;
    /*!< Reserved */

  }agc_info_chunk_0;
  /*!< 16-bit field to store AGC information */

  PACK(struct)
  {
    uint16 lna_state_ch0lna1:3;
    /*!< LNA state for a given LNA#1 on a Chain 0. During  four carrier 
    dual band (DB-4C) with diversity operation, upto 4 LNA can be 
    used simultaneously. Allocation of LNA for each carrier may 
    differ based on the mode of operation. */ 

    uint16 lna_state_ch0lna2:3;
    /*!< LNA state for a given LNA#2 on a Chain 0  */

    uint16 lna_state_ch1lna1:3;
    /*!< LNA state for a given LNA#1 on a Chain 1  */

    uint16 lna_state_ch1lna2:3;
    /*!< LNA state for a given LNA#2 on a Chain 1 */

    uint16 :4;
    /*!< Reserved */

  }agc_info_chunk_1;
  /*!< 16-bit field to store AGC information */

  PACK(struct)
  {
    uint16 pa_on:2;
    /*!< Indicates if PA is in ON state or not */

    uint16 pa_range:4;
    /*!< Current PA Range */

    uint16 tx_on:1;
    /*!< Indicates if the Tx is ON */

    uint16 txagc_frozen_ch0cr0:1;
    /*!< Flag indicating if the TxAGC is in frozen state for 
    Chain 0 and carrier 0 */

    uint16 txagc_frozen_ch0cr1:1;
    /*!< Flag indicating if the TxAGC is in frozen state for 
    Chain 0 and carrier 1 */

    uint16 :7;
    /*!< Reserved */

  } agc_info_chunk_2;
  /*!< 16-bit field to store AGC information */

  int16 rx_agc_chain0_carr0;
  /*!< Most recent RxAGC value for the given Chain 0  and Carrier 1. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. 
   
  For conversion to dBm unit, following formula should be used 
   
             RxAGC_dBm = -106 + (RxAGC + 512)/10,
   
  where RxAGC is the output from the target. */

  int16 rx_agc_chain0_carr1;
  /*!< Most recent RxAGC value for the given Chain 0  and Carrier 1. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain0_carr2;
  /*!< Most recent RxAGC value for the given Chain 0  and Carrier 2. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain0_carr3;
  /*!< Most recent RxAGC value for the given Chain 0  and Carrier 3. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain1_carr0;
  /*!< Most recent RxAGC value for the given Chain 1  and Carrier 0. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain1_carr1;
  /*!< Most recent RxAGC value for the given Chain 1  and Carrier 1. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain1_carr2;
  /*!< Most recent RxAGC value for the given Chain 1  and Carrier 2. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 rx_agc_chain1_carr3;
  /*!< Most recent RxAGC value for the given Chain 1  and Carrier 3. 
  This field will be valid only if its corresponding AGC status field is 
  enabled. */

  int16 tx_agc_chain0_carr0;
  /*!< Tx output power for chain 0 carrier 0. This field can be converted to dBm 
  using
   
             Tx_dBm = -70 + (TX_AGC + 512)/10,
   
  where TX_AGC is the output from the target. Vailidy of this
  field will be based on PA ON */

  int16 tx_agc_chain0_carr1;
  /*!< Tx output power for chain 0 carrier 1. Vailidy of this
  field will be based on PA ON */

  int16 tx_agc_total;
  /*!< Total Tx output power */

  int16 tx_agc_lut_idx;
  /*!< This is the PDM value to adjust TX AGC gain. Will be valid only  if 
  “TX_ON” is active */

  int16 tx_pwr_lim;
  /*!< Maximum Tx Power limit (-512 to 511) in 1/12 dBm steps 
   
             Tx Power = -70 + (MAX_TX + 512)/10 in dBm
   
  Will be valid only  if “TX_ON” is 1 */ 

  int16 hdet;
  /*!< This is the raw HDET conversion obtained from the power detector device.
  Will be active only  if “TX_ON” is 1 */

  int16 iq_gain;
  /*!< Digital IQ Gain value applied in Tx signal path */

  uint16 dpd_index;
  /*!< DPD Index applied in Tx */

  int16 env_scale;
  /*!< Envelope Scale value applied in Tx signal path */

  uint16 pa_current;
  /*!< Quiescent current applied to power amplifier */

  uint32 pa_bias;
  /*!< Bias applied to power amplifier */

  uint8 antenna_position;
  /*!< Current Antenna switch position */

  uint8 xpt_mode;
  /*!< Current xPT mode: APT, EPT or ET */

  int16 pin_comp;
  /*!< Pin Compensation value */

  int16 pout_comp;
  /*!< Pout Compensation value */

  uint8 pin_rgi;
  /*!< Pin RGI value */

  int16 delay;
  /*!< Delay */

} rfwcdma_log_agc_param_type;

/*----------------------------------------------------------------------------*/
/*! Definition for AGC log sub packet */
typedef PACK(struct)
{
  rfwcdma_log_sub_pkt_header_type header;
  /*!< Sub-packet header*/ 

  uint8 num_samples;
  /*!< Number of samples of AGC data in the log packet*/

  uint8 start_cfn; 
  /*!< Frame number from which the logging was started for this log */

  rfwcdma_log_agc_param_type agc_params[RFWCDMA_LOG_AGC_SAMPLES];
  /*!< AGC parameter */

} rfwcdma_log_agc_sub_packet_type;

/*----------------------------------------------------------------------------*/
/*! Structure defining the log structure for AGC log */
typedef PACK(struct)
{
  rfcommon_log_rf_header_type rf_header;
  /*!< RF log packet header, this contains information about the version of RF 
  log packet and the number of sub packets */ 

  rfwcdma_log_agc_sub_packet_type agc_sub_pkt;
  /*!< Structure to hold AGC sub packet*/

} rfwcdma_log_agc_packet_type;

/*! @} */

/*----------------------------------------------------------------------------*/
/*! Count the Log Packet length */
#define RFWCDMA_LOG_AGC_V3_LENGTH( num_sample )  \
(\
  FPOS( rfwcdma_log_agc_sub_packet_type, agc_params ) + \
  ( num_sample * sizeof( rfwcdma_log_agc_param_type ) ) \
)


/*----------------------------------------------------------------------------*/
typedef struct
{ 
  sys_modem_as_id_e_type w_sub;
  /*!< Add subscriber id*/

  uint16 counter;
  /*!< Variable to store the counter of AGC param samples, 50 for now */

  rfwcdma_log_agc_packet_type *log_packet;
  /*!< Data struture to hold the log packet data */
  
  uint16 num_of_sub_packet;
  /*!< Number of Sub packets in the log packet. SW supports only 1 sub packet*/

} rfwcdma_log_agc_packet_data_type;

typedef struct
{
   /*!< is the subsciber assigned */
   boolean is_sub_assigned;
   
   /*!< return the log_handle for teh subscriber being assigned*/
   uint16 log_handle;

} rfwcdma_log_sub_table_info_type;

/*----------------------------------------------------------------------------*/
/*! Data structure to hopld housekeeping data for WCDMA AGC log */
typedef struct
{
  boolean debug_mode;
  /*!< Flag indicating debug mode */

  word agc_log_code;
  /*!< Variable to store the log code for AGC logging */
  
  rfwcdma_log_sub_table_info_type sub_table[SYS_MODEM_AS_ID_MAX];
  /*!< Data struture to hold teh subscriber info*/
  
  rfwcdma_log_agc_packet_data_type *agc_packet[RFM_WCDMA_LOG_AGC_MAX_HANDLE];
 /*!< agc data packet for both SUB_IDS*/

} rfwcdma_log_agc_type;


/*! Struture to hold housekeeping data for all WCDMA log */
typedef struct
{
  rfwcdma_log_agc_type agc_log;
  /*!< Struture to hold housekeeping data for WCDMA AGC log */

} rfwcdma_log_data_type;

/*----------------------------------------------------------------------------*/
/*! Variable to hold housekeeping data for all WCDMA log */
static rfwcdma_log_data_type rfwcdma_log_data;

/*============================================================================*/
/*!
  @addtogroup RFWCDMA_LOG_AGC_INT_HELPER
 
  @brief
  This module contains the internal helper functions to be used during AGC 
  logging
  @{
*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get pointer to AGC log data
 
  @details
  This function returns the pointer to the AGC log data
 
  @return
  Pointer to AGC log data
*/
static rfwcdma_log_agc_type*
get_agc_log_data
(
  void
)
{
  return &(rfwcdma_log_data.agc_log);
} /* get_agc_log_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Initialize AGC log
 
  @return
  Flag indicating the status of initialization procedure
*/
static boolean
init_agc_log
(
  void
)
{
  rfwcdma_log_agc_type* log_data = NULL; /* Pointer to AGC Log */

  /* Get Pointer to log data */
  log_data = get_agc_log_data();

  /* Clear the Log data first */
  memset(log_data,0, sizeof(rfwcdma_log_agc_type));

  log_data->debug_mode = FALSE;

  /* Enable WCDMA AGC log code for logging */

  /* Init Housekeeping Data */
  log_data->agc_log_code = WCDMA_AGC_0x4176_LOG_PACKET;

  return TRUE;

} /* init_agc_log */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Check if AGC log is enabled
 
  @param log_data
  Pointer to data structure containing AGC log data
 
  @return
  Flag indicating if the log is enabled or not
*/
static boolean
is_agc_log_enabled
(
  rfwcdma_log_agc_type* log_data
)
{
  if ( log_data != NULL )
  {
    return log_status( log_data->agc_log_code ) ;
  }
  else
  {
    return FALSE;
  }
} /* is_agc_log_enabled */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create AGC log instance.
 
  @details
  This function creates an instance of AGC log packet. WL1 is expected to call this function to
  create unique handle per subscriber to log AGC for each subscriber. The handle returned by
  this function should be used by L1 to perform any AGC related action with RF.

 
  @param subscriber_id
  Susbscriber ID used to create AGC log. This information is used by RF
  to populate a field in the log packet. Beside this, RF has no use case of this information.
 
 
  @return
  Unique handle generated for AGC logging 
*/

rfm_wcdma_log_agc_handle_type rfwcdma_log_agc_create(sys_modem_as_id_e_type subscriber_id) 
{
  uint16 handle_index =0;
  rfwcdma_log_agc_type* log_data = NULL; /* Pointer to AGC Log */
  rfm_wcdma_log_agc_handle_type ret_val = RFM_WCDMA_LOG_AGC_MAX_HANDLE;

  /* Get Pointer to log data */
  log_data = get_agc_log_data();

  /* NULL pointer check */
  if ( log_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_log_agc_create: NULL log Data" );
    return RFM_WCDMA_LOG_AGC_MAX_HANDLE;
  } /* if ( log_data == NULL ) */

  if(( subscriber_id <= SYS_MODEM_AS_ID_NONE ) || ( subscriber_id >= SYS_MODEM_AS_ID_MAX ))
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_log_agc_create: Invalid subscriber_id: %d",
              subscriber_id); 
    return RFM_WCDMA_LOG_AGC_MAX_HANDLE;
  }

  if ( log_data->sub_table[subscriber_id].is_sub_assigned == FALSE )
  {
    for (handle_index = RFM_WCDMA_LOG_AGC_HANDLE_0; handle_index < RFM_WCDMA_LOG_AGC_MAX_HANDLE ; handle_index++)
    {
      if((log_data->agc_packet[handle_index] == NULL))
      {
        /*Perform the memory allocation for AGC packet - This is just going to create pointer to AGC log packet*/
        log_data->agc_packet[handle_index] = 
       (rfwcdma_log_agc_packet_data_type *)modem_mem_alloc(sizeof(rfwcdma_log_agc_packet_data_type),MODEM_MEM_CLIENT_RFA ); 

        if ( (log_data->agc_packet[handle_index] != NULL) )
        {
          /* Clear the rfwcdma_log_agc_packet_type first */
          memset(log_data->agc_packet[handle_index],0, sizeof(rfwcdma_log_agc_packet_data_type));

          log_data->agc_packet[handle_index]->w_sub = subscriber_id;
          log_data->agc_packet[handle_index]->num_of_sub_packet = 1;
          log_data->agc_packet[handle_index]->counter = 0;

          if (log_data->agc_packet[handle_index]->log_packet == NULL) 
          {
            /* Perform the memory allocation for AGC log packet - which includes RF header and AGC log sub packet*/
            log_data->agc_packet[handle_index]->log_packet = (rfwcdma_log_agc_packet_type*)modem_mem_alloc( sizeof(rfwcdma_log_agc_packet_type), MODEM_MEM_CLIENT_RFA );
          }
         
          if ( (log_data->agc_packet[handle_index]->log_packet != NULL) )
          {
            /* Clear the rfwcdma_log_agc_packet_type first */
            memset(log_data->agc_packet[handle_index]->log_packet,0, sizeof(rfwcdma_log_agc_packet_type));

            /* Initialize RF header (which includes Log header) and Sub Packet ID header information */
            log_data->agc_packet[handle_index]->log_packet->rf_header.pkt_version= RFWCDMA_LOG_AGC_VER_2;
            log_data->agc_packet[handle_index]->log_packet->rf_header.num_sub_pkt = 1;   
            log_data->agc_packet[handle_index]->log_packet->rf_header.hdr_reserved = 0x0410; /* Magic number. Has no meaning */
            log_set_code( log_data->agc_packet[handle_index]->log_packet, log_data->agc_log_code ); // This is a must to set the CODE in the LOG HEADER

            log_data->agc_packet[handle_index]->log_packet->agc_sub_pkt.header.pkt_id = RFWCDMA_LOG_AGC_SUBPACKET_ID;
            log_data->agc_packet[handle_index]->log_packet->agc_sub_pkt.header.pkt_ver = RFWCDMA_LOG_AGC_SUBPACKET_VER_2;
            log_data->agc_packet[handle_index]->log_packet->agc_sub_pkt.header.pkt_size = sizeof(rfwcdma_log_agc_sub_packet_type);

            log_data->sub_table[subscriber_id].is_sub_assigned = TRUE;
            log_data->sub_table[subscriber_id].log_handle= handle_index;
            ret_val= handle_index;
            RF_MSG_2( RF_HIGH, "rfwcdma_log_agc_create: SUB_ID %d,Log Handle %d is Assigned", subscriber_id,
                      handle_index );
          }

          else
          {
            RF_MSG_2( RF_FATAL, "rfwcdma_log_agc_create: Mem Alloc failure for AGC Log packet (Header + Sub Log) for SUB_ID %d, Log Handle %d",
                      subscriber_id, handle_index);
            return RFM_WCDMA_LOG_AGC_MAX_HANDLE;
          }/*( (log_data->agc_packet[handle_index]->log_packet != NULL) )*/

          break;
        }/*(log_data->agc_packet[handle_index] != NULL)*/

        else
        {
          RF_MSG_2( RF_FATAL, "rfwcdma_log_agc_create: Mem Alloc failure for AGC data packet for SUB_ID %d, Log Handle %d",
                    subscriber_id, handle_index);
          return RFM_WCDMA_LOG_AGC_MAX_HANDLE;
        }/* (log_data->agc_packet[handle_index] == NULL)*/

      }/*(log_data->agc_packet[handle_index] == NULL)*/
    }/*(handle_index = 0; handle_index < RFM_WCDMA_LOG_AGC_MAX_HANDLE ; handle_index++)*/
  } /*   if ( log_data->sub_table[subscriber_id].is_sub_assigned == FALSE ) */

  else
  {
    RF_MSG_1( RF_HIGH, "rfwcdma_log_agc_create: the subscriber_id is already allocated :%d",subscriber_id );
    ret_val = log_data->sub_table[subscriber_id].log_handle;
  }

 return ret_val;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Compute the total log size
 
  @param agc_log
  Pointer to data structure containing the log data
 
  @return
  Total size of the log packet
*/
static uint16
compute_log_size
(
  rfwcdma_log_agc_packet_data_type *agc_packet
)
{

  uint16 tot_size = 0;

  tot_size += sizeof(rfcommon_log_rf_header_type);

  if (agc_packet == NULL) 
  {
    RF_MSG( RF_ERROR, "compute_log_size: AGC Data packet is NULL");
    return tot_size;
  }

  /* Param Validation */
  if ( agc_packet->log_packet == NULL )
  {
    RF_MSG( RF_ERROR, "compute_log_size: AGC Log packet (Header + Sub Log) is NULL ");
    return tot_size;
  } /* if (agc_packet->log_packet == NULL) */

  /* Number of Sub Packet is 1 */
  tot_size += agc_packet->log_packet->agc_sub_pkt.header.pkt_size;

  return tot_size;

} /* compute_log_size */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Flush AGC log
 
  @details
  This function is called from AGC log function to flush log to diag.
 
  @param agc_log
  Pointer to data stucture containing AGC log data
 
  @return
  Flag indicating the status of log flush
*/
static boolean 
flush_agc_log
(
  sys_modem_as_id_e_type subscriber_id,
  rfwcdma_log_agc_packet_data_type *agc_packet
)
{
  boolean ret_val = TRUE; /* Return value */
  rfwcdma_log_agc_packet_type *log_packet = NULL; // Pointer to memory allocated by APEX api
  uint16 total_log_size = 0;
  uint32 sub_id_to_diag = 1; // By default, Sub ID to be passed to Diag is Sub Index = 0 + 1 = 1

  /* NULL Pointer check */
  if ( agc_packet != NULL )
  {
    /* Param Validation */
    if ( agc_packet->log_packet == NULL )
    {
      RF_MSG( RF_ERROR, "flush_agc_log: AGC Log packet (Header + Sub Log) is NULL ");
      return FALSE;
    } /* if (agc_packet->log_packet == NULL) */

    else
    {
      agc_packet->log_packet->agc_sub_pkt.num_samples = agc_packet->counter;
      agc_packet->log_packet->agc_sub_pkt.header.pkt_size = 
                        RFWCDMA_LOG_AGC_V3_LENGTH(agc_packet->counter);

      /* Compute the Total log size */
      total_log_size = compute_log_size(agc_packet);

      /* Pass Sub ID = Subscriber ID index + 1 to Diag so that Index 0 and 1
      are printed as Sub ID 1 and 2 respectively */
      sub_id_to_diag = (uint32)subscriber_id + 1;

      /* Set the length of thr Log Packet */
      log_set_length( agc_packet->log_packet, total_log_size ); 

      /* Commit Log */
      if ( log_submit_ext((PACKED void *)agc_packet->log_packet, 1, (void*)(&sub_id_to_diag)) == FALSE )
      {
        RF_MSG( RF_ERROR, "flush_agc_log: Log Commit Failure");
        ret_val = FALSE;
      } /* if (log_submit_ext((PACKED void *) agc_packet->log_packet, 1, (void*)(&sub_id)) == FALSE ) */

      else
      {
        /* Reset all required data */
        agc_packet->log_packet->agc_sub_pkt.num_samples = 0;
        agc_packet->counter = 0;
        ret_val = TRUE;
      }
    }/*( agc_packet->log_packet != NULL )*/

  } /*  if ( agc_log != NULL )*/

  else
  {
    RF_MSG( RF_ERROR, "flush_agc_log: AGC Data packet is NULL");
    ret_val = FALSE;
  } /* if ! ( agc_log != NULL ) */

  return ret_val;
} /* flush_agc_log */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Delete AGC log instance.
 
  @details
  This function deletes an instance of AGC log packet. All memory allocated
  for AGC for the given handle will be released.


  @param  log_handle
  log_handle	Log handle pertaining to the AGC log, which is to be deleted

 
  @return
  Status of AGC log deletion  
*/

boolean rfwcdma_log_agc_delete (rfm_wcdma_log_agc_handle_type log_handle)
{
  rfwcdma_log_agc_type* log_data = NULL; /* Pointer to AGC Log */
  sys_modem_as_id_e_type w_sub = SYS_MODEM_AS_ID_MAX;

  /* Get Pointer to log data */
  log_data = get_agc_log_data();

  if ( log_handle >= RFM_WCDMA_LOG_AGC_MAX_HANDLE )
  {
     RF_MSG( RF_ERROR, "rfwcdma_log_agc_delete: Bad Handle" );
    return FALSE;
  } /*   if ( log_handle >= RF_WCDMA_LOG_AGC_HANDLE_MAX ) */


  /* NULL pointer check */
  if ( log_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_log_agc_delete: NULL log Data" );
    return FALSE;
  } /* if ( log_data == NULL ) */
  /* Check if agc_log data is NULL*/

  if ( (log_data->agc_packet[log_handle] != NULL) )
  {
    w_sub = log_data->agc_packet[log_handle]->w_sub;

    if(( w_sub <= SYS_MODEM_AS_ID_NONE ) || ( w_sub >= SYS_MODEM_AS_ID_MAX ))
    {
      RF_MSG_1( RF_ERROR, "rfwcdma_log_agc_delete: Invalid subscriber_id: %d",
                w_sub); 
      return FALSE;
    }

    if (log_data->agc_packet[log_handle]->log_packet != NULL) 
    {
      /* De-allocate the AGC log packet which has RF header + Sub log packet*/
      modem_mem_free(log_data->agc_packet[log_handle]->log_packet, MODEM_MEM_CLIENT_RFA);
    }

    log_data->agc_packet[log_handle]->log_packet = NULL;

    /* De-allocate the AGC data packet */
    modem_mem_free(log_data->agc_packet[log_handle], MODEM_MEM_CLIENT_RFA);
    log_data->agc_packet[log_handle] = NULL;

    log_data->sub_table[w_sub].is_sub_assigned = FALSE;
    log_data->sub_table[w_sub].log_handle= RFM_WCDMA_LOG_AGC_MAX_HANDLE;
    return TRUE;
  }

  else
  {
    RF_MSG( RF_ERROR, "rfwcdma_log_agc_delete:already set to NULL");
    return FALSE;
  }
 
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Fetch AGC parameter
 
  @details
  This function is a helper function for WCDMA AGC logging function to
  propulate the AGC parameter fields with the most recent values.
 
  @param param_ptr
  Pointer to the structure where the agc parameter will be stored
 
  @param debug_mode
  Flag indicating if logging is done in debug mode
*/
static void 
rfwcdma_core_log_get_agc_param
(
  rfwcdma_log_agc_param_type *param_ptr,
  boolean debug_mode,
  rfm_wcdma_log_agc_param_type *intf_param
)
{
  int16 rxagc_val = 0;
  rfwcdma_core_util_txagc_logging_type txagc_data = {0};
  rfm_device_enum_type device = RFM_INVALID_DEVICE;

  rfm_wcdma_rxagc_input_params rxagc_input_info = {0};
  rfm_wcdma_rxagc_output_params rxagc_output_info = {0};

  /* Parameter validation */
  if ( param_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_get_agc_param : NULL pointer" );
    return;
  } /* if ( param_ptr == NULL ) */

  /*Initialize the params */
  param_ptr->agc_info_chunk_1.lna_state_ch0lna1=7;
  param_ptr->agc_info_chunk_1.lna_state_ch0lna2=7;
  param_ptr->agc_info_chunk_1.lna_state_ch1lna1=7;
  param_ptr->agc_info_chunk_1.lna_state_ch1lna2=7;

  param_ptr->agc_info_chunk_2.pa_on = FALSE;
  param_ptr->agc_info_chunk_2.pa_range=7;
  param_ptr->agc_info_chunk_2.tx_on = FALSE;
  param_ptr->agc_info_chunk_2.txagc_frozen_ch0cr0 = 0;
  param_ptr->tx_agc_chain0_carr0 =0;
  param_ptr->tx_agc_chain0_carr1 =0;

  param_ptr->agc_info_chunk_0.agc_status_ch0cr0= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch0cr1= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch0cr2= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch0cr3= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch1cr0= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch1cr1= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch1cr2= FALSE;
  param_ptr->agc_info_chunk_0.agc_status_ch1cr3= FALSE;

  param_ptr->rx_agc_chain0_carr0 = -512; 
  param_ptr->rx_agc_chain0_carr1 = -512;
  param_ptr->rx_agc_chain0_carr2 = -512;
  param_ptr->rx_agc_chain0_carr3 = -512;
  param_ptr->rx_agc_chain1_carr0 = -512;
  param_ptr->rx_agc_chain1_carr1 = -512; 
  param_ptr->rx_agc_chain1_carr2 = -512; 
  param_ptr->rx_agc_chain1_carr3 = -512; 

  param_ptr->antenna_position = 0;

   /* device interface validation */
  if ( intf_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_get_agc_param : NULL pointer for intf_param" );
    return;
  } /* if ( param_ptr == NULL ) */

  // get through the parameter instead of deriving

  /*======================PRIMARY===============================*/
  // First check for first Carrier on Sub 1
  device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].rfm_device; 
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {
    param_ptr->agc_info_chunk_1.lna_state_ch0lna1 = 
                 rfwcdma_core_util_get_lna_state( device);
  }

  /* Else check for first carrier on Sub 2*/
  else if ( ((device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device) >= RFM_DEVICE_0) && 
            ((device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device) < RFM_WCDMA_MAX_DEVICE) )
  {
    param_ptr->agc_info_chunk_1.lna_state_ch0lna1 = 
                 rfwcdma_core_util_get_lna_state( device);
  }
  
  // First check for second Carrier on Sub 1
  device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__1].rfm_device; 
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {         
    param_ptr->agc_info_chunk_1.lna_state_ch0lna2 =  rfwcdma_core_util_get_lna_state( device );
  }

  /* Else check for second carrier on Sub 2*/
  else if ( ((device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__3].rfm_device) >= RFM_DEVICE_0) && 
            ((device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__3].rfm_device) < RFM_WCDMA_MAX_DEVICE) )
  {              
    param_ptr->agc_info_chunk_1.lna_state_ch0lna2 =  rfwcdma_core_util_get_lna_state( device );
  }

  /*======================DIVERSITY===============================*/
  // First check for first Carrier on Sub 1
  device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__0].rfm_device; 
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {
    param_ptr->agc_info_chunk_1.lna_state_ch1lna1 = 
                 rfwcdma_core_util_get_lna_state( device);
  }

  /* Else check for first carrier on Sub 2*/
  else if ( ((device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__2].rfm_device) >= RFM_DEVICE_0) && 
            ((device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__2].rfm_device) < RFM_WCDMA_MAX_DEVICE) )
  {
    param_ptr->agc_info_chunk_1.lna_state_ch1lna1 = 
                 rfwcdma_core_util_get_lna_state( device);
  }

  // First check for second Carrier on Sub 1
  device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__1].rfm_device; 
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {         
    param_ptr->agc_info_chunk_1.lna_state_ch1lna2 =  rfwcdma_core_util_get_lna_state( device );
  }

  /* Else check for second carrier on Sub 2*/
  else if ( ((device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__3].rfm_device) >= RFM_DEVICE_0) && 
            ((device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__3].rfm_device) < RFM_WCDMA_MAX_DEVICE) )
  {              
    param_ptr->agc_info_chunk_1.lna_state_ch1lna2 =  rfwcdma_core_util_get_lna_state( device );
  }
                  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* Populate bit fields from AGC Chunk 2 */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /* Use Tx Device from caller */
  device = intf_param->rfm_device_tx;
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {
    param_ptr->agc_info_chunk_2.pa_on = rfwcdma_core_util_get_rflm_tx_on();
    param_ptr->agc_info_chunk_2.pa_range = rfwcdma_core_util_get_rflm_pa_range();
    param_ptr->agc_info_chunk_2.tx_on = rfwcdma_core_util_get_rflm_tx_on();
    param_ptr->agc_info_chunk_2.txagc_frozen_ch0cr0 = 
                                   rfwcdma_mdsp_is_tx_accum_frozen(RFCOM_MULTI_CARRIER_ID__0);
  }
  
  #ifdef FEATURE_TABASCO_MODEM
  param_ptr->agc_info_chunk_2.txagc_frozen_ch0cr1 = 1;
  #else
  param_ptr->agc_info_chunk_2.txagc_frozen_ch0cr1 = rfwcdma_mdsp_is_tx_accum_frozen(RFCOM_MULTI_CARRIER_ID__1);
  #endif



  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* Populate bit fields from AGC Chunk 3 */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /* None for now */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* Populate bit fields from AGC Chunk 0 and Populate rest of the payload */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /* Logging for carrier idx 0, ant idx 0 */
  rxagc_input_info.device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].oper_mode;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
	  rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);   
    
     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
     
     param_ptr->agc_info_chunk_0.agc_status_ch0cr0 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain0_carr0 = rxagc_output_info.rxagc_val;
  }
  
  /* Logging for carrier idx 1, ant idx 0 */
  rxagc_input_info.device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__1].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__1].oper_mode;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
	  rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);      

     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__1;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch0cr1 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain0_carr1 = rxagc_output_info.rxagc_val;
  }
  
  /* Logging for carrier idx 2, ant idx 0 */
  rxagc_input_info.device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].oper_mode;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		   rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	


     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__2;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch0cr2 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain0_carr2 = rxagc_output_info.rxagc_val;
  }

#ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE

  /* Logging for carrier idx 3, ant idx 0 */
  rxagc_input_info.device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__3].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__3].oper_mode;

  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		   rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	

     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__3;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch0cr3 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain0_carr3 = rxagc_output_info.rxagc_val;
  }
#endif
 
  /* Logging for carrier idx 0, ant idx 1 */
  rxagc_input_info.device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__0].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__0].oper_mode;

  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		 rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	


     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch1cr0 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain1_carr0 = rxagc_output_info.rxagc_val;
  }
  
  /* Logging for carrier idx 1, ant idx 1 */
  rxagc_input_info.device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__1].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__1].oper_mode;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		 rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	

     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__1;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch1cr1 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain1_carr1 = rxagc_output_info.rxagc_val;
  }

  /* Logging for carrier idx 2, ant idx 1 */
  rxagc_input_info.device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__2].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__2].oper_mode;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	

		 
     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__2;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch1cr2 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain1_carr2 = rxagc_output_info.rxagc_val;
  }

#ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE

  /* Logging for carrier idx 3, ant idx 1 */
  rxagc_input_info.device = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__3].rfm_device;
  rxagc_input_info.oper_mode = intf_param->rfm_device_rxd[RFCOM_MULTI_CARRIER_ID__3].oper_mode;

  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
     rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
	 rxagc_input_info.band = (rxagc_input_info.oper_mode == RFM_WCDMA_STANDALONE_MODE)? 
		rfwcdma_data_get_curr_band(rxagc_input_info.device): rfwcdma_data_get_meas_curr_band(rxagc_input_info.device);	


     rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__3;
     rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
     rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;

     rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);

     param_ptr->agc_info_chunk_0.agc_status_ch1cr3 = rxagc_output_info.rxagc_on;
     param_ptr->rx_agc_chain1_carr3 = rxagc_output_info.rxagc_val;
  }
#endif 


  /* Use Tx Device from caller */
  device = intf_param->rfm_device_tx;
  if ((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
  {
    param_ptr->tx_agc_chain0_carr0 = rfwcdma_core_util_get_rflm_txagc_dB10();
    param_ptr->tx_agc_chain0_carr1 = 0;
    param_ptr->tx_agc_total = param_ptr->tx_agc_chain0_carr0;

    if(param_ptr->tx_agc_total <= RFWCDMA_LOG_TX_AGC_THRESHOLD)
    {
      param_ptr->agc_info_chunk_2.pa_on = FALSE;
      param_ptr->agc_info_chunk_2.tx_on = FALSE;
    }
       
    rfwcdma_core_util_get_rflm_txagc_logging(&txagc_data);
    param_ptr->tx_agc_lut_idx = txagc_data.rgi;
    param_ptr->tx_pwr_lim = rfwcdma_mdsp_get_tx_power_limit();
    param_ptr->hdet = rfwcdma_core_txplim_get_hdet();
    param_ptr->iq_gain = txagc_data.iq_gain;
    param_ptr->dpd_index = txagc_data.dpd_index;
    param_ptr->env_scale = txagc_data.env_scale;
    param_ptr->pa_current = txagc_data.icq_current;
    param_ptr->pa_bias = txagc_data.smps_bias;
  }

  #ifdef FEATURE_RF_ASDIV
  // First check for first Carrier on Sub 1
  if ( (intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].rfm_device>= RFM_DEVICE_0) && 
       (intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].rfm_device< RFM_WCDMA_MAX_DEVICE) )
  {
    device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__0].rfm_device; 
  }

  // Else check for first Carrier on Sub 2
  else  if ( (intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device>= RFM_DEVICE_0) && 
             (intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device< RFM_WCDMA_MAX_DEVICE) )
  {
    device = intf_param->rfm_device_prx[RFCOM_MULTI_CARRIER_ID__2].rfm_device; 
  }

  else
  {
    device = RFM_INVALID_DEVICE;
  }

  if (rfcommon_asdiv_is_switch_in_progress() == FALSE)
  {
    param_ptr->antenna_position = rfcommon_asdiv_get_current_position(device);
  }
  else
  {
    param_ptr->antenna_position = rfcommon_asdiv_get_previous_position(device);
  }

  #else
  param_ptr->antenna_position = 0;
  #endif /* FEATURE_RF_ASDIV */
  param_ptr->xpt_mode = txagc_data.xpt_mode;
  param_ptr->pin_comp = txagc_data.pin_comp;
  param_ptr->pout_comp = txagc_data.pout_comp;
  param_ptr->pin_rgi = 0xFF;
  param_ptr->delay = txagc_data.xpt_delay;

  /* Logging debug mode */
  if ( debug_mode == TRUE )
  {
    RF_MSG_9( RF_HIGH, "rfwcdma_core_log_get_agc_param: "
              "[ AGC Status %d, LNA State %d, PA On %d ] "
              "[ RxAGC ch0cr0 %d, RxAGC ch0cr1 %d, RxAGC ch1cr0 %d, "
              "RxAGC ch1cr1 %d ] "
              "[ TxAGC ch0cr0 %d, HDET %d ]",
              param_ptr->agc_info_chunk_0.agc_status_ch0cr0,
              param_ptr->agc_info_chunk_1.lna_state_ch0lna1,
              param_ptr->agc_info_chunk_2.pa_on,
              param_ptr->rx_agc_chain0_carr0,
              param_ptr->rx_agc_chain0_carr1,
              param_ptr->rx_agc_chain1_carr0,
              param_ptr->rx_agc_chain1_carr1,
              param_ptr->tx_agc_chain0_carr0,
              param_ptr->hdet );
  } /* if ( debug_mode == TRUE ) */

  return;
} /* rfwcdma_core_log_get_agc_param */

/*! @} */

/*============================================================================*/
/*!
  @addtogroup RFWCDMA_LOG_AGC_MC_INTERFACE
 
  @brief
  This module contains the interfaces exported to perform AGC logging
  @{
*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform AGC logging.

  @details
  This function does one sample of AGC logging. This function is called by WL1 every frame (10ms).
  Based on the parameter, this function can do buffered logging or one-log-and-flush logging. In buffered logging,
  each time when the function is called, the log will be buffered. When the buffer (which is of size 50 samples) is full,
  this function automatically flushes the log packet to diag. In one-log-and-flush logging, existing unflushed buffer will be flushed,
  then one sample of AGC will be taken and flushed.
  
  @Parameters
    log_handle	AGC log handle on which the AGC is to be sampled.
    intf_param	Pointer to the structure containing interface params. The content of this structure is expected to be valid till the end of the execution function.

  @Returns
    Status AGC logging 

*/

boolean
rfwcdma_core_log_agc_data
( 
  rfm_wcdma_log_agc_handle_type log_handle,
  rfm_wcdma_log_agc_param_type *intf_param  
)
{
  rfwcdma_log_agc_type* log_data = NULL; /* Pointer to AGC Log */
  boolean ret_val = TRUE;
  sys_modem_as_id_e_type sub_id = SYS_MODEM_AS_ID_MAX;
  uint32 diag_sub_id = (uint32)(sub_id + 1);

  /* Get Pointer to log data */
  log_data = get_agc_log_data();

  /* NULL pointer check */
  if ( log_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_agc_data: NULL log Data" );
    return FALSE;
  } /* if ( log_data == NULL ) */

  if ( intf_param == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_agc_data : NULL pointer for intf_param" );
    return FALSE;
  } /* if ( param_ptr == NULL ) */


  if (log_data->agc_packet[log_handle] == NULL)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_log_agc_data: No memory for AGC Data packet for Log Handle %d",
             log_handle);
    return FALSE;
  }

  if (log_data->agc_packet[log_handle]->log_packet == NULL)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_log_agc_data: No memory for AGC Log packet (Header + Sub Log) for Log Handle %d",
             log_handle);
    return FALSE;
  }

  /* Check if the log is enabled */
  if( is_agc_log_enabled(log_data) )
  {
    /* Log until the maximum instance of AGC before flushing it to diag */
    if ( log_data->agc_packet[log_handle]->counter < RFWCDMA_LOG_AGC_SAMPLES )
    {
      sub_id = log_data->agc_packet[log_handle]->w_sub;

      /* Flush Log Packet if Single Instance is RUE */
      if ( ( log_data->agc_packet[log_handle]->counter > 0 ) && ( intf_param->single_instance == TRUE ) )
      {
        ret_val &= flush_agc_log( sub_id,
                                  log_data->agc_packet[log_handle]);
      } /* if ( ( log_instance->counter > 0 ) && ( intf_param->single_instance == TRUE ) ) */

      /* For the first AGC Log instance, populate the header with log
      timestamp and the frame number */
      if ( log_data->agc_packet[log_handle]->counter == 0 )
      {
        /* Set Time Stamp */
        log_set_timestamp_ext(&log_data->agc_packet[log_handle]->log_packet, 1, &diag_sub_id);

        /* Get the CFN for the Sub ID */
        log_data->agc_packet[log_handle]->log_packet->agc_sub_pkt.start_cfn = wl1_seq_get_cfn((wsub_id_e_type)sub_id);
      } /* if ( rfm_wcdma_agc_log_count == 0 ) */

      /* Fetch the AGC params */
      rfwcdma_core_log_get_agc_param( 
        &log_data->agc_packet[log_handle]->log_packet->agc_sub_pkt.agc_params[log_data->agc_packet[log_handle]->counter],
        FALSE,intf_param);
  
      /* Increment AGC log counter */
      log_data->agc_packet[log_handle]->counter ++;
    } /* if ( rfm_wcdma_agc_log_count < RFM_WCDMA_AGC_LOG_SAMPLES ) */

    /* For case, when the AGC log instance reaches maximum OR Single instance is true, flush the log
    to diag */
    if ( (log_data->agc_packet[log_handle]->counter == RFWCDMA_LOG_AGC_SAMPLES) ||
         (intf_param->single_instance == TRUE)
        )
    {
      sub_id = log_data->agc_packet[log_handle]->w_sub;

      ret_val &= flush_agc_log(sub_id,
                               log_data->agc_packet[log_handle]);
    } /*     if ( (log_data->agc_packet[log_handle]->counter == RFWCDMA_LOG_AGC_SAMPLES) ||
         (intf_param->single_instance == TRUE)
        ) */
  } /* if( is_agc_log_enabled(log_data) ) */

  return ret_val;

} /* rfwcdma_core_log_agc_data */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Flush AGC Log

  @details
  This function will flush the AGC log data buffered till now
*/ 
boolean 
rfwcdma_core_log_agc_data_flush
( 
  rfm_wcdma_log_agc_handle_type log_handle
)
{
  rfwcdma_log_agc_type *log_data = NULL; /* Pointer to AGC Log */
  sys_modem_as_id_e_type sub_id = SYS_MODEM_AS_ID_MAX;
  boolean ret_val = TRUE;

  /* Get Pointer to log data */
  log_data = get_agc_log_data();

  /* NULL pointer check */
  if ( log_data == NULL )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_agc_data_flush: NULL log Data" );
    return FALSE;
  } /* if ( log_data == NULL ) */

   /* Param Validation */
  if ( log_handle > RFM_WCDMA_LOG_AGC_MAX_HANDLE )
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_log_agc_data_flush: Bad Handle" );
    return FALSE;
  } /*   if ( log_handle >= RF_WCDMA_LOG_AGC_HANDLE_MAX ) */


  if (log_data->agc_packet[log_handle] == NULL)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_log_agc_data_flush: No memory for AGC Data packet for Log Handle %d",
             log_handle);
    return FALSE;
  }

  if (log_data->agc_packet[log_handle]->log_packet == NULL)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_log_agc_data_flush: No memory for AGC Log packet (Header + Sub Log) for Log Handle %d",
             log_handle);
    return FALSE;
  }

  /* Check if the log is enabled */
  if( is_agc_log_enabled(log_data) )
  {
    /* Flush Log Packet if counter > 0 */
    if (log_data->agc_packet[log_handle]->counter > 0)
    {
      sub_id = log_data->agc_packet[log_handle]->w_sub;

      ret_val &= flush_agc_log(sub_id,
                               log_data->agc_packet[log_handle]);
    }

  } /* if( is_agc_log_enabled(log_data) ) */

  return ret_val;

} /* rfwcdma_core_log_agc_data_flush */

/*! @} */

/*============================================================================*/
/*!
  @addtogroup RFWCDMA_LOG_COMMON
 
  @brief
  This module exports common WCDMA log interface
  @{
*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Initialize WCDMA logging

  @details
  This function will called during WCDMA initialization to perform actions
  necessary to prepare WCDMA for logging.
 
  @return
  Flag indicating the status of log initialization
*/
boolean 
rfwcdma_log_init
(
  void
)
{
  boolean ret_val = TRUE;

  /* Initialize AGC log */
  ret_val &= init_agc_log();

  return ret_val;

} /* rfwcdma_log_init */

/*! @} */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is the handler for the command RFLM_WCDMA_NPA_ISSUE_CMD. 
  
  @details
  Calls the function npa_issue_required_request. 
 
*/

void
rflm_wcdma_npa_issue_cmd_handler
(
  void* cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data 
)
{
#if (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)

  rflm_wcdma_npa_issue_payload_type* cmd_data = NULL;
  
  npa_client_handle client;
  npa_resource_state state;
   
  /* Extract the payload information form the cmd ptr*/
  cmd_data = (rflm_wcdma_npa_issue_payload_type *)((rf_cmd_type*)cmd_ptr)->payload;

  client = cmd_data->client;
  state = cmd_data->state;

  npa_issue_required_request(client, state);
  
#endif
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is the handler for the command RFLM_WCDMA_NPA_CANCEL_CMD. 
  
  @details
  Calls the function npa_cancel_request. 
 
*/

void
rflm_wcdma_npa_cancel_cmd_handler
(
  void* cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data 
)
{
#if (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)

  rflm_wcdma_npa_cancel_payload_type* cmd_data = NULL;
  npa_client_handle client;
     
  /* Extract the payload information form the cmd ptr*/
  cmd_data = (rflm_wcdma_npa_cancel_payload_type *)((rf_cmd_type*)cmd_ptr)->payload;

  client = cmd_data->client;
  
  npa_cancel_request(client);
  
#endif
}

/*--------------------------------------------------------------------------*/
