
/*===========================================================================

                   P S _ L O G G I N G _ H E L P E R . C

GENERAL DESCRIPTION
  This file defines helper routines to log data

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2004-2010 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

 $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/ps_logging_helper.c#2 $
 $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
07/09/10    sy     Fixed type-punned compiler warnings.
10/24/08    pp     Fixed compile warning observed in OFFTARGET build.
08/18/06    mjp    Fixed IPHC and ROHC Logging
08/31/06    mjp    Added IPHC and ROHC Logging
07/17/06    mp     Fixed logging of zero length DPL packets
06/05/06    mp     Added DPL PPP Partial Logging
05/12/05    ks     Fixed Lint errors.
01/12/05    msr    Added code review comments.
01/10/05    msr    Added support for flow and link logging.
11/15/04   ks/msr  Sending values to DPLI_BUILD_LOG_PACKET. Fixed a bug
                   where DSM is checked to see if it is corrupted.
10/31/04   ks/msr  Created module.
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "dsm.h"
#include "ps_svc.h"
#include "ps_logging.h"
#include "ps_iface.h"

#include "amssassert.h"
#include "ps_logging_helper.h"
#include "ds_Utils_DebugMsg.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#include "ds_profile.h"
#include "ds_profile_3gpp.h"
#include "ds_profile_3gpp2.h"
#include "ps_ifacei.h"

/*===========================================================================
                            LOCAL DEFINITIONS
===========================================================================*/
/* Used to set final bit in logging packet */
#define DPLI_SEGNUM_FINAL_BIT  0x8000

/* 0xFFFFFFFF is used to indicate that a data item failed to allocate*/
#define DPL_LOG_ALLOC_FAILED   (0xFFFFFFFF)

/* Used to represent short negative one so overflow sets num to zero when
          incremented */
#define ONE_LESS_THAN_ZERO     0xFFFF

#define IP_HDR_LEN_MASK        0x0F
#define IP_VERSION_MASK        0xF0
#define IPV4_VERSION           0x40
#define IPV6_VERSION           0x60
#define IPV6_HEADER_LENGTH     40

/*===========================================================================
MACRO    DPLI_BUILD_LOG_PACKET_HDR

DESCRIPTION
  Constructs the log packet header.

PARAMETERS
  log_ptr       : Pointer to the log packet
  iid           : IID associated with this log packet
  frame_seq_num : Sequence number for this log packet
  seg_num       : Segment number for this log packet

SIDE EFFECTS
  log_ptr is advanced past the constructed dpl log header

RETURN VALUE
  NONE
===========================================================================*/
#define DPLI_BUILD_LOG_PACKET_HDR(log_ptr, iid, frame_seq_num, seg_num)    \
  /*casting to void* to take care of type-punned compiler warning    */\
  log_ptr = put32(log_ptr, *(uint32 *)((void *) &(iid)));              \
  log_ptr = hput16(log_ptr, frame_seq_num);                            \
  log_ptr = hput16(log_ptr, seg_num);


/* Used for setting version number for DPM flow control logging version*/
#define DPM_LOGGING_VERSION 1


/*===========================================================================

                          INTERNAL FUNCTION DEFINITONS

===========================================================================*/
/*===========================================================================
FUNCTION    DPLI_LOG_FULL_PACKET

DESCRIPTION
  Logs a packet by potentially splitting it to multiple segments

PARAMETERS
  item_ptr      : pointer to the dsm chain to be logged
  len           : length of the packet to be logged
  iid           : IID of the data log point
  frame_seq_num : sequence number for the packet being logged

RETURN VALUE
  TRUE  : packet was logged
  FALSE : packet was not logged

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpli_log_full_packet
(
  dsm_item_type  * item_ptr,
  uint32           len,
  dpl_iid_type     iid,
  uint16           frame_seq_num
)
{
  uint8   * log_ptr;           /* pointer to log buffer                    */
  uint8   * return_ptr;
  uint16    offset;            /* byte offset into DSM chain               */
  uint16    seg_num;           /* segment number sent in each logged frame */
  uint16    payload_len;       /* # of bytes sent in log packet            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  offset      = 0;
  seg_num     = 0;

  /*-------------------------------------------------------------------------
    Although dsm_seek_extract() modifies this value when we call
    DPLI_BUILD_LOG_PACKET(), value is reset to MAX_PAYLOAD when
    dsm_seek_extract() succeeds. So there is no need to reset the value
    before each call to DLI_BUILD_LOG_PACKET().
  -------------------------------------------------------------------------*/
  payload_len = MAX_PAYLOAD;

  /*-------------------------------------------------------------------------
    Log full-length segments while outstanding payload is > maximum log
    frame length.

    NOTE: Loop is unrolled in to two parts to optimize this function. So if
          body of this loop changes, change other part also.
  -------------------------------------------------------------------------*/
  while (len > MAX_PAYLOAD)
  {
    log_ptr = (uint8 *) log_alloc(LOG_DATA_PROTOCOL_LOGGING_C,
                                  MAX_PAYLOAD + DPL_TOTAL_HDR_SIZE);

    if (NULL == log_ptr)
    {
      return FALSE;
    }

    /*-----------------------------------------------------------------------
      Have to reset max_payload every time as DPLI_BUILDLOG_PACKET changes
      its value
    -----------------------------------------------------------------------*/
    return_ptr =  log_ptr;
    log_ptr    += LOG_HDR_SIZE;

    DPLI_BUILD_LOG_PACKET(&item_ptr,
                          log_ptr,
                          &offset,
                          iid,
                          &payload_len,
                          frame_seq_num,
                          seg_num++);

    if (MAX_PAYLOAD != payload_len)
    {
      log_free(return_ptr);
      return FALSE;
    }

    log_commit (return_ptr);

    /*-----------------------------------------------------------------------
      Check for segment number wrap around
    -----------------------------------------------------------------------*/
    seg_num &= 0x7FFF;
    len     -= MAX_PAYLOAD;
  }

  /*-------------------------------------------------------------------------
    This part builds the log packet when len becomes < = MAX_PAYLOAD.
    len can never be zero as the condition used in above loop is
    "len > MAX_PAYLOAD". So here "0 < len <= MAXPAYLOAD".
  -------------------------------------------------------------------------*/
  log_ptr = (uint8 *) log_alloc(LOG_DATA_PROTOCOL_LOGGING_C,
                                len + DPL_TOTAL_HDR_SIZE);

  if (log_ptr == NULL)
  {
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    As this is the last segment, set the F bit (most significant bit) of
    the segment number to 1
  -------------------------------------------------------------------------*/
  seg_num     |= DPLI_SEGNUM_FINAL_BIT;
  return_ptr  =  log_ptr;
  log_ptr     += LOG_HDR_SIZE;
  /**/
  payload_len = (uint16)len;
  /**/

  DPLI_BUILD_LOG_PACKET(&item_ptr,
                        log_ptr,
                        &offset,
                        iid,
                        &payload_len,
                        frame_seq_num,
                        seg_num);

  if (len != payload_len)
  {
    log_free(return_ptr);
    return FALSE;
  }

  log_commit (return_ptr);

  return TRUE;

} /* dpli_log_full_packet() */



