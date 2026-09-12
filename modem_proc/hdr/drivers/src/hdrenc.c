/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     H D R   E N C O D E R   D R I V E R

GENERAL DESCRIPTION
  This is the HDR encoder driver.  It provides an interface with the HDR
  encoder and encoder buffer hardware.  

EXTERNALIZED FUNCTIONS
  hdrenc_init
    Initialize the HDR encoder
  hdrenc_activate
    Activate the encoder driver
  hdrenc_deactivate
    Deactivate the encoder driver
  hdrenc_is_ready
    Returns whether the encoder buffer is ready for data
  hdrenc_send_frame
    Load data into the encoder
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrenc_init must be called before any other encoder functions.  
  
Copyright (c) 2000, 2001, 2003 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2004, 2005, 2006 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2007, 2008, 2009 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2010, 2011, 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrenc.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/14/14   wsh     Changes to support Jolokia bringup
05/27/14   wsh     Fixed compile error for Jolokia build
02/26/14   mbs     Used the correct ERAM index for BOLT
12/19/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/08/13   mbs     Added changes to write data at new FW locations
10/08/13   mbs     Added support for BOLT interface changes
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
05/06/13   mbs     Fixed compiler errors for BOLT build
04/02/13   smd     Passed the correct eram index to FW.
12/19/12   smd     Read ERAM index instead of its address. 
                   Added RUMI TX only support.
12/04/12   mbs     Added unified TX related changes for Triton
04/30/12   dsp     Fixed compiler warnings on Dime.
03/19/12   kss     Updated for Dime.
09/26/11   wsh     Fixed compiler warnings
08/26/11   kss     Added workaround in which reads are done between ERAM writes
07/27/11   ssu     Added hdrenc_get_rri_sizes and hdrenc_get_rri_reva_sizes.  
07/07/11   kss     Removed compiler warnings.
06/07/11   kss     Added changes for SVDO.
05/24/11   kss     Initial workarounds for encoder issue.
04/26/11   kss     Added FW fix.
06/21/10   cnx     Fixed compiler warnings.
12/14/09   kss     Set TX_MODE for MDM9k. 
08/07/09   etv     Removed use of hdrasm_write_burst_data.
07/20/09   etv     Removed hdrasm.h
11/10/08   kss     Remove intlocks.
10/13/08   kss     Lint fixes.
06/16/08   etv     Added timestamp to RL phy layer log packets.
02/04/08   etv     Added support for logging RL physical layer packet dump.
02/12/08   rmg     Created hdrenc_load_frame() for RevA/B.  Removed _rev_a 
                   and _rev_b versions.
09/25/07   rmg     Added hdrenc_load_rev_b_frame().
03/10/07   kss     Fixed compiler warning.
06/26/06   jyw     Added support to MPC.
06/19/06   etv     Added sanity check before logging RL packet.
05/01/06   kss     Removed FATAL lvl debug msg (mistakenly checked in earlier).
02/06/06   kss     Increase encoder buffer to size of physical layer packet.
07/25/05   kss     Moved subframe_id calc and DSP config to hdrrmac3.
07/25/05   kss     Fixed Rev A Format A frame loading.
06/20/05   kss     Added hdrenc_subframe_id_is_valid().
06/10/05   kss     Use interlace-based scramber seed.
05/20/05   kss     Free dsm items after loading into encoder buffer.
05/02/05   kss     Added hdrenc_is_rev_a_ready() function.
03/29/05   kss     Use FEATURE_HDR_REVA_L1; use HWIO_OUT().
02/10/05   kss     Added Rev A support.
11/12/04   kss     Updated for Rev 0.
09/03/04   ts      Derived from //depot/asic/msm6500/hdr/enc/hdrenc.c#1
                   Added changes for MSM6800 PLT builds
11/07/02   kss     Delete unused functions.
08/12/02   kss     Reset encoder in hdrenc_init().
05/30/01   kss     Fixed bug in hdrenc_send_frame().
05/25/01   kss     Integration changes -- several register writes were in the
                   wrong order for starting encoder load.
04/16/01   kss     Modified for the MSM5500.  (No longer uses BIF)
01/08/01   kss     Added workaround to enable encoder to work with MSM5000
                   UART.   (ASB bus cycle change.)
