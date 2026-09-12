#ifndef A2_COMMON_H
#define A2_COMMON_H
/*!
  @file a2_common.h

  @brief
   A header specifying common types needed by different parts of the A2.

*/


/*===========================================================================

  Copyright (c) 2014 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/a2/a2_common.h#3 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
05/11/20   av      CR 2680695: A2 MDUMP Changes
10/01/19   av      CR 2538294: HA 1.0 Demback ID changes
07/06/15   mm      CR 866981: Adding support for Linux MOB
11/25/14   ca      CR:762863: UL CA API Interface changes
                   required for RLC and MAC
08/12/14   ca      Added Key_stream_offset to a2_cipher_info_s
05/23/14   mm      Added A2_TECHNOLOGY_MAX
05/09/14   mm      Changed a2_dl_phy_demback_id_e -> a2_demback_id_e; added
                   A2_SUB_ID_MAX
01/13/14   ca      DSDA feature is implemented.
01/15/14   mm      Removed redundant dcinva instruction in a2_invalidate_dcache
07/10/13   vd      CR511894, Bug fixes to relocatable image
05/08/13   vd      CR489850: Relocatable Memory
04/17/13   yjz     Add a2_timetick_t, CR462233
03/20/13   ars     CR465878: Added checksum offload feature
01/23/13   ars     QMAP changes for muxing
11/26/12   ag      Change for QTF build
07/31/12   ar      changed A2_TECHNOLOGY_DEFAULT enum value to 0
03/19/12   ar      added A2_PER_MUX_HDR_OPEN_CMD_RES_BIT0 macro
08/28/12   ars     Fixed the feature around cache invalidate 
04/11/12   bn      Added in support for DIME
03/19/12   ar      added A2_PER_MUX_HDR_OPEN_CMD_RES_BIT0 macro
02/29/12   kp      Added code for security algo ZUC.
01/10/12   bn      Added support to enable/disable PC using MUX command
10/06/11   ar      added a2 power collapse support for TDSCDMA
03/11/11   sm      Removed "barrier" call from a2_invalidate_dcache()
03/07/11   yuw     Modified macro name
03/05/11   yuw     Added A2_PER_CSFB_CTL_SIG_DTR_MASK
02/08/11   yuw     Supported tethered RmNet and DUN call on CSFB Fusion
09/06/10   sm      changed A2_TECHNOLOGY_CDMA_1X to A2_TECHNOLOGY_DO
08/25/10   enj     change T_QDSP6 to __qdsp6__
08/06/10   ar      increased the A2_MAX_SRC_IOVEC_COUNT_PER_DMA_CHAIN count to
                   200 to address long ip packets chains due to small TBs
08/06/10   ar      CR 248847 fix: removed A2_TECHNOLOGY_INVALID technology enum
07/08/10   bn      Increased the number of dsm per chain in iov to 120
06/16/10   ar      exported A2_INVALIDATE_CACHE macro and corresponding
                   a2_invalidate_dcache api
05/04/10   bn      Added in support for A2 DL muxing (FEATURE_A2_FUSION)
12/22/09   yuw     Check in on-target changes for framer and deframer
12/09/09   sm      Added A2_MAX_SRC_IOVEC_COUNT_PER_DMA_CHAIN
11/09/09   ar      extended a2_technology_e to support CDMA 1x and default mode
08/14/09   yuw     Supports SWI_Release_2009_07_24
06/17/09   ar      fixed comments for a2_integrity_algo_e enum
06/01/09   ar      added integrity and ciphering related enums and structures
05/27/09   sm      Added a2_crc_size_e and a2_endian_e
05/14/09   yuw     Added a2_status_e to indicate the return status of the A2 
                   API functions
04/26/09   sm      Added a2_technology_e
04/22/09   sah     Add IP type to common header.
04/21/09   sm      Added some typedef/enums related to ciphering and iovec
03/20/09   sah     Add function pointer calback type.
01/20/09   sah     Add new hdr task
12/24/08   yg      Added new macro A2_BYTES_TO_WORDS_ROUNDUP.
08/08/08   ahe     Removed def to cell_id
07/11/08   sah     Initial Checkin
===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include <sys.h>
/*===========================================================================
                          EXTERNAL FUNCTION 
                          
===========================================================================*/
/*==============================================================================

  FUNCTION:  a2_qsh_update_mdump_info

==============================================================================*/
/*!
  @brief
  This function updates crash client & active rat and called before ERR_FATAL.
*/
/*============================================================================*/
void a2_qsh_update_mdump_info
(
  void
);
 
