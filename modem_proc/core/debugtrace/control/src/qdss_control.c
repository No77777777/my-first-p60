/*=============================================================================

FILE:         qdss_control.c

DESCRIPTION:  Implements handlers for diag comamnds to configure and control
              SSC QDSS.

================================================================================
              Copyright (c) 2012 Qualcomm Technologies Incorporated.
                         All Rights Reserved.
                QUALCOMM Proprietary and Confidential
==============================================================================*/
#include "qdss_control_priv.h"
#include "qdss_control.h"
#include "wcss_dbg_csr.h"
#include "qdss_diag.h"
#include "DALDeviceId.h"
#include "qdss.h"
#include "qdss_tfunnel.h"
#include "dll_global_def.h"


struct qdss_control qdss;


/*-------------------------------------------------------------------------*/
int qdss_enable_local_etb(void) 
{
   int nErr;
   int tmc_mode = TMC_MODE_HW_FIFO;
   
   if ( (qdss.current_trace_sink == QDSS_TRACESINK_CBUF) ||
        (qdss.current_trace_sink == QDSS_TRACESINK_HWFIFO)) {
      tmc_mode = qdss.current_trace_sink;
   }
   else {
      qdss.current_trace_sink = QDSS_TRACESINK_HWFIFO;
      tmc_mode = TMC_MODE_HW_FIFO;
   }
   TRY(nErr,DalTMC_SetMode(qdss.hTMC,tmc_mode));
   TRY(nErr,DalTMC_EnableTrace(qdss.hTMC));

   CATCH(nErr){}

   return nErr;
}

/*-------------------------------------------------------------------------*/

#define _TRY(nErr, func)  (func)


int qdss_wcss_dbg_clk_on(void)
{
   int nErr;

   if (NULL==qdss.clk.hClock) {
      TRY(nErr,DAL_ClockDeviceAttach(DALDEVICEID_CLOCK, 
                                     &qdss.clk.hClock));

      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_apb_clk",
                                   &qdss.clk.wcss_dbg_apb_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_apb_clk_id));


      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_atb_clk",
                                   &qdss.clk.wcss_dbg_atb_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_atb_clk_id));

      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_ts_clk",
                                   &qdss.clk.wcss_dbg_ts_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_ts_clk_id));


      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_css_apb_clk",
                                   &qdss.clk.wcss_dbg_css_apb_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_css_apb_clk_id));


      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_css_atb_clk",
                                   &qdss.clk.wcss_dbg_css_atb_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_css_atb_clk_id));

      TRY(nErr,DalClock_GetClockId(qdss.clk.hClock,
                                   "wcss_dbg_css_ts_clk",
                                   &qdss.clk.wcss_dbg_css_ts_clk_id));

      _TRY(nErr,DalClock_EnableClock(qdss.clk.hClock,qdss.clk.wcss_dbg_css_ts_clk_id));
   }

   nErr = DAL_SUCCESS;

   CATCH(nErr) {}

   return nErr;
}


/*-------------------------------------------------------------------------*/

/**
  @brief  Ensure QDSS HW is ready to be configure

 This function votes QDSS clocks on, attach to drivers and init hardware
 If this is already done this function returns success

  @return 0 if successful, error code otherwise
      QDSS_CONTROL_SUCCESS = Successful.
      QDSS_CONTROL_BAD_STATE = A handle is not valid.
      QDSS_CONTROL_FUSE_BLOWN = QDSS fuse is blown.
 */
int qdss_ensure_hw_ready(void)
{
   int nErr;


   if ((NULL == qdss.hTMC)||(NULL == qdss.hFunnel)||
       (NULL == qdss.hTPDM)||(NULL == qdss.hTPDA)) {

      //TODO:wcss_dbg clocks are now enabled by default on boot-up in
      //core/power/ccpm_r/src/hwio/ccpm_clk_config/src/ccpmr_pm_clk.c
      //If this goes away will need to enable debug clocks here

      if (NULL == qdss.hTMC) {
         TRY(nErr, DAL_TMCDeviceAttach("DALDEVICEID_TMC_WCSS",&qdss.hTMC));
         TRY(nErr, qdss_enable_local_etb());
      }

      if (NULL == qdss.hFunnel) {
         TRY(nErr,DAL_TFunnelDeviceAttach(DALDEVICEID_TFUNNEL_0,&qdss.hFunnel));
      }

      if (NULL == qdss.hTPDM) {
         TRY(nErr, DAL_TPDMDeviceAttach("DALDEVICEID_TPDM",&qdss.hTPDM));
      }

      if (NULL == qdss.hTPDA) {
         TRY(nErr, DAL_TPDMDeviceAttach("DALDEVICEID_TPDA",&qdss.hTPDA));
      }

      TRY(nErr,wcss_dbg_csr_init());

   }

   nErr = QDSS_CONTROL_SUCCESS;

   CATCH(nErr){
      if (DAL_ERROR==nErr) {
         nErr = QDSS_CONTROL_BAD_STATE;
      }
      else if (DAL_ERROR_DEVICE_ACCESS_DENIED==nErr)
      {
         nErr = QDSS_CONTROL_FUSE_BLOWN;
      }
   }

   return nErr;
}



int qdss_control_on_bootup(void);

/*-------------------------------------------------------------------------*/

#define QDSS_INVALID_TRACE_SINK  0xFF

/**
  @brief Initializes the QDSS control subsystem.

  This is called from RC init. QDSS HW is not touched or clocked.
  That happens when explicitly turned on by DIAG commands

  @return None
 */
DLL_API_GLOBAL void qdss_control_init(void)
{
   int nErr;
   qdss_diag_init();
   qdss.hTracer = 0;
   qdss.hTMC = 0;
   qdss.hFunnel = 0;
   qdss.hTPDM = 0;
   qdss.hTPDA = 0;
   qdss.hSync = 0;
   qdss.current_trace_sink = QDSS_TRACESINK_HWFIFO;
   qdss.bTraceSinkEnabled = FALSE;
   TRY(nErr,DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE, &qdss.hSync, NULL));
   qdss.npa_client = npa_create_sync_client("/clk/qdss", "wcss_dbg", NPA_CLIENT_REQUIRED);
   
   TRY(nErr,qdss_control_on_bootup());

   CATCH(nErr) {
   }
   IGNORE(nErr);
}

int qdss_tpdm_dsb_test(void);
int qdss_tpdm_cmb_test(void);



/*-------------------------------------------------------------------------*/


int qdss_control_on_bootup(void)
{
   int nErr = QDSS_CONTROL_SUCCESS;

#if 0
   TRY(nErr,qdss_tpdm_dsb_test());
   TRY(nErr,qdss_tpdm_cmb_test());

   CATCH(nErr) {}
#endif

   return nErr;
}



