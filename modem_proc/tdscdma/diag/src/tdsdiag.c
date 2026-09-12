/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                Diagnostics Packet Processing Routines
                  TD-SCDMA Target specific Routines

General Description
  Diagnostic packet processing routines that are specific to TD-SCDMA
  target.

Copyright (c) 2012-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                           Edit History

$Header: //components/rel/tdscdma.mpss/4.3/diag/src/tdsdiag.c#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/06/13   yuw     Supported HSUPA PLT on Bolt
08/24/11   mkh     Enabled Diag Commands for TDSCDMA
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_TDSCDMA

#ifdef __cplusplus
  extern "C" {
#endif

#include <string.h>
#include "tdsdiag.h"
#include "diagcmd.h"
#include "msg.h"
#include "err.h"
#include "mobile.h"


#ifdef FEATURE_CM
#include "cmutil.h"  /* prototype for cm_num_fill() */
#include "cm.h"
#endif

#ifndef  FEATURE_TDSCDMA_PLT
#include "mm.h"
#endif

#ifdef FEATURE_TDSCDMA_TCXO_RESOURCE_SUPPORT
#include "tdsl1pos.h"
#endif

#include "tdsl1extif.h"
#include "ghdi_exp.h"

#ifdef FEATURE_TDSCDMA_SELF_HOSTING
#include "tdscdmaselfhosting.h"
#endif

#if  defined(FEATURE_TDSCDMA_L1_MANUAL_CQI_UPD)
#include "tdsdechs.h"
#endif /* defined(FEATURE_TDSCDMA_L1_MANUAL_CQI_UPD) */

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
#include "tdsrlcfc.h"
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

#include "tdsedpch.h"
#include "mcaltds_dec.h"
#include "tdsrrcdata.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcpg1.h"
#include "tdsrrcscmgr.h"
#include "tdsrrccommon.h"
#include "log_msgs_tdscdma.h"
#include "tdsmacrach.h"
/*zhengming: add one header file*/
//#include "tpltrpccmd.h"
#ifdef FEATURE_CMI
#include "wcdmadiagif.h"
#endif
#include "tdsseq.h"

#ifdef FEATURE_TDSCDMA_PLT
#include "tplt_cntrl.h"
#include "tdsl1m.h"
#include "tpltrpccmdi.h"
#include "tpltrpccmd.h"
#include "tdsdldecdata.h"
#endif

#include "tdsrxdiv.h"



/* TMC states defined in Serial Interface Control Document for W-CDMA */
const uint8 TDS_TMC_STATE_OFFLINE = 0;
const uint8 TDS_TMC_STATE_ONLINE  = 1;
const uint8 TDS_TMC_STATE_LPM     = 2;
const uint8 TDS_TMC_STATE_UNKNOWN = 99;

#ifndef  FEATURE_TDSCDMA_PLT
/* Copies of counters, so we can simulate a "reset" of these variables
** without modifying the originals. */
uint32 tdsreset_rrcpg1_num_pages = 0;
uint32 tdsreset_rrcpg1_num_pages_matched = 0;
#endif

#ifdef FEATURE_CM
LOCAL static cm_call_id_type call_id = 0;
extern boolean cm_call_id_is_allocated( cm_call_id_type ); /* For tdscdmadiag_call_end() */
#endif

#ifdef FEATURE_TDSCDMA_PLT
/*For diag command logging*/
DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  tplt_diag_cmd_logs[TPLT_MAX_DIAG_CMD_LOG];
uint8                          tplt_diag_cmd_logs_idx;
extern tplt_cmd_status_type  tplt_drx_cmd_proc(tplt_cmd_drx_param_type *param);
#endif

#ifdef FEATURE_TDSCDMA_SELF_HOSTING
tds_self_hosting_event_type tds_self_hosting_event;
#endif

/* -----------------------------------------------------------------------
** Local functions
** ----------------------------------------------------------------------- */

PACKED void *tdscdmadiag_verno (PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_rrc_get_state_handler(PACKED void *req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_change_bler_rate(PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_call_orig (PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_call_end (PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_status (PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_additional_status (PACKED void* req_pkt, uint16 pkt_len);

PACKED void * tdscdmadiag_manual_cqi_upd_config_handler(PACKED void *req_ptr, uint16 pkt_len);

PACKED void * tdscdmadiag_rxd_enable_disable_handler(PACKED void *req_ptr, uint16 pkt_len);

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
PACKED void * tdscdmadiag_rlc_flow_control_config_handler(PACKED void *req_ptr, uint16 pkt_len);
#endif

#ifdef FEATURE_TDSCDMA_HSUPA
PACKED void * tdscdmadiag_manual_eul_ul_var_upd_handler(PACKED void *req_ptr,uint16 pkt_len);
#endif

PACKED void *tdscdmadiag_tdscdma_stats (PACKED void* req_pkt, uint16 pkt_len);

PACKED void *tdscdmadiag_clear_tdscdma_stats (PACKED void* req_pkt, uint16 pkt_len);

PACKED void * tdscdmadiag_dec_debug_dump_handler(PACKED void *req_ptr, uint16 pkt_len);

#if defined(FEATURE_TDSCDMA_OFFLINE_MODEM)
PACKED void * tdscdmadiag_tdscdma_cqi_params_handler(PACKED void *req_ptr, uint16 pkt_len);
#endif

#ifdef FEATURE_TDSCDMA_PLT
PACKED void * tdscdmadiag_tplt_mode_handler(PACKED void *req_ptr, uint16 pkt_len);
#endif
uint32 tl1_get_frame_number_for_diag(void);

//extern tplt_cmd_status_type TPLT_CMD_START_MODE (void);

/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================
Function: tdscdmadiag_verno
Description:

       This function processes the tdscdma specific version request packet
       from QXDM and sends the response back.

============================================================================*/

PACKED void *tdscdmadiag_verno (PACKED void* req_pkt, uint16 pkt_len)
{

  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_VERNO_F_rsp_type *rsp_ptr;
  //uint32 val;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_VERNO_F_rsp_type);

  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDSCMDADIAG::Version Number Request");
  
  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_VERNO_F_rsp_type *)diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_VERNO_F, rsp_len );
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_VERNO_F, rsp_len);
  }
  else
  {
    /* Fill the response packet */
  
    //rsp_ptr->class_mark = CLASS_MARK ;
  
    /* 1 - 2000 and c for 12 month */
    //rsp_ptr->g3gpp_rev = G3GPP_REV ;
  
    //rsp_ptr->g3gpp_rel = 4 ;
  
    rsp_ptr->vdsp_ver = VOC_DSP_VER;
  
    /* Not supported in new WFW API yet. */
    rsp_ptr->mdsp_ver_rom = 0;
    rsp_ptr->mdsp_ver_ram = 0;
  
    //rsp_ptr->drx_index = DRX_INDEX;
  
    /*  last 8 bits of HW_REVISION_NUMBER represent the MSM version  */
  
    //val = INP32 (HW_REVISION_NUMBER);
    //rsp_ptr->msm_ver_maj = (val & 0xff) ;
    //rsp_ptr->msm_ver_min = 0;
  
    /*  TDSCDMA model number is 90 */
    //rsp_ptr->model_num = mob_model ;
  }
  return rsp_ptr ;

}

/*==========================================================================

PACKET   TDSCDMADIAG_RRC_GET_STATE_HANDLER

PURPOSE  Request sent from the DM to the DMSS, to return the current state
         of RRC

RETURN VALUE
         Pointer to response packet.

============================================================================*/

PACKED void *tdscdmadiag_rrc_get_state_handler(PACKED void *req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_RRC_GET_STATE_F_rsp_type *rsp_ptr;
  //uint32 val;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_RRC_GET_STATE_F_rsp_type);

  /* Allocate the length as the response packet */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_RRC_GET_STATE_F_rsp_type *) 
    diagpkt_subsys_alloc (DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_RRC_GET_STATE_F, rsp_len);

  if(rsp_ptr)
  {
    rsp_ptr->rrc_curr_state = (uint8) tdsrrc_get_state();
  }
  else
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_RRC_GET_STATE_F, rsp_len);
  }
 
  return rsp_ptr;
} /* tdsscdmadiag_rrc_get_state_handler */


/*===========================================================================
Function: tdscdma_diag_change_bler_rate
Description:

       This function processes the tdscdma specific L1 bler measure rate change.

============================================================================*/

/* Variable for automatically measure BLER.
   Value 0 means that timer is infinite */
extern uint16  tdsdl_bler_meas_duration_ms;
/* set this variable to TRUE if the duration is required to be updated.
   The time will be updated in next 10 ms */
extern boolean tdsdl_bler_meas_timer_update;

PACKED void *tdscdmadiag_change_bler_rate(PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_BLER_RATE_CHANGE_F_req_type *req_ptr = ( DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_BLER_RATE_CHANGE_F_req_type *)req_pkt;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_BLER_RATE_CHANGE_F_rsp_type *rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_BLER_RATE_CHANGE_F_rsp_type);

  tdsdl_bler_meas_timer_update = TRUE;

  tdsdl_bler_meas_duration_ms = req_ptr->bler_meas_duration_ms;

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_BLER_RATE_CHANGE_F_rsp_type * )diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_BLER_RATE_CHANGE_F,
                                 rsp_len );

  return rsp_ptr;
}


/*===========================================================================
Function: tdscdmadiag_call_orig
Description:

       This function processes the tdscdma specific call orig request packet
       from QXDM and sends the response back.

============================================================================*/

PACKED void *tdscdmadiag_call_orig (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_ORIG_F_rsp_type *rsp_ptr;

#ifdef FEATURE_CM
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_ORIG_F_req_type *req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_ORIG_F_req_type *)req_pkt;
  cm_num_s_type                numbers_buf;
  uint8                        work_buf[CM_MAX_NUMBER_CHARS];
  uint8                        i;
  cm_gw_cs_orig_params_s_type  call_orig_params;
  cm_num_s_type                calling_num;
  cm_orig_alpha_s_type         alpha_buf;
