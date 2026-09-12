#include "customer.h"

#include "hdr_variation.h"
#ifdef FEATURE_HDR_REVA_L1


#ifndef HDRFMACUTIL_H
#define HDRFMACUTIL_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         H D R   F O R W A R D   T R A F F I C   C H A N N E L   M A C   
       
                          P R O T O C O L   U T I L
                                    
                   E X T E R N A L   H E A D E R   F I L E       


GENERAL DESCRIPTION
  This file contains the public definitions and interfaces of the utility 
  functions for Enhanced subtype of HDR Forward Traffic Channel Media Access 
  Control (MAC) protocol.


EXTERNALIZED FUNCTIONS
    
  hdrfmacutil_set_rev_a_channel_params
    Sets the Enhanced FMAC subtype channel parameters given in TCAM.
     
  hdrfmacutil_put_rev_a_pkt
    Process a received Rev-A packet.

  hdrfmacutil_process_attrib_update 
    Process attribute update (GAUP) indication. 

  hdrfmacutil_activate_enhanced_subtype
    Activate Enhanced FMAC subtype by writing the config params to mDSP 
    registers


INITIALIZATION AND SEQUENCING REQUIREMENTS
  The procotol must be activated before Forward Traffic Channel packets can 
  be processed.

  Copyright (c) 2005, 2006, 2007  by Qualcomm Technologies, Inc.  All Rights Reserved.
  Copyright (c) 2008  by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrfmacutil.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/09   etv     Process GAUP update for MC FMAC attributes.
05/22/09   etv     Added support for Q6 message interface.
02/12/08   etv     Added support for MC MU User statistics log packet.
01/08/08   etv     Added API hdrfmac_serving_sector_locked_across_sub_asets.
11/05/07   etv     Set Demod to RevB for MC subtype.
08/28/07   etv     Updated hdrfmacutil_set_rev_a_channel_params for Rev-B
07/28/07   etv     Changed hdrfmacutil_put_rev_a_pkt to put_rev_a_pkt_chain.
07/24/07   etv     Included header files to avoid compiler errors.
04/18/06   etv     Added support for DRC/DSCBoostLength & ChannelGainBoost.
11/21/05   etv     Added header tags for perforce to fill in
10/12/05   etv     Added support for MUP User statistics summary log packet.
09/06/05   etv     Created Module.

===========================================================================*/

/* EJECT */
/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "dsm.h"

#include "hdrscmfmac.h"
#include "hdrfmac.h"
#include "hdrfmaci.h"
#include "hdrerrno.h"

/* Enum for forward traffic channel MAC packet types */
typedef enum
{
  HDRFMACUTIL_SU_SIMPLEX_FMT_A = 0x01,  /* Single-user simplex packet with 
                                           format-A connection layer packet */
  HDRFMACUTIL_SU_MULTIPLEX = 0x02,      /* Single-user Multiplex packet */
  HDRFMACUTIL_SU_SIMPLEX_FMT_B = 0x03   /* Single-user simplex packet with 
                                           format-B connection layer packet */
} hdrfmacutil_rev_a_su_packet_type;

typedef enum
{
  HDRFMACUTIL_MULTI_USER = 0x00,        /* Multi-user packet */
} hdrfmacutil_rev_a_mu_packet_type;

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_INIT_GAUP_ATTRIB_LUT                                     EXTERNAL

DESCRIPTION
  Initialize LUT with pointers to all GAUPable attributes based on the current
  subtype
  
DEPENDENCIES
  None.
  
PARAMETERS
  subtype - Current Subtype of the FMAC protocol.
  hdrfmac_cfg - Pointer to FMAC config struct.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

extern void hdrfmacutil_init_gaup_attrib_lut 
( 
  uint16 subtype,  
  hdrscmfmac_config_struct_type *hdrfmac_cfg
);

/* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_POWERUP_INIT

DESCRIPTION
  This function initializes the protocol prior to use.  It should be called
  once upon system startup. 
  
DEPENDENCIES
  None.
  
PARAMETERS
  hdrfmac_cfg - Pointer to FMAC config struct.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacutil_powerup_init 
( 
  hdrscmfmac_config_struct_type *hdrfmac_cfg 
);


/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_SET_REV_A_CHANNEL_PARAMS                        EXTERNAL

DESCRIPTION
  This function sets the Enhanced FMAC subtype channel parameters such as 
  DSC channel gain, DSC length and DSC timing registers (dscHalfSlotCount 
  and dscHalfSlotTrigger)

DEPENDENCIES
  None.

PARAMETERS
  tca_params            - The FMAC channel parameter structure including 
                          params received over Traffic channel Assignment msg.
  hdrfmac_tcam_params   - Pointer to FMAC's instance of TCAM params.
  hdrfmac_frame_offset  - Frame offset.
  
RETURN VALUE

SIDE EFFECTS
  The Enhanced forward channel parameters are written to corresponding mDSP 
  registers.
  
===========================================================================*/
extern void hdrfmacutil_set_rev_a_channel_params
(
  hdrfw_msg_u *const fw_cmd,
    /* Firmware command message to program the TCAM parameters */
  hdrfmac_tcam_params_type *const tca_params
);


 /* EJECT */
