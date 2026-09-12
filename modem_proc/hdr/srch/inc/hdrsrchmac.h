#ifndef HDRSRCHMAC_H
#define HDRSRCHMAC_H

#include "hdr_variation.h"
#ifdef FEATURE_HDR_REVA_L1
/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           H D R    S R C H		M A C

                          Search MAC Header File

GENERAL DESCRIPTION
  This file provides all external interface functions to MAC for the HDR 
  Searcher.

EXTERNALIZED FUNCTIONS
  hdrsrchmac_get_mac_aset_info
  hdrsrchmac_eng_to_ecio
  hdrsrchmac_ecio_to_eng
  hdrsrchmac_get_demod_idx
  
REGIONAL FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
  Copyright (c) 2005 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
  Copyright (c) 2006 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
  Copyright (c) 2007 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
  Copyright (c) 2008 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/inc/hdrsrchmac.h#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
10/17/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
08/12/2014   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold 
05/08/2014   wsh     Fixed concurrent RF/ASDiv call that led to RF issue
06/26/2013   wsh     Added Selective TxD feature
12/09/2012   smd     Used AuxDRCCover to detect CSM6850 for DTX. 
10/11/2010   rmg     Increased resolution of reported Ec/Io from Q.1 to Q.2. 
08/22/2008   lyl     Provided best ASP filtered primary chain energy.
05/12/2008   rmg     Added hdrsrchmac_get_demod_idx.
02/11/2008   jyw     Added hdrsrchmac_rmac3_get_mod_index.
02/08/2008   jyw     Modified the parameter in hdrsrchmac_get_mac_aset_info.
09/06/2007   jyw     Updated with revB MAC/searcher interfaces.
02/07/2007   rmg     Added support for TxT2P adjust (DeltaT2P)
10/13/2005   ljl     Added SlotFRAB
09/28/2005   ljl     Changed slot_rab to slot_qrab
04/27/2005   ljl     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdrsrchtypes.h"
#include "hdrsrchdef.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

typedef struct
{
  uint16                     pn_offset;
    /* PN offset of sector. HDR_NO_PN_OFFSET if invalid. */
    
  int8                       delta_t2p;
    /* TxT2P adjust value in 1/4 dB */
    
  uint16                     pilot_strength;
    /* Linear pilot strength for sector, scaled to 512 (0 dB = 512) */

  uint8                      drc_lock;
    /* DRC Lock bit for sector. 1 if in lock, 0 of not. */

  int8                       slot_qrab;
    /* Hard-limited slot_qrab for sector. (-1/1) */

  int16                     slot_frab;
    /* SlotFRAB value from MDSP */

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
  uint8                      aux_drc_cover;
    /* AuxDRCCover */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
}
hdrsrchmac_aset_info_type;

typedef struct
{
  hdrsrchmac_aset_info_type  active_sectors[ HDRSRCH_ASET_MAX_SIZE ];
    /* Info for each sector in ASET */

  uint8                      best_sector_idx;
    /* Best sector index in the active_sects[] array. */

#ifdef FEATURE_HDR_REVB
  boolean                    reference_subaset;
    /* Set if it is the reference subactive set, i.e. the 1st subactive
       set in the TCA subactive set list */
#endif /* FEATURE_HDR_REVB */
} hdrsrchmac_active_sectors_struct_type;

#ifdef FEATURE_HDR_SELECTIVE_TXD
typedef void (*hdrsrchmac_txd_tx_shutdown_cb_type)(void);
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/* EJECT */
/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/



/*==========================================================================

FUNCTION HDRSRCHMAC_GET_MAC_ASET_INFO

DESCRIPTION
  This function provides pilot strength, QRABn,s and DRCLock per sector.

DEPENDENCIES
  None

PARAMETERS
  link_info

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchmac_get_mac_aset_info
( 
#ifdef FEATURE_HDR_REVB
  int8                                   tca_idx, 
    /* The subactive set index under request */
#endif /* FEATURE_HDR_REVB */

  hdrsrchmac_active_sectors_struct_type *mac_aset
	/* Pointer to the mac aset info structure */
);

/*==========================================================================

FUNCTION HDRSRCHMAC_GET_BAST_PRI_CHAIN_ENG

DESCRIPTION
  This function provides primay chain filtered pilot strength of best ASP.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Best ASP filtered primay chain energy

SIDE EFFECTS
  None

==========================================================================*/

uint16 hdrsrchmac_get_basp_pri_chain_eng
(
  int8                       sa_idx
    /* Subaset index */
);