#endif /* FEATURE_CM */

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_ORIG_F_rsp_type);

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_ORIG_F_rsp_type *)diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_ORIG_F,
                                 rsp_len );

#ifdef FEATURE_CM
  // Initialize the buffer to CM */
  memset(&call_orig_params,
         CM_CALL_CMD_PARAM_DEFAULT_VALUE,
         sizeof(call_orig_params));

  if ( (req_ptr->num_digits > DIAG_MAX_TDSCDMA_DIGITS)
       || (req_ptr->num_digits > CM_MAX_NUMBER_CHARS) )
  {
    TDSL1_MSG0(ERROR,"Dialed too many digits");
    return rsp_ptr;
  }

  // initialize work_buf
  memset (work_buf, 0, ARR_SIZE(work_buf));

  for (i=0; i<req_ptr->num_digits; i++) {
      /* Check for dial digits (0 - 9, *, #) */
      if ( ((req_ptr->digits[i] < '0') || (req_ptr->digits[i] > '9')) &&
           (req_ptr->digits[i] != '*')  &&
           (req_ptr->digits[i] != '#')) {
          /* Was an invalid digit (not 0 - 9, *, #) */
          TDSL1_MSG1(ERROR,"Bad dial digit: %c", req_ptr->digits[i]);
          return rsp_ptr;
      }
      work_buf[i] = req_ptr->digits[i];
  }

  alpha_buf.len = 
    memscpy( alpha_buf.buf, (CM_MAX_ALPHA_LEN_FOR_CALL_ORIG*sizeof(uint8) ), (const byte *)"DIAG CALL", strlen("DIAG CALL")+1);

  cm_num_init( &calling_num );

  cm_num_fill(&numbers_buf, work_buf, req_ptr->num_digits, CM_DIGIT_MODE_4BIT_DTMF);


  (void) cm_mm_call_cmd_orig(
      NULL, /* client callback function */
      NULL, /* pointer to client callback data block */
      CM_CLIENT_ID_ANONYMOUS,
      CM_CALL_TYPE_VOICE,
      CM_SRV_TYPE_AUTOMATIC,
      &calling_num,
      &numbers_buf,
      &alpha_buf,        /* Alpha buffer for call  */
      NULL,              /* CDMA call orig params  */
      &call_orig_params, /* GW CS call orig params */
      NULL,              /* GW PS call orig params */
      &call_id
      );

#endif /* FEATURE_CM */

  return rsp_ptr ;

}

/*===========================================================================
Function: tdscdmadiag_call_end
Description:

       This function processes the tdscdma specific call end request packet
       from QXDM and sends the response back.

============================================================================*/

PACKED void *tdscdmadiag_call_end (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_END_F_rsp_type *rsp_ptr;


#ifdef FEATURE_CM
  cm_end_params_s_type end_params;
  uint8 num_call_ids = 0;
  int i;
#endif

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_END_F_rsp_type);

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_END_F_rsp_type *)diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_END_F,
                                 rsp_len );

#ifdef FEATURE_CM
  memset( &end_params,
          CM_CALL_CMD_PARAM_DEFAULT_VALUE,
          sizeof ( cm_end_params_s_type));


  /* Among active calls, the first active call returned by cm_call_id_is_allocated() is ended */
  for (i = 0; i < CM_CALL_ID_MAX; i++)
  {
    if ( cm_call_id_is_allocated ((cm_call_id_type) i) )
    {

      num_call_ids++;
      end_params.call_id = (cm_call_id_type) i;
      end_params.info_type = CM_CALL_MODE_INFO_GW_CS;
      TDSL1_MSG1(HIGH,"TDSCMDADIAG::END_REQ: Adding call_id %d to the end list",
      end_params.call_id);

      break;
    } /* if ( cm_call_id_is_allocated ((cm_call_id_type) i) ) */
  } /* for (i = 0; i < CM_CALL_ID_MAX; i++) */



  if (num_call_ids > 0)
  {
    (void) cm_mm_call_cmd_end(
      NULL, /* client callback function */
      NULL, /* pointer to client callback data block */
      CM_CLIENT_ID_ANONYMOUS,
      1,
      &end_params
      );
  }
#endif /* FEATURE_CM */
  return rsp_ptr ;

}

/*===========================================================================
Function: tdscdmadiag_status
Description:

       This function processes the tdscdma specific status request packet
       from QXDM and sends the response back.

============================================================================*/


#if defined(FEATURE_WCDMA_DIME_SW) && (!defined(FEATURE_TRITON_MODEM))

PACKED void *tdscdmadiag_status (PACKED void* req_pkt, uint16 pkt_len)
{

  DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_rsp_type *rsp_ptr;
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDSCMDADIAG::tdscdmadiag_status REQUEST for DI");

   /*  Allocate our packet */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_rsp_type *) diagpkt_subsys_alloc_v2 (
    DIAG_SUBSYS_TDSCDMA,
    DIAG_TDSCDMA_MAX_F,
    sizeof (DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_rsp_type) );

  if(rsp_ptr == NULL)
  {
    TDSL1_MSG0(ERROR,"tdscdmadiag_status: unabled to alloc mem for rsp");
  }
  else
  {
    diagpkt_subsys_reset_delayed_rsp_id(rsp_ptr);
    rsp_ptr->status = (uint8)3;
    rsp_ptr->value = 0;

    /* Set the first (0) response count */
    diagpkt_subsys_set_rsp_cnt(rsp_ptr, 0);

    /* Set the status in the response to SUCCESS */
    diagpkt_subsys_set_status(rsp_ptr, 0);
  }
  return rsp_ptr ;
}

#else

PACKED void *tdscdmadiag_status (PACKED void* req_pkt, uint16 pkt_len)
{

  sys_oprt_mode_e_type oprt_state; 
  uint8 oprt_state_return;

  DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_F_rsp_type *rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_F_rsp_type);

  TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDSCMDADIAG::tdscdmadiag_status REQUEST for non-DI PL");

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_STATUS_F_rsp_type *)diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_STATUS_F,
                                 rsp_len );

  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_STATUS_F, rsp_len);
  }
  else
  {
    /* Fill the response packet */
    oprt_state = cm_ph_get_oprt_mode();
  
    switch (oprt_state)
    {
    case SYS_OPRT_MODE_OFFLINE:
    case SYS_OPRT_MODE_OFFLINE_AMPS:
    case SYS_OPRT_MODE_OFFLINE_CDMA:
        oprt_state_return = TDS_TMC_STATE_OFFLINE;
        break;

    case SYS_OPRT_MODE_LPM:
        oprt_state_return = TDS_TMC_STATE_LPM;
        break;

    case SYS_OPRT_MODE_ONLINE:
        oprt_state_return = TDS_TMC_STATE_ONLINE;
        break;
  
    default:
        oprt_state_return = TDS_TMC_STATE_UNKNOWN;
        break;
    }
  
    rsp_ptr->tmc_state = oprt_state_return;

  }

  return rsp_ptr ;


}

#endif /**/

/*===========================================================================
Function: tdscdmadiag_additional_status
Description:

       This function processes the tdscdma specific additional status request
       packet from QXDM and sends the response back.

============================================================================*/
PACKED void *tdscdmadiag_additional_status (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_ADDITIONAL_STATUS_F_rsp_type *rsp_ptr ;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_ADDITIONAL_STATUS_F_rsp_type);

#if defined(FEATURE_DUAL_SIM) && !defined(FEATURE_TDSCDMA_PLT)
  /* Retrieve Access Stratum ID in use by TDSCDMA RAT. */
  sys_modem_as_id_e_type  as_id = tdsrrc_get_as_id();
#endif

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_ADDITIONAL_STATUS_F_rsp_type *)diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_ADDITIONAL_STATUS_F,
                                 rsp_len );

  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_ADDITIONAL_STATUS_F, rsp_len);
  }
  else
  {
#ifdef  FEATURE_TDSCDMA_PLT
    memset( (byte*)rsp_ptr->imei, 0, sizeof(rsp_ptr->imei) );
    memset( (byte*)rsp_ptr->imsi, 0, sizeof(rsp_ptr->imsi) );
#else
  
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_IMEI)
    if (tdsrrc_is_sim_id_valid)
    {
      if (as_id != SYS_MODEM_AS_ID_NONE) 
      {
        (void)ghdi_ds_read_imei((byte *)rsp_ptr->imei, as_id);
      }
      else
      {
        ERR_FATAL("Invalid AS_ID %d", as_id, 0, 0);
      }
    }
    else
    {
      TDSL1_MSG0(HIGH,"DSIM: SIM INSERTED REQ not received, zero out IMEI info");
      memset((void *)&rsp_ptr->imei[0], 0x00, sizeof(rsp_ptr->imei));
    }
#else
    (void)ghdi_read_imei( (byte *)rsp_ptr->imei );
#endif

#ifdef FEATURE_DUAL_SIM
    if (tdsrrc_is_sim_id_valid)
    {
      if (as_id != SYS_MODEM_AS_ID_NONE) 
      {
        mm_ds_get_imsi((imsi_data_T*)&rsp_ptr->imsi[0], as_id);
    }
	else
	{
        ERR_FATAL("Invalid AS_ID %d", as_id, 0, 0);
      }
    }
	else
	{
	  TDSL1_MSG0(HIGH,"DSIM: SIM INSERTED REQ not received, zero out IMSI info");
	  memset((void *)&rsp_ptr->imsi[0], 0x00, sizeof(imsi_data_T)); 
	}
#else
    mm_get_imsi((imsi_data_T*)&rsp_ptr->imsi[0]);
#endif
  
#endif
  
    rsp_ptr->l1_state = tdsl1m_get_state();
  }

  return rsp_ptr ;

}

/*===========================================================================

FUNCTION tdscdmadiag_rxd_enable_disable_handler

DESCRIPTION
  This function handles the request to enable disable CQI offset.

============================================================================*/

