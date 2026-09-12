/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              H D R    S R C H    M A C    I N T E R F A C E

                           Search MAC Module

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
  
  Copyright (c) 2005 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmac.c#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
10/14/2014   wsh     Changes to support Jolokia bringup 
09/22/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
05/08/2014   wsh     Fixed concurrent RF/ASDiv call that led to RF issue
06/26/2013   wsh     Added Selective TxD feature
01/18/2012   smd     Added RUMI Tx support.
12/09/2012   smd     Used AuxDRCCover to detect CSM6850 for DTX. 
11/18/2010   ljl     Protected aset access.  
10/11/2010   rmg     Increased resolution of reported Ec/Io from Q.1 to Q.2.
10/11/2010   rmg     Changed hdrsrchmac_get_mac_aset_info() to report max of
                     primary and diversity chain energy as pilot strength.
                     Primary chain energy was being reported earlier.
04/12/2010   kss     Fixed RAB reads by casting to int16.
02/16/2010   lyl     Removed ASSERT if no valid ASET info is found.
04/24/2009   lyl     Updated pilot strength with pri_chain_inst_eng;
02/05/2009   lyl     Used macro HDR_SRCH_MAX_PILOT_ENG in hdrsrchdrv.h
08/29/2008   lyl     Used primary chain inst /filtered energy for RMAC/AMAC
07/29/2008   kss     Initialize deltaT2P in hdrsrchmac_get_aset_info().
05/13/2008   rkc     Fixed compiler warnings.
05/12/2008   rmg     Added hdrsrchmac_get_demod_idx.
03/21/2008   rmg     Fixed slotQRAB update.
02/11/2008   jyw     Added hdrsrchmac_rmac3_get_mod_index.
02/08/2008   jyw     Modified the parameter in hdrsrchmac_get_mac_aset_info.
01/30/2008   jyw     Fixed the ref subaset index in the srch/mac interface.
12/17/2007   jyw     Output reference subaset in hdrsrchmac_get_mac_aset_info.
10/31/2007   jyw     Merged from Rev B branch.
02/07/2007   rmg     Added support for TxT2P adjust (DeltaT2P)
01/23/2007   rmg     Corrected bestASP returned by get_mac_aset_info()
11/16/2006   etv     Leave slot QRAB unloaded if ASP's fingers are out-of-lock.
12/12/2005   ljl     Added maximum of pilot energy
11/21/2005   ljl     Removed F3 messages
11/09/2005   ljl     Initialized mac_aset structure
10/13/2005   ljl     Added SlotFRAB
09/28/2005   ljl     Used instantaneous pilot strength
09/28/2005   ljl     Changed macro HDRASP_GET_RAB_SOFT to HDRASP_GET_QRAB_SOFT
06/15/2005   ljl     Used filtered pilot strength
05/31/2005   ljl     Updated QRAB and pilot strength
04/27/2005   ljl     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "hdrsrch.h"
#include "hdrsrchset.h"
#include "hdrsrchmac.h"
#include "hdrmdspmcr.h"
#include "hdrdebug.h"
#include "hdrsrchdrv.h"

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_REVA_L1

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/* EJECT */
/*======================================================================
 hdrsrchmac_ps2eng[] : This table is used for converting the pilot strength
 units to energy units.

 Description :
 Following program is used to generate the ps2eng_table[] array

    long Io = 20736; 

    void main()
    {
       double ecOverIo, ecOverIoLinear, ec, nt, ecBiased, cti;
       for(int i=0; i<64; i++)
       {
          ecOverIo = i;
          ecOverIoLinear = pow(10.0,ecOverIo/(-20.0));
          ec = Io*ecOverIoLinear;
          nt = Io-ec;
          ecBiased = ec+(nt/96.0);
           //bias is the same regardless of noncoh sums
          cti = ecBiased / (Io - ecBiased);
          cti *= 512;
          if( cti > 65535 ) cti = 65535;
          cout << (long)cti << endl;
       }
    }
*======================================================================*/

#define HDRSRCHMAC_PS2ENG_TABLE_SIZE 512

