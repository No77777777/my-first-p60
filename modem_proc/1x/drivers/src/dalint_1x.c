/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        DEVICE ACCESS LIBRARY INTERRUPT CONTROLLER

GENERAL DESCRIPTION
  This module contains the wrapper functions for DAL interrupt controller
  (DALInt). DALInt is the replacement for the existing TRAMP controller.
  This module has been made backward compatible so that the existing
  TRAMP functions can be used just by defining the feature. So this can
  readily replace all the use of TRAMP in our 1x code. For these functions
  to work, 1x clients just need to include the dalint_1x.h header file.

EXTERNALIZED FUNCTIONS
  dalint_1x_intr_reg - This function registers the DAL Interrupt Controller
  dalint_1x_intr_dereg - This function deregisters the DAL Interrupt Controller
  dalint_1x_intr_dereg_all - This function deregisters all the IRQs for a
                             particular client
  dalint_1x_irq_reg_by_client - This function tells if the client uses a
                                particular IRQ or not.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 2012 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/dalint_1x.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
09/05/14   srk     Remove the unused Quick paging ISR and update demback
                   IRQ IDs for Jolokia and Thor.
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
03/03/14   srk     Dynamically specify stack size for ISRs.
10/15/13   cjb     Removed unused IRQs and related cleanup
10/10/13   cjb     Changes for DEMBACK0 vs DEMBACK1 usage.
09/16/13   cjb     Updated DEMBACK0 and DEMBACK1 IRQ vectors for BOLT
05/06/13   cjb     Resolve compiler warnings
04/02/13   pap     Feature cleanup
03/15/13   cjb     Print reason for DAL interrupt registration failure cases.
11/09/12   dkb     Ensure the critical section is initialized before using it
                   in dereg.
09/24/12   cjb     Switched from 1x_rtc_pnroll_irq to demss_cdma_pnroll_irq.
08/15/12   cjb     Created a single variable with access to all essential info.
05/24/12   cjb     Initialize the critical section before using it.
02/17/12   srk     Removed the defensive check which prevents different isr
                   callback functions being registered for same Interrupt IDs
02/16/12   srk     Created
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* 1X common */
#include "modem_1x_utils.h"

/* Drivers */
#include "dalint_1x.h"
#include "drivers_common.h"

/* DALInterruptController */
#include "DalDevice.h"            /* DAL_OPEN_SHARED */
#include "DALDeviceId.h"
#include "DALSys.h"               /* DAL_DeviceAttach() */
#include "DALStdErr.h"            /* DAL_SUCCESS */
#include "DDIInterruptController.h"

/* Other */
#include "err.h"
#include "m1x_diag.h"
#include "rex.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*-------------------------------------------------------------------------
            Macro Definitions
-------------------------------------------------------------------------*/
#ifdef FEATURE_MODEM_1X_JOLOKIA
#define DAL1X_CDMA_DEMBACK0_DEC_DONE_IRQ   187 /* demback0_dec_done_irq */
#define DAL1X_CDMA_DEMBACK1_DEC_DONE_IRQ   187 /* demback1_dec_done_irq */
#elif defined( FEATURE_MODEM_1X_THOR )
#define DAL1X_CDMA_DEMBACK0_DEC_DONE_IRQ   256 /* demback0_dec_done_irq */
#define DAL1X_CDMA_DEMBACK1_DEC_DONE_IRQ   265 /* demback1_dec_done_irq */
#elif defined( FEATURE_MODEM_1X_BOLT )
#define DAL1X_CDMA_DEMBACK0_DEC_DONE_IRQ   136 /* demback0_dec_done_irq */
#define DAL1X_CDMA_DEMBACK1_DEC_DONE_IRQ   145 /* demback1_dec_done_irq */
#endif /* FEATURE_MODEM_1X_BOLT */


/*-------------------------------------------------------------------------
                     Typedefs
-------------------------------------------------------------------------*/

/* Structure for holding general dalint info */
typedef struct
{
  boolean                         isClient_initialized;
  DalDeviceHandle*                dalint_1x_handle;
} dalint_1x_client_info;

/* Structure for holding irq info */
typedef struct
{
  DALInterruptID                  irq_id;
  uint32                          interrupt_trigger;
  uint8                           client_mask;
} dalint_1x_irq_info;