10/23/00   kss     Added EDI interface support. 
04/15/00   kss     Created.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"   /* Needed for byte, uint16, uint32, INTLOCK(), etc. */
#include "hdrmod.h"   /* HDR modulator */

#ifndef FEATURE_HDR_VI_VERIFICATION
#include "msm.h"      /* MSM5500 register definitions */
#include "hdrenc.h"   /* Externalized hdrenc declarations */
#include "err.h"      /* Error handling */
#include "hdrmod.h"   /* HDR modulator */
#include "hdrdebug.h" /* HDR Debug messages */
#include "hdrlogi.h"  /* Logging */
#include "hdrmdspmcr.h" /* TBD: to make the HWIO work for QTF */

#endif  /* FEATURE_HDR_VI_VERIFICATION */

#include "hdrts.h"

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h" /* needed to access hdrhit_rumi_cfg */
#endif /* FEATURE_HDR_RUMI */

/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.
                                
===========================================================================*/

/* ------------------------------------------------------------------------
                    REVMOD_END_MODE REGISTER DEFINITIONS
------------------------------------------------------------------------ */

/* Channel interleaver disable */
#define CHN_INT_OFF_ON_V      HWIO_REVMOD_ENC_MODE_CHN_INT_OFF_BMSK
#define CHN_INT_OFF_OFF_V     0x0000 

/* ------------------------------------------------------------------------
             REVMOD_EDI_CONTROL INTERFACE REGISTER BIT DEFINITIONS 
------------------------------------------------------------------------ */

/* Signals ARM that EDI is ready for a new byte. (Read) */
#define EDI_READY_M         HWIO_REVMOD_EDI_CONTROL_EDI_READY_BMSK
#define EDI_READY_V         HWIO_REVMOD_EDI_CONTROL_EDI_READY_BMSK

/* Signals end of frame to the EDI (Write)*/
#define EDI_EOF_V           HWIO_REVMOD_EDI_CONTROL_EDI_EOF_BMSK

/* Abort frame signal to EDI (Write) */
#define EDI_ABORT_V         HWIO_REVMOD_EDI_CONTROL_EDI_ABORT_BMSK


/* ------------------------------------------------------------------------
             REVMOD_TENC_STATUS INTERFACE REGISTER BIT DEFINITIONS 
------------------------------------------------------------------------ */

/* Signals ARM that encoder is ready for new data. */
#define TENC_STATUS_EDI_READY_M    HWIO_REVMOD_TENC_STATUS_EDI_READY_BMSK
#define TENC_STATUS_EDI_READY_V    HWIO_REVMOD_TENC_STATUS_EDI_READY_BMSK


/* ------------------------------------------------------------------------
             REVMOD_TENC_CTL INTERFACE REGISTER BIT DEFINITIONS 
------------------------------------------------------------------------ */

/* Selects MCDO carrier specific ERAM bank. */
#define MCDO_CARRIER_SEL_M    HWIO_MOD_ERAM_DATA_CTL_MCDO_CARRIER_SEL_BMSK
#define MCDO_CARRIER_SEL_S    HWIO_MOD_ERAM_DATA_CTL_MCDO_CARRIER_SEL_SHFT


/* ------------------------------------------------------------------------
             MOD_ERAM_DATA_CTL INTERFACE REGISTER BIT DEFINITIONS 
------------------------------------------------------------------------ */

/* Signals encoder/modulator that data is being written to EDI. */
#define DATA_WRITE_V    HWIO_REVMOD_TENC_CTL_DATA_BMSK
#define DATA_DONE_V     0x0000


/* ------------------------------------------------------------------------
                   ENCODER FRAME SIZE DEFINITIONS
------------------------------------------------------------------------ */

/* Frame sizes (in bytes) for each rate when they are ready to be sent 
** to the encoder.  Does not include CRC and tail bits, which are added 
** by encoder hardware.
*/
#define RATE_9600_FRAME_SIZE     29
#define RATE_19200_FRAME_SIZE    61
#define RATE_38400_FRAME_SIZE   125
#define RATE_76800_FRAME_SIZE   253
#define RATE_153600_FRAME_SIZE  509

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
uint32 hdrenc_enc_sel = 0x3;

