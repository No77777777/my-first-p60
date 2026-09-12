#ifndef TDSMAC_DL_HS_H
#define TDSMAC_DL_HS_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              D O W N L I N K   M A C   H S D P A   H E A D E R    F I L E

DESCRIPTION
   MAC HS specific type declarations.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdlhs.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who      what, where, why
--------   -------- --------------------------------------------------------
10/24/10   guo      Initial release Upmerged WCDMA to TDS
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "tdsmacrrcif.h"
#include "tdsmacinternal.h"
#include "rex.h"
/*===========================================================================

                     DEFINES AND MACROS FOR MODULE

===========================================================================*/

#define TDSMAC_HS_DL_MIN_MAC_D_FLOWS                  1
#define TDSMAC_HS_DL_MAX_MAC_D_FLOWS                  UE_MAX_MAC_D_FLOW
#define TDSMAC_HS_DL_MIN_MAC_D_FLOW_ID                0
#define TDSMAC_HS_DL_MAX_MAC_D_FLOW_ID                (UE_MAX_MAC_D_FLOW - 1)
#define TDSMAC_HS_DL_MIN_QUEUES                       1
#define TDSMAC_HS_DL_MAX_QUEUES                       UE_MAX_HS_QUEUE
#define TDSMAC_HS_DL_MIN_LC_PER_FLOW                  1
#define TDSMAC_HS_DL_MAX_LC_PER_FLOW                  UE_MAX_LOGCHAN_PER_DFLOW
#define TDSMAC_HS_DL_MIN_QUEUE_ID                     0
#define TDSMAC_HS_DL_MAX_QUEUE_ID                     (UE_MAX_HS_QUEUE - 1)
#define TDSMAC_HS_DL_MIN_SIDS                         1
#define TDSMAC_HS_DL_MAX_SIDS                         UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE
#define TDSMAC_HS_DL_MIN_PDU_SIZE                      1
#define TDSMAC_HS_DL_MAX_PDU_SIZE                     5000
#define TDSMAC_HS_DL_MIN_PDU_SIZE_INDEX               0
#define TDSMAC_HS_DL_MAX_PDU_SIZE_INDEX               (UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE - 1)
#define TDSMAC_HS_DL_MAX_MAC_D_PDUS_PER_SUB_FRAME     TDSUE_MAX_DL_RLC_PDUS
#define TDSMAC_EHS_DL_MAX_PDUS_PER_SUB_FRAME          26
#define TDSMAC_HS_DL_MAX_WINDOW_SIZE                  64
#define TDSMAC_HS_DL_MAX_TSN                          (TDSMAC_HS_DL_MAX_WINDOW_SIZE - 1)
#define TDSMAC_HS_DL_MIN_REL_TIMER_VAL                10
#define TDSMAC_HS_DL_MAX_REL_TIMER_VAL                400
#define TDSMAC_HS_DL_UNDEF_TSN                        0xFF
#define TDSMAC_HS_MAX_NUM_TB_SET_PER_10_MS_FRAME      32
#define TDSMAC_HS_DL_MAX_TSN_PER_10_MS_FRAME          (32 - TDSUE_MAX_TRCH)

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
#define TDSMAC_EHS_DL_MAX_TSN_PER_10_MS_FRAME         3
#endif

#define TDSINVALID_HS_QUEUE_ID                        8

#define TDSMAC_HS_MAX_SAVED_TSNS_DURING_RECONFIG      32

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#define TDSMAC_HS_DL_STATUS_LOG_TIMER_VAL             1000
/*  Store the information for 50 TSNs */
#define TDSMAC_EHS_NO_OF_TSN_LOG          50
/*  In one TSN network can send maximum of 26 puds*/
/* Arun- the logging is also done in MACHS, putting it to 26 causes outofbound access and causes crash*/
#define TDSMAC_EHS_NO_OF_PDU_IN_TSN_LOG           70
/*  In one TSN network can send maximum of 2 partial pdus*/
#define TDSMAC_EHS_NO_OF_PART_PDU_IN_TSN_LOG      2
#endif