/*===========================================================================
FUNCTION    DPLI_LOG_PPP_PARTIAL_PACKET

DESCRIPTION
  Logs a packet potentially only partially and possibly by splitting it
  to multiple segments

PARAMETERS
  item_ptr          : pointer to the dsm chain to be logged
  dpl_id            : IID associated with this packet
  log_flag_link_cb  : pointer to the ppp link layer control block

RETURN VALUE
  TRUE  : partial packet was logged
  FALSE : partial packet was not logged

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dpli_log_ppp_partial_packet
(
  const dsm_item_type  *  item_ptr,
  dpl_iid_type            dpl_id,
  dpl_link_dir_cb_type *  log_flag_link_cb
)
{
  uint8   * log_ptr;         /* pointer to log buffer                      */
  uint8   * return_ptr;      /* save the start of log buffer here          */
  uint8   * src_ptr;         /* address of current data byte observed      */
  uint8   * src_end_ptr;     /* address of current data byte observed      */
  uint8   * dest_ptr;        /* where to copy payload (output log buffer)  */
  uint32    pbytes_logged;   /* partial bytes logged of the ppp packet     */
  uint32    snaplen;         /* Current snaplength                         */
  uint16    seg_num;         /* segment number sent in each logged frame   */
  uint16    dest_room_left;  /* length of the data to be logged            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Setup state
  -------------------------------------------------------------------------*/
  seg_num       = log_flag_link_cb->seg_num[DPL_IID_LINKPROT_PPP_IN_HDLC];
  pbytes_logged = log_flag_link_cb->partial_bytes_logged;
  snaplen       = log_flag_link_cb->snaplen;
  if (snaplen == SNAPLEN_ALL)
  {
    snaplen = 65535;
  }

  if ((NULL == item_ptr)||(NULL == item_ptr->data_ptr))
  {
    return;
  }
  src_ptr       = item_ptr->data_ptr;
  src_end_ptr   = src_ptr + item_ptr->used;

  /*-------------------------------------------------------------------------
    Loop until all input PPP data is logged
  -------------------------------------------------------------------------*/
  while (item_ptr)
  {
    /*-----------------------------------------------------------------------
      Check if the snap length has been reached and if so run through looking
      for 7E
    -----------------------------------------------------------------------*/
    if (pbytes_logged == snaplen)
    {
      while (*src_ptr != 0x7E)
      {
        while (*src_ptr != 0x7E && src_ptr != src_end_ptr)
        {
          src_ptr++;
        }
        /* Check if we got to the end of the current input DSM item */
        if (src_ptr == src_end_ptr)
        {
          /* Go to next DSM item */
          item_ptr = item_ptr->pkt_ptr;
          if (item_ptr == 0)
          {
             /* There is nothing more to read, log and leave */
             log_flag_link_cb->seg_num[DPL_IID_LINKPROT_PPP_IN_HDLC]
               = seg_num;
             log_flag_link_cb->partial_bytes_logged
               = pbytes_logged;
             return;
          }
          src_ptr      = item_ptr->data_ptr;
          src_end_ptr  = src_ptr + item_ptr->used;
        }
        else /* We reached a 7E */
        {
          pbytes_logged = 0;
          break;
        }
      }
    }

    /*-----------------------------------------------------------------------
      Alocate full-length log segment to work with
    -----------------------------------------------------------------------*/
    log_ptr = (uint8 *) log_alloc(LOG_DATA_PROTOCOL_LOGGING_C,
                                  MAX_PAYLOAD + DPL_TOTAL_HDR_SIZE);

    if (NULL == log_ptr)
    {
      LOG_MSG_INFO2_0("dpli_log_ppp_partial_packet(): "
                      "log_alloc failed");
      pbytes_logged = DPL_LOG_ALLOC_FAILED;
      break;  /* make sure to break to save back in control block */
    }

    /*-------------------------------------------------------------------------
      At the start of a new packet this is reset to the payload size
    -------------------------------------------------------------------------*/
    return_ptr     =  log_ptr;
    log_ptr        += LOG_HDR_SIZE;
    dest_ptr       =  log_ptr + DPL_LOG_HDR_SIZE;
    dest_room_left =  MAX_PAYLOAD;

    /*-----------------------------------------------------------------------
      Create log header
    -----------------------------------------------------------------------*/
    /**/
    DPLI_BUILD_LOG_PACKET_HDR(
      log_ptr,
      dpl_id,
      log_flag_link_cb->seq_num[DPL_IID_LINKPROT_PPP_IN_HDLC],
      seg_num );
    /* log ptr is now pointing after the DPL header */

    /**/

    /*-----------------------------------------------------------------------
      Run through Data creating one log packet (segment) at a time
    -----------------------------------------------------------------------*/
    while (dest_room_left)
    {
      /*---------------------------------------------------------------------
        Run through Data in the input DSM
      ---------------------------------------------------------------------*/
      while (src_ptr != src_end_ptr && dest_room_left)
      {
        if (*src_ptr == 0x7E)
        {
          /* 7E indicates the start of a packet.  There could be multiple
             7Es back to back, in which case stack them all at the start of
             the current packet.  If there is any data between 7E's, then the
             7E delineates the next packet.

             Hence, if we  */
          if (pbytes_logged <= 1)
          {
            /* All 7E's in a row are considered one byte */
            pbytes_logged = 1;

            /* Copy to the data log */
            *dest_ptr = *src_ptr;
            dest_ptr++;
            dest_room_left--;
          }
          else
          {
            /*---------------------------------------------------------------
              a 7E has been encountered after non 7E data.
              - shorten log packet,
              - reset last 2 bytes of segnum to include finish bit (0x8000)
            ---------------------------------------------------------------*/
            log_shorten( return_ptr,
                         DPL_TOTAL_HDR_SIZE + MAX_PAYLOAD - dest_room_left );
            dest_room_left = 0;

            (void) hput16(log_ptr - 2, seg_num | DPLI_SEGNUM_FINAL_BIT);

            /* a new ppp packet is starting so increment sequence number
                                                and byte count */
            log_flag_link_cb->seq_num[DPL_IID_LINKPROT_PPP_IN_HDLC]++;
            seg_num = ONE_LESS_THAN_ZERO;
            pbytes_logged = 0;
            break; /* leave iterating throgh DSM to send log packet */
          }
        }
        /* Check if we have reached the snaplength */
        else if( (pbytes_logged == snaplen) &&
                 (pbytes_logged != DPL_LOG_ALLOC_FAILED) )
        {
          /*-----------------------------------------------------------------
            Snaplen characters have been encountered
            - shorten log packet,
            - reset last 2 bytes of segnum to include finish bit (0x8000)
          -----------------------------------------------------------------*/
          log_shorten( return_ptr,
                       DPL_TOTAL_HDR_SIZE + MAX_PAYLOAD - dest_room_left );
          dest_room_left = 0;

          (void) hput16(log_ptr - 2, seg_num | DPLI_SEGNUM_FINAL_BIT);

          /* a new ppp packet is starting so increment sequence number
                                                and byte count */
          log_flag_link_cb->seq_num[DPL_IID_LINKPROT_PPP_IN_HDLC]++;
          seg_num = ONE_LESS_THAN_ZERO;
          break; /* leave iterating throgh DSM to send log packet */
        }
        /* If bytes are to be logged still, log this non 7E byte */
        else if( (pbytes_logged < snaplen) &&
                 (pbytes_logged != DPL_LOG_ALLOC_FAILED) )
        {
          /* Copy to the data log */
          *dest_ptr = *src_ptr;
          dest_ptr++;
          dest_room_left--;
          pbytes_logged++;
        }
        /* Move to next element in the DSM */
        src_ptr++;
      }

      /* Check if we got to the end of the current input DSM item */
      if (src_ptr == src_end_ptr)
      {
        /* Go to next DSM item */
        item_ptr = item_ptr->pkt_ptr;
        if (item_ptr == 0)
        {
           /* Send Packet if there is nothing more to read */
           /* shorten  log packet */
           log_shorten( return_ptr,
                        DPL_TOTAL_HDR_SIZE + MAX_PAYLOAD - dest_room_left );
           dest_room_left = 0;
           break;
        }
        src_ptr      = item_ptr->data_ptr;
        src_end_ptr  = src_ptr + item_ptr->used;
      }
    }

    /*-----------------------------------------------------------------------
      Dispatch the log packet containing the current segment, then
      increment the segment number count (accounting for wraparound) in
      preparation for the next segment.
    -----------------------------------------------------------------------*/
    log_commit (return_ptr);
    seg_num++;
    seg_num &= 0x7FFF;
  }

  /*-------------------------------------------------------------------------
    Run out of data. Need to save state to the control block
  -------------------------------------------------------------------------*/
  log_flag_link_cb->seg_num[DPL_IID_LINKPROT_PPP_IN_HDLC] = seg_num;
  log_flag_link_cb->partial_bytes_logged                  = pbytes_logged;

} /* dpli_log_ppp_partial_packet() */

