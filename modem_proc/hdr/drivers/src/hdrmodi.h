#ifndef HDRMODI_H
#define HDRMODI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   H D R    M O D U L A T O R    D R I V E R
                    
                   I N T E R N A L    H E A D E R    F I L E

GENERAL DESCRIPTION
  This module contains internal register definitions and declarations for 
  using the HDR modulator driver.
  
  Copyright (c) 2000 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrmodi.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/14/14   wsh     Changes to support Jolokia bringup
11/08/13   mbs     Supported new long code state for BOLT
10/08/13   mbs     Added support for BOLT interface changes
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
03/19/12   ljl     Fixed Dime compiler errors.
02/01/11   rmg     Added PN_LONG_STATE_L/H.
03/23/10   kss     Updated register field name for Voyager 2.0
07/13/09   kss     Update for 9K.
05/17/09   rkc     Modified PA/TX enable and disable macros for Poseidon2.
05/28/08   kss     Force correct tx_punct bit to be used; update defines.
11/17/05   jyw     Removed the unused macro.
04/25/05   kss     Additional changes for Rev A support.
11/12/04   kss     Changes for MSM6800 Rev 0 support.
11/06/00   kss     Added frame rate counter.
10/23/00   kss     Added frame size definitions.
05/25/00   kss     Created.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"



/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/* Reverse rate frame sizes (bytes) */ 
#define REV_RATE_9600_SIZE        32
#define REV_RATE_19200_SIZE       64
#define REV_RATE_38400_SIZE      128
#define REV_RATE_76800_SIZE      256
#define REV_RATE_153600_SIZE     512 


/* ---- DEBUG INFO --- */
/* Documenting register documentation errors until they can be corrected 
   in register mapping files.

   In REVMOD_PA_CTL, three bits are defined incorrectly:
   Correct definitions (from HW folks) are:
     bit 5: TX_PUNC_CTL
     bit 7: PA_WARMUP_EN
     bit 8: TX_WARMUP_EN
*/

/* Masks and bit definitions for MOD_PA_CTL register. 
** Use with HDRMOD_PA_CTL_OUTM().
*/
#define HDRMOD_DRC_DISABLE_M      HWIO_REVMOD_PA_CTL_DRC_DISABLE_BMSK
#define HDRMOD_TX_PUNCT_M         0x0020 //HWIO_REVMOD_PA_CTL_TX_PUNC_CTL_BMSK
#define HDRMOD_PA_PUNCT_M         HWIO_REVMOD_PA_CTL_PA_PUNC_CTL_BMSK
#define HDRMOD_MOD_MASK_M         HWIO_REVMOD_PA_CTL_MOD_MASK_CTL_BMSK
#define HDRMOD_RRI_DISABLE_M      HWIO_REVMOD_PA_CTL_RRI_DISABLE_BMSK

#define HDRMOD_DRC_DISABLE_ON     HDRMOD_DRC_DISABLE_M
#define HDRMOD_TX_PUNCT_ON        HDRMOD_TX_PUNCT_M
#define HDRMOD_PA_PUNCT_ON        HDRMOD_PA_PUNCT_M
#define HDRMOD_MOD_MASK_ON        HDRMOD_MOD_MASK_M
#define HDRMOD_RRI_DISABLE_ON     HDRMOD_RRI_DISABLE_M

#define HDRMOD_DRC_DISABLE_OFF    0x0000
#define HDRMOD_TX_PUNCT_OFF       0x0000
#define HDRMOD_PA_PUNCT_OFF       0x0000
#define HDRMOD_MOD_MASK_OFF       0x0000
#define HDRMOD_PA_ON_OFF          0x0000
#define HDRMOD_RRI_DISABLE_OFF    0x0000


/* Masks and bit definitions for MOD_PN_CTL register.
** Use with HDRMOD_PN_CTL_OUTM().
*/
#define HDRMOD_PN_BOZO_M          HWIO_REVMOD_PN_CTL_BOZO_MODE_BMSK
#define HDRMOD_PN_BOZO_ON         0x0000
#define HDRMOD_PN_BOZO_OFF        HDRMOD_PN_BOZO_M


