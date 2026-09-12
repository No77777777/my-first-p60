#ifndef TDS_EXT_API_H
#define TDS_EXT_API_H

/*==========================================================================

      Diagnostic Services Packet Processing Command Code Defintions

Description
  This file contains the definitions for the external APIs used by non Modem
  modules.  

Copyright (c) 1993 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/api/public/tds_ext_api.h#2 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---    -----------------------------------------------------------

===========================================================================*/


#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "sys.h"
#include "msg.h"
#include "tds_ivoice_if.h"

/* TDS scenarios where scalar (and/or vector) Q6, DDR need be bumped up 
   for higher MIPS/bandwidth requirements */
typedef enum
{
  TDS_SCENARIO_BASE          = 0x00, /* placeholder, not used */
  TDS_SCENARIO_RXD_OFF_2JDS  = 0x01,
  TDS_SCENARIO_RXD_OFF_3JDS  = 0x02,
  TDS_SCENARIO_RXD_OFF_4JDS  = 0x03,
  TDS_SCENARIO_RXD_ON_1JDS   = 0x04,
  TDS_SCENARIO_RXD_ON_2JDS   = 0x05,
  TDS_SCENARIO_RXD_ON_3JDS   = 0x06,
  TDS_SCENARIO_RXD_ON_4JDS   = 0x07,
  TDS_SCENARIO_T2G_IRAT_GAP  = 0x08,
  TDS_SCENARIO_T2L_IRAT_GAP  = 0x09,
  TDS_SCENARIO_QTA_GAP       = 0x0A,
  TDS_SCENARIO_QTA_GAP_END   = 0x0B,
  TDS_SCENARIO_CPHY_SETUP    = 0x0C,
  TDS_SCENARIO_RXD_STATE_TRANSITION  = 0x0D,
  TDS_NUM_SCENARIO
}TDS_CLK_SCENARIO_NUM;


#define TDSCDMA_EOK          ( 0x00000000 ) /**< Success; completed with no errors. */
#define TDSCDMA_EFAILED      ( 0x00000001 ) /**< General failure. */
#define TDSCDMA_EBADPARAM    ( 0x00000002 ) /**< Bad operation parameter(s). */
#define TDSCDMA_EUNSUPPORTED ( 0x00000003 ) /**< Unsupported routine/operation. */
#define TDSCDMA_EVERSION     ( 0x00000004 ) /**< Unsupported version. */
#define TDSCDMA_EUNEXPECTED  ( 0x00000005 ) /**< Unexpected problem encountered. */
#define TDSCDMA_EPANIC       ( 0x00000006 ) /**< Unhandled problem occurred. */

#define TDSCDMA_ENORESOURCE  ( 0x00000007 ) /**< Unable to allocate resource(s). */
#define TDSCDMA_EHANDLE      ( 0x00000008 ) /**< Invalid handle. */
#define TDSCDMA_EALREADY     ( 0x00000009 ) /**< Operation is already processed. */
#define TDSCDMA_ENOTREADY    ( 0x0000000A ) /**< Operation is not ready to be processed. */
#define TDSCDMA_EPENDING     ( 0x0000000B ) /**< Operation is pending completion. */
#define TDSCDMA_EBUSY        ( 0x0000000C ) /**< Operation could not be accepted or processed. */
#define TDSCDMA_EABORTED     ( 0x0000000D ) /**< Operation aborted due to an error. */
#define TDSCDMA_EPREEMPTED   ( 0x0000000E ) /**< Operation preempted by a higher priority. */
#define TDSCDMA_ECONTINUE    ( 0x0000000F ) /**< Operation requests intervention to complete. */
#define TDSCDMA_EIMMEDIATE   ( 0x00000010 ) /**< Operation requests immediate intervention to complete. */
#define TDSCDMA_ENOTIMPL     ( 0x00000011 ) /**< Operation is not implemented. */
#define TDSCDMA_ENEEDMORE    ( 0x00000012 ) /**< Operation needs more data or resources. */
#define TDSCDMA_ELPC         ( 0x00000013 ) /**< Operation is a local procedure call. */
#define TDSCDMA_ENOMEMORY    ( 0x00000014 ) /**< Unable to allocate enough memory. */
#define TDSCDMA_ENOTEXIST    ( 0x00000015 ) /**< Item does not exist. */