/*===========================================================================
FUNCTION    DPLI_fill_apn_info

DESCRIPTION
  Reterive APN info from 3GPP and 3GPP2 profile and fill into log pointer

PARAMETERS

  log_ptr               : log pointer
  bridged_iface_ptr : bridged iface ptr(will be used in case of RM ifaces) 

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
uint8 * dpl_fill_apn_info
(
  ps_iface_type  * this_iface_ptr,
  uint8          * log_ptr,
  uint8            size
)
{
  byte                           apn_string[DS_PROFILE_3GPP_MAX_APN_STRING_LEN+1];
  int8                           apn_len = 0;
  ds_profile_hndl_type           profile_hndl = NULL;
  ds_profile_info_type           param_info;
  ds_profile_status_etype        profile_status;
  uint32                         profile_id_3gpp;
  uint32                         profile_id_3gpp2;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  profile_id_3gpp   = PS_IFACEI_GET_3GPP_PROFILE_ID(this_iface_ptr);
  profile_id_3gpp2  =  PS_IFACEI_GET_3GPP2_PROFILE_ID(this_iface_ptr);
    
  profile_status    =  ds_profile_begin_transaction (
                          DS_PROFILE_TRN_READ,
                          DS_PROFILE_TECH_3GPP,
                          (ds_profile_num_type)(profile_id_3gpp),
                          &profile_hndl);
 
  if ((DS_PROFILE_REG_RESULT_SUCCESS == profile_status) && 
      (NULL != profile_hndl))
  {
  
    memset(&param_info, 0, sizeof(param_info));
    memset(&apn_string[0], 0, sizeof(apn_string));
    
    param_info.buf = apn_string;
    param_info.len = DS_PROFILE_3GPP_MAX_APN_STRING_LEN+1;
  
    if(DS_PROFILE_REG_RESULT_SUCCESS == ds_profile_get_param(profile_hndl, 
                                          DS_PROFILE_3GPP_PROFILE_PARAM_PDP_CONTEXT_APN, 
                                          &param_info))
    {
      /*-----------------------------------------------------------------------
            Copy 3GPP APN name,
      -----------------------------------------------------------------------*/
      if (0 != strlen((char *)apn_string) && ( strlen((char *)apn_string) < size ))
      {
        apn_len = snprintf((char *)log_ptr, size, " %s", apn_string);

        if (apn_len > 0)
        {
          log_ptr = log_ptr + apn_len;
        }
        else
        {
          /* 3GPP apn copy failed , resetting it to zero
             to copy 3GPP2 APN name in next operation*/
          apn_len = 0;
        }

      }
    }
    (void)ds_profile_end_transaction(profile_hndl, DS_PROFILE_ACTION_COMMIT);
  }

  profile_status =  ds_profile_begin_transaction(
                      DS_PROFILE_TRN_READ,
                      DS_PROFILE_TECH_3GPP2,
                      (ds_profile_num_type)profile_id_3gpp2,
                      &profile_hndl);
  
  if((DS_PROFILE_REG_RESULT_SUCCESS == profile_status) && (NULL != profile_hndl))
  {
    memset(&param_info, 0, sizeof(param_info));
    memset(&apn_string[0], 0, sizeof(apn_string));
    
    param_info.buf = apn_string;
    param_info.len = DS_PROFILE_3GPP2_APN_MAX_VAL_LEN+1;
  
    if(DS_PROFILE_REG_RESULT_SUCCESS == ds_profile_get_param(profile_hndl, 
                                          DS_PROFILE_3GPP2_PROFILE_PARAM_APN_STRING, 
                                          &param_info))
    {
      /*-----------------------------------------------------------------------
            Copy 3GPP2 APN name,
      -----------------------------------------------------------------------*/
      if ((0 != strlen((char *)apn_string)) && ( strlen((char *)apn_string) < (uint8)(size - apn_len)))
      {
        apn_len = snprintf((char *)log_ptr, size - apn_len, " %s", apn_string);
        
        if (apn_len > 0)
        {
          log_ptr = log_ptr + apn_len;
        }
      }
    }
     (void)ds_profile_end_transaction(profile_hndl,DS_PROFILE_ACTION_COMMIT);
  }

  return log_ptr;
}/*dpl_fill_apn_info*/

