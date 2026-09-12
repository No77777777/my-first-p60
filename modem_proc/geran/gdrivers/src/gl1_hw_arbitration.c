/*========================================================================
                      GSM GPRS NPL1 MSG LAYER ARBITRATION
DESCRIPTION
   This module implements the arbitration interface exposed by the the GSM/GPRS 
   frame layer. The interface allows an external arbitration component 
   to be installed in the GSM/GPRS frame layer and perform Rx and Tx arbitration
   against external access stratums.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS   
   
Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
========================================================================== */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_arbitration.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
19/08/15   mc        CR.874662 : CXM Freq ID support for X2G startup and X2G preload
23/10/14   pg        CR744613 Use gas_id to differentiate default priorities per sub 
21/03/14   mc        CR.588853 : COEX : GL1 changes for Enhanced FCCH algorithm
05/03/14   mc        CR.583282 : Limit the number of PSCAN monitors per frame to that which can be COEX protected in X+G DSDA
04/11/13   mc        CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
29/07/13   sk        CR519663 Partial QBTA bringup changes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
========================================================================== */

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gl1_hw_arbitration.h"
#include "gl1_hw_g.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
gl1_hw_arbitration_t gl1_hw_arbitration[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_COEX

/*===========================================================================

FUNCTION gl1_hw_arbitration_null_notify_event

DESCRIPTION
  This implements the NULL arbitrator

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void gl1_hw_arbitration_null_notify_event(gl1_hw_arbitrated_event_t event, gas_id_t gas_id)
{
   int i;

   if(event==ARBITRATE_RX)
   {
      for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
      {
         if(gl1_hw_arbitration[gas_id].rx.slots[i].configured == TRUE)
         {
            *gl1_hw_arbitration[gas_id].rx.slots[i].priority = gas_id;
            *gl1_hw_arbitration[gas_id].rx.slots[i].desense_id = 0xFFFFFFF0;
         }
      }
   }
   else
   if(event==ARBITRATE_TX)
   {
      for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
      {
         if(gl1_hw_arbitration[gas_id].tx.slots[i].configured == TRUE)
         {
            *gl1_hw_arbitration[gas_id].tx.slots[i].priority = gas_id;
            *gl1_hw_arbitration[gas_id].tx.slots[i].desense_id = 0xFFFFFFF0;
            *gl1_hw_arbitration[gas_id].tx.slots[i].backoff_vbatt = 0;
            *gl1_hw_arbitration[gas_id].tx.slots[i].backoff_desense = 0;
         }
      }
   }
   else
   if(event==ARBITRATE_MONITORS)
   {
      for(i=0;i<gl1_hw_arbitration[gas_id].monitors.count;i++)
      {
         gl1_hw_arbitration[gas_id].monitors.monitor[i].desense_id = 0xFFFFFFF0;
         gl1_hw_arbitration[gas_id].monitors.monitor[i].priority = gas_id;
      }

      gl1_hw_arbitration[gas_id].monitors.max_per_frame = 100; /* unrestricted */
      gl1_hw_arbitration[gas_id].monitors.nozone_count = 0;
   }
   else
   if(event==ARBITRATE_ACQ)
   {
      gl1_hw_arbitration[gas_id].acq.desense_id = 0xFFFFFFF0;
      gl1_hw_arbitration[gas_id].acq.priority = gas_id;
      gl1_hw_arbitration[gas_id].acq.enhanced_fcch_detection = FALSE;
   }
   else
   if(event==ARBITRATE_SCH)
   {
      gl1_hw_arbitration[gas_id].sch.desense_id = 0xFFFFFFF0;
      gl1_hw_arbitration[gas_id].sch.priority = gas_id;
   }
   if(event==ARBITRATE_X2G)
   {
      gl1_hw_arbitration[gas_id].x2g.activity_priority = gas_id;
      gl1_hw_arbitration[gas_id].x2g.no_activity_priority = gas_id;
      gl1_hw_arbitration[gas_id].x2g.desense_id = 0xFFFFFFF0;
   }
}

/*===========================================================================

FUNCTION      gl1_hw_get_arbitration

DESCRIPTION
  This function returns a pointer to the arbitration interface of the 
  specified GAS.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the arbitration interface for the specified GAS.

===========================================================================*/
gl1_hw_arbitration_t* gl1_hw_get_arbitration(gas_id_t gas_id)
{
   return &gl1_hw_arbitration[gas_id];
}

/*===========================================================================

FUNCTION      gl1_hw_cfg_arbitration

DESCRIPTION
  This function configures a functions that the frame layer can use to
  initiate registration or deregistration with an arbitrator.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the arbitration interface for the specified GAS.

===========================================================================*/
void gl1_hw_cfg_arbitration(gl1_hw_arbitration_reg_t registration, gl1_hw_arbitration_reg_t deregistration, gas_id_t gas_id)
{
   gl1_hw_arbitration[gas_id].registration   = registration;
   gl1_hw_arbitration[gas_id].deregistration = deregistration;
}

/*===========================================================================

FUNCTION      gl1_hw_arbitration_init

DESCRIPTION
  This function initialises the arbitration interface.

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void gl1_hw_arbitration_init(gas_id_t gas_id)
{
   gl1_hw_arbitration[gas_id].notify_event = gl1_hw_arbitration_null_notify_event;
}

#endif /* FEATURE_GSM_COEX */