/*===========================================================================
                               MACROS
   
==========================================================================*/

#define A2_BYTES_TO_BITS(x) ((x)<<3)
#define A2_BITS_TO_BYTES(x) ((x)>>3)
#define A2_BITS_TO_BYTES_ROUNDUP(x) ((x+7)>>3)

/* @brief the maximum payload of the filltask in bytes */
#define A2_FILL_TASK_MAX_PAYLOAD 16

/*! @brief The maximum payload of the a2 hdr task in bits */
#define A2_HDR_TASK_MAX_PAYLOAD 32

#define A2_BITS_TO_WORDS(x) ((x)>>5)
#define A2_BYTES_TO_WORDS(x) ((x)>>2)
#define A2_BYTES_TO_WORDS_ROUNDUP(x) (((x)+3)>>2)

#define A2_WORDS_TO_BYTES(x) ((x)<<2)

/*! @brief Returns the number of bits of remainder when dividing by a
    word
 */
#define A2_BIT_WORD_REMAINDER(num_bits) ((num_bits)&0x1f)

/*!
  Maximum number of iovec (address+length) count per DMA chain
  TODO: need to optimize this number by either dropping long chains or by
  programming a2 dl per in parts. also this macro is common for multiple modules
  such as UL SEC(for hspa apis) and PER. Not all modules need high IOVEC COUNT
  */
#define A2_MAX_SRC_IOVEC_COUNT_PER_DMA_CHAIN 200

/*set the unique_id for dsm ptr*/
#define A2_COMMON_SET_DSM_PKT_UNIQUE_ID(dsm_packet_ptr, unique_id) \
           (dsm_packet_ptr)->kind = unique_id
/*get the unique_id for dsm ptr*/
#define A2_COMMON_GET_DSM_PKT_UNIQUE_ID(dsm_packet_ptr) \
           (dsm_packet_ptr)->kind
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/* @brief maximum number of cipher key indexes (valid range 0..15) */
#define A2_CIPHER_MAX_NUM_KEY_IDX   16

/*! cache line size in bytes  */
#define A2_CACHELINE_SIZE A2_CACHELINE_SIZE_BYTES

/*! cache line size in bytes  */
#define A2_CACHELINE_SIZE_BYTES 32


/*! @brief Aligns the address to the previous cache aligned address
  */
#define A2_CACHE_ALIGN(addr) \
  ((void*)(((uint32)(addr))& (~((A2_CACHELINE_SIZE_BYTES)-1))))

  /*! @brief current sub id in the system. This is used for the wrapper 
      fucntions. Once all other modules implement DSDA changes, this can be 
      removed*/
#define A2_SUB_ID_DEFAULT SYS_MODEM_AS_ID_1

  /*! @brief current demback bf for DL PHY HSPA in the system. This is used for the wrapper 
      fucntions. Once all other modules implement DSDA changes, this can be 
      removed*/
#define A2_DEMBACK_BF_DEFAULT_DL_PHY_HSPA    A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_0)

/*! @brief current demback bf for cat6  in the system. This is used for the wrapper 
    fucntions. Once all other modules implement DSDA changes, this can be 
    removed*/
#define A2_DEMBACK_BF_DEFAULT_CAT6           A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_0)

/*! @brief current demback bf for cat6  in the system. This is used for the wrapper 
    fucntions. Once all other modules implement DSDA changes, this can be 
    removed*/
#define A2_DEMBACK_BF_DEFAULT_UL_PHY_SEC  A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_INVALID)

/*!
  @brief
  The system and subframe number.

  @note  This may be a common structure and not defined by the a2.
  */
//typedef uint32 lte_sfn_t;