/* RRI Frame Sizes Table In Bytes */
uint16 hdrenc_rri_sizes[] = {0,
                             RATE_9600_FRAME_SIZE,
                             RATE_19200_FRAME_SIZE,
                             RATE_38400_FRAME_SIZE,
                             RATE_76800_FRAME_SIZE,
                             RATE_153600_FRAME_SIZE};   

#ifdef FEATURE_HDR_REVA_L1
/* Rev A Frame sizes (in bytes) for each rate when they are ready to be 
** sent to the encoder.  Does not include CRC and tail bits, which are
** added by encoder hardware.
*/
#define PAYLOAD_0_SIZE            0
#define PAYLOAD_128_SIZE         12
#define PAYLOAD_256_SIZE         28
#define PAYLOAD_512_SIZE         60 
#define PAYLOAD_768_SIZE         92
#define PAYLOAD_1024_SIZE       124
#define PAYLOAD_1536_SIZE       188
#define PAYLOAD_2048_SIZE       252
#define PAYLOAD_3072_SIZE       380
#define PAYLOAD_4096_SIZE       508 
#define PAYLOAD_6144_SIZE       764
#define PAYLOAD_8192_SIZE      1020
#define PAYLOAD_12288_SIZE     1532


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* RRI Frame Sizes Table In Bytes */
uint16 hdrenc_rri_reva_sizes[] = { PAYLOAD_0_SIZE,
                                   PAYLOAD_128_SIZE,
                                   PAYLOAD_256_SIZE,
                                   PAYLOAD_512_SIZE,
                                   PAYLOAD_768_SIZE,
                                   PAYLOAD_1024_SIZE,
                                   PAYLOAD_1536_SIZE,
                                   PAYLOAD_2048_SIZE,
                                   PAYLOAD_3072_SIZE,
                                   PAYLOAD_4096_SIZE,
                                   PAYLOAD_6144_SIZE,
                                   PAYLOAD_8192_SIZE,
                                   PAYLOAD_12288_SIZE };


/* Temporary buffer for loading. Sized in uint32's (thus the div by 4) */
uint32 hdrenc_buffer[(PAYLOAD_12288_SIZE/4)+1];

#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_UNIFIED_TX
#define TX_HW_MOD_DATA_REG TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0
#else
#define TX_HW_MOD_DATA_REG MOD_ERAM_DATA
#endif  /* FEATURE_HDR_TRITON_MODEM */

#ifndef FEATURE_HDR_VI_VERIFICATION
rex_crit_sect_type              enc_eram_crit_sect;
boolean                         enc_eram_init = FALSE;
#endif /* FEATURE_HDR_VI_VERIFICATION */

void hdrenc_eram_lock( void )
{
#ifndef FEATURE_HDR_VI_VERIFICATION
  // init on first time through
  if (enc_eram_init == FALSE)
  {
    enc_eram_init = TRUE;
    rex_init_crit_sect(&enc_eram_crit_sect);
  }

  // enter crit_sect (lock)
  rex_enter_crit_sect(&enc_eram_crit_sect);
#endif /* FEATURE_HDR_VI_VERIFICATION */
}

void hdrenc_eram_unlock( void )
{
#ifndef FEATURE_HDR_VI_VERIFICATION
  // leave crit sect (unlock)
  rex_leave_crit_sect(&enc_eram_crit_sect);
#endif /* FEATURE_HDR_VI_VERIFICATION */
}


/*===========================================================================
FUNCTION HDRENC_INIT                                                 EXTERNAL

DESCRIPTION
  Initialize the encoder to known state.
  
DEPENDENCIES
  None.
  
PARAMETERS
  None.  

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrenc_init( void ) 
{
#ifndef FEATURE_HDR_UNIFIED_TX
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Set up encoder:  Interleaver on */
  HWIO_OUT(REVMOD_ENC_MODE, CHN_INT_OFF_OFF_V );

  /* Setup encoder input buffer for word writes */
  HWIO_OUT(MOD_ERAM_DATA_CTL, hdrenc_enc_sel );
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_UNIFIED_TX */
}



