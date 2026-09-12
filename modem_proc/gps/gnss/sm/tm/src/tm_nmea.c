/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          NMEA functions definition file

GENERAL DESCRIPTION
  This file contains the functions to generate NMEA strings from the fix and
  measurement data received from MGP.
   
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2005 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_nmea.c#2 $ 
  $DateTime: 2020/01/14 02:11:57 $ 
  $Author: pwbldsvc $ 
  
when       who   what, where, why
--------   ---   ----------------------------------------------------------
11/22/19   py    Validate Heading based on Minimum velocity
12/05/18   py    Added Navigational status to EMPTY strings $--RMC,$--GNS and "Signal ID" in $--GSV
10/25/18   nath  Added support for GAGNS
09/25/18   nath  Adding PQDTM changes
04/25/18   sum   Added check while populating NMEAs to cover NHz scenario.
04/12/18   sum   Filling pqxfi hepe using ellipticle Unc.
04/17/18   sum   Added extra check while generating PQGSA
02/14/18   sum   Added extra field in GSA to make it ECall compliant.
11/27/17   nath  Added a value of a flag and a condition to report non-empty nmea strings
10/26/17   ak    Remove check for lock, as dsm_queue_init takes care of it.
10/12/17   sum   Added comprehensive check while gemerating NMEAs
09/22/17   py    Added Navigational status to $--RMC,$--GNS and "Signal ID" in $--GSV
09/13/17   sum   Added support for GPDTM NMEA
04/18/17   sum   Added support for GN based NMEA GNGGA GNVTG and GNRMC
03/22/17   sum   Added support for GAGSA GAVTG GARMC GAGGA and GAGNS
02/13/2017 sum   Replaced svs[u_i] with q_svid
1/12/17   gk    COG shouldnt be greater than 360 degrees for VTG
12/02/16   gk    COG shouldnt be greater than 360 degrees
09/13/16   mj    Changes to fix negative leap second correction
09/08/16   mj    Handle positive leap second correction during rollover
09/01/16   mj    Add changes for Leap Second Correction
04/05/16   gk    handle QZSS empty string
03/01/16   kpn   Differential flag needs to be set correctly for all the relevant nmea strings
01/05/16   mc    Dynamically allocate variables for huge structures to reduce stack usage
11/25/15   mc    Removed debug message in tm_nmea_port_put()
05/21/15   nl    Refactor code
03/14/15   rh    Added support for MultiSIM NV  
08/17/14   jv    Added Galileo Support
08/08/14   jv    Added 16-bit SVID support
10/08/13   ah    Removed DGPS fix reporting for MS-Assisted fixes
11/30/12   mj    Removed unused function tm_nmea_clear_gnss_meas_update
11/06/13   rk    Fixed minutes rounding error - displayed as 60 instead 59.99x.
01/20/10   rb    Populate Magnetic Deviation field in RMC string
06/10/09   gk      Added Get UTC offset from NV
08/25/08   mr    Added Geoidal seperation table to convert HAE to MSL
08/06/08   jlp   Changed NMEA output to report C/No at the antenna instead
                 of the baseband reading.
01/08/08   lt    Moved the converstion to UTC time into a TM util. function
05/23/07   mr    Added code to generate proprietary NMEA string
08/07/06   mr    File created
===========================================================================*/

#include "gps_variation.h"
#include "customer.h"  /* Customer configuration file */
#include "cgps_api.h"
#include "sm_api.h"
#include "tm_nmea.h"
#include "tm_common.h"
#include "aries_os_api.h"
#include "tm_pdapi_client.h"
#include "tm_data.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <locale.h>
#include "location_common.h"
#include "comdef.h"
#include <inttypes.h>
#include "gps_common.h"

#include "sio.h"
#include "dsm.h"
#include "task.h"

#ifdef FEATURE_RUNTIME_DEVMAP
#include "rdevmap.h"
#endif

#ifdef FEATURE_CGPS_LBS_TASK
#include "lbs_pdapi_iface.h"
#endif

#include "sm_util.h"

/* Constant definitions:
*/
#define TM_NMEA_MAX_NMEA_STRING_SIZE 640
#define TM_NMEA_GGA_MAX_SV_COUNT     12


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/


/*---------------------------------------------------------------------------
  Request packets are sent from the DM to the mobile.  Request packet is 
  reconstructed from pieces received from SIO.
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Data structures needed for communicating with SIO.
---------------------------------------------------------------------------*/
int tm_nmea_sio_flag = FALSE;

#ifndef __GRIFFON__
sio_stream_id_type       tm_nmea_sio_stream_id = SIO_NO_STREAM_ID;
/* Stream ID returned by 
   sio_open                   */
#endif
LOCAL dsm_watermark_type tm_nmea_to_sio_wmq;     /* WaterMark Queue used when
                                                 data is transmitted to SIO */
LOCAL q_type             tm_nmea_to_sio_q;       /* Queue around which
                                                 tm_nmea_to_sio_wmq is built   */
#ifndef __GRIFFON__
sio_open_type            tm_nmea_open;           /* Data structure used in 
                                                 opening SIO stream.        */
#endif
tm_nmea_diag_open_port_id_e_type  tm_nmea_diag_port_to_open   = TM_NMEA_DIAG_OPEN_PORT_NONE;

/*---------------------------------------------------------------------------
  Global variables
---------------------------------------------------------------------------*/
static uint32  q_LastGPSMeasRefCount;
static uint32  q_LastGLOMeasRefCount;
static uint32  q_LastGALMeasRefCount;
static uint32  q_LastBDSMeasRefCount;
static uint32  q_LastQZSSMeasRefCount;
static uint32  q_LastNAVICMeasRefCount;
static boolean b_IsFreshGPSMeas;
static boolean b_IsFreshGLOMeas;
static boolean b_IsFreshGALMeas;
static boolean b_IsFreshBDSMeas;
static boolean b_IsFreshQZSSMeas;
static boolean b_IsFreshNAVICMeas;

/*===========================================================================
  FUNCTION PROTOTYPES
===========================================================================*/
static boolean tm_nmea_is_differential_fix(const sm_FixInfoStructType *p_fix);
static boolean tm_nmea_is_propagated_fix(const sm_FixInfoStructType *p_fix);

/*===========================================================================

FUNCTION tm_nmea_send_message_no_payload

DESCRIPTION
 Send IPC to TM task with no IPC payload.

DEPENDENCIES
None.

RETURN VALUE
None.  This procedure does not return.

SIDE EFFECTS
None.

===========================================================================*/
void tm_nmea_send_message_no_payload(uint32 q_ipcMsg)
{
  os_IpcMsgType              *p_ipc_msg;

  /*No payload required for this callback message*/
  p_ipc_msg = os_IpcCreate(0, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipc_msg != NULL)
  {
    /* Set the msg id and init the command which failed. */
    p_ipc_msg->q_MsgId = q_ipcMsg;

    if (!os_IpcSend(p_ipc_msg, (uint32)THREAD_ID_SM_TM))
    {
      (void)os_IpcDelete(p_ipc_msg);
      TM_ERROR_1("Failed to send %d IPC", q_ipcMsg);
    }
  } /* End p_ipc_msg */
  else
  {
    TM_ERROR_1("Failed to create %d IPC", q_ipcMsg);
  } /* End p_ipc_msg */
}


#ifdef FEATURE_RUNTIME_DEVMAP
/*===========================================================================

FUNCTION tm_nmea_open_cb

DESCRIPTION
  This callback is called when RDM wants NMEA to open a port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_open_cb(sio_port_id_type port_id)
{
  os_IpcMsgType              *p_ipc_msg;

  TM_MED_1("Received RDM OPEN CB for port %d", (uint32)port_id);
  /*No payload required for this callback message*/
  p_ipc_msg = os_IpcCreate(sizeof(sio_port_id_type), IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipc_msg != NULL)
  {
    /* Set the msg id and init the command which failed. */
    p_ipc_msg->q_MsgId = TM_CORE_NMEA_MSG_ID_SIO_OPEN_CB;
    p_ipc_msg->q_Size  = sizeof(sio_port_id_type);
    memscpy(p_ipc_msg->p_Data, p_ipc_msg->q_Size, &port_id, sizeof(sio_port_id_type));
    p_ipc_msg->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;

    if (!os_IpcSend(p_ipc_msg, (uint32)THREAD_ID_SM_TM))
    {
      (void)os_IpcDelete(p_ipc_msg);
      TM_ERROR_0("Failed to send RDM OPEN CB IPC mesasge");
    }
  } /* End p_ipc_msg */
  else
  {
    TM_ERROR_0("Failed to create RDM OPEN CB IPC message");
  } /* End p_ipc_msg */

}

/*===========================================================================

FUNCTION tm_nmea_close_cb

DESCRIPTION
  This callback is called when RDM wants NMEA to close its port.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_close_cb(void)
{
  os_IpcMsgType              *p_ipc_msg;

  TM_MED_0("Received RDM CLOSE CB");
  tm_nmea_open.port_id = SIO_PORT_NULL;

  /*No payload required for this callback message*/
  p_ipc_msg = os_IpcCreate(0, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

  if (p_ipc_msg != NULL)
  {
    /* Set the msg id and init the command which failed. */
    p_ipc_msg->q_MsgId = TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB;

    if (!os_IpcSend(p_ipc_msg, (uint32)THREAD_ID_SM_TM))
    {
      (void)os_IpcDelete(p_ipc_msg);
      TM_ERROR_0("Failed to send RDM OPEN CB IPC message");
    }
  } /* End p_ipc_msg */
  else
  {
    TM_ERROR_0("Failed to create RDM OPEN CB IPC message");
  } /* End p_ipc_msg */

  rdm_notify(RDM_NMEA_SRVC, RDM_DONE_S);
}

/*===========================================================================

FUNCTION tm_nmea_close

DESCRIPTION
  This callback is closes the NMEA connection, if there is one.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tm_nmea_close(void (*close_func_ptr)(void))
{
  TM_MED_1("NMEA Close Port: %d", (int32)tm_nmea_sio_stream_id);

  if (tm_nmea_sio_stream_id != SIO_NO_STREAM_ID)
  {
    sio_close(tm_nmea_sio_stream_id, close_func_ptr);
    tm_nmea_sio_flag      = FALSE;
    tm_nmea_sio_stream_id = SIO_NO_STREAM_ID;
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_nmea_sio_close_cb

DESCRIPTION
  This callback is closes the NMEA connection, if there is one.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_sio_close_cb(void)
{
  TM_MED_0("Received SIO CLOSE CB");
  tm_nmea_send_message_no_payload(TM_CORE_NMEA_MSG_ID_SIO_CLOSE_CB_2);
}

/*===========================================================================

FUNCTION tm_nmea_open_handler_part2

DESCRIPTION
  This function is used to open NMEA message output port.

RETURN VALUE
  None

DEPENDENCIES

===========================================================================*/
void tm_nmea_open_handler_part2(void)
{
  TM_MED_0("NMEA open handler part2");
  tm_nmea_sio_stream_id = sio_open(&tm_nmea_open);

  if (tm_nmea_sio_stream_id != SIO_NO_STREAM_ID)
  {
    tm_nmea_sio_flag = TRUE;
    rdm_notify(RDM_NMEA_SRVC, RDM_DONE_S);
  }
  else
  {
    tm_nmea_sio_flag = FALSE;
    rdm_notify(RDM_NMEA_SRVC, RDM_NOT_ALLOWED_S);
  }
}

/*===========================================================================

FUNCTION tm_nmea_open_handler

DESCRIPTION
This function is used to open NMEA message output port.

RETURN VALUE
None

DEPENDENCIES

===========================================================================*/
void tm_nmea_open_handler(void *p_data)
{
  TM_MED_0("RDM OPEN CB handler called");

  if (NULL == p_data)
  {
    TM_ERROR_0("Null pointer argument");
    return;
  }
  /* We were told to open, so try to do so.  Start by closing 
  ** any existing connection.
  */

  /* save the port to open once close callback is called */
  tm_nmea_open.port_id = *((sio_port_id_type *)p_data);

  TM_MED_1("RDM OPEN CB handler port: %d", (int32)tm_nmea_open.port_id);

  if (!tm_nmea_close(tm_nmea_sio_close_cb))
  {
    tm_nmea_open_handler_part2();
  }
}

/*===========================================================================

FUNCTION tm_nmea_close_handler

DESCRIPTION
  This function is used to close NMEA message's output port .

RETURN VALUE
  None

DEPENDENCIES

===========================================================================*/
void tm_nmea_close_handler(void)
{
  TM_MED_0("RDM CLOSE CB handler called");
  /* We were told to close, so close.
  */
  tm_nmea_close(NULL);
}
#endif /* FEATURE_RUNTIME_DEVMAP */



#ifndef __GRIFFON__

