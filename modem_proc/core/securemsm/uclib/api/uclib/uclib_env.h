#ifndef _UCLIB_ENV
#define _UCLIB_ENV

/* ====================================================================
 * Copyright (c) 2017 - 2021 Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary – Qualcomm Technologies, Inc.
 * ====================================================================
 */

/*===========================================================================

Unified Crypto Library - Environment APIs

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define UCLIB_ENV_VERSION_MAJOR   1
#define UCLIB_ENV_VERSION_MINOR   0
#define UCLIB_ENV_MAGIC_NUMBER    0xD0E1A2D3

/*===========================================================================
                 DEFINITIONS AND TYPE DECLARATIONS
 ===========================================================================*/
/** Environment Function Pointers */

/** CE HW Base Address */
typedef struct env_ce_hw_s
{
  uint32_t ce_base;
  uint32_t ce_bam_base;
  uint32_t ce_irq_num;
  uint32_t prng_base;
  uint32_t pka_base;
}env_ce_hw_t;

typedef enum
{
  ENV_CE_BASE                = 0x0,
  ENV_CE_BAM_BASE            = 0x1,
  ENV_CE_IRQ_NUM             = 0x2,
  ENV_PRNG_BASE              = 0x3,
  ENV_PKA_BASE               = 0x4,
  ENV_BASE_ADDR_INVALID      = 0x7FFFFFFF
}env_base_addr_var_t;

/** CE HW BW Configuration API */
// Level of clock frequence
typedef enum
{
  INACTIVE          = 0x0,
  LOW               = 0x1,
  MEDIUM            = 0x2,
  HIGH              = 0x3,
  CLKLEV_COUNT_MAX  = 0x4,
  CLKLEV_MAX_ENTRY  = 0x7FFFFFFF
}env_clklev;

// Voteable clock resource, will
// ensure to vote for all dependencies
typedef enum
{
  CE1                 = 0x1,
  CE2                 = 0x2,
  CE3                 = 0x3,
  PRNG                = 0x4,
  PKA                 = 0x5,
  CLK_RESOURCE_COUNT  = 0x6,
  CLK_RESOURCE_MAX    = 0x7FFFFFFF
}env_resource;

typedef enum
{
  GET               = 0x1,
  SET               = 0x2,
  CLR               = 0x3,
  INCR              = 0x4,
  DECR              = 0x5,
  OP_MAX            = 0x7FFFFFFF
}env_var_op;

typedef enum
{
  ENV_PRNG_INIT       = 0x1,
  ENV_INIT_VAR_COUNT  = 0x2,
  ENV_INIT_VAR_INVALID= 0x7FFFFFFF
}env_init_var;

/** Sets Clock frequency for CE HW */
typedef int (*env_set_bandwidth)(env_resource ce, env_clklev lvl, uint32_t flags);
typedef size_t (*env_clk_ref)(env_var_op operation, env_resource ce);

typedef struct env_clk_cfg_s
{
  env_resource      ce_clk;           /** Clock Resource Name */
  env_clklev        lvl;              /** Bandwidth level     */
}env_clk_cfg_t;

typedef struct env_clk_fp_s
{
  env_clk_ref       ref;              /** Reference count     */
  env_set_bandwidth set_bw_fp;        /** Mandatory Env. callback if HW crypto is supported */
}env_clk_fp_t;

/** Heap API's */
typedef void * (*env_malloc)(uint32_t);
typedef void (*env_free)(void *);
typedef void * (*env_realloc)(void *, uint32_t);
typedef struct env_mem_magic_num_s
{
  uint32_t *magic_num;
  uint16_t *magic_num_index_array;
  uint16_t  magic_num_index;
}env_mem_magic_num_t;

typedef struct env_static_mem_s
{
  uint32_t            magic_num[5];
  uint32_t            mem_desc[32] ;
  uint16_t            magic_num_index_array[6];
  env_mem_magic_num_t mem_magic_num;
  bool                mem_init_done;

  uint8_t             *mem_pool_ptr;
  size_t              mem_pool_size;
}env_static_mem_t;

typedef struct env_mm_s
{
  env_malloc       malloc_fp;          /** Mandatory Env. callback if heap management is supported in the image */
  env_free         free_fp;            /** Mandatory Env. callback if heap management is supported in the image */
  env_realloc      realloc_fp;         /** Mandatory Env. callback if heap management is supported in the image */

  env_static_mem_t *mem_mgr_ptr;       /** Mandatory pointer to Env. structure if heap management is not supported in the image */
}env_mm_t;