PACKED void * tdscdmadiag_rxd_enable_disable_handler(PACKED void *req_ptr, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_req_type *config_req_ptr;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_rsp_type *config_rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_rsp_type);
  config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_req_type *) req_ptr;

  TDSL1_MSG0(HIGH,"MR_DBG: executing tdscdmadiag_rxd_enable_disable_handler");
  /* A basic error checking on the received values */
  if ((config_req_ptr->rxd_state != TRUE)     &&
      (config_req_ptr->rxd_state != FALSE))
  {
    TDSL1_MSG0(HIGH,"MR_DBG: error checking on the received values , rxd_state is NULL");

     return diagpkt_err_rsp(
        DIAG_BAD_PARM_F,
        config_req_ptr,
        sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_req_type)
        );
  }

  /* Prepare the response packet */
  config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F, rsp_len );

#if 0
  config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_rsp_type *)
                        diagpkt_alloc(
                           diagpkt_get_cmd_code( req_ptr ),
                           sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F_rsp_type)
                        );
 #endif
  if ( config_rsp_ptr == NULL )
  {
     /* Allocation failed, and diagpkt_err_rsp will fail as well.
     ** So just return NULL.
     */
     ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F, rsp_len);
  }

  /* Copy the header from the request packet to the response packet */
  (void)memscpy(
     (void*)config_rsp_ptr,
	 rsp_len,
     (void*)config_req_ptr,
     sizeof(diagpkt_subsys_header_type)
  );

  /* Generate the response */
  config_rsp_ptr->status = 1;

   /* Call the RxDiv function that will take the values from diag request packet
      and update the RxDiv On Off flag */

     tdsrxdiv_set_enable_disable_mask(config_req_ptr->rxd_state);

   return (void *) config_rsp_ptr;

}

/*===========================================================================

FUNCTION tdscdmadiag_manual_cqi_upd_config_handler

DESCRIPTION
  This function handles the request to manually update CQI offset.

============================================================================*/
PACKED void * tdscdmadiag_manual_cqi_upd_config_handler(
   PACKED void *req_ptr,
   uint16       pkt_len
)
{
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_req_type *config_req_ptr;
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_rsp_type *config_rsp_ptr;

   config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_req_type *) req_ptr;

   /* A basic error checking on the received values */
   if ((config_req_ptr->manual_cqi_update_flag != TRUE)     &&
       (config_req_ptr->manual_cqi_update_flag != FALSE))
   {
      return diagpkt_err_rsp(
         DIAG_BAD_PARM_F,
         config_req_ptr,
         sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_req_type)
         );
   }

   /* Prepare the response packet */
   config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_rsp_type *)
                         diagpkt_alloc(
                            diagpkt_get_cmd_code( req_ptr ),
                            sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_rsp_type)
                         );

   if ( config_rsp_ptr == NULL )
   {
      /* Allocation failed, and diagpkt_err_rsp will fail as well.
      ** So just return NULL.
      */
      return NULL;
   }

   /* Copy the header from the request packet to the response packet */
   (void)memscpy(
      (void*)config_rsp_ptr,
	  sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_MANUAL_CQI_UPD_CONFIG_F_rsp_type),
      (void*)config_req_ptr,
      sizeof(diagpkt_subsys_header_type)
   );
   
   /* Generate the response */
   config_rsp_ptr->status = 1;

#if  defined(FEATURE_TDSCDMA_L1_MANUAL_CQI_UPD)
   /* Call the HS function that will take the values from diag request packet
      and update the CQI offset variables */
   dec_hs_set_manual_cqi_upd_params(
         config_req_ptr->manual_cqi_update_flag,
         config_req_ptr->manual_cqi_update_offset);
#endif
   return (void *) config_rsp_ptr;

} 