/*===========================================================================

FUNCTION tm_nmea_sio_init

DESCRIPTION
  This function contains the initialization for the NMEA sio.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_sio_init(void) /* Not used by diag_task */
{
  /* Set up Queue for sending data to SIO for transmission.
  */
  (void)q_init(&tm_nmea_to_sio_q);
  tm_nmea_to_sio_wmq.q_ptr                = &tm_nmea_to_sio_q;

  /* Set up watermarks for WaterMark Queue.  Also, set-up
  ** functions which will be called when the watermarks are
  ** reached.

  ** While NMEA's own flow control is handled by the new DSM's
  ** flow management table, SIO still uses this watermark to store
  ** DSM items when the FIFO is full.
  */
  tm_nmea_to_sio_wmq.lo_watermark         = DSM_DS_SMALL_ITEM_SIZ; /*lint !e26 !e10 */
  tm_nmea_to_sio_wmq.hi_watermark         = 100;
  tm_nmea_to_sio_wmq.dont_exceed_cnt      = 100 * 5;
  tm_nmea_to_sio_wmq.gone_empty_func_ptr  = NULL;
  tm_nmea_to_sio_wmq.non_empty_func_ptr   = NULL;
  tm_nmea_to_sio_wmq.lowater_func_ptr     = NULL;
  tm_nmea_to_sio_wmq.hiwater_func_ptr     = NULL;

  /* WaterMark Queue starts out empty.
  */
  tm_nmea_to_sio_wmq.current_cnt          = 0;

  /* dsm_init() is commented out because it gets called in TMC at bootup */
  /* Calling it twice causes System instability since dsm_init() cannot 
   * handle multiple inits */
   //    dsm_init();

   /*DSM already takes care that if “dsm_queue_init” is called multiple times,
     only first time the WMQ is initialized and the next time, it takes care not to create memory leaks
     hence check for tm_nmea_to_sio_wmq.lock is not needed*/
  dsm_queue_init(&tm_nmea_to_sio_wmq,
  (int)tm_nmea_to_sio_wmq.dont_exceed_cnt, &tm_nmea_to_sio_q ); 

  /* Prepare SIO opening parameters. */
  tm_nmea_open.stream_mode    = SIO_GENERIC_MODE;
  tm_nmea_open.tx_queue       = &tm_nmea_to_sio_wmq;

  tm_nmea_open.port_id        = SIO_PORT_NULL;

  /* SIO is configured to look for tail character.
  */
  tm_nmea_open.tail_char_used = TRUE;
  tm_nmea_open.tail_char      = 0x07e;
  tm_nmea_open.rx_func_ptr    = NULL;

  /* Use whatever the flow control method is currently.
  */
  tm_nmea_open.rx_flow        = SIO_FCTL_OFF;
  tm_nmea_open.tx_flow        = SIO_FCTL_OFF;

  /* Use 115.2Kbps for now
  */
  tm_nmea_open.rx_bitrate     = SIO_BITRATE_115200;
  tm_nmea_open.tx_bitrate     = SIO_BITRATE_115200;

  /* Initialize the structure lconv for snprintf */
  (void)setlocale(LC_ALL, "C");

  return;
}
/*===========================================================================

FUNCTION tm_nmea_sio_transmit

DESCRIPTION
  This function contains the transmit call for the NMEA sio.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_sio_transmit(char *p_str)
{
  dsm_mempool_id_enum_type   dsm_pool_id;
  dsm_item_type *tx_item_ptr = NULL; /* Packet into which outgoing packet */

  /* Allocate a DSM item */
  if (tx_item_ptr == NULL)
  {
    dsm_pool_id = DSM_DS_POOL_SIZE(TM_NMEA_MAX_NMEA_STRING_SIZE);

    tx_item_ptr = dsm_new_buffer(dsm_pool_id);

    if (tx_item_ptr == NULL)
    {
      TM_ERROR_0("Ran out of DSM buffers");
      return;
    }
    else
    {
      tx_item_ptr->used = 0;
    }
  }

  /* Copy data to tx_item_ptr */
  while (*p_str != '\0')
  {
    tx_item_ptr->data_ptr[tx_item_ptr->used++] = *p_str;
    p_str++;
  }
  if (tm_nmea_sio_stream_id != SIO_NO_STREAM_ID)
  {
    sio_transmit(tm_nmea_sio_stream_id, tx_item_ptr);
  }
  else
  {
    (void)dsm_free_buffer(tx_item_ptr);
  }
  tx_item_ptr = NULL;

  return;
}


#endif  // __GRIFFON__


/*===========================================================================
FUNCTION tm_nmea_port_put

DESCRIPTION
  tm_nmea_port_put() puts the constructed NMEA string out to a serial port 
  (to be picked up by Street Atlas, for example).

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
 
===========================================================================*/
void tm_nmea_port_put(uint8 u_Port, char *p_str)
{
  /* dont send NMEA out for on demand sessions and internal sessions */
  if (tm_core_is_internal_session_on() != TRUE)
  {
    if (tm_nmea_sio_flag == TRUE)
    {

#ifndef __GRIFFON__

      tm_nmea_sio_transmit(p_str);

#endif  // __GRIFFON__

    }
  }
}
/* End of tm_nmea_port_put(). */


/*===========================================================================
FUNCTION tm_nmea_pdapi_send

DESCRIPTION
  tm_nmea_pdapi_send() puts the constructed NMEA sends the NMEA strings on
  PDAPI interface.

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
 
===========================================================================*/
void tm_nmea_pdapi_send(char *p_str, pdsm_pa_nmea_type e_nmea_type)
{
  pdsm_ext_status_info_s_type *pz_z_ext_status_info_type = NULL;

  if (NULL == p_str)
  {
    TM_ERROR_0("NULL NMEA string received for PDAPI send");
    return;
  }

  pz_z_ext_status_info_type = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_z_ext_status_info_type)
  {
    TM_ERROR_0("Cannot allocate memory for pdsm_ext_status_info_s_type in tm_nmea_pdapi_send");
    return;
  }
  memset(pz_z_ext_status_info_type, 0, sizeof(*pz_z_ext_status_info_type));


  pz_z_ext_status_info_type->client_id = tm_core_info.session_info.pd_param.client_id;
  pz_z_ext_status_info_type->ext_status_info.ext_nmea_pos_report_type.nmea_type = e_nmea_type;
  pz_z_ext_status_info_type->ext_status_info.ext_nmea_pos_report_type.nmea_length = strlen(p_str);
  pz_z_ext_status_info_type->ext_status_type = PDSM_EXT_STATUS_NMEA_POS_REPORT;
  (void)GNSS_STRLCPY((char *)(pz_z_ext_status_info_type->ext_status_info.ext_nmea_pos_report_type.nmea_data),
                     p_str, PDSM_NMEA_DATA_LENGTH);
  
  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_NMEA_POS_REPORT, 
                                       pz_z_ext_status_info_type); 
  (void)os_MemFree((void **)&pz_z_ext_status_info_type);
}

/*===========================================================================
FUNCTION nmea_var_len_pdapi_send

DESCRIPTION
  nmea_var_len_pdapi_send() puts the constructed NMEA sends the NMEA strings
  on PDAPI interface.

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS

===========================================================================*/
static void nmea_var_len_pdapi_send(sm_DebugNMEAVarLen *pDebugNMEA)
{
  pdsm_ext_status_info_s_type *pz_z_ext_status_info_type = NULL;

  if (NULL == pDebugNMEA)
  {
    TM_ERROR_0("NULL NMEA string received for PDAPI send");
    return;
  }

  pz_z_ext_status_info_type = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_z_ext_status_info_type)
  {
    TM_ERROR_0("Cannot allocate memory for pdsm_ext_status_info_s_type in tm_nmea_pdapi_send");
    return;
  }
  memset(pz_z_ext_status_info_type, 0, sizeof(*pz_z_ext_status_info_type));

  pz_z_ext_status_info_type->client_id = tm_core_info.session_info.pd_param.client_id;
  /* Explicitly set the NMEA type to given value. PE may not set this field. */
  pz_z_ext_status_info_type->ext_status_info.ext_nmea_var_len_pos_report_type = *pDebugNMEA;
  pz_z_ext_status_info_type->ext_status_type = PDSM_EXT_STATUS_NMEA_POS_REPORT;

  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_NMEA_POS_REPORT,
                                     pz_z_ext_status_info_type);
  (void)os_MemFree((void **)&pz_z_ext_status_info_type);
}

/*===========================================================================
FUNCTION tm_nmea_sentenceTerminate
 
DESCRIPTION
  tm_nmea_sentenceTerminate() is used to append the NMEA sentence with the 
  standard terminator. This consists of a "*0x2.2X\r\n" fragment. 

 'The checksum is the 8-bit exclusive OR (no start or stop bits) of all the
  characters in the sentence, including the "," delimiters, between but not
  including the "$" and the "*" delimiters.'

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
 
===========================================================================*/
static void tm_nmea_sentenceTerminate(char *p_string)
{
  uint8 u_Checksum = 0;

  p_string++;                     /* Skip the $ token. */

  while (*p_string != '\0')      /* Scan up to the end of the sentence. */
  {
    u_Checksum ^= *p_string++;
  }

  (void)snprintf(p_string, 200, "*%02X\r\n", u_Checksum);

} /* End of tm_nmea_sentenceTerminate(). */

/*===========================================================================
FUNCTION tm_nmea_terminate_and_put
 
DESCRIPTION
  tm_nmea_terminate_and_put() is used to transmit the nmea message after
  terminating the nmea sentence

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
 
===========================================================================*/
static void tm_nmea_terminate_and_put(char *p_str, pdsm_pa_nmea_type e_nmea_type)
{
  /* Append the "*CS\r\n" to the end of the sentenvce */
  tm_nmea_sentenceTerminate(p_str);
  /* Transmit the constructed sentence */
  tm_nmea_port_put(0, p_str);
  /*Send NMEA string over to PDAPI*/
  tm_nmea_pdapi_send(p_str,e_nmea_type);
}

/*===========================================================================
FUNCTION tm_nmea_empty_put
 
DESCRIPTION
  tm_nmea_empty_put() is used to prepare and transmit the empty message 

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
static void tm_nmea_empty_put(pdsm_pa_nmea_type e_nmea_type)
{
  char b_sentence[200], *p_str = b_sentence;
  int q_len = 0, q_buf_len = sizeof(b_sentence);

  switch (e_nmea_type)
  {
    case PDAPI_NMEA_GPGGA:
      q_len = snprintf(p_str, q_buf_len, "$GPGGA,,,,,,0,,,,,,,,");
      break;
    case PDAPI_NMEA_GPRMC:
      q_len = snprintf(p_str, q_buf_len, "$GPRMC,,V,,,,,,,,,,N,V");
      break;
    case PDAPI_NMEA_GPGSV:
      q_len = snprintf(p_str, q_buf_len, "$GPGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GPGSA:
      q_len = snprintf(p_str, q_buf_len, "$GPGSA,A,1,,,,,,,,,,,,,,,,");
      break;
    case PDAPI_NMEA_GPVTG:
      q_len = snprintf(p_str, q_buf_len, "$GPVTG,,T,,M,,N,,K,N");
      break;
    case PDAPI_NMEA_GLGSV:
      q_len = snprintf(p_str, q_buf_len, "$GLGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GNGSA:
      q_len = snprintf(p_str, q_buf_len, "$GNGSA,A,1,,,,,,,,,,,,,,,,");
      break;
    case PDAPI_NMEA_GNGNS:
      q_len = snprintf(p_str, q_buf_len, "$GNGNS,,,,,,NNN,,,,,,,V");
      break;
    case PDAPI_NMEA_GNGGA:
      q_len = snprintf(p_str, q_buf_len, "$GNGGA,,,,,,0,,,,,,,,");
      break;
    case PDAPI_NMEA_GNVTG:
      q_len = snprintf(p_str, q_buf_len, "$GNVTG,,T,,M,,N,,K,N");
      break;
    case PDAPI_NMEA_GNRMC:
      q_len = snprintf(p_str, q_buf_len, "$GNRMC,,V,,,,,,,,,,N");
      break;
    case PDAPI_NMEA_PQGSA:
      q_len = snprintf(p_str, q_buf_len, "$PQGSA,A,1,,,,,,,,,,,,,,,,");
      break;
    case PDAPI_NMEA_PQGSV:
      q_len = snprintf(p_str, q_buf_len, "$PQGSV,1,1,0,");
      break;
    case PDAPI_NMEA_PQXFI:
      q_len = snprintf(p_str, q_buf_len, "$PQXFI,,,,,,,,,,");
      break;
    case PDAPI_NMEA_PSTIS:
      q_len = snprintf(p_str, q_buf_len, "$PSTIS,");
      break;
    case PDAPI_NMEA_GAGGA:
      q_len = snprintf(p_str, q_buf_len, "$GAGGA,,,,,,0,,,,,,,,");
      break;
    case PDAPI_NMEA_GARMC:
      q_len = snprintf(p_str, q_buf_len, "$GARMC,,V,,,,,,,,,,N,V");
      break;
    case PDAPI_NMEA_GAGSV:
      q_len = snprintf(p_str, q_buf_len, "$GAGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GAGSA:
      q_len = snprintf(p_str, q_buf_len, "$GAGSA,A,1,,,,,,,,,,,,,,,,");
      break;
    case PDAPI_NMEA_GAVTG:
      q_len = snprintf(p_str, q_buf_len, "$GAVTG,,T,,M,,N,,K,N");
      break;
    case PDAPI_NMEA_GPDTM:
      q_len = snprintf(p_str, q_buf_len, "$GPDTM,P90,,,N,,W,,W84");
      break;
    case PDAPI_NMEA_GAGNS:
      q_len = snprintf(p_str, q_buf_len, "$GAGNS,,,,,,NNN,,,,,,,V");
      break;
    case PDAPI_NMEA_GNDTM:
      q_len = snprintf(p_str, q_buf_len, "$GNDTM,P90,,,N,,W,,W84");
      break;
    case PDAPI_NMEA_GBGGA:
      q_len = snprintf(p_str, q_buf_len, "$GBGGA,,,,,,0,,,,,,,,");
      break;
    case PDAPI_NMEA_GBGSA:
      q_len = snprintf(p_str, q_buf_len, "$GBGSA,A,1,,,,,,,,,,,,,,,,");
      break;
    case PDAPI_NMEA_GBGSV:
      q_len = snprintf(p_str, q_buf_len, "$GBGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GQGSV:
      q_len = snprintf(p_str, q_buf_len, "$GQGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GIGSV:
      q_len = snprintf(p_str, q_buf_len, "$GIGSV,1,1,0,");
      break;
    case PDAPI_NMEA_GBRMC:
      q_len = snprintf(p_str, q_buf_len, "$GBRMC,,V,,,,,,,,,,N,V");
      break;
    case PDAPI_NMEA_GBVTG:
      q_len = snprintf(p_str, q_buf_len, "$GBVTG,,T,,M,,N,,K,N");
      break;
    default:
      TM_ERROR_1("NMEA type %d not supported", e_nmea_type);
      break;
  }

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_str);
  tm_nmea_terminate_and_put(b_sentence, e_nmea_type);
}

/******************************************************************************
@brief      Function to initialize NMEA sentence

@param[in]  Pointer to buffer
@param[in]  Length of buffer
@param[in]  NMEA sentence to be initialized

@return     Number of bytes written to buffer
******************************************************************************/
static int tm_nmea_init_sentence(char *p_str, int q_buf_len, pdsm_pa_nmea_type e_nmea_type)
      {
  int q_len = -1;

  if ((NULL == p_str) || (q_buf_len <= 0))
      {
    TM_ERROR_1("NULL Pointer or Invalid Length %d", q_buf_len);
    return q_len;
  }

  switch (e_nmea_type)
  {
    case PDAPI_NMEA_GPGGA:
      q_len = snprintf(p_str, q_buf_len, "$GPGGA,");
      break;
    case PDAPI_NMEA_GPRMC:
      q_len = snprintf(p_str, q_buf_len, "$GPRMC,");
      break;
    case PDAPI_NMEA_GPGSV:
      q_len = snprintf(p_str, q_buf_len, "$GPGSV,");
      break;
    case PDAPI_NMEA_GPGSA :
      q_len = snprintf(p_str, q_buf_len, "$GPGSA,");
      break;
    case PDAPI_NMEA_GPVTG:
      q_len = snprintf(p_str, q_buf_len, "$GPVTG,");
      break;
    case PDAPI_NMEA_GLGSV:
      q_len = snprintf(p_str, q_buf_len, "$GLGSV,");
      break;
    case PDAPI_NMEA_GNGSA:
      q_len = snprintf(p_str, q_buf_len, "$GNGSA,");
      break;
    case PDAPI_NMEA_GNGNS:
      q_len = snprintf(p_str, q_buf_len, "$GNGNS,");
      break;
    case PDAPI_NMEA_GNGGA:
      q_len = snprintf(p_str, q_buf_len, "$GNGGA,");
      break;
    case PDAPI_NMEA_GNVTG:
      q_len = snprintf(p_str, q_buf_len, "$GNVTG,");
      break;
    case PDAPI_NMEA_GNRMC:
      q_len = snprintf(p_str, q_buf_len, "$GNRMC,");
      break;
    case PDAPI_NMEA_PQGSA:
      q_len = snprintf(p_str, q_buf_len, "$PQGSA,");
      break;
    case PDAPI_NMEA_PQGSV:
      q_len = snprintf(p_str, q_buf_len, "$PQGSV,");
      break;
    case PDAPI_NMEA_PQXFI:
      q_len = snprintf(p_str, q_buf_len, "$PQXFI,");
      break;
    case PDAPI_NMEA_PSTIS:
      q_len = snprintf(p_str, q_buf_len, "$PSTIS,");
      break;
    case PDAPI_NMEA_GAGGA:
      q_len = snprintf(p_str, q_buf_len, "$GAGGA,");
      break;
    case PDAPI_NMEA_GARMC:
      q_len = snprintf(p_str, q_buf_len, "$GARMC,");
      break;
    case PDAPI_NMEA_GAGSV:
      q_len = snprintf(p_str, q_buf_len, "$GAGSV,");
      break;
    case PDAPI_NMEA_GAGSA:
      q_len = snprintf(p_str, q_buf_len, "$GAGSA,");
      break;
    case PDAPI_NMEA_GAVTG:
      q_len = snprintf(p_str, q_buf_len, "$GAVTG,");
      break;
    case PDAPI_NMEA_GPDTM:
      q_len = snprintf(p_str, q_buf_len, "$GPDTM,");
      break;
    case PDAPI_NMEA_GAGNS:
      q_len = snprintf(p_str, q_buf_len, "$GAGNS,");
      break;
    case PDAPI_NMEA_GNDTM:
      q_len = snprintf(p_str, q_buf_len, "$GNDTM,");
      break;
    case PDAPI_NMEA_GBGGA:
      q_len = snprintf(p_str, q_buf_len, "$GBGGA,");
      break;
    case PDAPI_NMEA_GBGSA:
      q_len = snprintf(p_str, q_buf_len, "$GBGSA,");
      break;
    case PDAPI_NMEA_GBGSV:
      q_len = snprintf(p_str, q_buf_len, "$GBGSV,");
      break;
    case PDAPI_NMEA_GQGSV:
      q_len = snprintf(p_str, q_buf_len, "$GQGSV,");
      break;
    case PDAPI_NMEA_GIGSV:
      q_len = snprintf(p_str, q_buf_len, "$GIGSV,");
      break;
    case PDAPI_NMEA_GBRMC:
      q_len = snprintf(p_str, q_buf_len, "$GBRMC,");
      break;
    case PDAPI_NMEA_GBVTG:
      q_len = snprintf(p_str, q_buf_len, "$GBVTG,");
      break;
    default:
      TM_ERROR_1("NMEA type %d not supported", e_nmea_type);
      break;
  }

  return q_len;
  }