/*===========================================================================
FUNCTION    DPLI_LOG_IFACE_DESC_AND_STATUS

DESCRIPTION
  Logs a packet with iface status(UP/DOWN) and iface description

PARAMETERS

  this_iface_ptr      : logged this iface description
  bridged_iface_ptr : bridged iface ptr(will be used in case of RM ifaces) 

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dpli_log_iface_desc_and_status
(
  ps_iface_type                   *this_iface_ptr,
  ps_iface_type                   *bridged_iface_ptr
)
{
  uint8          * log_ptr;      
  uint8          * return_ptr;
  uint16           space_left     = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(this_iface_ptr))
  {
  
     LOG_MSG_ERROR_0("dpli_log_iface_desc_and_status: "
                     "Null iface pointer" );
     return;
  }
  
  /*-----------------------------------------------------------------------
   DPL Iface description is present in physical iface DPL control block.
   Ignore Logical ifaces
    -----------------------------------------------------------------------*/

  if (this_iface_ptr != PS_IFACE_GET_BASE_IFACE(this_iface_ptr))
  {
  
     LOG_MSG_ERROR_0("dpli_log_iface_desc_and_status: "
                     "Logical Iface desc will not be logged " );
     return;
  }

  log_ptr = (uint8 *) log_alloc(LOG_DPL_IFACE_DESCRIPTION_AND_STATUS_C,
                                  MAX_DPL_DESC_LEN + LOG_HDR_SIZE);
  

  if (NULL == log_ptr)
  {
    
    LOG_MSG_INFO2_0("dpli_log_iface_desc_and_status: "
                      "log_alloc failed ");
    return;
  }
  
  return_ptr =  log_ptr;
  
  log_ptr   += LOG_HDR_SIZE;
    
  log_ptr = put32(log_ptr, *(uint32 *)((void *) &( this_iface_ptr->dpl_net_cb.tx_dpl_id)));

  space_left = MAX_DPL_DESC_LEN - (log_ptr- return_ptr);

  log_ptr += snprintf((char *)log_ptr, 
                        space_left,
                        "%s", this_iface_ptr->dpl_net_cb.desc);
 
  space_left = MAX_DPL_DESC_LEN - (log_ptr- return_ptr);
 
  if (this_iface_ptr->name  == SIO_IFACE )
  {
   /*-----------------------------------------------------------------------
      Valid Bridge iface pointer is required for RM Ifaces to figure out APN info
       -----------------------------------------------------------------------*/
    if(PS_IFACE_IS_VALID(bridged_iface_ptr))
    {
      log_ptr = dpl_fill_apn_info(bridged_iface_ptr, log_ptr, space_left);
    }
  }
  else
  {
    log_ptr = dpl_fill_apn_info(this_iface_ptr, log_ptr, space_left);
  }
  
  log_shorten(return_ptr, (uint32) (log_ptr - return_ptr));

  log_commit(return_ptr);
  
  return;

}/*dpli_log_iface_desc_and_status*/

