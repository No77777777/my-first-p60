/**********************************************************************
 * flash_clocks.c
 *
 * SERVICES:    QPIC/PNOC clock and SPI NOR related node voting management
 *
 * DESCRIPTION: Provides flash operation clocks voting for NAND/SPI NOR
 *
 * PUBLIC CLASSES:  Not Applicable
 *
 * INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 *
 * Copyright (c) 2013-2014,2016,2017 QUALCOMM  Technologies Incorporated.
 * All Rights Reserved.
 * QUALCOMM Confidential and Proprietary
 *===================================================================
 *
 *                   EDIT HISTORY FOR MODULE
 *
 * This section contains comments describing changes made to the module.
 * Notice that changes are listed in reverse chronological order.
 *
 * $Header: //components/rel/core.mpss/10.0/storage/flash/src/dal/flash_clocks.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $
 *
 * when         who     what, where, why
 * --------     ---     ----------------------------------------------
 * 05/26/17     eo      Use ICB APIs for clock support
 * 10/12/16     eo      nand driver multi-client support
 * 02/26/16     bb      Enable spi-nor support 
 * 09/26/14     sb      NPA handle for PCNOC on Bear family
 * 05/23/13     eo      Initial Version
 *===================================================================*/

 /*===================================================================

===========================================================================*/

#include "core_variation.h"
#include "flash.h"

#define FLASH_TIMER_BASED_CLOCK_VOTE

#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
  #include "DDITimer.h"
  #include "flash_dal.h"
#endif

#include "flash_clocks.h"
#include "flash_dal_config.h"

#include "icbarb.h"
#include "npa.h"

/* Temporarily disable NPA votes */
//#define FEATURE_SPI_DISABLE_NPA_VOTE

/* CPU clock request for 288 MIPS */
#ifndef SPI_NOR_CPU_REQ_MIPS
  #define SPI_NOR_CPU_REQ_MIPS 288
#endif

/* Init PNOC source request of 100MHZ */ 
#define FLASH_NAND_NPA_PNOC_KHZ 100000

/* Init requiest of 400MB/s of bus bandwith for PNOC and QPIC */ 
#define FLASH_400MB  (400 * 1000 * 1000)

/* QPIC/PCNOC bandwidth requests require routing via ICB APIs since 
 * clocks controlled via RPMh 
 */
ICBArb_MasterSlaveType aMasterSlaveFromQpic[] =
{ 
  { ICBID_MASTER_QPIC, ICBID_SLAVE_EBI1 }
};

ICBArb_MasterSlaveType aMasterSlaveToQpic[] =
{ 
  { ICBID_MASTER_MSS_PROC, ICBID_SLAVE_QPIC }
};

/* IB=400 MB/s, AB= 0, latency=0 */
ICBArb_RequestType aRequest[] = 
{
  { ICBARB_REQUEST_TYPE_3, { FLASH_400MB, 0, 0 } }
};

/* NPA handles for PCNOC and QPIC bus bandwithd requests */
static npa_client_handle flash_hdl_to_qpic = NULL;
static npa_client_handle flash_hdl_from_qpic = NULL;

/* NPA handle for PCNOC bus bandwithd requests for SPI-NOR interface */
static npa_client_handle flash_spinor_npa_pnoc_handle = NULL;

/* Global provides ability to adjust value in t32 for tuning and testing */
uint32 flash_pnoc_clock_request = FLASH_NAND_NPA_PNOC_KHZ;
  
/* NPA handle for latency */
static npa_client_handle spi_npa_latency_handle = NULL;

/* NPA handle for CPU voting */
static npa_client_handle spi_npa_cpu_handle = NULL;

/* Enable NPA clock voting */
static uint32 flash_npa_enable_vote = TRUE;

/* CPU clock request value */
uint32 spi_clock_request = SPI_NOR_CPU_REQ_MIPS;

/* Counter for flash_clocks_requests without an equivalent */
uint32 flash_clocks_req_counter = 0;