/*===========================================================================
FUNCTION tm_nmea_get_gps_fix_from_refpos

This function takes a reference position of type gps_RefLocStructType and
populates a sm_GpsFixRptStructType with only the fields required to generate 
NMEA strings. This is used in case of MSA where the final fix from the server
doesn't contain all the information required to generate NMEA.
   
DESCRIPTION

DEPENDENCIES

RETURN VALUE sm_GpsFixRptStructType* 

SIDE EFFECTS
 
===========================================================================*/
void tm_nmea_get_gps_fix_from_refpos(const gps_RefLocStructType *p_refpos,
                                     sm_FixInfoStructType *p_fix)
{
  uint32 q_LeapSeconds = 0;

  if (p_refpos == NULL || p_fix == NULL)
  {
    return;
  }

  memset(p_fix, 0, sizeof(sm_FixInfoStructType));
  p_fix->z_NavPos.z_PosFlags.b_IsValid = TRUE;
  /* Compute UTC time of fix, adjusting for the GPS leap seconds. */
  q_LeapSeconds = cgps_GetNvUtcOffset();

  if (p_refpos->q_GpsMsec / 1000L < q_LeapSeconds)
  {
    p_fix->z_NavPos.q_UtcTimeMs = p_refpos->q_GpsMsec +
       (WEEK_SECS - q_LeapSeconds) * 1000L;
  }
  else
  {
    p_fix->z_NavPos.q_UtcTimeMs = p_refpos->q_GpsMsec - q_LeapSeconds * 1000L;
  }

  p_fix->z_NavPos.d_PosLla[0] = p_refpos->d_Latitude;
  p_fix->z_NavPos.d_PosLla[1] = p_refpos->d_Longitude;

  p_fix->z_NavPos.f_PuncLatLonMeters[0] = p_refpos->f_LocUncrtntySemiMajor;
  p_fix->z_NavPos.f_PuncLatLonMeters[1] = p_refpos->f_LocUncrtntySemiMinor;

  if (p_refpos->u_AltitudeIsValid)
  {
    p_fix->z_NavPos.d_PosLla[2] = (DBL)p_refpos->f_Altitude;
  }

  p_fix->z_NavPos.z_PosFlags.b_Is3D = TRUE;
  p_fix->z_NavPos.z_PosFlags.b_IsExternal = TRUE;

  if (p_refpos->u_VelocityIsValid && p_refpos->u_HeadingIsValid)
  {
    p_fix->z_NavPos.z_PosFlags.b_IsVelValid = TRUE;
    p_fix->z_NavPos.f_VelEnu[0] = p_refpos->f_VelHor * cos(p_refpos->f_Heading);
    p_fix->z_NavPos.f_VelEnu[1] = p_refpos->f_VelHor * sin(p_refpos->f_Heading);
    p_fix->z_NavPos.f_VelEnu[2] = p_refpos->f_VelVert;

    /*Calculate Heading here*/
    if (p_fix->z_NavPos.f_VelEnu[0] != 0 || p_fix->z_NavPos.f_VelEnu[1] != 0)
    {
      FLT d_azimuth;

      d_azimuth  =  (FLT)atan2(p_fix->z_NavPos.f_VelEnu[0], p_fix->z_NavPos.f_VelEnu[1]);
      if (d_azimuth < (FLT)0.0)
      {
        d_azimuth += (FLT)(2.0 * C_PI);
      }

      p_fix->z_NavPos.f_HeadingRad = d_azimuth;
      p_fix->z_NavPos.z_PosFlags.b_IsHeadingValid = TRUE;
    }
    else
    {
      p_fix->z_NavPos.f_HeadingRad = 0;
    }
  }

  p_fix->z_NavPos.w_GpsWeek   = p_refpos->w_GpsWeek;
  p_fix->z_NavPos.q_GpsTimeMs = p_refpos->q_GpsMsec;

  /*Information not provided by protocol sub-modules. 
    Set default values*/
  memset((void *)&p_fix->z_NavSvInfo, 0, sizeof(p_fix->z_NavSvInfo));

  p_fix->z_NavPos.f_HDOP = 0.1;
  p_fix->z_NavPos.f_VDOP = 0.1;
  p_fix->z_NavPos.f_PDOP = 0.14142; /*PDOP = sqrt( HDOP*HDOP + VDOP*VDOP )*/
  p_fix->z_NavPos.f_TDOP = 0.1;
  p_fix->z_NavPos.f_GDOP = 0.1;  /*Initialising with default value as done with HDOP and VDOP */

} /*tm_nmea_get_gps_fix_from_refpos*/

/******************************************************************************
@brief      Utility function to get SV ID for NMEA sentences
******************************************************************************/
static uint16 tm_nmea_get_svid(uint16 w_svId)
{
  uint16 w_offset = 0;

  /* GPS, SBAS and GLO Offsets are 0 */
  if (GAL_ID_OK(w_svId))
    w_offset = GAL_SV_ID_OFFSET;

  else if (BDS_ID_OK(w_svId))
    w_offset = BDS_SV_ID_OFFSET;
  else if (QZSS_PRN_OK(w_svId))
    w_offset = QZSS_SV_ID_OFFSET;
  else if (QZSSL1S_PRN_OK(w_svId))
    w_offset = QZSSL1S_SV_ID_OFFSET;
  else if (NAVIC_PRN_OK(w_svId))
    w_offset = NAVIC_SV_ID_OFFSET;

  return (w_svId - w_offset);
}

/******************************************************************************
@brief      Utility function to get GNSS Signal ID for NMEA based on
            the source of measurement block
******************************************************************************/
static tm_nmea_signal_id_e_type tm_nmea_get_signal_id(const sm_GpsMeasRptStructType *p_meas_report)
{
  tm_nmea_signal_id_e_type e_RetVal = TM_NMEA_SIGNAL_ID_ALL;
  if (NULL == p_meas_report)
{
    TM_ERROR_0("tm_nmea_get_signal_id() : NULL MeasBlkPtr. Returning TM_NMEA_SIGNAL_ID_ALL");
    return e_RetVal;
}

  switch (p_meas_report->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)
{
    case GNSS_MEAS_BLK_SRC_GPS:
      e_RetVal = TM_NMEA_SIGNAL_ID_GPS_L1_CA;
      break;
    case GNSS_MEAS_BLK_SRC_GPS_L2C:
      e_RetVal = TM_NMEA_SIGNAL_ID_GPS_L2C_L;
      break;
    case GNSS_MEAS_BLK_SRC_GPS_L5Q:
      e_RetVal = TM_NMEA_SIGNAL_ID_GPS_L5_Q;
      break;
    case GNSS_MEAS_BLK_SRC_GLO:
      e_RetVal = TM_NMEA_SIGNAL_ID_GLONASS_G1_CA;
      break;
    case GNSS_MEAS_BLK_SRC_GLO_G2:
      e_RetVal = TM_NMEA_SIGNAL_ID_GLONASS_G2_CA;
      break;
    case GNSS_MEAS_BLK_SRC_BDS:
      e_RetVal = TM_NMEA_SIGNAL_ID_BDS_B1_I;
      break;
    case GNSS_MEAS_BLK_SRC_BDS_B2:
      e_RetVal = TM_NMEA_SIGNAL_ID_BDS_B2_Q;
      break;
    case GNSS_MEAS_BLK_SRC_BDS_B2AQ:
      e_RetVal = TM_NMEA_SIGNAL_ID_BDS_B2_A;
      break;
    case GNSS_MEAS_BLK_SRC_BDS_B1CQ:
      e_RetVal = TM_NMEA_SIGNAL_ID_BDS_B1_Q;
      break;
    case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
      if (QZSS_PRN_OK(p_meas_report->z_MeasBlk.z_Gnss[0].w_Sv))
  {
        e_RetVal = TM_NMEA_SIGNAL_ID_QZSS_L1_CA;
  }
      else // SBAS
  {
        e_RetVal = TM_NMEA_SIGNAL_ID_GPS_L1_CA;
      }
      break;
    case GNSS_MEAS_BLK_SRC_GAL:
      e_RetVal = TM_NMEA_SIGNAL_ID_GALILEO_L1_BC;
      break;
    case GNSS_MEAS_BLK_SRC_GAL_E5A:
      e_RetVal = TM_NMEA_SIGNAL_ID_GALILEO_E5_A;
      break;
    case GNSS_MEAS_BLK_SRC_QZSS_L2C:
      e_RetVal = TM_NMEA_SIGNAL_ID_QZSS_L2C_L;
      break;
    case GNSS_MEAS_BLK_SRC_QZSS_L5Q:
      e_RetVal = TM_NMEA_SIGNAL_ID_QZSS_L5_Q;
      break;
    case GNSS_MEAS_BLK_SRC_NAVIC:
      e_RetVal = TM_NMEA_SIGNAL_ID_NAVIC_L5_SPS;
      break;
    default:
      TM_ERROR_1("tm_nmea_get_signal_id(). Invalid Meas %d. Default to ALL",
                 p_meas_report->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc);
      break;
    }
  return e_RetVal;
  }

/*
******************************************************************************
* FUNCTION:
* tm_nmea_get_altitude_and_geoidal_separation
*
* DESCRIPTION:
*  This function calculates geoidal separation and altitude in meters using
*  position fix report.
*
* PARAMETERS:
*  @param[in] Pointer to Fix Info
*  @param[out] altitude in meters
*
* RETURN VALUE:
*  float - geoidal separation
******************************************************************************
*/
static float tm_nmea_get_altitude_and_geoidal_separation(const sm_FixInfoStructType *p_fix, double *pd_altitudeOut)
  {
  float f_GeoidalSeparation = 0.0f;
  double d_latitude, d_longitude, d_altitude = 0.0;
  double d_PosEcefPZ90[3] = {0};
  double d_PosLlaPZ90[3] = {0};

  if (NULL == p_fix)
    {
    return f_GeoidalSeparation;
    }

    if(TM_CORE_CONFIG_DD( u_control_nmea_sentence ) & PDSM_PA_PZ90_DATUM_NMEA_SENTENCE)
    {
    tm_util_convert_Wgs84ToPZ90_datum(p_fix, d_PosEcefPZ90, d_PosLlaPZ90);
    d_latitude = d_PosLlaPZ90[0] * (180.0 / C_PI);
    d_longitude = d_PosLlaPZ90[1] * (180.0 / C_PI);
    d_altitude = d_PosLlaPZ90[2];
    }
  else
    {
    d_latitude = p_fix->z_NavPos.d_PosLla[0] * (180.0 / C_PI);
    d_longitude = p_fix->z_NavPos.d_PosLla[1] * (180.0 / C_PI);
    d_altitude = p_fix->z_NavPos.d_PosLla[2];
    }

  f_GeoidalSeparation = loc_GetGeoidalSeparation(d_latitude, d_longitude);

  if (NULL != pd_altitudeOut)
      {
    *pd_altitudeOut = d_altitude - f_GeoidalSeparation;
        }
  return f_GeoidalSeparation;
          }

