#ifndef TDSRF_LM__H
#define TDSRF_LM__H
/*===========================================================================

     T D - S C D M A    R  F   L I N K   M A N A G E R  I N T E R F A C E

DESCRIPTION
  This file contains global declarations and external references
  for TD-SCDMA L1 Link Manager.


EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2011 by Qualcomm Technologies, Incorporated. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsrf_lm.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/14/14   sk      New APIs for LM
11/22/13   sk      RxLM optimization phase 2 - CR56584  
09/05/13   kguo    reduced RxLM enum number
05/22/12   kguo    Added support for RxD 
08/29/11   asm     Removed function parameters for tdsrf_lm_deallocate_rxlm_buffers()
08/23/11   asm     Cleaned up and consolidated RF-related procedures in RF module
08/18/11   asm     Initial revision - Created RF Interface Definitions and Link Manager interfaces

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"

#include "lm_types.h"
#include "rxlm_intf.h"
#include "txlm_intf.h"

/*-------------------------------------------------------------------------*/
/*                        MACROS                                           */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/*                        TYPE DEFINITIONS                                 */
/*-------------------------------------------------------------------------*/
/* TDSCDMA L1 RF Interface Rx enums for each Link type */
typedef enum {
  TDSRF_RX_CHAIN_0_HOME,
  TDSRF_RX_CHAIN_0_BHO,
  TDSRF_RX_CHAIN_0_IRAT_GSM_NBR,
  TDSRF_RX_CHAIN_0_IRAT_LTE_NBR, 
  TDSRF_RX_CHAIN_1_IRAT_LTE_NBR,
  TDSRF_RX_CHAIN_1_HOME,
  TDSRF_RX_CHAIN_1_BHO,
  TDSRF_RX_CHAIN_MAX
} tdsrf_rx_link_enum_type;

#define TDSRF_RX_CHAIN_0_INTERF_NBR  TDSRF_RX_CHAIN_0_BHO 
#define TDSRF_RX_CHAIN_1_INTERF_NBR  TDSRF_RX_CHAIN_1_BHO 
#define TDSRF_RX_CHAIN_INVALID  0xFF

/* TDSL1 RF Interface Tx enums for each Link type */
typedef enum {
  TDSRF_TX_CHAIN_0_HOME,
  TDSRF_TX_CHAIN_0_BHO,
  TDSRF_TX_CHAIN_MAX
} tdsrf_tx_link_enum_type;

#define 	TDSRF_TX_CHAIN_INVALID 0xFF

/* To swap rx buffers, swap tx buffers, or swap rx and tx buffers at the same time (0ms BHO) */
typedef enum
{
  TDSRF_BUF_RX,
  TDSRF_BUF_TX,
  TDSRF_BUF_RXTX,
  TDSRF_BUF_MAX,
} tdsrf_buf_swap_t;

/* Internal data structure for each LM buffer common to Rx and Tx */
typedef struct {
  /* Buffer in use or free */
  boolean in_use;
  /* RxLM/TxLM buffer index */
  uint8 idx;

  /* LM tech type: Client and Operation */
  lm_tech_type cl_tech;
  lm_tech_type op_tech;
} tdsrf_lm_buf_type;

/* One global data structure for managing TDS internal RxLM and TxLM buffers */
typedef struct {
  tdsrf_lm_buf_type            rxlm_buf[TDSRF_RX_CHAIN_MAX];
  tdsrf_lm_buf_type            txlm_buf[TDSRF_TX_CHAIN_MAX];
} tdsrf_lm_type;

/* ------------------------------------------------------------------------*/
/*                     GLOBAL DATA DECLARATIONS                            */
/* ----------------------------------------------------------------------- */

/* TDSL1 RXLM/TXLM maintenance */
extern tdsrf_lm_type tdsrf_lm_global; 

extern uint8 not_in_use_rxlm_buffer_num;
extern uint8 deallocate_succ_rxlm_buffer_idx[TDSRF_RX_CHAIN_MAX];


/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION tdsrf_lm_init

DESCRIPTION
  Function initializes the TDSL1 RxLM and TxLM data structure.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_init(void);

/*===========================================================================
FUNCTION tdsrf_lm_init_rx_buffers

DESCRIPTION
  Function initializes the WL1 RxLM buffer array.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_init_rx_buffers(void);

/*===========================================================================
FUNCTION tdsrf_lm_init_tx_buffers

DESCRIPTION
  Function initializes the TDSL1 TxLM buffer array.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_init_tx_buffers(void);

/*===========================================================================
FUNCTION tdsrf_lm_rx_get_chain_and_tech

DESCRIPTION
  Function translates the Rx client into RxLM chain and tech type and returns 
  status.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if translation went through, FALSE otherwise

SIDE EFFECTS
  Input chain and tech will be written with the correct value for the client
  
===========================================================================*/
boolean tdsrf_lm_rx_get_chain_and_tech(
  /* Client to be translated */
  tdsrf_rx_link_enum_type client, 
  /* Chain for the client */
  rxlm_chain_type *chain, 
  /* Tech for the client */
  lm_tech_type  *tech);

/*===========================================================================
FUNCTION tdsrf_lm_allocate_rxlm_buffer

DESCRIPTION
  This function allocates required RxLM buffer.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_allocate_rxlm_buffer( tdsrf_rx_link_enum_type client );

/*===========================================================================
FUNCTION tdsrf_lm_allocate_txlm_buffer

DESCRIPTION
  Function allocates a buffer from TxLM for client specified.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_allocate_txlm_buffer( tdsrf_tx_link_enum_type client );

/*===========================================================================
FUNCTION tdsrf_lm_deallocate_rxlm_buffer

DESCRIPTION
  Deallocates a buffer from RxLM for client specified.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_deallocate_rxlm_buffer( tdsrf_rx_link_enum_type client );

/*===========================================================================
FUNCTION tdsrf_lm_deallocate_txlm_buffer

DESCRIPTION
  Deallocates a buffer from TxLM for client specified.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrf_lm_deallocate_txlm_buffer( tdsrf_tx_link_enum_type client );
#endif /* TDSRF_LM__H */