/*===========================================================================

                     KEY MACROS USED IN WINDOW PROCESSING IN MAC-HS

===========================================================================*/

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is with in the window (T2<lower edge>,T3<upper edge>)
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_INSIDE_WIN(t1, t2, t3) \
         TDSMAC_HS_IS_TSN_LATER_OR_EQ(t1, t2) && TDSMAC_HS_IS_TSN_BEFORE_OR_EQ(t1, t3)

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is outside the window (T2<lower edge>,T3<upper edge>)
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_OUTSIDE_WIN(t1, t2, t3) \
         TDSMAC_HS_IS_TSN_BEFORE(t1, t2) || TDSMAC_HS_IS_TSN_LATER(t1, t3)

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is later than (>) T2
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_LATER(t1, t2)                                   \
      (((t1) > (t2)) ?                                                \
      (((t1) - (t2)) < TDSMAC_HS_DL_MAX_WINDOW_SIZE/2) :                 \
      (((t2) - (t1)) > TDSMAC_HS_DL_MAX_WINDOW_SIZE/2))

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is later than or equal to (>=) T2
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_LATER_OR_EQ(t1, t2)                             \
      (TDSMAC_HS_IS_TSN_LATER(t1, t2) || (t1 == t2) )

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is befire than (<) T2
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_BEFORE(t1, t2)                                  \
      (((t1) >= (t2)) ?                                               \
      (((t1) - (t2)) > TDSMAC_HS_DL_MAX_WINDOW_SIZE/2):                  \
      (((t2) - (t1)) < TDSMAC_HS_DL_MAX_WINDOW_SIZE/2))

/*--------------------------------------------------------------------------
 * Routine that indicates if T1 is before than or equal to (<=) T2
 *-------------------------------------------------------------------------*/
#define TDSMAC_HS_IS_TSN_BEFORE_OR_EQ(t1, t2)                            \
      (TDSMAC_HS_IS_TSN_BEFORE(t1, t2) || (t1 == t2) )

/*--------------------------------------------------------------------------
 * Routines to advance a TSN by a given amount
 *-------------------------------------------------------------------------*/
#define TDSADD_MOD( a, b, m ) \
      ( ( (a) + (b) < (m) ) ? ( (a) + (b) ) : ( (a) + (b) - (m) ) )

/* Routine to Add TSNs */
#define TDSADD_TSN( a, b ) TDSADD_MOD( (a), (b), TDSMAC_HS_DL_MAX_WINDOW_SIZE )

/*--------------------------------------------------------------------------
 * Routines to decrement a TSN by a given amount
 *-------------------------------------------------------------------------*/
#define TDSSUB_MOD( a, b, m ) \
      ( ( (a) >= (b) ) ? ( (a) - (b) ) : ( (m) + (a) - (b) ) )

/* Routine to subtract TSNs */
#define TDSSUB_TSN( a, b ) TDSSUB_MOD( (a), (b), TDSMAC_HS_DL_MAX_WINDOW_SIZE )


/*===========================================================================

                     GENERAL PURPOSE MACROS

===========================================================================*/

#define TDSMAC_HS_DL_QUEUE_ID_IS_INVALID(queue_id)\
      (queue_id > TDSMAC_HS_DL_MAX_QUEUE_ID)

#define TDSMAC_HS_DL_TSN_IS_INVALID(tsn)\
      (tsn > TDSMAC_HS_DL_MAX_TSN)

#define TDSMAC_HS_DL_NUM_MAC_D_FLOW_IS_INVALID(num_flows)\
      ((num_flows < TDSMAC_HS_DL_MIN_MAC_D_FLOWS) || \
       (num_flows > TDSMAC_HS_DL_MAX_MAC_D_FLOWS))

#define TDSMAC_HS_DL_MAC_D_FLOW_ID_IS_INVALID(flow_id)\
       (flow_id > TDSMAC_HS_DL_MAX_MAC_D_FLOW_ID)

#define TDSMAC_HS_DL_NUM_QUEUE_IS_INVALID(num_queues)\
       (num_queues > TDSMAC_HS_DL_MAX_QUEUES)

#define TDSMAC_HS_DL_NUM_LC_PER_FLOW_IS_INVALID(tdsnum_dlc)\
       ((tdsnum_dlc < TDSMAC_HS_DL_MIN_LC_PER_FLOW) || \
        (tdsnum_dlc > TDSMAC_HS_DL_MAX_LC_PER_FLOW))

#define TDSMAC_HS_DL_NUM_SID_IS_INVALID(num_sids)\
       ((num_sids < TDSMAC_HS_DL_MIN_SIDS) || \
        (num_sids > TDSMAC_HS_DL_MAX_SIDS))

#define TDSMAC_HS_DL_PDU_SIZE_IS_INVALID(pdu_size)\
        ((pdu_size < TDSMAC_HS_DL_MIN_PDU_SIZE) || \
         (pdu_size > TDSMAC_HS_DL_MAX_PDU_SIZE))

#define TDSMAC_HS_DL_PDU_SIZE_INDEX_IS_INVALID(pdu_size_index)\
         (pdu_size_index > TDSMAC_HS_DL_MAX_PDU_SIZE_INDEX)

#define TDSMAC_HS_DL_NUM_TB_PER_TSN_IS_INVALID(num_tb)\
         (num_tb > TDSMAC_HS_DL_MAX_MAC_D_PDUS_PER_SUB_FRAME)

