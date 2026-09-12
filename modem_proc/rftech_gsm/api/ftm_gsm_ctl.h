#ifndef FTM_GSM_CTL_H
#define FTM_GSM_CTL_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

												 F T M  G S M  C T L   

GENERAL DESCRIPTION
	This is the header file for the embedded FTM RF Commands 

Copyright (c) 1994, 1995, 1996 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998, 1999 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Copyright (c) 2000 -2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

												EDIT HISTORY FOR MODULE

	This section contains comments describing changes made to the module.
	Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/ftm_gsm_ctl.h#1 $ 
$DateTime: 2021/03/09 05:10:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/04/15   ck      Support GSM fbrx power meas
07/16/15   tsr     Added GSM Support for FTM Device manager  
06/10/15   tsr     Added FTM and Non-Signalling Support on a chain if TX device is not supported 
05/11/15   tsr     Add split Rx/Tx support to Non-Signalling 
04/20/15   zbz     Add support for tools to get correct NV container
04/15/15   sc      Rely on chain vars for split Rx/Tx info throughout call-flow
04/14/15   ck      add ftm command to return nv container 
04/08/15   tsr     Introduced ftm gsm chains based framework for split RX/TX devices 
12/09/14   ck      Add api to overwrite alt_path 
11/19/13   cpm     Added EGSM support for FTM_PROGRAM_QFE_SCENARIO
07/01/14   sw      Move the calls to the geran test mode api into rftech_gsm 
04/03/13   ec	   Pass rfm_dev to NS function
04/02/13   aka     Support for band change in frame sweep functionality. 
02/26/13   tws     Make the TX frequency offset device specific.
02/21/13   tws     Fix GSM FTM variable initialisation.
01/18/13   tws     Set TX frequency offset is not device dependent.
01/3/13    av      Changes to support chain2 for GSM
12/03/12   tws     Move the FTM variable initialisation back to core.
11/27/12   tws     Add device id to FTM.
12/17/07   ad      Support ftm_gsm_toggle_modes() for EDGE only support
12/10/02   xw      Renamed it to ftm_gsm_ctl.h
08-28-02   rjr     Created 
===========================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#include "target.h"
#include "rfgsm_nv_cmn.h"
#ifdef FEATURE_GERAN_RF_SELF_TEST
  #include "ftmdiag.h"
#endif
#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h" 
#include "gs.h"
#ifdef FEATURE_GSM
#include "ftm.h"


/*-----------------------------------------------------------------------------
                               Prototypes
-----------------------------------------------------------------------------*/

void ftm_gsm_start_geran( void * vars_ptr );

void ftm_gsm_stop_geran( rfm_device_enum_type rfm_dev );

sys_modem_as_id_e_type ftm_gsm_ctl_map_sub_id_to_as_id(uint8 sub_id);

void ftm_gsm_set_mode( rfm_device_enum_type dispatch_device,
                       ftm_mode_id_type mode );    

void ftm_gsm_frame_sweep_set_band( rfm_device_enum_type rfm_dev,
                                   ftm_mode_id_type mode );

void ftm_tdma_isr_sub0( sys_modem_as_id_e_type as_id );

void ftm_tdma_isr_sub1( sys_modem_as_id_e_type as_id );

void ftm_gsm_init_once( void );

void ftm_gsm_init_rx( rfm_device_enum_type rfm_dev );

void ftm_gsm_init_tx( rfm_device_enum_type rfm_dev );

gs_status_T ftm_send_FTM_STOP_GSM_MODE_REQ( sys_modem_as_id_e_type as_id);

extern void ftm_gsm_toggle_modes( void );

void ftm_gsm_set_tx_frequency_offset( rfm_device_enum_type rfm_dev, int32 offset );
#ifdef FEATURE_GERAN_RF_SELF_TEST
boolean ftm_gsm_enable_selftest(rfm_device_enum_type rfm_dev, boolean enable);

boolean ftm_gsm_selftest_write_capture_params_to_commonFW(rfm_device_enum_type  device, uint8 cap_type_sw);

void ftm_gsm_fbrx_meas_tx_params
(
   rfcom_device_enum_type device,   
   rfcom_device_enum_type tx_device,
   uint16 num_averages,
   uint32 capture_offset,
   uint32 tx_measurement_config,
   void* cmd_ptr,
   ftm_rsp_pkt_type *rsp_pkt
);
void ftm_gsm_selftest_config
(
  rfm_device_enum_type device,
  rfm_device_enum_type fbrx_dev,
  boolean config
);
#endif

void ftm_gsm_program_qfe_scenario(void);


boolean ftm_gsm_overwrite_alt_path(rfm_device_enum_type rfm_dev, uint8 alt_path, boolean overwrite);

boolean ftm_gsm_get_nv_container(rfm_device_enum_type rx_device, ftm_mode_id_type band, rfgsm_nv_container_device_type nv_cont_type, rfgsm_nv_path_type* nv_container);

#endif /* FTM_HAS_GSM */
#endif // FEATURE_FACTORY_TESTMODE
#endif  // FTM_GSM_CTL_H
