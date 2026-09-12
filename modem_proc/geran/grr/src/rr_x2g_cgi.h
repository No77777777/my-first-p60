#ifndef RR_X2G_CGI_H
#define RR_X2G_CGI_H

/*! \file rr_x2g_cgi.h 
 
  This module implements a state machine that handles CGI requests from other RATs.
 
                Copyright (c) 2011-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_x2g_cgi.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_LTE_TO_GSM_CGI

#include "comdef.h"
#include "rr_events.h"
#include "rr_task.h"
#include "geran_multi_sim.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/*!
 * \brief Initialise the module, allocating any memory as necessary.
 *  
 *  This function MUST be called before any of this module's other APIs are used.
 *  
 * \param gas_id 
 */
extern void rr_x2g_cgi_init(
  const gas_id_t gas_id
);

/*!
 * \brief Deinitialises the module, freeing any associated memory.
 * 
 * \param gas_id 
 */
extern void rr_x2g_cgi_deinit(
  const gas_id_t gas_id
);

/*!
 * \brief This is the main state machine for this module.
 * 
 * \param message - An input message for this module.
 * \param gas_id 
 */
extern void rr_x2g_cgi_control(
  rr_cmd_bdy_type * message,
  const gas_id_t gas_id
);

#endif /* FEATURE_LTE_TO_GSM_CGI */

#endif /* #ifndef RR_X2G_CGI_H */

/* EOF */