uint8  hdrsrchmac_ps2eng_table[ HDRSRCHMAC_PS2ENG_TABLE_SIZE ] =
{
   126,
   126,
   126,
   126,
   126,
   110,
    94,
    90,
    86,
    82,
    78,
    74,
    72,
    70,
    68,
    66,
    65,
    64,
    62,
    61,
    60,
    59,
    58,
    57,
    56,
    55,
    55,
    54,
    53,
    53,
    52,
    51,
    51,
    50,
    49,
    49,
    48,
    48,
    47,
    47,
    46,
    46,
    45,
    45,
    44,
    44,
    44,
    43,
    43,
    42,
    42,
    42,
    41,
    41,
    40,
    40,
    40,
    39,
    39,
    39,
    38,
    38,
    38,
    37,
    37,
    37,
    37,
    36,
    36,
    36,
    36,
    35,
    35,
    35,
    35,
    34,
    34,
    34,
    34,
    33,
    33,
    33,
    33,
    32,
    32,
    32,
    32,
    31,
    31,
    31,
    31,
    31,
    30,
    30,
    30,
    30,
    30,
    29,
    29,
    29,
    29,
    29,
    29,
    28,
    28,
    28,
    28,
    28,
    28,
    27,
    27,
    27,
    27,
    27,
    27,
    26,
    26,
    26,
    26,
    26,
    26,
    26,
    25,
    25,
    25,
    25,
    25,
    25,
    25,
    24,
    24,
    24,
    24,
    24,
    24,
    24,
    23,
    23,
    23,
    23,
    23,
    23,
    23,
    23,
    22,
    22,
    22,
    22,
    22,
    22,
    22,
    22,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    21,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    20,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    14,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     9,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     8,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     7,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     6,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     5,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     4,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     3,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     2,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     1,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0, 
     0, 
     0 

}; /*hdrsrchmac_ps2eng_table*/


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

  hdrsrchmac_active_sectors_struct_type  *mac_aset
    /* Pointer to the link info structure */
)
{
  uint8                      a;
    /* loop active set structure index */

  uint8                      subaset_idx = 0;
    /* active set structure index */
  
  uint8                      asp_idx;
    /* ASP index */

  hdrsrchmac_aset_info_type  *link_ptr;
    /* Pointer to aset info structure */

#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type     demod_idx = 0;
    /* The demod carrier idx associated with this subaset. */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( mac_aset == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Null pointer passed by MAC: mac_aset" );

    return;
  }

#ifdef FEATURE_HDR_REVB
  ASSERT( ( tca_idx >=0 )&&( tca_idx<HDRSRCH_MAX_NUM_SUBASETS ) );
#endif /* FEATURE_HDR_REVB */

  /* Init the mac structure. */
  for ( a = 0; a < HDRSRCH_ASET_MAX_SIZE; a++ )
  {
    mac_aset->active_sectors[a].pn_offset         = HDR_NO_PN_OFFSET;
      /* initialization */
    mac_aset->active_sectors[a].drc_lock          = 0;
    mac_aset->active_sectors[a].pilot_strength    = 0;
    mac_aset->active_sectors[a].slot_frab         = -1;
      /* Unloaded */
    mac_aset->active_sectors[a].slot_qrab         = -1;
      /* Unloaded */
    mac_aset->active_sectors[a].delta_t2p         = 0;

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
    mac_aset->active_sectors[a].aux_drc_cover = 0;
    /* AuxDRCCover initialization */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
  }

#ifdef FEATURE_HDR_REVB
  /* Find the subaset with the right tca_idx */
  for ( a=0; a<hdrsrchset.subaset_cnt; a++ )
  {
    if ( hdrsrchset.subaset[a].tca_idx == tca_idx )
    {
      subaset_idx = a;
      break;
    }
  }

  /* Make sure the tca index is found, otherwise, return from here */
  if ( a == hdrsrchset.subaset_cnt )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "ASET info currently invalid for mac; subaset_cnt:%d",
                    hdrsrchset.subaset_cnt );
    return;
  }

  /* Protect aset access */
  rex_enter_crit_sect(&hdrsrch.crit_sect);
    
  /* Fill the mac table */
  for ( a=0; a < hdrsrchset.subaset[subaset_idx].aset_cnt; a++ )
  {
    asp_idx             = hdrsrchset.subaset[subaset_idx].aset[a]->aset.asp_idx;
      /* aset.asp_idx is between 0 to 5 in revB */
    demod_idx           = hdrsrchset.subaset[subaset_idx].demod_idx;

    link_ptr            = &mac_aset->active_sectors[ asp_idx ];
    link_ptr->pn_offset      = hdrsrchset.subaset[subaset_idx].aset[a]->pn_offset;
    link_ptr->pilot_strength = MAX( 
      hdrsrchset.subaset[subaset_idx].aset[a]->inst_eng, 
      hdrsrchset.subaset[subaset_idx].aset[a]->pri_chain_inst_eng );
    link_ptr->delta_t2p      = hdrsrchset.subaset[subaset_idx].aset[a]->aset.delta_t2p;

    /* Limit pilot energy to 512 */
    if ( link_ptr->pilot_strength > HDR_SRCH_MAX_PILOT_ENG )
    {
      link_ptr->pilot_strength = HDR_SRCH_MAX_PILOT_ENG;
    }

    link_ptr->drc_lock = HDRASP_GET_DRC_LOCK( demod_idx, asp_idx );

    /* If the pilot is locked, set QRAB. If the fingers are out-of-lock,
       QRAB is set to -1 implicitly because it is initialized to -1 
       (Unloaded) */
    if (HDRASP_GET_LOCK( demod_idx, asp_idx ))
    {
      link_ptr->slot_qrab = 
      ( ((int16)HDRASP_GET_QRAB_SOFT( demod_idx, asp_idx )) > 0 )? 1 : -1;
    }

    link_ptr->slot_frab = (int16)HDRASP_GET_FRAB_SOFT( demod_idx, asp_idx );

#ifdef FEATURE_HDR_DEVICE_ONLY_DTX
    link_ptr->aux_drc_cover = hdrsrchset.subaset[subaset_idx].aset[a]->aset.aux_drc_cover;
    /* Get AuxDRCCover */
#endif /* FEATURE_HDR_DEVICE_ONLY_DTX */
  }

  mac_aset->best_sector_idx  = HDRASP_GET_BASP_FL( demod_idx );
    /* Get the best asp */

  mac_aset->reference_subaset = 
              ( subaset_idx == hdrsrchset_get_ref_subaset_idx() )? TRUE : FALSE;
    /* Get the reference subaset */

  rex_leave_crit_sect(&hdrsrch.crit_sect);