#define TDSMAC_HS_DL_NUM_BLKS_PER_SID_IS_INVALID(num_blks)\
         (num_blks > TDSMAC_HS_DL_MAX_MAC_D_PDUS_PER_SUB_FRAME)

#define TDSMAC_HS_DL_REL_TIMER_IS_INVALID(timer_val)\
        ((timer_val < TDSMAC_HS_DL_MIN_REL_TIMER_VAL) || \
         (timer_val > TDSMAC_HS_DL_MAX_REL_TIMER_VAL))

#define TDSMAC_HS_DL_WINDOW_SIZE_IS_INVALID(size)\
         (size > TDSMAC_HS_DL_MAX_WINDOW_SIZE)

#define TDSMAC_HS_IS_DATA_RECEIVED(data_received, int_queue_ptr, tsn)  \
       data_received = (((int_queue_ptr->tsn_info[tsn] != NULL) &&  \
                         (int_queue_ptr->tsn_info[tsn]->received == TRUE)) ? TRUE : FALSE)

#define TDSMAC_HS_GET_TSN_PTR(int_queue_ptr, tsn, tsn_info_ptr) \
        tsn_info_ptr = int_queue_ptr->tsn_info[tsn] 

#define TDSMAC_HS_PUT_TSN_PTR(int_queue_ptr, tsn, tsn_info_ptr) \
        int_queue_ptr->tsn_info[tsn] = tsn_info_ptr 

#define TDSMAC_HS_GET_SAVED_TSN_PTR(tdsmac_hs_saved_tsn_data, i, tsn_info_ptr) \
        tsn_info_ptr = tdsmac_hs_saved_tsn_data[i].tsn_info 

#define TDSMAC_HS_PUT_SAVED_TSN_PTR(tdsmac_hs_saved_tsn_data, i, tsn_info_ptr) \
        tdsmac_hs_saved_tsn_data[i].tsn_info = tsn_info_ptr 

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
#define TDSMAC_EHS_DL_NUM_TB_PER_TSN_IS_INVALID(num_tb)\
         (num_tb > TDSMAC_EHS_DL_MAX_PDUS_PER_SUB_FRAME)
#endif /* defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS) */



#define TDSMAC_HS_GET_REORDERING_QUEUE_PTR(int_queue_ptr,queue_id) \
         int_queue_ptr = tdsmac_hs_reordering_entity.reordering_queue[queue_id]

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

#define TDSHSPA_PLUS_MAC_HS_TYPE_VALID_CHECK(tdsmac_hs_or_ehs) \
\
   if(tdsmac_hs_or_ehs == MAC_HS_TYPE_INVALID) {  \
\
     TDSMAC_MSG3(MSG_LEGACY_ERROR,"EHS_ERR: MAC_HS_TYPE is Invalid ",0,0,0);   \
\
   }  \

#endif

#define TDSIS_TSN_NOT_WITHIN_BUF(tsn_info_ptr)  ((tsn_info_ptr < &mcaltds_dec_tsn_info_buf[0]) || \
				( tsn_info_ptr > &mcaltds_dec_tsn_info_buf[MCALTDS_DEC_MAX_TSN_INFO_BUF - 1]))

/*===========================================================================

                     TYPE DEFINITIONS FOR MODULE

===========================================================================*/

/*--------------------------------------------------------------------------
 * Data type to store HS timer information
 *-------------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
typedef struct
{
   /* Timer value */
   uint16                              release_timer_val;
   uint16                              release_timer_opt_val;//The value of release_timer after optimization

   /* The TSN associated with this timer.*/
   uint8                               tsn;

   /* Timer block associated with this timer */
   rex_timer_type                      timer_blk;
   boolean                             release_timer_opt_done;//Release_timer optimization is done
} tdsmac_hs_timer_info_type;
#else
typedef struct
{
   /* Timer value */
   uint16                              release_timer_val;

   /* The TSN associated with this timer.*/
   uint8                               tsn;

   /* Timer block associated with this timer */
   rex_timer_type                      timer_blk;
} tdsmac_hs_timer_info_type;
#endif

/*--------------------------------------------------------------------------
 * Data type to store HS window information
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* The lower edge of the window.*/
   uint8                               lower_edge;

   /* The upper edge of the window.*/
   uint8                               upper_edge;

   /* window size as configured by RRC */
   uint8                               window_size;
}tdsmac_hs_window_info_type;


/*--------------------------------------------------------------------------
 * Data type to store mac-d pdu that belongs to a TSN
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* True indicates that this pdu is valid */
   boolean                             valid;

   /* DL RLC logical channel ID */
   rlc_lc_id_type                      rlc_id;

   /* Actual pdu data */
   dsm_item_type                       *pdu_ptr;
}tdsmac_hs_mac_d_pdu_info_type;


