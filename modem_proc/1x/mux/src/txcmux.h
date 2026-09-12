#ifndef TXCMUX_H
#define TXCMUX_H

/*===========================================================================

             C D M A     M U L T I P L E X I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the multiplexing module that
  are used by the txc unit.  This header file is internal to the txc unit.

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

$PVCSPath: O:/src/asw/COMMON/vcs/txcmux.h_v   1.5   24 Jul 2002 17:05:10   hkulkarn  $
$Header: //components/rel/1x.mpss/8.0/mux/src/txcmux.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/25/11   jtm     Added Mux Vocoder interface module.
12/15/10   jtm     Feature cleanup.
10/30/10   jtm     Modem 1x SU API changes.
06/10/10   jtm     Eliminate dependency on log_dmss.h
02/04/10   jtm     Removed FEATURE_PLT.
01/20/10   mca     Updated PACKED macro
03/26/09   jtm     Eliminated implicit includes.
04/20/04   jrp     Fixed lint warnings.
07/24/02   hrk     Added prototype for txc_tdso_sec_frame_size().
06/17/02   hrk     Changed prototype for txc_build_frame().
06/12/02   hrk     Added prototype for txc_sec().
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     changed prototype of build_blank_frame () to support DCCH.
01/11/01   tc      Merged MSM_MUX1X.00.00.06.
11/23/00   tc      Fixed txc_data_pump_build_frame for MSM5100 PLT.
12/07/00   bgc     Fixed typo for FEATURE_PLT.
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"

//mux
#include "txcmc_i.h"
#include "txctraffic_i.h"
#include "mux_logtypes.h"

//other
#include "log.h"
#include "queue.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Log buffer for accumulated MUX frame log */
typedef PACK( struct )
{
  mux_log_r_mux1_type   entry;
  byte                  count;
} txc_log_mux_buf_type;

extern txc_log_mux_buf_type txc_log_mux_buf; /* Log buffer for accumulated Markov frame log */

#define TXC_SEND_MUX1_LOG()                                     \
  do {                                                          \
    (void) log_submit(&txc_log_mux_buf);                        \
                                                                \
    /* Re-initialize counters */                                \
    txc_log_mux_buf.count = 0;                                  \
    txc_log_mux_buf.entry.hdr.len = FSIZ(mux_log_r_mux1_type, hdr); \
  } while (0)

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION TXC_PRI

DESCRIPTION
  This function provides the primary traffic to the Multiplex sublayer
  dependant on what the current primary service option is set to.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_pri (void);

/*===========================================================================

FUNCTION TXC_BUILD_FRAME

DESCRIPTION
  This function builds the next frame to be transmitted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_build_frame
(
  txc_sig_frame_type  signext
);

/*===========================================================================

FUNCTION BUILD_BLANK_FRAME

DESCRIPTION
  This function builds a blank-and-burst signaling frame.  Used both for FCH and
  DCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
byte build_blank_frame ( txc_dsch_type dsch );

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION TXC_SEC

DESCRIPTION
  This function provides the secondary traffic to the Multiplex sublayer
  dependant on what the current secondary service option is set to.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_sec (void);

/*===========================================================================

FUNCTION TXC_TDSO_SEC_FRAME_SIZE

DESCRIPTION
  This function determines the size of the TDSO data to be built
  when TDSO is assigned as Secondary traffic stream.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern word txc_tdso_sec_frame_size
(
  word    phy_channel,
  boolean msg_on_channel
);

/*===========================================================================

FUNCTION TXC_CHECK_FOR_RLP_SIGNALING

DESCRIPTION
  Check with RLP to find out if signaling needs to be sent.

  Note: this should only be done if no L3 signaling is present.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating if RLP signaling needs to be Txd.

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_check_for_rlp_signaling(void);

#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION TXC_SIG_FITS_IN_FRAME

DESCRIPTION
  Check if the remaining signaling payload will fit in the available
  space of the current voice frame.

DEPENDENCIES
  Intended for use during voice traffic when Dim and Burst is used.

RETURN VALUE
  Boolean indicating if the signalling payload will fit.
  True if yes, false otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_sig_fits_in_frame(cai_data_rate_type frame_rate);

#endif /*TXCMUX_H*/