/*===========================================================================
FUNCTION HDRENC_IS_READY                                             EXTERNAL

DESCRIPTION
  Returns true if the encoder buffer is ready for data. (For Rev 0)
  
DEPENDENCIES
  None.

PARAMETERS
  None.  

RETURN VALUE
  TRUE  - encoder buffer ready for data.
  FALSE - encoder buffer not ready, still working on previous data.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrenc_is_ready( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX
  return ( HWIO_IN(TX_UNIFIED_ENC_STATUS) == 0 );
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  return ( (HWIO_REVMOD_TENC_STATUS_INM(TENC_STATUS_EDI_READY_M)) == 
          TENC_STATUS_EDI_READY_V );
#else
  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Enc ready %d", hdrts_get_rtc_cx1() );
  return TRUE;
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */
}


/*===========================================================================
FUNCTION HDRENC_SEND_FRAME                                           EXTERNAL

DESCRIPTION
  Sends an entire HDR frame to the encoder, given the data, last byte value,
  data length, and RRI value.  Zeros are written to pad if the data length 
  does not fill the entire frame.  
Example for one rev0 pkt size
- 001: 9.6Kbps, payload 256
256 includes 16bit CRC and 6bits tailbits
Total data written = 256 - 16 - 6 = 234bits of data
(256 - 16 - 6) / 32 = 8 32bit words = 7 32bits + 10 bits
SW needs to zero pad 32-10 = 22 bits
    -----------------------------------------------------------
    |<----32bit---->|   ......   |<----32bit---->|
    -----------------------------------------------------------
    |<--------------7words of 32bit------------->|-------------

    |232|233|0|0|0|0|0|0|224|225|226|227|228|229|230|231| ...  8|9|10|11|12|13|14|15|0|1|2|3|4|5|6|7|
    bits in little endian

Hence last byte is shifted by 8 to align with position 14 and 15
From TX_UNIFIED_ENC_ERAM_DATA_INCRACC_0
  
DEPENDENCIES
  None

PARAMETERS
  data_ptr    - pointer to data.
  data_length - length of data in data_ptr, in bytes.
  last_byte   - the value that the last byte in the frame should be.
  rri         - the RRI value that the frame should be send with.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrenc_send_frame(

  /* Frame payload */
  uint8         *data_ptr,

  /* Length of data in data_ptr (bytes) */
  uint16         data_length,
  
  /* Last byte of frame. Only 2 bits used. */
  uint8          last_byte,  

  /* RRI of frame. */
  uint8          rri

)
{

  /* Frame size, in bytes */
  int frame_size;

  /* Loop counter */
  uint16 i, j;

  /* Temporary pointer */
  void* tmp_ptr;

#ifndef FEATURE_HDR_UNIFIED_TX
  hdrfw_enc_evdo_ib_t  *data_write_ptr;
#endif /* FEATURE_HDR_UNIFIED_TX */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_RUMI
  last_byte = 0xC0;
    /* force last byte to be 0xC0 to match with system vector */
#endif /* FEATURE_HDR_RUMI */

  /* Get frame size, in bytes. Subtract one to account for the last byte */
  frame_size = hdrenc_rri_sizes[rri];
  
  /* Sanity check. */
  if(frame_size < data_length)
  {
    ERR_FATAL("RRI->Encoder bytes load mismatch! %d < %d",
        frame_size,data_length,0);
  }

  HDR_MSG_PROT_3( MSG_LEGACY_LOW, 
                  "Load encoder bytes %d %d %d", 
                  frame_size, data_length, rri);

  hdrenc_eram_lock();

  /* Set RRI channel in modulator before triggering encoder */
  hdrmod_set_rri(rri);

#ifdef FEATURE_HDR_UNIFIED_TX
  /* Tell encoder/modulator we are about to load data */
  HWIO_OUT(TX_UNIFIED_ERAM_CTL, 0); // Set the starting address
  HWIO_OUT(TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 1); // Activate the starting address
  HWIO_OUT(TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 0); // One cycle delay to ensure address is activated
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Setup encoder input buffer for word writes */
  HWIO_OUT(MOD_ERAM_DATA_CTL, hdrenc_enc_sel );
  /* Tell encoder/modulator we are about to load data */
  HWIO_OUT( REVMOD_TENC_CTL, DATA_WRITE_V);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

  /* Zero out bytes to fill out last data word. Used for partial frame. */
  data_ptr[data_length] = 0;
  data_ptr[data_length + 1] = 0;
  data_ptr[data_length + 2] = 0;

  /* Set last byte as mac trailer. Used for full frame. 
     Note that in full frame mode the mac_trailer is written automatically
     when the word containing the last data byte is loaded. */
  data_ptr[frame_size] = last_byte;

#ifndef FEATURE_HDR_UNIFIED_TX
  data_write_ptr = (hdrfw_enc_evdo_ib_t *) hdrmdsp_shared_mem_interface->read.encIbPtr;
#endif /* FEATURE_HDR_UNIFIED_TX */
  
  /* Loop through and load */
  for (i=0, j=0; i<data_length; i+=4, j++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning:
       cast increases required alignment of target type. */
    tmp_ptr = (void *)&(data_ptr[i]);
    /* Load into ERAM interface */

#ifndef FEATURE_HDR_UNIFIED_TX
    data_write_ptr->car[0].packet[0].samp[j] = *(uint32 *)tmp_ptr;
#else    
    HWIO_OUT( TX_HW_MOD_DATA_REG, *(uint32*)tmp_ptr);
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.verbose_debug )
    {
      HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                      *(uint32*)tmp_ptr, i, j);
    }
