#ifndef CDMA2KDIAGI_H
#define CDMA2KDIAGI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C D M A   2 K   D I A G   I N T E R N A L   H E A D E R

GENERAL DESCRIPTION
  This module contains declarations pertaining to the cdma2k diag interface.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2009 - 2020 Qualcomm Technologies, Inc. 
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

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/20   dmd     Reading EFS(74201) during boot-up time only
08/11/20   dmd     Changes for software dipswitches cleanup, new NV ID-74201
03/15/10   vks     Remove Genesis revisit feature as DAL support is now
                   available on MDM9K.
08/03/09   adw     Initial revision.

===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Other */




/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Constants
-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/

/* Dipswitch mapping */
typedef enum
{
  /* Dipswitch 0 = Enable IF loopback (0x0001) */              
  CDMA2KDIAG_SW_ENABLE_IF_LOOPBACK  ,

  /* Dipswitch 1 = Reload reverse link long code each 80-ms frame (0x0002)*/
  CDMA2KDIAG_SW_RELOAD_LONG_CODE  ,

  /* Dipswitch 2 = Disable forward link scrambling (0x0004)*/
  CDMA2KDIAG_SW_DISABLE_FL_SCRAMBLE    ,

  /* Dipswitch 3 = Disable forward link puncturing (0x0008)*/
  CDMA2KDIAG_SW_DISABLE_FL_PUNCTURE  ,

  /* Dipswitch 4 = Disable reverse link power control (0x0010)*/
  CDMA2KDIAG_SW_DISABLE_RL_PWR_CTRL  ,

  /* Dipswitch 5 = Enable Tx power on during acquisition and sync channel (0x0020) */
  CDMA2KDIAG_SW_ENABLE_TX_PWR_ACQ_SYNC  ,

  /* Dipswitch 6 = Enable sounder mode (0x0040)*/
  CDMA2KDIAG_SW_ENABLE_SOUNDER_MODE  ,

  /* Dipswitch 7 = Force reverse link voice to full rate (0x0080)*/
  CDMA2KDIAG_SW_FORCE_RL_FULL_RATE  ,

  /* Dipswitch 8 = Disable sleep mode operation (0x0100)*/
  CDMA2KDIAG_SW_DISABLE_SLEEP  ,

  /* Dipswitch 9 = Supress vocoder and audio filters (0x0200) */
  CDMA2KDIAG_SW_DISABLE_VOCODER  ,

  /* Dipswitch 10 = Disable reverse link max power protection (0x0400)*/
  CDMA2KDIAG_SW_DISABLE_RL_MAX_PWR_PROT  ,

  /* Dipswitch 11 = Disable other frequency searching (0x0800)*/
  CDMA2KDIAG_SW_DISABLE_OFREQ_SRCH ,
  /* Dipswitch 12 = Disable quick paging channel (0x1000)*/
  CDMA2KDIAG_SW_DISABLE_QPCH ,

  /* Dipswitch 13 = Disable quick paging channel estimator (0x2000)*/
  CDMA2KDIAG_SW_DISABLE_CH_EST  ,

  /* Dipswitch 14 = Disable quick paging channel offline timeline (0x4000)*/
  CDMA2KDIAG_SW_DISABLE_QPCH_OFFTL  ,

  /* Dipswitch 15 = Enable acquisition testing (0x8000)*/
  CDMA2KDIAG_SW_ENABLE_ACQ_TEST       
}
cdma2kdiag_dip_sw_type;



/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION       CDMA2KDIAG_SWITCH_INIT

DESCRIPTION    This function initializes the global cdma2kdiag switches 
               DAL handle and must be called before any other switch
               function.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Initializes the 1x handle into the switches DAL.

===========================================================================*/
void cdma2kdiag_switch_init( void );

/*===========================================================================

FUNCTION       CDMA2KDIAG_GET_DEFAULT_DIPSWITCH_STATE

DESCRIPTION    This function sets   default value for   cdma2kdiag dipswitches
             
DEPENDENCIES   None.

RETURN VALUE   Returns default state of  dipswitches.

SIDE EFFECTS None.

===========================================================================*/
uint32 cdma2kdiag_get_default_dipswitch_state( void );

/*===========================================================================


FUNCTION       SRCH_CDMA2KDIAG_DIPSWITCH_READ_NV_ITEM

DESCRIPTION    This function returns the state of a dipswitch stored in EFS
               and stores in dip_sw_state.

DEPENDENCIES None.

RETURN VALUE None

SIDE EFFECTS   None.

===========================================================================*/
void srch_cdma2kdiag_dipswitch_read_nv_item( void );

/*===========================================================================

FUNCTION       CDMA2KDIAG_GET_SWITCH

DESCRIPTION    This function returns the state of a dipswitch stored in EFS.

DEPENDENCIES The srch_cdma2kdiag_dipswitch_read_nv_item() routine must be 
             called prior to invoking this function.

RETURN VALUE   Returns the configured Dipswitch values

               TRUE - the dipswitch is set.

SIDE EFFECTS   None.

===========================================================================*/
boolean cdma2kdiag_get_switch
(
  /* Dip switch to check */
  cdma2kdiag_dip_sw_type dip_sw
);

/*===========================================================================

FUNCTION       CDMA2KDIAG_SET_SWITCH

DESCRIPTION    This function sets the state of a dipswitch in EFS.

DEPENDENCIES The srch_cdma2kdiag_dipswitch_read_nv_item() routine must be 
             called prior to invoking this function.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void cdma2kdiag_set_switch
(
  /* Dip switch to set */
  cdma2kdiag_dip_sw_type dip_sw
);

/*===========================================================================

FUNCTION       CDMA2KDIAG_CLEAR_SWITCH

DESCRIPTION    This function clears the state of a dipswitch.

DEPENDENCIES   The srch_cdma2kdiag_dipswitch_read_nv_item() routine must be 
               called prior to invoking this function.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void cdma2kdiag_clear_switch
(
  /* Dip switch to set */
  cdma2kdiag_dip_sw_type dip_sw
);

#endif /* CDMA2KDIAGI_H */
