/*===========================================================================
               D O W N L I N K   M A C    L O G G I N G

DESCRIPTION
  This file provides the functions for MAC downlink logging.

EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdllog.c_v   1.2   29 Aug 2001 10:49:20   rchar  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacdllog.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* --------------------------------------------------------------------------
** Include Files
** ------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsmacdllog.h"
#include "msg.h"
#include <string.h>

#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
/*===========================================================================

FUNCTION MAC_DL_SEND_CFG_LOG_PACKET

DESCRIPTION
   This function prepares a log packet for configured Downlink MAC Transport
   Channels and sends the packet to log services.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_dl_send_cfg_log_packet
(
  tdsmac_dl_config_type *cfgptr        /* Ptr to Downlink MAC config          */
)
{
  TDSCDMA_DL_MAC_CFG_LOG_PACKET_type *dl_cfg_ptr; /* Ptr to DL MAC config log*/
                                                /* Packet                  */
  tdsmac_dl_bch_cctrch_config_type *bch_info;      /* Ptr to BCH info         */
  tdsmac_dl_pch_cctrch_config_type *pch_info;      /* Ptr to PCH info         */
  tdsmac_dl_fach_trch_config_type  *fach_info;     /* Ptr to FACH info        */
  tdsmac_dl_dch_trch_config_type   *dch_info;      /* Ptr to DCH info         */
  tdsmac_dl_ded_logch_config_type  *dlc_ptr;       /* Ptr to Logical ch. info */
  uint8                         *dl_log_ptr = NULL;
                                                /* Temp Ptr to Log packet  */
  uint8                         tdsnum_trch = 0;   /* Num of Transport Channel*/
  uint8                         active_fach = 0;/* Num of Active FACHs     */
  uint8                         num_logchan = 0;/* Num of Logical Channels */
  uint16                                              i,k;
 uint8 no_Logical_fach=0;
  /*-------------------------------------------------------------------------
   Calculate the Number of Transport Channels.
  -------------------------------------------------------------------------*/
  tdsnum_trch = (cfgptr->num_bchs + cfgptr->num_pchs + cfgptr->num_fachs +
             cfgptr->num_dchs + cfgptr->num_dschs);

  /*-------------------------------------------------------------------------
    Calculate the Number of Logical Channels for each transport channel.
  -------------------------------------------------------------------------*/

  /* Number of Logical Channels mapped to BCH */
  for (i = 0; (i < cfgptr->num_bchs) && (i< TDSMAC_DL_MAX_CCTRCH);i++ )
  {
    num_logchan++;
  }
  /* Number of Logical Channels mapped to PCH */
  for (i = 0; (i < cfgptr->num_pchs) && (i< TDSMAC_DL_MAX_CCTRCH);i++ )
  {
    num_logchan++;
  }
  /* Number of Logical Channels mapped to FACH */
  for (i = 0; (i < cfgptr->num_fachs) && (i<TDSUE_MAX_TRCH);i++ )
  {
        fach_info = cfgptr->fach_info[i];

    if (fach_info->bcch_enable == TRUE)
    {
      active_fach++;
    }
    if (fach_info->ccch_enable == TRUE)
    {
      active_fach++;
    }
    if (fach_info->ctch_enable == TRUE)
    {
      active_fach++;
    }
    active_fach += fach_info->ndlchan;
    num_logchan += active_fach;
  }
  /* Number of Logical Channels mapped to DCH */
  for (i = 0; (i < cfgptr->num_dchs) && (i < TDSUE_MAX_TRCH); i++)
  {
        dch_info = cfgptr->dch_info[i];
    num_logchan += dch_info->ndlchan;
  }

  /* Allocate memory here.*/
  dl_cfg_ptr = (TDSCDMA_DL_MAC_CFG_LOG_PACKET_type *)log_alloc(TDSCDMA_DL_MAC_CFG_LOG_PACKET,
                                TDSMAC_DL_CFG_PACKET_LEN(tdsnum_trch, num_logchan));

  if (dl_cfg_ptr != NULL)
  {
        /*dl_log_ptr = (uint8 *)dl_cfg_ptr;
        dl_log_ptr += FPOS(WCDMA_DL_MAC_CFG_LOG_PACKET_type,tdsnum_trch);

        *(dl_log_ptr++) = tdsnum_trch;
        *(dl_log_ptr++) = cfgptr->cipher_info[0].cipher_enable;
        *(dl_log_ptr++) = cfgptr->cipher_info[0].key_idx;
        *((uint32 *)dl_log_ptr)++ = cfgptr->cipher_info[0].hfn; */

    dl_cfg_ptr->version_num = TDSMAC_LOG_VERSION;
    dl_cfg_ptr->num_trch  = tdsnum_trch;
    // Store ciphering parameters for CS domain only
    dl_cfg_ptr->ciphering_flag = cfgptr->cipher_info[0].cipher_enable;
    dl_cfg_ptr->ciphering_key_id = cfgptr->cipher_info[0].key_idx;
    dl_cfg_ptr->hfn = cfgptr->cipher_info[0].hfn;

    dl_log_ptr = (uint8 *)dl_cfg_ptr;
        dl_log_ptr += FPOS(TDSCDMA_DL_MAC_CFG_LOG_PACKET_type,trch_info);

    /* Store BCH information.*/
    for (i = 0; (i < cfgptr->num_bchs) && (i< TDSMAC_DL_MAX_CCTRCH);i++)
    {
      bch_info = &cfgptr->bch_info[i];

      /* Store CCtrch ID instead of the Transport Channel ID */
      *(dl_log_ptr++) = bch_info->cctrch_id;

      /* Store the Transport Channel Type - BCH */
      *(dl_log_ptr++) = (uint8)UE_DL_TRCH_BCH;

      /* Store the Number of Logical Channels mapped to BCH - always 1*/
      *(dl_log_ptr++) = 1;

      /* Store the Logical Channel ID */
      *(dl_log_ptr++)  = bch_info->bcch_id;

      /* Store the Logical Channel Type - BCCH */
      *(dl_log_ptr++)  = (uint8)UE_LOGCHAN_BCCH;

      /* Store the Logical Channel Mode */
      *(dl_log_ptr++) = (uint8)UE_MODE_TRANSPARENT;

      /* Radio Bearer ID not applicable - Set it to 0*/
      *(dl_log_ptr++) = 0;

      /* MAC ID not applicable - Set it to 0  */
      *(dl_log_ptr++) = 0;

      /* MAC Priority not applicable - Set it to 0  */
      *(dl_log_ptr++) = 0;

    }

    /* Store PCH information.*/
    for (i = 0; (i < cfgptr->num_pchs) && (i< TDSMAC_DL_MAX_CCTRCH);i++)
    {
            pch_info = &cfgptr->pch_info[i];

      /* Store CCtrch ID instead of the Transport Channel ID */
      *(dl_log_ptr++) = pch_info->cctrch_id;

      /* Store the Transport Channel Type - PCH */
      *(dl_log_ptr++) = (uint8)UE_DL_TRCH_PCH;

      /* Store the Number of Logical Channels mapped to PCH - always 1*/
      *(dl_log_ptr++) = 1;

      /* Store the Logical Channel ID */
      *(dl_log_ptr++)  = (uint8)pch_info->pcch_id;

      /* Store the Logical Channel Type - PCCH */
      *(dl_log_ptr++)  = (uint8)UE_LOGCHAN_PCCH;

      /* Store the Logical Channel Mode */
      *(dl_log_ptr++) = (uint8)UE_MODE_TRANSPARENT;

      /* Radio Bearer ID not applicable - Set it to 0*/
      *(dl_log_ptr++) = 0;

      /* MAC ID not applicable - Set it to 0  */
      *(dl_log_ptr++) = 0;

      /* MAC Priority not applicable - Set it to 0  */
      *(dl_log_ptr++) = 0;

    }

    /* Store FACH information.*/
    for (i = 0; (i < cfgptr->num_fachs) && (i<TDSUE_MAX_TRCH);i++)
    {
            fach_info = cfgptr->fach_info[i];
        no_Logical_fach = 0;
        if (fach_info->bcch_enable == TRUE)
        {
          no_Logical_fach++;
        }
        if (fach_info->ccch_enable == TRUE)
        {
          no_Logical_fach++;
        }
        if (fach_info->ctch_enable == TRUE)
        {
          no_Logical_fach++;
        }
        no_Logical_fach += fach_info->ndlchan;

      /* Store the Transport Channel ID */
      *(dl_log_ptr++) = (uint8)fach_info->trch_id;

      /* Store the Transport Channel Type */
      *(dl_log_ptr++) = (uint8)UE_DL_TRCH_FACH;

      /* Store the Number of Logical Channels mapped to FACH*/
      *(dl_log_ptr++) = no_Logical_fach;

      /* If BCCH is enabled, store BCCH information */
      if (fach_info->bcch_enable == TRUE)
      {
        /* Store the Logical Channel ID */
        *(dl_log_ptr++) = fach_info->bcch_id;

        /* Store the Logical Channel Type - BCCH */
        *(dl_log_ptr++)= (uint8)UE_LOGCHAN_BCCH;

        /* Store the Logical Channel Mode */
        *(dl_log_ptr++)= (uint8)UE_MODE_TRANSPARENT;

        /* Radio Bearer ID not applicable - Set it to 0*/
        *(dl_log_ptr++) = 0;

        /* MAC ID not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

        /* MAC Priority not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

      }

      /* If CCCH is enabled, store CCCH information */
      if (fach_info->ccch_enable == TRUE)
      {
        /* Store the Logical Channel ID */
        *(dl_log_ptr++) = fach_info->ccch_id;

        /* Store the Logical Channel Type - CCCH */
        *(dl_log_ptr++) = (uint8)UE_LOGCHAN_CCCH;

        /* Store the Logical Channel Mode */
        *(dl_log_ptr++)= (uint8)UE_MODE_UNACKNOWLEDGED;

        /* Radio Bearer ID not applicable - Set it to 0*/
        *(dl_log_ptr++) = 0;

        /* MAC ID not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

        /* MAC Priority not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

      }

      /* If CTCH is enabled, store CTCH information */
      if (fach_info->ctch_enable == TRUE)
      {
        /* Store the Logical Channel ID */
        *(dl_log_ptr++)  = fach_info->ctch_id;

        /* Store the Logical Channel Type - CTCH */
        *(dl_log_ptr++) = (uint8)UE_LOGCHAN_CTCH;

        /* Store the Logical Channel Mode */
        *(dl_log_ptr++)= (uint8)UE_MODE_UNACKNOWLEDGED;

        /* Radio Bearer ID not applicable - Set it to 0*/
        *(dl_log_ptr++) = 0;

        /* MAC ID not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

        /* MAC Priority not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;

      }

      /* Dedicated logical channels mapped to DCH */
      for (k = 0; (k < fach_info->ndlchan) && (k < MAX_DED_LOGCHAN_PER_TRCH);k++)
      {
        /* Store the Logical Channel ID */
        *(dl_log_ptr++) = fach_info->dlchan_info[k].rlc_id;

        /* Store the Logical Channel Type */
        *(dl_log_ptr++) = (uint8)fach_info->dlchan_info[k].chan_type;

        /* Store the Logical Channel Mode */
        *(dl_log_ptr++) = (uint8)fach_info->dlchan_info[k].rlc_mode;

        /* Store the Radio Bearer ID */
        *(dl_log_ptr++) = (uint8)fach_info->dlchan_info[k].rb_id;

        /* Store the MAC ID */
        *(dl_log_ptr++) = fach_info->dlchan_info[k].mac_id;

        *(dl_log_ptr++) = 0;

      }
    } /* for FACH */

    /* Store DCH information.*/
    for (i = 0; (i < cfgptr->num_dchs) && (i<TDSUE_MAX_TRCH);i++)
    {
            dch_info = cfgptr->dch_info[i];

      /* Store the Transport Channel ID */
      *(dl_log_ptr++) = (uint8)dch_info->trch_id;

      /* Store the Transport Channel Type */
      *(dl_log_ptr++) = (uint8)UE_TRCH_DCH;

      /* Store the Number of Logical channels mapped to DCH */
      *(dl_log_ptr++) = dch_info->ndlchan;

      for (k = 0; (k < dch_info->ndlchan) && (k < MAX_DED_LOGCHAN_PER_TRCH);k++)
      {
        dlc_ptr = &dch_info->dlchan_info[k];

        /* Store the Logical Channel ID */
        *(dl_log_ptr++) = dlc_ptr->rlc_id;

        /* Store the Logical Channel Type */
        *(dl_log_ptr++) = (uint8)dlc_ptr->chan_type;

        /* Store the Logical Channel Mode */
        *(dl_log_ptr++) = (uint8)dlc_ptr->rlc_mode;

        /* Store the Radio Bearer ID */
        *(dl_log_ptr++) = (uint8)dlc_ptr->rb_id;

        /* Store the MAC ID */
        *(dl_log_ptr++) = dlc_ptr->mac_id;

        /* MAC Priority not applicable - Set it to 0  */
        *(dl_log_ptr++) = 0;
      }
    } /* for DCH */
    /* Commit the buffer to log services.*/
    log_commit(dl_cfg_ptr);
  }
  else
  {
    TDSMAC_MSG2(MSG_LEGACY_LOW,"Dropped 0x%x,code enabled: %d\n",TDSCDMA_DL_MAC_CFG_LOG_PACKET,log_status(TDSCDMA_DL_MAC_CFG_LOG_PACKET));
  }
}/* tdsmac_dl_send_cfg_log_packet() */


#endif /* FEATURE_TDSCDMA_MAC_QXDM_LOGGING */