/*==========================================================================

FUNCTION HDRSRCHMAC_ENG_TO_ECIO

DESCRIPTION
  This function converts an energy value into Ec/Io in units of 0.25dB.
  Range for dB conversion is 0 (-27dB)  to 511 (0dB)

DEPENDENCIES
  None

PARAMETERS
  eng

RETURN VALUE
  Ec/Io in 0.25dB units.

SIDE EFFECTS
  None

==========================================================================*/

int hdrsrchmac_eng_to_ecio
(   
  uint16	eng
	/* Energy to be converted to 0.25dB units */
);

/*==========================================================================

FUNCTION HDRSRCHMAC_ECIO_TO_ENG

DESCRIPTION
  This function converts an energy value in Ec/Io (-0.5dB) to a linear scale
  value.

  Range for linear covnersion is 0 (-27dB)  to 511 (0dB)

DEPENDENCIES
  None

PARAMETERS
  eng in -0.5dB

RETURN VALUE
  Linear scale value. (0-511)

SIDE EFFECTS
  None

==========================================================================*/

uint16 hdrsrchmac_ecio_to_eng
(   
  int  eng
    /* Energy in -0.5dB units */
); 


#ifdef FEATURE_HDR_REVB
/*==========================================================================

FUNCTION HDRSRCHMAC_RMAC3_GET_MOD_INDEX

DESCRIPTION
  The function is used to pass the demod/mod override mode information from
  searcher to rmac3. The demod/mod override mode can be used to make the revA 
  traffic go through any of the demod/mod chain. The function is used for both
  test mode and normal operational mode.

DEPENDENCIES
  The function is expected to use only with RMAC3. This is because with the revB
  (RMAC4), the demod/mod information is passed from LMAC to RMAC4. However, such 
  a route does not exist for a revA session (RMAC3).

PARAMETERS
  None

RETURN VALUE
  The modulator index or reverse link carrier index to be used by RMAC3.

SIDE EFFECTS
  None

==========================================================================*/

uint8 hdrsrchmac_rmac3_get_mod_index( void );
#endif /* FEATURE_HDR_REVB */

/*==========================================================================

FUNCTION HDRSRCHMAC_GET_DEMOD_IDX

DESCRIPTION
  The function is used to obtain the demod index corresponding to the TCA
  subactive set index.

DEPENDENCIES
  None.

PARAMETERS
  tca_idx - Subactive set index in TCA

RETURN VALUE
  The demod index corresponding to the tca_idx.

SIDE EFFECTS
  None

==========================================================================*/

hdrsrch_demod_idx_type hdrsrchmac_get_demod_idx
(
  uint8 tca_idx
    /* The subactive set index under request */
);

#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*=========================================================================

FUNCTION HDRSRCH_NEXT_ACC_SEQ

DESCRIPTION 
  This function notifies HDR SRCH that access seq has ended. sTxD
  uses this information to do BomBom algorithm

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

=========================================================================*/
void hdrsrchmac_next_acc_seq( void );

/*=========================================================================

FUNCTION HDRSRCH_TXD_IS_IN_TX_MODE

DESCRIPTION 
  This checks if TxD is in one of the TX mode. Per requirement, antenna
  switch must be reset to proper position when exiting TX. Caller need
  to ensure TRM lock is not released before the switching is complete.
 
DEPENDENCIES
  None

RETURN VALUE
  True - if TxD is in the process of transitioning out of RXTX mode
  False - Otherwise

SIDE EFFECTS
  None

=========================================================================*/
boolean hdrsrchmac_txd_is_in_tx_mode( void );

/*=========================================================================

FUNCTION HDRSRCH_TXD_REG_TX_SHUTDOWN_CB

DESCRIPTION 
  Wait for TRM to grant mode out of TX
 
DEPENDENCIES
  Must be called from HDRSRCH task

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchmac_txd_reg_tx_shutdown_cb
(
  hdrsrchmac_txd_tx_shutdown_cb_type tx_shutdown_cb
);

/*=========================================================================

FUNCTION HDRSRCHMAC_TXD_RF_CALL_STARTED

DESCRIPTION 
  Informs ASD that a conflicting RF call has started. This function
  requests (and may block waiting) for ASDiv critical section
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchmac_txd_rf_call_started( void );

/*=========================================================================

FUNCTION HDRSRCHMAC_TXD_RF_CALL_FINISHED

DESCRIPTION 
  Informs ASD that a conflicting RF call has finished. This function
  releases ASDiv critical section
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchmac_txd_rf_call_finished( void );

#endif /* FEATURE_HDR_SELECTIVE_TXD */

#endif /* HDRSRCHMAC_H */