/*===========================================================================
Function: tdscdmadiag_tdscdma_stats
Description:

  This function processes the TDSCDMA specific TDSCDMA_DIAG_TDSCDMA_STATS_F
  command, which requests the statistics related to Paging recorded in WL1 and RRC.

  This function is called when "send_data 75 4 26 0" is issued from cmd 
  line interface from QXDM

  Where the 75 is DIAG_SUBSYS_CMD_F {Subssytem dispatcher (extended diag cmd)}
  The 4 is DIAG_SUBSYS_TDSCDMA
  and the 26 is for TDSCDMA_DIAG_TDSCDMA_STATS_F, which is the function we are using.
  
  Sample output would be:
  16:54:56.580 0x4B 04 1A 00 02 00 00 00 03 00 00 00 00 00 00 00 00 01 00 00 F7 04 00 00
                              ---------   ---------   ---------   ---------   --------- 

   rrcpg1_num_pages          = 0x00 00 00 02
   rrcpg1_num_pages_matched  = 0x00 00 00 03
   drx_pich_demod_error      = 0x00 00 00 00
   drx_cycle_len             = 0x00 00 01 00
   num_drx_cycles            = 0x00 00 04 F7

============================================================================*/
PACKED void *tdscdmadiag_tdscdma_stats (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_STATS_F_rsp_type *rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_STATS_F_rsp_type);
  tdsrrc_debug_global_var_type* tdsrrc_debug_global_ptr;
  tdsrrc_debug_get_global_var(&tdsrrc_debug_global_ptr);

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_STATS_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_TDSCDMA_STATS_F,
                                   rsp_len );
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_TDSCDMA_STATS_F, rsp_len);
  }
  else
  {
    /* Reacq slew stats*/
    rsp_ptr->tdssrchzz_reacq_slew_stats = tdssrchzz_reacq_slew_stats;
  
    /* Reacq search stats*/
    rsp_ptr->tdssrchzz_reacq_srch_stats = tdssrchzz_reacq_srch_stats;
  
    /* Number of times SFN mismatch is detected */
    rsp_ptr->tdsdl_num_times_sfn_mismatch = tdsdl_num_times_sfn_mismatch;
  
    /* Paging stats*/
  
  #ifdef  FEATURE_TDSCDMA_PLT
    /* RRC doesn't exist if we're making a WPLT build. */
    rsp_ptr->page_stats.rrcpg1_num_pages = 0;
    rsp_ptr->page_stats.rrcpg1_num_pages_matched = 0;
  #else
    rsp_ptr->page_stats.rrcpg1_num_pages = tdsrrcpg1_get_num_pages() - tdsreset_rrcpg1_num_pages;
    rsp_ptr->page_stats.rrcpg1_num_pages_matched = tdsrrcpg1_get_num_pages_matched() -
                                                          tdsreset_rrcpg1_num_pages_matched;
  #endif /*  FEATURE_TDSCDMA_PLT */
    rsp_ptr->page_stats.drx_pich_demod_error = 
                                   tds_drx_parms.num_zero_pisum + tds_drx_parms.num_po_overruns;
    rsp_ptr->page_stats.drx_cycle_len = tds_drx_parms.drx_cycle_len;
    rsp_ptr->page_stats.num_drx_cycles = tds_drx_parms.total_drx_cycles;
  
  #ifdef  FEATURE_TDSCDMA_PLT
    /* RRC doesn't exist in a WPLT build. */
    rsp_ptr->intra_freq_resel_stats.intra_freq_resel_success_cnt = 0;
    rsp_ptr->intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt = 0;
    rsp_ptr->intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt = 0;
    rsp_ptr->intra_freq_resel_stats.intra_freq_transition_fail_cnt = 0;
    rsp_ptr->intra_freq_resel_stats.intra_freq_failed_other_causes_cnt = 0;
  
    rsp_ptr->ttog_resel_stats.ttog_resel_cnt = 0;
    /*rsp_ptr->ttog_resel_stats.ttog_resel_failure_cnt = 0;*/
  
    rsp_ptr->oos_stats.oos_idle_cnt = 0;

    /*rsp_ptr->oos_stats.oos_connected_cnt = 0;*/
  
    rsp_ptr->rach_transmission_stats.succful_rrc_tran_retry_cnt = 0;
    rsp_ptr->rach_transmission_stats.rrc_retry_fail_due_to_l2_ack_ind = 0;
    rsp_ptr->rach_transmission_stats.rrc_retry_fail_due_to_others = 0;
  #else
    /* Various Intra freq cell reselection statistics */
    rsp_ptr->intra_freq_resel_stats.intra_freq_resel_success_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt;
    rsp_ptr->intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt;
    rsp_ptr->intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt;
    rsp_ptr->intra_freq_resel_stats.intra_freq_transition_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt;
    rsp_ptr->intra_freq_resel_stats.intra_freq_failed_other_causes_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt;

    /* Various Inter freq cell reselection statistics */
    rsp_ptr->inter_freq_resel_stats.inter_freq_resel_success_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_inter_freq_resel_stats.inter_freq_resel_success_cnt;
    rsp_ptr->inter_freq_resel_stats.inter_freq_nbcch_setup_failure_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_inter_freq_resel_stats.inter_freq_nbcch_setup_failure_cnt;
    rsp_ptr->inter_freq_resel_stats.inter_freq_sib_wait_timeout_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_inter_freq_resel_stats.inter_freq_sib_wait_timeout_cnt;
    rsp_ptr->inter_freq_resel_stats.inter_freq_transition_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_inter_freq_resel_stats.inter_freq_transition_fail_cnt;
    rsp_ptr->inter_freq_resel_stats.inter_freq_failed_other_causes_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_inter_freq_resel_stats.inter_freq_failed_other_causes_cnt;
  
    /* Various TDSCDMA to GSM cell reselection statistics */
    rsp_ptr->ttog_resel_stats.ttog_resel_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_cnt;
    rsp_ptr->ttog_resel_stats.ttog_resel_success_cnt = 
        tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_cnt-tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt;
    rsp_ptr->ttog_resel_stats.ttog_resel_failure_bch_reading_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_NBCCH_SETUP_FAILURE;
    rsp_ptr->ttog_resel_stats.ttog_resel_failure_acq_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_ACQ_FAILURE;
    rsp_ptr->ttog_resel_stats.ttog_resel_failure_sib_timeout_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_SIB_WAIT_TIMEOUT;
    rsp_ptr->ttog_resel_stats.ttog_resel_failure_other_cause_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_OTHER_CAUSE;
  
    /* Various OOS related statistics */
    rsp_ptr->oos_stats.oos_idle_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_oos_stats.oos_idle_cnt;
    rsp_ptr->oos_stats.oos_connected_cell_dch_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_oos_stats.oos_connected_cnt_cell_dch;
    rsp_ptr->oos_stats.oos_connected_cell_fach_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_oos_stats.oos_connected_cnt_cell_fach;
    rsp_ptr->oos_stats.oos_connected_cell_pch_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_oos_stats.oos_connected_cnt_cell_pch;
  
    /* Various RACH transmission related statistics,This is currently TBD. Its values are set to Zero for now*/
    rsp_ptr->rach_transmission_stats.succful_rrc_tran_retry_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt;
    rsp_ptr->rach_transmission_stats.rrc_retry_fail_due_to_l2_ack_ind = tdsrrc_debug_global_ptr->tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt;
    rsp_ptr->rach_transmission_stats.rrc_retry_fail_due_to_others = tdsrrc_debug_global_ptr->tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt;

    /*ttol*/
    rsp_ptr->ttol_resel_stats.ttol_resel_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_cnt;
    rsp_ptr->ttol_resel_stats.ttol_resel_success_cnt = 
        tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_cnt - tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt;
    rsp_ptr->ttol_resel_stats.ttol_resel_failure_bch_reading_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_NBCCH_SETUP_FAILURE;
    rsp_ptr->ttol_resel_stats.ttol_resel_failure_acq_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_ACQ_FAILURE;
    rsp_ptr->ttol_resel_stats.ttol_resel_failure_sib_timeout_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_SIB_WAIT_TIMEOUT;
    rsp_ptr->ttol_resel_stats.ttol_resel_failure_other_cause_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_OTHER_CAUSE;
    /*gtot*/
    rsp_ptr->gtot_resel_stats.gtot_resel_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_cnt;
    rsp_ptr->gtot_resel_stats.gtot_resel_success_cnt =
        tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_cnt -tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt;
    rsp_ptr->gtot_resel_stats.gtot_resel_failure_bch_reading_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_NBCCH_SETUP_FAILURE;
    rsp_ptr->gtot_resel_stats.gtot_resel_failure_acq_fail_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_ACQ_FAILURE;
    rsp_ptr->gtot_resel_stats.gtot_resel_failure_sib_timeout_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_SIB_WAIT_TIMEOUT;
    rsp_ptr->gtot_resel_stats.gtot_resel_failure_other_cause_cnt= tdsrrc_debug_global_ptr->tdsrrc_debug_g2td_resel_stats.g2td_resel_failue_cnt_OTHER_CAUSE;

    /*ltot*/
    rsp_ptr->ltot_resel_stats.ltot_resel_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_l2td_resel_stats.l2td_resel_cnt;
    rsp_ptr->ltot_resel_stats.ltot_resel_failure_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_l2td_resel_stats.l2td_resel_failure_cnt;

    /*acq*/
    rsp_ptr->acq_attemp_stats.td_acq_attemp_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_acq_stats.td_acq_attemp_cnt;
    rsp_ptr->acq_attemp_stats.td_acq_attemp_succeed_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_acq_stats.td_acq_attemp_succeed_cnt;
    rsp_ptr->acq_attemp_stats.td_acq_attemp_failed_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_acq_stats.td_acq_attemp_failed_cnt;

    /*bplmn*/
    rsp_ptr->bplmn_stats.td2td_bplmn_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_bplmn_stats.td2td_bplmn_cnt;
    rsp_ptr->bplmn_stats.td2g_bplmn_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_bplmn_stats.l2td_bplmn_cnt;
    rsp_ptr->bplmn_stats.td2l_bplmn_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_bplmn_stats.td2l_bplmn_cnt;
    rsp_ptr->bplmn_stats.l2td_bplmn_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_bplmn_stats.l2td_bplmn_cnt;
    rsp_ptr->bplmn_stats.g2td_bplmn_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_bplmn_stats.g2td_bplmn_cnt;

    /*fast return*/
    rsp_ptr->fast_return_stats.fast_return_triggered_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_gtot_fast_return_stats.fast_return_trigger_cnt;
    rsp_ptr->fast_return_stats.fast_return_succeed_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_gtot_fast_return_stats.fast_return_succeed_cnt;

    /*rrc connection setup*/
    rsp_ptr->rrc_connetion_setup_stats.rce_attemp_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_rce_stats.tdsrrc_rce_attemp_cnt;
    rsp_ptr->rrc_connetion_setup_stats.rce_succeed_cnt = tdsrrc_debug_global_ptr->tdsrrc_debug_rce_stats.tdsrrc_rce_succeed_cnt;
    rsp_ptr->rrc_connetion_setup_stats.rce_failed_cnt = 
        tdsrrc_debug_global_ptr->tdsrrc_debug_rce_stats.tdsrrc_rce_attemp_cnt - tdsrrc_debug_global_ptr->tdsrrc_debug_rce_stats.tdsrrc_rce_succeed_cnt;
  #endif /*  FEATURE_TDSCDMA_PLT */

    /* Sarao: Various RACH parameters from L2*/
    rsp_ptr->tdscdma_rach_stats.rach_success_cnt = tdsl2_get_rach_success_cnt();
    rsp_ptr->tdscdma_rach_stats.rach_failure_due_to_abort_cnt = tdsl2_get_rach_failure_due_to_abort_cnt();
    rsp_ptr->tdscdma_rach_stats.rach_failure_due_to_max_rach_attempts_cnt = tdsl2_get_rach_failure_due_to_max_rach_attempts_cnt();
    rsp_ptr->tdscdma_rach_stats.rach_failure_due_to_other_reasons_cnt = tdsl2_get_rach_failure_due_to_other_reasons_cnt();

    /* + defang 02/21/2011: commented from original code */
  #if 0
    /* RACH Transmission Preamble related statistics */
    memscpy((void *)(rsp_ptr->rach_transmission_preamble_stats.rach_ack_prmbl_cnt),sizeof(rach_ack_prmbl_cnt),
           (void *)rach_ack_prmbl_cnt, sizeof(rach_ack_prmbl_cnt));
  
    rsp_ptr->rach_transmission_preamble_stats.rach_noack_cnt = tdsrach_noack_cnt;
    rsp_ptr->rach_transmission_preamble_stats.rach_nak_prmbl_max_cnt = 
                                                      tdsrach_nak_prmbl_max_cnt;
    rsp_ptr->rach_transmission_preamble_stats.rach_max_prmbl_cnt = 
                                                      tdsrach_max_prmbl_cnt;
    rsp_ptr->rach_transmission_preamble_stats.rach_total_attempts =
                                                      tdsrach_total_attempts;
    rsp_ptr->rach_transmission_preamble_stats.rach_prmbl_detected_max_pwr_dB =
                                                      tdsrach_prmbl_detected_max_pwr_dB;
    rsp_ptr->rach_transmission_preamble_stats.rach_detected_last_prmbl_avg_pwr_dB =
                                                      tdsrach_detected_last_prmbl_avg_pwr_dB;
    rsp_ptr->rach_transmission_preamble_stats.rach_noack_last_prmbl_avg_pwr_dB =
                                                      tdsrach_noack_last_prmbl_avg_pwr_dB;
    rsp_ptr->rach_transmission_preamble_stats.rach_noack_prmbl_max_pwr_dB =
                                                     tdsrach_noack_prmbl_max_pwr_dB;
  #endif
    /* - defang 02/21/2011 */
  }
  return rsp_ptr ;

}

/*===========================================================================
Function: tdscdmadiag_clear_tdscdma_stats 
Description:

  This function processes the TDSCDMA specific TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F
  command, which clears the statistics related to Paging recorded in WL1 and RRC.
  This is implemented by setting auxiliary variables to the current values, 
  and then subtracting these values from the actual values, and then returning them.

  This function is called when "send_data 75 4 27 0" is issued from cmd 
  line interface from QXDM

============================================================================*/
PACKED void *tdscdmadiag_clear_tdscdma_stats (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F_rsp_type *rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F_rsp_type);

  /* "Clear out" all the paging statistics, by setting reset counters,
  ** Because we dont want to reset the counters themselves. */
#ifndef  FEATURE_TDSCDMA_PLT
  /* RRC doesn't exist in a WPLT build. */
  tdsreset_rrcpg1_num_pages = tdsrrcpg1_get_num_pages();
  tdsreset_rrcpg1_num_pages_matched = tdsrrcpg1_get_num_pages_matched();
#endif /*  FEATURE_TDSCDMA_PLT */
  tds_drx_parms.num_zero_pisum = 0;
  tds_drx_parms.num_po_overruns = 0;
  tds_drx_parms.total_drx_cycles = 0;

  /* Clear out the SFN mismatch counter */
  tdsdl_num_times_sfn_mismatch = 0;

  /* Clear out all reacquisition statistics. */
  memset((void*)&tdssrchzz_reacq_slew_stats, 0x00, sizeof(srchzz_reacq_slew_stats_type));
  memset((void*)&tdssrchzz_reacq_srch_stats, 0x00, sizeof(srchzz_reacq_srch_stats_type));

#ifndef  FEATURE_TDSCDMA_PLT
  /* Clear out all RRC statistics. */
  TDSRRC_DEBUG_CLEAR_ALL_STATS();
#endif /*  FEATURE_TDSCDMA_PLT */

  /*Sarao: Clear all TDS L2 stats*/
  tdsl2_clear_rach_stats();


  /* + defang 02/21/2011: commented from original code */ 