/* DALINT_1X Main Structure */
typedef struct
{
  rex_crit_sect_type              crit_sect;
  boolean                         initialized;
  dalint_1x_irq_info              irq_info[ DALINT_1X_MAX_IRQ ];
  dalint_1x_client_info           client_info[ DALINT_1X_MAX_CLIENTS ];
} dalint_1x_type;

/*-------------------------------------------------------------------------
                      Static Variables
-------------------------------------------------------------------------*/

/* DALINT_1X Main Structure */
static dalint_1x_type dalint_1x =
{
  .initialized = FALSE,

  /* Initialize the structure for all clients */
  .client_info =
  {
    {FALSE, NULL},
    {FALSE, NULL},
    {FALSE, NULL},
    {FALSE, NULL},
    /*
      Whenever a new client is added in the header file,
      its corresponding entry should be initialized here.
    */
  },

  /* Initialize the structure for all IRQs */
  .irq_info =
  {
    { DAL1X_CDMA_DEMBACK0_DEC_DONE_IRQ, DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER, 0 },
    { DAL1X_CDMA_DEMBACK1_DEC_DONE_IRQ, DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER, 0 },
    /*
      Whenever a new IRQ is added in the header file, its corresponding entry
      should be initialized here. You just need to update the 1st and 2nd
      elements of the dalint_1x_irq_info structure. The 3rd entry should be 0.
    */
  },

  /* zero rest of struct... */

};

/*===========================================================================

                    FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION       DALINT_1X_INTR_REG

DESCRIPTION    This function registers the DAL Interrupt Controller

DEPENDENCIES   None

RETURN VALUE   boolean

SIDE EFFECTS   None

===========================================================================*/
boolean dalint_1x_intr_reg(dalint_1x_client_name client_id,
                           dalint_1x_irq_types   irq_vector,
                           dalint_1x_isr         isr_callback,
                           dalint_1x_stack_size  stack_size)
{
  boolean status = TRUE;

  /* Return value from DAL interface */
  DALResult result = DAL_ERROR;

  /* Critical section initialization */
  if( dalint_1x.initialized == FALSE )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Critical section initialization done" );
    rex_init_crit_sect( &dalint_1x.crit_sect );
    dalint_1x.initialized = TRUE;
  }

  if((irq_vector < DALINT_1X_MAX_IRQ) && (client_id < DALINT_1X_MAX_CLIENTS))
  {
    /* Ensure that only one client can initialize and Register the
       DAL interrupt controller at a time*/
    DRV_ENTER_CRITICAL_SECTION(dalint_1x.crit_sect);

    /* Make sure if the client is not initialized */
    if(dalint_1x.client_info[client_id].isClient_initialized == FALSE)
    {
      /* Attaches the DALInt and returns an interrupt handle */
      result = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER,
                         &dalint_1x.client_info[client_id].dalint_1x_handle) ;

      /* Error Checking */
      if((result != DAL_SUCCESS) ||
         (dalint_1x.client_info[client_id].dalint_1x_handle == NULL))
      {
        /* Make the handle NULL */
        dalint_1x.client_info[client_id].dalint_1x_handle = NULL;

        /* Make return status as False */
        status = FALSE;

        ONEX_ERR_FATAL( "Unable to attach DAL interrupt controller, "
                   "Client ID = %d, Client State = %d, result = %d",
                   client_id,
                   dalint_1x.client_info[client_id].isClient_initialized,
                   result );
      }
      else
      {
        /* Change the Client State */
        dalint_1x.client_info[client_id].isClient_initialized = TRUE;

        M1X_MSG( MUX, LEGACY_MED,
          "Attached DAL interrupt controller, "
          "Client ID = %d, Client State = %d, result = %d",
          client_id,
          dalint_1x.client_info[client_id].isClient_initialized,
          result );
      }
    }

    /* Check for valid handle and client state */
    if((dalint_1x.client_info[client_id].dalint_1x_handle != NULL) &&
       (dalint_1x.client_info[client_id].isClient_initialized == TRUE))
    {
      uint32 intr_trigger = dalint_1x.irq_info[irq_vector].interrupt_trigger;

      if ( stack_size == DALINT_1X_STACK_4K )
      {
        intr_trigger |= DALINTCTRL_CONFIG_STACK_4K;
      }

      /* Register ISR -> Alternative for tramp_set_isr */
      result = DalInterruptController_RegisterISR(
                    dalint_1x.client_info[client_id].dalint_1x_handle,
                    dalint_1x.irq_info[irq_vector].irq_id,
                    isr_callback,
                    NULL,
                    intr_trigger);

      /* Error Checking */
      if((result != DAL_SUCCESS))
      {
        /* Make return status as False */
        status = FALSE;

        ONEX_ERR_FATAL( "Unable to Register ISR, " 
                        "Client ID = %d, INTR ID = %d, result = %d",
                         client_id, irq_vector, result );
      }
      else
      {
        /* Set the client's bit in the IRQ client mask */
        dalint_1x.irq_info[irq_vector].client_mask |= (0x01 << client_id);

        M1X_MSG( MUX, LEGACY_MED,
          "Registered ISR, Client ID = %d, INTR ID = %d, result = %d",
          client_id,
          irq_vector,
          result );
      }
    }
    else
    {
      /* Make return status as False */
      status = FALSE;

      ONEX_ERR_FATAL( "Client either uninitialized or has an invalid handle, "
                 "Client ID = %d, Client State = %d, Handle = %d",
                 client_id,
                 dalint_1x.client_info[client_id].isClient_initialized,
                 dalint_1x.client_info[client_id].dalint_1x_handle );
    }

    /* Leave the Critical section after registration is done */
    DRV_LEAVE_CRITICAL_SECTION(dalint_1x.crit_sect);
  }
  else
  {
    /* Make return status as False */
    status = FALSE;
  }

  return status;

} /* dalint_1x_intr_reg */


