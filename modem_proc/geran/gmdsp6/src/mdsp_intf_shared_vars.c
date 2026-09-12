/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L A Y E R  1  - M D S P   I N T E R F A C E
                    mDSP Shared Variables Access Functions

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS


  Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_intf_shared_vars.c#1 $

when         who     what, where, why
----------   ------- ------------------------------------------------------
18/07/15     zf      CR857733: Remove cmCleanupAlpha variable.
12/06/15     jk      CR821180:Feature flag cleanup
23/01/15     jk      CR788012:x2gRxfeConfigAlpha set to 30 for T2G and for other X2G set to 0
27/03/13     cja     CR468678 Set cmCleanupAlpha shared variable for FW for T2G
12-10-22     tjw     Initial version.
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "msg.h"
#include "mdsp_intf_shared_vars.h"
#include "mdsp_intf_g.h"

void mdsp_intf_shared_var_write(MdspGeranSharedVariableType sharedVariable,  uint16 value , gas_id_t gas_id )
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
  
  switch(sharedVariable)
  {
  case GFWSHAREDVARIABLES_RX_X2GSTARTALPHA:
    public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.x2gStartAlpha = value;
    break;
  case GFWSHAREDVARIABLES_RX_X2GRXFECONFIGALPHA:
    public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.x2gRxfeConfigAlpha = value;
    break;
  case GFWSHAREDVARIABLES_EDGE_MSC:
    public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.msc = value;
    break;
  case GFWSHAREDVARIABLES_EDGE_COVERITHRESH:
    public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.cOverIthresh = value;
    break;
  case GFWSHAREDVARIABLES_EDGE_COVERISIGMA:
    public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.cOverISigma = value;
    break;
  default:
    MSG_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "Invalid sharedVar %d", sharedVariable);
    break;
  }
}
  
void * mdsp_intf_shared_var_read(MdspGeranSharedVariableType sharedVariable , gas_id_t gas_id )
{
  void *pResult = NULL;
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  switch(sharedVariable)
  {
  case GFWREADVARIABLESSTRUCT_TCHNUMZEROSOFTDECS:
    //pResult = &public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.tchNumZeroSoftDecs;
    MSG(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "tchNumZeroSoftDecs not available");
    break;
  case GFWREADVARIABLESSTRUCT_GFWSLEEPSTATE:
    pResult = (void *)&public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwSleepState;
    break;
  case GFWREADVARIABLESSTRUCT_GSISRPROCDELAY:
    //pResult = &public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gsIsrProcDelay;
    MSG(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "gsIsrProcDelay not available");
    break;
  case GFWREADVARIABLESSTRUCT_PWRMSRBURSTSIZE:
    //pResult = &public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.pwrMsrBurstSize;
    MSG(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "pwrMsrBurstSize not available");
    break;
  case GFWSHAREDVARIABLES_RX_X2GSTARTALPHA:
    pResult = (void *)&public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.x2gStartAlpha;
    break;

  case GFWSHAREDVARIABLES_RX_X2GRXFECONFIGALPHA:
    pResult = (void *)&public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.x2gRxfeConfigAlpha;
    break;

  case GFWMETRICSBUFFERS:
    pResult = (void *)&public_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers;
    break;
  default:
    MSG_1(MSG_SSID_DFLT, MSG_LEGACY_ERROR, "Invalid sharedVar %d", sharedVariable);
    break;
  }   
return pResult;
}