#if 0
  /* Clear out RACH Transmission Preamble related statistics */
  memset((void*)rach_ack_prmbl_cnt, 0x00, sizeof(rach_ack_prmbl_cnt));
  tdsrach_noack_cnt = 0;
  tdsrach_nak_prmbl_max_cnt =0;
  tdsrach_max_prmbl_cnt =0;
  tdsrach_total_attempts =0;
  tdsrach_prmbl_detected_max_pwr_dB = RACH_STATS_INITIAL_TXAGC;
  tdsrach_noack_prmbl_max_pwr_dB = RACH_STATS_INITIAL_TXAGC;
  tdsrach_detected_last_prmbl_avg_pwr_dB = 0;
  tdsrach_noack_last_prmbl_avg_pwr_dB = 0;
  tdsrach_detected_last_prmbl_tot_pwr_dB = 0;
  tdsrach_noack_last_prmbl_tot_pwr_dB = 0;
  tdsrach_ack_cnt = 0;
  tdsrach_nack_cnt = 0;
#endif
  /* - defang 02/21/2011 */

  /* Allocate the memory for this unused response item. */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F,
                                   rsp_len );

  return rsp_ptr ;
}

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================

FUNCTION TDSCDMADIAG_RLC_FLOW_CONTROL_CONFIG_HANDLER

DESCRIPTION
  This function handles the request to RLC Flow Controller thresholds

============================================================================*/
PACKED void * tdscdmadiag_rlc_flow_control_config_handler(
   PACKED void *req_ptr,
   uint16       pkt_len
)
{
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_req_type *config_req_ptr;
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_rsp_type *config_rsp_ptr;

   config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_req_type *) req_ptr;

   /* skip parameters validation */

   config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_rsp_type *)
                         diagpkt_alloc(
                            diagpkt_get_cmd_code( req_ptr ),
                            sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_rsp_type)
                         );

   if ( config_rsp_ptr == NULL )
   {
      /* Allocation failed, and diagpkt_err_rsp will fail as well.
      ** So just return NULL.
      */
      return NULL;
   }

   (void)memscpy(
      (void*)config_rsp_ptr,
	  sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F_rsp_type),
      (void*)config_req_ptr,
      sizeof(diagpkt_subsys_header_type)
   );

   config_rsp_ptr->status = 1;

   #ifdef FEATURE_UL_CPU_BASED_FC
   tdsrlc_ul_set_fc_config(config_req_ptr->ws_step_period,
                     config_req_ptr->ws_step_stage,
                     config_req_ptr->init_ws);
   #endif

   #ifdef FEATURE_DL_CPU_BASED_FC
   tdsrlc_dl_set_fc_config(config_req_ptr->ws_step_period,
                     config_req_ptr->ws_step_stage,
                     config_req_ptr->init_ws);
   #endif
   return (void *) config_rsp_ptr;

} /* rex_diag_taskprofile_handler */
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION  tdscdmadiag_manual_eul_ul_var_upd_handler

DESCRIPTION
  This function handles request to manually update eul uplink module variables.
  Right now this handler is used to enable or disable eul pwr restriction.

============================================================================*/
PACKED void * tdscdmadiag_manual_eul_ul_var_upd_handler(
   PACKED void *req_ptr,
   uint16       pkt_len
)
{
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_req_type *upd_req_ptr;
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_rsp_type *upd_rsp_ptr;

   upd_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_req_type *) req_ptr;

   /* Prepare the response packet */
   upd_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_rsp_type *)
                         diagpkt_alloc(
                            diagpkt_get_cmd_code( req_ptr ),
                            sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_rsp_type)
                         );

   if ( upd_rsp_ptr == NULL )
   {
      /* Allocation failed, and diagpkt_err_rsp will fail as well.
      ** So just return NULL.
      */
      return NULL;
   }

   /* Copy the header from the request packet to the response packet */
   (void)memscpy(
      (void*)upd_rsp_ptr,
	  sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DIAG_EUL_UL_VAR_UPD_F_rsp_type),
      (void*)upd_req_ptr,
      sizeof(diagpkt_subsys_header_type)
   );
   
   /* Generate the response */
   upd_rsp_ptr->status = 1;

   /* Call the eul uplink function that will take the values from diag request packet
      and update the variable */
   tdseul_set_var_through_qxdm(
         upd_req_ptr->debug_enable_f3_message_type,upd_req_ptr->debug_var_val);

   return (void *) upd_rsp_ptr;

} 
#endif /*FEATURE_TDSCDMA_HSUPA*/

/*===========================================================================

FUNCTION tdscdmadiag_dec_debug_dump_handler

DESCRIPTION
   This function handles the request to dump TBset data. It does a basic sanity
   of the input parameters, converts the PACKEd structure to unpacked and sends
   it to decoder for marking what needs to be dumped.
  
============================================================================*/
PACKED void * tdscdmadiag_dec_debug_dump_handler(
   PACKED void *req_ptr,
   uint16       pkt_len
)
{
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_req_type *config_req_ptr;
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_rsp_type *config_rsp_ptr;
   /* To convert from PACKED struct to unpacked */
   #if 0
   uint8 index;
   uint8 cctrch_ref_type[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
   uint8 trch_ref_type[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
   uint16 tb_size[TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM];
   #endif

   config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_req_type *) req_ptr;

   /* A basic error checking on the received values */
   if ((config_req_ptr->dump_state > TDSCDMA_DEC_MAX_DUMP_STATE) ||
       (config_req_ptr->num_cctrch_ref_type > TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM) ||
       (config_req_ptr->num_trch_ref_type > TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM)   ||
       (config_req_ptr->num_tb_size > TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM))
   {
      return diagpkt_err_rsp(
         DIAG_BAD_PARM_F,
         config_req_ptr,
         sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_req_type)
         );
   }

   /* Prepare the response packet */
   config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_rsp_type *)
                         diagpkt_alloc(
                            diagpkt_get_cmd_code( req_ptr ),
                            sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_rsp_type)
                         );

   if ( config_rsp_ptr == NULL )
   {
      /* Allocation failed, and diagpkt_err_rsp will fail as well.
      ** So just return NULL.
      */
      return NULL;
   }

   /* Copy the header from the request packet to the response packet */
   (void)memscpy(
      (void*)config_rsp_ptr,
	  sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_DEC_TBSET_DUMP_F_rsp_type),
      (void*)config_req_ptr,
      sizeof(diagpkt_subsys_header_type)
   );
   
   /* Generate the response */
   config_rsp_ptr->status = 1;

#if 0
   /* Convert PACKED structure to unpacked to pass in as arguments to decoder function */
   for (index = 0; index < config_req_ptr->num_cctrch_ref_type; index++)
   {
     cctrch_ref_type[index] = config_req_ptr->cctrch_ref_type[index];
   }
   for (index = 0; index < config_req_ptr->num_trch_ref_type; index++)
   {
     trch_ref_type[index] = config_req_ptr->trch_ref_type[index];
   }
   for (index = 0; index < config_req_ptr->num_tb_size; index++)
   {
     tb_size[index] = config_req_ptr->tb_size[index];
   }
   /* Call the decoder function to set the tbset dump information */
   /*zhengming: we don't need this now.*/
   mcaltdscdma_dec_process_tbset_dump_cmd(
         config_req_ptr->dump_state,
         config_req_ptr->num_cctrch_ref_type,
         cctrch_ref_type,
         config_req_ptr->num_trch_ref_type,
         trch_ref_type,
         config_req_ptr->num_tb_size,
         tb_size);
 
   #endif 
   
   #if 0
   if (index < TDSCDMA_DEC_TBSET_DUMP_MAX_PARAM)
   {
         tb_size[index] = config_req_ptr->tb_size[index];
   }

   /* Call the decoder function to set the tbset dump information */
   mcaltdscdma_dec_process_tbset_dump_cmd(
         config_req_ptr->dump_state,
         config_req_ptr->num_cctrch_ref_type,
         cctrch_ref_type,
         config_req_ptr->num_trch_ref_type,
         trch_ref_type,
         config_req_ptr->num_tb_size,
         tb_size);
#endif
   return (void *) config_rsp_ptr;
} 


#if defined(FEATURE_TDSCDMA_OFFLINE_MODEM)
/*===========================================================================

FUNCTION tdscdmadiag_tdscdma_cqi_params_handler

DESCRIPTION
  This function handles the request to manually set cqi params.

============================================================================*/
PACKED void * tdscdmadiag_tdscdma_cqi_params_handler(
   PACKED void *req_ptr,
   uint16       pkt_len
)
{
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_req_type *config_req_ptr;
   DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_rsp_type *config_rsp_ptr;

   config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_req_type *) req_ptr;

   /* A basic error checking on the received values */
   if (config_req_ptr->override > 1)
   {
      return diagpkt_err_rsp(
         DIAG_BAD_PARM_F,
         config_req_ptr,
         sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_req_type)
         );
   }

   /* Prepare the response packet */
   config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_rsp_type *)
                         diagpkt_alloc(
                            diagpkt_get_cmd_code( req_ptr ),
                            sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_rsp_type)
                         );

   if ( config_rsp_ptr == NULL )
   {
      /* Allocation failed, and diagpkt_err_rsp will fail as well.
      ** So just return NULL.
      */
      return NULL;
   }

   /* Copy the header from the request packet to the response packet */
   (void)memscpy(
      (void*)config_rsp_ptr,
	  sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_CQI_OVERIDE_PARAMS_F_rsp_type),
      (void*)config_req_ptr,
      sizeof(diagpkt_subsys_header_type)
   );
   
   /* Generate the response */
   config_rsp_ptr->status = 1;
  #ifdef FEATURE_QDSP6 
  /*commented out by owen
	  tdsenchs_set_cqi_override_from_diag(
      config_req_ptr->override,
      config_req_ptr->pattern_indicator,
      config_req_ptr->repetition_cycle_repeat_count,
      config_req_ptr->pattern_len,
 	  config_req_ptr->test_pattern);
   */
  #endif
   return (void *) config_rsp_ptr;

} 
#endif