#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
  /* Delay 5 * 10^6 usec to check flash clocks voting release. */ 
  #define FLASH_NAND_VOTE_DELAY_USEC (5 * 1000 * 1000)

  /* DAL timer struct to call DAL Timer */
  static struct
  {
    DalDeviceHandle    *timer_handle;    /* DAL callback timer */
    DALSYSEventHandle  timer_event;      /* DAL callback timer event */
    flash_client_ctxt  *client_ctxt;     /* Flash client context */
    uint32             qpic_clocks_enabled;
    uint32             qpic_clocks_req_counter; /* Enable flash operations */
    uint32             qpic_clocks_rel_vote_counter;

    DALSYS_EVENT_OBJECT(timer_object);   /* DAL timer event object */
  } dal_qpic_timer = {NULL, NULL, NULL, 0, 0, 0, NULL};  

  /* Structure for flash ops syncronization */
  static struct 
  {
    DALSYS_SYNC_OBJECT(flash_qpic_sync_object); /* Synchronizaton object */
    DALSYSSyncHandle handle_flash_qpic_sync;    /* Handle to sync object */
    uint32 qpic_synclock_init;
  } flash_qpic_locks = {NULL, NULL, FALSE};
#endif
/* Flash Operations Clocks enable */
void flash_clocks_enable(void)
{
  flash_npa_enable_vote = TRUE;
}

/* Flash Operations Clocks init */
void flash_clocks_init(void)
{
  /* Check spi-nor device driver type is active */
  if (flash_dal_config.config == FLASH_DAL_CONFIG_NOR)
  {
    /* check if spi npa latency handle exists */
    if (!spi_npa_latency_handle)
    {
      /* Create this client's NPA handle for latency requests only once.
       * NPA resource release by client should be done by invoking
       * npa_complete_request() API.
       */
      spi_npa_latency_handle = npa_create_sync_client(
        "/core/cpu/latency", "SPI_NOR", NPA_CLIENT_REQUIRED);
    }

    /* check if flash nand npa pnoc handle exists */
    if (!flash_spinor_npa_pnoc_handle)
    {
      /* Create this client's NPA handle for PNOC requests only once.
       * NPA resource release by client should be done by invoking
       * npa_complete_request() API.
       */
      flash_spinor_npa_pnoc_handle = npa_create_sync_client(
        "/clk/pcnoc", "SPINOR_PNOC", NPA_CLIENT_REQUIRED);
    }

    /* check if spi npa cpu handle exists */
    if (!spi_npa_cpu_handle)
    {
      /* Create this client's NPA handle for cpu only once.
       * NPA resource release by client should be done by invoking
       * npa_complete_request() API.
       */
#ifndef FEATURE_SPI_DISABLE_NPA_VOTE
      spi_npa_cpu_handle = npa_create_sync_client(
        "/core/cpu", "SPI_NOR", NPA_CLIENT_REQUIRED);
#endif
    }
  }
}

/* Flash Operations Clocks deinit */
void flash_clocks_deinit(void)
{
  /* Check spi-nor device driver type is active */
  if (flash_dal_config.config == FLASH_DAL_CONFIG_NOR)
  {
    /* check if spi npa latency handle is registered and if so, 
     * deregister it 
     */
    if (spi_npa_latency_handle)
    {
      npa_destroy_client(spi_npa_latency_handle);
      spi_npa_latency_handle = NULL;
    }
  
    /* check if spi npa cpu handle is registered and if so, deregister
     * it 
     */
    if (spi_npa_cpu_handle)
    {
      npa_destroy_client(spi_npa_cpu_handle);
      spi_npa_cpu_handle = NULL;
    }

    /* check if npa pnoc handle is registered */
    if (flash_spinor_npa_pnoc_handle)
    {
      npa_destroy_client(flash_spinor_npa_pnoc_handle);
      flash_spinor_npa_pnoc_handle = NULL;
    }

  }
}

/* Enter vote for latency and PNOC clocks for flash before
 * begining of flash operations.
 */
