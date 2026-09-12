#ifndef __HWKM_H__
#define __HWKM_H__

/*===========================================================================
  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**Enable this flag to allow run of standalone builds (without TZ) */
extern bool g_hwkm_modem_standalone;

typedef void* HWKM_HANDLE;

#define HWKM_MAX_KEY_SIZE                                                    \
    32 /* Maximum number of bytes in a key used in a KEY_SLOT_RDWR operation \
          */
#define HWKM_MAX_CTX_SIZE                                                    \
    64 /* Maximum number of bytes in a SW ctx used in a SYSTEM_KDF operation \
          */
#define HWKM_MAX_BLOB_SIZE                                              \
    68 /* Maximum number of bytes in a WKB used in a key wrap or unwrap \
          operation */

/* Opcodes to be set in the op field of a command */
typedef enum {
    NIST_KEYGEN = 0,   /* Opcode to generate a random key */
    SYSTEM_KDF,        /* Opcode to derive a key */
    QFPROM_KEY_RDWR,   /* Used only by HW */
    KEY_WRAP_EXPORT,   /* Opcode to wrap a key and export the wrapped key */
    KEY_UNWRAP_IMPORT, /* Opcode to import a wrapped key and unwrap it in the
                          specified key slot */
    KEY_SLOT_CLEAR,    /* Opcode to clear a slot */
    KEY_SLOT_RDWR,     /* Opcode to read or write a key from/to a slot */
    SET_TPKEY, /* Opcode to broadcast a TPKEY to all slaves configured to
                  receive a TPKEY */

    HWKM_MAX_OP,

    HWKM_UNDEF_OP = 0xFF
} hwkm_op_t;

/* Algorithm values which can be used in the alg_allowed field of the key policy
 */
typedef enum {
    AES128_ECB = 0,
    AES256_ECB = 1,
    DES_ECB = 2,
    TDES_ECB = 3,
    AES128_CBC = 4,
    AES256_CBC = 5,
    DES_CBC = 6,
    TDES_CBC = 7,
    AES128_CCM_TC = 8,
    AES128_CCM_NTC = 9,
    AES256_CCM_TC = 10,
    AES256_CCM_NTC = 11,
    AES256_SIV = 12,
    AES128_CTR = 13,
    AES256_CTR = 14,
    AES128_XTS = 15,
    AES256_XTS = 16,
    SHA1_HMAC = 17,
    SHA256_HMAC = 18,
    AES128_CMAC = 19,
    AES256_CMAC = 20,
    SHA384_HMAC = 21,
    SHA512_HMAC = 22,
    AES128_GCM = 23,
    AES256_GCM = 24,
    KASUMI = 25,
    SNOW3G = 26,
    ZUC = 27,
    PRINCE = 28,
    SIPHASH = 29,
    QARMA64 = 30,
    QARMA128 = 31,

    HWKM_ALG_MAX,

    HWKM_UNDEF_ALG = 0xFF
} hwkm_alg_t;

/* Key type values which can be used in the key_type field of the key policy */
typedef enum {
    KEY_DERIVATION_KEY = 0,
    KEY_WRAPPING_KEY = 1,
    KEY_SWAPPING_KEY = 2,
    TRANSPORT_KEY = 3,
    GENERIC_KEY = 4,

    HWKM_TYPE_MAX,

    HWKM_UNDEF_KEY_TYPE = 0xFF
} hwkm_type_t;

/* Destinations which a context can use */
typedef enum {
    KM_MASTER = 0,
    GPCE_SLAVE = 1,
    MCE_SLAVE = 2,
    PIMEM_SLAVE = 3,
    ICE0_SLAVE = 4,
    ICE1_SLAVE = 5,
    ICE2_SLAVE = 6,
    ICE3_SLAVE = 7,
    DP0_HDCP_SLAVE = 8,
    DP1_HDCP_SLAVE = 9,
    ICEMEM_SLAVE = 10,

    HWKM_DESTINATION_MAX,

    HWKM_UNDEF_DESTINATION = 0xFF
} hwkm_destination_t;