#if defined __cplusplus
  }
#endif

/*===========================================================================
FUNCTION        TL1_GET_FRAME_NUMBER_FOR_DIAG

DESCRIPTION     This function returns the frame number for diag

PARAMETER       Void.

RETURN VALUE    uint32 value.
===========================================================================*/
uint32 tl1_get_frame_number_for_diag(void)
{

  uint32 frame_number;

  if ( tdsl1m_is_connected() )
  {
    frame_number = tdsseq_get_cfn();
  }
  else
  {
    frame_number = tdsseq_get_sfn();
  }

  return frame_number;
}

/**************************************************************************
** RRC diag DISPATCH TABLE.
***************************************************************************/

static const diagpkt_user_table_entry_type tdsdiag_tbl[] =
{
     {TDSCDMA_DIAG_VERNO_F, TDSCDMA_DIAG_VERNO_F, tdscdmadiag_verno}
     ,{TDSCDMA_DIAG_RRC_GET_STATE_F, TDSCDMA_DIAG_RRC_GET_STATE_F, tdscdmadiag_rrc_get_state_handler}
  ,{TDSCDMA_DIAG_BLER_RATE_CHANGE_F, TDSCDMA_DIAG_BLER_RATE_CHANGE_F, tdscdmadiag_change_bler_rate}
     ,{TDSCDMA_DIAG_ORIG_F, TDSCDMA_DIAG_ORIG_F, tdscdmadiag_call_orig}
    ,{TDSCDMA_DIAG_END_F, TDSCDMA_DIAG_END_F, tdscdmadiag_call_end}
  ,{TDSCDMA_STATUS_F, TDSCDMA_STATUS_F, tdscdmadiag_status}
  ,{TDSCDMA_ADDITIONAL_STATUS_F, TDSCDMA_ADDITIONAL_STATUS_F, tdscdmadiag_additional_status}
  ,{TDSCDMA_MANUAL_CQI_UPD_CONFIG_F, TDSCDMA_MANUAL_CQI_UPD_CONFIG_F, tdscdmadiag_manual_cqi_upd_config_handler}
  ,{TDSCDMA_DIAG_EUL_UL_VAR_UPD_F,TDSCDMA_DIAG_EUL_UL_VAR_UPD_F,tdscdmadiag_manual_eul_ul_var_upd_handler}
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  ,{TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F, TDSCDMA_RLC_FLOW_CONTROL_CONFIG_F, tdscdmadiag_rlc_flow_control_config_handler}
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  ,{TDSCDMA_DIAG_TDSCDMA_STATS_F, TDSCDMA_DIAG_TDSCDMA_STATS_F, tdscdmadiag_tdscdma_stats}
  ,{TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F, TDSCDMA_DIAG_TDSCDMA_CLEAR_STATS_F, tdscdmadiag_clear_tdscdma_stats}
,{TDSCDMA_DEC_TBSET_DUMP_F, TDSCDMA_DEC_TBSET_DUMP_F, tdscdmadiag_dec_debug_dump_handler}
  ,{TDSCDMA_CQI_OVERIDE_PARAMS_F, TDSCDMA_CQI_OVERIDE_PARAMS_F, tdscdmadiag_tdscdma_cqi_params_handler}  
  ,{TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F, TDSCDMA_RXD_ENABLE_DSABLE_PARAMS_F, tdscdmadiag_rxd_enable_disable_handler}  
    
  #ifdef FEATURE_TDSCDMA_PLT
  /*zhengming: add a DIAG command for TPLT*/  
  ,{TDSCDMA_PLT_COMMAND_F, TDSCDMA_PLT_COMMAND_F, tdscdmadiag_plt_command_handler}
  ,{TDSCDMA_PLT_RESULT_F, TDSCDMA_PLT_RESULT_F, tdscdmadiag_plt_result_handler}
  #endif
  #ifdef FEATURE_TDSCDMA_PLT
  ,{TDSCDMA_PLT_MODE_F, TDSCDMA_PLT_MODE_F, tdscdmadiag_tplt_mode_handler}
  #endif
};

static const diagpkt_user_table_entry_type tdsdiag_tbl_v2[] =
{   
  #ifdef FEATURE_TDSCDMA_PLT
  /*zhengming: add a DIAG command for TPLT*/  
  {TDSCDMA_PLT_COMMAND_F, TDSCDMA_PLT_COMMAND_F, tdscdmadiag_plt_command_handler_v2}
  #endif
  #ifdef FEATURE_TDSCDMA_PLT
  //,{TDSCDMA_PLT_MODE_F, TDSCDMA_PLT_MODE_F, tdscdmadiag_tplt_mode_handler_v2}
  #endif
};



#ifdef __cplusplus

  TDSDIAGPKT_DISPATCH_AUTOREGISTER ((word)DIAG_SUBSYS_TDSCDMA, tdsdiag_tbl);

  DIAGPKT_DISPATCH_AUTOREGISTER_DELAY ((word)DIAG_SUBSYS_TDSCDMA, tdsdiag_tbl_v2);

#endif

void tdsdiag_init (void)
{
#ifndef __cplusplus

   DIAGPKT_DISPATCH_TABLE_REGISTER ((word)DIAG_SUBSYS_TDSCDMA, tdsdiag_tbl);

   DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY_PROC(DIAG_MODEM_PROC, DIAG_SUBSYS_CMD_VER_2_F, DIAG_SUBSYS_TDSCDMA, tdsdiag_tbl_v2);

     /*log for received diag command*/
  #ifdef FEATURE_TDSCDMA_PLT
  tplt_diag_cmd_logs_idx = 0;
  #endif
#endif
}


#ifdef FEATURE_TDSCDMA_PLT
/*===========================================================================

FUNCTION tdscdmadiag_rxd_enable_disable_handler

DESCRIPTION
  This function handles the request to enable disable CQI offset.

============================================================================*/

PACKED void * tdscdmadiag_tplt_mode_handler(PACKED void *req_ptr, uint16 pkt_len)
{

  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_MODE_F_req_type *config_req_ptr;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_MODE_F_rsp_type *config_rsp_ptr;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_MODE_F_rsp_type);
  config_req_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_MODE_F_req_type *) req_ptr;

  TDSL1_MSG0(HIGH,"MR_DBG: executing tdscdmadiag_tplt_mode_handler");
  
  /* Prepare the response packet */
  config_rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_MODE_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_MODE_F, rsp_len );

  if ( config_rsp_ptr == NULL )
  {
     /* Allocation failed, and diagpkt_err_rsp will fail as well.
     ** So just return NULL.
     */
     ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_MODE_F, rsp_len);
  }

  /* Copy the header from the request packet to the response packet */
  (void)memscpy(
     (void*)config_rsp_ptr,
	 rsp_len,
     (void*)config_req_ptr,
     sizeof(diagpkt_subsys_header_type)
  );

  /* Generate the response */
  config_rsp_ptr->status = 1;

  /* Call the RxDiv function that will take the values from diag request packet
      and update the RxDiv On Off flag */

  if( (tdsl1m_global.l1_state == TDSL1M_IDLE) || (tdsl1m_global.l1_state == TDSL1M_STOPPED) )
  {
    TDSL1_MSG1(HIGH,"MR_DBG: setting tplt mode to TRUE, tdsl1_state: %d",tdsl1m_global.l1_state);
    tplt_set_plt_mode(config_req_ptr->tplt_mode);
  }
  else
  {
    TDSL1_MSG1(HIGH,"MR_DBG: L1 state is not idle. tdsl1_state: %d",tdsl1m_global.l1_state );
  }

  return (void *) config_rsp_ptr;

}

#endif

/*zhengming for TPLT*/
#ifdef FEATURE_TDSCDMA_PLT