/*===========================================================================

FUNCTION       DALINT_1X_INTR_DEREG

DESCRIPTION    This function deregisters the DAL Interrupt Controller

DEPENDENCIES   None

RETURN VALUE   boolean

SIDE EFFECTS   None

===========================================================================*/
boolean dalint_1x_intr_dereg(dalint_1x_client_name client_id,
                             dalint_1x_irq_types irq_vector)
{
  boolean status = TRUE;

  /* Return value from DAL interface */
  DALResult result = DAL_ERROR;

  /* Critical section initialization */
  if( dalint_1x.initialized == FALSE )
  {
    M1X_MSG( MUX, LEGACY_MED,
      "Critical section initialization done" );
    rex_init_crit_sect( &dalint_1x.crit_sect );
    dalint_1x.initialized = TRUE;
  }

  if((client_id < DALINT_1X_MAX_CLIENTS) && (irq_vector < DALINT_1X_MAX_IRQ))
  {
    /* Ensure that only one client can initialize and Register the
       DAL interrupt controller at a time*/
    DRV_ENTER_CRITICAL_SECTION(dalint_1x.crit_sect);

    if((dalint_1x.irq_info[irq_vector].client_mask != 0) &&
       (dalint_1x_irq_reg_by_client(client_id, irq_vector) == TRUE))
    {
      /* Check for valid handle and client state */
      if((dalint_1x.client_info[client_id].dalint_1x_handle != NULL) &&
         (dalint_1x.client_info[client_id].isClient_initialized == TRUE))
      {
        /* Unregister ISR -> Alternative for tramp_set_isr */
        result = DalInterruptController_Unregister(
                      dalint_1x.client_info[client_id].dalint_1x_handle,
                      dalint_1x.irq_info[irq_vector].irq_id);

        /* Error Checking */
        if((result != DAL_SUCCESS))
        {
          /* Make return status as False */
          status = FALSE;

          ONEX_ERR_FATAL( "Unable to de-register ISR, Client ID = %d, INTR ID = %d, "
                      "result = %d", client_id, irq_vector, result );
        }
        else
        {
          /* Clear the client's bit in the IRQ client mask */
          dalint_1x.irq_info[irq_vector].client_mask &= (~(0x01 << client_id));

          M1X_MSG( MUX, LEGACY_MED,
            "De-registered successfully, Client ID = %d, INTR ID = %d, "
            "result = %d",
            client_id,
            irq_vector,
            result );
        }
      }
    }
    /* Leave the Critical section after registration is done */
    DRV_LEAVE_CRITICAL_SECTION(dalint_1x.crit_sect);
  }
  else
  {
    /* Make return status as False */
    status = FALSE;
  }

  return status;

} /* dalint_1x_intr_dereg */


