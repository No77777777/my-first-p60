#ifndef DLWPLTDRV_H
#define DLWPLTDRV_H
/*===========================================================================
                          D L W P L T D R V . H

DESCRIPTION
This file contains typedefs, constants and prototypes for functions that
make up the low level demod routines for WPLT.

    Copyright (c) 1999, 2000, 2001
                    by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/dlwpltdrv.h_v   1.1   19 Mar 2001 13:50:34   mlevin  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/dlwpltdrv.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/10/14    pr      MCAL DL Changes for WplusW
05/31/2005  sk      Lint Fixes
03/19/2001  mdl     added prototypes

===========================================================================*/


typedef enum {
  DL_WPLT_STATUS_PASS,
  DL_WPLT_STATUS_INVALID_CMD,
  DL_WPLT_STATUS_BAD_PARMS,
  DL_WPLT_STATUS_BAD_STATE,
  DL_WPLT_STATUS_FW_ERROR,
  DL_WPLT_STATUS_GET_AWAY_FROM_ME_BOY_YA_BOTHER_ME
} dl_wplt_status_enum_type;

#define DL_WPLT_IS_SUCCESS(status) (status == DL_WPLT_STATUS_PASS)


/* masks for determining which parameters to update in
   dl_set_cmn_fing_parm */
#define WPLT_CMN_FING_PARM_MASK_TTK1            0x01
#define WPLT_CMN_FING_PARM_MASK_TTK2            0x02
#define WPLT_CMN_FING_PARM_MASK_LOCK_HI         0x04
#define WPLT_CMN_FING_PARM_MASK_LOCK_LO         0x08
#define WPLT_CMN_FING_PARM_MASK_PCB_COMB_THESH  0x10
#define WPLT_CMN_FING_PARM_MASK_RSSI_GAIN       0x20
#define WPLT_CMN_FING_PARM_MASK_PILOT_GAIN      0x40
#define WPLT_CMN_FING_PARM_MASK_WNTIO_GAIN      0x80


/******************************************************************************
*******************************************************************************
                          FUNCTION PROTOTYPES
*******************************************************************************
******************************************************************************/

/*===========================================================================
FUNCTION        DL_WPLT_LOG_FING_STATUS

FILE            dlwpltdrv.c

DESCRIPTION     This function performs a status dump and logs the finger
                status

DEPENDENCIES    None

RETURN VALUE    If the status dump is successful (and it is a bad error if
                it is not) DL_WPLT_STATUS_PASS, else DL_WPLT_STATUS_FW_ERROR

SIDE EFFECTS
===========================================================================*/
dl_wplt_status_enum_type dl_wplt_log_fing_status(void);

#endif