typedef enum
{
  /* A2 internal memory where Ciphering Keys, filters etc needs to be stored */
  A2_MEM_INTERNAL,
  /*! Encoder RAM */
  A2_MEM_ERAM,
  /*! Decoder RAM */
  A2_MEM_DOB,
  /*! external memory (where DSM items etc resides) */
  A2_MEM_EXTERNAL
} a2_mem_type_e;

/*!
  @brief enumeration to indicate which technology is using A2 HW
  */
typedef enum
{
  A2_TECHNOLOGY_DEFAULT,  /*!< DEFAULT tech (NONE) */
  A2_TECHNOLOGY_HSPA,     /*!< HSPA tech */
  A2_TECHNOLOGY_LTE,      /*!< LTE tech */
  A2_TECHNOLOGY_TDSCDMA,  /*!< TDSCDMA tech */
  A2_TECHNOLOGY_MAX
} a2_technology_e;

/*!
   @brief
   enum for dem back index
*/

typedef enum
{
  /*! demback id 0 */
  A2_DEMBACK_ID_0,
  /*! demback id 1 */
  A2_DEMBACK_ID_1,
  /*! the total number of dembacks in the system */
  A2_DEMBACK_ID_MAX = 1, /*<Removing 1 Demback>*/               
  /*! invalid demback id */   
  A2_DEMBACK_ID_INVALID = A2_DEMBACK_ID_MAX
} a2_demback_id_e;


/*!
   @brief
   macro to get hw inst bit mask from hw inst id */

#define A2_DEMBACK_ID_BMSK(demback_id) \
  ((demback_id == A2_DEMBACK_ID_INVALID) ? 0 : (1 << (demback_id)))

/*!
   @brief
   typedefinition for demback bf type - is is in bit fields.
*/
typedef uint8 a2_demback_bf_t;

/*!
   @brief
   typedefinition for system defined sub id type.
*/

typedef sys_modem_as_id_e_type a2_sub_id_t;

#define A2_SUB_ID_MAX   (SYS_MODEM_AS_ID_MAX - 1)

  /*!
     @brief
     macro validates the sub id to check if it is in the proper limits
  */
#define A2_VALIDATE_SUB_ID(sub_id) \
        ((sub_id > SYS_MODEM_AS_ID_NONE)&&(sub_id < SYS_MODEM_AS_ID_NO_CHANGE))
                          
/*!< maximum possible value for demback bf. */
#define  A2_DEMBACK_BF_MAX  ((1 << A2_DEMBACK_ID_MAX) - 1) 

/*!< invalid demback bf. Make sure it is not over flown. */
#define  A2_DEMBACK_BF_INVALID  (A2_DEMBACK_BF_MAX + 1)

  /*!
     @brief
     macro validates the demback id to check if it is in the proper limits
  */
#define A2_VALIDATE_DEMBACK_BF(demback_bf) (demback_bf & A2_DEMBACK_BF_MAX)

/*!
  @brief
  enumeration to indicate the return status of the A2 API functions
  Generic outcomes of A2 operations
*/
typedef enum
{
  A2_STATUS_SUCCESS = 0, /*!<   Operation successful */
  A2_STATUS_FAILURE      /*!< Operation failed due to err */
} a2_status_e;

/*!
  @brief
  enumeration for endianess of memory.
  Value of enumeration matches with the endianess setting of A2 SWI.
*/
typedef enum
{
  A2_ENDIAN_LITTLE = 0, /*!< Little Endian */
  A2_ENDIAN_BIG         /*!< Big Endian */
} a2_endian_e;

/*! @brief Which direction the ciphering parameters should be set for.
    @note These enums are same as HW defined definations. See a2_init_verify().
    @todo need to see how we can make sure that enum doesnt always result in
     4 bytes allocation on windows and above target. May be used PACKED. Can we
     avoid PACKED?
*/
typedef enum
{
  A2_CIPHER_DIRECTION_UL = 0,
  A2_CIPHER_DIRECTION_DL = 1,
} a2_cipher_direction_t;