/*--------------------------------------------------------------------------
 * Data Type to store the size indicator information for mac-d pdus.
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Actual value of the Size Indicator.*/
   uint8                               sid_val;

   /* Number of PDUs of the same size */
   uint8                               num_blks;

   /* Pdu size */
   uint16                              pdu_size;
}tdsmac_hs_sid_info_type;


/*--------------------------------------------------------------------------
 * Type to store TSN specific information
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Sequence Number. */
   uint8                               tsn;

   /* TSN is processed and shipped to RLC. */
   boolean                             processed;

   /* TSN is received and sitting in the re-ordering queue.*/
   boolean                             received;

   /* Number of size indicators received for this TSN */
   uint8                               num_sids;

   /* Data pertaining to the SIDs received for this TSN */
   tdsmac_hs_sid_info_type                sid_data[TDSMAC_HS_DL_MAX_SIDS];

   /* This is an array of size 70 as there can be 70 mac-d pdus per MAC-HS pdu.*/
   tdsmac_hs_mac_d_pdu_info_type          pdu_data[TDSMAC_HS_DL_MAX_MAC_D_PDUS_PER_SUB_FRAME];
}tdsmac_hs_tsn_info_type;


typedef struct
{
  /* Queue ID. */
  uint8                               queue_id;

  /* Information about the timer associated with this queue */
  tdsmac_hs_timer_info_type              timer_info;

  /* Information about the window associated with this queue */
  tdsmac_hs_window_info_type             window_info;

  /* Number of valid pdu sizes for this queue as configured by RRC. */
  uint8                               num_valid_pdu_sizes;

  /* mac-d pdu size information valid for this queue as configured by RRC. */
  tdsmac_hs_pdu_info_struct_type         valid_pdu_sizes[TDSMAC_HS_DL_MAX_SIDS];

  /* Mac-d flow information mapped to this queue as configured by RRC.*/
  tdsdl_dch_config_info_type             mac_d_flow_info;
  
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  /* Mac-enhanced/HSPA+ flow information mapped to this queue as configured by RRC.*/
  tdsdl_dch_config_info_type             mac_enh_flow_info;
#endif
} tdsmac_hs_queue_config_info_type;


/*--------------------------------------------------------------------------
 * Information pertaining to a re-ordering queue.
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Indicate whether this re-ordering entity is enabled or not.*/
   boolean                             enabled;

   /* Queue ID. */
   uint8                               queue_id;

   /* Next Expected TSN  */
   uint8                               next_expected_tsn;
   /* Saved next expected TSNs */
   uint8                               saved_next_expected_tsn;
   /* Number of TSN received in last 10 ms TTI */
   uint8                               curr_num_tsn_rcvd;

   /* List of TSNs received in last 10 ms TTI */
   uint8                               curr_rcvd_tsn[5*TDSMAC_HS_DL_MAX_TSN_PER_10_MS_FRAME];

   /* Number of TSN received in last 10 ms TTI */
   uint8                               new_num_tsn_rcvd;

   /* List of TSNs received in last 10 ms TTI */
   uint8                               new_rcvd_tsn[5*TDSMAC_HS_DL_MAX_TSN_PER_10_MS_FRAME];
 /* Number of TSN received in last 10 ms TTI */
   uint8                               new_num_tsn_rcvd_temp;

   /* List of TSNs received in last 10 ms TTI, temporary array to copy TSN info in one shot in ISR */
   uint8                               new_rcvd_tsn_temp[5*TDSMAC_HS_DL_MAX_TSN_PER_10_MS_FRAME];

   /* Information about the timer associated with this queue */
   tdsmac_hs_timer_info_type              timer_info;

   /* Information about the window associated with this queue */
   tdsmac_hs_window_info_type             window_info;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
   /* Mac-enhanced/HSPA+ flow information mapped to this queue as configured by RRC.*/
   tdsdl_dch_config_info_type             mac_enh_flow_info;
#endif

   /* Number of valid pdu sizes for this queue as configured by RRC. */
   uint8                               num_valid_pdu_sizes;

   /* mac-d pdu size information valid for this queue as configured by RRC. */
   tdsmac_hs_pdu_info_struct_type         valid_pdu_sizes[TDSMAC_HS_DL_MAX_SIDS];

   /* Mac-d flow information mapped to this queue as configured by RRC.*/
   tdsdl_dch_config_info_type             mac_d_flow_info;

   /* TSN information stored for this queue.*/
   tdsl1_dec_hs_tsn_info_type            *tsn_info[TDSMAC_HS_DL_MAX_WINDOW_SIZE];
} tdsmac_hs_queue_info_type;