/** memory API's */
typedef void * (*env_memcpy)(void *dst, const void *src, size_t sz);
typedef void * (*env_memset)(void *str, int c, size_t n);
typedef int (*env_memcmp)(const void *str1, const void *str2, size_t n);
typedef int (*env_timesafe_memcmp)(const void *str1, const void *str2, size_t n);
typedef void * (*env_memmove)(void *dst, const void *src, size_t cpy_sz);

/** VA to PA conversion API */
typedef int (*env_vtop)(uintptr_t va, uintptr_t *pa);

/** DCache clean and invalidate API */
typedef void (*env_dcache_civ)(void *addr, size_t len);
/** DCache line size API */
typedef size_t (*env_dcache_sz)(void);

/** Memory barrier API */
typedef void (*env_mem_bar)(void);
/** Instruction barrier API */
typedef void (*env_ins_bar)(void);

typedef struct env_mu_s
{
  env_memcpy          memcpy_fp;           /** Mandatory Env. callback */
  env_memset          memset_fp;           /** Mandatory Env. callback */
  env_memcmp          memcmp_fp;           /** Mandatory Env. callback */
  env_timesafe_memcmp timesafe_memcmp_fp;  /** Mandatory Env. callback */
  env_memmove         memmove_fp;          /** Mandatory Env. callback */

  env_vtop            vtop_fp;             /** Mandatory Env. callback if HW crypto BAM mode is supported  */
  env_dcache_civ      dcache_civ_fp;       /** Mandatory Env. callback if HW crypto BAM mode is supported  */
  env_dcache_sz       dcache_sz_fp;        /** Mandatory Env. callback if HW crypto BAM mode is supported  */
  env_mem_bar         mem_barrier_fp;      /** Mandatory Env. callback if HW crypto BAM mode is supported  */
  env_ins_bar         inst_barrier_fp;     /** Mandatory Env. callback if HW crypto BAM mode is supported  */

}env_mu_t;

/** Mutex API's */
typedef int (*env_mutex_init)(void *lock);
typedef int (*env_mutex_lock)(void *lock);
typedef int (*env_mutex_release)(void *lock);
typedef struct env_mtx_cfg_s
{
  void*             lock;              /** Optional CE lock       */
}env_mtx_cfg_t;

typedef struct env_mtx_fp_s
{
  env_mutex_init    mutex_init_fp;     /** Optional Env. callback */
  env_mutex_lock    mutex_lock_fp;     /** Optional Env. callback */
  env_mutex_release mutex_release_fp;  /** Optional Env. callback */
}env_mtx_fp_t;

/** PRNG Access API's */
typedef int (*env_prng_enable_access)(void);
typedef int (*env_prng_disable_access)(void);
/** PRNG Get Data API */
typedef size_t (*prng_cb) (uint8_t *buf, size_t buf_sz);

typedef struct env_prng_cfg_s
{
  bool                    prng_ac_en;  /** Mandatory flag         */
  bool                    init_done;
  env_clk_cfg_t           prng_clk;
  uint8_t                 ee;             /** PRNG module number: TZ = 1, Boot = 2 and Max EE Number is 15           */
  void*                   lock;           /** Mandatory for the multi-thread in UEFI                                 */
  uint32_t                ver;            /** Init with 0 and to be set from RNG_CM_PRNG_HW_VERSION register reading */
}env_prng_cfg_t;

typedef struct env_prng_fp_s
{
  env_prng_enable_access  prng_en_fp;  /** Optional Env. callback */
  env_prng_disable_access prng_dis_fp; /** Optional Env. callback */
  prng_cb                 prng_cb_fp;  /** Mandatory Env. callback if UCLIB is integrated in user space image */
  env_mutex_lock          prng_lock_fp;        /** Mandatory for the multi-thread in UEFI         */
  env_mutex_release       prng_release_fp;     /** Mandatory for the multi-thread in UEFI         */
}env_prng_fp_t;

/** Log API's */
typedef void (*env_log)(const char* fmt, va_list ap);

typedef enum
{
  ENV_BAM_TX_DESC           = 0x0,
  ENV_BAM_RX_DESC           = 0x1,
  ENV_BAM_RSLT              = 0x2,
  ENV_BAM_CONTAINER         = 0x3,
  ENV_BAM_CMD_DESC_BUF      = 0x4,
  ENV_BAM_ADDR_COUNT        = 0x5,
  ENV_BAM_ADDR_INVALID      = 0x7FFFFFFF
}env_bam_var_t;

/** BAM API's */
typedef int (*event_fp)(void);
typedef void (*reg_dump_init_fp)(void *);

