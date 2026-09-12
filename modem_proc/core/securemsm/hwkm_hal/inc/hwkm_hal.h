#ifndef __HWKM_HAL_H__
#define __HWKM_HAL_H__

/*===========================================================================
  Copyright (c) 2019-2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "hwkm.h"
#include "msg.h"
#include "msg_mask.h"
#include "msgcfg.h"

// Command lengths (words)
#define NIST_KEYGEN_CMD_WORDS 4
#define SYSTEM_KDF_CMD_MIN_WORDS 4
#define SYSTEM_KDF_CMD_MAX_WORDS 29
#define KEYSLOT_CLEAR_CMD_WORDS 2
#define UNWRAP_IMPORT_CMD_WORDS 19
#define WRAP_EXPORT_CMD_WORDS 5
#define SET_TPKEY_CMD_WORDS 2
#define KEYSLOT_RDWR_CMD_WORDS 12
#define QFPROM_RDWR_CMD_WORDS 2

// Response lengths (words)
#define NIST_KEYGEN_RSP_WORDS 2
#define SYSTEM_KDF_RSP_WORDS 2
#define KEYSLOT_CLEAR_RSP_WORDS 2
#define UNWRAP_IMPORT_RSP_WORDS 2
#define WRAP_EXPORT_RSP_WORDS 19
#define SET_TPKEY_RSP_WORDS 2
#define KEYSLOT_RDWR_RSP_WORDS 12
#define QFPROM_RDWR_RSP_WORDS 2

// Field lengths (words)
#define OPERATION_INFO_WORDS 1
#define KEY_POLICY_WORDS 2
#define BSVE_WORDS 3
#define MAX_SWC_WORDS 16
#define RESPONSE_KEY_WORDS 8
#define KEY_BLOB_WORDS 17

// Field lengths (bytes)
#define OPERATION_INFO_LENGTH (OPERATION_INFO_WORDS * sizeof(uint32_t))
#define KEY_POLICY_LENGTH (KEY_POLICY_WORDS * sizeof(uint32_t))
#define MAX_BSVE_LENGTH (BSVE_WORDS * sizeof(uint32_t))
#define MAX_SWC_LENGTH (MAX_SWC_WORDS * sizeof(uint32_t))
#define RESPONSE_KEY_LENGTH (RESPONSE_KEY_WORDS * sizeof(uint32_t))
#define KEY_BLOB_LENGTH (KEY_BLOB_WORDS * sizeof(uint32_t))

// Command indices
#define COMMAND_KEY_POLICY_IDX 1
#define COMMAND_KEY_VALUE_IDX 3
#define COMMAND_WRAPPED_KEY_IDX 1
#define COMMAND_KEY_WRAP_BSVE_IDX 1
#define COMMAND_CTX_INDEX 6

// Response indices
#define RESPONSE_ERR_IDX 1
#define RESPONSE_KEY_POLICY_IDX 2
#define RESPONSE_KEY_VALUE_IDX 4
#define RESPONSE_WRAPPED_KEY_IDX 2

#define HWKM_GUARD(cond, err)                                                  \
  if (!(cond)) {                                                               \
    MSG_3(                                                                     \
        MSG_SSID_DFLT, MSG_LEGACY_ERROR,                                       \
        "SUCCESS assert failed in function %s line %d with err_code %d\n",     \
        __func__, __LINE__, err);                                              \
    return (err);                                                              \
  }

#define HWKM_GUARD_LOG_EXIT(cond, err)                                         \
  if (!(cond)) {                                                               \
    MSG_3(                                                                     \
        MSG_SSID_DFLT, MSG_LEGACY_ERROR,                                       \
        "SUCCESS assert failed in function %s line %d with err_code %d\n",     \
        __func__, __LINE__, err);                                              \
    goto __Exit;                                                               \
  }

#define HWKM_GUARD_EXIT(cond) \
    if (!(cond)) {            \
        goto __Exit;          \
    }

#define C_LENGTHOF(array) \
    ((void*)&(array) == (void*)(array) ? sizeof(array) / sizeof *(array) : 0)

#define GUARD(status)                   \
  do {                                  \
    int _st = (status);                 \
    if (_st != 0) return _st;           \
  } while (0)

#define CHECK(is_valid)                 \
  do {                                  \
    if (!(is_valid)) {                  \
      return 1;                         \
    };                                  \
  } while (0)

typedef struct {
    unsigned dbg_qfprom_key_rd_iv_sel : 1;             // [0]
    unsigned reserved0 : 1;                            // [1]
    unsigned wrap_with_tpkey : 1;                      // [2]
    unsigned hw_destination : 4;                       // [3:6]
    unsigned reserved1 : 1;                            // [7]
    unsigned propagate_sec_level_to_child_keys : 1;    // [8]
    unsigned security_level : 2;                       // [9:10]
    unsigned swap_export_allowed : 1;                  // [11]
    unsigned wrap_export_allowed : 1;                  // [12]
    unsigned key_type : 3;                             // [13:15]
    unsigned kdf_depth : 8;                            // [16:23]
    unsigned decrypt_allowed : 1;                      // [24]
    unsigned encrypt_allowed : 1;                      // [25]
    unsigned alg_allowed : 6;                          // [26:31]
    unsigned key_management_by_tz_secure_allowed : 1;  // [32]
    unsigned key_management_by_nonsecure_allowed : 1;  // [33]
    unsigned key_management_by_modem_allowed : 1;      // [34]
    unsigned key_management_by_spu_allowed : 1;        // [35]
    unsigned reserved2 : 28;                           // [36:63]
} __attribute__((packed)) hwkm_serialized_policy_t;

typedef struct {
    unsigned mks : 8;                         // [0:7]
    unsigned key_policy_version_en : 1;       // [8]
    unsigned apps_secure_en : 1;              // [9]
    unsigned msa_secure_en : 1;               // [10]
    unsigned lcm_fuse_row_en : 1;             // [11]
    unsigned boot_stage_otp_en : 1;           // [12]
    unsigned swc_en : 1;                      // [13]
    uint64_t fuse_region_sha_digest_en : 64;  // [14:78]
    unsigned child_key_policy_en : 1;         // [79]
    unsigned mks_en : 1;                      // [80]
    unsigned reserved : 16;                   // [81:95]
} __attribute__((packed)) hwkm_kdf_bsve_t;

typedef struct {
    unsigned key_policy_version_en : 1;       // [0]
    unsigned apps_secure_en : 1;              // [1]
    unsigned msa_secure_en : 1;               // [2]
    unsigned lcm_fuse_row_en : 1;             // [3]
    unsigned boot_stage_otp_en : 1;           // [4]
    unsigned swc_en : 1;                      // [5]
    uint64_t fuse_region_sha_digest_en : 64;  // [6:69]
    unsigned child_key_policy_en : 1;         // [70]
    unsigned mks_en : 1;                      // [71]
    unsigned reserved : 24;                   // [72:95]
} __attribute__((packed)) hwkm_wrapping_bsve_t;

typedef struct {
    unsigned op : 4;           // [0-3]
    unsigned irq_en : 1;       // [4]
    unsigned slot1_desc : 8;   // [5,12]
    unsigned slot2_desc : 8;   // [13,20]
    unsigned op_flag : 1;      // [21]
    unsigned context_len : 5;  // [22-26]
    unsigned len : 5;          // [27-31]
} __attribute__((packed)) hwkm_operation_info_t;

_Static_assert(sizeof(hwkm_serialized_policy_t) == KEY_POLICY_LENGTH,
               "Key policy struct length must be 2 words");
_Static_assert(sizeof(hwkm_kdf_bsve_t) == MAX_BSVE_LENGTH,
               "BSVE struct length must be 3 words");
_Static_assert(sizeof(hwkm_operation_info_t) == OPERATION_INFO_LENGTH,
               "Operation struct must be 1 word");

/**
 * @brief Perform configuration out of reset for HWKM control registers on
 * Master and all supported slaves
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_configure_registers_out_of_reset(void);

/**
 * @brief Enable TPKEY receive on all supported slaves and set TPKEY_DKS on all
 * slaves to default TPKEY slot for that slave
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_enable_tpkey_receive(void);

/**
 * @brief Disable TPKEY receive on all supported slaves
 *
 * @return HWKM_SUCCESS if successful. HWKW Error Code otherwise.
 */
int hwkm_disable_tpkey_receive(void);

/**
 * @brief Vote to enable clock for destination HWKM instance
 *
 * @param dest   [in] Destination instance
 *
 * @return HWKM_SUCCESS on success
 */
int hwkm_clk_enable(hwkm_destination_t dest);

/**
 * @brief Vote to disable clock for destination HWKM instance
 *
 * @param dest   [in] Destination instance
 *
 * @return HWKM_SUCCESS on success
 */
int hwkm_clk_disable(hwkm_destination_t dest);

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
                         size_t rsp_words);

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
int hwkm_get_instance_info(hwkm_destination_t dest, hwkm_info_t* info);

/**
 * @brief Checks if the device supports HWKM.
 *
 * @return true if HWKM is supported, false otherwise.
 */
bool hwkm_is_supported(void);

#endif