/*--------------------------------------------------------------------------
 * Data type to hold all the re-odering queues.
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Number of queues as configured by RRC */
   uint8                              num_queues;

   /* cctrch_id for this configuration */
   cctrch_id_type                     cctrch_id;

/*---------------------------------------------------------------------------
 *In DL data ISR received TSNs are put into this queue; MAC-hs dequeue
 *TSN items from this queue in task context and put them into reordering buffer.
--------------------------------------------------------------------------*/

   /* queue information (both configuration and data). */
   tdsmac_hs_queue_info_type             *reordering_queue[TDSMAC_HS_DL_MAX_QUEUES];
}tdsmac_hs_reordering_entity_type;

/*--------------------------------------------------------------------------
 * Data type to hold the list of tsn and queue_id received in last 10 ms TTI
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* The queue Id to which this TSN belongs. */
   uint8                              queue_id;

   /* The TSN received in the header information. */
   uint8                              tsn;
   /* cctrch_id for this configuration */
   cctrch_id_type                     cctrch_id;

   /* index into the holding buffer */
   uint8                              saved_tsn_idx;

}tdsmac_hs_tsn_to_queue_mapping_info_type;

/*--------------------------------------------------------------------------
 * Data type to store the operation to be performed during config/reconfig
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Save non-hs releated parameters if set */
   boolean save_non_hs_params;

   /* Save hs releated parameters if set */
   boolean save_hs_params;

   /* Perform reset */
   boolean perform_hs_reset;
   /* Restore the saved TSNs */
   boolean perform_restore_tsns;
   /* Perform Flush Only*/
   boolean perform_hs_flush_only;
}tdsmac_hs_dl_operation_type;


/*--------------------------------------------------------------------------
 * Data type used to store the necessary information to continue configuration
 * in MAC-HS task context. The configuration from RRC is received in L2 DL
 * Task context.
 *-------------------------------------------------------------------------*/
typedef struct
{
   /* Num of mac_dflows received from RRC */
   uint8    num_dflow;

   /* Action to be performed */
   tdshsdpa_action_enum_type   action;

   /* Operation to be performed */
   tdsmac_hs_dl_operation_type operation;

   /* List of mac_dflow entities configured */
   tdsmac_dflow_struct_type *hsdpa_info[UE_MAX_MAC_D_FLOW];

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

   mac_hs_e_type mac_hs_or_ehs;

   /*Informs transition between HS and EHS, if present */
   tdsmac_hs_transition_e_type mac_hs_transition_type;

  /* Number of dedicated RLC logical channels mapped */
   uint8       ndlchan_macehs;

   /* Info of dedicated logical channels mapped */
   tdsmac_dl_ehs_logch_config_type * dl_macehs_logchan_info[TDSUE_MAX_DL_LOGICAL_CHANNEL];
   
   uint8 num_mac_ehs_queue;
   
   tdsmac_ehs_queue_struct_type *hsdpa_ehs_queue_info[UE_MAX_HS_QUEUE];
#endif  /* FEATURE_TDSCDMA_HSDPA_MAC_EHS  */

}tdsmac_hs_dl_config_req_type;



typedef struct
{
   /* Queue ID. */
   uint8                               queue_id;

   /* TSN information stored for this queue.*/
   tdsl1_dec_hs_tsn_info_type            *tsn_info;

} tdsmac_hs_saved_tsns_during_reconfig_type;



/*--------------------------------------------------------------------------
 * Enum Type for indicating the various stated MAC-HS can be
 *-------------------------------------------------------------------------*/
typedef enum
{
   TDSMAC_HS_INIT,
   TDSMAC_HS_ACTIVE,
   TDSMAC_HS_INACTIVE,
   TDSMAC_HS_CHECK_RESET,
   TDSMAC_HS_INVALID
}tdsmac_hs_dl_state_type;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
/*--------------------------------------------------------------------------
 *  strcture types used for logging 0x421E 
 *-------------------------------------------------------------------------*/

typedef struct
{
  uint8 queue_lc_id;
  uint16 pdu_size;
}tdsmac_ehs_complete_pdu_log_info_type;

typedef struct
{
  tdsl1_dec_rlc_pdu_segmentation_type segmentation_info;
  uint8 queue_lc_id;
  uint16 pdu_size;
}tdsmac_ehs_partial_pdu_log_info_type;