#endif /* FEATURE_HDR_RUMI */
  }

  /* For partial frames, we need to pad with zeros */
  if (data_length != frame_size)
  {
    /* Loop for zero padding */
    for (;i<frame_size-1; i+=4, j++)
    {
#ifndef FEATURE_HDR_UNIFIED_TX
      data_write_ptr->car[0].packet[0].samp[j] = 0;
#else    
      HWIO_OUT( TX_HW_MOD_DATA_REG, 0);
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
      if ( hdrhit_rumi_cfg.verbose_debug )
      {
        HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                        0, i, j);
      }
#endif /* FEATURE_HDR_RUMI */
    }

#ifndef FEATURE_HDR_UNIFIED_TX
    data_write_ptr->car[0].packet[0].samp[j] = (last_byte<<8);
#else    
    HWIO_OUT( TX_HW_MOD_DATA_REG, (last_byte<<8));
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.verbose_debug )
    {
      HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                      (last_byte << 8), i, j);
    }
#endif /* FEATURE_HDR_RUMI */
  }


#ifdef FEATURE_HDR_UNIFIED_TX
  /* Trigger DO encoder. This register is programmed after programming
   * all DO encoder parameters for all carriers. For rev0 SW writes this
   * register. For revA/B FW writes this register.
   */
  HWIO_OUT(TX_DO_ENC_TRIGGER, 1);    
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Write RRI to encoder */
  HWIO_OUT( REVMOD_ENC_RATE, rri);
  /* Tell the HW that the frame has been loaded */
  HWIO_OUT( MOD_ERAM_WR_DONE, 1);
  /* Tell EDI that the frame has been loaded. */ 
  HWIO_OUT( REVMOD_EDI_CONTROL, EDI_EOF_V);
  /* Tell encoder/modulator that we are done writing data. */
  HWIO_OUT( REVMOD_TENC_CTL, DATA_DONE_V);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

  hdrenc_eram_unlock();
}



#ifdef FEATURE_HDR_REVA_L1