/******************************************************************************
@brief      Utility function to get the number of GPS SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_gps_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
          {
  if (NULL == pz_svInfo)
      {
    return 0;
      }

  return (pz_svInfo->u_GpsNumSvPosFix ? pz_svInfo->u_GpsNumSvPosFix : pz_svInfo->u_GpsNumSvVelFix);
      }

/******************************************************************************
@brief      Utility function to get the number of GLO SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_glo_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
      {
  if (NULL == pz_svInfo)
      {
    return 0;
      }

  return (pz_svInfo->u_GloNumSvPosFix ? pz_svInfo->u_GloNumSvPosFix : pz_svInfo->u_GloNumSvVelFix);
    }

/******************************************************************************
@brief      Utility function to get the number of GAL SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_gal_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
    {
  if (NULL == pz_svInfo)
	{
    return 0;
	}

  return (pz_svInfo->u_GalNumSvPosFix ? pz_svInfo->u_GalNumSvPosFix : pz_svInfo->u_GalNumSvVelFix);
    }

/******************************************************************************
@brief      Utility function to get the number of BDS SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_bds_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
{
  if (NULL == pz_svInfo)
  {
    return 0;
  }

  return (pz_svInfo->u_BdsNumSvPosFix ? pz_svInfo->u_BdsNumSvPosFix : pz_svInfo->u_BdsNumSvVelFix);
  }
  
/******************************************************************************
@brief      Utility function to get the number of QZSS SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_qzss_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
  {
  if (NULL == pz_svInfo)
  {
    return 0;
  }
  
  return (pz_svInfo->u_QzssNumSvPosFix ? pz_svInfo->u_QzssNumSvPosFix : pz_svInfo->u_QzssNumSvVelFix);
  }
  
/******************************************************************************
@brief      Utility function to get the number of NAVIC SVs used for positioning
******************************************************************************/
static uint8 tm_nmea_num_navic_svs_in_fix(const sm_GnssNavSvInfo *pz_svInfo)
  {
  if (NULL == pz_svInfo)
  {
    return 0;
  }

  return (pz_svInfo->u_NavicNumSvPosFix ? pz_svInfo->u_NavicNumSvPosFix : pz_svInfo->u_NavicNumSvVelFix);
}

/*===========================================================================
FUNCTION tm_nmea_gga_put

DESCRIPTION
tm_nmea_gga_put() is used to build and transmit the NMEA GGA sentence.
 
 Global Positioning System Fix Data.
 
  Time, position and fix related data for a GPS receiver.
 
  1 2   3         4       5 6        7 8 9  10 11  12 13 14 15  16   17
  | |   |         |       | |        | | |  |   |   | |   | |   |    |
 $aaccc,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
 
 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
  3) Universal Time Coordinated (UTC) of position.
  4) Latitude  (DDmm.mm)    DD = two digits of degrees, mm.mm = minutes.
  5) N or S (North or South).
  6) Longitude (DDDmm.mm).
  7) E or W (East or West).
  8) GPS Quality Indicator,
     0 - fix not available or invalid,
     1 - GPS SPS Mode, fix valid,
     2 - Differential GPS, SPS Mode, fix valid
     3 - GPS PPS Mode, fix valid
     4 - Real Time Kinematic:  Satellite system used in RTK mode with fixed
         integers,
     5 - Float RTK:  Satellite system used in RTK mode, floating integers
     6 - Estimated (dead reckoning) Mode,
     7 - Manual Input Mode,
     8 - Simulator Mode.
  9) Number of satellites in use, 00 - 12.
 10) Horizontal Dilution of Precision (HDOP).
 11) Antenna Altitude above/below mean-sea-level (geoid), meters.
 12) Units of antenna altitude, meters.
 13) Geoidal separation, the difference between the WGS-84 earth ellipsoid and
     the mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid.
 14) Units of geoidal separation, meters.
 15) Age of differential GPS data, time in seconds since last RTCM-SC104 
     Type 1 or Type 9 update (null field when DGPS is not used).
 16) Differential reference station ID, 0000-1023.
 17) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
static void tm_nmea_gga_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix, pdsm_pa_nmea_type q_nmeaType)
{
  char b_sentence[ 200 ];
  char *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0 ;
  boolean b_utc_valid = FALSE;
  tm_CalendarTimeT z_Time;
  float f_geoidal_sep = 0.0f;
  float f_altitude = 0.0f;
  uint32 q_TotalDays = 0;
  uint8 u_Qual = 0;
  uint8 u_numSv = 0;
  double d_altitude = 0.0;

  if( p_fix == NULL )
  {
    return;
  }
       
  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, q_nmeaType);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    /*  Universal Time Coordinated (UTC) of position: */
  b_utc_valid = tm_util_get_fix_time(p_fix, &z_Time, &q_TotalDays);

  if (b_utc_valid == TRUE)
  {
    q_len = snprintf( p_b, q_buf_len,
                     "%02" PRIu32 // Hour
                     "%02" PRIu32        // Minute
                     "%02" PRIu32        // Second
                     ".%02" PRIu32 "," , // Millisecond/10
                     z_Time.q_Hour, z_Time.q_Minute,
                     z_Time.q_Second, z_Time.q_Millisecond / 10);
  }
  else
      {
    q_len = snprintf(p_b, q_buf_len, ",");
      }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  /* END - UTC of position */

  if(u_no_fix)
  {
    /* Construct the 'know nothing position sentence */
      q_len = snprintf( p_b, q_buf_len,",,,,0,,,,,,,," );  

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
      }
  else
  {
      /* Latitude and longitude: */
    q_len = tm_util_calculate_position_print(p_fix, p_b, q_buf_len);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /* END - Latitude and Longitude */

    /* Quality */
        if (p_fix->z_NavPos.z_PosFlags.b_IsValid)
        {
      if (TRUE == tm_nmea_is_differential_fix(p_fix))
          {
        u_Qual = 2;  /* DGPS fix */
          }
      else if (TRUE == tm_nmea_is_propagated_fix(p_fix))
          {
        u_Qual = 6; /* Estimated (dead reckoning) fix */
          }
          else
          {
        u_Qual = 1; /* GPS fix */
          }
        }

    /* GNGGA - Default number of SVs */
    u_numSv = tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo) +
      tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo) +
      tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo) +
      tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo) +
      tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo) +
      tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo);

    /* Override u_numSv for talker IDs other than GN */
    switch (q_nmeaType)
        {
      case PDAPI_NMEA_GPGGA:
        u_numSv = tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo);
        break;
      case PDAPI_NMEA_GAGGA:
        u_numSv = tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo);
        break;
      case PDAPI_NMEA_GBGGA:
        u_numSv = tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo);
        break;
      default:
        break;
        }
    /*Limiting the number of SV's to 12*/
    u_numSv = MIN(u_numSv, TM_NMEA_GGA_MAX_SV_COUNT);

        q_len = snprintf( p_b, q_buf_len, "%d,%02d,%.1f," , 
                     u_Qual, u_numSv, p_fix->z_NavPos.f_HDOP);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

      /* Altitude, etc.: */
      
    /*Find the Altitude and Geoidal seperation before signs are manipulated*/
    f_geoidal_sep = tm_nmea_get_altitude_and_geoidal_separation(p_fix, &d_altitude);
      q_len = snprintf( p_b, q_buf_len,"%.1lf,M,%.1f,M,," , 
                     d_altitude, f_geoidal_sep);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /* END - Altitude */

    /* TBD: AI NIKHILL
    * 15) Age of differential GPS data. AI Nikhil. This field is currently set to NULL but will be used since DGPS is supported (with QZSS L1). Need to check if ME can provide this information.
    * 16) Differential reference station ID. AI Nikhil. This field is currently set to NULL but will be used since DGPS is supported (with QZSS L1). Need to check if ME can provide this information.
       *   The "null" entries are the ,,,, portion of the string. 
      */
    }/*else for if(u_no_fix)*/

  tm_nmea_terminate_and_put(b_sentence, q_nmeaType);
}

/******************************************************************************
@brief      Utility function to generate GGA sentence for GPS
******************************************************************************/
void tm_nmea_gpgga_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
{
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GPGGA);
    return;
  }

  tm_nmea_gga_put(p_fix, u_no_fix, PDAPI_NMEA_GPGGA);
}

/******************************************************************************
@brief      Utility function to generate GGA sentence for GAL
******************************************************************************/
void tm_nmea_gagga_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
  {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GAGGA);
    return;
  }

  tm_nmea_gga_put(p_fix, u_no_fix, PDAPI_NMEA_GAGGA);
    }

/******************************************************************************
@brief      Utility function to generate GGA sentence for BDS
******************************************************************************/
void tm_nmea_gbgga_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
  {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)))
    {
    tm_nmea_empty_put(PDAPI_NMEA_GBGGA);
      return;
    }

  tm_nmea_gga_put(p_fix, u_no_fix, PDAPI_NMEA_GBGGA);
      }

/******************************************************************************
@brief      Utility function to generate GGA sentence for 
            multiple constellations (Talker ID $GN)
******************************************************************************/
void tm_nmea_gngga_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
      {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) || /* Position invalid */
      ((0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))))
      {
    tm_nmea_empty_put(PDAPI_NMEA_GNGGA);
        return;
      }

  tm_nmea_gga_put(p_fix, u_no_fix, PDAPI_NMEA_GNGGA);
    }

/*============================================================
FUNCTION tm_nmea_dtm_put

DESCRIPTION
tm_nmea_dtm_put() is used to build and transmit the NMEA DTM sentence.

Offset for PZ90 coordinate system
 
Time, position and fix related data for a GPS receiver.
 
 1 2    3  4  5   6   7   8      9
 | |    |  |  |   |   |   |      |
$aaccc,ccc,a,x.x,y.y,z.z,ccc,*hh<CR><LF>
 
 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
3) Local datum Code WGS84 = W84. PZ90 = P90.
4) Local Datum Subdivision Code.
5) Latitude Offset.
6) Longitude Offset.
7) Altitude Offset.
8) Reference Datum Code.
9) Checksum.

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
================================================================*/
static void tm_nmea_dtm_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix, pdsm_pa_nmea_type q_nmeaType)
{
  char b_sentence[200], *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0;
  double d_PosEcefPZ90[3] = { 0 };
  double d_PosLlaPZ90[3] = { 0 };
  double d_Lat, d_Long, d_Alt, d_LatMins, d_LongMins = 0.0;
  char c_LatHem, c_LongHem;
  const double *d_PosLlaRef;
  const double *d_PosLlaLocal;
  char* cz_ref = "W84";
  char* cz_local = "P90";

  if ((p_fix == NULL) || (u_no_fix == TRUE))
  {
    tm_nmea_empty_put(q_nmeaType);
    return;
  }

  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, q_nmeaType);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  tm_util_convert_Wgs84ToPZ90_datum(p_fix, d_PosEcefPZ90, d_PosLlaPZ90);

  if (TM_CORE_CONFIG_DD(u_control_nmea_sentence) & PDSM_PA_PZ90_DATUM_NMEA_SENTENCE)
  {
    cz_ref = "P90";
    cz_local = "W84";
    d_PosLlaRef = d_PosLlaPZ90;
    d_PosLlaLocal = p_fix->z_NavPos.d_PosLla;
    TM_ERROR("PZ90 Values    Lat : %d Long : %d Alt : %d", d_PosLlaRef[0], d_PosLlaRef[1], d_PosLlaRef[2]);
    TM_ERROR("WGS84 Values   Lat : %d Long : %d Alt : %d", d_PosLlaLocal[0], d_PosLlaLocal[1], d_PosLlaLocal[2]);

  }
  else
    {
    cz_ref = "W84";
    cz_local = "P90";
    d_PosLlaRef = p_fix->z_NavPos.d_PosLla;
    d_PosLlaLocal = d_PosLlaPZ90;
    TM_ERROR("WGS84 Values    Lat : %d Long : %d Alt : %d", d_PosLlaRef[0], d_PosLlaRef[1], d_PosLlaRef[2]);
    TM_ERROR("PZ90 Values     Lat : %d Long : %d Alt : %d", d_PosLlaLocal[0], d_PosLlaLocal[1], d_PosLlaLocal[2]);
    }

  /* Latitude offset */
  d_Lat = (d_PosLlaLocal[0] - d_PosLlaRef[0]) * (180.0 / C_PI);
  
  /* Longitude offset */
  d_Long = fmod(((d_PosLlaLocal[1] - d_PosLlaRef[1]) * (180.0 / C_PI)), 360.0);
  if (d_Long < -180.0)
    {
    d_Long += 360.0;
    }
  else if (d_Long > 180.0)
    {
    d_Long -= 360.0;
    }

  /* Altitude offset */
  d_Alt = (d_PosLlaLocal[2] - d_PosLlaRef[2]);

  if (d_Lat > 0.0)
      {
    c_LatHem = 'N';
        }
        else
        {
    c_LatHem = 'S';
    d_Lat *= -1.0;
          }

  d_LatMins = fmod(d_Lat * 60.0, 60.0);

  if (d_Long < 0.0)
          {
    c_LongHem = 'W';
    d_Long *= -1.0;
        }
      else
      {
    c_LongHem = 'E';
      }

  d_LongMins = fmod(d_Long * 60.0, 60.0);

  q_len = snprintf(p_b, q_buf_len, "%s,,%02d%09.6lf,%c,%03d%09.6lf,%c,%.3lf,%s",
                   cz_local,
                   (uint8)floor(d_Lat), d_LatMins, c_LatHem,
                   (uint8)floor(d_Long), d_LongMins, c_LongHem,
                   d_Alt, cz_ref);

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  tm_nmea_terminate_and_put(b_sentence, q_nmeaType);
    }

/******************************************************************************
@brief      Utility function to generate DTM sentence for GPS
******************************************************************************/
void tm_nmea_gpdtm_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
    {
  tm_nmea_dtm_put(p_fix, u_no_fix, PDAPI_NMEA_GPDTM);
	}

/******************************************************************************
@brief      Utility function to generate DTM sentence for 
            multiple constellations (Talker ID $GN)
******************************************************************************/
void tm_nmea_gndtm_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
    {
  tm_nmea_dtm_put(p_fix, u_no_fix, PDAPI_NMEA_GNDTM);
}