/*===========================================================================
Function: tdscdmadiag_plt_start_tdscdma_mode_handler
Description:

  This function processes the TDSCDMA specific TDSCDMA_PLT_START_TDSCDMA_MODE_F
  command, which requests TL1 to start TDSCDMA mode.

  This function is called when "send_data 75 87 40 0" is issued from cmd 
  line interface from QXDM

  Where the 75 is DIAG_SUBSYS_CMD_F {Subssytem dispatcher (extended diag cmd)}
  The 87 is DIAG_SUBSYS_TDSCDMA
  and the 40 is for TDSCDMA_PLT_START_TDSCDMA_MODE_F, which is the function we are using.  

============================================================================*/
PACKED void *tdscdmadiag_plt_command_handler (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *req_ptr = 
    (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *)req_pkt;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_type *rsp_ptr;
  uint32 cmd_id;
  /*uint16 subsys_id;
  uint16 sybsys_cmd_code;
  uint16 cmd_code;*/

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_type);

  cmd_id = (uint16)req_ptr->cmd_id;
  TDSL1_MSG2(HIGH,"TPLT receives cmd mode from DIAG: %d, %d",cmd_id,req_ptr->cmd_id);

  //MSG_HIGH("TPLT receives message header: %d, value %d", req_ptr->xx_header, 
    //       req_ptr->params.start_param.tds_mode, 0);

  //cmd_code  = req_ptr->xx_header & 0xFF;
  //subsys_id = (req_ptr->xx_header & 0xFF00)>>8;
  //sybsys_cmd_code = req_ptr->xx_header >>16;

   //MSG_HIGH("TPLT receives message header: subsys_id %d, cmd_code %d, subsys_cmd_code %d", 
     //       subsys_id, cmd_code, sybsys_cmd_code);

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, rsp_len );
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, rsp_len);
  }
  else
  {
    switch (cmd_id)
    {
        case TPLT_CMD_START_MODE:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_START_TDSCDMA_MODE from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_start_tdscdma_mode();   
            /*log the received command*/               
            break;
        case TPLT_CMD_STOP_MODE:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_START_TDSCDMA_MODE from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_stop_tdscdma_mode(&req_ptr->params.stop_param);   
            /*log the received command*/               
            break;	    
        case TPLT_CMD_RESET_PLT:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_RESET_PLT from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_reset_plt();   
            /*log the received command*/               
            break;
         case TPLT_CMD_ACTIVATE_CHAN:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ACTIVATE_CHAN from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_activate_chan_cmd_proc();   
            /*log the received command*/               
            break;
        case TPLT_CMD_ACQ: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ACQ from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_acq(&req_ptr->params.acq_param);                  
            }
            break;
        case TPLT_CMD_DL_CONFIG: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_DL_CONFIG from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_dl_config(&req_ptr->params.dl_config_param);                  
            }
            break;
        case TPLT_CMD_UL_CONFIG: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_CONFIG from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_config(&req_ptr->params.ul_config_param);                  
            }
            break;
        case TPLT_CMD_UL_TRCH_DATA:
            {
    	          tplt_save_diag_cmd_log(cmd_id, req_ptr);
    	          TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_TRCH_DATA from DIAG");
    	          rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_trch_data(&req_ptr->params.ul_trch_data);
            }
            break;
        case TPLT_CMD_UL_HW_RAM_DATA:
    	      {
    	         tplt_save_diag_cmd_log(cmd_id, req_ptr);
    	         TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_HW_RAM_DATA from DIAG");
    	         rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_hw_ram_data(&req_ptr->params.ul_hw_ram_data);
    	      }
            break;
        case TPLT_CMD_ERUCCH_TEST_CONFIG:
    	     {
    		       tplt_save_diag_cmd_log(cmd_id, req_ptr);
    		       TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ERUCCH_TEST_CONFIG from DIAG");
    		       rsp_ptr->tplt_status = (uint8)tplt_erucch_test_config_cmd_proc(&req_ptr->params.erucch_test_config);
    	     }
           break;
          case TPLT_CMD_STMR:
          {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_STMR from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_cmd_stmr(&req_ptr->params.stmr_param);
          }
          break;		    	
        case TPLT_CMD_ASET_UPDATE:
    	    {
    		     tplt_save_diag_cmd_log(cmd_id, req_ptr);
    		     TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ASET_UPDATE from DIAG");
    		     rsp_ptr->tplt_status = (uint8)tplt_aset_update_cmd(&req_ptr->params.aset_update_data);
    	    }
    	    break;

        case TPLT_CMD_HSUPA_CONFIG:
            {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UPA_ENCODER_TEST from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_cmd_hsupa_config(&req_ptr->params.upa_encoder_config);
            }
            break;

        case TPLT_CMD_UPA_ENCODER_TEST:
            {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UPA_ENCODER_TEST from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_cmd_upa_encoder_proc(&req_ptr->params.upa_encoder_params);
            }
            break;
    
        case TPLT_CMD_UPA_ENCODER_DATA:
            {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UPA_ENCODER_DATA from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_cmd_upa_encoder_proc_vector(&req_ptr->params.upa_encoder_data);
            }
            break;

		case TPLT_CMD_DROP_ALL:
          	{
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_DROP_ALL from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_drop_all_cmd_proc();
          	}
          	break;
        //L2T TPLT commands
		case TPLT_CMD_L2T_IRAT_INIT:
		    {
				tplt_save_diag_cmd_log(cmd_id, req_ptr);
				TDSL1_MSG1(HIGH,"TPLT receives TPLT_CMD_L2T_IRAT_INIT from DIAG: dummy %d", req_ptr->params.l2t_init_data.dummy);
				rsp_ptr->tplt_status = (uint8)tplt_diag_l2t_init_cmd_proc();
			}
		break;

		case TPLT_CMD_L2T_IRAT_ACQ_REQ:
		{
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_L2T_IRAT_ACQ_REQ from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_diag_l2t_acq_cmd_proc(&req_ptr->params.l2t_acq_data);
			TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
			break;
		}

		case TPLT_CMD_L2T_IRAT_MEAS_REQ:
		{
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_L2T_IRAT_MEAS_REQ from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_diag_l2t_meas_cmd_proc(&req_ptr->params.l2t_meas_data);
			TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
			
			break;
		}

        //G2T IRAT PLT commands
		case TPLT_CMD_G2T_IRAT_START:
	    {
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_G2T_IRAT_START from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_cmd_g2t_irat_start();
	    }
		break;

		case TPLT_CMD_G2T_IRAT_ACQ:
		{
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_G2T_IRAT_ACQ from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_cmd_g2t_irat_acq(&req_ptr->params.g2t_irat);
			TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
			break;
		}

		case TPLT_CMD_G2T_IRAT_MEAS:
		{
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_G2T_IRAT_MEAS from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_cmd_g2t_irat_meas(&req_ptr->params.g2t_irat);
			TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);			
			break;
		}

        case TPLT_CMD_G2T_IRAT_STOP:
		{
			tplt_save_diag_cmd_log(cmd_id, req_ptr);
			TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_G2T_IRAT_STOP from DIAG");
			rsp_ptr->tplt_status = (uint8)tplt_cmd_g2t_irat_stop();
			TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);			
			break;
		}


        case TPLT_CMD_DRX:
        {
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_DRX from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_drx_cmd_proc(&req_ptr->params.drx_data);
        }
        case TPLT_CMD_HS_CONFIG:
        {
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_HS_CONFIG from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_hsdpa_config(&req_ptr->params.hsdpa_config);
            TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
            break;
        }
        case TPLT_CMD_DECIPHER:
        {
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_CIPHERING from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_ciphering(&req_ptr->params.ciphering_cmd);
            TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
            break;
        }
        case TPLT_CMD_UL_TEST_CTRL:
        {
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_TEST_CTRL from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_diag_ul_test_ctrl_cmd_proc(&req_ptr->params.ul_test_ctrl);
            TDSL1_MSG1(HIGH,"TPLT DIAG response to be sent back to DIAG: status %d", rsp_ptr->tplt_status);
        }
        default:
            TDSL1_MSG1(HIGH,"Error: TPLT receives unknown command %d from DIAG", cmd_id);
          break;
           // rsp_ptr->tplt_status = (uint8)TPLT_CMD_START_MODE();
    }    
  }
  return rsp_ptr;
}
#ifdef FEATURE_TDSCDMA_PLT
/*===========================================================================
Function: tdscdmadiag_plt_result_handler
Description:

  This function processes the TDSCDMA specific TDSCDMA_PLT_RESULT_F
  command, which requests the result for sepecific command from store.

  This function is called when "send_data 75 87 41 0" is issued from cmd 
  line interface from QXDM

  Where the 75 is DIAG_SUBSYS_CMD_F {Subssytem dispatcher (extended diag cmd)}
  The 87 is DIAG_SUBSYS_TDSCDMA
  and the 41 is for TDSCDMA_PLT_RESULT_F, which is the function we are using.  

============================================================================*/
PACKED void *tdscdmadiag_plt_result_handler (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_RESULT_F_req_type  *req_ptr = 
    (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_RESULT_F_req_type  *)req_pkt;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_RESULT_F_rsp_type *rsp_ptr;
  uint32 cmd_id;

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_RESULT_F_rsp_type);

  cmd_id = (uint16)req_ptr->cmd_id;
  TDSL1_MSG2(HIGH, "TPLT receives result cmd from DIAG: %d, %d",cmd_id,req_ptr->cmd_id);

  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_RESULT_F_rsp_type *)
              diagpkt_subsys_alloc(DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_RESULT_F, rsp_len );	
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
    DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_RESULT_F, rsp_len);
    return rsp_ptr;
  }

    switch (cmd_id)
    {
      case TPLT_CMD_START_MODE:
      {
        TDSL1_MSG0(HIGH, "TPLT receives TPLT_CMD_START_TDSCDMA_MODE result request from DIAG");
        rsp_ptr->cmd_id = TPLT_CMD_START_MODE;
        rsp_ptr->rsp_result.start_param = tplt_result.start;
        return rsp_ptr;
      }
      case TPLT_CMD_ACQ: 
      {
        TDSL1_MSG0(HIGH, "TPLT receives TPLT_CMD_ACQ result request from DIAG ");    
        rsp_ptr->cmd_id = TPLT_CMD_ACQ;
        rsp_ptr->rsp_result.acq_param = tplt_result.acq;
        return rsp_ptr;
      }
      case TPLT_CMD_DL_CONFIG: 
      {
        TDSL1_MSG0(HIGH, "TPLT receives TPLT_CMD_DL_CONFIG result request from DIAG ");    
        rsp_ptr->cmd_id = TPLT_CMD_DL_CONFIG;
        tplt_result_dl_config_update();
        rsp_ptr->rsp_result.dl_channel_crc = tplt_result.dl_chan;
        return rsp_ptr;
      }
      default:
        TDSL1_MSG1(HIGH, "Error: TPLT receives unknown command %d from DIAG", cmd_id);
      break;
    }
  return rsp_ptr;
}
#endif
/*===========================================================================
Function: tdscdmadiag_plt_start_tdscdma_mode_handler
Description:

  This function processes the TDSCDMA specific TDSCDMA_PLT_START_TDSCDMA_MODE_F
  command, which requests TL1 to start TDSCDMA mode.

  This function is called when "send_data 75 87 40 0" is issued from cmd 
  line interface from QXDM

  Where the 75 is DIAG_SUBSYS_CMD_F {Subssytem dispatcher (extended diag cmd)}
  The 87 is DIAG_SUBSYS_TDSCDMA
  and the 40 is for TDSCDMA_PLT_START_TDSCDMA_MODE_F, which is the function we are using.  

============================================================================*/
uint32 tdscdmadiag_plt_command_delay_rsp_id = 0;