/* Key security levels which can be set in the security_lvl field of key policy
 */
typedef enum {
    SW_KEY = 0, /* Can be read by SW in plaintext using KEY_SLOT_RDWR cmd. */
    MANAGED_KEY = 1, /* Usable by SW, but not readable in plaintext. */
    HW_KEY = 2,      /* Not usable by SW. */

    HWKM_SECURITY_LEVEL_MAX,

    HWKM_UNDEF_SECURITY_LEVEL = 0xFF
} hwkm_security_level_t;

typedef struct {
    uint32_t ipcat_version;
    uint32_t key_policy_version;
    uint32_t key_table_size;
    bool tpkey_ready;
    uint8_t tpkey_slot;
    bool bist_failed;
} hwkm_info_t;

typedef struct {
    bool km_by_spu_allowed;
    bool km_by_modem_allowed;
    bool km_by_nsec_allowed;
    bool km_by_tz_allowed;

    hwkm_alg_t alg_allowed;

    bool enc_allowed;
    bool dec_allowed;

    hwkm_type_t key_type;
    uint8_t kdf_depth;

    bool wrap_export_allowed;
    bool swap_export_allowed;

    hwkm_security_level_t security_lvl;

    hwkm_destination_t hw_destination;

    bool wrap_with_tpk_allowed;
} hwkm_key_policy_t;

typedef struct {
    bool enabled;
    bool km_key_policy_ver_en;
    bool km_apps_secure_en;
    bool km_msa_secure_en;
    bool km_lcm_fuse_en;
    bool km_boot_stage_otp_en;
    bool km_swc_en;
    bool km_child_key_policy_en;
    bool km_mks_en;
    uint64_t km_fuse_region_sha_digest_en;
} hwkm_bsve_t;

typedef struct {
    uint8_t dks;              /* Destination Key Slot */
    hwkm_key_policy_t policy; /* Key policy */
} hwkm_keygen_cmd_t;

typedef struct {
    uint8_t slot;                   /* Key Slot */
    bool is_write;                  /* Whether this is a write or read op */
    hwkm_key_policy_t policy;       /* Key policy. Needed for is_write = true */
    uint8_t key[HWKM_MAX_KEY_SIZE]; /* Key. Needed for is_write = true */
    size_t sz; /* Length of key in bytes. Needed for is_write = true */
} hwkm_rdwr_cmd_t;

typedef struct {
    uint8_t dks; /* Destination Key Slot */
    uint8_t kdk; /* Key Derivation Key Slot */
    uint8_t mks; /* Mixing key slot. Only used if bsve.km_mks_en == true */
    hwkm_key_policy_t policy;       /* Key policy. */
    hwkm_bsve_t bsve;               /* Binding state vector */
    uint8_t ctx[HWKM_MAX_CTX_SIZE]; /* Context */
    size_t sz;                      /* Length of context in bytes */
} hwkm_kdf_cmd_t;

typedef struct {
    uint8_t sks; /* The slot to use as the TPKEY */
} hwkm_set_tpkey_cmd_t;

typedef struct {
    uint8_t dks;                     /* Destination Key Slot */
    uint8_t kwk;                     /* Key Wrapping Key Slot */
    uint8_t wkb[HWKM_MAX_BLOB_SIZE]; /* Wrapped Key Blob */
    uint8_t sz;                      /* Length of WKB in bytes */
} hwkm_unwrap_cmd_t;

typedef struct {
    uint8_t sks;      /* Destination Key Slot */
    uint8_t kwk;      /* Key Wrapping Key Slot */
    hwkm_bsve_t bsve; /* Binding state vector */
} hwkm_wrap_cmd_t;

typedef struct {
    uint8_t dks;        /* Destination key slot */
    bool is_double_key; /* Whether this is a double key */
} hwkm_clear_cmd_t;