/*===========================================================================
FUNCTION tm_nmea_gngns_put

DESCRIPTION
tm_nmea_gngns_put() is used to build and transmit the NMEA GNS sentence.

 GNSS Fix data
 
  Time, position and fix related data for a GNSS receiver.
 
  1 2   3         4       5 6        7 8    9  10  11  12  13  14  15  
  | |   |         |       | |        | |    |  |   |   |   |   |   |   
 $aaccc,hhmmss.ss,llll.ll,a,yyyyy.yy,a,c--c,xx,x.x,x.x,x.x,x.x,x.x*hh<CR><LF>
 
 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
  3) Universal Time Coordinated (UTC) of position.
  4) Latitude  (DDmm.mm)    DD = two digits of degrees, mm.mm = minutes.
  5) N or S (North or South).
  6) Longitude (DDDmm.mm).
  7) E or W (East or West).
  8) Mode Indicator,
     N - fix not available or invalid,
     A - Autonomous. Satellite system used in non-differential mode in position fix
     D - Differential GPS, SPS Mode, fix valid
     P - GPS PPS Mode, fix valid
     R - Real Time Kinematic:  Satellite system used in RTK mode with fixed
         integers,
     F - Float RTK:  Satellite system used in RTK mode, floating integers
     E - Estimated (dead reckoning) Mode,
     M - Manual Input Mode,
     S - Simulator Mode.
  9) Number of satellites in use, (gps+glo+gal).
 10) Horizontal Dilution of Precision (HDOP).
 11) Antenna Altitude above/below mean-sea-level (geoid), meters.
 12) Geoidal separation, the difference between the WGS-84 earth ellipsoid and
     the mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid.
 13) Age of differential GPS data, time in seconds since last RTCM-SC104 
     Type 1 or Type 9 update (null field when DGPS is not used).
 14) Differential reference station ID.
 15) Navigational Status
 16) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
void tm_nmea_gngns_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
{
  char b_sentence[200];
  char *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0;
  boolean b_utc_valid = FALSE;
  tm_CalendarTimeT z_utc_time;
  float f_geoidal_sep = 0.0;
  double d_altitude = 0.0;
  uint32 q_TotalDays = 0;
  uint16 u_numSv = 0;
  char u_qualGps = 'N';
  char u_qualGlo = 'N';
  char u_qualGal = 'N';
  char u_qualBds = 'N', u_qualQzss = 'N', u_qualNavic = 'N';

  if (p_fix == NULL)
  {
    tm_nmea_empty_put(PDAPI_NMEA_GNGNS);
    return;
  }

  u_numSv = (tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo) +
             tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo) +
             tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo) +
             tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo) +
             tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo) +
             tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo));

  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, PDAPI_NMEA_GNGNS);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  /*  Universal Time Coordinated (UTC) of position: */
  b_utc_valid = tm_util_get_fix_time(p_fix, &z_utc_time, &q_TotalDays);
  if (b_utc_valid == TRUE)
  {
    q_len = snprintf(p_b, q_buf_len,
                     "%02" PRIu32 // Hour
                     "%02" PRIu32        // Minute
                     "%02" PRIu32        // Second
                     ".%02" PRIu32 ",", // Millisecond/10
                     z_utc_time.q_Hour, z_utc_time.q_Minute,
                     z_utc_time.q_Second, z_utc_time.q_Millisecond / 10);
  }
  else
  {
    q_len = snprintf(p_b, q_buf_len, ",");
  }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  /* END - UTC of position */

  if (u_no_fix)
  {
    /* Construct the 'know nothing position sentence */
    q_len = snprintf(p_b, q_buf_len, ",,,,NNN,,,,,,,V");

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  }
  else
  {
    /* Latitude and longitude: */
	
    q_len = tm_util_calculate_position_print(p_fix, p_b, q_buf_len);
    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /* END - Latitude and Longitude */

    /* Mode Indicator: */
      if (p_fix->z_NavPos.z_PosFlags.b_IsValid)
      {
      if (TRUE == tm_nmea_is_differential_fix(p_fix))
        {
        /* DGPS fix */
        u_qualGps = (tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        u_qualGlo = (tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        u_qualGal = (tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        u_qualBds = (tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        u_qualQzss = (tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        u_qualNavic = (tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo) ? 'D' : 'N');
        }
      else if (TRUE == tm_nmea_is_propagated_fix(p_fix))
        {
        /* Estimated (dead reckoning) fix */
        u_qualGps = 'E';
        u_qualGlo = 'E';
        u_qualGal = 'E';
        u_qualBds = 'E';
        u_qualQzss = 'E';
        u_qualNavic = 'E';
      }
      else if (0 == u_numSv)
          {
        /* Autonomous fix */
        u_qualGps = 'A';
        u_qualGlo = 'A';
        u_qualGal = 'A';
        u_qualBds = 'A';
        u_qualQzss = 'A';
        u_qualNavic = 'A';
          }
          else
          {
        /* Non DGPS fix, Autonomous fix */
        u_qualGps = (tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        u_qualGlo = (tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        u_qualGal = (tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        u_qualBds = (tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        u_qualQzss = (tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        u_qualNavic = (tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo) ? 'A' : 'N');
        }
      }

    q_len = snprintf(p_b, q_buf_len, "%c%c%c%c%c%c,%02d,%.1f,",
                     u_qualGps, u_qualGlo, u_qualGal,
                     u_qualBds, u_qualQzss, u_qualNavic,
                     u_numSv, p_fix->z_NavPos.f_HDOP);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    /* Altitude, etc.: */

    /*Find the Geoidal seperation before signs are manipulated*/
    f_geoidal_sep = tm_nmea_get_altitude_and_geoidal_separation(p_fix, &d_altitude);
    /* "Age of Differential Data" and "Differential reference station ID" both NULL*/
	  /* 'V'as Nav status for equipment not providing navigational status */
      q_len = snprintf( p_b, q_buf_len,"%.1lf,%.1f,,,V" , 
                     d_altitude, f_geoidal_sep);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /* END - Altitude */

  } /*else for if(u_no_fix)*/

  tm_nmea_terminate_and_put(b_sentence, PDAPI_NMEA_GNGNS);
}

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*===========================================================================
FUNCTION tm_nmea_pqxfi_put

DESCRIPTION
tm_nmea_pqxfi_put() is used to build and transmit the NMEA 
Proprietary Qualcomm eXtended Fix Information message. 

Global Positioning System Extended Fix Data.

Extended fix related data for a GPS receiver.

 1     2         3       4 5        6        7 8 9  10 11  12 13 14 15  16   17
 |     |         |       | |        |        | | |  |   |   | |   | |   |    |
$PQXFI,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>

Field Number: 
1) Proprietary Qualcommm eXtended Fix Information.
2) Universal Time Coordinated (UTC) of position.
3) Latitude  (DDmm.mm)    DD = two digits of degrees, mm.mm = minutes.
4) N or S (North or South).
5) Longitude (DDDmm.mm).
6) E or W (East or West).
7) Antenna Altitude above/below mean-sea-level (geoid), meters.
8) HEPE (m, ~63% confidence, 2D horizontal error estimate = root-sum-square of 68% confidence east error estimate and north error estimate)
9) Vertical Uncertainty (m, 68% confidence, 1-sided, 1-dimensional)
10) Velocity Uncertainty (m/s, 20% confidence, 3D = root-sum-square of 68% confidence error estimates on east, north & up velocities) 
11) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
void tm_nmea_pqxfi_put(const sm_FixInfoStructType *p_fix, boolean u_no_fix)
{
  char b_sentence[200];
  char *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0;
  boolean b_utc_valid = FALSE;
  utc_time z_utc_time;
  double d_Hepe = 0.0;
  double d_altitude = 0.0;

  if (p_fix == NULL)
  {
    tm_nmea_empty_put(PDAPI_NMEA_PQXFI);
    return;
  }

  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, PDAPI_NMEA_PQXFI);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  /*  Universal Time Coordinated (UTC) of position: */
  b_utc_valid = tm_util_calculate_utc(p_fix, NULL, &z_utc_time);
  if (b_utc_valid == TRUE)
  {
    q_len = snprintf(p_b, q_buf_len, "%02ld%02ld%02ld.%01ld,",
                       z_utc_time.q_hours, z_utc_time.q_mins,
                       z_utc_time.q_secs, z_utc_time.q_tenths);
    }
  else
  {
    q_len = snprintf(p_b, q_buf_len, ",");
  }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  /* END - UTC of position */

  if (u_no_fix)
  {
    /* Construct the 'know nothing position sentence */
    q_len = snprintf(p_b, q_buf_len, ",,,,,,,");

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    }
  else /* if(u_no_fix) */
  {
    /* Lat, Lon, Alt, and Position Errors */
    /* Latitude and longitude*/
    q_len = tm_util_calculate_position_print(p_fix, p_b, q_buf_len);
    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /*End - Lat Long */

    /* Altitude, etc.: */
    /*Find the Geoidal seperation before signs are manipulated*/
    (void)tm_nmea_get_altitude_and_geoidal_separation(p_fix, &d_altitude);

    q_len = snprintf(p_b, q_buf_len, "%.1lf,", d_altitude);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
    /* END - Altitude */

    /* compute RSS of the 2d uncertainty to get HEPE */
        d_Hepe =  sqrt((p_fix->z_NavPos.f_ErrorEllipse[1] * 
                        p_fix->z_NavPos.f_ErrorEllipse[1]) + 
                       (p_fix->z_NavPos.f_ErrorEllipse[2] * 
                        p_fix->z_NavPos.f_ErrorEllipse[2]));

    /* error information */
    q_len = snprintf(p_b, q_buf_len, "%.2lf,%.2f,%.2f",
                     d_Hepe,
                     p_fix->z_NavPos.f_PuncVertMeters,
                     p_fix->z_NavPos.f_Vunc3dMps);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  } /*else for if(u_no_fix)*/

  tm_nmea_terminate_and_put(b_sentence, PDAPI_NMEA_PQXFI);
}

#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*===========================================================================
FUNCTION tm_nmea_gsv_put

DESCRIPTION
tm_nmea_gsv_put() is used to transmit the GSV - Satellites in View.

     Number of satellites (SVs) in view, satellite ID numbers, elevation, 
 azimuth, and SNR value. Four satellites maximum per transmission, additional
 satellite data sent in the second or third message.

 1 2   3 4 5  6  7  8   9      10      11 12 13  14 15  16
 | |   | | |  |  |  |   |       |      |  |  |   |  |  |
$aaccc,x,x,xx,xx,xx,xxx,xx,.. .. .. ..,xx,xx,xxx,xx,xx*hh<CR><LF>

 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
3) Total number of messages, 1 to 9.
4) Message number, 1 to 9.
  5) Total number of satellites in view.
  6) SV1 - Satellite ID number.
  7) SV1 - Elevation, degrees, 90 degrees maximum.
  8) SV1 - Azimuth, degrees True, 000 to 359.
  9) SV1 - SNR (C/No) 00-99 dB, NULL when not tracking.
 10) SV2 to SV3.
 11) SV4 - Satellite ID number.
 12) SV4 - Elevation, degrees, 90 degrees maximum.
 13) SV4 - Azimuth, degrees True, 000 to 359.
 14) SV4 - SNR (C/No) 00-99 dB, NULL when not tracking.
15) Signal ID
 16) Checksum

     Null fields are not required for unused sets when less then four sets
 are transmitted. 

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
static void tm_nmea_gsv_put(const sm_GpsMeasRptStructType *p_meas, pdsm_pa_nmea_type q_nmeaType)
{
  char b_sentence[200], *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence), q_len = 0;
  uint8 u_sv_i, u_msg, u_total_messages, u_i;
  uint8 u_total_sv = 0;
  uint16 w_rfLoss = 0;
  const gnss_MeasStructType *p_meas_report = NULL;
  boolean v_svId_valid = FALSE;

  if (NULL == p_meas) return;

  p_meas_report = p_meas->z_MeasBlk.z_Gnss;
  u_total_sv = p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs;
  /* Restrict the number of SVs per message to 4 (length limitation) */
  /* We can send multiple messages */
  u_total_messages = u_total_sv >> 2;
  if (u_total_sv != (u_total_messages << 2))
  {
    u_total_messages++;
  }

  for (u_sv_i = 0, u_msg = 1; u_msg <= u_total_messages; u_msg++)
  {
    /* Total number of messages, message number, total number of 
     * satellites in view. 
    */
    p_b = b_sentence;
    q_buf_len = sizeof(b_sentence);
    q_len = 0;

    //Init NMEA Sentence
    q_len = tm_nmea_init_sentence(p_b, q_buf_len, q_nmeaType);
    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    q_len = snprintf(p_b, q_buf_len, "%d,%d,%02d",
                     u_total_messages, u_msg, u_total_sv);

    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    for (u_i = 0; (u_sv_i < u_total_sv) && (u_i < 4); u_i++, u_sv_i++, p_meas_report++)
    {
      v_svId_valid = (GNSS_ID_OK(p_meas_report->w_Sv) || SBAS_ID_OK(p_meas_report->w_Sv));

      if ((TRUE == v_svId_valid) ||
           (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_GSV_EXTENDED))
      {
        q_len = snprintf(p_b, q_buf_len, ",%02d,", tm_nmea_get_svid(p_meas_report->w_Sv));
      }
      else
      {
        q_len = snprintf(p_b, q_buf_len, ",,");
      }

      TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

      if (p_meas_report->u_SvDirValid && (p_meas_report->f_Elevation > 0))
      {
        if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_GSV_EXTENDED)
        {
          float x_elev = (float)(p_meas_report->f_Elevation * C_RAD_TO_DEG);
          float l_azim = (float)(p_meas_report->f_Azimuth   * C_RAD_TO_DEG);

          /*elevation, azimuth  */
          q_len = snprintf(p_b, q_buf_len, "%.1f,%.1f,",
                           x_elev,
                           l_azim);
        }
        else
        {
          int16 x_elev = (int16)(p_meas_report->f_Elevation * C_RAD_TO_DEG);
          uint32 l_azim = (uint32)(p_meas_report->f_Azimuth * C_RAD_TO_DEG);
          /*elevation, azimuth  */
          q_len = snprintf(p_b, q_buf_len, "%02d,%03ld,",
                           x_elev,
                           l_azim);
        }
      }
      else
      {
        /* NULL elevation, NULL azimuth*/
        q_len = snprintf(p_b, q_buf_len, ",,");
      }

      TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

      if (p_meas_report->w_Cno > 0)     /* Tracked. */
      {
        if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_GSV_EXTENDED)
        {
          float cno_report = (float)(p_meas_report->w_Cno) / 10.0f;

          q_len = snprintf(p_b, q_buf_len, "%.1f", cno_report);
        }
        else
        {
          /* Add 5 to round up */
          uint16 cno_report = (p_meas_report->w_Cno + p_meas_report->w_RfLoss + 5) / 10;

          q_len = snprintf(p_b, q_buf_len, "%d", cno_report);
        }

        TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
        }
      }

    /* Signal ID */
    q_len = snprintf(p_b, q_buf_len, ",%d", tm_nmea_get_signal_id(p_meas));
    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
      
    tm_nmea_terminate_and_put(b_sentence, q_nmeaType);
    }
  }