typedef struct
{
  uint8 tsn;
  uint8 num_complete_pdus;
  tdsmac_ehs_complete_pdu_log_info_type complete_pdu_info[TDSMAC_EHS_NO_OF_PDU_IN_TSN_LOG];
  uint8 num_partial_pdus;
  tdsmac_ehs_partial_pdu_log_info_type partial_pdu_info[TDSMAC_EHS_NO_OF_PART_PDU_IN_TSN_LOG];
}tdsmac_ehs_log_tsn_buf_type;
#endif /* FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING */

/*--------------------------------------------------------------------------
 * Enum Type for indicating whether partial PDU combination is valid or Invalid
 *-------------------------------------------------------------------------*/
typedef enum
{
   TDSPARTIAL_COMBO_INVALID,
   TDSPARTIAL_COMBO_VALID
}tdspartial_valid_type;

extern mac_hs_e_type tdsmac_hs_or_ehs;
#endif

/*===========================================================================

                         GLOBAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION: tdsprocess_dl_hs_dsch_tbs_hdr

DESCRIPTION:
  This function processes the MAC-HS pdu header received from L1.
  This function executes in the ISR context. In addition to validating
  the header information, it fills in the ciphering parameters and
  sets the transport block id for each transport block (mac-d pdu)
  received as part of the transport block set (MAC-HS pdu).

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsprocess_dl_hs_dsch_tbs_hdr(
   uint8 tbs_id,
   uint8 tsn_to_queue_map_idx,
   tdsl1_dl_tb_set_hdr_struct_type  *tbs_hdr_ptr
   );

/*===========================================================================
FUNCTION: tdsprocess_dl_hs_dsch_tbs_data

DESCRIPTION:
  This function handles the data received from L1, one MAC-HS pdu at a time.
  This function is executed in the ISR context. If the data is correctly
  received, mac-d pdus are stored locally depending upon the queue to which
  they belong. The window/timer processing then delivers the mac-d pdus to
  RLC appropriately.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsprocess_dl_hs_dsch_tbs_data(
   uint8 tbs_id,
   uint8 *mac_hs_num_tsn_rcvd,
   tdsl1_dl_tb_set_data_struct_type   *tbsptr
   );

#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*===========================================================================
FUNCTION: tdsmac_hs_dl_crit_sect_init

DESCRIPTION:
  This function performs initialization of critcal section for HS
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_hs_dl_crit_sect_init( void ); //triton error: add void as parameters
#endif

/*===========================================================================
FUNCTION: tdsmac_hs_dl_crit_sect_init

DESCRIPTION:
  This function performs initialization of critcal section for HS
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_rcinit_init( void );//triton error: Since code use this function, add declaration and add void as parameters for it

/*===========================================================================
FUNCTION: tdsmac_hs_dl_init

DESCRIPTION:
  This function performs the intialization of some global data and creates
  timers for all the reordering queues. This function is invoked when the
  mac-hs task is created.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_init( void );

/*===========================================================================
FUNCTION: tdsmac_hs_dl_data_handler

DESCRIPTION:
  This function executes in the mac-hs task context once it's signalled
  about the arrival of one or more MAC-HS PDUs from the ISR.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_data_handler( void );

/*===========================================================================
FUNCTION: tdsmac_hs_dl_timer_expiry_handler

DESCRIPTION:
  This function handle the timer expiry event for a given reordering queue.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_timer_expiry_handler(uint8 queue_id);

/*===========================================================================
FUNCTION: tdsmac_hs_dl_process_dsch_config

DESCRIPTION:
  This function handles the HS configuration (re-config) received from the
  RRC in mac-hs task context.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_process_dsch_config(
   uint8 num_mac_d_flow,
   tdshsdpa_action_enum_type   action,
   tdsmac_dflow_struct_type *hsdpa_info[]
   );

/*===========================================================================
FUNCTION: tdsmac_hs_queue_init

DESCRIPTION:
  This function performs the intialization of some global data and creates
  timers for all the configured reordering queues. This function is invoked after the
  mac-hs allocates the memory to the reordering queue.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

void tdsmac_hs_queue_init(uint8 queue_id);


/*===========================================================================
FUNCTION: tdsmac_hs_queue_free

DESCRIPTION:
  This function frees the  allocated memory to the reordering queue.


DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_hs_queue_free(uint8 queue_id);


/*===========================================================================
FUNCTION: tdsmac_hs_dl_init_ZI_opt

DESCRIPTION:
  This function is used for dynamic memory allocation to MAC_HS data structures


DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_hs_dl_init_ZI_opt( void );


#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)

/*===========================================================================
FUNCTION: mac_enhs_dl_process_dsch_config

DESCRIPTION:
  This function handles the En-HS configuration (re-config) received from the
  RRC in mac-hs task context.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_enhs_dl_process_dsch_config(void);

#endif

/*===========================================================================
FUNCTION: tdsprocess_dl_hs_dsch_config

DESCRIPTION:
  This function is called in the L2-DL task context upon receiving HS
  configuration from RRC. This function saves the necessary pointers given
  by RRC in the configuration and signals the MAC-HS task to pick up
  futher processing.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsprocess_dl_hs_dsch_config(tdsmac_dl_config_type *cfgptr);

/*===========================================================================
FUNCTION: mac_hs_dl_start_dsch_reset_req

DESCRIPTION:
  This function starts the hs reset processing. It delivers all the
  MAC-HS pdus within the window to RLC, stops any active timers, resets the
  window and next_expected_tsn and finally performs a flush.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_start_reset_req( void );

/*===========================================================================
FUNCTION: tdsmac_hs_dl_finish_reset_req

DESCRIPTION:
  This function completed the hs reset processing by sending a reset req
  to RLC with a list of rlc ids.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_finish_reset_req( void );

/*===========================================================================
FUNCTION: tdsmac_hs_dl_validate_config_req

DESCRIPTION:
  This function performs initial validation of HS configuration received
  from RRC. Specifically, the action type and the reset flag indicators are
  examined to see if they result in a correct state transition in MAC-HS.
  This functions tells us, whether we have have to perform any or all of the
  following; 1) perform hs reset; 2) save non-hs data; 3) save hs data.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_validate_config_req(tdsmac_dl_config_type    *cfgptr);


/*===========================================================================
FUNCTION: tdsmac_hs_dl_config_req_handler

DESCRIPTION:
  This function is the called when mac-hs task is signalled from the L2 DL
  task indicating the arrival of a new MAC HS configuration from RRC. The
  processing of MAC HS configuration is handled in the mac-hs task context.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_config_req_handler( void );

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
/*===========================================================================
FUNCTION: tdsmac_hs_dl_send_hs_config_log_packet

DESCRIPTION:
  This function sends the the HS config log packet.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_send_hs_config_log_packet(tdsmac_dl_config_type *cfgptr);

/*===========================================================================
FUNCTION: tdsmac_hs_dl_status_log_timer_expiry_handler

DESCRIPTION:
  This function sends the the HS config log packet.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
extern void tdsmac_hs_dl_status_log_timer_expiry_handler(void);

#endif /* FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING  */

