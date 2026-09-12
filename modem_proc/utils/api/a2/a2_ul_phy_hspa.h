#ifndef LTE_A2_UL_PHY_HSPA_H
#define LTE_A2_UL_PHY_HSPA_H

/*!
  @file a2_ul_phy_hspa.h

  @brief
  The interface to the uplink phy portion of the a2 driver for HSPA.

  Assumption is that all the APIs in this file will be called from the same
  context and hence there is no concurrency issues addressed within these APIs.


  @ingroup a2_ext_interface
*/

/*==============================================================================

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/a2/a2_ul_phy_hspa.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/13/14   ca      DSDA feature is implemented.
10/29/23   ars     Update FW PDU done count and make A2 DONE ISR optional
10/09/13   ar      move internal api prototypes to src folder header files 
09/16/13   mm      Updated api function prototypes with ERAM location changes
05/30/13   ars     Increasing max source iovec count to 40
05/22/13   ars     DCHSUPA Interface Changes
11/13/12   ars     Added new API for HSPA harq re-tx
08/08/12   ars     DIME PHY changes after RUMI validation 
10/06/11   ar      added a2 power collapse support
11/17/09   ar      fixed Q6 compiler warnings
11/02/09   ar      removed a2_ul_phy_hspa_set_technology()
10/04/09   sm      Removed feature FEATURE_A2_UL_PHY_HSPA_TEST
08/03/09   ar      externed a2_ul_phy_hspa_test_init()
05/27/09   sm      Initial version

==============================================================================*/


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <dsm.h>
#include <a2_common.h>
#include <comdef.h>
#include <a2_ul_phy.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*==============================================================================

                             MACROS

==============================================================================*/

/*!
  @brief Maximum number of source iovec (address+length) count
  */
#define A2_UL_PHY_HSPA_MAX_SRC_IOVEC_COUNT 40

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_register_dma_from_eram_cb(dma_from_eram_cb_fn_ptr)  \
  a2_ul_phy_hspai_register_dma_from_eram_cb(dma_from_eram_cb_fn_ptr)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_dma_from_eram_and_commit(src_addr, src_offset_in_bits,  \
  src_len_in_bytes, dest_addr, interrupt_reqd, user_data)  \
  a2_ul_phy_hspai_dma_from_eram_and_commit(A2_SUB_ID_DEFAULT, src_addr,  \
  src_offset_in_bits, src_len_in_bytes, dest_addr, interrupt_reqd, user_data)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_register_harq_commit_cb(harq_commit_cb_fn_ptr)  \
  a2_ul_phy_hspai_register_harq_commit_cb(harq_commit_cb_fn_ptr)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_harq_init()  \
  a2_ul_phy_hspai_harq_init(A2_SUB_ID_DEFAULT)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_write_first_fill_task(source_address, dest_address,  \
  length_in_bits)                                                          \
  a2_ul_phy_hspai_write_first_fill_task(A2_SUB_ID_DEFAULT, source_address,    \
  dest_address, length_in_bits)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_write_and_commit_fill_task(dest_addr, data, size_in_bytes) \
  a2_ul_phy_hspai_write_and_commit_fill_task(A2_SUB_ID_DEFAULT, dest_addr, data, size_in_bytes)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_write_fill_task(source_address, length_in_bits)  \
  a2_ul_phy_hspai_write_fill_task(A2_SUB_ID_DEFAULT,source_address, length_in_bits)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_gather_and_cipher(cipher_info_ptr, num_src_iovec, \
  src_iovec_list)                                                        \
  a2_ul_phy_hspai_gather_and_cipher(A2_SUB_ID_DEFAULT, cipher_info_ptr,     \
  num_src_iovec,src_iovec_list)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_dma_ext_mem_to_eram(source_address, src_len_in_bytes, \
  dest_address)                                                              \
  a2_ul_phy_hspai_dma_ext_mem_to_eram(A2_SUB_ID_DEFAULT, source_address,        \
  src_len_in_bytes, dest_address)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_write_padding_bits(num_pad_bits)  \
  a2_ul_phy_hspai_write_padding_bits(A2_SUB_ID_DEFAULT, num_pad_bits)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA changes,
  this macro can take an extra argument as sub_id.
  */
#define a2_ul_phy_hspa_harq_commit(tb_size_in_bits, interrupt_reqd,  \
  user_data)                                                        \
  a2_ul_phy_hspai_harq_commit(A2_SUB_ID_DEFAULT, tb_size_in_bits, interrupt_reqd, \
  user_data)


/*==============================================================================

                       STRUCTURES AND UNIONS

==============================================================================*/