typedef struct env_bam_ctx_cfg_s
{
  void         *bam_drv_ctx;
  uint8_t      *tx_desc;                    /** Mandatory if BAM Engine is used. Uncached Memory with 8-byte alignment               */
  uint8_t      *rx_desc;                    /** Mandatory if BAM Engine is used. Uncached Memory with 8-byte alignment               */
  uint8_t      *rslt;                       /** Mandatory if BAM Engine is used. Size: 256 bytes with cache line size alignment      */
  uint8_t      *container;                  /** Mandatory if BAM Engine is used. Size: 256 bytes with cache line size alignment      */

  uint8_t      *cmd_desc_buf;               /** Mandatory if BAM Engine is used. Size is dependent on the max number of commands we
                                                expect to write to the CE engine for a single operation                              */
  size_t       cmd_desc_buf_sz;             /** Mandatory if BAM Engine is used. Size of command descriptor buffer                   */
  size_t       desc_sz;                     /** Mandatory if BAM Engine is used. Size in bytes. Must be multiple of 8 bytes          */
  bool         uses_550_features;           /** Indicates whether CE 5.5.0 features (pipe keys, pattern processing) are required.    */
  bool         cmds_over_hwio;              /** Indicates if need to use HWIO for sending command while use BAM for data             */
  uint8_t      default_pipeset_num;         /** Indicates the default pipe set num. 0 - for TZ; 2 - for uefi                         */
  size_t       max_poll_count;              /** Indicates the max. iterations to poll on a BAM pipe, per descriptor                  */

}env_bam_ctx_cfg_t;

typedef struct env_bam_ctx_fp_s
{
  /** BAM interrupt mode */
  event_fp              event_create;    /** Mandatory if BAM is used in interrupt mode */
  event_fp              event_delete;    /** Mandatory if BAM is used in interrupt mode */
  event_fp              event_trigger;   /** Mandatory if BAM is used in interrupt mode */
  event_fp              event_wait;      /** Mandatory if BAM is used in interrupt mode */
  reg_dump_init_fp      reg_dump_init;   /** For dumping crypto and bam registers upon crash */
}env_bam_ctx_fp_t;

typedef enum
{
  PKA_CMD_BIT_SERIAL_MOD,
  PKA_CMD_BIT_SERIAL_MOD_DP,
  PKA_CMD_C25519_PMULT,
  PKA_CMD_CALC_MP,
  PKA_CMD_CALC_R_INV,
  PKA_CMD_CALC_R_SQR,
  PKA_CMD_CRT,
  PKA_CMD_CRT_KEY_SETUP,
  PKA_CMD_ED25519_PADD,
  PKA_CMD_ED25519_PDBL,
  PKA_CMD_ED25519_PMULT,
  PKA_CMD_ED25519_PVER,
  PKA_CMD_ED25519_SHAMIR,
  PKA_CMD_IS_A_M3,
  PKA_CMD_IS_P_EQUAL_Q,
  PKA_CMD_IS_P_REFLECT_Q,
  PKA_CMD_MODADD,
  PKA_CMD_MODDIV,
  PKA_CMD_MODEXP,
  PKA_CMD_MODINV,
  PKA_CMD_MODMULT,
  PKA_CMD_MODSUB,
  PKA_CMD_PADD,
  PKA_CMD_PADD_STD_PRJ,
  PKA_CMD_PDBL,
  PKA_CMD_PDBL_STD_PRJ,
  PKA_CMD_PMULT,
  PKA_CMD_PVER,
  PKA_CMD_REDUCE,
  PKA_CMD_SHAMIR,
  PKA_CMD_STD_PRJ_TO_AFFINE,
  PKA_CMD_MODMULT_521,
  PKA_CMD_MULT,
  PKA_CMD_M_521_MONTMULT,
  PKA_CMD_PADD_521,
  PKA_CMD_PDBL_521,
  PKA_CMD_PMULT_521,
  PKA_CMD_PVER_521,
  PKA_CMD_SHAMIR_521,
  PKA_CMD_LAST
} pka_cmd_t;

typedef struct
{
  size_t        key_size_bits;

  const uint8_t *p;
  const uint8_t *a;
  const uint8_t *b;
  const uint8_t *g_x;
  const uint8_t *g_y;
  const uint8_t *n;

  const uint8_t *m_prime;
  const uint8_t *r_sqr_mod_m;
  const uint8_t *n_prime;
  const uint8_t *r_sqr_mod_n;
} ec_curve_t;

