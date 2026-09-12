/*===========================================================================
  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "hwkm_hal.h"
#include "hwkm.h"
#include "hwkm_err.h"

#define UNUSED_VARIABLE(var) (void) var

/**
 * @brief Vote to enable clock for destination HWKM instance
 *
 * @param dest   [in] Destination instance
 *
 * @return HWKM_SUCCESS on success
 */
int hwkm_clk_enable(hwkm_destination_t dest)
{
    UNUSED_VARIABLE(dest);
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Vote to disable clock for destination HWKM instance
 *
 * @param dest   [in] Destination instance
 *
 * @return HWKM_SUCCESS on success
 */
int hwkm_clk_disable(hwkm_destination_t dest)
{
    UNUSED_VARIABLE(dest);
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Perform configuration out of reset for HWKM control registers on
 * Master and all supported slaves
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_configure_registers_out_of_reset(void)
{
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Enable TPKEY receive on all supported slaves and set TPKEY_DKS on all
 * slaves to default TPKEY slot for that slave
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_enable_tpkey_receive(void)
{
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Disable TPKEY receive on all supported slaves
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_disable_tpkey_receive(void)
{
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Read HWIO Shared RG registers to get info selected instance
 *        Info includes IPCAT version, Key Policy version, and number of key
 * slots
 *
 * @param dest            [in]  Destination KM instance
 * @param info            [out] Struct to write HWKM info to
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_get_instance_info(hwkm_destination_t dest, hwkm_info_t* info)
{
    UNUSED_VARIABLE(dest);
    UNUSED_VARIABLE(info);
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Send a command packet to the selected KM instance and read the
 * response
 *
 * @param dest            [in]  Destination KM instance
 * @param cmd_packet      [in]  pointer to start of command packet
 * @param cmd_words       [in]  words in the command packet
 * @param rsp_packet      [out] pointer to start of response packet
 * @param rsp_words       [in]  words in the response buffer
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_run_transaction(hwkm_destination_t dest, const uint32_t* cmd_packet,
                         size_t cmd_words, uint32_t* rsp_packet,
                         size_t rsp_words)
{
    UNUSED_VARIABLE(dest);
    UNUSED_VARIABLE(cmd_packet);
    UNUSED_VARIABLE(cmd_words);
    UNUSED_VARIABLE(rsp_packet);
    UNUSED_VARIABLE(rsp_words);
    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Checks if the device supports HWKM.
 *
 * @return true if HWKM is supported, false otherwise.
 */
bool hwkm_is_supported(void)
{
    return false;
}

/**
 * @brief Initialization sequence for HWKM out of reset.
 *
 * @return HWKM_SUCCESS on success
 */
int hwkm_initialize_out_of_reset(void)
{
    return HWKM_SUCCESS;
}

/**
 * @brief Creates a context for HWKM operation and returns a
 *        handle to the caller. It is the caller responsiblity
 *        to call hwkm_free() to properly clean up
 *        HWKM context and release the resources.
 *
 * @return Non zero value if successful.
 */
HWKM_HANDLE hwkm_new(void){
    return NULL;
}

/**
 * @brief Clears HWKM context and releases internal resources.
 *
 * @param h      [in] handle to the HWKM context
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_free(HWKM_HANDLE h){
    UNUSED_VARIABLE(h);

    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Initializes HWKM context in order to access an specific
 *        HWKM instance in SOC.
 *
 * @param h       [in] handle to the HWKM context
 * @param dest    [in] HWKM instance to use in this context
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_init(HWKM_HANDLE h, hwkm_destination_t dest){
    UNUSED_VARIABLE(h);
    UNUSED_VARIABLE(dest);

    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Add a new request to the list of CMD/RSP pair of
 *        the HWKM context.
 *
 * @param h       [in]  handle to the HWKM context
 * @param cmd     [in]  Pointer to CMD descriptor
 * @param rsp     [in]  Pointer to RSP descriptor
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_add_req(HWKM_HANDLE h, hwkm_cmd_t* cmd, hwkm_rsp_t* rsp){
    UNUSED_VARIABLE(h);
    UNUSED_VARIABLE(cmd);
    UNUSED_VARIABLE(rsp);

    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Get information about KM instance handled in this context
 *
 * @param h       [in]  handle to the HWKM context
 * @param info    [out] info about HWKM (versioning, key table size)
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_get_info(HWKM_HANDLE h, hwkm_info_t* info){
    UNUSED_VARIABLE(h);
    UNUSED_VARIABLE(info);

    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Run queued commands. Commands will be executed in the
 *        order that they are recieved thru hwkm_add_req() API.
 *        If a command causes a HW error, an error will be returned
 *        and the queue will be cleared. Otherwise the response's status
 *        field should be checked to see if the command succeeded.
 *
 * @param h       [in] handle to the HWKM context
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_run_cmds(HWKM_HANDLE h){
    UNUSED_VARIABLE(h);

    return HWKM_NOT_SUPPORTED;
}

/**
 * @brief Extract the policy for a key from a wrapped key blob
 *
 * @param wkb      [in]  pointer to the wrapped key blob
 * @param wkb_size [in]  size of the wrapped key blob in bytes
 * @param policy   [out] policy extracted from wkb
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_extract_policy(const uint8_t* wkb, size_t wkb_size,
                        hwkm_key_policy_t* policy){
    UNUSED_VARIABLE(wkb);
    UNUSED_VARIABLE(wkb_size);
    UNUSED_VARIABLE(policy);

    return HWKM_NOT_SUPPORTED;
}