/*===========================================================================

FUNCTION HDRFMAC_PUT_REV_A_PKT_CHAIN                             EXTERNAL 

DESCRIPTION
  If the protocol is active, this function processes the FTC packet. It 
  figures out the type of FMAC packet by decoding the MAC trailer and 
  forwards to appropriate local routines for processing based on their 
  packet type. The FMAC packet is discarded if the MAC trailer is invalid. 
  The packet is discarded if the protocol is inactive.
  
DEPENDENCIES
  None.

PARAMETERS
  ftc_pkt_ptr - pointer to DSM chain consisting of a single Forward traffic 
                channel packet.

ASSUMPTIONS
  The DSM item chain contains only one FTC Physical layer packet.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void hdrfmacutil_put_rev_a_pkt
( 
  dsm_item_type *ftc_pkt_ptr
);


 /* EJECT */
/*===========================================================================
FUNCTION HDRFMACUTIL_PROCESS_ATTRIB_UPDATE                                LOCAL

DESCRIPTION
  This function processes the GAUP Update indication by querying SCM to find
  the updated attributes and gets the values of updated attributes and takes
  appropriate action.
  
DEPENDENCIES
  None.

PARAMETERS
  hdrfmac_tcam_params - FMAC's instance of TCAM Parameters.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void hdrfmacutil_process_attrib_update
( 
  hdrfmac_tcam_params_type *const tcam_params
);


/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_ACTIVATE_ENHANCED_SUBTYPE

DESCRIPTION
  Activate Enhanced FMAC subtype by writing the configuration parameters to 
  mDSP registers.
  
DEPENDENCIES
  None.

PARAMETERS
  subtype - FTCMAC subtype.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void hdrfmacutil_activate_enhanced_subtype
( 
  hdrfw_msg_u *const fw_cmd,
    /* Firmware command message to program the TCAM parameters */
  uint16 subtype
);


/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_RECORD_VERSION_NUMS

DESCRIPTION
  The Configuration attribute version numbers are needed only for GAUPable
  attributes in order to query the SCM DB and find out the updated attribs. 
  This function is used to store the version number of GAUPable attributes
  into static memory.
  
DEPENDENCIES
  None.

PARAMETERS
  hdrfmac_ver_no - Array of version numbers one for each FMAC config attrib.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void hdrfmacutil_record_version_nums
( 
  uint8 *hdrfmac_ver_no
);

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_LOG_MU_USER_STATS_SUMM                        EXTERNAL

DESCRIPTION
  Logs and ships out Multi-user packet user statistics summary log packet.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacutil_log_mu_user_stats_summ(void);


/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_RESET_MU_USER_STATS_SUMM                       EXTERNAL

DESCRIPTION
  Resets the Multi-user packet User statistics to zero.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacutil_reset_mu_user_stats_summ(void);

/* EJECT */
#ifdef FEATURE_HDR_REVB

/*===========================================================================

FUNCTION HDRFMACUTIL_SERVING_SECTOR_LOCKED_ACROSS_SUB_ASETS         EXTERNAL

DESCRIPTION
  Can AT select a different serving sector on each of the Active sets?
  
DEPENDENCIES
  None.

PARAMETERS
  locked - Pointer to the boolean where the result is stored.
           Set to TRUE for locked Mode and FALSE otherwise. 

RETURN VALUE
  E_SUCCESS     - If the session is open and current FMAC subtype is 
                  Multi-carrier FTCMAC (Subtype 2).
  E_NOT_ALLOWED - Otherwise.

SIDE EFFECTS
  None.
  
===========================================================================*/
hdrerrno_enum_type hdrfmacutil_serving_sector_locked_across_sub_asets
( 
  boolean *locked
);

#endif /* FEATURE_HDR_REVB */

/* EJECT */
/*===========================================================================

FUNCTION HDRFMACUTIL_LOG_MC_MU_USER_STATS_SUMM                        EXTERNAL

DESCRIPTION
  Logs and ships out Multi-carrier Multi-user packet user statistics summary 
  log packet.
  
DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void hdrfmacutil_log_mc_mu_user_stats_summ( void );

#endif /* FEATURE_HDR_REVA_L1 */

#endif /* HDRFMACUTIL_H */