typedef struct env_pka_hw_s
{
  void *               pka_info;
  short                ee;
  bool                 always_reload_fw;

  const uint32_t *     fw_arr;
  size_t               fw_arr_len;
  const uint16_t *     entry_points_arr;
  size_t               entry_points_arr_len;
  const ec_curve_t *   pka_curve_info_arr;
  size_t               pka_curve_info_arr_len;

  env_clk_cfg_t        pka_clk;
} env_pka_hw_cfg_t;

typedef enum
{
  ENV_INIT_BAM_DRV_CTX       = 0x0,
  ENV_INIT_PKA_INFO          = 0x1,
  ENV_INIT_DATA_INVALID      = 0x7FFFFFFF
}env_init_data_var_t;

typedef enum
{
  CRYPTO_OPERATION_AES = 0,
  CRYPTO_OPERATION_ECC = 1,
  CRYPTO_OPERATION_ECDH = 2,
  CRYPTO_OPERATION_MAC = 3,
  CRYPTO_OPERATION_KDF = 4,
  CRYPTO_OPERATION_RSA = 5,
  CRYPTO_OPERATION_SHA = 6,
  CRYPTO_OPERATION_TDES = 7,
  CRYPTO_OPERATION_SM2 = 8,
  CRYPTO_OPERATION_INVALID
} crypto_operation_enum;

typedef bool (*env_is_crypto_operation_allowed)(crypto_operation_enum operation);
typedef bool (*env_is_fips_enabled)(void);

/** Crypto Yield API */
typedef void (*env_crypto_yield)(bool);

/** Busywait */
typedef void (*env_busywait_fp)(uint32_t us);

/* Environment interface for Hardware Key Manager */
typedef struct env_hwkm_key_info_s
{
  const uint8_t *key;
  size_t key_sz;
  uint8_t pipe;
  bool odd_idx;
  bool auth_key;
  bool encr_key;
} env_hwkm_key_info_t;

typedef int (*env_hwkm_install_fp)(env_hwkm_key_info_t *key);

typedef int (*env_remote_ecc_verify_fp)(int curve, const uint8_t* pubkey, size_t pubkeylen,
                                        const uint8_t* hash, size_t hashlen,
                                        const uint8_t* sig, size_t siglen);

typedef int (*env_remote_ecc_sign_fp)(int curve, const uint8_t* privkey, size_t privkeylen,
                                      const uint8_t* hash, size_t hashlen,
                                      uint8_t* sig, size_t siglen, size_t* lenout);

typedef struct env_remote_fp_s
{
  env_remote_ecc_verify_fp ecc_verify;
  env_remote_ecc_sign_fp ecc_sign;
} env_remote_fp_t;

/** Callback Function API's */
typedef const struct env_fp_s
{
  /** Clock Callback Function API's */
  env_clk_fp_t clk_fp;

  /** Heap Callback Function API's */
  env_mm_t mm;

  /** memory Callback Function API's */
  env_mu_t mem;

  /** Mutex Callback Function API's */
  env_mtx_fp_t mtx_fp;

  /** PRNG Callback Function Access API's */
  env_prng_fp_t prng_fp;

  /** Log Callback Function API's */
  env_log log_fp;

  /** BAM Callback Function API's */
  env_bam_ctx_fp_t bam_fp;

  /** selftest Callback Function API's */
  env_is_crypto_operation_allowed is_crypto_operation_allowed_fp;
  env_is_fips_enabled is_fips_enabled_fp;

  /** Crypto Yield Callback Function API */
  env_crypto_yield crypto_yield_fp;

  env_busywait_fp busywait;

  env_hwkm_install_fp install_hw_key;

  /** Remote engine callbacks */
  env_remote_fp_t remote;
} env_fp_t;

/** Configurable env struct */
typedef struct env_cfg_s
{
  /** CE HW Base Address */
  env_ce_hw_t ce;

  /** Clock API's */
  env_clk_cfg_t clk;

  /** Mutex API's */
  env_mtx_cfg_t mtx;

  /** PRNG Access API's */
  env_prng_cfg_t prng;

  /** BAM Driver Buffer Allocations */
  env_bam_ctx_cfg_t bam;

  env_pka_hw_cfg_t pka;
} env_cfg_t;

struct env_s
{
  uint32_t ver;
  uint32_t magic;

  env_fp_t env_fp;

  env_cfg_t *env_cfg_ptr;
};

typedef const struct env_s env_t;

/**
 * @brief Returns pointer to environment structure that has been
 *        initialized by the image.
 *
 * @return pointer to environment structure.
 */
env_t * ucenv_get_env(void);

#endif /* _UCLIB_ENV */