/*===========================================================================
FUNCTION HDRENC_LOAD_FRAME

DESCRIPTION
  Writes packet data to the encoder input buffer, and send the frame at the
  RRI indicated. Padding is added if needed. 
  
DEPENDENCIES
  The encoder must have been initialized via hdrenc_init().  Must call
  hdrenc_is_ready previous to this function to verify encoder is ready.

PARAMETERS
  phy_chan_id - Physical channel id
  data_ptr    - pointer to the data to load into the encoder
  last_byte   - the last byte (containing the last 2 bits) of the frame
  length      - the byte length of the data to load from *data_ptr

RETURN VALUE
  For FEATURE_HDR_REVA_L1 hw this function returns a handle for the sent packet.
  The value of the handle is 0 - 127. Any number over 127 is invalid.  This
  is so the caller can keep track of packets in case they are not accepted or
  nacked in the future.

SIDE EFFECTS
  Note that the dsm item sent is NOT freed!
  
===========================================================================*/
void hdrenc_load_frame
(
  /* Physical Channel id */
  uint8          phy_chan_id,

  /* Frame payload */
  dsm_item_type *item_ptr,

  /* Length of data in data_ptr (bytes) */
  uint16         data_length,
  
  /* Last byte of frame. Only 2 bits used. */
  uint8          last_byte,  

  /* RRI of frame. */
  uint8          rri

)
{

  /* Frame size, in bytes */
  int frame_size;

  /* Loop counter */
  uint16 i=0, j = 0;

  /* Byte pointer for buffer */
  byte *data_ptr;

  void* tmp_ptr;
    /* Temporary pointer */

#ifndef FEATURE_HDR_UNIFIED_TX
  hdrfw_enc_evdo_ib_t  *data_write_ptr = NULL;
#else
  uint8 bank_index = 0;
    /* bank index */
#endif /* FEATURE_HDR_UNIFIED_TX */

  uint8 eram_index = 0;
    /* ERAM index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_RUMI
  last_byte = 0xC0;
    /* force last byte to be 0xC0 to match with system vector */
#endif /* FEATURE_HDR_RUMI */
  
  /* Get frame size, in bytes. Subtract one to account for the last byte */
  frame_size = hdrenc_rri_reva_sizes[rri];
   
  /* Sanity check. */
  if((frame_size < data_length) || (data_length > PAYLOAD_12288_SIZE))
  {
    ERR_FATAL("RRI->Encoder bytes load mismatch! %d < %d %d", 
        frame_size, data_length, rri);
  }

  HDR_MSG_PROT_3( MSG_LEGACY_LOW, 
            "Load encoder bytes %d %d %d", 
            frame_size, data_length, rri);

  hdrenc_eram_lock();

  /* For now, extract entire packet into buffer. */
  /* TODO: extract in pieces, as needed to avoid using huge buffer */
  data_ptr = (byte*)hdrenc_buffer;
  (void) dsm_extract(item_ptr, 0, data_ptr, data_length);
     
  if ( hdrmdsp_shared_mem_interface == NULL )
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, "FW shared memory ptr NULL");
    return;
  }

#ifndef FEATURE_HDR_UNIFIED_TX
  data_write_ptr = (hdrfw_enc_evdo_ib_t *) hdrmdsp_shared_mem_interface->read.encIbPtr;

  /* Select ERAM bank for the specified carrier from FW interface */
  eram_index = hdrmdsp_shared_mem_interface->read.rmac_subframe_ind.
      rmacInfo[phy_chan_id].rlPacket.eramIndex;

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_PROT_4( MSG_LEGACY_MED, 
                  "enc_load_frame: eramIndex %d carrier_id %d data %d frame_size %d",
                  eram_index, phy_chan_id, data_length, frame_size );
#else
  HDR_MSG_PROT_2( MSG_LEGACY_LOW, 
                  "enc_load_frame: eramIndex %d carrier_id %d",
                   eram_index, phy_chan_id );
#endif /* FEATURE_HDR_RUMI */

#else /* FEATURE_HDR_UNIFIED_TX */
  /* Select ERAM bank for the specified carrier from FW interface */
  bank_index = hdrmdsp_shared_mem_interface->read.rmac_subframe_ind.
      rmacInfo[phy_chan_id].rlPacket.eramIndex;

  eram_index = ( 4*phy_chan_id )+ bank_index;
   /* FW populate bank index (0-3) in shared memory. SW needs to convert it to
      eram index. 
         carrier 0: eram index is 0-3
         carrier 1: eram index is 4-7
         carrier 2: eram index is 8-11
   */

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_PROT_5( MSG_LEGACY_MED, 
                  "enc_load_frame: bank_index %d eramIndex %d carrier_id %d data %d frame_size %d",
                  bank_index, eram_index, phy_chan_id, data_length, frame_size );
#else
  HDR_MSG_PROT_3( MSG_LEGACY_LOW, 
                  "enc_load_frame: bank_index %d eramIndex %d carrier_id %d",
                  bank_index, eram_index, phy_chan_id );
#endif /* FEATURE_HDR_RUMI */
#endif /* FEATURE_HDR_UNIFIED_TX */


