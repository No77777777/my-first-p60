/*==============================================================================

FILE:      GPIOInt_NPA.c

DESCRIPTION
  This modules implements the API to be utilized by GPIO interrupt controller
  for internal NPA related functions for XO resource voting.
  The definitions in this module are implemented internally and should not be 
  exposed for external client usage directly without using the accompanying DDI
  interface file for this module.

REFERENCES

       Copyright © 2010-2017 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt_NPA.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/15/2017  hvm     First draft created. 
===========================================================================*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "GPIOInt_DirConn.h"
#include "GPIOInt_Summary.h"

/*=========================================================================
      Macros
==========================================================================*/

/*
 * Disable XO shutdown if any non-PDC interrupts are enabled.
 */
#define GPIOINT_XO_SHUTDOWN_DISABLE               1

/*=========================================================================
      Data Types
==========================================================================*/

/*=========================================================================
      Forward references
==========================================================================*/

/*------------------------------------------------------------------------------
  Internal NPA related functions for XO resource voting.
------------------------------------------------------------------------------*/

#ifdef DIRCONNGPIO_USE_NPA
void DirConnGpio_Init_NPA(void *context, 
                          unsigned int event_type, 
                          void *resource_name, 
                          unsigned int resource_strlen);

void DirConnGpio_UpdateNPARequest(void *context);
#endif


#ifdef SUMMARYGPIO_USE_NPA
void SummaryGpio_Init_NPA(void *context, 
                          unsigned int event_type, 
                          void *resource_name, 
                          unsigned int resource_strlen);

void SummaryGpio_UpdateNPARequest(void *context);
#endif


/*==========================================================================

  FUNCTION      DirConnGpio_Init_NPA

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

#ifdef DIRCONNGPIO_USE_NPA

void DirConnGpio_Init_NPA
( 
  void          *context,
  unsigned int  event_type, 
  void          *resource_name,
  unsigned int  resource_strlen
)
{
  DirConnGpioCfgCtxtType *pCtxt = (DirConnGpioCfgCtxtType *)context;

  pCtxt->hNpaClient = npa_create_sync_client(resource_name, 
                                             "dirconn_gpio", 
                                             NPA_CLIENT_REQUIRED);
  if(pCtxt->hNpaClient != NULL)
  {
      DirConnGpio_UpdateNPARequest(pCtxt);
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 0, 
                     "DirConnGpio_Init_NPA: Device could not create a Client Handle in DirConnGpio_Init_NPA");
  }

}/* END DirConnGpio_Init_NPA */


/*==========================================================================

  FUNCTION      DirConnGpio_UpdateNPARequest

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

void DirConnGpio_UpdateNPARequest
(
  void *context
)
{
  DirConnGpioCfgCtxtType *pCtxt = (DirConnGpioCfgCtxtType *)context;
  /* 
   * If the server is not ready we cannot make this request
   */
  if(pCtxt->hNpaClient == NULL)
  {
      return;
  }
  /*
   * Disable interrupts
   */
  //GPIOINT_TODO: (void)DALSYS_SyncEnter(device->gpioint_synchronization);
  
  if(pCtxt->non_mpm_interrupts )
  {
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 1, 
                     "DirConnGpio_UpdateNPARequest: Issuing request for CXO, total number of non-pdc interrupts: %d", 
                     ( pCtxt->non_mpm_interrupts));
    
      /*
       *  If there are are interrupts that are not wakeup sources we need to 
       *  request for xo clk source so that we can keep state of the interrupt 
       *  controller and not have it wiped out at a lower voltage.
       */
      npa_issue_required_request(pCtxt->hNpaClient, GPIOINT_XO_SHUTDOWN_DISABLE);
  }
  else 
  {
      /*
       *  If only interrupts that are wakeup sources are enabled we can cancel request 
       *  for xo clk and save power. We don't have to maintain state at 
       *  the interrupt controller as the PDC interrupt controller already has this state.
       */
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 1, 
                     "DirConnGpio_UpdateNPARequest: Cancel request for CXO total number of non-pdc interrupts: %d",
                     pCtxt->non_mpm_interrupts);

      npa_cancel_request(pCtxt->hNpaClient);
  }

  /*
   * Enable interrupts
   */
  //GPIOINT_TODO: (void)DALSYS_SyncLeave(device->gpioint_synchronization);

}/* END DirConnGpio_UpdateNPARequest */

#endif /* DIRCONNGPIO_USE_NPA */


/*==========================================================================

  FUNCTION      SummaryGpio_Init_NPA

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

#ifdef SUMMARYGPIO_USE_NPA

void SummaryGpio_Init_NPA
( 
  void          *context,
  unsigned int  event_type, 
  void          *resource_name,
  unsigned int  resource_strlen
)
{
  SummaryGpioCfgCtxtType *pCtxt = (SummaryGpioCfgCtxtType *)context;

  pCtxt->hNpaClient = npa_create_sync_client(resource_name, 
                                             "summary_gpio", 
                                             NPA_CLIENT_REQUIRED);
  if(pCtxt->hNpaClient != NULL)
  {
      SummaryGpio_UpdateNPARequest(pCtxt);
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 0, 
                     "SummaryGpio_Init_NPA: Device could not create a Client Handle in SummaryGpio_Init_NPA");
  }

}/* END SummaryGpio_Init_NPA */


/*==========================================================================

  FUNCTION      SummaryGpio_UpdateNPARequest

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

void SummaryGpio_UpdateNPARequest
(
  void *context
)
{
  SummaryGpioCfgCtxtType *pCtxt = (SummaryGpioCfgCtxtType *)context;
  /* 
   * If the server is not ready we cannot make this request
   */
  if(pCtxt->hNpaClient == NULL)
  {
      return;
  }
  /*
   * Disable interrupts
   */
  //GPIOINT_TODO: (void)DALSYS_SyncEnter(device->gpioint_synchronization);
  
  if(pCtxt->non_mpm_interrupts)
  {
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 1, 
                     "SummaryGpio_UpdateNPARequest: Issuing request for CXO, total number of non-pdc interrupts: %d", 
                     (pCtxt->non_mpm_interrupts));
    
      /*
       *  If there are are interrupts that are not wakeup sources we need to 
       *  request for xo clk source so that we can keep state of the interrupt 
       *  controller and not have it wiped out at a lower voltage.
       */
      npa_issue_required_request(pCtxt->hNpaClient, GPIOINT_XO_SHUTDOWN_DISABLE);
  }
  else 
  {
      /*
       *  If only interrupts that are wakeup sources are enabled we can cancel request 
       *  for xo clk and save power. We don't have to maintain state at 
       *  the interrupt controller as the PDC interrupt controller already has this state.
       */
      GPIOIntLog_Out(GPIOINTLOG_NPAREQ, pCtxt->hULog, 1, 
                     "SummaryGpio_UpdateNPARequest: Cancel request for CXO total number of non-pdc interrupts: %d",
                     pCtxt->non_mpm_interrupts);

      npa_cancel_request(pCtxt->hNpaClient);
  }

  /*
   * Enable interrupts
   */
  //GPIOINT_TODO: (void)DALSYS_SyncLeave(device->gpioint_synchronization);

}/* END SummaryGpio_UpdateNPARequest */

#endif /* SUMMARYGPIO_USE_NPA */