/*!
  @brief
  structure to hold ciphering information

  @note
  If cipher_algo is set to A2_CIPHER_ALGO_NONE then remaining fields are dont
  care.
*/
typedef struct
{
  /*! The type of ciphering algorithm to use. If set to A2_CIPHER_ALGO_NONE
      then data will not be ciphered and remaining fields will be ignored */
  a2_cipher_algo_e algo;
  /*! Offset in bytes from where data needs to be ciphered using ciphering
      configuration indicated in this structure. (eg. set it to RLC static
      header size for RLC PDU) */
  uint16           ciph_start_offset_in_bytes;
  /*! Key-stream offset in bytes. Needs to be set to 0 by default. For future
      use only. */
  uint16           ciph_key_stream_offset_in_bytes;
  /*! Ciphering key index. Valid values: 0..5 */
  uint8            key_index;
  /*! radio_bearer_id minus 1. Valid values: 0..31 */
  uint8            bearer_id;
  /*! The count-c to be used for ciphering */
  uint32           count_c;
} a2_ul_phy_hspa_cipher_info_t;

/*!
  @brief
  Enum to indicate an area in ERAM 
  area_id0 : ERAM bank 0 carrier 0  
  area_id1 : ERAM bank 1 carrier 0 
  area_id2 : ERAM bank 0 carrier 1 
  area_id3 : ERAM bank 1 carrier 1 

*/
typedef enum
{
  /* ERAM bank 0 carrier 0 */
  A2_UL_PHY_HSPA_AREA_0 = 0,
  /* ERAM bank 1 carrier 0 */
  A2_UL_PHY_HSPA_AREA_1,
  /* ERAM bank 0 carrier 1 */
  A2_UL_PHY_HSPA_AREA_2,
  /* ERAM bank 1 carrier 1 */
  A2_UL_PHY_HSPA_AREA_3,
  /* TD area 0 */
  A2_UL_PHY_TDSCDMA_AREA_0,
  /* TD area 1 */
  A2_UL_PHY_TDSCDMA_AREA_1,
  /* Max for boundary */
  A2_UL_PHY_HSPA_AERA_MAX
}a2_ul_phy_hspa_area_id_e;

typedef void (*a2_ul_phy_hspa_harq_commit_cb)(uint32);

typedef void (*a2_ul_phy_hspa_dma_from_eram_cb) (uint32);

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/
/* #ifdef FEATURE_A2_UL_PHY_HSPA_TEST */

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_register_dma_from_eram_cb