void flash_clocks_request(void)
{
  if (flash_clocks_req_counter == 0)
  {
    /* Check spi-nor device driver type is active */
    if (flash_dal_config.config == FLASH_DAL_CONFIG_NOR)
    {

      if (flash_spinor_npa_pnoc_handle)
      {
        /* Request pnoc clock for flash nand transfer */
        npa_issue_required_request(flash_spinor_npa_pnoc_handle,
          flash_pnoc_clock_request);
      }

      /* Request for a latency of a minimum value so that there wont be any
       * latency in processing the SPI interrupts 
       */
      if (spi_npa_latency_handle)
      {
        /* Request for max allowable latency of 1 sclk */
        npa_issue_required_request(spi_npa_latency_handle, 1);
      }

      if (spi_npa_cpu_handle)
      {
        /* Request for CPU to prevent the clocks from scaling down */
        npa_issue_required_request(spi_npa_cpu_handle, spi_clock_request);
      }
    }
  }
  flash_clocks_req_counter++;
}

/* Release vote for latency and PNOC clocks for flash at end of 
 * flash operations.
 */
void flash_clocks_request_complete(void)
{
  flash_clocks_req_counter--;
  
  if (flash_clocks_req_counter == 0)
  { 
    /* Check spi-nor device driver type is active */
    if (flash_dal_config.config == FLASH_DAL_CONFIG_NOR)
    {

      if (flash_spinor_npa_pnoc_handle)
      {
        /* Release pnoc clock vote after flash nand transfer*/
        npa_complete_request(flash_spinor_npa_pnoc_handle);
      }

      if (spi_npa_latency_handle)
      {
        /* complete request for latency so that there is no restriction on 
         * latency 
         */
        npa_complete_request(spi_npa_latency_handle);
      }

      if (spi_npa_cpu_handle)
      {
        /* Complete NPA request for CPU */
        npa_complete_request(spi_npa_cpu_handle);
      }
    }
  }
}

#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
/* Flash DAL timer callback function */
void flash_nand_callback(void)
{
  /* Synchronize with other clients */
  DALSYS_SyncEnter(flash_qpic_locks.handle_flash_qpic_sync);
  
  if ((dal_qpic_timer.qpic_clocks_enabled == TRUE) && (dal_qpic_timer.qpic_clocks_req_counter == 0))
  {
    dal_qpic_timer.qpic_clocks_rel_vote_counter++;
	  
    if (flash_hdl_from_qpic && flash_hdl_to_qpic)
    {
      /* Remove pnoc/qpic vote after flash nand transfer*/
      npa_complete_request(flash_hdl_from_qpic);
      npa_complete_request(flash_hdl_to_qpic);
    }
    dal_qpic_timer.qpic_clocks_enabled = FALSE;
  }
  else
  {
    if (DAL_SUCCESS != DalTimer_Register(dal_qpic_timer.timer_handle, 
	  dal_qpic_timer.timer_event, FLASH_NAND_VOTE_DELAY_USEC))
    {
	  DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
	    "Flash: Error invoking DAL timer register");			  
    }
  }

  /* Leave Synchronization */
  DALSYS_SyncLeave(flash_qpic_locks.handle_flash_qpic_sync);
}