typedef struct {
    hwkm_op_t op; /* Operation */

    union /* Structs with opcode specific parameters */
    {
        hwkm_keygen_cmd_t keygen;
        hwkm_rdwr_cmd_t rdwr;
        hwkm_kdf_cmd_t kdf;
        hwkm_set_tpkey_cmd_t set_tpkey;
        hwkm_unwrap_cmd_t unwrap;
        hwkm_wrap_cmd_t wrap;
        hwkm_clear_cmd_t clear;
    };
} hwkm_cmd_t;

typedef struct {
    hwkm_key_policy_t
        policy; /* Key policy. Only available for read operations */
    uint8_t key[HWKM_MAX_KEY_SIZE]; /* Only available for read operations */
    size_t sz;                      /* Length of the key (bytes) */
} hwkm_rdwr_rsp_t;

typedef struct {
    uint8_t wkb[HWKM_MAX_BLOB_SIZE]; /* Wrapping key blob */
    size_t sz;                       /* Length of the key blob (bytes) */
} hwkm_wrap_rsp_t;

typedef struct {
    uint32_t status;

    union /* Structs with opcode specific outputs */
    {
        hwkm_rdwr_rsp_t rdwr;
        hwkm_wrap_rsp_t wrap;
    };
} hwkm_rsp_t;

typedef enum {
    /** L1 KDKs. Not usable by SW. Used by HW to derive L2 KDKs */
    NKDK_L1 = 0,
    PKDK_L1 = 1,
    SKDK_L1 = 2,
    UKDK_L1 = 3,

    /** L2 KDKs, used to derive keys by SW. Cannot be used for crypto, only key
       derivation */
    TZ_NKDK_L2 = 4,
    TZ_PKDK_L2 = 43,        // the L2 key slots numbers for agatti is updated from 5-9
    TZ_SKDK_L2 = 44,        // to 43-47 due to the HW bug QCTDD06821880.
    MODEM_PKDK_L2 = 45,
    MODEM_SKDK_L2 = 46,
    TZ_UKDK_L2 = 47,

    /** Slots reserved for TPKEY */
    TPKEY_EVEN_SLOT = 10,
    TPKEY_KEY_ODD_SLOT = 11,

    /** Slots reserved for Swap key */
    TZ_SWAP_KEY_EVEN_SLOT = 12,
    TZ_SWAP_KEY_ODD_SLOT = 13,

    /** Reserved for wrapping keys to persist or unwrap keys */
    TZ_WRAP_KEY_EVEN_SLOT = 14,
    TZ_WRAP_KEY_ODD_SLOT = 15,

    /** Reserved for intermediate operations in IHWKeyFactory */
    TZ_GENERAL_PURPOSE_SLOT1 = 16,
    TZ_GENERAL_PURPOSE_SLOT2 = 17,

    /**
     * Reserved for privileged use cases which need to persist a key
     * and share it between execution environments.
     *
     * WARNING: Modifying these values may cause issues in execution
     * environments which depend on these specific slots being used for
     * privileged persistent use cases.
     */
    PERSISTENT_SHARED_SLOT_PAIR1 = 18,
    PERSISTENT_SHARED_SLOT_PAIR1_ODD = 19,
    PERSISTENT_SHARED_SLOT_PAIR2 = 20,
    PERSISTENT_SHARED_SLOT_PAIR2_ODD = 21,

    /** Reserved for mixing keys in KDF */
    TZ_MIXING_KEY_SLOT = 22,

    /** Reserved for intermediate operations in Modem */
    MSS_GENERAL_PURPOSE_SLOT1 = 24,

    UNDEF_SLOT = 0xFF
} hwkm_master_key_slots_t;