uint32 dpli_get_ip_header_length
(
  dsm_item_type  * item_ptr
)
{
  uint8                              ip_hdr_first_byte;
  uint8                              header_length = 0;
  uint8                              version;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Peek in to the first byte to determine lenght of the IP header
  -----------------------------------------------------------------------*/
  (void) dsm_extract(item_ptr, 0, &ip_hdr_first_byte, 1);

  version = ip_hdr_first_byte & IP_VERSION_MASK;
  if (IPV4_VERSION == version)
  {
    header_length = ip_hdr_first_byte & IP_HDR_LEN_MASK;
    /*-----------------------------------------------------------------------
      multiply by 4 as the lenght is stored in 32-bit words
    -----------------------------------------------------------------------*/
    return (header_length * 4); 
  }
  /*-----------------------------------------------------------------------
    For IPv6 packet, IP header length is constant (40 bytes)
  -----------------------------------------------------------------------*/
  else if (IPV6_VERSION == version)
  {
    return IPV6_HEADER_LENGTH;
  }
  else
  {
    return header_length;
  }
}

/*===========================================================================
FUNCTION    DPL_LOG_SIO_DESC_AND_STATUS

DESCRIPTION
  Log sio config information

PARAMETERS
  dpl_sio_config : DPL sio config information
  by_timer       : TRUE if invoked by timer, otherwise FALSE

RETURN VALUE
  TRUE on success
  FALSE otherwise

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpl_log_sio_desc_and_status
(
  dpl_sio_config       *dpl_sio_info_ptr,
  uint8                 meta_info_type
)
{
  uint8              * log_ptr;
  uint8              * return_ptr;
  uint8                index              = 0;
  dpl_iface_config   * dpl_iface_info_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( NULL == dpl_sio_info_ptr || 0 == dpl_sio_info_ptr->num_valid_ifaces ||
      dpl_sio_info_ptr->num_valid_ifaces > DPL_NUM_IFACES_PER_PDN)
  {
    return FALSE;
  }
  log_ptr = (uint8 *) log_alloc(LOG_DPL_HW_SIO_CONF_C, 
                                MAX_PDN_INFO_SIZE + LOG_HDR_SIZE + 1);
  if (NULL == log_ptr)
  {
  
    LOG_MSG_INFO2_1("dpl_log_sio_desc_and_status: "
                    "log_alloc failed  mux id %d", dpl_sio_info_ptr->mux_id);
    return FALSE;
  }
  
  return_ptr =  log_ptr;
  
  log_ptr   += LOG_HDR_SIZE;

  log_ptr = put8(log_ptr, meta_info_type);
    
  //Store MUX ID
  log_ptr = put8(log_ptr, dpl_sio_info_ptr->mux_id);
  //Store Number of iface
  log_ptr = put8(log_ptr, dpl_sio_info_ptr->num_valid_ifaces);
    
  memscpy( log_ptr, 
           DPL_APN_DESC_S_LEN+1,
           dpl_sio_info_ptr->apn_desc,
           DPL_APN_DESC_S_LEN+1);
  
  log_ptr = log_ptr + (DPL_APN_DESC_S_LEN+1);
    
  for( index = 0 ; index < dpl_sio_info_ptr->num_valid_ifaces; index++)
  {
    dpl_iface_info_ptr = &dpl_sio_info_ptr->ifaces_list[index];

    log_ptr = put8(log_ptr, dpl_iface_info_ptr->iface_type);
    log_ptr = put8(log_ptr, dpl_iface_info_ptr->iface_id);
    log_ptr = put8(log_ptr, dpl_iface_info_ptr->ep_id);

    memscpy( log_ptr, 
             DPL_IFNAME_DESC_S_LEN+1,
             dpl_iface_info_ptr->iface_desc,
             DPL_IFNAME_DESC_S_LEN+1);
    
   log_ptr = log_ptr + (DPL_IFNAME_DESC_S_LEN + 1);    
  }
    
  log_shorten(return_ptr, (uint32) (log_ptr - return_ptr));

  log_commit(return_ptr);

  return TRUE;
} /* dpl_log_sio_desc_and_status */