/******************************************************************************
@brief      Utility function to generate GSV sentence for GAL
******************************************************************************/
void tm_nmea_gagsv_put(const sm_GpsMeasRptStructType *p_meas)
{
  if ((NULL == p_meas) ||
      (GNSS_MEAS_BLK_SRC_GAL != sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
  {
    return;
  }
  tm_nmea_gsv_put(p_meas, PDAPI_NMEA_GAGSV);
  }

/******************************************************************************
@brief      Utility function to generate GSV sentence for GPS
******************************************************************************/
void tm_nmea_gpgsv_put(const sm_GpsMeasRptStructType *p_meas)
  {
  gnss_MeasBlkSrcEnumType e_system;
  sm_GpsMeasRptStructType meas_copy = { 0 };

  if ((NULL == p_meas) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0)) return;

  e_system = sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc);

  if ((GNSS_MEAS_BLK_SRC_QZSS_SBAS != e_system) &&
      (GNSS_MEAS_BLK_SRC_GPS != e_system))
  {
    return;
  }

  //Copy the measurements since we may need to alter it
  sm_GnssMeas_memscpy((void *)&meas_copy, sizeof(sm_GpsMeasRptStructType), p_meas);

  if (GNSS_MEAS_BLK_SRC_QZSS_SBAS == e_system)
  {
    tm_util_clean_qzss_sbas_meas_rpt(&meas_copy, TRUE);
  }

  if (meas_copy.z_MeasBlk.z_GnssHeader.u_NumSvs > 0)
  {
    tm_nmea_gsv_put(&meas_copy, PDAPI_NMEA_GPGSV);
      }
      }

/******************************************************************************
@brief      Utility function to generate GSV sentence for BDS and QZSS.
            This is a Qualcomm Proprietary sentence with talker id $PQ
            (PQGSV will be deprecated in LocTech 20)
******************************************************************************/
void tm_nmea_pqgsv_put(const sm_GpsMeasRptStructType *p_meas)
      {
  gnss_MeasBlkSrcEnumType e_system;
  sm_GpsMeasRptStructType meas_copy = {0};

  if ((NULL == p_meas) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0)) return;

  e_system = sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc);

  if ((GNSS_MEAS_BLK_SRC_BDS != e_system) &&
      (GNSS_MEAS_BLK_SRC_QZSS_SBAS != e_system))
      {
        return;
      }

  //Copy the measurements since we may need to alter it
  sm_GnssMeas_memscpy((void *)&meas_copy, sizeof(sm_GpsMeasRptStructType), p_meas);

  if (GNSS_MEAS_BLK_SRC_QZSS_SBAS == e_system)
      {
    tm_util_clean_qzss_sbas_meas_rpt(&meas_copy, FALSE);
        }
  if (meas_copy.z_MeasBlk.z_GnssHeader.u_NumSvs > 0)
        {
    tm_nmea_gsv_put(&meas_copy, PDAPI_NMEA_PQGSV);
        }
        }

/******************************************************************************
@brief      Utility function to generate GSV sentence for GLO
******************************************************************************/
void tm_nmea_glgsv_put(const sm_GpsMeasRptStructType *p_meas)
      {
  if ((NULL == p_meas) ||
      (GNSS_MEAS_BLK_SRC_GLO != sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
    {
        return;
      }
  tm_nmea_gsv_put(p_meas, PDAPI_NMEA_GLGSV);
}

/******************************************************************************
@brief      Utility function to generate GSV sentence for BDS
******************************************************************************/
void tm_nmea_gbgsv_put(const sm_GpsMeasRptStructType *p_meas)
{
  if ((NULL == p_meas) ||
      (GNSS_MEAS_BLK_SRC_BDS != sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
  {
    return;
  }
  tm_nmea_gsv_put(p_meas, PDAPI_NMEA_GBGSV);
  }

/******************************************************************************
@brief      Utility function to generate GSV sentence for QZSS
******************************************************************************/
void tm_nmea_gqgsv_put(const sm_GpsMeasRptStructType *p_meas)
  {
  sm_GpsMeasRptStructType meas_copy = { 0 };

  if ((NULL == p_meas) ||
      (GNSS_MEAS_BLK_SRC_QZSS_SBAS != sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
  {
      return;
    }

  //Copy the measurements since we may need to alter it
  sm_GnssMeas_memscpy((void *)&meas_copy, sizeof(sm_GpsMeasRptStructType), p_meas);
      tm_util_clean_qzss_sbas_meas_rpt(&meas_copy, FALSE);

  if (meas_copy.z_MeasBlk.z_GnssHeader.u_NumSvs > 0)
{
    tm_nmea_gsv_put(&meas_copy, PDAPI_NMEA_GQGSV);
  }
  }

/******************************************************************************
@brief      Utility function to generate GSV sentence for NAVIC
******************************************************************************/
void tm_nmea_gigsv_put(const sm_GpsMeasRptStructType *p_meas)
  {
  if ((NULL == p_meas) ||
      (GNSS_MEAS_BLK_SRC_NAVIC != sm_util_get_master_constellation(p_meas->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc)) ||
      (p_meas->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
    {
      return;
    }
  tm_nmea_gsv_put(p_meas, PDAPI_NMEA_GIGSV);
}

/*===========================================================================
FUNCTION tm_nmea_vtg_put

DESCRIPTION
tm_nmea_vtg_put() is used to transmit VTG - Course Over Ground and Ground Speed.
    The actual course and speed relative to the ground.

  1 2   3   4 5   6 7   8 9  10 11
  | |   |   | |   | |   | |   | |
 $aaccc,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>

 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
  3) Course, degrees.
  4) T = True.
  5) Course, degrees.
  6) M = Magnetic.
  7) Speed, knots.
  8) N = Knots.
  9) Speed Kilometers Per Hour.
 10) K = Kilometers Per Hour.
 11) Checksum.

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
static void tm_nmea_vtg_put(const sm_FixInfoStructType *p_fix, pdsm_pa_nmea_type q_nmeaType)
{
  char b_sentence[200], *p_b = b_sentence;
  double  d_true_course, d_azimuth, d_speed, d_mag_course;
  double  d_north_vel, d_east_vel;
  int q_buf_len = sizeof(b_sentence), q_len = 0;
    char u_ModeIndicator = 'N';

  if (NULL == p_fix)
    {
      return;
    }

  // Fields 1 and 2: Talker ID + Sentence Type.
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, q_nmeaType);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    d_east_vel  = p_fix->z_NavPos.f_VelEnu[0];
    d_north_vel = p_fix->z_NavPos.f_VelEnu[1];

    d_speed = sqrt( d_north_vel * d_north_vel + d_east_vel * d_east_vel );

    /* Azimuth moves from north (0) to east (90), wrapping around.
     * Note that we are ignoring the vertical velocity.
     * Also, default azimuth to 0 if there is no North/East velocity
     */
    if(p_fix->z_NavPos.z_PosFlags.b_IsHeadingValid && d_speed >= TM_CORE_MIN_VELOCTY) 
    {
      d_azimuth = p_fix->z_NavPos.f_HeadingRad * (180.0 / C_PI);  
      if (d_azimuth + (double) 0.05 >= (double) 360.0) 
      { 
        d_azimuth = 0.0; 
      } 
      d_true_course = d_azimuth;
    q_len = snprintf(p_b, q_buf_len, "%.1lf,T,", d_true_course);
    }
    else
    {
      d_azimuth = 0.0;
    q_len = snprintf(p_b, q_buf_len, ",T,");
    }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

    /* magnetic deviation. */
	if (p_fix->z_NavPos.u_MagDevGood == TRUE)
  {
		d_mag_course = d_azimuth - p_fix->z_NavPos.f_MagDeviation;
        if (d_mag_course < 0.0) 
        {
          d_mag_course += 360.0;
        }
        /* COG shouldnt be less than 360 degrees account for rounding off errors due to sprintf */
        else if (d_mag_course + (double) 0.05 >= (double) 360.0) 
        { 
          d_mag_course -= 360.0; 
        }
  }
  else
  {
    d_mag_course = d_azimuth;
  }    

      if (tm_nmea_is_differential_fix(p_fix))
      {
        u_ModeIndicator = 'D'; /* Differential (DGPS) */
      }
      else
      {
    if (TRUE == tm_nmea_is_propagated_fix(p_fix))
      {
          u_ModeIndicator = 'E'; /* Estimated (dead reckoning) */
      }
      else
      {
          u_ModeIndicator = 'A'; /* Autonomous (Non DGPS) */
        }
      }

      if (d_speed < TM_CORE_MIN_VELOCTY)
      {
        /* If Speed < 0.15 m/sec e.g NF Heading Filter value of 0.1m/s, zero the NMEA speed report */
        d_speed = 0;
      }
    
      q_len = snprintf( p_b, q_buf_len,"%.1lf,M,%.1lf,N,%.1lf,K,%c",
                              d_mag_course,                /* Degrees. */
                              d_speed * (3600.0/1852.0),   /* Knots */
                              d_speed * 3.6,               /* Km / hour */
                              u_ModeIndicator);

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  tm_nmea_terminate_and_put(b_sentence, q_nmeaType);
    }

/******************************************************************************
@brief      Utility function to generate VTG sentence for GPS
******************************************************************************/
void tm_nmea_gpvtg_put(const sm_FixInfoStructType *p_fix)
{
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GPVTG);
    return;
  }

  tm_nmea_vtg_put(p_fix, PDAPI_NMEA_GPVTG);
}

/******************************************************************************
@brief      Utility function to generate VTG sentence for GAL
******************************************************************************/
void tm_nmea_gavtg_put(const sm_FixInfoStructType *p_fix)
  {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)))
    {
    tm_nmea_empty_put(PDAPI_NMEA_GAVTG);
    return;
  }

  tm_nmea_vtg_put(p_fix, PDAPI_NMEA_GAVTG);
  }

/******************************************************************************
@brief      Utility function to generate VTG sentence for BDS
******************************************************************************/
void tm_nmea_gbvtg_put(const sm_FixInfoStructType *p_fix)
  {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)))
    {
    tm_nmea_empty_put(PDAPI_NMEA_GBVTG);
    return;
  }

  tm_nmea_vtg_put(p_fix, PDAPI_NMEA_GBVTG);
      }

/******************************************************************************
@brief      Utility function to generate VTG sentence for 
            multiple constellations (Talker ID $GN)
******************************************************************************/
void tm_nmea_gnvtg_put(const sm_FixInfoStructType *p_fix)
    {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) || /* Position invalid */
      ((0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))))
    {
    tm_nmea_empty_put(PDAPI_NMEA_GNVTG);
      return;
    }

  tm_nmea_vtg_put(p_fix, PDAPI_NMEA_GNVTG);
  }

/*===========================================================================
FUNCTION tm_nmea_gsa_put

DESCRIPTION
tm_nmea_gsa_put() is used to transmit GSA Sentences
     

  1 2   3 4 5                                   6   7   8    9  10
  | |   | | |                                   |   |   |    |  |
$aaccc,d,e,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,xx,x.x,x.x,x.x,h*hh<CR><LF>

 Field Number: 
  1) Talker ID.
  2) Sentence format/type.
  3) Mode, M = Manual, A = Automatic  2D/3D.
  4) Mode, 1 = No fix, 2 = 2D, 3 = 3D.
  5) PRN used in the position solution.  Using "null" (",") for no SV for
     a total of 12 tokens.
  6) PDOP - Position Dilution Of Precision.
  7) HDOP - Horizontal Dilution Of Precision.
  8) VDOP - Vertical Dilution Of Precision.
9) GNSS System ID
  10) Checksum.

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS 
===========================================================================*/
static void tm_nmea_gsa_put(const sm_FixInfoStructType *p_fix, const uint8 num_svs,
                            const uint16 svs[], pdsm_pa_nmea_type e_nmea_type, tm_nmea_system_id_e_type e_system)
{
  char b_sentence[200], *p_b = b_sentence;
  uint8  u_i;
  uint8 u_mode = 2, u_sv_offset = 0;
  int q_buf_len = sizeof(b_sentence), q_len = 0;

  if ((NULL == p_fix) || (0 == num_svs) || (NULL == svs))
  {
    tm_nmea_empty_put(e_nmea_type);
    return;
  }

  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, e_nmea_type);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  //Update mode if this is a 3D Position
  if (p_fix->z_NavPos.z_PosFlags.b_Is3D)
  {
    u_mode = 3;
  }

  q_len = snprintf(p_b, q_buf_len, "A,%d,", u_mode);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  for (u_i = 0; u_i < 12; u_i++)
  {
    if (u_i < num_svs)
    {
      q_len = snprintf(p_b, q_buf_len, "%02d,", tm_nmea_get_svid(svs[u_i]));
    }
    else    /* Filler for this record. */
    {
      q_len = snprintf(p_b, q_buf_len, ",");
    }
    TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
      }

  /* NMEA v4.10 GNSS System ID field */
    q_len = snprintf(p_b, q_buf_len, "%.1f,%.1f,%.1f,%d",
                     p_fix->z_NavPos.f_PDOP,
                     p_fix->z_NavPos.f_HDOP,
                     p_fix->z_NavPos.f_VDOP, e_system);

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  tm_nmea_terminate_and_put(b_sentence, e_nmea_type);
}

/******************************************************************************
@brief      Utility function to generate GSA sentence for GPS
******************************************************************************/
void tm_nmea_gpgsa_put(const sm_FixInfoStructType *p_fix)
{
  uint8 u_numsv = 0;
  uint16 z_SvIDs[N_ACTIVE_GPS_CHAN];

  /*According to spec, if only GPS is used for fix, GPGSA should be sent.
    So, GPGSA should be generated when: GPS is used, No other system is used.
    Legacy behavior, so keep as it is even though it is not aligned to spec*/
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GPGSA);
    return;
  }

  u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                    GNSS_ARRAY_SIZEOF(z_SvIDs),
                                            p_fix->z_NavSvInfo.z_GpsSvInfo,
                                                    GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_GpsSvInfo));

    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GPGSA, TM_NMEA_SYSTEM_ID_GP);
  }

/******************************************************************************
@brief      Utility function to generate GSA sentence for GAL
******************************************************************************/
void tm_nmea_gagsa_put(const sm_FixInfoStructType *p_fix)
{
  uint8 u_numsv = 0;
  uint16 z_SvIDs[N_ACTIVE_GAL_CHAN];
  
  /*According to spec, if only GAL is used for fix, GAGSA should be sent.
    So, GAGSA should be generated when: GAL is used, No other system is used.
    Legacy behavior, so keep as it is even though it is not aligned to spec*/
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||
      (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GAGSA);
    return;
  }

  u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                    GNSS_ARRAY_SIZEOF(z_SvIDs),
                                            p_fix->z_NavSvInfo.z_GalSvInfo,
                                                    GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_GalSvInfo));

    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GAGSA, TM_NMEA_SYSTEM_ID_GA);
  }