/*! @brief Which direction the integrity parameters should be set for.
    @note These enums are same as HW defined definitions. See a2_init_verify().
    @todo need to see how we can make sure that enum doesnt always result in
    4 bytes allocation on windows and above target. May be used PACKED. Can we
    avoid PACKED?
*/
typedef enum
{
  A2_INTEGRITY_DIRECTION_UL = 0,
  A2_INTEGRITY_DIRECTION_DL = 1,
} a2_integrity_direction_t;

/*! @brief Ciphering Algorithm
    @note These enums are same as HW defined definations. See a2_init_verify().
    @todo need to see how we can make sure that enum doesnt always result in
     4 bytes allocation on windows and above target. May be used PACKED. Can we
     avoid PACKED?
*/
typedef enum
{
  /*! UMTS KASUMI; equivalent to uea1 */
  A2_CIPHER_ALGO_KASUMI       = 0,
  /*! UMTS SNOW-3G; equivalent to uea2 */
  A2_CIPHER_ALGO_UMTS_SNOW3G  = 1,
  /*! LTE SNOW-3G; equivalent to eea1 */
  A2_CIPHER_ALGO_LTE_SNOW3G   = 2,
  /*! LTE AES; equivalent to eea2 */
  A2_CIPHER_ALGO_AES          = 3,
  /*!< LTE ZUC; equivalent to eea3 or spare5 */
  A2_CIPHER_ALGO_ZUC         = 4,
  A2_CIPHER_ALGO_NONE         = 0x7, /*!< No Ciphering; equivalent to
                                          uea0/eea0 */
} a2_cipher_algo_e;

/*! @brief Integrity Algorithm
    @note These enums are same as HW defined definations. See a2_init_verify().
    @todo need to see how we can make sure that enum doesnt always result in
    4 bytes allocation on windows and above target. May be used PACKED. Can we
    avoid PACKED?
*/
typedef enum
{
  A2_INTEGRITY_ALGO_KASUMI      = 0,   /*!< UMTS KASUMI; equivalent to uea1 */
  A2_INTEGRITY_ALGO_LTE_SNOW3G  = 1,   /*!< LTE SNOW-3G; equivalent to eea1 */
  A2_INTEGRITY_ALGO_AES         = 2,   /*!< LTE AES; equivalent to eia2 */
  A2_INTEGRITY_ALGO_ZUC         = 3,   /*!< LTE AES; equivalent to eia3-v11xy 
                                         or spare5 */
  A2_INTEGRITY_ALGO_NONE        = 0x7, /*!< No integrity; equivalent to
                                            uia0/eia0 */
} a2_integrity_algo_e;

/*!
   @brief
   structure to hold sys fn, sub fn and cell id for a particular Harq id.
*/
typedef struct
{
  uint8   cell_id;
  uint16  sys_fn;
  uint16  sub_fn;
}a2_ul_phy_harq_id_fn_info_s;

/*!
   @brief
   structure to hold ciphering information
   NOTE: If cipher algo is set to A2_CIPHER_ALGO_NONE then remaining fields are
   don't care.
*/
typedef struct
{
  /*! The type of ciphering algorithm to use. If set to A2_CIPHER_ALGO_NONE
      then data will not be ciphered and remaining fields will be ignored */
  a2_cipher_algo_e       algo;
  /*! Ciphering key index. Valid values: 0..15 */
  uint8                  key_index;
  /*! radio_bearer_id minus 1. Valid values: 0..31 */
  uint8                  bearer_id;
  /*! The count-c to be used for ciphering, count-c is same as count for LTE */
  uint32                 count_c;
  /*!< The offset into the keystream in bytes from the start of the IP 
       Frame/RLC PDU which has been ciphered in the previous task */
  uint16                 key_stream_offset;
} a2_cipher_info_s;

