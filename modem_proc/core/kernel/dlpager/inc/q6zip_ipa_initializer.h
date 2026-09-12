#pragma once
/**
   @file q6zip_ipa_initializer.h 
   @author anandj 
   @brief Interface of an initializer for Q6ZIP IPA
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "ipa_api.h"
#include "q6zip_iface.h"

/**
 * @brief Initialize the IPA initializer module
 * @param [in] dict Dictionary to be loaded for Q6ZIP RO unzip operations
 * @param [in] initialized User specified callback to be invoked to notify 
 *        initialization result
 */
extern void q6zip_ipa_initializer_init (
    q6zip_iovec_t const *  dict,
    q6zip_initialized_cb_t initialized
);

/**
 * @brief Notify Q6ZIP IPA initializer of an event and its associated status
 * 
 * @param [in] event IPA initialization event
 * @param [in] status Status associated with the event
 */
extern void q6zip_ipa_initializer_notify (
    ipa_zip_cb_event_type_e  event,
    ipa_zip_cb_status_type_e status
);