/* Flash DAL timer init */
void flash_nand_timer_init(flash_client_ctxt *client_ctxt_data)
{

  if (client_ctxt_data == NULL)
  {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Timer init failed due, client ctxt data is NULL!");
  }
  dal_qpic_timer.client_ctxt = client_ctxt_data;

  /* check DAL timer handle and flash client context exist */
  if (!dal_qpic_timer.timer_handle)
  {
    dal_qpic_timer.qpic_clocks_req_counter = 0;
    dal_qpic_timer.qpic_clocks_rel_vote_counter= 0;
    dal_qpic_timer.qpic_clocks_enabled = FALSE;
    
    /* Create this client's DAL timer handle once. */
    DALSYS_InitMod(NULL);
    
    /* Attach to DAL timer device driver */
    if (DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_TIMER, 
      &dal_qpic_timer.timer_handle))
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error attaching to DAL Timer device driver!");
      
      dal_qpic_timer.timer_handle = NULL;
    }

    /* Create timer event with callback function notification */
    if (dal_qpic_timer.timer_handle)
    {
      if (DAL_SUCCESS != DALSYS_EventCreate(
        DALSYS_EVENT_ATTR_CALLBACK_EVENT | DALSYS_EVENT_ATTR_TIMEOUT_EVENT,
        &dal_qpic_timer.timer_event, &dal_qpic_timer.timer_object))
      {
        DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
          "Flash: Error creating to DAL timer event");      
      }

      if (DAL_SUCCESS != DALSYS_SetupCallbackEvent(dal_qpic_timer.timer_event,
        (DALSYSCallbackFunc)flash_nand_callback, dal_qpic_timer.client_ctxt))
      {
        DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
          "Flash: Error creating to DAL callback event");            
      }

      if (DAL_SUCCESS != DalTimer_Register(dal_qpic_timer.timer_handle, 
        dal_qpic_timer.timer_event, FLASH_NAND_VOTE_DELAY_USEC))
      {
        DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
          "Flash: Error invoking DAL timer register");            
      }
    }
  }
}
#endif

/* Flash NAND Operations Clocks init */
void flash_nand_clocks_init(uint32 *client_ctxt_data)
{
  /* check if flash nand npa pnoc handle exists */
  if (!flash_hdl_from_qpic)
  {
    /* Create this client's NPA handle for PNOC requests only once.
          * NPA resource release by client should be done by invoking
          * npa_complete_request() API.
          */
    flash_hdl_from_qpic= npa_create_sync_client_ex(
      "/icb/arbiter", 
      "qpic-ddr", 
      NPA_CLIENT_VECTOR,
      sizeof(aMasterSlaveFromQpic),
      &aMasterSlaveFromQpic);
  }
  if (!flash_hdl_to_qpic)
  {
    flash_hdl_to_qpic= npa_create_sync_client_ex(
      "/icb/arbiter", 
      "mss-qpic", 
      NPA_CLIENT_VECTOR,
      sizeof(aMasterSlaveToQpic),
      &aMasterSlaveToQpic);
  }
  if (!flash_hdl_from_qpic || !flash_hdl_to_qpic)
  {
    /* Error Fatal on NPA pnoc/qpic arbiter node registration failure. */
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
                    "Flash: NPA pcnoc/qpic arbiter node registration failed!");
  }

#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
  if (DAL_SUCCESS != DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
    &(flash_qpic_locks.handle_flash_qpic_sync),
    &(flash_qpic_locks.flash_qpic_sync_object)))
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
	  "Flash: QPIC SyncLock Create Failed!");
  }
  flash_qpic_locks.qpic_synclock_init = TRUE;
  flash_nand_timer_init((flash_client_ctxt *)client_ctxt_data);
#else
  flash_clocks_req_counter = 0;
#endif  
}

/* Flash NAND Operations Clocks deinit */
void flash_nand_clocks_deinit(void)
{
  /* Destroy pcnoc/qpic node clients */
  if (flash_hdl_from_qpic)
  {
    npa_destroy_client(flash_hdl_from_qpic);
    flash_hdl_from_qpic = NULL;
  }

  if (flash_hdl_to_qpic)
  {
    npa_destroy_client(flash_hdl_to_qpic);
    flash_hdl_to_qpic = NULL;
  }

#ifdef FLASH_TIMER_BASED_CLOCK_VOTE  
  /* if dal timer handle exists, free timer event and detach */
  if (dal_qpic_timer.timer_handle)
  {
    if (DAL_SUCCESS != DalTimer_UnRegister(dal_qpic_timer.timer_handle, 
      dal_qpic_timer.timer_event))
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error invoking DAL timer unregister");            
    }
	
    if (DALSYS_DestroyObject(&dal_qpic_timer.timer_event) != 
      DAL_SUCCESS)
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error Freeing DAL Timer Event!");
    }
    dal_qpic_timer.timer_event = NULL;

    if (DAL_DeviceDetach((DalDeviceHandle*)dal_qpic_timer.timer_handle) !=
      DAL_SUCCESS)
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error DAL Timer Detach!");
    }
    dal_qpic_timer.timer_handle = NULL;
    dal_qpic_timer.client_ctxt = NULL;
    dal_qpic_timer.qpic_clocks_req_counter = 0;
  }
  
  if (flash_qpic_locks.qpic_synclock_init == TRUE)
  { 
    if (DALSYS_DestroyObject(flash_qpic_locks.handle_flash_qpic_sync) != 
      DAL_SUCCESS)
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error Freeing DAL Timer Event!");
    }
    flash_qpic_locks.qpic_synclock_init = FALSE;
  }
