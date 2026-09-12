#ifndef HDRSRCHDEF_H
#define HDRSRCHDEF_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        H D R    S R C H   D E F I N E S

                          Search Defines Header File

GENERAL DESCRIPTION

   This file contains the defines used in HDR Search operations.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000, 2001, 2002, 2003, 2004
                2005, 2006, 2007, 2008 
                by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/inc/hdrsrchdef.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
03/29/2012   smd     Added HDR_PN_CIRC_WRAPUP_X8.
12/14/2011   wsh     Added interband OFS/HO threshold 
08/19/2008   smd     Added idle handoff improvement.
06/07/2006   ljl     Added SBHO optimization.
10/24/2005   jyw     Fixed the lint error.
08/18/2005   ljl     IHO optimization
04/05/2004   aaj     Minor comments corrections
03/04/2004   aaj     Added support for subnet handoff thresholds
03/11/2003   ajn     Updated Copyright notice
07/24/2002   aaj     Changed RAHO threshold to 2dB from 0.5dB
12/06/2001   aaj     Added soft constants for HDRSRCH handoffs
11/22/2000   ajn     Code review changes
10/23/2000   ajn     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/


/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/



/*-------------------------------------------------------------------------
                       H A R D    C O N S T A N T S
-------------------------------------------------------------------------*/


/*---------------------------*
 * Pilot PN Offset defines   *
 *---------------------------*/

#define HDR_PN_OFFSET_INCR             4         /* PN offset increment */
#define HDR_PILOT_INCR                64         /* PN offset increment */
#define HDR_MAX_PN_OFFSET             512        /* MAx PN offset */

#define HDR_NO_PN_OFFSET           0xffff        /* undefined pn offset */

/*---------------------------*
 * PN Circle related defines *
 *---------------------------*/

#define HDR_PN_CIRC_X1         (32768UL)          /* max PN count in chipx1 */
#define HDR_PN_CIRC_X2         (2*HDR_PN_CIRC_X1) /* max PN count in chipx2 */
#define HDR_PN_CIRC_X8         (8*HDR_PN_CIRC_X1) /* max PN count in chipx8 */

#define HDR_HALF_PN_CIRC_X8    (HDR_PN_CIRC_X8/2) /* half PN circ in x8 */
#define HDR_HALF_PN_CIRC_X2    (HDR_PN_CIRC_X2/2) /* half PN circ in x2 */

#define HDR_PN_CIRC_MASK_X8    0x3FFFF            /* mask for pn circ count */
#define HDR_PN_CIRC_MASK_X2    0xFFFF             /* mask for pn circ count */

#define HDR_PN_CIRC_WRAPUP_X8  0x40000

/*---------------------------*
 * Other defines             *
 *---------------------------*/

#define HDRSRCHSET_NO_COLOR_CODE   0xffff        /* unknown color code */


/*-------------------------------------------------------------------------
                       S O F T    C O N S T A N T S
-------------------------------------------------------------------------*/

/* HDR Srch idle state handoff hysterisis defines in 0.5 dB units. To avoid
   ping-pong handoff events, the new pilot has to be at least these many 
   dBs higher than the current pilot to be eligible for idle handoff */

#define  HDRSRCHIDLE_ACCESS_HO_THRESH_HALF_DB         6  /* 3 dB */ 
#define  HDRSRCHIDLE_MONITOR_HO_THRESH_HALF_DB        4  /* 2 dB */

#define  HDRSRCHIDLE_HO_THRESH_CACHED_LOW_HALF_DB        4  /* 2 dB */
#define  HDRSRCHIDLE_HO_THRESH_CACHED_HIGH_HALF_DB       6  /* 3 dB */
#define  HDRSRCHIDLE_HO_THRESH_NOTCACHED_LOW_HALF_DB     4  /* 2 dB */
#define  HDRSRCHIDLE_HO_THRESH_NOTCACHED_HIGH_HALF_DB    8  /* 4 dB */

#define  HDRSRCHIDLE_OFS_HO_THRESH_HALF_DB            4  /* 2 dB */
#define  HDRSRCHIDLE_IB_OFS_HO_THRESH_HALF_DB         14 /* 7 dB */

#define  HDRSRCHIDLE_REACQ_HO_THRESH_HALF_DB          4  /* 2 dB */

/* Added threshold for handoff that is planned to occur at the subnet boundary
   This threshold is added on top of the pre-decided threshold if it is found 
   that the pilots are at different subnets */

#define  HDRSRCHIDLE_SBHO_ADDON_HI_HALF_DB            4  /* 2.0 dB */
#define  HDRSRCHIDLE_SBHO_ADDON_LO_HALF_DB            2  /* 1.0 dB */

/* Threshold for SBHO handoff */

#define  HDRSRCHIDLE_SBHO_HI_HALF_DB                  10  /* 5.0 dB */
#define  HDRSRCHIDLE_SBHO_LO_HALF_DB                  6   /* 3.0 dB */

/* This is the threshold used to determine if the pilot is strong or weak. The
   intent is to use higher hysterisis threshold if pilot is strong enough */
   
#define  HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH         102  /* ~7.0 dB */
#define  HDRSRCHIDLE_SBHO_STRONG_PILOT_ENG_THRESH    69   /* ~9.0 dB */

/*==========================================================================

                     MACROS USED IN THE MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/



#endif /* HDRSRCHDEF_H */