#else
  for ( a = 0; a < hdrsrchset.aset_cnt; a++ )
  {
    asp_idx                  = hdrsrchset.aset[a]->aset.asp_idx;
    link_ptr                 = &mac_aset->active_sectors[ asp_idx - 1 ];
      /* aset.asp_idx is between 1 to 6 */

    link_ptr->pn_offset      = hdrsrchset.aset[a]->pn_offset;
    link_ptr->pilot_strength = hdrsrchset.aset[a]->pri_chain_inst_eng;
    link_ptr->delta_t2p      = hdrsrchset.aset[a]->aset.delta_t2p;

    /* Limit pilot energy to 512 */
    if ( link_ptr->pilot_strength > HDR_SRCH_MAX_PILOT_ENG )
    {
      link_ptr->pilot_strength = HDR_SRCH_MAX_PILOT_ENG;
    }

    link_ptr->drc_lock = HDRASP_GET_DRC_LOCK( HDRMDSP_DEMOD_CARRIER_0, asp_idx );

    /* If the pilot is locked, set QRAB. If the fingers are out-of-lock,
       QRAB is set to -1 implicitly because it is initialized to -1 
       (Unloaded) */
    if (HDRASP_GET_LOCK( HDRMDSP_DEMOD_CARRIER_0, asp_idx ))
    {
      link_ptr->slot_qrab = 
      ( HDRASP_GET_QRAB_SOFT( HDRMDSP_DEMOD_CARRIER_0, asp_idx ) > 0 )? 1 : -1;
    }

    link_ptr->slot_frab = HDRASP_GET_FRAB_SOFT( HDRMDSP_DEMOD_CARRIER_0, asp_idx );
  } /* For each entry in the aset */

  mac_aset->best_sector_idx  = HDRASP_GET_BASP_FL() - 1;
    /* Get the best asp */
#endif /* !FEATURE_HDR_REVB */
}  /* hdrsrchmac_get_mac_aset_info */