/*===========================================================================
FUNCTION    DPL_LOG_SKIP_HDR_BYTES

DESCRIPTION
  Log skip header bytes information

PARAMETERS
  mux_id         : MUX ID
  srt_endp       : Source end point ID
  hdr_bytes      : number of bytes for header
  direction      : 0 for UL and 1 for DL
  by_timer       : TRUE if invoked by timer, otherwise FALSE

RETURN VALUE
  TRUE on success
  FALSE otherwise

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpl_log_skip_hdr_bytes
(
  uint8                mux_id,
  uint8                src_endp,
  uint8                hdr_bytes,
  uint8                direction
)
{
  uint8              * log_ptr;
  uint8              * return_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  log_ptr = (uint8 *) log_alloc(LOG_DPL_HW_SIO_CONF_C, MAX_DPL_HDR_INFO_SIZE + LOG_HDR_SIZE + 1);

  if (NULL == log_ptr)
  {
  
    LOG_MSG_INFO2_0("dpl_log_skip_hdr_bytes: "
                    "log_alloc failed ");
    return FALSE;
  }
  
  return_ptr =  log_ptr;
  
  log_ptr   += LOG_HDR_SIZE;

  //Store META INFO type
  log_ptr = put8(log_ptr, (uint8)DPL_META_INFO_SKIP_HDR_BYTE);
  log_ptr = put8(log_ptr, (uint8)mux_id);
  log_ptr = put8(log_ptr, (uint8)src_endp);
  log_ptr = put8(log_ptr, (uint8)hdr_bytes);
  log_ptr = put8(log_ptr, (uint8)direction);
  
  log_shorten(return_ptr, (uint32) (log_ptr - return_ptr));

  log_commit(return_ptr);

  return TRUE;
} /* dpl_log_skip_hdr_bytes */

void ds_interface_logging_msg
(
  uint8                             file[],
  uint32                            line,
  ps_logging_category_enum_type     category_id,
  uint8                             *payload
)
{
  uint8 *   log_ptr       = NULL;
  uint8 *   base_log_ptr  = NULL;
  uint8 *   length_ptr    = NULL;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    /*----------------------------------------------------------------------
        1.Allocate log packet with size 252 (LOG_HDR_SIZE + DS_LOG_MAX_PAYLOAD) and memset to 0
        2.Fill the log packet as
           2.1 Fill log message version ID
           2.2 Copy file length and file name
           2.3 Copy line length and line number
           2.4 Copy category ID
           2.5 Copy payload, truncate to DS_MAX_PAYLOAD if needed
        3.Print the original log message if error occurs or log packet is not enabled
       -----------------------------------------------------------------------*/

    /* Step 1: Allocate log packet with size 252 (LOG_HDR_SIZE + DS_LOG_MAX_PAYLOAD) 
       and memset to 0*/
    log_ptr = (uint8*)log_alloc(LOG_DATA_INTERFACES_MESSAGES_C, LOG_HDR_SIZE + DS_LOG_MAX_PAYLOAD);
    
    if ( NULL == log_ptr )
    {
      break;
    }
    //Assign Base pointer
    base_log_ptr = log_ptr;
    log_ptr += LOG_HDR_SIZE;   
    memset(log_ptr, 0x0, DS_LOG_MAX_PAYLOAD);

    /* 2.1 Fill log message version ID */
    log_ptr = put8(log_ptr, DS_LOG_MSG_VER_ID);

    /* 2.2 Copy file length and file name */
    length_ptr = log_ptr++;
    *length_ptr = snprintf((char *)log_ptr, 
                            PS_LOG_SPACE_LEFT(DS_LOG_MAX_PAYLOAD, log_ptr, base_log_ptr),
                            "%s", file);
    log_ptr = log_ptr + *length_ptr;
    
    /*2.3 Copy line length and line number*/
     length_ptr = log_ptr++;
    *length_ptr = snprintf((char *)log_ptr, 
                           PS_LOG_SPACE_LEFT(DS_LOG_MAX_PAYLOAD, log_ptr, base_log_ptr),
                           "%d", (int) line);
     log_ptr = log_ptr + *length_ptr;

    /*2.4 Copy category ID*/
    log_ptr = put8(log_ptr, category_id);

    /*2.5 Copy payload, truncate to DS_MAX_PAYLOAD if needed*/
    if ( NULL != payload )
    { 
      length_ptr = log_ptr++;
      *length_ptr = memscpy(log_ptr, 
                        PS_LOG_SPACE_LEFT(DS_LOG_MAX_PAYLOAD, log_ptr, base_log_ptr),
                        payload,
                        strnlen((const char*)payload, 
                                 PS_LOG_SPACE_LEFT(DS_LOG_MAX_PAYLOAD, log_ptr, base_log_ptr)));
      log_ptr = log_ptr + *length_ptr;
    }
    else
    {
      break;
    }
    //Validate log copyupdate me
    
    if( ( log_ptr - base_log_ptr) <= (DS_LOG_MAX_PAYLOAD + LOG_HDR_SIZE))
    {
      log_shorten(base_log_ptr, (uint32) (log_ptr - base_log_ptr));
      log_commit(base_log_ptr);
    }
    else
    {
      LOG_MSG_INFO1_2( "ds_interface_logging_msg invalid copy "
                       " copied len %d  allowed len %d", 
                       ( log_ptr - base_log_ptr),
                        DS_LOG_MAX_PAYLOAD + LOG_HDR_SIZE);
      break;
    }
 
    return;
  }while(0);
 
  if (NULL != base_log_ptr)
  {
    log_free(base_log_ptr);
  }

  /* 3.Print the original log message if error occurs or log packet is not enabled*/
  LOG_MSG_STR_INFO1_03("%s:%d %s",file, line, payload);
  
  return;
}
/* ds_interface_logging_msg */