typedef enum {
    /* HW keys derived from L2 KDKs must be provisioned to these key slots to
       use for HW key operations */
    MSS_CRYPTO_PHK = 10,
    MSS_CRYPTO_SHK = 12,

    /* Default slot to use for TPKEY */
    MSS_CRYPTO_DEFAULT_TPKEY = 20,
    MSS_CRYPTO_DEFAULT_TPKEY_SECOND_SLOT = 21,

} hwkm_mce_key_slots_t;

/* These key slots are laid out in figure 3-4 of the CE HPG */
typedef enum {
    /* Slots 0-9 are empty. Using them will result in an error */
    CRYPTO_UNUSED_SLOTS_START = 0x00,
    CRYPTO_UNUSED_SLOTS_END = 0x09,

    /* Even pipe keys. For a pipe to be used, a key and policy must be
       provisioned to these key slots
       Each pipe uses a pair of slots */
    CRYPTO_ENCR_PIPE_KEY_FIRST = 0x0A,

    /* Odd pipe keys. For a pipe to be used, a key and policy must be
       provisioned to these key slots
       Each pipe uses a pair of slots */
    CRYPTO_ENCR_PIPE_ODD_KEY_FIRST = 0x1A,

    /* Auth pipe keys. For a pipe to be used with an auth key, a key and policy
       must be provisioned to these key slots
       Each pipe uses a pair of slots */
    CRYPTO_AUTH_PIPE_KEY_FIRST = 0x2A,

    /* HW keys derived from L2 KDKs must be provisioned to these key slots to
       use for HW key operations */
    CRYPTO_PHK = 0x3A,
    CRYPTO_SHK = 0x3C,

    /* Default slot to use for TPKEY */
    CRYPTO_DEFAULT_TPKEY = 0x3E,
    CRYPTO_DEFAULT_TPKEY_SECOND_SLOT = 0x3F,

    /** First key slot available for general purpose use cases */
    CRYPTO_GENERIC_SLOTS_START
} hwkm_gpce_key_slots_t;

/* Returns the key slot used for the given pipe's even key */
#define CRYPRO_ENCR_PIPE_EVEN_KEY(pipe) \
    (CRYPTO_ENCR_PIPE_KEY_FIRST + (2 * (pipe)))

/* Returns the key slot used for the given pipe's odd key */
#define CRYPRO_ENCR_PIPE_ODD_KEY(pipe) \
    (CRYPTO_ENCR_PIPE_ODD_KEY_FIRST + (2 * (pipe)))

/* Returns the key slot used for the given pipe's auth key */
#define CRYPRO_ENCR_PIPE_AUTH_KEY(pipe) \
    (CRYPTO_AUTH_PIPE_KEY_FIRST + (2 * (pipe)))

/**
 * @brief Creates a context for HWKM operation and returns a
 *        handle to the caller. It is the caller responsiblity
 *        to call hwkm_free() to properly clean up
 *        HWKM context and release the resources.
 *
 * @return Non zero value if successful.
 */
HWKM_HANDLE hwkm_new(void);

/**
 * @brief Clears HWKM context and releases internal resources.
 *
 * @param h      [in] handle to the HWKM context
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_free(HWKM_HANDLE h);

/**
 * @brief Initializes HWKM context in order to access an specific
 *        HWKM instance in SOC.
 *
 * @param h       [in] handle to the HWKM context
 * @param dest    [in] HWKM instance to use in this context
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_init(HWKM_HANDLE h, hwkm_destination_t dest);

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
int hwkm_add_req(HWKM_HANDLE h, hwkm_cmd_t* cmd, hwkm_rsp_t* rsp);

/**
 * @brief Get information about KM instance handled in this context
 *
 * @param h       [in]  handle to the HWKM context
 * @param info    [out] info about HWKM (versioning, key table size)
 *
 * @return HWKM_SUCCESS if successful. HWKM Error Code otherwise.
 */
int hwkm_get_info(HWKM_HANDLE h, hwkm_info_t* info);

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
int hwkm_run_cmds(HWKM_HANDLE h);

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
                        hwkm_key_policy_t* policy);
#endif