/*===========================================================================

FUNCTION       DALINT_1X_INTR_DEREG_ALL

DESCRIPTION    This function deregisters all the IRQs for a particular client

DEPENDENCIES   None

RETURN VALUE   boolean

SIDE EFFECTS   None

===========================================================================*/
boolean dalint_1x_intr_dereg_all(dalint_1x_client_name client_id)
{
  boolean status = TRUE;
  dalint_1x_irq_types irq_vector = DALINT_1X_FIRST_IRQ;

  if(client_id < DALINT_1X_MAX_CLIENTS)
  {
    for(irq_vector = DALINT_1X_FIRST_IRQ;
        irq_vector < DALINT_1X_MAX_IRQ;
        irq_vector++)
    {
      if(status == TRUE)
      {
        status = dalint_1x_intr_dereg(client_id, irq_vector);
        if(status == FALSE)
        {
          break;
        }
      }
    }
  }
  else
  {
    status = FALSE;

    ONEX_ERR_FATAL( "Client ID wrong, Client ID = %d", client_id, 0, 0 );
  }

  return status;

} /* dalint_1x_intr_dereg_all */


/*===========================================================================

FUNCTION       DALINT_1X_IRQ_REG_BY_CLIENT

DESCRIPTION    This function tells if the client uses a particular IRQ or not.

DEPENDENCIES   None

RETURN VALUE   boolean

SIDE EFFECTS   None

===========================================================================*/
boolean dalint_1x_irq_reg_by_client(dalint_1x_client_name client_id,
                                    dalint_1x_irq_types irq_vector)
{
  if((client_id < DALINT_1X_MAX_CLIENTS) && (irq_vector < DALINT_1X_MAX_IRQ))
  {
    return ((dalint_1x.irq_info[irq_vector].client_mask >> client_id) & 0x01);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Client ID or IRQ ID wrong, Client ID = %d, INTR ID = %d",
      client_id,
      irq_vector );
    return FALSE;
  }
} /* dalint_1x_irq_reg_by_client */


/*===========================================================================

FUNCTION       DALINT_1X_IRQ_PENDING

DESCRIPTION    This function returns whether an IRQ is pending or not.

DEPENDENCIES   Client should have registered for the IRQ

RETURN VALUE   boolean

SIDE EFFECTS   None

===========================================================================*/
boolean dalint_1x_irq_pending(dalint_1x_client_name client_id,
                              dalint_1x_irq_types irq_vector)
{
  boolean irq_pending = FALSE;  /* Return value */

  DALResult result = DAL_ERROR; /* Return value from DAL interface */
  uint32   irq_state = 0;       /* IRQ state from DAL */

  if((irq_vector < DALINT_1X_MAX_IRQ) && (client_id < DALINT_1X_MAX_CLIENTS))
  {
   /* Check for valid handle and client state */
    if((dalint_1x.client_info[client_id].dalint_1x_handle != NULL) &&
       (dalint_1x.client_info[client_id].isClient_initialized == TRUE))
    {
      result = DalInterruptController_IsInterruptPending(
                             dalint_1x.client_info[client_id].dalint_1x_handle,
                             dalint_1x.irq_info[irq_vector].irq_id,
                             &irq_state);

      /* Error Checking */
      if((result != DAL_SUCCESS))
      {
        /* Make return status as False */
        irq_pending = FALSE;

        ONEX_ERR_FATAL( "Unable to check IRQ status, "
                   "Client ID = %d, INTR ID = %d, result = %d",
                    client_id, irq_vector, result );
      }
      else if((result == DAL_SUCCESS)&&(irq_state == 1))
      {
        /* Make return status as True */
        irq_pending = TRUE;

        M1X_MSG( MUX, LEGACY_HIGH,
          "IRQ is pending: Client ID = %d, INTR ID = %d, result = %d",
          client_id,
          irq_vector,
          result );
      }
    }
  }
  else
  {
    /* Make return status as False */
    irq_pending = FALSE;

    M1X_MSG( MUX, LEGACY_ERROR,
      "Wrong Client ID or IRQ ID, Client ID=%d, INTR ID=%d, return=%d",
      client_id,
      irq_vector,
      irq_pending );
  }

  return irq_pending;

} /* dalint_1x_irq_pending */