/* HDRMOD_PN_IQ_LOAD() should be used to strobe the PN load bit */
#define HDRMOD_PN_IQ_LOAD_STROBE                                            \
                          ( HWIO_REVMOD_PN_CTL_PN_LONG_CODE_LOAD_BMSK   \
                          | HWIO_REVMOD_PN_CTL_PN_I_LONG_CODE_LOAD_BMSK \
                          | HWIO_REVMOD_PN_CTL_PN_Q_LONG_CODE_LOAD_BMSK )

/* HDRMOD_PN_IQ_LOAD:  Strobe PN loading bit in modulator PN control to
** load programmed PN mask into the modulator (the bit is not stored in 
** the shadow register). 
*/
#define HDRMOD_PN_IQ_LOAD()                                                  \
  HWIO_REVMOD_PN_CTL_OUT((HDRMOD_PN_IQ_LOAD_STROBE))


#define MOD_MODE_1X_V             0x0000

#ifdef T_MDM9X00A
#define MOD_MODE_HDR_V            HWIO_MOD_MODE_CDMA_MODE_BMSK
#else
#define MOD_MODE_HDR_V            HWIO_MOD_MODE_MOD_DO_EN_BMSK
#endif

#define MOD_MODE_HDR_REV0_V       0x0000
#define MOD_MODE_HDR_REVA_V       HWIO_MOD_MODE_DO_REV_BMSK

#define MOD_PA_ON_CTL_PA_RANGE_SEL_HDR_V   HWIO_MOD_PA_ON_CTL_PA_RANGE_SEL_BMSK
#define MOD_PA_ON_CTL_TX_PUNCT_SEL_HDR_V   HWIO_MOD_PA_ON_CTL_TX_PUNCT_SEL_BMSK
#define MOD_PA_ON_CTL_PA_PUNCT_SEL_HDR_V   HWIO_MOD_PA_ON_CTL_PA_PUNCT_SEL_BMSK

#if defined(FEATURE_HDR_BOLT_MODEM) && (!defined(FEATURE_HDR_UNIFIED_TX)) /* BOLT or THOR */  
#define PN_LONG_STATE_0_INIT      0x57
#define PN_LONG_STATE_1_INIT      0x2FF6
#define PN_LONG_STATE_2_INIT      0x2749
#else /* JOLOKIA or DIME */
#define PN_LONG_STATE_0_INIT      0x24B
#define PN_LONG_STATE_1_INIT      0x91bf
#define PN_LONG_STATE_2_INIT      0xd3a8
#endif /* FEATURE_HDR_BOLT_MODEM */

#define PN_LONG_STATE_L  (( PN_LONG_STATE_1_INIT << 16 ) | PN_LONG_STATE_2_INIT)
#define PN_LONG_STATE_H  PN_LONG_STATE_0_INIT

#if defined (T_POSEIDON2) || defined (T_MDM9X00) || defined FEATURE_HDR_DIME_MODEM
#define PA_ON_EN_V                HWIO_MODEM_PA_CTL_PA_ON_EN_BMSK
#define PA_ON_DIS_V               0

#define TX_ON_EN_V                HWIO_MODEM_TX_CTL_TX_ON_EN_BMSK
#define TX_ON_DIS_V               0
#else
#define PA_ON_EN_M                HWIO_MODEM_PA_ON_CTL_PA_ON_EN_BMSK
#define PA_ON_EN_V                PA_ON_EN_M
#define PA_ON_DIS_V               0x0000

#define TX_ON_EN_M                HWIO_MODEM_PA_ON_CTL_TX_ON_EN_BMSK
#define TX_ON_EN_V                TX_ON_EN_M
#define TX_ON_DIS_V               0x0000
#endif /* defined (T_POSEIDON2) || defined (T_MDM9X00) */

#endif /* HDRMODI_H */
