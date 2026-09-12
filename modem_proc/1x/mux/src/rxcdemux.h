#ifndef RXCDEMUX_H
#define RXCDEMUX_H
/*===========================================================================

           C D M A    D E -M U L T I P L E X I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the receive de-multiplexing module
  that are used by the rxc unit.  This header file is internal to the rxc
  unit.

  Copyright (c) 1997 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$PVCSPath: O:/src/asw/COMMON/vcs/rxcdemux.h_v   1.2   20 Apr 2001 11:47:36   sanjayr  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcdemux.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/26/09   jtm     Added comdef.h since it was implicitly included.
07/30/04   vlc     Changed curr_ch to rxc_curr_ch.
06/01/04   vlc     Added externs to allow F-PDCH code to call existing functions
                   for encapsulated MuxPDU5 processing.
04/18/03   bn      Removed JCDMA Features in FER so that it can apply to DIAG
04/20/01   sr      Merged in from MSM5100 archive
03/20/01   ych     Merged JCDMA Features.
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "comdef.h"
#include "customer.h"

//other
#include "dsdvrlp_rx.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

extern dsdvrlp_fwd_channel_enum_type rxc_curr_ch; /* FCH, DCCH, or PDCH is being demuxed */
extern dsrlp_mux_pdu_enum_type rxc_mux_pdu; /* MUX PDU type (1, 2...) */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION RXC_TC_DATA

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_data( void );

/*===========================================================================
FUNCTION      RXC_INIT_AVERAGED_FER

DESCRIPTION   This function initializes erasure frames for poor FER alarm.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
extern void  rxc_init_averaged_fer( void );

/*===========================================================================
FUNCTION      RXC_PRI_DATA

DESCRIPTION   Services primary traffic stream for the FCH, DCCH and PDCH for
              Data 3G SO.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
extern void rxc_pri_data
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX1 frame statistic                */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
);

/*===========================================================================
FUNCTION      RXC_SEC_DATA

DESCRIPTION   Services secondary traffic stream for the FCH, DCCH and PDCH for
              Data 3G SO.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
extern void rxc_sec_data
(
  byte bits[],                  /* Address of first byte of data           */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  word mux,                     /* CAI MUX frame statistic                 */
  rxc_dec_type *buf_ptr         /* Ptr to buffer which holds decoder data  */
);

/*===========================================================================

FUNCTION rxc_tc_data_get_rlp_list

DESCRIPTION
  This function gets 1 linked list element from RLP client to store
  information of 1 PDU payload.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_data_get_rlp_list (void);

/*===========================================================================

FUNCTION rxc_tc_data_give_rlp_list

DESCRIPTION
  This function gives 1 PDU worth of channel data to RLP client.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void rxc_tc_data_give_rlp_list (void);

#endif /* RXCDEMUX_H */