PACKED void *tdscdmadiag_plt_command_handler_v2 (PACKED void* req_pkt, uint16 pkt_len)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *req_ptr = 
    (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *)req_pkt;
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type *rsp_ptr;
  uint32 cmd_id;

  //uint32 delayed_rsp_id = 0;  
  /*uint16 subsys_id;
  uint16 sybsys_cmd_code;
  uint16 cmd_code;*/

  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type);

  cmd_id = (uint16)req_ptr->cmd_id;
  TDSL1_MSG2(HIGH,"TPLT receives cmd mode from DIAG: %d, %d",cmd_id,req_ptr->cmd_id);

  //MSG_HIGH("TPLT receives message header: %d, value %d", req_ptr->xx_header, 
      //cmd_id = (uint16) (diag_routing_header >> 16);  

  //cmd_code  = req_ptr->xx_header & 0xFF;
  //subsys_id = (req_ptr->xx_header & 0xFF00)>>8;
  //sybsys_cmd_code = req_ptr->xx_header >>16;

   //MSG_HIGH("TPLT receives message header: subsys_id %d, cmd_code %d, subsys_cmd_code %d", 
     //       subsys_id, cmd_code, sybsys_cmd_code);

  /* Allocate the memory for this */
  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type *)
              diagpkt_subsys_alloc_v2(DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, rsp_len );

  
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, rsp_len);
  }
  else
  {
  
    tdscdmadiag_plt_command_delay_rsp_id = rsp_ptr->delayed_rsp_id;
    switch (cmd_id)
    {
        case TPLT_CMD_START_MODE:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_START_TDSCDMA_MODE from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_start_tdscdma_mode();   
            /*log the received command*/               
            break;
        case TPLT_CMD_STOP_MODE:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_STOP_TDSCDMA_MODE from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_stop_tdscdma_mode(&req_ptr->params.stop_param);   
            /*log the received command*/               
            break;	    
        case TPLT_CMD_RESET_PLT:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_RESET_PLT from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_cmd_reset_plt();   
            /*log the received command*/               
            break;
         case TPLT_CMD_ACTIVATE_CHAN:
            tplt_save_diag_cmd_log(cmd_id, req_ptr);
            TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ACTIVATE_CHAN from DIAG");
            rsp_ptr->tplt_status = (uint8)tplt_activate_chan_cmd_proc();   
            /*log the received command*/               
            break;
        case TPLT_CMD_ACQ: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ACQ from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_acq(&req_ptr->params.acq_param);                  
            }
            break;
        case TPLT_CMD_DL_CONFIG: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_DL_CONFIG from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_dl_config(&req_ptr->params.dl_config_param);                  
            }
            break;
        case TPLT_CMD_UL_CONFIG: 
            {
                tplt_save_diag_cmd_log(cmd_id, req_ptr);
                TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_CONFIG from DIAG");
                rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_config(&req_ptr->params.ul_config_param);                  
            }
            break;
        case TPLT_CMD_UL_TRCH_DATA:
            {
    	          tplt_save_diag_cmd_log(cmd_id, req_ptr);
    	          TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_TRCH_DATA from DIAG");
    	          rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_trch_data(&req_ptr->params.ul_trch_data);
            }
            break;
        case TPLT_CMD_UL_HW_RAM_DATA:
    	      {
    	         tplt_save_diag_cmd_log(cmd_id, req_ptr);
    	         TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_UL_HW_RAM_DATA from DIAG");
    	         rsp_ptr->tplt_status = (uint8)tplt_cmd_ul_hw_ram_data(&req_ptr->params.ul_hw_ram_data);
    	      }
            break;
        case TPLT_CMD_ERUCCH_TEST_CONFIG:
    	     {
    		       tplt_save_diag_cmd_log(cmd_id, req_ptr);
    		       TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ERUCCH_TEST_CONFIG from DIAG");
    		       rsp_ptr->tplt_status = (uint8)tplt_erucch_test_config_cmd_proc(&req_ptr->params.erucch_test_config);
    	     }
           break;
          case TPLT_CMD_STMR:
          {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_STMR from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_cmd_stmr(&req_ptr->params.stmr_param);
          }
          break;		    	
          case TPLT_CMD_ASET_UPDATE:
          {
              tplt_save_diag_cmd_log(cmd_id, req_ptr);
              TDSL1_MSG0(HIGH,"TPLT receives TPLT_CMD_ASET_UPDATE from DIAG");
              rsp_ptr->tplt_status = (uint8)tplt_aset_update_cmd_proc(&req_ptr->params.aset_update_data);
          }
          break;
          default:
            TDSL1_MSG1(HIGH,"Error: TPLT receives unknown command %d from DIAG", cmd_id);
          break;
           // rsp_ptr->tplt_status = (uint8)TPLT_CMD_START_MODE();
    }    
  }
  diagpkt_commit(rsp_ptr);
  return NULL;
}
             // rsp_ptr->tplt_status = (uint8)TPLT_CMD_START_MODE();
void tdscdmadiag_plt_command_delay_rsp(tplt_client_cmd_enum_type cmd_id, boolean status)
{
  DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type* rsp_ptr;
  const int rsp_len = sizeof(DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type);

  TDSL1_MSG2(HIGH,"TPLT receives delay rsp cmd %d with status %d",cmd_id,status);

  rsp_ptr = (DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_rsp_v2_type *)diagpkt_subsys_alloc_v2_delay(
                                                                   DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, 
                                                                   tdscdmadiag_plt_command_delay_rsp_id, 
                                                                   rsp_len);
  if (!rsp_ptr)
  {
    ERR_FATAL("diagpkt_subsys_alloc(%d, %d, %d) returned NULL!", 
              DIAG_SUBSYS_TDSCDMA, TDSCDMA_PLT_COMMAND_F, rsp_len);
  }
  else
  {  
    if (tdscdmadiag_plt_command_delay_rsp_id == rsp_ptr->delayed_rsp_id)
    {
      TDSL1_MSG2(ERROR,"tplt delay rsp previous rsp id %d diff than curr rsp id %d", tdscdmadiag_plt_command_delay_rsp_id,rsp_ptr->delayed_rsp_id);
    }
    rsp_ptr->response_cnt = 1;
    rsp_ptr->tplt_status = status;
    diagpkt_delay_commit(rsp_ptr);
  }
}

/*for saving received DIAG commands*/
extern void tplt_save_diag_cmd_log(uint32 cmd_id, DIAG_SUBSYS_TDSCDMA_TDSCDMA_PLT_COMMAND_F_req_type  *req_ptr)
{
    if(!req_ptr)
    {
        ERR_FATAL("tplt_save_diag_cmd_log: Null req_ptr!", 0, 0, 0);
    }

    tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].cmd_id = cmd_id;
    switch (cmd_id)
    {
        case TPLT_CMD_START_MODE:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.start_param = req_ptr->params.start_param;
            break;
        case TPLT_CMD_RESET_PLT:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.reset_param = req_ptr->params.reset_param;
            break;
        case TPLT_CMD_ACTIVATE_CHAN:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.activate_param = req_ptr->params.activate_param;
            break;
        case TPLT_CMD_ACQ:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.acq_param = req_ptr->params.acq_param;
            break;
        case TPLT_CMD_DL_CONFIG:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.dl_config_param = req_ptr->params.dl_config_param;
            break;
        case TPLT_CMD_UL_CONFIG:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.ul_config_param = req_ptr->params.ul_config_param;
            break;
        case TPLT_CMD_UL_TRCH_DATA:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.ul_trch_data = req_ptr->params.ul_trch_data;
            break;
        case TPLT_CMD_UL_HW_RAM_DATA:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.ul_hw_ram_data = req_ptr->params.ul_hw_ram_data;
            break;
        case TPLT_CMD_ERUCCH_TEST_CONFIG:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.erucch_test_config = req_ptr->params.erucch_test_config;
            break;
        case TPLT_CMD_ASET_UPDATE:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.aset_update_data = req_ptr->params.aset_update_data;
            break;
        case TPLT_CMD_STMR:
            tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.stmr_param = req_ptr->params.stmr_param;
            break;            
        case TPLT_CMD_HSUPA_CONFIG:
          tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.upa_encoder_config = req_ptr->params.upa_encoder_config;
          break;
        case TPLT_CMD_UPA_ENCODER_TEST:
          tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.upa_encoder_params = req_ptr->params.upa_encoder_params;
          break;
        case TPLT_CMD_UPA_ENCODER_DATA:
          tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.upa_encoder_data = req_ptr->params.upa_encoder_data;
          break;

        case TPLT_CMD_HS_CONFIG:
          tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.hsdpa_config = req_ptr->params.hsdpa_config;
          break;

        case TPLT_CMD_L2T_IRAT_INIT:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.l2t_init_data = req_ptr->params.l2t_init_data;
			break;
		case TPLT_CMD_L2T_IRAT_ACQ_REQ:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.l2t_acq_data = req_ptr->params.l2t_acq_data;
			break;
		case TPLT_CMD_L2T_IRAT_MEAS_REQ:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.l2t_meas_data = req_ptr->params.l2t_meas_data;
			break;

        case TPLT_CMD_G2T_IRAT_START:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.g2t_irat = req_ptr->params.g2t_irat;
			break;
		case TPLT_CMD_G2T_IRAT_ACQ:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.g2t_irat = req_ptr->params.g2t_irat;
			break;
		case TPLT_CMD_G2T_IRAT_MEAS:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.g2t_irat = req_ptr->params.g2t_irat;
			break;
        case TPLT_CMD_G2T_IRAT_STOP:
			tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.g2t_irat = req_ptr->params.g2t_irat;
			break;
        case TPLT_CMD_UL_TEST_CTRL:
          tplt_diag_cmd_logs[tplt_diag_cmd_logs_idx].params.ul_test_ctrl = req_ptr->params.ul_test_ctrl;
			    break;  
          
        default:
            TDSL1_MSG1(HIGH,"tplt_save_diag_cmd_log(): unknown diag command type %d", cmd_id);
    }
        tplt_diag_cmd_logs_idx = (tplt_diag_cmd_logs_idx+1)%TPLT_MAX_DIAG_CMD_LOG;
}
#endif

#endif  /* FEATURE_TDSCDMA */