extern void tdsmac_hs_dl_reset_local_config_data(void);
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*===========================================================================
FUNCTION: mac_enhs_dl_reset_config_info

DESCRIPTION:
  This is a local function to reset the global data structure that saves
  some of the details of the configuration received from RRC. This function
  is used during power up initialization and after the config/re-config has
  finished.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

void tdsmac_enhs_dl_reset_local_config_data(void);
#endif 


/*===========================================================================
FUNCTION: tdsprocess_dl_hs_enh_dsch_tbs_data

DESCRIPTION:
  This function handles the data received from L1, one MAC-HS TSN at a time.
  This function is executed in the ISR context. If the data is correctly
  received, TSN will be stored locally depending upon the queue to which
  they belong. If the TSN is already present and validation failed, PDUs present
  in this TSN will be freed and TSN will be added into TSN Free queue.
  The window/timer processing then delivers the TSN to RLC appropriately.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

void tdsprocess_dl_hs_enh_dsch_tbs_data(uint8 *mac_hs_num_tsn_rcvd);


#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
/*===========================================================================
FUNCTION: tdsprocess_dl_ehs_dsch_tbs_hdr

DESCRIPTION:
  This function processes the MAC-HS pdu header received from L1.
  This function executes in the ISR context. In addition to validating
  the header information, it fills in the ciphering parameters and
  sets the transport block id for each transport block (mac-d pdu)
  received as part of the transport block set (MAC-HS pdu).

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsprocess_dl_ehs_dsch_tbs_hdr(
   uint8 tbs_id,
   uint8 *tsn_to_queue_map_idx,
   tdsl1_dl_tb_set_hdr_struct_type *tbs_hdr_ptr
   );

/*===========================================================================
FUNCTION: tdsmac_ehs_provide_partial_to_complete_info

DESCRIPTION:
  Prepare the partial-to-complete PDUs possible from every queue. Max number of 
  partial-to-complete are limited by the allowed_length in total of the PDU
  sizes.
  MAC also provides the ciphering parameters for each Partial-to-Complete
  PDU provided.
  
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_ehs_provide_partial_to_complete_info(uint16 allowed_length,
            tdsl1_dec_rlc_pdu_concatenate_info_struct_type *rlc_p_to_c_ptr);

/*===========================================================================
FUNCTION: tdsmac_ehs_dl_get_rlc_id

DESCRIPTION:
  This function retrieves the rlc_id(internal value) from mac_id (1...15 from OTA)

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

uint8 tdsmac_ehs_dl_get_rlc_id(uint8      mac_id);

/*===========================================================================
FUNCTION: tdsmac_ehs_discard_partial_update

DESCRIPTION:

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_ehs_discard_partial_update(tdsl1_dec_hs_tsn_info_type *tsn_info_ptr, 
                                   boolean first_discard, boolean second_discard);
/*===========================================================================
FUNCTION: tdsmac_ehs_discard_partial_upto_comp_tsn

DESCRIPTION:
  This function is called, if L1 missed the DM activity (partial to complete)
  and ignores the request further. Next batch of DM response came. 
  Work around is - to discard the partials in TSN upto the received batch first
  TSN and pass them to RLC.
  
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  As we are discarding valid PARTIALS and passing the TSN to RLC, RLC might 
  experience some holes in the receive window. This might cause, RLC to wait
  minimum one RTT for retransmission. In Very high data rates, if this happens
  quite often, we might see RLC level retransmission requests often causing 
  RLC Window Stall conditions in DL (In effect, peer NW might not schedule
  as not seeing the RLC ACK moving quickly and window is reached)
===========================================================================*/