#ifdef FEATURE_HDR_UNIFIED_TX
  /* Use the FW provided bank index to set the starting address */
  HWIO_OUT(TX_UNIFIED_ERAM_CTL, eram_index);
  /* Activate the starting address */
  HWIO_OUT(TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 1);
  /* One cycle delay to ensure address is activated */
  HWIO_OUT(TX_UNIFIED_ENC_ERAM_INCRACC_INIT, 0);
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Select ERAM bank for the specified carrier */
  HWIO_OUT( MOD_ERAM_DATA_CTL, 
            ((phy_chan_id << MCDO_CARRIER_SEL_S) & MCDO_CARRIER_SEL_M) |
            hdrenc_enc_sel );
       
  /* Write RRI to encoder */
  HWIO_OUT(REVMOD_ENC_RATE, rri );
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */
        
  /* Zero out bytes to fill out last data word. 
     Used when loading a partial frame. */
  data_ptr[data_length] = 0;
  data_ptr[data_length + 1] = 0;
  data_ptr[data_length + 2] = 0;

  /* Loop through and load */
  for (i=0, j = 0; i<data_length; i+=4, j++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    tmp_ptr = (void*)&(data_ptr[i]);

#if defined(FEATURE_HDR_BOLT_MODEM) && (!defined(FEATURE_HDR_UNIFIED_TX))
    data_write_ptr->car[phy_chan_id].packet[eram_index].samp[j] = *(uint32 *)tmp_ptr;
#else    
    /* Load into ERAM interface */
    HWIO_OUT( TX_HW_MOD_DATA_REG, *(uint32*)tmp_ptr);
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.verbose_debug )
    {
      HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                      *(uint32*)tmp_ptr, i, j);
    }
#endif /* FEATURE_HDR_RUMI */
  }

  /* For partial frames, we need to pad with zeros */
  if (data_length != frame_size)
  {
    /* Loop for zero padding */
    for (; i < frame_size; i += 4, j++)
    {
#ifndef FEATURE_HDR_UNIFIED_TX
      data_write_ptr->car[phy_chan_id].packet[eram_index].samp[j] = 0;
#else    
      /* Load into ERAM interface */
      HWIO_OUT( TX_HW_MOD_DATA_REG, 0);
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
      if ( hdrhit_rumi_cfg.verbose_debug )
      {
        HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                        0, i, j);
      }
#endif /* FEATURE_HDR_RUMI */
    }
  }
  
#ifndef FEATURE_HDR_UNIFIED_TX
   data_write_ptr->car[phy_chan_id].packet[eram_index].samp[j] = (uint32)last_byte;
#else    
   /* Load last word */
   HWIO_OUT( TX_HW_MOD_DATA_REG, (uint32)last_byte);
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.verbose_debug )
    {
      HDR_MSG_PROT_3( MSG_LEGACY_MED, "loading %x@%d/%d", 
                      last_byte, i, j);
    }
#endif /* FEATURE_HDR_RUMI */

#ifndef FEATURE_HDR_UNIFIED_TX
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Tell the HW that the frame has been loaded */
  HWIO_OUT(MOD_ERAM_WR_DONE, 1);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_UNIFIED_TX */

  hdrenc_eram_unlock();
} /* hdrenc_load_frame() */


 /* EJECT */