/******************************************************************************
@brief      Utility function to generate GSA sentence for BDS
******************************************************************************/
void tm_nmea_gbgsa_put(const sm_FixInfoStructType *p_fix)
{
  uint8 u_numsv = 0;
  uint16 z_SvIDs[N_ACTIVE_BDS_CHAN];

  /*According to spec, if only BDS is used for fix, GBGSA should be sent.
    So, GBGSA should be generated when: BDS is used, No other system is used.
    Legacy behavior, so keep as it is even though it is not aligned to spec*/
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||
      (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GBGSA);
    return;
  }

  u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                    GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                    p_fix->z_NavSvInfo.z_BdsSvInfo,
                                                    GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_BdsSvInfo));

  tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GBGSA, TM_NMEA_SYSTEM_ID_BDS);
    }

/******************************************************************************
@brief      Utility function to generate GSA sentence for BDS and QZSS.
            This is a Qualcomm Proprietary sentence with talker id $PQ
            (PQGSA will be deprecated in LocTech 20)
******************************************************************************/
void tm_nmea_pqgsa_put(const sm_FixInfoStructType *p_fix)
{
  uint8 u_numsv = 0;
  uint16 z_SvIDs[MAX_ACTIVE_GNSS_CHAN];  // max 16 active

  if ((NULL == p_fix) ||
    (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) || /* Position invalid */
    ((0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)) &&
    (0 == tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo))))
  {
    tm_nmea_empty_put(PDAPI_NMEA_PQGSA);
    return;
  }

  if (tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo))
      {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
      GNSS_ARRAY_SIZEOF(z_SvIDs),
      p_fix->z_NavSvInfo.z_BdsSvInfo,
      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_BdsSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_PQGSA, TM_NMEA_SYSTEM_ID_BDS);
      }

  if (tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo))
      {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                              p_fix->z_NavSvInfo.z_QzssSvInfo,
      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_QzssSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_PQGSA, TM_NMEA_SYSTEM_ID_QZSS);
    }
}

/******************************************************************************
@brief      Utility function to generate GSA sentence for 
            multiple constellations (Talker ID $GN)
******************************************************************************/
void tm_nmea_gngsa_put(const sm_FixInfoStructType *p_fix)
{
  uint16 z_SvIDs[MAX_ACTIVE_GNSS_CHAN];
  uint8 u_numsv = 0;
  /*According to spec, if multiple systems are used for fix, GNGSA should be sent.
    Our SW supports multiple systems, so ideally only GNGSA should be sent*/
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) || /* Position invalid */
      ((0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GNGSA);
    return;
  }

  if (tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo))
  {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_GpsSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_GpsSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_GP);
      }

  if (tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo))
    {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_GloSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_GloSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_GL);
    }

  if (tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo))
  {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_GalSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_GalSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_GA);
  }

  if (tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo))
  {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_BdsSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_BdsSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_BDS);
      }

  if (tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo))
      {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_QzssSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_QzssSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_QZSS);
        }

  if (tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))
      {
    u_numsv = sm_generate_sv_array_from_svinfo_struct(z_SvIDs,
                                                      GNSS_ARRAY_SIZEOF(z_SvIDs),
                                                      p_fix->z_NavSvInfo.z_NavicSvInfo,
                                                      GNSS_ARRAY_SIZEOF(p_fix->z_NavSvInfo.z_NavicSvInfo));
    tm_nmea_gsa_put(p_fix, u_numsv, z_SvIDs, PDAPI_NMEA_GNGSA, TM_NMEA_SYSTEM_ID_NAVIC);
    }
  }

/*===========================================================================
FUNCTION tm_nmea_rmc_put
 
DESCRIPTION
tm_nmea_rmc_put() is used to build and transmit the NMEA RMC sentence.

 Global Positioning System Fix Data.

 Time, position and fix related data for a GPS receiver.

  1 2   3         4 5       6 7        8 9  10  11     12  13 14 15
  | |   |         | |       | |        | |   |   |      |   | |  | 
 $aaccc,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxxxx,x.x,a,a*hh<CR><LF>

 Field Number: 
  1) Talker ID.
  2) Sentence format/type. (RMC)
  3) Universal Time Coordinated (UTC) of position.
  4) Status (A = Valid, V = Invalid)
  5) Latitude  (DDmm.mm)    DD = two digits of degrees, mm.mm = minutes.
  6) N or S (North or South).
  7) Longitude (DDDmm.mm).
  8) E or W (East or West).
  9) Speed (Knots)
 10) Course. Degrees True.
 11) Date ddmmyy
 12) Magnetic variation 
 13) Mag variation direction. (E/W). E subtracts mag var from true, W adds mag var to true.
 14) Mode indicator. 
15) Navigational status
16) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
static void tm_nmea_rmc_put(const sm_FixInfoStructType *p_fix, pdsm_pa_nmea_type q_nmeaType)
{
  char b_sentence[ 200 ];
  char *p_B = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0 ;
  boolean b_utc_valid = FALSE;
  tm_CalendarTimeT z_utc_time = {0};
  double  d_north_vel = 0, d_east_vel = 0, d_knots = 0, d_speed = 0, d_azimuth = 0;
  boolean heading_valid = FALSE;
  double d_MagVar = 0;
  char u_VarDir = 'E';
  char u_mode_indicator = 'N'; /* Data Not Valid */
  uint32 q_TotalDays = 0;

  if (NULL == p_fix)
  {
    return;
  }

  // Fields 1 and 2: Talker ID + Sentence Type.
  q_len = tm_nmea_init_sentence(p_B, q_buf_len, q_nmeaType);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);

  /*  Universal Time Coordinated (UTC) of position: */
  b_utc_valid = tm_util_get_fix_time(p_fix, &z_utc_time, &q_TotalDays);
    if (b_utc_valid == TRUE)
    {
      q_len = snprintf( p_B, q_buf_len,
                     "%02" PRIu32 // Hour
                       "%02" PRIu32        // Minute
                       "%02" PRIu32        // Second
                       ".%02" PRIu32 ",A," , // Millisecond/10
                       z_utc_time.q_Hour, z_utc_time.q_Minute,
                       z_utc_time.q_Second, z_utc_time.q_Millisecond/10);
    }
    else
    {
    q_len = snprintf(p_B, q_buf_len, ",A,");
    }

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);

    /* Latitude and longitude: */
  q_len = tm_util_calculate_position_print(p_fix, p_B, q_buf_len);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);
  /* END - Latitude and Longitude */

    /* Velocity estimate */
      d_east_vel  = p_fix->z_NavPos.f_VelEnu[0];
      d_north_vel = p_fix->z_NavPos.f_VelEnu[1];

      d_speed = sqrt( d_north_vel * d_north_vel + d_east_vel * d_east_vel );

      if (p_fix->z_NavPos.z_PosFlags.b_IsVelValid) 
      {
        if (d_speed >= TM_CORE_MIN_VELOCTY)
        {
          d_knots = d_speed * (3600.0/1852.0);
        }
        else
        {
          d_knots = 0.0;
        }

    /* Azimuth moves from north (0) to east (90), wrapping around.
       Note that we are ignoring the vertical velocity. */
        if(p_fix->z_NavPos.z_PosFlags.b_IsHeadingValid && d_speed >= TM_CORE_MIN_VELOCTY) 
        {
          d_azimuth = p_fix->z_NavPos.f_HeadingRad  * (180.0 / C_PI);
          heading_valid = TRUE;
          /* COG shouldnt be less than 360 degrees account for rounding off errors due to sprintf */
          if (d_azimuth + (double) 0.05 >= (double) 360.0) 
          { 
            d_azimuth = 0.0; 
          }
        }
        else
        {
          d_azimuth = 0.0;
          heading_valid = FALSE;
        }

        /* Don't worry about magnetic deviation. We can come back 
           and add this later if a customer has a burning need. */
        if(heading_valid == TRUE)
        {
          q_len = snprintf( p_B, q_buf_len, "%.1lf,%.1lf,",
                          d_knots,      /* Knots */
                          d_azimuth );    /* Degrees. */
        }
        else
        {
          q_len = snprintf( p_B, q_buf_len, "%.1lf,,",
                          d_knots);      /* Knots */
        }
      }
      else /* Velocity not available */
      {
        q_len = snprintf( p_B, q_buf_len, ",," );
        }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);

    /* Days, Months, Years */
      if (b_utc_valid == TRUE)
      {
        q_len = snprintf( p_B, q_buf_len, "%2.2lu%2.2lu%2.2lu,",
            z_utc_time.q_Day, z_utc_time.q_Month, z_utc_time.q_Year % 100 );
      }
      else
      {
        q_len = snprintf( p_B, q_buf_len, ",");
      }
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);

    /* Magnetic variation and direction */
      if (p_fix->z_NavPos.u_MagDevGood)
      {
        d_MagVar = p_fix->z_NavPos.f_MagDeviation;
        if (d_MagVar < 0.0)
        {
          u_VarDir = 'W';
          d_MagVar *= -1.0;
        }
        q_len = snprintf( p_B, q_buf_len, "%.1lf,%c,", d_MagVar, u_VarDir);
      }
      else
      {
        q_len = snprintf( p_B, q_buf_len, ",,");
      }

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);
      
  /* Mode Indicator and Navigational status*/


      if (tm_nmea_is_differential_fix(p_fix))
      {
        u_mode_indicator = 'D'; /* Differential (DGPS) */
      }
      else
      {
    if (TRUE == tm_nmea_is_propagated_fix(p_fix))
      {
          u_mode_indicator = 'E'; /* Estimated (dead reckoning) */
      }
      else
      {
          u_mode_indicator = 'A'; /* Autonomous (Non DGPS) */
        }
      }
  /* 'V'as Nav status for equipment not providing navigational status */
  q_len = snprintf(p_B, q_buf_len, "%c,V", u_mode_indicator);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_B);

  tm_nmea_terminate_and_put(b_sentence, q_nmeaType);
      }
      
/******************************************************************************
@brief      Utility function to generate RMC sentence for GPS
******************************************************************************/
void tm_nmea_gprmc_put(const sm_FixInfoStructType *p_fix)
{
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GPRMC);
    return;
  }

  tm_nmea_rmc_put(p_fix, PDAPI_NMEA_GPRMC);
    }

/******************************************************************************
@brief      Utility function to generate RMC sentence for GAL
******************************************************************************/
void tm_nmea_garmc_put(const sm_FixInfoStructType *p_fix)
    {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GARMC);
    return;
  }
  tm_nmea_rmc_put(p_fix, PDAPI_NMEA_GARMC);
      }

/******************************************************************************
@brief      Utility function to generate RMC sentence for BDS
******************************************************************************/
void tm_nmea_gbrmc_put(const sm_FixInfoStructType *p_fix)
      {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) ||  /* Position invalid */
      (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)))
        {
    tm_nmea_empty_put(PDAPI_NMEA_GBRMC);
        return;
      }
  tm_nmea_rmc_put(p_fix, PDAPI_NMEA_GBRMC);
    }

/******************************************************************************
@brief      Utility function to generate RMC sentence for 
            multiple constellations (Talker ID $GN)
******************************************************************************/
void tm_nmea_gnrmc_put(const sm_FixInfoStructType *p_fix)
      {
  if ((NULL == p_fix) ||
      (FALSE == p_fix->z_NavPos.z_PosFlags.b_IsValid) || /* Position invalid */
      ((0 == tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo)) &&
       (0 == tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))))
  {
    tm_nmea_empty_put(PDAPI_NMEA_GNRMC);
      return;
    }

  tm_nmea_rmc_put(p_fix, PDAPI_NMEA_GNRMC);
  }

/*===========================================================================
FUNCTION tm_nmea_pstis_put

DESCRIPTION
  tm_nmea_pstis_put() is used to transmit PSTIS - GPS session start indication
     
  12      3    
  ||      |      
  $PSTIS,*hh<CR><LF>

  Field Description: 
  1) Talker ID.
  2) PSTIS for Proprietary SnapTrack Inc. session control message.
  3) Checksum

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS
===========================================================================*/
void tm_nmea_pstis_put(void)
{
  char b_sentence[200], *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence), q_len = 0;

  //Init NMEA Sentence
  q_len = tm_nmea_init_sentence(p_b, q_buf_len, PDAPI_NMEA_PSTIS);
  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);

  tm_nmea_terminate_and_put(b_sentence, PDAPI_NMEA_PSTIS);
}

/*===========================================================================

FUNCTION tm_nmea_diag_sio_close_cb

DESCRIPTION
This callback is closes the NMEA connection, if there is one.

DEPENDENCIES
None.

RETURN VALUE
None.  This procedure does not return.

SIDE EFFECTS
None.

===========================================================================*/
void tm_nmea_diag_sio_close_cb(void)
{
  TM_MED_0("Received DIAG SIO CLOSE CB");
  tm_nmea_send_message_no_payload(TM_CORE_NMEA_MSG_ID_DIAG_SIO_CLOSE_CB_2);
}

/*===========================================================================

FUNCTION tm_nmea_diag_close_port

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tm_nmea_diag_close_port(void (*close_func_ptr)(void))
{
#ifndef __GRIFFON__
  TM_MED_2("Diag closing nmea port: %d, %x", (int32)tm_nmea_sio_stream_id, close_func_ptr);

  if (tm_nmea_sio_stream_id != SIO_NO_STREAM_ID)
  {
    sio_close(tm_nmea_sio_stream_id, close_func_ptr);
    tm_nmea_sio_flag      = FALSE;
    tm_nmea_sio_stream_id = SIO_NO_STREAM_ID;
    return TRUE;
  }
#endif
  return FALSE;
}

/*===========================================================================

 FUNCTION tm_nmea_diag_open_port_part2

 DESCRIPTION

 DEPENDENCIES
 None.

 RETURN VALUE
 None.  This procedure does not return.

 SIDE EFFECTS
 None.

 ===========================================================================*/