==============================================================================*/
/*!
  @brief
  Registers a callback function to be called whenever tag/interrupt task written
  from function a2_ul_phy_hspa_commit_dma_from_eram() is processed by A2 UL PHY.
*/
/*============================================================================*/
void a2_ul_phy_hspai_register_dma_from_eram_cb
(
  /*! The callback to be called whenever dma_from_eram tag status is received */
  a2_ul_phy_hspa_dma_from_eram_cb dma_from_eram_cb_fn_ptr
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_dma_from_eram_and_commit

==============================================================================*/
/*!
    @brief
    Adds following tasks to A2 UL PHY TASK Q:
     - DMA TASK (one source  <ERAM> and one destination <external memory>)
       - src_addr and src_offset makes a bit aligned ERAM source address
    - Writes Tag and Interrupt Task if 'interrupt_reqd' is set to TRUE
      - This is needed to call the registered call back function after all the
        tasks written since last commit executed successfully
    - commits the write pointer to trigger A2 to start working on written TASKs

    @note
    This function takes care of necessary cache flushing/invalidation of source/
    destination.
    This functions assumes that no virtual to physical mapping is needed for
    the address provided as part of the arguments i.e. virtual and physical
    memory address is same.

    @return
    None
*/
/*============================================================================*/
void a2_ul_phy_hspai_dma_from_eram_and_commit
(
  a2_sub_id_t             sub_id,    /*!< System subscriber id */
  /*! Source (ENCIB) offset in bytes from the start of VPE0_LMEM 
  (MODEM_LMEM_RAM_START) from where the data needs to be copied.*/
  uint32                  src_addr,
  /*! offset in bits (0..7) from src_addr. Offset in bits within the start
      Byte. Offset is measured from MSBit in byte pointed by src_eram_addr. */
  uint16                  src_offset_in_bits,
  /*! source length in bytes */
  uint16                  src_len_in_bytes,
  /*! destination address (external memory/dsm item memory) where deciphered
      data needs to be copied. Length to be copied to destination is assumed
      to be same as "src_len_in_bytes" */
  uint32                  dest_addr,
  /*! whether interrupt is required after all the tasks (written since last
      commit) have been executed */
  boolean                 interrupt_reqd,
  /*! user data to be returned as part of call back function a2_ul_phy_hspa_
      -dma_from_eram_cb(). This field is ignored if 'interrupt_reqd' argument
      is set to FALSE.*/
  uint32                  user_data
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_register_harq_commit_cb

==============================================================================*/
/*!
  @brief
  Registers a callback function to be called whenever tag and interrupt task
  written from function a2_ul_phy_hspai_harq_commit() is processed by A2 UL PHY.

  @caller
  WL1 or MAC (during task initialization)

*/
/*============================================================================*/
void a2_ul_phy_hspai_register_harq_commit_cb
(
  /*! The callback to be called whenever interrupt is received after interrupt
      is generated via function call a2_ul_phy_hspai_harq_commit() */
  a2_ul_phy_hspa_harq_commit_cb harq_commit_cb_fn_ptr
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_harq_init

==============================================================================*/
/*!
  @brief
  This function must be called first whenever a transport block is being
  built or a retx is done.

  This function performs following operations:
    - Resets HARQ_DONE register
    - Resets CRC computation
      - The CRC block starts computing CRC over any FILL and DMA TASKs

*/
/*============================================================================*/
void a2_ul_phy_hspai_harq_init
(
  a2_sub_id_t  sub_id    /*!< System subscriber id */
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_write_first_fill_task

==============================================================================*/
/*!
  @brief
  Write 'length_in_bits' from source_address to dest_address via FILL TASK in
  UL_PHY TASK Q.

  This task also contains the destination location that the subsequent DMA/FILL
  tasks will copy data to.

  This function is expected to be successful, upon failure it will throw an
  error fatal (which can be used to fine tune TASK Q size).

  @note
  Once this function returns then memory assocaited with source address can be
  modified. 'length_in_bits' shall be less than or equal to 128.
  FILL TASK is used to efficently copy small amounts (upto 128 bits) of data.
  For DCHSUPA case dest_eram_address should be 0

  @caller
   - HSPA MAC to write initial MAC header (MAC-e) of the transport block

*/
/*============================================================================*/
void a2_ul_phy_hspai_write_first_fill_task
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint32       source_address,  /*!< Source address from which data needs to be
                                 copied. If data is not mutiple of bytes then
                                 data to be copied shall sit in most significant
                                 bits of that byte.
                                 Length is measured from MSBit.
                                 @todo - may change based on the RUMI
                                  Validation*/
  uint32      dest_address,      /*!< Dest ENCIB offset in bytes from the start 
                                  of VPE0_LMEM (MODEM_LMEM_RAM_START) to where 
                                  the data needs to be copied.*/
  uint16      length_in_bits   /*!< The length of the data in bits to be copied
                                 from source_address. Valid values - 1..128 */
);



/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_write_fill_task

==============================================================================*/
/*!
  @brief
  Writes a fill task, which is used to efficently copy small amounts of data.

  Copies length in bits from MSBit part of src_data. Address is taken as the
  relative address from the one indicated in a2_ul_phy_hspa_write_first_fill_
  -task().

  This function is expected to be successful otherwise it will throw error
  fatal (in the beginning to fine tune TASK Q size).

  @note
  'length_in_bits' shall be less than or equal to 128 and measured from MSBit of
  'src_data'.
  FILL TASK is used to efficently copy small amounts (upto 128 bits) of data.

  @caller
   - HSPA MAC to write TSNs(mac-es header) and SI
*/
/*============================================================================*/
void a2_ul_phy_hspai_write_fill_task
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint32       source_address,  /*!< Source address from which data needs to be
                                 copied. If data is not mutiple of bytes then
                                 data to be copied shall sit in most significant
                                 bits of that byte.
                                 Length is measured from MSBit.
                                 @todo - may change based on the RUMI
                                  Validation*/
  uint16      length_in_bits /*!< The length of the data in bits to be copied
                             from source_address. Valid values - 1..32 */
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_gather_and_cipher

==============================================================================*/
/*!
    @brief
    Gathers data from multiple location to one location and if needed performs
    ciphering.

    @detail
    Add following tasks to A2 UL PHY TASK Q:
     - FILL TASK (for writing bytes equal to "cipher_byte_pffset".
     - CIPHER TASK (only if cipher_algo is not set to A2_CIPHER_ALGO_NONE)
     - DMA TASK (to gather from multiple sources <external memory> to one
       destination <external memory>)

    @note
    This function takes care of necessary cache flushing/invalidation of
    source(s) and destination.
    This functions assumes that no virtual to physical mapping is needed for
    the address provided as part of the arguments i.e. virtual and physical
    memory address is same.
    Lastly, its assumed that memory (addr, length) pointed by as part of this
    API call will not be over-written until next a2_ul_phy_hspa_init().

    @caller
     - HSPA RLC PDU Build function - to write RLC PDU (Header, LIs and Payload)

    @return
    None
*/
/*============================================================================*/
void a2_ul_phy_hspai_gather_and_cipher
(
  a2_sub_id_t                   sub_id,    /*!< System subscriber id */
  /*! pointer to ciphering info */
  a2_ul_phy_hspa_cipher_info_t *cipher_info_ptr,
  /*! number of source iovec's valid in src_iovec_list[] from which data needs
      to be gathered. 0 is invalid value. */
  uint16                        num_src_iovec,
  /*! source iovec list. Address shall be not NULL and length shall be non zero
      in first "num_src_iovec" array elements.
      @note first length in src_iovec_list shall be greater than or equal to
      'ciph_start_offset_in_bytes' whenever ciphering algo is not NONE. */
  a2_iovec_t                  src_iovec_list[A2_UL_PHY_HSPA_MAX_SRC_IOVEC_COUNT]
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_dma_ext_mem_to_eram

==============================================================================*/
/*!
  @brief
  Programs the hardware to DMA from external memory to Encoder RAM.

  @params
  - source_address     : Source address from which retx needs to
                         be performed
  - src_len_in_bytes   : Length of data in bytes
  - dest_address       : Dest ENCIB offset in bytes from the start 
                          of VPE0_LMEM (MODEM_LMEM_RAM_START) to where 
                          the data needs to be copied.
 
   @caller
    - HSPA Harq Re-tx function to move data from readback buffer to ERAM
*/
/*============================================================================*/

void a2_ul_phy_hspai_dma_ext_mem_to_eram
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint32       source_address,  /*!<  Source address from which data needs to be
                                 copied. If data is not mutiple of bytes then
                                 data to be copied shall sit in most significant
                                 bits of that byte. Len is measured from MSBit*/  
  uint16       src_len_in_bytes,      /*!< The length of the data in bytes to be 
                                 copied from source_address.*/  
  uint32       dest_address       /*!< Dest ENCIB offset in bytes from the start 
                                  of VPE0_LMEM (MODEM_LMEM_RAM_START) to where 
                                  the data needs to be copied.*/      
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_write_padding_bits

==============================================================================*/
/*!
  @brief
  Writes padding bits (all 0's) to ERAM

  @caller
    - HSPA MAC: For adding padding bits to the transport block
    - HSPA RLC (Optional): For adding padding bits to the RLC PDU
*/
/*============================================================================*/
void a2_ul_phy_hspai_write_padding_bits
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint16       num_pad_bits /*!< number of bits to be padded by A2 to destination
                           address as computed by previous DMA task for ongoing
                           transport block. */
);


/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_harq_commit

==============================================================================*/
/*!
  @brief
  This function shall be called after all the tasks associated with the
  transport block is written.
  This is needed to trigger HW to start working on the tasks written till this
  function call and after a2_ul_phy_hspai_harq_init().

  This function performs following operations:
    - Adds CRC computed till now using CRC REPORT TASK
    - Writes the task to set the HARQ_DONE register associated with a given bank
      - This is done to make sure that HW can catch SW Late in building TX PDUs
    - Writes Tag and Interrupt Task if 'interrupt_reqd' is set to TRUE
      - This is needed to call the registered call back function after all the
        tasks written since the last commit executed successfully
    - commits the write pointer to trigger A2 to start working on written TASKs

  This function is expected to be successful otherwise it will throw an error
  fatal (which can be used to fine tune TASK Q size)

  @caller
    - HSPA: MAC or L1 (for indicating data is ready to transmit on a given HARQ)
*/
/*============================================================================*/
void a2_ul_phy_hspai_harq_commit
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint16       tb_size_in_bits, /*!< For error handling purpose. TB size 
                                     excluding CRC bits */
  boolean      interrupt_reqd, /*!< whether interrupt is required after all the 
                               tasks (written since last commit) have been 
                               executed.Set it to TRUE for debugging or testing 
                               purpose
                               only. */
  uint32       user_data /*!< user data to be returned as part of call back 
                              function a2_ul_phy_hspa_harq_commit_cb(). 
                              This field is ignored if 'interrupt_reqd' argument 
                              is set to FALSE. */
);

/*==============================================================================

  FUNCTION:  a2_ul_phy_hspai_write_and_commit_fill_task

==============================================================================*/
/*!
    @brief
       This API is called to update the FW with given data.
       API assumes the destination memory address 
       is uncached.
 
    @param
     dest_addr,  Physical address where data needs to be copied (DST) 
     *data,      Data that needs to be written (SRC)  
     size        Length of the data to be written in bytes
 
    @return
       void
*/
/*============================================================================*/
void a2_ul_phy_hspai_write_and_commit_fill_task
(
  a2_sub_id_t  sub_id,    /*!< System subscriber id */
  uint32 dest_addr,  
  uint8  *data,
  uint8  size_in_bytes        
);

#endif /* LTE_A2_UL_PHY_HSPA_H */