/*===========================================================================
FUNCTION    DPL_LOG_FC_EVENT_PKT

DESCRIPTION
  Function to log the triggered Flow control event
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void dpl_log_fc_event_pkt
(
  dpl_log_fc_ev_type  * log_ev_info_ptr
)
{
  byte                                 * log_ptr;     /* pointer to log buffer       */
  uint16                                 len = 0;     /* length of log frame payload */
  dpl_log_fc_ev_type                * log_pkt_ptr; /* pointer to payload in the log pkt */
  byte                                 * log_payload_ptr; /* pointer to payload in the log pkt */
  ps_dpm_error_enum_type                 err_no = PS_DPM_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  log_ev_info_ptr->version = DPM_LOGGING_VERSION;
  
  do 
  {
    /* Size of the payload */
    len = sizeof(dpl_log_fc_ev_type);

    /* allocate memory for the log packet 0X19EF (DFC FLOW CONTROL TRIGGER)*/
    log_ptr = (byte *)log_alloc(LOG_DS_FLOW_CONTROL_TRIGGER_C, len + sizeof(log_hdr_type));

    if (NULL == log_ptr)
    {
      err_no = PS_DPM_ERROR_MEM_ALLOC_FAILED;
      break;
    }    

    /* Populate the payload information */
    log_payload_ptr        = log_ptr + sizeof(log_hdr_type);
    log_pkt_ptr            = (dpl_log_fc_ev_type*)log_payload_ptr;
    memscpy(log_pkt_ptr, 
            sizeof(dpl_log_fc_ev_type), 
            log_ev_info_ptr, 
            sizeof(dpl_log_fc_ev_type));
    log_commit(log_ptr);
    return;
  } while(0);

  LOG_MSG_ERROR_1("LOG_FC_TRIG : Error logging FC event pkt, err_no: %d", err_no);  

  return;
} /* dpli_log_fc_event_pkt */

/*===========================================================================
FUNCTION    DPL_LOG_FC_CFG_PKT

DESCRIPTION
  Function to log the Flow control configuration packet
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void dpl_log_fc_cfg_pkt
(
  dpl_log_pkt_cfg_fc_type  * log_fc_cfg_pkt_ptr
)
{
  byte                                 * log_ptr;     /* pointer to log buffer       */
  uint16                                 len = 0;     /* length of log frame payload */
  dpl_log_pkt_cfg_fc_type           * log_pkt_ptr; /* pointer to payload in the log pkt */
  byte                                 * log_payload_ptr; /* pointer to payload in the log pkt */
  ps_dpm_error_enum_type                 err_no = PS_DPM_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  log_fc_cfg_pkt_ptr->version = DPM_LOGGING_VERSION;
  
  do 
  {
    /* Size of the payload */
    len = sizeof(dpl_log_pkt_cfg_fc_type);

    /* allocate memory for the log packet  0X13AC (LOG_QMI_LAST_C - 2)*/
    log_ptr = (byte *)log_alloc(LOG_DS_FLOW_CONTROL_CONFIG_C, len + sizeof(log_hdr_type));

    if (NULL == log_ptr)
    {
      err_no = PS_DPM_ERROR_MEM_ALLOC_FAILED;
      break;
    }    

    /* Populate the payload information */
    log_payload_ptr        = log_ptr + sizeof(log_hdr_type);
    log_pkt_ptr            = (dpl_log_pkt_cfg_fc_type*)log_payload_ptr;
    memscpy(log_pkt_ptr, 
            sizeof(dpl_log_pkt_cfg_fc_type), 
            log_fc_cfg_pkt_ptr, 
            sizeof(dpl_log_pkt_cfg_fc_type));
    log_commit(log_ptr);
    return;
  } while(0);
  
  LOG_MSG_ERROR_1("LOG_FC_CFG : Error logging FC config pkt, err_no: %d", err_no);  
  return;
} /* dpli_log_fc_cfg_pkt */

