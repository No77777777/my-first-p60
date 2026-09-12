#ifndef PS_DPM_DPL_H
#define PS_DPM_DPL_H
/*===========================================================================

                     PS_DPM_DPL . H
DESCRIPTION
Header file describing global DPM config definitions.

Copyright (c) 2013 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/inc/ps_dpm_dpl.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/10/13    bvd     Initial Version
===========================================================================*/


/*===========================================================================

                         EXTERNAL FUNCTIONS

===========================================================================*/
/**
  @brief  This function initialize IPA DPL module. It needs to be called at power up.

  @return  N/A
  
  @code
  @endcode
*/
void ps_dpm_dpl_init
(
  void
);

/**
  @brief  This function flushes all packets from the IPA DPL watermark..

  @return  N/A

  @code
  @endcode
*/
void ps_dpm_dpl_deinit
(
  void
);

/**
  @brief  This function is used to return pointer to DPM DPL WM

  @return  dsm_watermark_type* - Pointer to ps_dpm_dpl_wmk

  @code
  @endcode
*/
dsm_watermark_type *ps_dpmi_dpl_get_wmk
(
  void
);

/**
  @brief  This function is used to return pointer to dropped pkt DPL watermark

  @return  dsm_watermark_type* - Pointer to ps_dpm_dpl_dropped_pkt_wmk

  @code
  @endcode
*/
dsm_watermark_type *ps_dpmi_dropped_dpl_get_wmk
(
  void
);

/**
  @brief  Signal handler for PS_DPM_DPL_DROPPED_PKT_SIGNAL
   
  @param[in] sig  - Signal
  @param[in] user_data_ptr  - User data pointer (unused) 
   
  @return  boolean   TRUE: no more processing to be done
                     FALSE: Needs to be called again.

  @code
  @endcode
*/
boolean ps_dpm_dpl_dropped_pkt_sig_handler
(
  ps_sig_enum_type    sig,
  void              * user_data_ptr
);
/**
  @brief  enable DNE callback function

  @param[in] wmk_ptr  - Watermark pointer
     
  @return  None

  @code
  @endcode
*/

void ps_dpm_dpl_enable_dne_callback
(
  dsm_watermark_type  *wmk_ptr
);
/**
  @brief  disable DNE callback function

  @param[in] wmk_ptr  - Watermark pointer
     
  @return  None

  @code
  @endcode
*/

void ps_dpm_dpl_disable_dne_callback
(
  dsm_watermark_type  *wmk_ptr
);

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_RESET_SIO_CONF

DESCRIPTION
  Initilize timer and queue to store a list of DPL sio config and skip header
  bytes information.

PARAMETERS
  mux_id  : MUX ID

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dpm_dpl_log_reset_sio_conf
(
  uint8 mux_id
);

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_TIMER

DESCRIPTION
  Print log and start timer if necessary.

PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dpm_dpl_log_timer
(
  void
);

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_SIO_DESC_AND_STATUS

DESCRIPTION
  Log a packet with sio info(mux id) and iface info

PARAMETERS
  dpl_pdn_info      :  PDN specific info

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dpm_dpl_log_sio_desc_and_status
(
  dpl_sio_config       *dpl_sio_info_ptr
);

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_SKIP_HDR_BYTES

DESCRIPTION
  Log a packet with number of bytes to be skipped in IP packet processing

PARAMETERS
  mux_id      :  MUX ID (PDN)
  src_endp    :  Source end point
  hdr_bytes   :  Number of bytes
  direction   :  Direction 

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dpm_dpl_log_skip_hdr_bytes
(
  uint8                mux_id,
  uint8                src_endp,
  uint8                hdr_bytes,
  uint8                direction
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/
#endif /* PS_DPM_DPL_H */