/*===========================================================================
FUNCTION HDRENC_LOG_RTC_PHY_LAYER_PKT                          INTERNAL

DESCRIPTION
  This function ships out one log packet for every Subtype-3 RTC MAC packet 
  sent to the encoder. 

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr - chain of DSM items containing the subtype-3 RTC packet.
  mac_trailer - MAC trailer. 
  data_length - Size of the subtype-3 RTC physical layer packet. 
  pkt_size_index - index to get the physical layer frame size.
  phy_rev - Physical layer revision
  carrier_id_ptr - Pointer to struct containing the band class and Channel no.
                   information. Contains valid info only for Rev-B.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrenc_log_rtc_phy_layer_pkt
(
  dsm_item_type *pkt_ptr,
  uint8 mac_trailer,
  uint16 data_length,
  uint16 pkt_size_index,
  uint8 phy_rev,
  hdrlog_mc_carrier_id_type *carrier_id_ptr,
  uint8 subframe_id
)
{
  LOG_HDR_RL_PHY_LAYER_PKT_C_type *log_ptr = NULL;
    /* pointer to RTC log packet */

  static uint16 log_seq_num = 0;
    /* Sequence number of Subtype-3 RTCMAC log packet. Needs to retain value
     * across function call. Hence declared static. */

  uint16 frame_size = 0;
    /* ST-3 RTC physical layer frame size */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 
  
  /* Get the size of the physical layer frame with out the CRC and TAIL */
  frame_size = hdrenc_rri_reva_sizes[pkt_size_index];

  /* If frame size is greater than or equal to the data length, go ahead 
     and log the pkt */
  if (frame_size >= data_length)
  {
    /* Allocate memory for the log packet */
    log_ptr = (LOG_HDR_RL_PHY_LAYER_PKT_C_type*) log_alloc( 
                LOG_HDR_RL_PHY_LAYER_PKT_C,
                (sizeof(LOG_HDR_RL_PHY_LAYER_PKT_C_type) +
                frame_size + 4 - 1) );


    /* if log_alloc failed, print error message and exit */
    if ( !log_ptr )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Logging RTC phy pkt failed" );
      return;
    }

    log_ptr->version = HDRLOG_RL_PHY_LAYER_PKT_LOG_VERSION;

    /* Record the sequence number of the log packet */ 
    log_ptr->log_seq_num = (++log_seq_num);

    /* Record the physical layer revision */
    log_ptr->phy_pkt.pkt_info.phy_rev = phy_rev;

    /* Record the subframe index corresponding to this packet tx */
    log_ptr->subframe_num = subframe_id;

#ifdef FEATURE_HDR_REVB
    if (phy_rev == HDRLOG_PHY_LAYER_REVISION_B)
    {
#ifdef FEATURE_MEMCPY_REMOVAL
      log_ptr->phy_pkt.carrier_id = *carrier_id_ptr;
#else
      memcpy ( (void*) &log_ptr->phy_pkt.carrier_id, 
               (void*) carrier_id_ptr,
               sizeof ( hdrlog_mc_carrier_id_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
    }
#endif /* FEATURE_HDR_REVB */

    /* Record the packet length in 16 bytes */
    log_ptr->phy_pkt.pkt_info.pkt_len_in_16bytes = ((frame_size+4) / 16);

    /* Extract the ST-3 RMAC packet directly into the log packet */
    (void) dsm_extract( 
      pkt_ptr, 
      0, 
      (void*) &(log_ptr->phy_pkt.data[0]), 
      data_length );

    /* Pad rest of the packet with zeroes */
    memset( 
      (void*) &(log_ptr->phy_pkt.data[data_length]),
      0x00,
      (frame_size + 4 - data_length) );

    /* Set the MAC trailer */
    log_ptr->phy_pkt.data[frame_size] = mac_trailer;

    /* Ship out! */
    log_commit( (log_type*) log_ptr );

  }
  else
  {
    /* Print an error message... */
    HDR_MSG_PROT_2(MSG_LEGACY_ERROR, 
                   "Frame size %d is less than data length %d. Skip logging",
                   frame_size, 
                   data_length);
  }

} /* hdrenc_log_rtc_phy_layer_pkt */
#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================
FUNCTION HDRENC_GET_RRI_SIZES

DESCRIPTION
  This function returns the value stored in the
  "index"th index of hdrenc_rri_sizes array. 

DEPENDENCIES
  None.

PARAMETERS
  index - the value of this index in the array is returned.

RETURN VALUE
  The value stored in the "index"th index of hdrenc_rri_sizes array.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrenc_get_rri_sizes
(
  int index
)
{
  return hdrenc_rri_sizes[index];
}/* hdrenc_get_rri_sizes */


/*===========================================================================
FUNCTION HDRENC_GET_RRI_REVA_SIZES

DESCRIPTION
  This function returns the value stored in the
  "index"th index of hdrenc_rri_reva_sizes array. 

DEPENDENCIES
  None.

PARAMETERS
  index - the value of this index in the array is returned.

RETURN VALUE
  The value stored in the "index"th index of hdrenc_rri_reva_sizes array.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrenc_get_rri_reva_sizes
(
  int index
)
{
  return hdrenc_rri_reva_sizes[index];
}/* hdrenc_get_rri_sizes */


#endif /* FEATURE_FACTORY_TESTMODE */
