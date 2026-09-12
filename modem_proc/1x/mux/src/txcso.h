#ifndef TXCSO_H
#define TXCSO_H

/*===========================================================================

   C D M A   T R A N S M I T   S E R V I C E   O P T I O N S  M O D U L E

DESCRIPTION
  This header file contains definitions for the transmit service options
  module that are used by the txc unit.  This header file is internal to
  the txc unit.

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

$PVCSPath: O:/src/asw/COMMON/vcs/txcso.h_v   1.5   06 Jun 2002 18:15:42   hkulkarn  $
$Header: //components/rel/1x.mpss/8.0/mux/src/txcso.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/17/12   jtm     Revert REL_A feature clean up.
01/04/12   srk     Feature Cleanup.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/25/11   jtm     Added Mux Vocoder interface module.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/02/10   jtm     Added the capability to track the synchronization
                   of MVS and TXC and report errors.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/05/09   jtm     Lint fixes.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
03/26/09   jtm     Eliminated implicit includes.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code.
07/30/04   vlc     Added support for rel C TDSO.
06/05/02   hrk     Added prototype for txc_tdso_sec_traffic().
06/15/01   day     Mainlined FEATURE_IS2000
04/20/01   sr      Merged in from MSM5100 archive
11/13/00   bgc     (merge) Separated FCH and SCH TDSO blocks.
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

//cp
#include "cai.h"
#include "tdso_v.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
extern tdso_frame_block_type fch_tdso_frame_block;
extern tdso_frame_block_type sch_tdso_frame_block;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TX_LOOPBACK_TRAFFIC

DESCRIPTION
  Get loopback data bits (primary only) from the Loopback Service Option
  layer.

DEPENDENCIES
  The Loopback Service Option layer should be initialized already.

RETURN VALUE
  Returns the address of the loopback data bits in a pointer passed to the
  procedure.

  Returns the rate of the loopback data.

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_loopback_traffic
(
  byte **data_ptr,
    /* Returns pointer to loopback data bits */
  txc_sig_frame_type signext
    /* Signaling mode for this frame */
);

/*===========================================================================

FUNCTION TXC_MAR_TRAFFIC

DESCRIPTION
  Get traffic data (primary or secondary) from Markov frame error rate tests.

DEPENDENCIES
  The tx.frame_cnt has to be right.

RETURN VALUE
  Returns the address of the FER data in a variable passed to the
  procedure.

  Returns the rate of the FER data.

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_mar_traffic
(
  byte **data_ptr
    /* Returns pointer to data */
);

/*===========================================================================

FUNCTION TXC_TDSO_TRAFFIC

DESCRIPTION
  Get traffic data (primary) from TDSO

DEPENDENCIES

RETURN VALUE
  Returns the data rate

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_tdso_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  txc_sig_frame_type signext
    /* Signaling mode for next packet */
);

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION TXC_TDSO_SEC_TRAFFIC

DESCRIPTION
  Get traffic data (Secondary) from TDSO

DEPENDENCIES
  Currently, TDSO is supported on either FCH or DCCH phyical channels.

RETURN VALUE
  Returns the data rate

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_tdso_sec_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  boolean msg_on_channel
    /* Signaling mode for next frame */
);
#endif /* FEATURE_IS2000_REL_A_SVD */

#endif /*TXCSO_H*/