/*!
   @brief
   structure to hold integrity information
   NOTE: If integrity algo is set to A2_INTEGRITY_ALGO_NONE then remaining fields
   are don't care.
*/
typedef struct
{
  /*! Type of integrity algorithm to use. If set to A2_INTEGRITY_ALGO_NONE then
   * integrity will not be computed and remaining fields will be ignored */
  a2_integrity_algo_e       algo;
  /*! integrity key index. Valid values: 0..15 */
  uint8                     key_index;
  /*! radio_bearer_id minus 1. Valid values: 0..31 */
  uint8                     bearer_id;
  /*! UL or DL direction */
  a2_integrity_direction_t  dir;
  /*! The count to be used for integrity */
  uint32                    count;
} a2_integrity_info_s;

/*!
  @brief
  structure to hold iovec (address and length)
*/
typedef struct
{
  uint32 addr;            /*!< address */
  uint32 length_in_bytes; /*!< The length in bytes */
} a2_iovec_t;

/*!
  @brief
  time type to replace timetick_type
*/
typedef uint32 a2_timetick_t;

/* @brief a void parameter/return value function pointer type.

   */
typedef void (*a2_vv_func_ptr_t)( void);

/*! @brief defined macro for the new port  */
#define A2_VIRTUAL_PORT       A2_ENDPOINT_EXT_MEM

/*! @brief defined error for function return  */
#define A2_ERR       -1

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifndef TEST_FRAMEWORK

/*===========================================================================

  FUNCTION:  a2_invalidate_dcache

===========================================================================*/
/*!
    @brief
    Invalidates data cache.

    @detail
    Invalidates mem_size bytes of memory from starting
    address physical_addr.

    @note:
      A) If number of bytes are not divisible by cache line size
         (for example 32 bytes), this function will invalidate alittle
         bit more (below is number of bytes to be invalidated)
         ceil(mem_size/cache_line_size)*cache_line_size. To be
         safe, mem_size should be a multiple of cache line size :-)

      B) This function assumps physical_addr is cache aligned. If
         physical_addr is not cache aligned, more data will be invalidated
         at the beginning and at the end (as long as physical_addr will
         be in a cache line, that line will be invalidated)

      c) If memory is both L1 and L2 cacheable, then two invalidates
         are needed. This is fixed in QDSP6 version>3. Currently
         L1 cacheable memory allocation (write through/write back),
         will be always L2 cacheable using CS (will be fixed later)

      d) Volatile has been added to each command to make sure
         that after calling this function, memory is correct and
         prevent any issue by compiler optimization.
         After moving to source directory, modem_fw_util.h has
         all these defined as static INLINE functions, and this
         function can be modified to follow WiMAX methodology.

      e) mem_size has to be non-zero

    @todo Verify barrier is necessary.

    @return
    memory will be invalidated
*/
/*=========================================================================*/
INLINE void a2_invalidate_dcache(uint32 physical_addr, uint32 mem_size)
{

  uint32 cur_cache_line;
  uint32 last_cache_line;

  /* The start of the first cache line */
  cur_cache_line = (uint32)A2_CACHE_ALIGN( physical_addr );

  /* The start address of the last cache line.  mem_size - 1 is used
     because bytes 0--(n-1) need to be flushed which translates to n bytes.
     */
  last_cache_line = (uint32)A2_CACHE_ALIGN( physical_addr + mem_size - 1 );

  /* Flush one cacheline at a time */
  for (;
      cur_cache_line <= last_cache_line;
      cur_cache_line += A2_CACHELINE_SIZE_BYTES)
  {
    /* use inline assembly code to do invalidate a cache line
       dcinva(Rs= (uint32 *) physical_addr); */
    asm volatile ("dcinva(%0)" : : "r" ((uint32 *)( cur_cache_line )));
  }
} /* a2_invalidate_dcache */

/*!
   @brief
    Macro to invalidate the cache.  This will be substituted with the OS
    mechanism at a later time

   @todo need to make sure whether simply doing invalidate is fine for
   destination address becasue there is a possibiliy that someone might have
   modified at the beginning and end of the destination. May be flush just
   the beginning and end cache lines for a given destination.
*/
#define A2_CACHE_INVALIDATE(start_addr, length_to_flush)  \
  a2_invalidate_dcache(start_addr, length_to_flush)

#else
#error code not present
#endif /* TEST_FRAMEWORK */

#endif /* A2_COMMON_H */