void tdsmac_ehs_discard_partial_upto_comp_tsn(q_type *mac_partial_tsn_q_ptr, 
                                           tdsl1_dec_hs_tsn_info_type *complete_tsn_ptr);
/*===========================================================================
FUNCTION: tdsmac_ehs_update_partial_first_info

DESCRIPTION:
  Update the Header info and remember the TSN Info from Partial Start

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

void tdsmac_ehs_update_partial_first_info(
             tdsl1_dl_partial_rlc_pdu_hdr_struct_type *rlc_pdu_hdr_info_ptr,
             tdsrlc_dl_tb_hdr_info_type *rlc_hdr_info_ptr, 
             tds_partial_pdu_type *partial_pdu_ptr,
             tdsl1_dec_hs_tsn_info_type *temp_tsn_ptr);

/*===========================================================================
FUNCTION: tdsmac_ehs_update_queue_due_to_invalid_combo

DESCRIPTION:
  Find the previous TDS_PARTIAL_FIRST before the clear_upto_tsn_ptr. Clear from that
  TSN onwards till and including the clear_upto_tsn_ptr

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_ehs_update_queue_due_to_invalid_combo(uint8 queue_id, 
                                              tdsl1_dec_hs_tsn_info_type *clear_upto_tsn_ptr);

/*===========================================================================
FUNCTION: mac_hs_dl_pass_tsn_pdus_to_partial_tsn_q

DESCRIPTION:
  Place the TSN in partial_tsn_q
  
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
boolean tdsmac_ehs_dl_pass_tsn_pdus_to_partial_tsn_q(tdsl1_dec_hs_tsn_info_type *tsn_info_ptr);

#endif /* defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS) */

/*===========================================================================
FUNCTION: tdsmac_hs_tsn_empty_recovery

DESCRIPTION:
This function will flush the all MAC-hs/MAC-ehs re-ordering queue

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/

void tdsmac_hs_tsn_empty_recovery( void );


/*===========================================================================
FUNCTION: tdsmac_hs_dl_discard_tsn

DESCRIPTION:
  This function discards a MAC-HS tsn. This includes freeing the dsm items
  for all the mac-d pdus forming the mac-hs pdu.

  Decrement the app_field part of the DSM item shown by iovec.
  Release the DSM Item, if the app_field is zero. Assign null to iovec.ptr
  Invalidate the TSN contents

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_hs_dl_discard_tsn(tdsl1_dec_hs_tsn_info_type *int_tsn_ptr);


#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
void tdsmac_ehs_dl_send_hs_config_log_packet(tdsmac_dl_config_type *cfgptr);

/*===========================================================================
FUNCTION: tdsmac_ehs_pass_comp_pdu_to_rlc

DESCRIPTION:
  For every complete PDU TSN present in queue, It will pass all the TSN from 
  Partial Start to Partial Last to RLC. All the TSN following Partial Last
  which doesnt have any Partials also will be delivered to RLC.
  
DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_ehs_pass_comp_pdu_to_rlc(void);
#endif /* defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS) */

#ifdef TEST_FRAMEWORK
#error code not present
#endif

extern void tds_mac_hs_dl_init_tcb(void);

extern rex_tcb_type* tds_mac_hs_dl_get_tcb(void);
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
/*===========================================================================
FUNCTION: tdsmac_hs_dl_timer_opt_handler

DESCRIPTION:
  This function handle the T1 timer optimization for a given reordering queue.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void tdsmac_hs_dl_timer_opt_handler(void);
#endif
#endif /* MAC_HS_DL_H */