void tm_nmea_diag_open_port_part2(void)
{
#ifndef __GRIFFON__
  TM_HIGH_1("DIAG NMEA Open Port_2: %d", (int32)tm_nmea_diag_port_to_open);

  if (tm_nmea_diag_port_to_open == TM_NMEA_DIAG_OPEN_PORT_USB)
  {
    TM_HIGH_0("USB NMEA port is not supported"); /*lint !e571 */
    return;
  }
  else if (tm_nmea_diag_port_to_open == TM_NMEA_DIAG_OPEN_PORT_UART1)
  {
    tm_nmea_open.port_id = SIO_PORT_UART_MAIN;
  }
  else if (tm_nmea_diag_port_to_open == TM_NMEA_DIAG_OPEN_PORT_UART2)
  {
#ifdef FEATURE_SECOND_UART
    tm_nmea_open.port_id = SIO_PORT_UART_AUX;
#else
    TM_HIGH_0("UART AUX NMEA port is not supported"); /*lint !e571 */
    return;
#endif /* FEATURE_SECOND_UART */
  }
  else
  {
    tm_nmea_open.port_id = SIO_PORT_SMD_GPS_NMEA;
  }

  tm_nmea_sio_stream_id = sio_open(&tm_nmea_open);

  if (tm_nmea_sio_stream_id != SIO_NO_STREAM_ID)
  {
    TM_HIGH_0("NMEA port opened"); /*lint !e571 */
    tm_nmea_sio_flag = TRUE;
  }
  else
  {
    TM_ERROR_0("NMEA port opening failed"); /*lint !e571 */
    tm_nmea_sio_flag = FALSE;
  }
#endif
}

/*===========================================================================

FUNCTION tm_nmea_diag_open_port

DESCRIPTION

DEPENDENCIES
None.

RETURN VALUE
None.  This procedure does not return.

SIDE EFFECTS
None.

===========================================================================*/
void tm_nmea_diag_open_port(tm_nmea_diag_open_port_id_e_type port_to_open)
{
#ifndef __GRIFFON__
  /* for Gobi, TM should not open the NMEA port */
#ifdef FEATURE_CGPS_LBS_TASK_NMEA_PORT
  return;
#else
  TM_MED("Diag opening nmea port: %d", (int32)port_to_open, 0, 0); /*lint !e571 */

  /* We were told to open, so try to do so.  Start by closing 
  ** any existing connection.
  */
  tm_nmea_diag_port_to_open = port_to_open;

  if (!tm_nmea_diag_close_port(tm_nmea_diag_sio_close_cb))
  {
    tm_nmea_diag_open_port_part2();
  }
#endif
#endif
}

/*===========================================================================

FUNCTION tm_nmea_debug

DESCRIPTION
  This function sends the debug NMEA sentences to LBS and also
  on the NMEA COM port if it is open.

  The debug NMEA string is already NMEA and NULL terminated.
  The string is sent out as is.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_nmea_debug(tm_nmea_data_s_type *pDebugNMEA)
{

#ifndef __GRIFFON__
#ifdef FEATURE_CGPS_LBS_TASK
  // LBS is NOT part of Griffon.

  (void)lbs_send_nmea_dbg_str(
     (const char *)&(pDebugNMEA->p_nmea_sentence[0]),
     pDebugNMEA->q_sentence_len);
#endif
#endif  // __GRIFFON__

  tm_nmea_port_put(0, (char *)&(pDebugNMEA->p_nmea_sentence[0]));
  TM_LOW_0("Sent Debug NMEA");
}

/*===========================================================================
FUNCTION tm_nmea_is_fresh_meas

DESCRIPTION
  Internal function to check if GPS Measurement is fresh

DEPENDENCIES

RETURN VALUE
  Boolean: TRUE if GPS Measurement is fresh, FALSE otherwise

SIDE EFFECTS

===========================================================================*/
static boolean tm_nmea_is_fresh_meas(const sm_GpsMeasRptStructType *p_Meas)
{
  uint32 q_I;
  const gnss_MeasStructType    *p_MeasStruct;

  for (q_I = 0; q_I < p_Meas->z_MeasBlk.z_GnssHeader.u_NumSvs; q_I++)
  {
    p_MeasStruct = (const gnss_MeasStructType*)&p_Meas->z_MeasBlk.z_Gnss[q_I];
    if ((p_MeasStruct->q_MeasStatus & (SM_MEAS_STATUS_VALID_FRESH | SM_MEAS_STATUS_INVALID_FLAGS)) == SM_MEAS_STATUS_SM_VALID)
  {
      TM_HIGH_2("Fresh Meas: MeasStatus=0x%x using SV=%d TRUE", p_MeasStruct->q_MeasStatus, p_MeasStruct->w_Sv);
          return TRUE;
        }
      }

  return FALSE;
}

/*===========================================================================
FUNCTION tm_nmea_set_gnss_meas_update

DESCRIPTION
  Stores the FCount from GPS measurement report

DEPENDENCIES

RETURN VALUE none

SIDE EFFECTS

===========================================================================*/
void tm_nmea_set_gnss_meas_update(const sm_GpsMeasRptStructType *p_Meas)
{
  const gnss_MeasBlkStructType *p_MeasBlk;
  gnss_MeasBlkSrcEnumType e_System;

  /* Sanity Check */
  if (p_Meas == NULL)
  {
    TM_ERROR_0("OEMDRETD Set Meas Update is NULL");
    return;
  }
  p_MeasBlk = (const gnss_MeasBlkStructType *)&(p_Meas->z_MeasBlk);
  e_System = sm_util_get_master_constellation(p_MeasBlk->z_GnssHeader.e_MeasBlkSrc);

  if (e_System == GNSS_MEAS_BLK_SRC_GPS)
  {
    q_LastGPSMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshGPSMeas      = tm_nmea_is_fresh_meas(p_Meas);
  }
  else if (e_System == GNSS_MEAS_BLK_SRC_GLO)
  {
    q_LastGLOMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshGLOMeas      = tm_nmea_is_fresh_meas(p_Meas);
  }
  else if (e_System == GNSS_MEAS_BLK_SRC_GAL)
  {
    q_LastGALMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshGALMeas      = tm_nmea_is_fresh_meas(p_Meas);
  }
  else if (e_System == GNSS_MEAS_BLK_SRC_BDS)
  {
    q_LastBDSMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshBDSMeas = tm_nmea_is_fresh_meas(p_Meas);
  }
  else if (e_System == GNSS_MEAS_BLK_SRC_QZSS_SBAS)
  {
    q_LastQZSSMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshQZSSMeas = tm_nmea_is_fresh_meas(p_Meas);
  }
  else if (e_System == GNSS_MEAS_BLK_SRC_NAVIC)
  {
    q_LastNAVICMeasRefCount = p_MeasBlk->z_GnssHeader.z_MeasClock.q_RefFCount;
    b_IsFreshNAVICMeas = tm_nmea_is_fresh_meas(p_Meas);
  }
}

/*===========================================================================
FUNCTION tm_nmea_is_differential_fix

DESCRIPTION
  Checks to see if the fix is differential or not

DEPENDENCIES

RETURN VALUE
  Boolean:
    TRUE  if the fix is differential (DGPS fix)
    FALSE if the fix is non-differential (non-DGPS fix)

SIDE EFFECTS

===========================================================================*/
static boolean tm_nmea_is_differential_fix(const sm_FixInfoStructType *p_fix)
{
  if (p_fix == NULL)
  {
    TM_ERROR_0("Is Differential Fix is NULL");
    return FALSE;
  }

  /* If iono correction flag is set, it means that the differential data from SBAS satellites is being applied. */
  if (p_fix->z_NavPos.z_PosFlags.b_IsSbasIonoCorr)
  {
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================
FUNCTION tm_nmea_is_propagated_fix

DESCRIPTION
  Checks to see if the fix is propagated (estimated) or not (valid)

DEPENDENCIES

RETURN VALUE
  Boolean:
    TRUE if the fix is invalid (propagated fix)
    FALSE if the fix is valid (non-propagated fix)

SIDE EFFECTS

===========================================================================*/
static boolean tm_nmea_is_propagated_fix(const sm_FixInfoStructType *p_fix)
{
  int32 l_FcountDiff;
   boolean b_IsPropagatedFix = FALSE;

  if (p_fix == NULL)
  {
    TM_ERROR_0("OEMDRETD Is Propagated Fix is NULL");
    return FALSE;
  }

  if (tm_core_info.config_info.oemdre_td_ctrl == FALSE)
  {
    TM_MED_0("OEMDRETD Is Propagated Fix Control disabled");
    return FALSE;
  }
  if (0 != p_fix->z_NavPos.z_PosFlags.b_IsInsOnlySln)
  {
    return TRUE;
  }

  /* Condition for the Automotive config (NV 73715) does NOT enable GPGSV_CONTROL (bit 2) and GPGSA_CONTROL (bit 3)
     Removing check for p_fix->z_NavPos.z_PosFlags.b_IsValid since it is always true */
  if (tm_nmea_num_gps_svs_in_fix(&p_fix->z_NavSvInfo) ||
      tm_nmea_num_glo_svs_in_fix(&p_fix->z_NavSvInfo) ||
      tm_nmea_num_gal_svs_in_fix(&p_fix->z_NavSvInfo) ||
      tm_nmea_num_bds_svs_in_fix(&p_fix->z_NavSvInfo) ||
      tm_nmea_num_qzss_svs_in_fix(&p_fix->z_NavSvInfo) ||
      tm_nmea_num_navic_svs_in_fix(&p_fix->z_NavSvInfo))
  {
    l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastGPSMeasRefCount;
     if (labs(l_FcountDiff) > 1000)
    {
       b_IsPropagatedFix &= !b_IsFreshGPSMeas;
    }
    l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastGLOMeasRefCount;
     if (labs(l_FcountDiff) > 1000)
    {
       b_IsPropagatedFix &= !b_IsFreshGLOMeas;
  }
     l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastGALMeasRefCount;
     if (labs(l_FcountDiff) > 1000)
  {
       b_IsPropagatedFix &= !b_IsFreshGALMeas;
    }
    l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastBDSMeasRefCount;
    if (labs(l_FcountDiff) > 1000)
    {
      b_IsPropagatedFix &= !b_IsFreshBDSMeas;
    }
    l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastQZSSMeasRefCount;
    if (labs(l_FcountDiff) > 1000)
    {
      b_IsPropagatedFix &= !b_IsFreshQZSSMeas;
    }
    l_FcountDiff = p_fix->z_NavPos.q_RefFCount - q_LastNAVICMeasRefCount;
    if (labs(l_FcountDiff) > 1000)
    {
      b_IsPropagatedFix &= !b_IsFreshNAVICMeas;
    }
     // return will be TRUE only when all of GPS, GLO and GAL is propagated fix
     return b_IsPropagatedFix;
  }
  else if (0 == p_fix->z_NavPos.z_PosFlags.b_IsInsOnlySln)
    return FALSE;
  else return TRUE;
}

/*===========================================================================
FUNCTION tm_nmea_fix_debug_report

DESCRIPTION
  This function generates debug NMEA sentence PQWS1 ans send it to LocMW
  directly via TM and also on the NMEA COM port if it is open.
  This function is not to be confused with tm_nmea_debug.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_fix_debug_report(tm_fix_status_debug_info_s_type* pz_FixDebugInfo)
  {
  char b_sentence[ 200 ];
  char *p_b = b_sentence;
  int q_buf_len = sizeof(b_sentence);
  int q_len = 0 ;
  boolean b_utc_valid = FALSE;
  tm_CalendarTimeT z_Time = {0};
  uint32 q_TotalDays = 0;

  if( NULL == pz_FixDebugInfo )
    {
    TM_ERROR_0("DEBUG NMEA: NULL payload");
    return;
    }
       
  /*  Universal Time Coordinated (UTC) of position: */
  b_utc_valid = tm_util_construct_gps_time(pz_FixDebugInfo->w_GpsWeek,
                                      pz_FixDebugInfo->q_GpsTimeMs,
                                      pz_FixDebugInfo->q_UtcTimeMs,
                                           &z_Time, &q_TotalDays);

  if (b_utc_valid == TRUE)
  {

      q_len = snprintf( p_b, q_buf_len,
                       "$PQWS1,%02" PRIu32 // Hour
                       "%02" PRIu32        // Minute
                       "%02" PRIu32        // Second
                       ".%02" PRIu32 ","   // Millisecond/10
                       "%hu" ","
                       "%lu", 
                       z_Time.q_Hour, z_Time.q_Minute,
                       z_Time.q_Second, z_Time.q_Millisecond / 10,
                       (uint16)pz_FixDebugInfo->w_FixInfoMask,
                       (uint32)pz_FixDebugInfo->q_HepeLimit);
  }
  else
  {
      q_len = snprintf( p_b, q_buf_len,
                        "$PQWS1,,"
                        "%hu" ","
                        "%lu",
                        (uint16)pz_FixDebugInfo->w_FixInfoMask,
                        (uint32)pz_FixDebugInfo->q_HepeLimit);
  }

  TM_NMEA_CHECK_LEN_AND_UPDATE_REMAINING(q_len, q_buf_len, p_b);
  /*Send NMEA string over to PDAPI*/
  tm_nmea_terminate_and_put( b_sentence , PDAPI_NMEA_DEBUG );

  }
/*===========================================================================
FUNCTION tm_nmea_debug

DESCRIPTION
  This function sends the debug NMEA sentences to LocMW directly via TM and also
  on the NMEA COM port if it is open. This function is not to be confused with
  tm_nmea_debug.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_debug_report(sm_DebugNMEA* pDebugNMEA)
{
  if (NULL == pDebugNMEA)
  {
    TM_ERROR_0("NULL NMEA string");
    return;
  }
  /* terminate the NMEA string and send it to HLOS*/
  tm_nmea_port_put(0, (char*)&(pDebugNMEA->u_Buffer[0]));

  /*Send NMEA string over to PDAPI*/
  tm_nmea_pdapi_send((char*)&(pDebugNMEA->u_Buffer[0]), PDAPI_NMEA_DEBUG);
}

/*===========================================================================
FUNCTION tm_nmea_var_len_debug_report

DESCRIPTION
  This function sends the debug NMEA sentences to LocMW directly via TM and also
  on the NMEA COM port if it is open. This function is not to be confused with
  tm_nmea_debug.

DEPENDENCIES
  On TM API tm_api_report_var_len_debug_nmea() to validate if NMEA debug sentence type is configured
  as enabled, before processing the variable length NMEA debug string.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tm_nmea_var_len_debug_report(sm_DebugNMEAVarLen *pDebugNMEA)
{
  if ((NULL == pDebugNMEA) || (NULL == pDebugNMEA->pu_NmeaBuff))
  {
    TM_ERROR_1("NULL NMEA str %p", pDebugNMEA);
    return;
  }
  TM_LOW("NMEA type %d len %lu buff %p", pDebugNMEA->nmea_type,
         pDebugNMEA->q_Length, pDebugNMEA->pu_NmeaBuff);

  /* terminate the NMEA string and send it to HLOS*/
  tm_nmea_port_put(0, (char *)pDebugNMEA->pu_NmeaBuff);

  /*Send NMEA string over to PDAPI*/
  nmea_var_len_pdapi_send(pDebugNMEA);
}