/** Open command (synchronous API).
 * Use this command to acquire a handle to a new TDSCDMA voice session
 * instance.
 */
#define TDSCDMA_IVOICE_CMD_OPEN                     ( 0x20030013 )
/** Close command (Synchronous Blocking API).
 * Use this command to close the previously opened TDSCDMA voice session
 * instance.
 */
#define TDSCDMA_IVOICE_CMD_CLOSE                    ( 0x20030014 )

/** Start command ( synchronous API ).
 * This command shows the client's readiness for vocoder packet exchange with
 * the TDSCDMA protocol software.
 */
#define TDSCDMA_IVOICE_CMD_START                    ( 0x20030015 )

/** Stop command (synchronous API.)
 * This command shows the client cannot do any more vocoder packet exchange
 * with the TDSCDMA protocol software. The client sends this command to the
 * TDSCDMA protocol software as an acknowledgement for the event
 * TDSCDMA_IVOICE_EVENT_REQUEST_STOP.
 */
#define TDSCDMA_IVOICE_CMD_STOP                     ( 0x20030016 )

/** codec mode event (asynchronous event).
 * The TDSCMDA protocol software sends this event as part of the voice traffic
 * set up.
 */
#define TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE     ( 0x20030017 )

/** SCR mode event (asynchronous event).
 * The TDSCMDA protocol software sends this event as part of the voice traffic
 * set up.
 */
#define TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE       ( 0x20030018 )

/** SET UL/DL Logical channel events (asynchronous event).
 * The TDSCDMA protocol software "RRC" provides uplink and downlink logical
 * channel ID's to the client. 
 */
#define TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS   ( 0x20030019 )

/** Select owner event (asynchronous event).
 * The TDSCDMA protocol software will send this event to the client in
 * scenarios (ex: SRVCC, IRAT HO) when it wants the client to take the
 * ownership of the vocoder resource.
 */
#define TDSCDMA_IVOICE_EVENT_SELECT_OWNER           ( 0x2003001A )

/** Voice traffic start request (asynchronous event).
 * The TDSCDMA protocol software uses this event to request client to set up
 * the vocoder and start packet exchange with it.
 */
#define TDSCDMA_IVOICE_EVENT_REQUEST_START          ( 0x2003001B )

/** Voice traffic stop request (asynchronous event).
 * The TDSCDMA protocol software uses this event to request client to stop
 * vocoder and the packet exchange.
 */
#define TDSCDMA_IVOICE_EVENT_REQUEST_STOP           ( 0x2003001C )

/** Set VFR notification command (synchronous API).
 * TDS-L2 will provide the VFR notification event to the client for the
 * ongoing traffic over respective access stratum.
 */
#define TDSCDMA_IVOICEL2_CMD_SET_VFR_NOTIFICATION   ( 0x2003001D )

/** VFR notification event (asynchronous event).
 * TDSCDMA MAC will send this event every 20msec which is aligned to DPDCH
 * frame boundary/TTI.
 */
#define TDSCDMA_IVOICEL2_EVENT_VFR_NOTIFICATION     ( 0x2003001E )

/** Register Uplink logical channels with watermark queues (synchronous API)
 */
#define TDSCDMA_IVOICEL2_CMD_REGISTER_UL_SERVICE    ( 0x2003001F )

/** Register Downlink logical channels with watermark queues(synchronous API)
 */
#define TDSCDMA_IVOICEL2_CMD_REGISTER_DL_SERVICE    ( 0x20030020 )

/** Send sample rate command (Synchronous Blocking API).
 *
 * The client shall use this command to provide current operating mode for 
 * the configured vocoder.
 *
 * The Server shall constitute a speech codec info indication and sends it
 * to the Call Manager. The server need not be caching the sample rate information.
 * 
 * After successful processing, the command shall return TDSCDMA_EOK to the caller.
 */
#define TDSCDMA_IVOICE_CMD_SEND_SAMPLE_RATE         ( 0x20030026 )


extern uint32 tds_ext_audio_api(uint32 cmd_id, void* params, uint32 size);

#endif