/*===========================================================================
FUNCTION PS_LOGGING_ERR_FATAL

DESCRIPTION
  Wrapper function for ERROR_FATALS
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_err_fatal
(  
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  const char       *str
)
{ 

  /* Print the File name ,Function name and Line number before calling ERR_FATAL */
  if (NULL != file_name && NULL != func_name) 
  {
    LOG_MSG_STR_INFO1_03("ERROR_FATAL at %s , function: %s ,line:%d ",
                          file_name,func_name,line_num);
  }
      
  if (NULL == str)
  {
    ERR_FATAL("Assertion failed at %s line: %d", file_name, line_num, 0 );
    return;
  }

  ERR_FATAL("Error Fatal: %s at %s line: %d", str,file_name, line_num);

}/* ps_logging_err_fatal */

/*===========================================================================

FUNCTION PS_LOGGING_ASSERT

DESCRIPTION
  Wrapper function for ASSERTION
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_assert
(
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  int              expression
)
{
  /* Print the file name and line number before calling ERR_FATAL */
  if ((!expression))
  {
    LOG_MSG_STR_INFO1_03("ASSERT at %s , function: %s , line: %d ASSERT",
                         file_name,func_name,line_num);
	
    ERR_FATAL("Assertion failed at %s ,function: %s , line: %d", file_name, func_name,line_num);
  }

}/* ps_logging_assert */

/*===========================================================================
  
  FUNCTION PS_LOGGING_ERROR_LOG_0
  
  DESCRIPTION
	Wrapper function for reporting error messages
   
  DEPENDENCIES 
  
  RETURN VALUE
   
  SIDE EFFECTS
  
  ===========================================================================*/
  void ps_logging_error_log_0
  (
	const char*   file_name,
	unsigned int  line_num,  
	const char*   format
  )
  {
	if ((NULL == file_name) || (NULL == format))
	{
	  return;
	}
	MSG_SPRINTF_3(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL, 	 
				  "%s line: %d: %s",
				  file_name, line_num, format);
  }/* ps_logging_error_log_0 */
  
  /*===========================================================================
  
  FUNCTION PS_LOGGING_ERROR_LOG_1
  
  DESCRIPTION
	Wrapper function for reporting error messages
   
  DEPENDENCIES 
  
  RETURN VALUE
   
  SIDE EFFECTS
  
  ===========================================================================*/
  void ps_logging_error_log_1
  (
	const char*   file_name,
	unsigned int  line_num,
	const char*   format,
	uint32		  arg1
  )
  {
	if ((NULL == file_name) || (NULL == format))
	{
	  return;
	}
	MSG_SPRINTF_4(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL, 	 
				  "%s line: %d: %s, arg1: %lu",
				  file_name, line_num, format, arg1);
  }/* ps_logging_error_log_1 */
  
  /*===========================================================================
  
  FUNCTION PS_LOGGING_ERROR_LOG_2
  
  DESCRIPTION
	Wrapper function for reporting error messages
   
  DEPENDENCIES 
  
  RETURN VALUE
   
  SIDE EFFECTS
  
  ===========================================================================*/
  void ps_logging_error_log_2
  (
	const char*   file_name,
	unsigned int  line_num,
	const char*   format,
	uint32		  arg1,
	uint32		  arg2
  )
  {
	if ((NULL == file_name) || (NULL == format))
	{
	  return;
	}
	MSG_SPRINTF_5(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL, 	 
				  "%s line: %d: %s, arg1: %lu, arg2: %lu",
				  file_name, line_num, format, arg1, arg2);
  }/* ps_logging_error_log_2 */
  
  /*===========================================================================
  
  FUNCTION PS_LOGGING_ERROR_LOG_3
  
  DESCRIPTION
	Wrapper function for reporting error messages
   
  DEPENDENCIES 
  
  RETURN VALUE
   
  SIDE EFFECTS
  
  ===========================================================================*/
  void ps_logging_error_log_3
  (
	const char*   file_name,
	unsigned int  line_num,  
	const char*   format,
	uint32		  arg1,
	uint32		  arg2,
	uint32		  arg3
  )
  {
	if ((NULL == file_name) || (NULL == format))
	{
	  return;
	}
	MSG_SPRINTF_6(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL, 	 
				  "%s line: %d: %s, arg1: %lu, arg2: %lu, arg3: %lu",
				  file_name, line_num, format, arg1, arg2, arg3);
  }/* ps_logging_error_log_3 */
  
  /*===========================================================================
  
  FUNCTION PS_LOGGING_ERROR_LOG_4
  
  DESCRIPTION
	Wrapper function for reporting error messages
   
  DEPENDENCIES 
  
  RETURN VALUE
   
  SIDE EFFECTS
  
  ===========================================================================*/
  void ps_logging_error_log_4
  (
	const char*   file_name,
	unsigned int  line_num,
	const char*   format,
	uint32		  arg1,
	uint32		  arg2,
	uint32		  arg3,
	uint32		  arg4
  )
  {
	if ((NULL == file_name) || (NULL == format))
	{
	  return;
	}
	MSG_SPRINTF_7(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL, 	 
				  "%s line: %d: %s, arg1: %lu, arg2: %lu, arg3: %lu, arg4: %lu",
				  file_name, line_num, format, arg1, arg2, arg3, arg4);
  }/* ps_logging_error_log_4 */
 