#endif
}

/* Request QPIC and PCNOC bandwidth for flash transfers before
 * begining of flash operations.
 */
void flash_nand_clocks_request(void)
{
#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
  /* Synchronize with other clients */
  DALSYS_SyncEnter(flash_qpic_locks.handle_flash_qpic_sync);
  
  if (dal_qpic_timer.qpic_clocks_req_counter == 0)
  {
    if (flash_hdl_from_qpic && flash_hdl_to_qpic)
    {
      /* Request pcnoc/qpic bus bandwidth for flash nand transfer */
      npa_issue_vector_request(
        flash_hdl_from_qpic,
        sizeof(aRequest)/sizeof(npa_resource_state),                       
        (npa_resource_state *)aRequest);
      npa_issue_vector_request(
        flash_hdl_to_qpic,
        sizeof(aRequest)/sizeof(npa_resource_state),                       
        (npa_resource_state *)aRequest);
    }
    dal_qpic_timer.qpic_clocks_enabled = TRUE;
  }
  dal_qpic_timer.qpic_clocks_req_counter++;

  /* Leave Synchronization */
  DALSYS_SyncLeave(flash_qpic_locks.handle_flash_qpic_sync);
#else
  if (flash_clocks_req_counter == 0)
  {
    if (flash_hdl_from_qpic && flash_hdl_to_qpic)
    {
      /* Request pnoc/qpic bandwidth for flash nand transfer */
      npa_issue_vector_request(
        flash_hdl_from_qpic,
        sizeof(aRequest)/sizeof(npa_resource_state),						 
       (npa_resource_state *)aRequest);
      npa_issue_vector_request(
        flash_hdl_to_qpic,
        sizeof(aRequest)/sizeof(npa_resource_state),						 
       (npa_resource_state *)aRequest);
    }
  }
  flash_clocks_req_counter++;
#endif
}

/* Release vote for QPIC and PNOC clocks for flash at end of 
 * flash operations.
 */
void flash_nand_clocks_request_complete(void)
{
#ifdef FLASH_TIMER_BASED_CLOCK_VOTE
  /* Synchronize with other clients */
  DALSYS_SyncEnter(flash_qpic_locks.handle_flash_qpic_sync);
  
  dal_qpic_timer.qpic_clocks_req_counter--;
  
  if (dal_qpic_timer.qpic_clocks_req_counter == 0)
  {
    if (DAL_SUCCESS != DalTimer_Register(dal_qpic_timer.timer_handle, 
      dal_qpic_timer.timer_event, FLASH_NAND_VOTE_DELAY_USEC))
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error invoking DAL timer register");				
    }
  }

  /* Leave Synchronization */
  DALSYS_SyncLeave(flash_qpic_locks.handle_flash_qpic_sync);
#else  
  flash_clocks_req_counter--;

  if (flash_clocks_req_counter == 0)
  {
    if (flash_hdl_from_qpic && flash_hdl_to_qpic)
    {
      /* Release pnoc/qpic bandwidth request after flash nand transfer*/
      npa_complete_request(flash_hdl_from_qpic);
      npa_complete_request(flash_hdl_to_qpic);
    }
  }
#endif
}