/*==========================================================================

FUNCTION HDRSRCHMAC_GET_BAST_PRI_CHAIN_ENG

DESCRIPTION
  This function provides primay chain filtered pilot strength of best ASP.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Best ASP primay chain filtered energy

SIDE EFFECTS
  None

==========================================================================*/

uint16 hdrsrchmac_get_basp_pri_chain_eng
(
  int8                       sa_idx
    /* Subaset index */
)
{
  uint8                      best_asp_idx;
    /* Best ASP index */

  int                        i;
    /* Loop index */

  hdrsrch_sect_struct_type   *sect;
    /* Sector pointer */

  boolean                    found = FALSE;
    /* Flag - Pilot has been found in aset pilots list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  best_asp_idx  = HDRASP_GET_BASP_FL( hdrsrchset.subaset[sa_idx].demod_idx );
    /* Get the best asp index */

  for ( i = 0; i < hdrsrchset.subaset[sa_idx].aset_cnt; i++ )
  {
    sect = hdrsrchset.subaset[sa_idx].aset[i];

    /* Compare index from mdsp to active set indices */
    if ( sect->aset.asp_idx == best_asp_idx )
    {
      found = TRUE;
      break;
    }
  }         
  
  if ( found )
  {
    return ( hdrsrchset.subaset[sa_idx].aset[i]->pri_chain_filt_eng );
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "No best ASP found in ASET" );
    return ( hdrsrchset.subaset[sa_idx].aset[0]->pri_chain_filt_eng );
  }

}  /* hdrsrchmac_get_basp_pri_chain_eng */


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
  uint16   eng
    /* Energy to be converted to 0.25dB units */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  eng = MAX( eng, 1 );
  eng = MIN( eng, HDRSRCHMAC_PS2ENG_TABLE_SIZE );

  return -((int16)hdrsrchmac_ps2eng_table[eng-1]);

}  /* hdrsrchmac_eng_to_ecio */
  
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
  int   eng
    /* Energy in -0.5dB units */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchset_ecio_to_eng(eng);

}  /* hdrsrchmac_ecio_to_eng */


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

uint8 hdrsrchmac_rmac3_get_mod_index( )
{

  uint8 mod_idx = 0;
    /* Modulator index to be used in the reverse link */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  mod_idx = ( HDRSRCH_DEMOD_ASSIGN_OVERRIDE_ENABLE )?
                      HDRSRCH_DEMOD_ASSIGN_OVERRIDE_INDEX : HDRSRCH_DEMOD_IDX_0;

  return mod_idx;
}  /* boolean hdrsrchmac_get_mod_index */
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
)
{
  uint8                      a, subaset_idx=0;
    /* subaset index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
     HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "TX RUMI return demod_idx %d", tca_idx );
     return tca_idx;
#endif

  /* Find the subaset with the right tca_idx */
  for ( a=0; a<hdrsrchset.subaset_cnt; a++ )
  {
    if ( hdrsrchset.subaset[a].tca_idx == tca_idx )
    {
      subaset_idx = a;
      break;
    }
  }

  if (a != hdrsrchset.subaset_cnt)
  {
    /* TCA index is found - Return demod index */
    return(hdrsrchset.subaset[subaset_idx].demod_idx);
  }
  else
  {
    /* TCA index is not found - Complain! */
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TCA index not found!" );

    return 0;
  }

}  /* hdrsrchmac_get_demod_idx */

#endif /* FEATURE_HDR_REVA_L1 */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*=========================================================================

FUNCTION HDRSRCH_NEXT_ACC_SEQ

DESCRIPTION 
  This function notifies HDR SRCH that the current access seq has ended.
  sTxD uses this information to do BomBom algorithm

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

=========================================================================*/
void hdrsrchmac_next_acc_seq( void )
{
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_NEXT_ACC_SEQ );
}

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
boolean hdrsrchmac_txd_is_in_tx_mode( void )
{
  return hdrsrchrftxd_is_in_tx_mode( );
}

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
)
{
  hdrsrchrftxd_reg_tx_shutdown_cb(tx_shutdown_cb);
}

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
void hdrsrchmac_txd_rf_call_started( void )
{
  hdrsrchrftxd_rf_call_started( );
}

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
void hdrsrchmac_txd_rf_call_finished( void )
{
  hdrsrchrftxd_rf_call_finished( );
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

