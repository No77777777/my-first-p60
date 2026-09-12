#ifndef VIDRV_IPA_H
#define VIDRV_IPA_H

/**---------------------------------------------------------------------------
 * @file     VIDRV_ipa.h
 *
 * @brief    This is the interface of IPA VI driver
 * 
 * Copyright (c) 2012 - 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------
**                          INCLUDES
** ----------------------------------------------------------------------- */
#include "ipa_defs.h"
#include "ipa_flavor_defs.h"
#include "ipa_debug.h"
#include "VIDRV_pipe_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
**                           DEFINES
** ----------------------------------------------------------------------- */

/**
 * @brief Value for case N/A ( zero )
*/
#define  VIDRV_IPA_NOT_APPLICABLE   0

/**
 * @def  IPA_GLOBAL_FILTER_TABLE
 *
 * @brief   Global filter table index. 
*/
#define IPA_GLOBAL_FILTER_TABLE 32

/**
 * @def  IPA_PERFORMANCE_ALIGN
 *
 * @brief This alignment for data allows to reach the maximal performance
*/
#define IPA_PERFORMANCE_ALIGN  128      

/**
 * @def  IPA_DATA_ALIGN
 *
 * @brief   Ipa alignment in bytes for data buffers (includes data FIFO of BAM). 
*/
#define IPA_DATA_ALIGN    4                     /**< standard alignment */

/**
 * @def  IPA_DESC_ALIGN
 *
 * @brief   Ipa alignment in bytes for data buffers (includes data FIFO of BAM). 
 * the descriptor FIFO must be 8-aligned ( as address as size )
*/
#define IPA_DESC_ALIGN        8  

/**
 * @def  IPA_RULE_ALIGN
 *
 * @brief   Ipa alignment in bytes for rules. 
*/
#define IPA_RULE_ALIGN        8 

/**
 * @def  IPA_TABLE_ALIGN
 *
 * @brief   Ipa table alignment in bytes. 
*/
#define IPA_TABLE_ALIGN 128

/**
 * @brief   Ipa lte keys alignment in bytes. 
*/

#define IPA_KEYS_ALIGN 128


/**
 * @def  IPA_IM_ALIGN
 *
 * @brief   The alignment of the immediate command parameter list in bytes.
*/
#define IPA_IM_ALIGN 8

/**
 * @brief   timeout for IPA transfer, in milliseconds
*/
#define IPA_TIMEOUT 5000

/**
 * @brief   Converts X Bytes to kilobytes. 
 *
 * @param   x  The number of bytes. 
*/
#define IPA_TO_KILOBYTES(x) ( (x) >> 10 ) 

/**
 * @brief   Converts X kilobytes to Bytes. 
 *
 * @param   x  The number of kilobytes. 
*/
#define IPA_KILOBYTES_TO_BYTES(x) ( (x) << 10 ) 

/**
 * @brief   Maximum aggregation time limit possible according to the SWI. 
 * @sa VIDRV_ipa_AggrPipeInit
*/
#define IPA_MAX_AGGREGATION_TIME_LIMIT_IN_MS 31

/**
 * @brief   Maximum aggregation byte limit possible according to the SWI in KB units. 
 * @sa VIDRV_ipa_AggrPipeInit
*/
#define IPA_MAX_AGGREGATION_BYTE_LIMIT_IN_KB 31 

/**
 * @brief   Maximum aggregation byte limit possible according to the SWI in bytes units. 
 * @sa VIDRV_ipa_AggrPipeInit
*/
#define IPA_MAX_AGGREGATION_BYTE_LIMIT_IN_BYTES (IPA_MAX_AGGREGATION_BYTE_LIMIT_IN_KB*1024)

/**
 * @brief   Maximum number of packet aggregation limit possible according to the SWI. 
 * @sa VIDRV_ipa_AggrPipeInit
 * @remark  [IPA 2.0 SWI].IPA_ENDP_INIT_AGGR_n
 *    "  Limit of aggregated packet count. Up to 63 packets can be specified. 
 *       ...
 *       Valid for Output Pipes only (IPA Producer). 
 *       Valid only for Generic Aggregation type. "
*/
#define IPA_MAX_AGGREGATION_PACKET_LIMIT 63

/**
 * @brief   uC end of file marker for uC binary image
*/
#define VIDRV_IPA_UC_IMAGE_EOF_MARKER                           0xFFFFFFFF

/**
 * @brief   Maximum pad factor possible according to the SWI.
 * @brief   Maximum number of entries in HW sniffer log.
 * @remark  [IPA 2.0 SWI].IPA_LOG_BUF_HW_SIZE.SIZE
 *    "  Buffer Length in 128B unit. 
 *       Zero length means no buffer and thus no logging. 
 *       Buffer Max size - 2^16*128B = 8,388,608 Bytes. 
 *       Note: 2^16 = 0x10000
 *    " 
*/
#define IPA_MAX_ENTRIES_IN_SNIFFER_LOG 0x10000

/**
 * @brief   Maximum byte threshold user may specify.
 * @remark  [IPA 2.0 SWI].IPA_ENDP_INIT_MODE_n.BYTE_THRESHOLD
 *    "  Byte threshold operation for DMA operation. 
 *       To have this threshold ignored, set it to a large value 
 *       beyond expected DMA chunk size, such as 0xffff. 
 *       Valid only for DMA Mode.
 * "
*/
#define IPA_MAX_BYTE_THRESHOLD 0xFFFF


/**
 * @brief   Maximum ciphering key index possible according to the SWI.
 * @sa VIDRV_ipa_DecipherSet
 * @remark  [IPA 2.0 Arch]."Immediate Commands"
 *    "  IPAv2 supports up to 16 keys, thus Max index is 15.
 *    "
*/
#define IPA_MAX_CIPHER_KEY_INDEX 0xF

/**
 * @brief   Maximum number can loaded to IPA memory.
*/
#define IPA_MAX_NUMBER_OF_KEYS (IPA_MAX_CIPHER_KEY_INDEX+1)

/**
 * @brief   Maximum valid value of bearer
*/
#define IPA_BEARER_MAX_VALUE  0x31

/**
 * @brief   Maximum ciphering key index possible according to the SWI.
 * @sa VIDRV_ipa_DecipherSet
 * @remark  [IPA 2.0 Arch]."Immediate Commands"
 *    "  LTE_DECIPH_INIT.Offset_Start is 14-bit value
 *    "
*/
#define IPA_MAX_CIPHER_OFFSET_START 0x3FFF

/**
 * @brief   LTE ciphering key size in bytes.
*/
#define IPA_LTE_CIPHER_KEY_SIZE 0x10

/**
 * @brief   Maximum packet size in bytes for dma task command. 
*/
#define IPA_MAX_PACKET_SIZE_IN_DMA_TASK         IPA_MAX_TRANSFER_SIZE

/**
 * @brief   Maximum number of packets in dma task command. 
*/
#define IPA_MAX_NUM_BUFFERS_IN_DMA_TASK (63) /* TODO: Should be 64 and still under debugging */

/**
 * @brief   Maximum number of fragments per packet in multiple dma task. 
*/
#define IPA_MAX_NUM_BUFFERS_IN_MULTIPLE_DMA_TASK_PER_PACKET (63)
/**
 * @brief   Frame header size in bytes for dma task command. 
*/
#define IPA_DMA_TASK_FRAME_HEADER_SIZE (8)

/**
 * @brief   Maximum expected size for IC DMA_SHARED_MEM. 
 * @remark  [Immediate Commands.xlsx]."DMA_SHARED_MEM" (column F)
*/
#define IPA_DMA_SHARED_MEM_BUUFER_MAX_EXPECTED_SIZE   0x800

/**
* @brief   Maximal small DMA transfer size in bytes. 
*/
#define IPA_SMALL_DMA_SIZE 8

/**
 * @def  IPA_MAX_IP_PACKET_SIZE
 *
 * @brief   Maximum IP packet size in bytes. 
*/
#define IPA_MAX_IP_PACKET_SIZE (2500)

/**
 * @def  IPA_MAX_TLP_PACKET_SIZE
 *
 * @brief   Maximum single packet size in bytes that is supported by the TLP protocol. 
*/
#define IPA_MAX_TLP_PACKET_SIZE (2047)

/**
 * @brief   Maximum number of OUT descriptors in the descriptor FIFO. 
*/
#define IPA_MAX_NUM_OF_OUT_DESC 4

/**
 * @brief   Maximum number of IN descriptors in the descriptor FIFO. 
*/
#define IPA_MAX_NUM_OF_IN_DESC 4

/**
* @brief   Pipe TLV FIFO size when working with GSI.
*/
#define IPA_GSI_PIPE_TLV_FIFO_SIZE (IPA_GSI_OVERALL_TLV_FIFO_SIZE/IPA_MAX_PIPE_NUM)

/**
* @brief   Pipe AOS FIFO size when working with GSI.
*/
#define IPA_GSI_PIPE_AOS_FIFO_SIZE (IPA_GSI_OVERALL_AOS_FIFO_SIZE/(IPA_MAX_PIPE_NUM+IPA_COALESCING_VCS_MAX))

/**
 * @brief   Maximum number of descriptors in chain of descriptors when EOT flag 
 *          is defined only for latest descriptor in the chain. 
*/
#define IPA_GSI_MAX_DESC_IN_CHAIN      IPA_GSI_PIPE_TLV_FIFO_SIZE
  
/**
 * @brief   Maximal number of rules in Fragmentation Offload table. 
 */
#define IPA_MAX_FRAG_RULES  16

/**
 * @def  IPA_BAM_DATA_FIFO_SIZE
 *
 * @brief   BAM data FIFO size in bytes. 
 *
 * @note This value could be 0xFFFF (maximum 16-bit value),   <br>
 * but the size of internal BAM accumulators are 16-bit       <br>
 * and there is recommendation to use size 32KB as maximum    <br>
 * to avoid the cases of overflowing during an calculations. 
*/
#define IPA_BAM_DATA_FIFO_SIZE 0x8000

/**
 * @def  IPA_BAM_DESC_FIFO_SIZE
 *
 * @brief   BAM descriptor FIFO size in bytes. 
*/
#define IPA_BAM_DESC_FIFO_SIZE 0x40

/**
 * @brief   The maximum value of time stamp in status
*/
#define IPA_MAX_STATUS_TIME_STAMP 0xffffff

/**
 * @def  IPA_MAX_HEADER_SIZE
 *
 * @brief   Maximum header size in bytes. 
*/
#define IPA_MAX_HEADER_SIZE 64

/**
 * @brief   Bit-mask of the field HDR_LEN in the register IPA_ENDP_INIT_HDR 
 *
*/
#define IPA_HEADER_LEN_MASK      0x3f

/**
 * @def  IPA_RETAIN_HEADER_ALIGNMENT
 *
 * @brief   the alignment of the removed header that will be retained on pipes. 
*/
#define IPA_RETAIN_HEADER_ALIGNMENT 4

/**
 * @def  IPA_ETHERNET_EMAC_HEADER_SIZE
 *
 * @brief   The size of the Ethernet and emac headers. 
*/
#define IPA_ETHERNET_EMAC_HEADER_SIZE 14

/**
 * @def  IPA_HWSNIFF_ALIGN
 *
 * @brief   Alignment and memory granularity for HW sniffer. 
*/
#define IPA_HWSNIFF_ALIGN       128

/**
 * @brief   Number of blocks for HW sniffer. 
*/
#define IPA_HWSNIFF_NUM_OF_BLOCKS 64

/**
 * @brief   The size of STATUS Packet in bytes. 
*/
#define IPA_STATUS_PACKET_SIZE_IN_BYTES      32

/**
 * @brief   Size in bytes of Status Queue buffer. 
 * @note    1. Should be enough to keep whole number of statuses.
 *          2. Should be an integer that is a multiple of 1KB
 */
#define IPA_SQ_BUF_SIZE         2048

/**
 * @brief   Initial count of buffers in Status Queue fifo.
 */
#define IPA_SQ_INIT_BUF_COUNT   (MIN(5, IPA_SQ_MAX_BUFFERS-1))

/**
 * @brief   Maximum number of routing tables. 
*/
#define IPA_MAX_NUM_OF_ROUTE_TABLES 32

/**
 * @brief   Number of supported IPv4 NAT tables. 
*/
#define IPA_NAT_IPV4_NUM_SUPPORTED_TABLES    1

/**
 * @brief   Number of supported IPv6 Connection Tracking tables. 
*/
#define IPA_IPV6_CONN_TRACK_NUM_SUPPORTED_TABLES    1

/**
 * @def  VIDRV_IPA_NET4_TO_UINT32
 *
 * @brief   Translate IPv4 address notation to uint32. 
*/
#define VIDRV_IPA_NET4_TO_UINT32(a,b,c,d)    ( (((uint32)a)<<24) | (((uint32)b)<<16) | (((uint32)c)<<8) | ((uint32)d) )

/**
 * @brief   The metadata mask into QMAP Header (without the offset !)
*/
#define VIDRV_IPA_QMAP_METADATA_MASK 0xFF

/**
 * @brief   The maximum value of QMAP frame size for aggregation and de aggregation
*/
#define VIDRV_IPA_QMAP_MAX_FRAME_SIZE 0x7FFF

/**
 * @brief   The maximum value of QMAP message size for aggregation and de aggregation
*/
#define VIDRV_IPA_QMAP_MAX_MSG_SIZE 0x3FFF

/**
 * @brief   Wrapper for regular VIDRV_ipa_PipeFree
 */
#define VIDRV_IPA_PIPE_FREE(ipaPipe) if (NULL != ipaPipe) { VIDRV_ipa_PipeFree(ipaPipe); ipaPipe = NULL; }

/**
 * @brief   Wrapper for regular VIDRV_ipa_RouteTableUninit
 */
#define VIDRV_IPA_ROUTE_TABLE_FREE(tableHandle)\
   if (NULL != tableHandle)\
   {\
      VIDRV_ipa_RouteTableUninit(tableHandle);\
      (void)VIDRV_ipa_RouteTableUpdate();\
      tableHandle = NULL;\
   }\

/**
 * @brief   Wrapper for regular VIDRV_ipa_FilterTableUninit
 */
#define VIDRV_IPA_FILTER_TABLE_FREE(tableHandle)\
   if (NULL != tableHandle)\
   {\
      VIDRV_ipa_FilterTableUninit(tableHandle);\
      (void)VIDRV_ipa_FilterTableUpdate();\
      tableHandle = NULL;\
   }\

/**
 * @brief   Wrapper for regular VIDRV_ipa_StatusQueueRelease
 */
#define VIDRV_IPA_STATUS_QUEUE_RELEASE(StatusQueueHandle)\
   if (NULL != StatusQueueHandle)\
   {\
      (void)VIDRV_ipa_StatusQueueRelease(StatusQueueHandle);\
      StatusQueueHandle = NULL;\
   }\

/**
 * @brief   Wrapper for regular VIDRV_ipa_NatIPv4RemoveEntry
 */
#define VIDRV_IPA_NAT_ENTRY_REMOVE(natEntry)\
   if (NULL != natEntry)\
   {\
      VIDRV_ipa_NatIPv4RemoveEntry(natEntry);\
      natEntry = NULL;\
   }\

/**
 * @brief   Wrapper for regular VIDRV_ipa_NatIPv4RemoveEntry
 */
#define VIDRV_IPA_CONN_TRACK_ENTRY_REMOVE(connTrackEntry)\
   if (NULL != connTrackEntry)\
   {\
      VIDRV_ipa_Ipv6ConnTrackRemoveEntry(connTrackEntry);\
      connTrackEntry = NULL;\
   }\

/**
 * @brief   The number of spare bytes that is used to set the threshold in bytes 
            for the IPH engine to start processing a packet ( see IpaIphThresholdType )
 */
#define VIDRV_IPA_IPH_THRESHOLD_SPARE_IN_BYTES        0

/**
 * @brief   The number of bytes that is used by IPA to hold internal data (Filtering/Routing Rules)
 *          in case of packet size is more than 256 bytes
 */
#define VIDRV_IPA_IPH_THRESHOLD_RULES_DATA_IN_BYTES   64

/**
 * @brief   A value to be used in the Routing rule action description when no offset has been configured.
 */
#define VIDRV_IPA_NO_ROUTING_RULE_OFFSET              (~0)

/**
 * @brief   A value to be used when adding a Routing rule to the UCP.
 */
#define VIDRV_IPA_XLAT_PIPE   0x1F

/**
 * @brief   The maximum value of ruleId
 */
#define  VIDRV_IPA_RULE_ID_MASK   0x3FF

/**
 * @brief   Number of times to attempt halting a pipe
 */
#define IPA_NUM_ATTEMPS_TO_PIPE_HALT (20)

/**
 * @brief   IPA source and destination resource operations. 
*/
/* The total number of the IPA Source Resources types */
#define VIDRV_IPA_SRC_RSRC_TYPE_NUMBER (VIDRV_IPA_RSRC_TYPE_SRC_NUMBER + VIDRV_IPA_RSRC_TYPE_NUMBER - VIDRV_IPA_RSRC_TYPE_DST_NUMBER)
#define VIDRV_IPA_SRC_RSRC_INDEX_TO_TYPE(i) (VIDRV_ipa_ResourceTypesType)((i) < VIDRV_IPA_RSRC_TYPE_SRC_NUMBER ? (i) : (i) + VIDRV_IPA_DST_RSRC_TYPE_NUMBER)
/* The total number of the IPA Destination Resources types */
#define VIDRV_IPA_DST_RSRC_TYPE_NUMBER (VIDRV_IPA_RSRC_TYPE_DST_NUMBER - VIDRV_IPA_RSRC_TYPE_SRC_NUMBER)
#define VIDRV_IPA_DST_RSRC_INDEX_TO_TYPE(i) (VIDRV_ipa_ResourceTypesType)((i) + VIDRV_IPA_RSRC_TYPE_SRC_NUMBER)
#define VIDRV_IPA_RSRC_TYPE_TO_INDEX(i) \
   ( ((i) < VIDRV_IPA_RSRC_TYPE_SRC_NUMBER ? (i) \
   : ((i) < VIDRV_IPA_RSRC_TYPE_DST_NUMBER ? (i) - VIDRV_IPA_RSRC_TYPE_SRC_NUMBER \
   : (i) - VIDRV_IPA_RSRC_TYPE_DST_NUMBER + VIDRV_IPA_RSRC_TYPE_SRC_NUMBER)) )
#define VIDRV_IPA_RSRC_TYPE_GROUP_NUMBER(i) \
   ( ((i) < VIDRV_IPA_RSRC_TYPE_SRC_NUMBER ? IPA_SRC_RSRC_GRP_NUMBER \
   : ((i) < VIDRV_IPA_RSRC_TYPE_DST_NUMBER ? IPA_DST_RSRC_GRP_NUMBER \
   : IPA_SRC_RSRC_GRP_NUMBER)) )

/**
 * @brief   Converts the number of HOLB timer ticks to time in mSec
*/
#define IPA_HOLB_TIMER_TICKS_TO_MS(__x__)  (uint32)(( ((uint64)(__x__)) * 1000 * 128)/(ipa.ipaClockFrequency))

/**
 * @brief   Converts time in mSec to the number of HOLB timer ticks
*/
#define IPA_HOLB_MS_TO_TIMER_TICKS(__x__)  (uint32)(( ((uint64)(__x__)) * (ipa.ipaClockFrequency))/(1000 * 128))



/**
 * @brief   This bit is used to prevent the validation of Immediate Commands opcode correctness. 
            This value MUST be a power of 2 and be 16bit value that more then (2^8-1)
*/
#define IPA_PREVENT_IMM_CMD_OPCODE_VALIDATION      (0x8000)

/**
 * @brief   The maximal amount of consumers pipes that can be configured to have deaggregation. Value depends on the specific IPA flavor
*/
#define IPA_MAX_AMOUNT_CONSUMERS_DEAGGR_PIPES ((IPA_PIPE_CONSUMER_DEAGGR_MAX_ID) - (IPA_PIPE_CONSUMER_DEAGGR_MIN_ID) + 1)

/**
 * @brief   Translates the consumer/producer pipe number to valid consumer/producer index
*/
#define IPA_PIPE_PRODUCER_NUM_TO_ID(num)           ((IPA_PIPE_CONSUMER_MIN_ID == IPA_PIPE_PRODUCER_MIN_ID) ? ( IPA_PIPE_CONSUMER_MIN_ID + num * 2  + 1) : IPA_PIPE_PRODUCER_MIN_ID + num)
#define IPA_PIPE_CONSUMER_NUM_TO_ID(num)           ((IPA_PIPE_CONSUMER_MIN_ID == IPA_PIPE_PRODUCER_MIN_ID) ? ( IPA_PIPE_CONSUMER_MIN_ID + num * 2     ) : IPA_PIPE_CONSUMER_MIN_ID + num)

/**
 * @brief   Slaves, like DDR, can not handle buffer which cross 1KB alignment (start before and end after address which is multiple of 1K). 
*/
#define IPA_UC_ADDRESS_BOUNDARY 0x400

/**
* @brief   uC maximum QMB transfer size in bytes
*/
#define IPA_UC_MAX_TRANSFER_SIZE 128
#define IPA_UC_MAX_QMB_TRANSFER_SIZE(base) (IPA_UC_MAX_TRANSFER_SIZE-((base)%8/*Bus width in bytes*/))


/* -----------------------------------------------------------------------
**                           ENUMERATIONS
** ----------------------------------------------------------------------- */

/**
 * @brief   IPA HW config parameters
 *
 * @note    The array ipaHwParamNames MUST be updated 
 *          for each element in this enum
 *          (see file ..\IPA\common\utils\ipa_hw_utils.c)
*/
typedef enum 
{
   IPA_HW_PARAM_QMB0_MAX_WR,
   IPA_HW_PARAM_QMB0_MAX_RD,
   IPA_HW_PARAM_QMB1_MAX_WR,
   IPA_HW_PARAM_QMB1_MAX_RD,
   IPA_HW_PARAM_D_DCPH_2_EN,
   IPA_HW_PARAM_INLINE_IMM_CMD_ENABLE,
   IPA_HW_PARAM_DST_RSRC_GRP_NUM,
   IPA_HW_PARAM_DPL_BANDWIDTH_REDUCTION_EN,
   IPA_HW_PARAM_MBIM_GENERIC_AGGREGATION_EN,             /**< MBIM Generic aggr is enabled from IPA 4.0  */
   IPA_HW_PARAM_USAGE_OF_HARD_BYTE_LIMIT_IS_UNLIMITED,   /**< The usage of Hard-Byte-Limit is unlimited  */
   IPA_HW_PARAM_INLINE_REGISTER_WRITE_IC_ENABLE,
   IPA_HW_PARAM_AXI_MASTER_AVAILABLE,
   IPA_HW_PARAM_PHY_2_PHY_ENABLE, /**< indicate using the phy2phy feature which is enabled starting from IPA4.5*/
   IPA_HW_PARAM_PDCP_DECIPHER_OFFLOAD_ENABLE,  /**< indicate using the pdcp decipher offload feature  which is enabled starting from IPA4.5   */
   IPA_HW_PARAM_CHECKSUM_OFFLOAD_ENABLE,  /**< indicate using the checksum offload feature which is enabled starting from IPA4.5   */
   IPA_HW_PARAM_MBIM_ENABLE,  /**< indicate using the MBIM feature which is disabled for IPA 4.2   */
   IPA_HW_PARAM_NUM                             /**< amount of IPA's HW config parameters       
                                                     Should be the latest line of this enum 
                                                */
} IpaHwConfigParametersType;

/**
 * @brief   Pipe selection option.
 * @note    The enumerator is used while randomizing the pipe numbers to be used for the different purposes.
 */
typedef enum VIDRV_ipa_PipeSelectionOption
{
   /* for consumers */
   IPA_PIPE_ANY_CONSUMER,             /* Any non-allocated pipe to be consumer */
   IPA_PIPE_DEAGGREGATION, 	        /* Any consumer pipe supporting deaggregation (pipes in the range 0 to IPA_MAX_DEAGGREGATION_PIPE) */
   IPA_PIPE_ANY_BUT_DEAGGREGATION,    /* Any consumer pipe not supporting deaggregation */
   /* for producers */
   IPA_PIPE_ANY_PRODUCER,             /* Any non-allocated pipe to be producer */
   IPA_PIPE_COALESCIONG_PRODUCER,     /* Any producer pipe supporting coalescing */
} VIDRV_ipa_PipeSelectionOption;

/**
 * @brief   RTL version of the IPA core.  
 * @note    This enumeration can be updated in future once more versions of IPA would exist. In such case
 *          need to update the implementation of VIDRV_ipa_GetVersion.
*/
typedef enum
{
  VIDRV_IPA_FIRST_VERSION,                                      /**< First Version */
  VIDRV_IPA_3_0_DEVELOPMENT_VERSION = VIDRV_IPA_FIRST_VERSION,  /**< one of the emulation versions during development of IPA 3.0 */
  VIDRV_IPA_3_0_ASIC_VERSION,                                   /**< RTL version used for IPA 3.0 */
  VIDRV_IPA_3_1_DEVELOPMENT_VERSION,                            /**< one of the emulation versions during development of IPA 3.1 */
  VIDRV_IPA_3_1_ASIC_VERSION,                                   /**< RTL version used for IPA 3.1 */
  VIDRV_IPA_3_5_DEVELOPMENT_VERSION,                            /**< one of the emulation versions during development of IPA 3.5 */
  VIDRV_IPA_3_5_ASIC_VERSION,                                   /**< RTL version used for IPA 3.5 */
  VIDRV_IPA_3_5_1_DEVELOPMENT_VERSION,                          /**< one of the emulation versions during development of IPA 3.5.1 */
  VIDRV_IPA_3_5_1_ASIC_VERSION,                                 /**< RTL version used for IPA 3.5.1 */
  VIDRV_IPA_4_0_0_DEVELOPMENT_VERSION,                          /**< one of the emulation versions during development of IPA 4.0.0 */
  VIDRV_IPA_4_0_0_ASIC_VERSION,                                 /**< RTL version used for IPA 4.0.0 */
  VIDRV_IPA_4_1_0_DEVELOPMENT_VERSION,                          /**< one of the emulation versions during development of IPA 4.1.0 */
  VIDRV_IPA_4_1_0_ASIC_VERSION,                                 /**< RTL version used for IPA 4.1.0 */
  VIDRV_IPA_4_2_0_DEVELOPMENT_VERSION,                          /**< one of the emulation versions during development of IPA 4.2.0 */
  VIDRV_IPA_4_2_0_ASIC_VERSION,                                 /**< RTL version used for IPA 4.2.0 */
  VIDRV_IPA_4_5_0_DEVELOPMENT_VERSION,                          /**< one of the emulation versions during development of IPA 4.5.0 */
  VIDRV_IPA_4_5_0_ASIC_VERSION,                                 /**< RTL version used for IPA 4.5.0 */
  VIDRV_IPA_UNKNOWN_VERSION,                                    /**< IPA version can not be determined. This is usually indication for error */
  VIDRV_IPA_LAST_VERSION = VIDRV_IPA_UNKNOWN_VERSION            /**< Last Version */
} VIDRV_ipa_VersionType;

/**
 * @brief   The list of equation's bit-masks in order that is defined by the architecture
*/
typedef enum 
{
   VIDRV_IPA_TOS_EQ                = (0x1<< 0), /**< Exact match on the TOS (Type Of Service) field of IPv4 header.  */
   VIDRV_IPA_PROTOCOL_EQ           = (0x1<< 1), /**< Exact match on the "PROTOCOL" field of IPv4/6 header. */
   VIDRV_IPA_TC_EQ                 = (0x1<< 2), /**< Exact match on the "Traffic Class" field of IPv6 header. */
   VIDRV_IPA_OFFSET_MEQ128_0       = (0x1<< 3), /**< Exact match on the MASKED 128-bit     qword that starts at (offset)         byte from the header. Used for      source address check in IPv6. */
   VIDRV_IPA_OFFSET_MEQ128_1       = (0x1<< 4), /**< Exact match on the MASKED 128-bit     qword that starts at (offset)         byte from the header. Used for destination address check in IPv6. */
   VIDRV_IPA_OFFSET_MEQ32_0        = (0x1<< 5), /**< Exact match on the MASKED  32-bit      word that starts at (offset)         byte from the header. */
   VIDRV_IPA_OFFSET_MEQ32_1        = (0x1<< 6), /**< Exact match on the MASKED  32-bit      word that starts at (offset)         byte from the header. */
   VIDRV_IPA_IHL_OFFSET_MEQ32_0    = (0x1<< 7), /**< Exact match on the MASKED  32-bit      word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_IHL_OFFSET_MEQ32_1    = (0x1<< 8), /**< Exact match on the MASKED  32-bit      word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_METADATA_MEQ32        = (0x1<< 9), /**< Exact match on the MASKED  32-bit      word with METADATA. */
   VIDRV_IPA_IHL_OFFSET_RANGE16_0  = (0x1<<10), /**< Range[L,G] checking on the 16-bit half-word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_IHL_OFFSET_RANGE16_1  = (0x1<<11), /**< Range[L,G] checking on the 16-bit half-word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_IHL_OFFSET_EQ32       = (0x1<<12), /**< Exact match on the         32-bit      word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_IHL_OFFSET_EQ16       = (0x1<<13), /**< Exact match on the         16-bit half-word that starts at (offset + IHL*4) byte from the header. */
   VIDRV_IPA_FL_EQ                 = (0x1<<14), /**< Exact match on the "Flow Label" field of IPv6 header. */
   VIDRV_IPA_IPV4_IS_FRAG          = (0x1<<15), /**< True when IPv4 packet is fragmented.*/
   /* 
   *  Special values:
   */
   VIDRV_IPA_EMPTY_EQ              = 0,                        /**< Empty equation (dummy equation) */
   VIDRV_IPA_LATEST_EQ             = VIDRV_IPA_IPV4_IS_FRAG,   /**< Mask of the latest equation in the list */
   VIDRV_IPA_INVALID_EQ            = (VIDRV_IPA_LATEST_EQ<<1)  /**< Special Mask to mark an invalid equation */
} VIDRV_ipa_EquationBitMaskType;

/**
 * @brief   Values that represent the type of DMA table. 
*/
typedef enum
{
   VIDRV_IPA_DMA_NAT_RULE_TABLE        = 0,   /**< NAT Rule Table type, to be used in TABLE_DMA IC as well as index in NAT tables */
   VIDRV_IPA_DMA_NAT_RULE_EXP_TABLE    = 1,   /**< NAT Rule Expansion Table type, to be used in TABLE_DMA IC as well as index in NAT tables */
   VIDRV_IPA_DMA_NAT_INDEX_TABLE       = 2,   /**< NAT Index Table type, to be used in TABLE_DMA IC as well as index in NAT tables */
   VIDRV_IPA_DMA_NAT_INDEX_EXP_TABLE   = 3,   /**< NAT Index Expansion Table type, to be used in TABLE_DMA IC as well as index in NAT tables */
   VIDRV_IPA_DMA_NAT_CONFIG_TABLE      = 4,   /**< NAT Config Table type */   
   VIDRV_IPA_DMA_NAT_NUM_TABLES        = 5,   /**< The total number of NAT tables */    
   VIDRV_IPA_DMA_NAT_TABLE_WITH_MAX_ENTRY_SIZE  = VIDRV_IPA_DMA_NAT_RULE_TABLE, /**< The DMA NAT table with bigger size of table's entry */   
   
   VIDRV_IPA_DMA_CONN_TRACK_RULE_TABLE       = 4,   /**< Conn Track Rule Table type, to be used in TABLE_DMA IC */
   VIDRV_IPA_DMA_CONN_TRACK_RULE_EXP_TABLE   = 5,   /**< Conn Track Rule Expansion Table type, to be used in TABLE_DMA IC */
   VIDRV_IPA_CONN_TRACK_NUM_TABLES           = 2,   /**< The total number of Conn Track tables */ 
   VIDRV_IPA_CONN_TRACK_RULE_TABLE           = 0,   /**< Conn Track Rule Table type, to be used as Conn Track tables as index */
   VIDRV_IPA_CONN_TRACK_RULE_EXP_TABLE       = 1,   /**< Conn Track Rule Expansion Table type, to be used as Conn Track tables as index */
   
} VIDRV_ipa_DmaTablesType;




/**
 * @brief Values that represent MHI engine states. 
*/
typedef enum VIDRV_ipa_MhiStatesType
{
   VIDRV_IPA_MHI_INIT,          /**< MHI engine is not initialized (pre initialization or uninit was called */
   VIDRV_IPA_MHI_RESET,         /**< MHI engine in reset state */
   VIDRV_IPA_MHI_READY,         /**< MHI engine in ready state */
   VIDRV_IPA_MHI_ERROR,         /**< MHI engine in system error state */
   VIDRV_IPA_MHI_RUNNING,       /**< MHI engine in running state */
   VIDRV_IPA_MHI_HALTED,        /**< MHI engine is halted by host */
} VIDRV_ipa_MhiStatesType;

/**
 * @brief Step types for which step mode would be enabled. Values may be used as mask.
 * @sa VIDRV_ipa_ConfigStepMode
 * @remark [IPA 2.0 SWI].IPA_STEP_MODE_REG
*/
typedef enum VIDRV_ipa_StepModePipeStepType
{
   VIDRV_IPA_STEP_MODE_DISABLE            = 0x0000,  /**< Normal operation mode */
   VIDRV_IPA_STEP_MODE_STEP_RX_START      = 0x0001,  /**< RX start (descriptor processing) */
   VIDRV_IPA_STEP_MODE_STEP_RX_CMD_WRITE  = 0x0002,  /**< RX command write */
   VIDRV_IPA_STEP_MODE_STEP_FETCH_PKT     = 0x0004,  /**< Fetcher start (packet processing start) */
   VIDRV_IPA_STEP_MODE_STEP_FETCH_CMD     = 0x0008,  /**< Fetcher RX command processing start */
   VIDRV_IPA_STEP_MODE_STEP_DECIPHER      = 0x0010,  /**< Decipher start */
   VIDRV_IPA_STEP_MODE_STEP_IPH_START     = 0x0020,  /**< IPH start */
   VIDRV_IPA_STEP_MODE_STEP_IPH_INIT      = 0x0040,  /**< IPH INIT start */
   VIDRV_IPA_STEP_MODE_STEP_FILTER        = 0x0080,  /**< IPH Filtering start */
   VIDRV_IPA_STEP_MODE_STEP_NAT_START     = 0x0100,  /**< IPH NAT start */
   VIDRV_IPA_STEP_MODE_STEP_NAT_INDEX     = 0x0200,  /**< IPH NAT Index Entry Request start */
   VIDRV_IPA_STEP_MODE_STEP_NAT_ENTRY     = 0x0400,  /**< IPH NAT Entry Request start */
   VIDRV_IPA_STEP_MODE_STEP_ROUTING       = 0x0800,  /**< IPH Routing start  */
   VIDRV_IPA_STEP_MODE_STEP_HDI           = 0x1000,  /**< IPH Header Insertion start */
   VIDRV_IPA_STEP_MODE_STEP_STATUS        = 0x2000,  /**< IPH Status start */
   VIDRV_IPA_STEP_MODE_STEP_FRAG          = 0x4000,  /**< IPH Frag start */
   VIDRV_IPA_STEP_MODE_STEP_PTX           = 0x8000,  /**< PTX start */
   VIDRV_IPA_STEP_MODE_STEP_TX            = 0x10000, /**< TX start (TX command processing) */
} VIDRV_ipa_StepModePipeStepType;


/**
 * @brief Values that represent the type of generic De-Aggregation / Aggregation.
 * @remark [IPA 2.0 Arch]."Generic Aggregation / Deaggregation"
*/
typedef enum VIDRV_ipa_GenericAggregationType
{
   VIDRV_IPA_GENERIC_AGGREGATION_DISABLED,                     /**< Generic de-aggregation/aggregation is not functional */
   VIDRV_IPA_GENERIC_AGGREGATION_TLP,                          /**< Generic de-aggregation/aggregation is configured for TLP */
   VIDRV_IPA_GENERIC_AGGREGATION_QMAP,                         /**< Generic de-aggregation/aggregation is configured for QMAP */
   VIDRV_IPA_GENERIC_AGGREGATION_RNDIS,                        /**< Generic de-aggregation/aggregation is configured for RNDIS */
   VIDRV_IPA_GENERIC_AGGREGATION_MBIM,                         /**< Generic de-aggregation/aggregation is configured for MBIM */
   VIDRV_IPA_GENERIC_AGGREGATION,                              /**< Generic de-aggregation/aggregation is configured */
} VIDRV_ipa_GenericAggregationType;



/**
 * @brief   The type represents value of the Endianness.
*/
typedef enum 
{  
   VIDRV_IPA_LITTLE_ENDIAN    = 0,  /**< Little Endian */
   VIDRV_IPA_BIG_ENDIAN       = 1   /**< Big Endian */
}  VIDRV_ipa_EndiannessType;

/**
 * @brief   The type represents value of the TotalLenOrPad.
*/
typedef enum 
{  
   VIDRV_IPA_PADDING_LENGTH   = 0,  /**< 0 - TOTAL_LEN_OR_PAD field is used as PAD length */
   VIDRV_IPA_TOTAL_LENGTH     = 1   /**< 1 - TOTAL_LEN_OR_PAD field is used as Total length (header + packet + padding) */
}  VIDRV_ipa_TotalLenOrPadType;

/**
 * @brief   The type represents value of the header's length 
            of TLP/QMAP/RNDIS protocols for Generic De-Aggr / Aggr.
*/
typedef enum 
{  
   VIDRV_IPA_GEN_AGGR_TLP_HDR_LEN_IN_BYTES         =  2, /**< The size (in bytes) of TLP Header. */
   VIDRV_IPA_GEN_AGGR_QMAP_HDR_LEN_IN_BYTES        =  4, /**< The size (in bytes) of QMAP Header. */
   VIDRV_IPA_GEN_AGGR_QMAP_UL_CS_HDR_LEN_IN_BYTES  =  4, /**< The size (in bytes) of the Uplink Checksum Header. */
   VIDRV_IPA_GEN_AGGR_RNDIS_HDR_LEN_IN_BYTES       = 44, /**< The size (in bytes) of RNDIS Header. */
   VIDRV_IPA_GEN_AGGR_MBIM_HDR_LEN_IN_BYTES        =  4, /**< The size (in bytes) of signature in MBIM Header. */
}  VIDRV_ipa_GenAggrHdrLenType;

/**
 * @brief   The type represents value of different offsets (as, for example, the offset of packet-length field) in header 
            of TLP/QMAP/RNDIS protocols for Generic De-Aggr / Aggr.
*/
typedef enum 
{  
   VIDRV_IPA_GEN_AGGR_TLP_OFST_PKT_SIZE_IN_BYTES               =  0, /**< The offset (in bytes) of the packet size in TLP Header. */

   VIDRV_IPA_GEN_AGGR_QMAP_OFST_PKT_SIZE_IN_BYTES              =  2, /**< The offset (in bytes) of the packet size in QMAP Header. */
   VIDRV_IPA_GEN_AGGR_QMAP_OFFSET_TO_PADDING_FIELD_IN_BYTES    =  0, /**< The offset (in bytes) of the padding field in QMAP Header. */
   VIDRV_IPA_GEN_AGGR_QMAP_OFFSET_TO_METADATA_FIELD_IN_BYTES   =  1, /**< The offset (in bytes) of the metadata field in QMAP Header. */   
   
   VIDRV_IPA_GEN_AGGR_RNDIS_OFST_PKT_SIZE_IN_BYTES             = 12, /**< The offset (in bytes) of the packet size in RNDIS Header. */
   VIDRV_IPA_GEN_AGGR_RNDIS_OFFSET_TO_PACKET_LOCATION_IN_BYTES =  8, /**< The size (in bytes) of the packet location in RNDIS Header. */
   VIDRV_IPA_GEN_AGGR_RNDIS_OFFSET_TO_LENGTH_FIELD_IN_BYTES    =  4, /**< The size (in bytes) of the length field in RNDIS Header. */

   VIDRV_IPA_GEN_AGGR_MBIM_OFFSET_TO_METADATA_FIELD_IN_BYTES   =  0, /**< The offset (in bytes) of the metadata field in user's metadata register. */   

}  VIDRV_ipa_GenAggrOffsetsType;

/**
 * @brief   The type represents size of IPH_THRESHOLD in bytes
 * @note    IPH_THRESHOLD will be = 
 *                64 + (64/128/256) + [source pipe L2_header length (MBIM etc.)]  
*/
typedef enum VIDRV_ipa_IphThresholdSizeType
{
   VIDRV_IPA_IPH_THRESHOLD_INVALID_SIZE_IN_BYTES   = 0,     /**< does not defined in SWI */
   VIDRV_IPA_IPH_THRESHOLD_256_SIZE_IN_BYTES       = 256,   /**< SWI (b'10): 256B (IPAv1 behavior +64B) */
   VIDRV_IPA_IPH_THRESHOLD_128_SIZE_IN_BYTES       = 128,   /**< SWI (b'00): 128B */
   VIDRV_IPA_30_IPH_THRESHOLD_SIZE_IN_BYTES        = 192    /**< only one threshhold size is valid for IPA3.0, and the value is 256-64=192 */
} VIDRV_ipa_IphThresholdSizeType;

/**
 * @brief   Represents the sequence types for the user.
*/
typedef enum
{
   VIDRV_IPA_SEQ_FIRST,
   VIDRV_IPA_SEQ_FILTER_ROUTE = VIDRV_IPA_SEQ_FIRST,  /**< Represents Filter/Route/NAT/HDRI sequence type. */
   VIDRV_IPA_SEQ_DMA_ONLY,                            /**< Represents pure DMA sequence type. */
   VIDRV_IPA_SEQ_DMA_WITH_PARSER,                     /**< Represents DMA with packet parser sequence type. */
   VIDRV_IPA_SEQ_FILTER_ROUTE_DECIPHER,               /**< Represents Filter/Route/Decipher sequence type. */
   VIDRV_IPA_SEQ_FILTER_ROUTE_CIPHER,                 /**< Represents Filter/Route/Cipher sequence type. */
   VIDRV_IPA_SEQ_DCMP,                                /**< Represents Decompression sequence type. */
   VIDRV_IPA_SEQ_ETHERNET_BRIDGING,                   /**< Represents Ethernet Bridging sequence type. */
   VIDRV_IPA_SEQ_UC_PACKET_PROCESSING_WITH_DECIPHER,  /**< Represents uC packet processing with decipher. */
   VIDRV_IPA_SEQ_XLAT,                                /**< Represents XLAT sequence type. */
   VIDRV_IPA_SEQ_XLAT_WITH_DECIPHER,                  /**< Represents XLAT with decipher sequence type. */
   VIDRV_IPA_SEQ_LAST = VIDRV_IPA_SEQ_XLAT_WITH_DECIPHER

} VIDRV_ipa_UserSequenceType;

typedef enum VIDRV_ipa_FeatureControl
{
   IPA_EN_FEAT_USE_DEFAULTS                  = 0,
   IPA_EN_FEAT_SKIP_HWSNIFFER                = (1<<0),
   IPA_EN_FEAT_USE_CUSTOM_GSI_FW             = (1<<1),
   IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_SHIFT     = 2, /* FW index is located right after IPA_EN_FEAT_USE_CUSTOM_GSI_FW. */
   IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_MASK      = 7, /* We're giving 3 bits to FW index. */
   /* bit (1<<2) is reserved for :*/
   IPA_EN_FEAT_CUSTOM_GSI_FW_UC_INTERACTION  = (IPA_EN_FEAT_USE_CUSTOM_GSI_FW | (0 << IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_SHIFT)),
   IPA_EN_FEAT_CUSTOM_GSI_FW_IPA_REG_ACCESS  = (IPA_EN_FEAT_USE_CUSTOM_GSI_FW | (1 << IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_SHIFT)),
   IPA_EN_FEAT_CUSTOM_GSI_FW_MCS_TEST_BUS    = (IPA_EN_FEAT_USE_CUSTOM_GSI_FW | (2 << IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_SHIFT)),
   IPA_EN_FEAT_CUSTOM_GSI_FW_CUSTOM_STOP     = (IPA_EN_FEAT_USE_CUSTOM_GSI_FW | (3 << IPA_EN_FEAT_CUSTOM_GSI_FW_INDEX_SHIFT)),
   IPA_EN_FEAT_SKIP_PIPE_IF_INIT             = (1<<5), /* Skip initialization of pipe interface (BAM/GSI). Used in multiple EEs tests */
   IPA_EN_FEAT_SKIP_SYSTEM_PIPE_ALLOC        = (1<<3), /* Skip system pipe allocation */
} VIDRV_ipa_FeatureControl;

/**
 * @brief   The type represents the resource group configuration.
*/
typedef enum
{
   VIDRV_IPA_RESOURCE_CONFIG_MAV       = 0,
   VIDRV_IPA_RESOURCE_CONFIG_MBB       = 1,
   VIDRV_IPA_RESOURCE_CONFIG_MSM       = 2,
   VIDRV_IPA_RESOURCE_CONFIG_NUMBER
} VIDRV_ipa_ResourceConfigType;



/* -----------------------------------------------------------------------
**                           TYPES
** ----------------------------------------------------------------------- */

/**
 * @brief   The IPA pipe structure
*/
struct IpaPipeType;

/**
* @brief    Filter rule structure for internal use
*/
struct VIDRV_ipa_FilterRuleListNode;

/**
 * @brief   Routing rule structure for internal use
*/
struct VIDRV_ipa_RouteRuleListNode;

/**
 * @brief  Routing table structure
*/
struct VIDRV_ipa_RouteTableQueueNode;

/**
 * @brief   IPA header entry structure
*/
struct VIDRV_ipa_HeaderEntryStruct;

/**
 * @brief   A structure to save the information about the command sequences being constructed
*/
struct VIDRV_ipa_PacketProcessingCommandSequenceStruct;

/**
 * @brief   A structure that represents filtering table information
*/
struct VIDRV_ipa_FilterTableParamsType;

/**
 * @brief   Various type definitions to encapsulate the implementation details
 */
typedef struct VIDRV_ipa_FilterRuleListNode                    * VIDRV_ipa_FilterRuleHandle;                      /**< @brief Handle of a IPA filter rule */
typedef struct VIDRV_ipa_FilterTableQueueNode                  * VIDRV_ipa_FilterTableHandle;                     /**< @brief Handle of a IPA filter table */
typedef struct VIDRV_ipa_RouteRuleListNode                     * VIDRV_ipa_RouteRuleHandle;                       /**< @brief Handle of a IPA Routing rule */
typedef struct VIDRV_ipa_RouteTableQueueNode                   * VIDRV_ipa_RouteTableHandle;                      /**< @brief Handle of a IPA routing table */
typedef void                                                   * VIDRV_ipa_RouteRuleActionDescriptionHandle;      /**< @brief Handle to a Routing rule action description */ 
typedef void                                                   * VIDRV_ipa_NatIPv4EntryHandle;                    /**< @brief Handle to an IPv4 NAT entry */
typedef struct IpaIpv4NatConfigEntryStruct                     * VIDRV_ipa_NatIpv4ConfigEntryHandle;              /**< @brief Handle to an IPv4 NAT config entry */
typedef struct VIDRV_ipa_PacketProcessingCommandSequenceStruct * VIDRV_ipa_PacketProcessingCommandSequenceHandle; /**< @brief Handle to a packet processing command sequence */ 
typedef void                                                   * VIDRV_ipa_Ipv6ConnTrackEntryHandle;              /**< @brief Handle to an IPv6 Conn Track entry */

typedef uint32 VIDRV_ipa_Net4Addr;           /**< @brief IPv4 IP address */
    
typedef uint8 VIDRV_ipa_DecipherKey[IPA_LTE_CIPHER_KEY_SIZE];     /**< @brief LTE 128b cipher key. In case of Snow 3G on 32-bit LSB would be used*/

/**
 * @brief   IPv6 IP address .
 */
typedef struct
{
    uint32 addr[4];                           /**< IPv6 IP address */
} VIDRV_ipa_Net6Addr;

typedef uint16 VIDRV_ipa_Port;               /**< @brief IP port */

/**
 * @brief   Defines the configuration structure that passed to the IPA driver. 
*/
typedef struct 
{
   uint64 bamBasePhysAddress; /**< BAM physical address */
   void* bamBaseVirtAddress; /**< BAM virtual address */
   uint32 bamIntNum;          /**< BAM interrupt number */
   uint64 gsiBasePhysAddress; /**< GSI physical address */
   void* gsiBaseVirtAddress; /**< GSI virtual address */
   uint32 gsiIntNum;          /**< GSI interrupt number */
} VIDRV_ipa_ConfigType;

/**
 * @brief    A structure that represents filtering tables information. 
*/
typedef struct VIDRV_ipa_FilterTableParamsType
{
   boolean              isHashedTable; /**< TRUE- Configure Hashed-Table, FALSE - Configure NON-Hashed-Table */
   boolean              isLocal;       /**< TRUE - Save in local memory, FALSE - Save in system memory */
   boolean              isIPv6;        /**< TRUE- Configure IPv6 table, FALSE - Configure IPv4 table */
   VIDRV_ipa_PipeHandle hPipe;         /**< The handle to the pipe or NULL for global table */
} VIDRV_ipa_FilterTableParamsType;

/**
 * @brief    A structure that represents routing tables information. 
*/
typedef struct
{
   boolean              isHashedTable; /**< TRUE- Configure Hashed-Table, FALSE - Configure NON-Hashed-Table */
   boolean              isLocal;       /**< TRUE - Save in local memory, FALSE - Save in system memory */
   boolean              isIPv6;        /**< TRUE- Configure IPv6 table, FALSE - Configure IPv4 table */
   uint32               tableNum;      /**< The number of the table */
} VIDRV_ipa_RouteTableParamsType;

/**
 * @brief    A structure that contains information necessary to describe an action associated with a Routing rule. 
*/
typedef struct
{
   uint32   headerOffset;  /**<  Offset in words of the header to be added. 
                                 This offset refers to the beginning of the Header Table which is defined in Local or System memory */
   boolean  isHdrInLocal;  /**<  TRUE means header resides in Local memory, FALSE - in System */
} VIDRV_ipa_RouteRuleActionDescriptionType;

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   The filter rule structures as it appears in memory.
 *  0                   1                   2                   3                   4                   5                   6
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
 * |            enRule             | action  | Routing |r| PDN   |s|      Priority       | reserved|  Rule ID          | reserved  |  
 * |                               |         |Table ID |H| index |M|                     |         |                   |           |
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
 * | function ( 64-bit alignment ) - optional; extra_word_1/2 are a part of the function (if any)                                  #
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
*/
typedef  PACK(struct) 
{
   /* Rule Header:
   */
   uint64 enRule          : 16;     /**< The enable rule bitmap.  */
   uint64 action          :  5;     /**< Filter action.  */
   uint64 routingTableId  :  5;     /**< Routing table ID.  */
   uint64 retainHeader    :  1;     /**< Upon this bit the header insertion will retain the original packet header */
   uint64 pdnIndex        :  4;     /**< PDN index. Valid starting from the IPA 4.0. */
   uint64 setMetadata     :  1;     /**< Set metadata condition. Valid starting from the IPA 4.0. */
   uint64 priority        :  10;    /**< The Priority field defines the order of the rule within the integrated filter rules set. */
   uint64 reserved1       :  6;     /**< reserved field.  */
   uint64 ruleId          :  10;    /**< The Rule ID provided to SW within the Status. Value 0x3FF is reserved, and cannot be used */
   uint64 reserved2       :  6;     /**< reserved field.  */

   /* Rule Function: 
   */
   uint64* functions;               /**< functions of the rule; extra_word_1/2 should be included into the functions */
} VIDRV_ipa_FilterHwRuleType;

#ifdef WIN32
#pragma pack(pop) /* Revert alignment to what it was previously */
#endif

/**
* @brief   The filter rule structures as it appears in memory + additional info about the rule (like function size etc).
*/
typedef struct 
{
   VIDRV_ipa_FilterHwRuleType hwRule;        /**< The filter rule structures as it appears in memory.  */
   uint16                     functionSize;  /**< size (in bytes) the function-area of the rule.  */
} VIDRV_ipa_FilterRuleType;

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   The Routing rule structures as it appears in memory.
 *  0                   1                   2                   3                   4                   5                   6
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
 * |            enRule             |pipe dst.|L| Header Offset   |C|      Priority       | reserved|  Rule ID          | reserved  |  
 * |                               |  index  | |                 | |                     |         |                   |           |
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
 * | function ( 64-bit alignment ) - optional; extra_word_1/2 are a part of the function (if any)                                  #
 * #-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#-+-+-+-+-+-+-+-#
 * where:
 *    L - Local
 *    C - Context
*/

typedef  PACK(struct)  
{
   uint64 enRule        : 16;    /**< The enable rule bitmap.  */
   uint64 pipeDstId     :  5;    /**< Destination pipe ID  */
   uint64 isHdrInSystem :  1;    /**< 1 means header (from the Header Table) resides in System memory, 0 - in Local */
   uint64 hdrOffset     :  9;    /**< Offset in words of the header to be added. 
                                      This offset refers to the beginning of the Header Table which is defined in Local or System memory */
   uint64 context       :  1;    /**< Indicates Header processing context/directives format */
   uint64 priority      :  10;   /**< The Priority field defines the order of the rule within the integrated filter rules set. */
   uint64 reserved0     :  6;    /**< reserved field.  */
   uint64 ruleId        :  10;   /**< The Rule ID provided to SW within the Status. Value 0x3FF is reserved, and cannot be used */
   uint64 reserved1     :  6;    /**< reserved field.  */

   /* Rule Function: 
   */
   uint64* functions;            /**< functions  */
} VIDRV_ipa_RouteHwRuleType;

#ifdef WIN32
#pragma pack(pop) /* Revert alignment to what it was previously */
#endif

/**
* @brief   The Routing rule structures as it appears in memory + additional info about the rule (like function size etc).
*/
typedef struct  
{
   VIDRV_ipa_RouteHwRuleType  hwRule;        /**< The Routing rule structures as it appears in memory.  */
   uint16                     functionSize;  /**< size (in bytes) the function-area of the rule.  */
} VIDRV_ipa_RouteRuleType;

/**
 * @brief   The Header Information type. 
*/
typedef struct 
{
   uint32 headerSize;   /**< The header size in bytes - maximum of 64 bytes */
   uint8* header;       /**< The header to be added */
} VIDRV_ipa_HeaderParamsType;

/**
 * @brief   Defines the IPv4 NAT table configuration information that passed to the IPA driver. 
*/
typedef struct
{
   boolean            ruleTableLocal;           /**< NAT rule table location. TRUE - Save in local memory, FALSE - Save in system memory */
   boolean            ruleExpansionTableLocal;  /**< NAT rule expansion table location. TRUE - Save in local memory, FALSE - Save in system memory */
   boolean            indexTableLocal;          /**< Index rule table location. TRUE - Save in local memory, FALSE - Save in system memory */
   boolean            indexExpansionTableLocal; /**< Index rule expansion table location. TRUE - Save in local memory, FALSE - Save in system memory */
   uint16             numberTableEntries;       /**< Number of entries in the Rule and Index tables. Must be a power of two.  */
   uint16             numberExpansionTableEntries;/**< Number of entries in the Rule and Index expansion tables. Must be a power of two. */
   VIDRV_ipa_Net4Addr publicIPaddress;          /**< The public IP address to be used during the address translation */
   VIDRV_ipa_Port     portUpperRange;           /**< Upper range of Hardware allocatable ports for NAT table - Not supported */
   VIDRV_ipa_Port     portLowerRange;           /**< Lower range of Hardware allocatable ports for NAT table - Not supported*/
   uint8              configTableSize;          /**< NAT config table size in entries. Valid starting from the IPA 4.0. */
   VIDRV_ipa_NatIpv4ConfigEntryHandle configTablePtr;/**< NAT config table pointer. Valid starting from the IPA 4.0. */
}VIDRV_ipa_NatIPv4TableParamsType;

/**
 * @brief   Defines the IPv6 Conn Track table configuration information that passed to the IPA driver. 
*/
typedef struct 
{
   boolean  ruleTableLocal;              /**< Conn Track rule table location. TRUE - Save in local memory, FALSE - Save in system memory */
   boolean  ruleExpansionTableLocal;     /**< Conn Track rule expansion table location. TRUE - Save in local memory, FALSE - Save in system memory */
   uint16   numberTableEntries;          /**< Number of entries in the Rule table. Must be a power of two.  */
   uint16   numberExpansionTableEntries; /**< Number of entries in the Rule expansion tables. Must be a power of two. */
} VIDRV_ipa_Ipv6ConnTrackTableParamsType;

/**
 * @brief   The struct that describes the IPv4 NAT entry fields.
*/
typedef struct  
{
   VIDRV_ipa_Net4Addr privateIpAddr;    /**< The private ip address to be used during the address translation */
   VIDRV_ipa_Port     privatePort;      /**< The port to be used during the address translation */
   VIDRV_ipa_Net4Addr targetIpAddr;     /**< The target ip address as appear in IP header */
   VIDRV_ipa_Port     targetPort;       /**< The target port as appear in TCP/UDP header */ 
   VIDRV_ipa_Port     publicPort;       /**< The public port to be used during the address translation */
   uint8              protocol;         /**< The protocol as appear in IP header */
   uint8              pdnIndex;         /**< PDN index. Valid starting from the IPA 4.0. */
} VIDRV_ipa_NatIPv4SessionType;

/**
 * @brief   Defines the IPv6 CONN TRACK (Connection Tracking) table configuration information. 
*/
typedef struct
{
   uint32   outboundSrcAddr[4]; /**< UL / Outbound Src IPv6 Address */
   uint32   outboundDstAddr[4]; /**< UL / Outbound Dst IPv6 Address */
   uint16   flags;                                          
   uint32   timestamp;
   uint8    protocol;
   uint16   nextIndex;
   uint16   outboundDstPort;
   uint16   outboundSrcPort;
   boolean  outAllowed;
   boolean  inAllowed;
   uint32   swSpecificParams;
   VIDRV_ipa_ConnTrackType type; /**< Outbound or Inbound direction */
} VIDRV_ipa_Ipv6ConnTrackEntryType;



/**
 * @brief   Parameters for configuring the different debug buses. 
 * @sa VIDRV_ipa_DebugBusRead
*/
typedef  struct
{
   VIDRV_ipa_DebugBusBlockSelectType   externalBlockSelect; /**< The specific block in IPA which generates the debug information*/
   uint32                            internalBlockSelect; /**< the specific type of debug information generated by the block*/
   VIDRV_ipa_PipeHandle              hPipe;               /**< Select pipe for debug information*/
}VIDRV_ipa_DebugBusConfigType;

/**
  * @brief   The register IPA_ENDP_INIT_HDR_EXT 
  *
  * @remark [Immediate Commands.xlsx]."IPAv2 Register Changes" 
  *
*/
typedef struct   
{
   boolean  endianness              ;  /**< [  0  ]   0-Little Endian, 
                                                   1-Big Endian (default !)
                                       */
   boolean  totalLenOrPadValid      ;  /**< [  1  ]   0-Ignore TOTAL_LEN_OR_PAD field, 
                                                   1-Process TOTAL_LEN_OR_PAD field 
                                       */
   boolean  totalLenOrPad           ;  /**< [  2  ]   0-TOTAL_LEN_OR_PAD field is used as PAD length, 
                                                   1-TOTAL_LEN_OR_PAD field is used as Total length (header + packet + padding)
                                       */
   boolean  payloadLenIncPadding    ;  /**< [  3  ]   0-IPA_ENDP_INIT_HDR_n’s HDR_OFST_PKT_SIZE does not includes padding bytes size, payload_len = packet length,
                                                   1-IPA_ENDP_INIT_HDR_n’s HDR_OFST_PKT_SIZE includes padding bytes size, payload_len = packet length + padding
                                       */
   uint32   totalLenOrPadOffset     ;  /**< [ 9: 4]   Offset to length field containing either total length or pad length, per TOTAL_LEN_OR_PAD config */
   uint32   padToAlignment          ;  /**< [12:10]   Pad packet to specified bytes alignment (2^pad to alignment value). */
} VIDRV_ipa_RegHeaderExtType;

/**
 * @brief   IPA pipe Generic De-Aggregation initialization parameters 
 * @sa VIDRV_ipa_AggrPipeInit
*/
typedef  struct
{
   VIDRV_ipa_GenericAggregationType  genericDeAggrType;   /**< Type of the generic de-aggregation sort */
   VIDRV_ipa_RegHeaderExtType        hdrExt;              /**<  parameters of the register IPA_ENDP_INIT_HDR_EXT */
   
   boolean  hdrLenIncDeaggHdr;      /**<- header removal configuration to remove
                                          0: only first HDR_LEN bytes, 
                                             while generic deaggregation removes aggregation header. (default value) 
                                          1: first HDR_LEN + IPA_ENDP_INIT_DEAGGR_n.DEAGGR_HDR_LEN bytes 
                                             while generic deaggregation does not remove aggregation header.
                                    */
   uint32   hdrOfstPktSize;         /**<  The header packet size offset. 
                                          IPA will update "packet size" field within the header in accordance to the configuration. */
   boolean  checksumEn;             /**<  Enable(1) / Disable(0) flag of the Uplink Checksum Header existence. */
   uint32   deaggrHdrLen;           /**<  Deaggregation Header length in bytes (depends on the checksumEn for QMAP protocol). 
                                          Valid only for Input Pipes (IPA consumer), 
                                          which are configured for ’Generic’ deaggregation. */
   boolean  packetOffsetValid;      /**<  0: PACKET_OFFSET is not used, 
                                          1: PACKET_OFFSET is used */
   uint32   packetOffsetLocation;   /**<  Location of packet offset field, 
                                          which specifies the offset to the packet 
                                          from the start of the packet offset field. */
} VIDRV_ipa_GenericDeaggrInitParamsType;            

/**
 * @brief   IPA pipe aggregation/de-aggregation initialization parameters 
 * @sa VIDRV_ipa_AggrPipeInit
*/
typedef  struct
{  
   /* aggregation parameters only 
   */
   uint32               byteLimit;           /**< The byte limit in bytes. Relevant only for aggregation */
   uint32               timeLimit;           /**< The time limit in milliseconds. Relevant only for aggregation */
   uint32               packetLimit;         /**< The packet limit. Limit of aggregated packet count. 0 means ignore packet limit. 
                                             *  Value shall not exceed IPA_MAX_PACKET_AGGREGATION_LIMIT. 
                                             *  Valid only for Generic Aggregation type and producer pipe.*/                       
   boolean              eofCloseAggrEn;      /**< SW may close aggregation. 
                                             *  FALSE : EOF does not close aggregation. Only HW closes aggregation <br>
                                             *  TRUE  : EOF closes aggregation in addition to HW based aggregation closure. Valid for producer pipes only */
   boolean              hardByteLimitEn;     /**< Enable hard byte limit - new feature for IPA2.1 */

   /** Parameters of generic de-aggregation only (if it is used)
   *  these parameters are valid and SHOULD be defined ONLY when:
   *    aggrEnType  = IPA_ENABLE_DEAGGREGATION
   *    aggrType    = IPA_GENERIC
   *
   *  Note: The configuration of a generic de-aggregation will be defined automatically (by the VI IPA driver)
   *  when the sort of generic de-aggregation is known (TLP/RNDIS/QMAP)
   */
   VIDRV_ipa_GenericDeaggrInitParamsType  genericDeAggrParams; 

   /* common aggregation and de-aggregation parameters 
   */
   VIDRV_ipa_AggrEnType   aggrEnType;          /**< Type of the aggregation enabling (bypass/aggregation/deaggregation) */
   VIDRV_ipa_AggrType     aggrType;            /**< Type of the aggregation */

} VIDRV_ipa_AggrInitParamsType;            

/**
 * @brief   IPA sniffer parameters. 
*/
typedef  struct
{
   const uint8* pLogAddress;   /**< output parameter - the address of log buffer*/
   uint32       numLogEntries; /**< Number of entries in log (Value shall not exceed IPA_MAX_ENTRIES_IN_SNIFFER_LOG)*/
   boolean      enable;        /**< TRUE logging is enabled, FALSE logging is disabled*/
} VIDRV_ipa_SnifferParamsType;






/**
 * @brief   IPA pipe header initialization parameters. 
 * @sa VIDRV_ipa_HeaderPipeInit
*/
typedef  struct
{
   /* for register IPA_ENDP_INIT_HDR_n, n=[0..19]  */
   uint32   hdrLen;                 /**< [ 5: 0]   Length of the header (for removal or insertion) 
                                          Valid for both Input/Output Pipes (IPA Consumer/Producer)
                                    */
   boolean  hdrMetadataValid;       /**< [  6  ]   True to header metadata valid FALSE to use userMetadata instead 
                                          Valid for both Input/Output Pipes (IPA Consumer/Producer)
                                    */
   uint32   hdrMetadataOffset;      /**< [12: 7]   The header metadata offset 
                                          Valid for both Input/Output Pipes (IPA Consumer/Producer).
                                    */
   uint32   additionalLen;          /**< [18:13]   Length of the additional header 
                                          Valid for Output Pipes only (IPA Producer)
                                    */
   boolean  hdrOfstPktSizeValid;    /**< [ 19  ]   TRUE to header packet size valid (Valid for producer pipes only) 
                                          Valid for Output Pipes only (IPA Producer)
                                    */
   uint32   hdrOfstPktSize;         /**< [25:20]   The header packet size offset 
                                          Valid for both Input/Output Pipes (IPAConsumer/Producer). 
                                             Output Pipe -  Upon Header Insertion operation, 
                                                            IPA will update this field within the header with payload length. 
                                             Input Pipe -   Upon Deaggragation operation, 
                                                            IPA will read this field from aggregation header in order to know payload length
                                    */

   boolean  hdrMetadataRegValid;    /**< [ 28  ]   0: Metadata from register IPA_ENDP_INIT_HDR_METADATA_n is not valid, 
                                                   1: Metadata from registerPA_ENDP_INIT_HDR_METADATA_n is valid.
                                          Valid only for Input Pipes (IPA Consumer). 
                                    */

   /* for register IPA_ENDP_INIT_HDR_METADATA_MASK_n, n=[0..19]  
      Valid for Output Pipes only (IPA Producer)
   */
   uint32   metadataMask;           /**< [31: 0] Mask specifying which metadata bits to write (Valid for producer pipes only) */

   /* for register IPA_ENDP_INIT_HDR_METADATA_n, n=[0..19] 
      Valid for Input Pipes only (IPA Consumer)
   */
   uint32   userMetadata;           /**< [31: 0] User metadata value to be used in case header metadata is not valid (Valid for consumer pipes only) */

   /* for register IPA_ENDP_INIT_HDR_EXT_n, n=[0..19], Reset State: 0x00000001 
      see also VIDRV_ipa_RegHeaderExtType
   */
   uint32   padToAlignment;         /**< [12:10]   Pad packet to specified bytes alignment (2^pad to alignment value). */

   VIDRV_ipa_GenericAggregationType genericAggregationType; /**< The type of generic aggregation. Upon this field the function would utilize the proper union fields */
   
} VIDRV_ipa_HeaderPipeInitParamsType;

/**
 * @brief   Attributes of First Error Captured. 
*/
typedef  struct
{
   uint32 opcode                 :  6; /**< [ 5: 0] Opcode idenfitying which error took place, per interrupt source               */
   uint32 misc1                  :  2; /**< [ 7: 6] Value & usage are opcode specific. Reserved for future use.                   */
   uint32 pipeNum                :  5; /**< [12: 8] Pipe number assosiated with captured error. Used only by PROC_ERR in IPA_2.0! */
   uint32 misc2                  : 19; /**< [31:13] Value & usage are opcode specific. Reserved for future use.                   */
} VIDRV_ipa_FecAttrType;

/**
 * @brief   Attributes of First Error Captured, in case it is an MBIM De-Aggregation error. 
*/
typedef  struct
{
   uint32 opcode                  :  6; /**< [ 5: 0]   Opcode identify which error took place, per interrupt source      */
   uint32 reservedBits_15_6       : 10; /**< [15: 6]   reserved bits                                                     */
   uint32 mbimNthLengthMismatch   :  1; /**< [16]      error type - NTH header length mismatch                           */
   uint32 mbimNdpSigErr           :  1; /**< [17]      error type - NDP header signature error                           */
   uint32 mbimNdpIndexOutOfRange  :  1; /**< [18]      error type - NDP header index out of range                        */
   uint32 mbimDatagramOutOfRange  :  1; /**< [19]      error type - datagram index out of range                          */
   uint32 mbimNthSigErr           :  1; /**< [20]      error type - NTH header signature error                           */
   uint32 mbimLtMinFrame          :  1; /**< [21]      error type - frame length smaller than minimum                    */
   uint32 mbimNdpLengthOutOfRange :  1; /**< [22]      error type - NDP header length out of range. Exist from ipa 2.5   */
   uint32 reservedBits_23         :  1; /**< [23]      reserved bit                                                      */
   uint32 pipeNum                 :  8; /**< [31:24]   pipe number that caused error, 
                                                      this should be used to differentiate MBIM from generic-deaggr error status 
                                       */
} VIDRV_ipa_MbimDeaggrFecAttrType;

/**
 * @brief   Attributes of First Error Captured, in case it is an Generic (QMAP/RNDIS/TLP) De-Aggregation error. 
*/
typedef  struct
{
   uint32 opcode                       :  6; /**< [ 5: 0]   Opcode idenfitying which error took place, per interrupt source   */
   uint32 reservedBits_15_6            : 10; /**< [15: 6]   reserved bits                                                     */
   uint32 genDeaggrLenGtEotLen         :  1; /**< [16]      error type - frame length larger than EOT length      */
   uint32 genDeaggrLenGtMaxFrame       :  1; /**< [17]      error type - frame length larger than maximum         */
   uint32 genDeaggrLenLtMinFrame       :  1; /**< [18]      error type - frame length smaller than minimum        */
   uint32 genDeaggrOffsetPlusLenGtMsg  :  1; /**< [19]      error type - data_offset+data_len > msg_len (where applicable)             */
   uint32 genDeaggrPadGtMsg            :  1; /**< [20]      error type - data_padding > msg_len (where applicable)            */
   uint32 reservedBits_23_21           :  3; /**< [23:21]   reserved bits                                         */
   uint32 pipeNum                      :  8; /**< [31:24]   pipe number that caused error, 
                                                            this should be used to differentiate MBIM from generic-deaggr error status 
                                             */
} VIDRV_ipa_GenDeaggrFecAttrType;

/**
 * @brief   Structure parsing for Attributes of First Error Captured
 */
typedef struct
{
    union RegUnion                                       /**< register of the Attributes of First Error Captured depends on the opcode*/
    {   
        uint32                           value;            /**< represent the 32bits register value                */ 
        VIDRV_ipa_MbimDeaggrFecAttrType  mbimDeAggrAttr;   /**< in case it is an MBIM De-Aggregation error.        */
        VIDRV_ipa_GenDeaggrFecAttrType   genDeAggrAttr;    /**< in case it is an Generic De-Aggregation error.     */
        VIDRV_ipa_FecAttrType            fecAttr;          /**< in all the other cases                             */
    } reg;     /**< register value union*/   
    VIDRV_ipa_IntSrcOpcodeType opcode;                   /**< opcode of the FEC Attributes */                                       
} VIDRV_ipa_FecAttrParsingType;


/**
 * @brief   Structure of SW bypass clock-gating configuration
 */
typedef struct
{
   boolean cgcOpenMisc;       /**< Enables SW bypass clock-gating controls for IPA_MISC submodule;
                                 0: CGC is enabled by internal logic, 
                                 1: No CGC (clk is always ’ON’)
                              */

   boolean cgcOpenTx;         /**< Enables SW bypass clock-gating controls for IPA_TX submodule;
                                 0: CGC is enabled by internal logic, 
                                 1: No CGC (clk is always ’ON’)
                              */

   boolean cgcOpenProc;       /**< Enables SW bypass clock-gating controls for IPA_PROC submodule;
                                 0: CGC is enabled by internal logic, 
                                 1: No CGC (clk is always ’ON’)
                              */

   boolean cgcOpenRx;         /**< Enables SW bypass clock-gating controls for IPA_RX submodule;
                                 0: CGC is enabled by internal logic, 
                                 1: No CGC (clk is always ’ON’)
                              */
}VIDRV_ipa_SwBypassClkGatingConfigType;

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   Structure parsing for ENDP Hashing Tuple "disable" Mask. 
 */
typedef PACK(union)
{
   uint16 mask;
   PACK(struct) HashingTupleStruct
   {
      uint16 maskSrcId        : 1;  /**< When asserted SRC_ID is not part of the hashing tuple     */
      uint16 maskSrcIpAddr    : 1;  /**< When asserted SRC_IP_ADD is not part of the hashing tuple */
      uint16 maskDstIpAddr    : 1;  /**< When asserted DST_IP_ADD is not part of the hashing tuple */
      uint16 maskSrcPort      : 1;  /**< When asserted SRC_PORT is not part of the hashing tuple   */
      uint16 maskDstPort      : 1;  /**< When asserted DST_PORT is not part of the hashing tuple   */
      uint16 maskProtocol     : 1;  /**< When asserted PROTOCOL is not part of the hashing tuple   */
      uint16 maskMetadata     : 1;  /**< When asserted METADATA is not part of the hashing tuple   */
      uint16 reserved         : 9;  /**< reserved bits */
   } tuple;
} VIDRV_ipa_HashingTuple;

/**
 * @brief   Structure parsing for Filter and Routing hash flushing. 
 */
typedef struct VIDRV_ipa_FilterRoutingHashFlushParameters
{
   uint32   Ipv6RouterHashFlush : 1;      /**< bit[ 0] : If set, router Hash is flushed for IPv6 packets, self-cleared bit */
   uint32   reserved_1_3        : 3;      /**< not in use */
   uint32   Ipv6FilterHashFlush : 1;      /**< bit[ 4] : If set, filter Hash is flushed for IPv6 packets, self-cleared bit */
   uint32   reserved_5_7        : 3;      /**< not in use */
   uint32   Ipv4RouterHashFlush : 1;      /**< bit[ 8] : If set, router Hash is flushed for IPv4 packets, self-cleared bit */
   uint32   reserved_9_11       : 3;      /**< not in use */
   uint32   Ipv4FilterHashFlush : 1;      /**< bit[12] : If set, filter Hash is flushed for IPv4 packets, self-cleared bit */
   uint32   reserved_13_31      : (3+16); /**< not in use */
} VIDRV_ipa_FilterRoutingHashFlushParameters;

#ifdef WIN32
#pragma pack(pop) /* Revert alignment to what it was previously */
#endif

typedef struct VIDRV_ipa_HwSniffingElement
    {
        VIDRV_ipa_HwSniffClients sourceClient;
        uint32 enabled : 1;
        uint32 onPush : 1;
} VIDRV_ipa_HwSniffingElement;

typedef struct VIDRV_ipa_HwSniffInit
{
    uint32 memoryBufferSize;
    VIDRV_ipa_HwSniffingElement element[IPA_HWSNIFF_COUNT];
} VIDRV_ipa_HwSniffInit;

typedef union VIDRV_ipa_CmdQueCommand
{
   uint64 raw[2];
    struct RX_HPS
    {
        uint32 CMDQ_PACKET_LEN : 16;    /**< Destination length. */
        uint32 CMDQ_DEST_LEN : 16; /**< Packet length. */

        uint32 CMDQ_SRC_PIPE : 8; /*SRC pipe. */
        uint32 CMDQ_ORDER : 2; /* Order : 00 - middle, 01 - first, 10 - last, 11 - only */
        uint32 CMDQ_FLAGS : 6; /* Flags : bit 0 (lsb)-No - acknowledge(bit 10 of the register), bit 1 - Deaggregation frame start(bit 11 of the register), bit 2 - EOT(bit 12 of the register), bit 3 - COMPLETE(bit 13 of the register), bit 4 - EOF(bit 14 of the register), bit 5 - FLUSH(bit 15 of the register) */
        uint32 CMDQ_OPCODE : 8;/* CMDQ opcode. */
        uint32 CMDQ_METADATA : 8;/*Metadata information from CMDQ feed. */

        uint32 CMDQ_ADDR_LSB;
        uint32 CMDQ_ADDR_MSB;
    } rx_hps;
    struct DPS_TX
    {
        uint32 CMDQ_CTX_ID : 4;     /**< Context ID related to the command. */
        uint32 CMDQ_SRC_ID : 8;     /**< Sequence ID related to the command. Incremental per source pipe. */
        uint32 CMDQ_SRC_PIPE : 5;   /**< Source pipe related to the command. */
        uint32 CMDQ_OPCODE : 2;     /**< CMDQ opcode.*/
        uint32 CMDQ_REP : 1;        /**< Replicated packet indication. */
    } dps_tx;
} VIDRV_ipa_CmdQueCommand;

/**
 * @brief   Structure of HW sniffer pointers
 */
typedef struct{
   const VIDRV_ipa_HwSniffCommand*  pWritePtr;     /**< Write pointer of external FIFO */
   VIDRV_ipa_HwSniffCommand*        pReadIntPtr;   /**< Read  pointer of internal FIFO */
   VIDRV_ipa_HwSniffCommand*        pWriteIntPtr;  /**< Write pointer of internal FIFO */
}VIDRV_ipa_SnifferCmdPositionType;

/**
 * @brief  Entry types of HW sniffer status
 */
typedef enum{
   IPA_HW_SNIFFER_STATUS_TYPE          = 0x01,
   IPA_HW_SNIFFER_FRAG_STATUS_TYPE     = 0x02,
   IPA_HW_SNIFFER_DROPPED_STATUS_TYPE  = 0x04,
   IPA_HW_SNIFFER_DPL_STATUS_TYPE      = 0x10,
   IPA_HW_SNIFFER_DCMP_STATUS_TYPE     = 0x20,
}VIDRV_ipa_HwStatusEntryType;

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   Structure of HW sniffer status
 */
typedef PACK(struct)
{ /* entry types 1,4,8,0x20 */
   /* Word 0 */
   uint32 entryType     : 8;
   uint32 exception     : 8;
   uint32 statusMask    :16;
   /* Word 1 */
   uint32 packetLen     :16;
   uint32 srcEndp       : 5;
   uint32 res1          : 3;
   uint32 dstEndp       : 5;
   uint32 res2          : 3;   
   /* Word 2 */
   uint32 metaData      :32;
   /* Word 3 */
   uint32 filterL       : 1;
   uint32 filterH       : 1;
   uint32 filterG       : 1;
   uint32 filterRet     : 1;
   uint32 filterRuleId  :10;
   uint32 routeL        : 1;
   uint32 routeH        : 1;
   uint32 ucpOn         : 1;
   uint32 routeTableId  : 5;
   uint32 routeRuleId   :10;
   /* Word 4 */
   uint32 natHit        : 1;
   uint32 natIdx        :13;
   uint32 natType       : 2;
   uint32 tagInfoLsb    :16;
   /* Word 5 */
   uint32 tagInfoMsb    :32;
   /* Word 6 */
   uint32 seqNum        : 8;
   uint32 tod           :24;
   /* Word 7 */
   uint32 headerL       : 1;
   uint32 headerOffset  :10;
   uint32 fragRuleHit   : 1;
   uint32 fragRule      : 4;
   uint32 serial        :16;
}VIDRV_ipa_HwSniffStatusType;

/**
 * @brief   Structure of HW sniffer frag status
 */
typedef PACK(struct)
{ /* entry type 2 */
   /* Word 0 */
   uint32 entryType     : 8;
   uint32 fragRuleIndx  : 4;
   uint32 res1          : 3;
   uint32 fragTable     : 1;
   uint32 endpSrcIndex  : 5;
   uint32 exception     : 1;
   uint32 res2          : 2;
   uint32 seqNum        : 8;
   /* Word 1 */
   uint32 srcIpAddress  :32;
   /* Word 2 */
   uint32 dstIpAddress  :32;
   /* Word 3 */
   uint32 headerRet     : 1;
   uint32 res3          : 5;
   uint32 natType       : 2;
   uint32 ipProtocol    : 8;
   uint32 id            :16;
   /* Word 4 */
   uint32 translatedIp  :32;
   /* Word 5 */
   uint32 headerL       : 1;
   uint32 headerOffset  :10;
   uint32 endDstIndex   : 5;
   uint32 ipChecksumDiff:16;
   /* Word 6 */
   uint32 metadata      :32;
   /* Word 7 */
   uint32 res4          :16;
   uint32 serial        :16;
}VIDRV_ipa_HwSniffFragStatusType;

/**
 * @brief   Structure of HW sniffer DPL status
 */
typedef PACK(struct)
{ /* entry type 0x10 */
   /* Word 0 */
   uint32 entryType     : 8;
   uint32 res1          :24;
   /* Word 1 */
   uint32 packetLen     :16;
   uint32 srcEndp       : 5;
   uint32 res2          :11;
   /* Word 2 */
   uint32 metadata      :32;
   /* Word 3 */
   uint32 res3          :32;
   /* Word 4 */
   uint32 res4          :16;
   uint32 todLsb        :16;
   /* Word 5 */
   uint32 todMsb        :32;
   /* Word 6 */
   uint32 seqNum        : 8;
   uint32 res5          :24;
   /* Word 7 */
   uint32 res6          :16;
   uint32 serial        :16;
}VIDRV_ipa_HwSniffDplStatusType;

/**
 * @brief   Structure of HW sniffer decompression status
 */
typedef PACK(struct)
{ /* entry type 0x20 */
   /* Word 0 */
   uint32 entryType     : 8;
   uint32 errorMask     : 4;
   uint32 res1          :20;
   /* Word 1 */
   uint32 packetLen     :16;
   uint32 srcEndp       : 5;
   uint32 res2          : 3;
   uint32 dstEndp       : 5;
   uint32 res3          : 3;
   /* Word 2 */
   uint32 res4          :32;
   /* Word 3 */
   uint32 res5          :32;
   /* Word 4 */
   uint32 res6          :32;
   /* Word 5 */
   uint32 numProcCycles :32;
   /* Word 6 */
   uint32 seqNum        : 8;
   uint32 tod           :24;
   /* Word 7 */
   uint32 res7          :16;
   uint32 serial        :16;
}VIDRV_ipa_HwSniffDcmpStatusType;

/**
 * @brief   Structure of HW sniffer status element
 */
typedef PACK(union){
   VIDRV_ipa_HwStatusEntryType      entryType : 8;
   VIDRV_ipa_HwSniffStatusType      status;
   VIDRV_ipa_HwSniffFragStatusType  frag;
   VIDRV_ipa_HwSniffDplStatusType   dpl;
   VIDRV_ipa_HwSniffDcmpStatusType  dcmp;
}VIDRV_ipa_HwSniffStatusElementType;

#ifdef WIN32
#pragma pack(pop) /* Revert alignment to what it was previously */
#endif

/**
 * @brief   Structure of HW sniffer status pointers
 */
typedef struct{
   const VIDRV_ipa_HwSniffStatusElementType*  pWritePtr;     /**< Write pointer of external FIFO */
   VIDRV_ipa_HwSniffStatusElementType*        pReadIntPtr;   /**< Read  pointer of internal FIFO */
   VIDRV_ipa_HwSniffStatusElementType*        pWriteIntPtr;  /**< Write pointer of internal FIFO */
}VIDRV_ipa_SnifferStatusPositionType;


/* -----------------------------------------------------------------------
**                           STATUS QUEUE
** ----------------------------------------------------------------------- */

/**
 * @brief   Types of messages in status queue. 
 */
typedef enum VIDRV_ipa_StatusQueueItemOpCode
{
   IPA_STATUS_ANY_ITEM           = -1,     /**< Any Item */
   IPA_STATUS_PACKET             = 1 << 0, /**< Packet status */
   IPA_STATUS_FRAG_NEW_RULE      = 1 << 1, /**< Frag. new rule */
   IPA_STATUS_DROPPED_PACKET     = 1 << 2, /**< Dropped packet */
   IPA_STATUS_SUSPENDED_PACKET   = 1 << 3, /**< Suspended packet */
   IPA_STATUS_LOG                = 1 << 4, /**< Log status */
   IPA_STATUS_DCMP               = 1 << 5, /**< De/compression status */
   IPA_STATUS_PACKET_2ND_PASS    = 1 << 6  /**< Packet status from IPH SECOND */
} VIDRV_ipa_StatusQueueItemOpCode;

/**
 * @brief   Exception code for IPA_STATUS_PACKET and IPA_STATUS_DROPPED_PACKET. See exceptionType field in VIDRV_ipa_SQPacketType 
 */
typedef enum VIDRV_ipa_SQExeptionType
{
    IPA_SQEXP_NO_EXCEPTION          = 0x0,   /**< No exception */
    IPA_SQEXP_DEAGGREGATION         = 0x1,   /**< De-Aggregation exception */
    IPA_SQEXP_RESERVED              = 0x2,   /**< Reserved exception */
    IPA_SQEXP_IP_TYPE               = 0x4,   /**< IP type exception */
    IPA_SQEXP_PACKET_LENGTH         = 0x8,   /**< Packet Length (oldest IHL) exception */
    IPA_SQEXP_IPH_PARSER_THRESHOLD  = 0x9,   /**< IPH Packet Parser Threshold exception */
    IPA_SQEXP_FRAG_MISS             = 0x10,  /**< Frag. Rule miss */
    IPA_SQEXP_SW_FILTER             = 0x20,  /**< SW filter exception */
    IPA_SQEXP_NAT                   = 0x40,  /**< NAT exception */
    IPA_SQEXP_UCP                   = 0x80,  /**< UCP exception */
} VIDRV_ipa_SQExeptionType;

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif


/**
 * @brief   Packet status bit mask 
 */
typedef PACK(struct) VIDRV_ipa_SQStatusMaskType {
   uint16 fragProcess: 1;  /**< 1 if packet was processed by frag block and Fragmentation info is valid. */
   uint16 filtProcess: 1;  /**< 1 if packet was processed by filtering block and filtering info is valid. */
   uint16 natProcess: 1;   /**< 1 if packet was processed by NAT block and NAT info is valid. */
   uint16 routeProcess: 1; /**< 1 if packet was processed by Routing block and routing info is valid. */
   uint16 tagValid: 1;     /**< 1 if TAG is valid and TAG info is valid. */
   uint16 fragment: 1;     /**< 1 if packet is an IP fragment (Does not depend on FRAG_OFFLOAD_EN setting) */
   uint16 firstFragment: 1;/**< 1 if packet is a first IP fragment (Does not depend on FRAG_OFFLOAD_EN setting) */
   uint16 isIpV4: 1;       /**< 1 if packet is IPv4, 0 if packet is IPv6. Not valid if IPA Packet Processing did not complete, such as in case of IP type exception.*/
   uint16 checksumProcess: 1;/**< 1 if packet was processed by checksum block, which means that checksum trailer exists */
   uint16 aggrProcess: 1;  /**< 1 if packet was processed by aggregation block */
   uint16 destEot  : 1;    /**< 1 if aggregation ended with this packet (if Packet Length is zero, then aggregation ended due to timer).  */
   uint16 deaggrProcess: 1;/**< 1 if packet was processed by deaggregation block */
   uint16 deaggrFirst: 1;  /**< 1 if this is the first packet in a deaggregation frame  */
   uint16 srcEot: 1;       /**< 1 if source endpoint has asserted End of Transfer when sending this buffer. Only set in case of threshold based DMA mode, where source data is sent in chunks. EOT bit will be set on the last chunk of data / last part of a packet in such a transfer. */
   uint16 prevEot: 1;      /**< 1 if EOT was sent just before the packet as part of Aggregation hard byte limit behavior (Packet did not fit within the open frame’s byte limit. That frame was closed and packet is part of a new frame). IPA 2.1 only. */
   uint16 byteLimit: 1;    /**< 1 if Packet is over a configured byte limit. In case of a packet drop due to a packet being over the aggregation hard byte limit, this bit will be set. IPA 2.1 only. */
} VIDRV_ipa_SQStatusMaskType;

/** 
* @brief status bitmask
*/
typedef PACK(union) VIDRV_ipa_SQStatusBits
{
   uint16 raw; /**< raw mapping*/
   VIDRV_ipa_SQStatusMaskType bits; /**< bits mapping*/
} VIDRV_ipa_SQStatusBits;

/** 
* @brief protocol encoding
*/
typedef enum VIDRV_ipa_SQProtocolEncoding
{
   IPA_SQPROT_NONE          = 0x0,   /**< Protocol Encoding is not set */
   IPA_SQPROT_TCP           = 0x1,   /**< TCP */
   IPA_SQPROT_UDP           = 0x2,   /**< UDP */
   IPA_SQPROT_ICMP          = 0x3,   /**< ICMP or IPv6-ICMP (status mask
                                          indicates if it is IPv4 or IPv6) */
} VIDRV_ipa_SQProtocolEncoding;

/**
 * @brief   Structure of IPA_STATUS_PACKET. 
 */
typedef PACK(struct) VIDRV_ipa_SQPacketType {
    /* word 0 */
    uint16 opCode                      : 8;  /**< VIDRV_ipa_StatusQueueItemOpCode */
    uint16 exceptionType               : 8;  /**< VIDRV_ipa_SQExeptionType */
    VIDRV_ipa_SQStatusBits statusMask;       /**< Status bitmask*/

    /* word 1 */
    uint32 packetLength             : 16; /**< Packet payload length including header (header added as part of header addition) + payload, including retained header if used. Does NOT include padding or checksum trailer length. */
    uint32 endpointSrcIndex         : 5;  /**< Source end point index */
    uint32 pureAck                  : 1;  /**< Pure Ack*/
    uint32 syn                      : 1;  /**< SYN*/
    uint32 finRst                   : 1;  /**< FIN/RST*/
    uint32 endpointDestIndex        : 5;  /**< Destination end point index */
    uint32 protocolEncoding         : 3;  /**< Protocol Encoding*/

    /* word 2 */
    uint32 metaData;                      /**< Metadata*/
    
    /* word 3 */
    uint32 isFilterTableInLocalMemory        : 1;  /**< 1: filter table in local memory, 0: filter table in system memory */
    uint32 isMatchingFilterRuleInHashTable   : 1;  /**< 1: Matching filter rule is in hashed table, 0: not in hashed table */
    uint32 _res3                             : 1;  /**< Reserved field */
    uint32 isHeaderRetainSpecified           : 1;  /**< 1: filter rule specifies to retain header, 0: filter rule does not specify to retain header */
    uint32 filterRuleIndex                   : 10; /**< Filter rule index, 0x3FF on filter miss */
    uint32 isRoutingTableInLocalMemory       : 1;  /**< 1: routing table in local memory, 0: routing table in system memory */
    uint32 isMatchingRoutingRuleInHashTable  : 1;  /**< 1: Matching Routing rule is in hashed table, 0: not in hashed table */
    uint32 isUcpProcessed                    : 1;  /**< 1: the packet underwent uC data path processing (uCP stage had processed the packet). 0: otherwise */
    uint32 routingTableIndex                 : 5;  /**< Routing table index, 0x3FF on routing miss */
    uint32 routingRuleIndex                  : 10; /**< Starts from 1 */

    /* word 4 and 5 */
    uint16 isNatHit                          : 1;  /**< NAT hit/miss (1 bit) - 1: hit, 0: miss */
    uint16 natTableIndex                     : 13; /**< NAT table index */
    uint16 natType                           : 2;  /**< 00: No NAT, 01: Source NAT, 10: Destination NAT, 11: Reserved */
    uint16 tagInfo[3];                             /**< SW defined value provided via immediate command */

    /* word 6 */
    uint32 sequenceNumber                    : 8;  /**< Per source endpoint unique 8 bit packet sequence Number */
    uint32 timeOfDayCounter                  : 24; /**< Running counter from 32KHz clock */
    
    /* word 7 */
    uint32 isHeaderTableInSystemMemory       : 1;  /**< 0: header resides in the local memory, 1: header resides in the system memory */
    uint32 headerOffset                      : 10; /**< Valid only if ROUTE_PROCESS is set */
    uint32 isFragHit                         : 1;  /**< Frag rule hit in HW frag table */
    uint32 fragRule                          : 4;  /**< Frag rule index in HW frag table */
    uint32 sniffSerialNumber                 : 16; /**< Status serial number, HW specific */
} VIDRV_ipa_SQPacketType;

/**
 * @brief   Structure of IPA_STATUS_FRAG_NEW_RULE. 
 */
typedef PACK(struct) VIDRV_ipa_SQFragNewRuleType
{
   /* word 0 */
    uint32 opCode                      : 8;     /**< VIDRV_ipa_StatusQueueItemOpCode */
    uint32 fragRuleIndex               : 4;     /**< Frag rule index in HW frag table */
    uint32 _res1                       : 3;     /**< Reserved field */
    uint32 fragmentTableIndex          : 1;     /**< Fragment Table Index. ( 0 – first fragment table, 1 - second fragment table ) */
    uint32 epSrcIndex                  : 5;     /**< Consumer pipe ID*/
    uint32 exceptionRule               : 1;		/**< If set identifies the rule as an exception rule added to frag rule table (i.e. SW filter or NAT exception) */
    uint32 _res2                       : 2;     /**< Reserved field*/
    uint32 sequenceNumber              : 8;     /**< Per source endpoint unique 8 bit packet sequence Number */
    /* word 1 */
    uint32 srcIpAddress;                        /**< Source IP address*/
    /* word 2 */
    uint32 dstIpAddress;                        /**< Destination IP address*/
    /* word 3 */
    uint32 isHeaderRetain              : 1;     /**< If set, source header is retained all packet fragments */
    uint32 _res3                       : 5;     /**< Reserved field */
    uint32 natType                     : 2;     /**< 00: No NAT 01: Source NAT 10: Destination NAT 11: Reserved */
    uint32 ipProto                     : 8;     /**< IP protocol code */
    uint32 fragId                      : 16;    /**< Used to identify the fragments of one datagram from those of another as defined within the IPv4 header */
    /* word 4 */
    uint32 transIpAddress;                      /**< Translated IP Address (32bit): The IP address the source or destination IP address need to be translated to, depending on the NAT type (src/dst) */
    /* word 5 */
    uint32 isHeaderTableInSystemMemory : 1;     /**< The header table that holds the header to be added on top of the packet : 0 - local memory; 1 - system memory */
    uint32 hdrOffset                   : 10;    /**< Offset in words of the header to be added. This offset refers to the beginning of the Header Table defined by the ‘L’ field */
    uint32 epDstIndex                  : 5;     /**< Index of endpoint to which packet should be routed */
    uint32 ipChecksumDiff              : 16;    /**< Checksum correction that should be used to fix the packet’s IP Checksum field, following an address translation (similar to the one defined in the matching NAT rule, however in this case only IP address change is corrected for. Port does not change) */
    /* word 6 */
    uint32 metaData;                            /**< Metadata information*/
    /* word 7 */
    uint32 _res4;                               /**< Reserved field*/
} VIDRV_ipa_SQFragNewRuleType;

/**
 * @brief   The structure of IPA_STATUS_DROPPED_PACKET is the same as IPA_STATUS_PACKET status element, except a different Status Opcode is used.
 */
typedef VIDRV_ipa_SQPacketType VIDRV_ipa_SQDroppedPacketType;

/**
 * @brief   The structure of IPA_STATUS_SUSPENDED_PACKET is the same as IPA_STATUS_PACKET status element, except a different Status Opcode is used.
 */
typedef VIDRV_ipa_SQPacketType VIDRV_ipa_SQSuspendedPacketType; 

/**
 * @brief   Structure of IPA_STATUS_LOG. 
 */
typedef PACK(struct) VIDRV_ipa_SQLogType
{
   /* word 0 */
    uint16 opCode                      : 8; /**< VIDRV_ipa_StatusQueueItemOpCode */
    uint16 _res1                       : 8; /**< VIDRV_ipa_SQExeptionType. Only IPA_SQEXP_NO_EXCEPTION (0) and IPA_SQEXP_DEAGGREGATION (1) are valid values */
    uint16 _res2;

    /* word 1 */
    uint32 packetLength                : 16; /**< Packet payload length including header (header added as part of header addition) + payload, including retained header if used. Does NOT include padding or checksum trailer length. */
    uint32 endpointSrcIndex            : 5;  /**< Source end point index */
    uint32 _res3                       : 11;  /**< Reserved field*/

    /* word 2 */
    uint32 metaData;                         /**< Metadata*/
    
    /* word 3 */
    uint32 _res4;                            /**< Reserved field*/
    
    /* word 4 and 5 */
    uint16 _res5                       : 16; /**< Reserved field */
    uint16 dplTimeOfDay[3];                  /**< DPL Time of day, in 1msec granularity. Slow clock only, even if Time of Day is changed to use IPA clock */

    /* word 6 */
    uint32 sequenceNumber              : 8;  /**< Per source endpoint unique 8 bit packet sequence Number */
    uint32 _res6                       : 24; /**< Running counter from 32KHz clock */
    
    /* word 7 */
    uint32 _res7                       : 16; /**< Reserved field */
    uint32 sniffSerialNumber           : 16; /**< Status serial number, HW specific */
} VIDRV_ipa_SQLogType;

/**
 * @brief   Structure of IPA_STATUS_DCMP.
 */
typedef PACK(struct) VIDRV_ipa_SQDcmpType
{
   /* word 0 */
    uint32 opCode             : 8;  /**< VIDRV_ipa_StatusQueueItemOpCode */
    uint32 errorMask          : 4;  /**< Used to indicate error coming from CMP/DCMP engine */
    uint32 _res1              : 20; /**< Reserved field*/

    /* word 1 */
    uint32 packetLength       : 16; /**< Packet payload length including header (header added as part of header addition) + payload, including retained header if used. Does NOT include padding or checksum trailer length. */
    uint32 endpointSrcIndex   : 5;  /**< Source end point index */
    uint32 _res2              : 3;  /**< Reserved field*/
    uint32 endpointDestIndex  : 5;  /**< Destination end point index */
    uint32 _res3              : 3;  /**< Reserved field*/

    /* word 2 - 4*/
    uint32 _res4[3];                /**< Reserved field*/

    /* word 5 */
    uint32 numberOfProcessingCycles;/**< Internal De/comp counter indicating how many cycles the engine ran for this packet*/

    /* word 6 */
    uint32 sequenceNumber     : 8;  /**< Per source endpoint unique 8 bit packet sequence Number */
    uint32 timeOfDayCounter   : 24; /**< Running counter from 32KHz clock */
    
    /* word 7 */
    uint32 _res5              : 16; /**< Reserved field */
    uint32 sniffSerialNumber  : 16; /**< Status serial number, HW specific */
} VIDRV_ipa_SQDcmpType;

/**
* @brief   Union for status mapping. 
*/
typedef PACK(union) VIDRV_ipa_StatusQueuePacketType
{
    uint32                          raw            : 8;  /**< Raw mapping*/
    VIDRV_ipa_StatusQueueItemOpCode opCode         : 8;  /**< Opcode mapping*/
    VIDRV_ipa_SQPacketType          packetStatus;        /**< Packet status mapping*/
    VIDRV_ipa_SQFragNewRuleType     fragStatus;          /**< Frag status mapping*/
    VIDRV_ipa_SQDroppedPacketType   droppedStatus;       /**< Dropped status mapping*/
    VIDRV_ipa_SQSuspendedPacketType suspendedStatus;     /**< Suspended status mapping*/
    VIDRV_ipa_SQLogType             logStatus;           /**< Log status mapping*/
    VIDRV_ipa_SQDcmpType            dcmpStatus;          /**< De/comp status mapping*/
} VIDRV_ipa_StatusQueuePacketType;

/**
 * @brief   Handle for Status Queue. 
 */
struct IpaStatusQueue;

/**
 * @brief   Handle for Status Queue. 
 */
typedef struct IpaStatusQueue* VIDRV_ipa_StatusQueueHandle; 


/**
 * @brief   Structure of IPA_STATE register.
*/
typedef PACK(struct)
{
   uint32 rxWait                 :  1; /**<    [ 0] RX_WAIT                            If set, RX is waiting for more data */
   uint32 rxIdle                 :  1; /**<    [ 1] RX_IDLE                            If set, all RX pipes are empty */
   uint32 txIdle                 :  1; /**<    [ 2] TX_IDLE                            If set, TX is idle */
   uint32 txWait                 :  1; /**<    [ 3] TX_WAIT                            if set, TX is waiting for buffers */
   uint32 bamIdle                :  1; /**<    [ 4] BAM_IDLE                           Indicates BAM is IDLE.
                                                                                       Hysteresis of 15 cycles is implemented.
                                                                                       Meaning the bit is 1 only if BAM is idle for 15 cycles or more. */
   uint32 ipaStatusSnifferIdle   :  1; /**<    [ 5] IPA_STATUS_SNIFFER_IDLE            If set, SNIFFER is idle */
   uint32 ipaNocIdle             :  1; /**<    [ 6] IPA_NOC_IDLE                       If set, NOC axi master is IDLE */
   uint32 aggrIdle               :  1; /**<    [ 7] AGGR_IDLE                          If set, aggregation is idle */
   uint32 mbimAggrIdle           :  1; /**<    [ 8] MBIM_AGGR_IDLE                     If set, MBIM aggregation is idle */
   uint32 ipaRsrcMngrDbEmpty     :  1; /**<    [ 9] IPA_RSRC_MNGR_DB_EMPTY(Since 3.1)  If set, RSRC manager is empty */
   uint32 ipaRsrcStateIdle       :  1; /**<    [10] IPA_RSRC_STATE_IDLE(Since 3.1)     If set, RSRC manager state machine is in IDLE */
   uint32 ipaAckMngrDbEmpty      :  1; /**<    [11] IPA_ACKMNGR_DB_EMPTY(Since 3.1)    If set, ACK manager is empty */
   uint32 ipaAckMngrStateIdle    :  1; /**<    [12] IPA_ACKMNGR_STATE_IDLE(Since 3.1)  If set, ACK manager state machine is in IDLE */
   uint32 ipaTxAckqFull          :  1; /**<    [13] IPA_TX_ACKQ_FULL(Since 3.1)        If set, ACK manager is full */
   uint32 ipaProdAckmngrDbEmpty  :  1; /**<    [14] IPA_PROD_ACKMNGR_DB_EMPTY(Since 3.5) */
   uint32 ipaProdAckmngrStateIdle:  1; /**<    [15] IPA_PROD_ACKMNGR_STATE_IDLE(Since 3.5) */
   uint32 ipaProdBrespIdle       :  1; /**<    [16] IPA_PROD_BRESP_IDLE(Since 3.5)     */
   uint32 reservedBbits_17       :  1; /**<    [17] RESERVED_BITS17                    Reserved bits */
   uint32 ipaNtfTxEmpty          :  1; /**<    [18] IPA_NTF_TX_EMPTY                   If set, queue is empty */
   uint32 ipaTxAckqEmpty         :  1; /**<    [19] IPA_TX_ACKQ_EMPTY                  If set, queue is empty */
   uint32 ipaUcAckqEmpty         :  1; /**<    [20] IPA_UC_ACKQ_EMPTY                  If set, queue is empty */
   uint32 ipaRxAckqEmpty         :  1; /**<    [21] IPA_RX_ACKQ_EMPTY                  If set, queue is empty */
   uint32 ipaTxCommanderCmdqEmpty:  1; /**<    [22] IPA_TX_COMMANDER_CMDQ_EMPTY        If set, queue is empty */
   uint32 ipaRxSpltCmdq0Empty    :  1; /**<    [23] IPA_RX_SPLT_CMDQ_0_EMPTY           If set, queue is empty */
   uint32 ipaRxSpltCmdq1Empty    :  1; /**<    [24] IPA_RX_SPLT_CMDQ_1_EMPTY           If set, queue is empty */
   uint32 ipaRxSpltCmdq2Empty    :  1; /**<    [25] IPA_RX_SPLT_CMDQ_2_EMPTY           If set, queue is empty */
   uint32 ipaRxSpltCmdq3Empty    :  1; /**<    [26] IPA_RX_SPLT_CMDQ_3_EMPTY           If set, queue is empty */
   uint32 ipaRxSpltCmdq4Empty    :  1; /**<    [27] IPA_RX_SPLT_CMDQ_4_EMPTY           If set, queue is empty */
   uint32 ipaRxHpsEmpty          :  1; /**<    [28] IPA_RX_HPS_EMPTY                   Bitwise AND on IPA_RX_HPS_CMDQ_STATUS_EMPTY. If set, all queues are empty */
   uint32 ipaHpsDpsEmpty         :  1; /**<    [29] IPA_HPS_DPS_EMPTY                  Bitwise AND on IPA_HPS_DPS_CMDQ_STATUS_EMPTY. If set, all queue are empty */
   uint32 ipaDpsTxEmpty          :  1; /**<    [30] IPA_DPS_TX_EMPTY                   Bitwise AND on IPA_DPS_TX_CMDQ_STATUS_EMPTY. If set, all queue are empty */
   uint32 ipaUcRxHandleCmdqEmpty :  1; /**<    [31] IPA_UC_RX_HND_CMDQ_EMPTY           If set, uC proc queue is empty */
}
VIDRV_ipa_StateType;

/**
 * @brief   Structure of IPA_STATE_FETCHER register.
*/
typedef PACK(struct)
{
   uint32 hpsFtchStateIdle       :  1; /**<    [ 0] IPA_HPS_FTCH_STATE_IDLE */
   uint32 hpsFtchAllocStateIdle  :  1; /**<    [ 1] IPA_HPS_FTCH_ALLOC_STATE_IDLE */
   uint32 hpsFtchPktStateIdle    :  1; /**<    [ 2] IPA_HPS_FTCH_PKT_STATE_IDLE */
   uint32 hpsFtchImmStateIdle    :  1; /**<    [ 3] IPA_HPS_FTCH_IMM_STATE_IDLE */
   uint32 hpsFtchCmpltStateIdle  :  1; /**<    [ 4] IPA_HPS_FTCH_CMPLT_STATE_IDLE */
   uint32 hpsDmarStateIdle       :  7; /**<    [5:11] IPA_HPS_DMAR_STATE_IDLE */
   uint32 hpsDmarRepStateIdle    :  7; /**<    [12:18] IPA_HPS_DMAR_REP_STATE_IDLE */
   uint32 hpsImmCmdExecStateIdle :  1; /**<    [19] IPA_HPS_IMM_CMD_EXEC_STATE_IDLE */
   uint32 dpsFtchPktStateIdle    :  1; /**<    [20] IPA_DPS_FTCH_PKT_STATE_IDLE */
   uint32 dpsFtchCmpltStateIdle  :  1; /**<    [21] IPA_DPS_FTCH_CMPLT_STATE_IDLE */
   uint32 dpsDmarStateIdle       :  5; /**<    [22:26] IPA_DPS_DMAR_STATE_IDLE */
}
VIDRV_ipa_StateFetcherType;

/**
 * @brief   Structure of IPA_STATE_ACL register.
*/
typedef PACK(struct)
{
   uint32 hpsHDcphEmpty       :  1; /**<    [ 0] IPA_HPS_H_DCPH_EMPTY */
   uint32 hpsHDcphActive      :  1; /**<    [ 1] IPA_HPS_H_DCPH_ACTIVE */
   uint32 hpsPktParserEmpty   :  1; /**<    [ 2] IPA_HPS_PKT_PARSER_EMPTY */
   uint32 hpsPktParserActive  :  1; /**<    [ 3] IPA_HPS_PKT_PARSER_ACTIVE */
   uint32 hpsFilterNatEmpty   :  1; /**<    [ 4] IPA_HPS_FILTER_NAT_EMPTY */
   uint32 hpsFilterNatActive  :  1; /**<    [ 5] IPA_HPS_FILTER_NAT_ACTIVE */
   uint32 hpsRouterEmpty      :  1; /**<    [ 6] IPA_HPS_ROUTER_EMPTY */
   uint32 hpsRouterActive     :  1; /**<    [ 7] IPA_HPS_ROUTER_ACTIVE */
   uint32 hpsHdriEmpty        :  1; /**<    [ 8] IPA_HPS_HDRI_EMPTY */
   uint32 hpsHdriActive       :  1; /**<    [ 9] IPA_HPS_HDRI_ACTIVE */
   uint32 hpsUcpEmpty         :  1; /**<    [10] IPA_HPS_UCP_EMPTY */
   uint32 hpsUcpActive        :  1; /**<    [11] IPA_HPS_UCP_ACTIVE */
   uint32 hpsEnqueuerEmpty    :  1; /**<    [12] IPA_HPS_ENQUEUER_EMPTY */
   uint32 hpsEnqueuerActive   :  1; /**<    [13] IPA_HPS_ENQUEUER_ACTIVE */
   uint32 dpsDDcphEmpty       :  1; /**<    [14] IPA_DPS_D_DCPH_EMPTY */
   uint32 dpsDDcphActive      :  1; /**<    [15] IPA_DPS_D_DCPH_ACTIVE */
   uint32 dpsDcmpEmpty        :  1; /**<    [16] IPA_DPS_DCMP_EMPTY */
   uint32 dpsDcmpActive       :  1; /**<    [17] IPA_DPS_DCMP_ACTIVE */
   uint32 dpsDispatcherEmpty  :  1; /**<    [18] IPA_DPS_DISPATCHER_EMPTY */
   uint32 dpsDispatcherActive :  1; /**<    [19] IPA_DPS_DISPATCHER_ACTIVE */
   uint32 dpsDDcph2Empty      :  1; /**<    [20] IPA_DPS_D_DCPH_2_EMPTY */
   uint32 dpsDDcph2Active     :  1; /**<    [21] IPA_DPS_D_DCPH_2_ACTIVE */
   uint32 reservedBbits_31_22 :  10;/**<    [22:31] RESERVED_BITS31_22*/
}
VIDRV_ipa_StateAclType;

/**
 * @brief   Structure of IPA_SNOC_FEC_EE_n registers (System-NOC First Error Captured). 
*/
typedef PACK(union) VIDRV_ipa_SnocFecUnion
{
   uint32 regRawValue;
   PACK(struct) regSnocFecStruct
   {
      uint32 client                 :  8; /**< [ 7: 0]   QMB-client that issued the transaction that caused the error on SNOC.         */
      uint32 qmbIndex               :  1; /**< [ 8]      QMB-instance that executed the transaction that caused the error on SNOC.     */
      uint32 reserved0              :  3; /**< [11: 9]   reserved */
      uint32 tid                    :  8; /**< [19:12]   TID of transaction that caused the error on SNOC.                             */
      uint32 reserved1              : 11; /**< [30:20]   reserved */
      uint32 rw                     :  1; /**< [31]      Denotes whether Read/Write access-error: 0x1 - Read, 0x0 - Write              */
   } regSnocFec;
}
VIDRV_ipa_SnocFecType;

/**
 * @brief   Quota/Tethering statistics counters structure .
 */
typedef PACK(struct) VIDRV_ipa_StatisticsDataCountersStruct
{
   uint64   numIpv4Bytes;
   uint32   numIpv4Pkts;
   uint32   numIpv6Pkts;
   uint64   numIpv6Bytes;
} VIDRV_ipa_StatisticsDataCountersType;

/**
 * @brief   Tethering statistics config structure .
 */
typedef PACK(struct) VIDRV_ipa_TetheringStatisticsConfigStruct
{
   uint32   dstMask;
   uint32   offset;
} VIDRV_ipa_TetheringStatisticsConfigType;

/**
 * @brief   FnR statistics config structure .
 */
typedef PACK(struct) VIDRV_ipa_FnrStatisticsConfigStruct
{
   uint32   enMask;
   uint16   actOffset;
   uint16   cntOffset;
} VIDRV_ipa_FnrStatisticsConfigType;

/**
 * @brief   Fnr statistics counters structure .
 */
typedef PACK(struct) VIDRV_ipa_StatisticsRuleCountersStruct
{
   uint32   numPktsHash;
   uint32   numPkts;
} VIDRV_ipa_StatisticsRuleCountersType;

#ifdef WIN32
#pragma pack(pop) /* Revert alignment to what it was previously */
#endif

/**
 * @brief   Quota statistic configuration structure
 */
typedef struct VIDRV_ipa_QuotaStatisticConfigStruct
{
   VIDRV_ipa_PipeHandle* pipes;
   uint32 numOfPipes;
} VIDRV_ipa_QuotaStatisticConfigType;

/**
 * @brief   Tethering statistic configuration structure
 */
typedef struct VIDRV_ipa_TetheringStatisticConfigSrcStruct
{
   VIDRV_ipa_PipeHandle srcPipe, dstPipe;
} VIDRV_ipa_TetheringStatisticConfigPairType;

typedef struct VIDRV_ipa_TetheringStatisticConfigStruct
{
   uint32 numOfPairs;
   VIDRV_ipa_TetheringStatisticConfigPairType* pairs;
} VIDRV_ipa_TetheringStatisticConfigType;

/**
 * @brief   Configuration of filter/router V4/V6 statistic
 */
typedef struct VIDRV_ipa_FnrRuleStatisticStruct
{
   uint32* ruleIds;
   uint32 numOfRules;
}VIDRV_ipa_FnrRuleStatisticType;

/**
 * @brief   Enumeration of filter/router V4/V6 statistic
 */
typedef enum VIDRV_ipa_FnrStatisticsType
{
   IPA_FILTER_V4_STATISTICS,
   IPA_ROUTER_V4_STATISTICS,
   IPA_FILTER_V6_STATISTICS,
   IPA_ROUTER_V6_STATISTICS,
   IPA_FNR_STATISTICS_NUM
}VIDRV_ipa_FnrStatisticsType;

/**
 * @brief   Fnr statistic configuration structure
 */
typedef struct VIDRV_ipa_FnrStatisticConfigStruct
{
   VIDRV_ipa_FnrRuleStatisticType instance[IPA_FNR_STATISTICS_NUM];
}VIDRV_ipa_FnrStatisticConfigType;

/* -----------------------------------------------------------------------
**                           INTERFACES
** ----------------------------------------------------------------------- */


/** @defgroup VIDRV_ipa_interface IPA driver interface
 *
 *  The following functions describe the interface of the VI IPA driver
 *
 *  @{
*/

/* -----------------------------------------------------------------------
**                         GENERAL INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup General
 *
 *  The following functions describe the general functionality of the IPA driver
 *
 *  @{
 */


/**
 * @brief   Gets the RTL version of IPA code.
 *
 * @return  The RTL version. In case the version is unknown VIDRV_IPA_UNKNOWN_VERSION will be returned. 
*/
VIDRV_ipa_VersionType VIDRV_ipa_GetVersion(void);

/**
 * @brief   Constructor of the IPA driver. The function will initialize the IPA HAL and the BAM driver.
 * Note that when the BAM is initialized, it is also reset and enabled.
 *
 * @param   ipaConfig   If non-null, the IPA configuration. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_Init(VIDRV_ipa_ConfigType ipaConfig);

/**
 * @brief   The IPA driver destructor.
 * The function will  free the IPA HAL and release the BAM driver.
 * It will also free all the IPA pipes and there corresponding descriptors and requests.
*/
void VIDRV_ipa_Cleanup(void);

/**
 * @fn   VIDRV_ipa_ResultType VIDRV_ipa_Enable(void)
 *
 * @brief   Enables the IPA, configures BCRs, IPA interrupt handler, HW sniffer, system pipe.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_Enable(void);

/**
 * @brief   Same as VIDRV_ipa_Enable, but allows selectively skip various default features. 
 *
 * @param   ipaFeatureControl        Bitmap of IPA feature settings.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableEx(VIDRV_ipa_FeatureControl ipaFeatureControl);

/**
 * @fn   VIDRV_ipa_ResultType VIDRV_ipa_Disable(void)
 *
 * @brief   Disables the IPA.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_Disable(void);

/**
 * @fn   VIDRV_ipa_ResultType VIDRV_ipa_Reset(void)
 *
 * @brief   Resets the core and the corresponding pipes.
 * The function will reset the core all the pipes.
 * It will also remove all transfers and free the BAM descriptors.
 *
 * @note The BAM itself will not be reseted.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_Reset(void);

/**
 * @brief   Set default BCR setting for IPA.
 *
 * @note In order to control the BCR bit setting need to call to define manager
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetBcr(void);

/**
 * @brief   Resets the specified pipe and frees the descriptors and transfers. 
 *
 * @param   hPipe The pipe handle. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_PipeReset(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Gets the shared memory address (the starting address of the SRAM).
 *
 * @return  The address of the shared memory available for SW, in case of an error 0 will be returned. 
*/
void* VIDRV_ipa_GetSharedMemoryAddress(void);

/**
 * @brief   Gets the uC instruction ram address (the starting address of the IRAM).
 *
 * @return  The address of the shared memory available for SW, in case of an error 0 will be returned. 
*/
void* VIDRV_ipa_GetUcIramAddress(void);

/**
 * @brief   Gets the uC data ram address (the starting address of the DRAM).
 *
 * @return  The address of the shared memory available for SW, in case of an error 0 will be returned. 
*/
void* VIDRV_ipa_GetUcDramAddress(void);

/**
 * @brief   Gets the uC instruction ram size.
 *
 * @return  The size of IRAM
*/
uint32 VIDRV_ipa_GetUcIramSize(void);

/**
 * @brief   Gets the uC data ram size.
 *
 * @return  The size of DRAM
*/
uint32 VIDRV_ipa_GetUcDramSize(void);

/**
 * @brief   Gets the size of the shared memory available for SW in bytes. 
 *
 * @return  The size of shared/local memory available for SW in bytes, in case of an error 0 will be returned. 
*/
uint32 VIDRV_ipa_GetSharedMemorySize(void);

/**
 * @brief   Reset the internal/local/shared memory (SRAM)
 *
 * @param   resetValue      The value (pattern) to be written in each SRAM 32-bit word. 
*/
void VIDRV_ipa_ResetSharedMemory(uint32 resetValue);

/**
 * @brief   Gets the base address of a shared memory between the driver and the uC. 
 *
 * @return  The shared memory base address, 0 on failure. 
*/
size_t VIDRV_ipa_ucGetSharedMemAddress(void);

/**
 * @brief   Writes to the shared memory. 
 * 
 * @param   offset      The offset inside the shared memory. 
 * @param   size        The size of the buffer to write in bytes. 
 * @param [in] data     If non-null, the address of the data buffer. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SharedMemoryWrite(uint32 offset, uint32 size, const void* data);

/**
 * @brief   Reads from the shared memory. 
 * 
 * @param   offset      The offset inside the shared memory. 
 * @param   size        The size of the buffer to read in bytes. 
 * @param [out] data    If non-null, the address of the data buffer. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SharedMemoryRead(uint32 offset, uint32 size, void* data);

/**
 * @brief   Allocate HW Sniffer's Command buffer of the desired size and enable the logging. 
 *
 * @param   size                The desired size of HW Sniffer's Command buffer in bytes. 
 * @param   snifferElementCfg   The sniffer element configurations. 0=1st element; 1=2nd ... etc.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableHwSnifferCmdLogging(const VIDRV_ipa_HwSniffInit *snifferElementCfg);

/**
 * @brief   Disable command logging in HW Sniffer.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisableHwSnifferCmdLogging(void);

/**
 * @brief   Return pointer to HW Command Sniffer memory buffer. 
 *
 * @param [out]  pSizeInBytes    If non-null, return size of the buffer in bytes. 
 *
 * @return  NULL if it fails, else pointer to beginning of HW Status Sniffer buffer. 
 *
 */
const void* VIDRV_ipa_GetHwSnifferCmdBuffer(uint32 *pSizeInBytes);

/**
 * @brief   Return pointer to HW Command Sniffer write location. Basically it means, that valid data is behind this point.
 *
 * @return  Write pointer. NULL if HW Sniffer is not supported.
 */
const VIDRV_ipa_HwSniffCommand* VIDRV_ipa_GetHwSnifferCmdWrPointer(void);

/**
 * @brief   Get internal pointers into the HW Sniffer's Command buffer.
 *
 * @param [out]  pReadIntPtr    Internal read pointer.
 * @param [out]  pWriteIntPtr   Internal write pointer.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 *
 */
VIDRV_ipa_ResultType VIDRV_ipa_GetHwSnifferCmdInternalPointers(const VIDRV_ipa_HwSniffCommand **pReadIntPtr, const VIDRV_ipa_HwSniffCommand **pWriteIntPtr);

/**
 * @brief   HW Sniffer commands iterator. 
 * @note    This function is automatically invoked by VVDRV_ipa_Enable(TRUE) so there is no need to call it again within test, unless 
 *
 * @param   rewind   TRUE to drop all contents of the buffer and set internal pointer to the current WR pointer (or start of memory buffer). 
 *
 * @return  Pointer to current sniffer item. NULL of there is no new item available or HW sniffer is not configured.
 */
const VIDRV_ipa_HwSniffCommand* VIDRV_ipa_GetHwSnifferCmdNextItem(boolean rewind);

/**
 * @brief   Allocate HW Sniffer's Status buffer of the desired size and enable the logging.
 *
 * @param   size    The desired size of HW Sniffer's Status buffer in bytes. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 *
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableHwSnifferStatusLogging(uint32 size);

/**
 * @brief   Disable status logging in HW Sniffer.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisableHwSnifferStatusLogging(void);

/**
 * @brief   Return pointer to HW Status Sniffer memory buffer. 
 *
 * @param [out]  pSizeInBytes    If non-null, return size of the buffer in bytes. 
 *
 * @return  NULL if it fails, else pointer to beginning of HW Status Sniffer buffer. 
 *
 */
const void* VIDRV_ipa_GetHwSnifferStatusBuffer(uint32 *pSizeInBytes);

/**
 * @brief   Return pointer to HW Status Sniffer write location. Basically it means, that valid data is behind this point.
 *
 * @return  Write pointer. NULL if HW Sniffer is not supported.
 */
const void* VIDRV_ipa_GetHwSnifferStatusWrPointer(void);

/**
 * @brief   Get internal pointers into the HW Sniffer's Status buffer.
 *
 * @param [out]  pReadIntPtr    Internal read pointer.
 * @param [out]  pWriteIntPtr   Internal write pointer.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
 */
VIDRV_ipa_ResultType VIDRV_ipa_GetHwSnifferStatusInternalWrPointers(const void **pReadIntPtr, const void **pWriteIntPtr);

/**
 * @brief   Invoke IP_PACKET_INIT immediate command. 
 *
 * @param [in] commonParams Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] cmdParams  IP packet init command parameters. See VIDRV_ipa_IpPacketInitParamsType. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_IpPacketInit(const VIDRV_ipa_ImmediateCommandCommonParamsType* commonParams,
                                            const VIDRV_ipa_IpPacketInitParamsType* cmdParams);

/**
 * @brief   Invoke IP_PACKET_INIT_EX immediate command in order to set the stages in IPA pipe that the consecutive packet shall skip. 
 *
 * @param [in] commonParams Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] specificParams Commands parameters. See VIDRV_ipa_IpPacketInitExParamsType.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_IpPacketInitEx(
   const VIDRV_ipa_ImmediateCommandCommonParamsType* commonParams, 
   const VIDRV_ipa_IpPacketInitExParamsType* specificParams
);

/**
 * @brief   Invoke REGISTER_WRITE immediate command. 
 *
 * @param [in] commonParams   Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] immCmdParams   Parameters of REGISTER_WRITE Immediate Command. See VIDRV_ipa_ImmCmdRegisterWriteType. 

 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_ImmCmdRegisterWrite(
   const VIDRV_ipa_ImmediateCommandCommonParamsType   * commonParams,
   const VIDRV_ipa_RegisterWriteParamsType            * immCmdParams
);

/**
 * @brief   Invoke REGISTER_WRITE immediate command via uC command queue. 
 *
 * @param [in] commonParams   Immediate command common parameters.
 * @param [in] immCmdParams   Parameters of REGISTER_WRITE Immediate Command. See VIDRV_ipa_ImmCmdRegisterWriteType. 

 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_ImmCmdRegisterWriteViaUcRxCmdq(
   const VIDRV_ipa_ImmediateCommandCommonParamsType   * commonParams,
   const VIDRV_ipa_RegisterWriteParamsType            * immCmdParams
   );

/**
 * @brief   Invoke REGISTER_READ immediate command. 
 *
 * @param [in] commonParams   Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] immCmdParams   Parameters of REGISTER_READ Immediate Command. See VIDRV_ipa_ImmCmdRegisterReadType. 

 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_ImmCmdRegisterRead(
   const VIDRV_ipa_ImmediateCommandCommonParamsType * commonParams,
   const VIDRV_ipa_RegisterReadParamsType           * immCmdParams);

/**
 * @brief   Invoke DMA_SHARED_MEM immediate command. 
 *
 * @param [in] commonParams   Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] immCmdParams   Parameters of DMA_SHARED_MEM Immediate Command. See VIDRV_ipa_ImmCmdDmaSharedMemType. 

 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
*/
VIDRV_ipa_ResultType VIDRV_ipa_ImmCmdDmaSharedMem(
   const VIDRV_ipa_ImmediateCommandCommonParamsType   * commonParams,
   const VIDRV_ipa_DmaSharedMemParamsType             * immCmdParams
);

/**
 * @brief   Invoke IP_PACKET_TAG_STATUS immediate command. 
 *
 * @param [in] commonParams Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] cmdParams  IP packet tag status command parameters. See VIDRV_ipa_IpPacketTagParamsType. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_IpPacketTagStatus(const VIDRV_ipa_ImmediateCommandCommonParamsType* commonParams, 
                                                 const VIDRV_ipa_IpPacketTagStatusParamsType* cmdParams);



/**
 * @brief   Reserve pipes supporting deaggregation from automatic allocation.
 * 
 * @note Reserve pipes supporting deaggregation from automatic allocation.
 * The function is needed for the cases when several pipes are allocated randomly and
 * only after that we allocate a pipe that must support deaggregation.
 * In this case there's a chance that all the deaggregation pipes will be exhausted by the time we need them.
*/
void VIDRV_ipa_ReserveDeaggregationPipesFromGeneralAllocation(void);

/**
 * @brief   Release pipes supporting deaggregation to automatic allocation.
 * 
 * @note Release pipes supporting deaggregation to automatic allocation.
 * Should be called at the end of the test.
*/
void VIDRV_ipa_ReleaseDeaggregationPipesToGeneralAllocation(void);

/**
 * @brief   Allocate a new IPA pipe. If the pipe was already allocated returns the pipe handle.
 *
 * @param   pipeNum     The pipe number. Let BAM_PIPE_NUMBER_AUTO as pipe number to get 1st available 
 * @param   direction   The direction. 
 *
 * @note    This function cannot prevent the allocation of a deaggtegation pipes 
 *          if a user sets the pipe number manually, and does not use the BAM_PIPE_NUMBER_AUTO option.
 *
 * @return  The pipe handle, NULL if pipeNum equals to System pipe num. 
*/
VIDRV_ipa_PipeHandle VIDRV_ipa_PipeAlloc(uint32 pipeNum, BAM_DRV_PipeDir direction);

/**
 * @brief   Allocate a new IPA pipe. If the pipe was already allocated returns the pipe handle.
 *
 * @param   pipeNum             The pipe number. Let BAM_PIPE_NUMBER_AUTO as pipe number to get 1st available 
 * @param   direction           The direction. 
 * @param   pipeConfiguration   The pipe configuration.
 *
 * @note    This function cannot prevent the allocation of a deaggtegation pipes 
 *          if a user sets the pipe number manually, and does not use the BAM_PIPE_NUMBER_AUTO option.
 *
 * @return  The pipe handle, NULL if pipeNum equals to System pipe num. 
*/
VIDRV_ipa_PipeHandle VIDRV_ipa_PipeAllocEx(uint32 pipeNum, BAM_DRV_PipeDir direction, void* pipeConfiguration);

/**
 *
 * @brief   Allocate a new IPA pipe supporting deaggregation configuration.
 * 
 * @note Allocates a new consumer pipe supporting deaggregation or returns the pipe handle if asked for a specific pipe and it was already allocated.
 *
 * @param   pipeNum     The pipe number. Let BAM_PIPE_NUMBER_AUTO as pipe number to get 1st available 
 *
 * @return  The pipe handle, NULL if pipeNum equals to System pipe number, isn't a pipe number that supports deaggregation. 
*/
VIDRV_ipa_PipeHandle VIDRV_ipa_DeaggregationPipeAlloc(uint32 pipeNum);

/**
 *
 * @brief   Initialize pipe number randomization.
 * 
 * @note Prepares the internal data structures for the subsequent pipe number randomization.
*/
void VIDRV_ipa_InitializePipeNumberRandomization(void);

/**
 *
 * @brief   Randomize an unallocated pipe number.
 * 
 * @note Randomly chooses an unallocated pipe matching the requested selection options.
 * VIDRV_ipa_InitializePipeNumberRandomization function must be called before one or several related calls to VIDRV_ipa_RandomizePipeNumber.
 *
 * @param   pipeSelectionOption     Pipe selection options that may limit randomization to a particular pipe group.
 *
 * @return  Randomized pipe number if there is at least one applicable pipe, or IPA_MAX_PIPE_NUM as an error indication.
*/
uint32 VIDRV_ipa_RandomizePipeNumber(VIDRV_ipa_PipeSelectionOption pipeSelectionOption);

/**
 *
 * @brief   Release the IPA pipe and the corresponding descriptors and transfers. 
 *
 * @param   pipeHandle  Handle of the pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PipeFree(VIDRV_ipa_PipeHandle pipeHandle);

/**
 *
 * @brief   Allocate an endpoint without configuring its BAM pipe.
 * 
 * @note If the pipe was already allocated returns the pipe handle.
 *
 * @param   pipeNum     The pipe number. Let BAM_PIPE_NUMBER_AUTO as pipe number to get 1st available 
 * @param   direction   The direction. 
 *
 * @return  The pipe handle, NULL if pipeNum equals to System pipe num. 
*/
VIDRV_ipa_PipeHandle VIDRV_ipa_PipeAllocEndpointOnly(uint32 pipeNum, BAM_DRV_PipeDir direction);

/**
 * @brief   Get the pipe handle to the system pipe and allocate it if it wasn't allocated. 
 *
 * @return  The matching pipe handle or NULL if error occurred. 
*/
VIDRV_ipa_PipeHandle VIDRV_ipa_GetSystemPipeHandle(void);

/**
 * @brief   Get the interface pipe handle. 
 *
 * @param   pipeHandle  Handle of the IPA pipe. 
 *
 * @return  The handle to the interface pipe, or NULL in case pipeHandle is NULL. 
*/
void* VIDRV_ipa_GetBamPipeHandle(VIDRV_ipa_PipeHandle pipeHandle);

/**
 * @brief   Wait till the end of the current transaction in the pipe.
 *
 * @note Uses BAM driver's function to wait till transaction's end.
 * The function isn't suitable when there's a need to wait for the completion of several transactions.
 *
 * @param   hPipe The pipe. 
 * @param   timeoutMsec timeout in millisecond
 *
 * @return  TRUE if it succeeds, FALSE if it fails. 
 */
#define VIDRV_ipa_WaitUntilTransactionEnd(hPipe, timeoutMsec) VIDRV_pipe_interface_WaitUntilTransactionEnd(hPipe, timeoutMsec)

/**
 * @brief   Check if IPA pipe is empty of pending transactions.
 *
 * @note Calls the corresponding BAM driver's function.
 *
 * @param   hPipe The pipe. 
 *
 * @return  TRUE if it empty, FALSE otherwise. 
 */
/* TODO: When the unified pipe interface is fully finished remove this definition and switch to the new function everywhere in the code. */
extern boolean (*VIDRV_pipe_interface_PipeIsEmpty)(VIDRV_ipa_PipeHandle hPipe);

#define VIDRV_ipa_PipeIsEmpty(hPipe) VIDRV_pipe_interface_PipeIsEmpty(hPipe)

/**
 * @brief   Issue a transfer on BAM2SYSTEM pipe. 
 *
 * @param   hPipe                The pipe. 
 * @param [in,out]   bufferArray If non-null, array of buffers. 
 * @param   bufferArraySize      Size of the buffer array. 
 * @param   flags                The BAM descriptor flags.
 *
 * @note User shall not specify PIPE_DESC_FLAG_EOT for producer pipe.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_Transfer(VIDRV_ipa_PipeHandle hPipe, const VIDRV_ipa_BufferType* bufferArray, 
                                        uint32 bufferArraySize, BAM_DRV_PipeDescFlags flags);

/**
 * @brief   Queue a transfer on BAM2SYSTEM pipe without starting it.
 *
 * @param   hPipe                The pipe. 
 * @param [in,out]   bufferArray If non-null, array of buffers. 
 * @param   bufferArraySize      Size of the buffer array. 
 * @param   flags                The BAM descriptor flags.
 *
 * @note user shall not specify PIPE_DESC_FLAG_EOT for producer pipe.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_TransferNoStart(VIDRV_ipa_PipeHandle hPipe, const VIDRV_ipa_BufferType* bufferArray, 
                                        uint32 bufferArraySize, BAM_DRV_PipeDescFlags flags);

/**
 * @brief   IPA BAM pipe interrupt enable. 
 *
 * @param   hPipe The pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
/* TODO: When the unified pipe interface is fully finished remove this definition and switch to the new function everywhere in the code. */
extern VIDRV_ipa_ResultType (*VIDRV_pipe_interface_PipeInterruptEnable)(VIDRV_ipa_PipeHandle hPipe);

#define VIDRV_ipa_BamPipeInterruptEnable(hPipe) VIDRV_pipe_interface_PipeInterruptEnable(hPipe)

/* -----------------------------------------------------------------------
**                         IRQ (interrupt) INTERFACE
** ----------------------------------------------------------------------- */
/** @addtogroup IPA_IRQ
 *
 *  The following functions is IPA driver IF for IRQ's SWI
 *
 *  @{
 */

/**
 * @brief   Enable/disable interrupts in the specified EE according to the status vector. 
 *
 * @param   tEe            The EE. 
 * @param   statusVector   The status vector. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetInterruptEnable(VIDRV_ipa_EeType tEe, uint32 statusVector);

/**
 * @brief   Get the interrupt status vector. 
 *
 * @param   tEe   The EE. 
 *
 * @param [out] status  The status vector as was read by ISR.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed 
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetInterruptStatus(VIDRV_ipa_EeType tEe, uint32* status);

/**
* @brief   Gets the interrupt status for the specified execution environment. 
*
* @param   tEe   The execution environment. 
*
* @return  The IRQ status. 
*/
uint32 VIDRV_ipa_GetIrqStatus(VIDRV_ipa_EeType tEe);

/**
 * @brief   Clear the stored interrupt status vector. 
 *
 * @param   tEe   The EE. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_ClearStoredInterruptStatus(VIDRV_ipa_EeType tEe);

/**
 * @brief   Clear the interrupt status vector of the specified EE. 
 *
 * @param   tEe            The EE. 
 * @param   statusVector   The status vector. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_ClearInterruptStatus(VIDRV_ipa_EeType tEe, uint32 statusVector);

/**
 * @brief   Set interrupt coalescing register. 
 *
 * @param   tEe                The EE. 
 * @param   timerThreshold     The timer threshold. 
 * @param   counterThreshold   The counter threshold. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetInterruptCoalescing(VIDRV_ipa_EeType tEe, uint32 timerThreshold, uint32 counterThreshold);

/**
 * @brief   Set interrupt coalescing timer granularity. 
 *
 * @param   granularity        The granularity. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetInterruptCoalescingTimerGranularity(VIDRV_ipa_TimerGranularityType granularity);

/**
 * @brief   Get the interrupt counter. 
 *
 * @return  The number of interrupts occurred since last clear, or -1 on failure. 
*/
int32 VIDRV_ipa_GetInterruptCounter(void);

/**
 * @brief   Clear the interrupt counter. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_ClearInterruptCounter(void);

/**
 * @brief   Clears TX_SUSPEND irq state per pipe
 *
 * @param   tEe   -  The execution environment identifier
 * @param   mask  -  Pipe's mask (1 - Enable, 0 - Disable)
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_ClrTxSuspendIrqStatePerPipe( VIDRV_ipa_EeType tEe, uint32 mask );

/**
 * @brief   Enables/Disables TX_SUSPEND irq per pipe
 *
 * @param   tEe   -  The execution environment identifier
 * @param   mask  -  Pipe's mask (1 - Enable, 0 - Disable)
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetTxSuspendIrqCfgPerPipe( VIDRV_ipa_EeType tEe, uint32 mask );

/**
 * @brief   Clears TX_HOLB_DROP irq state per pipe
 *
 * @param   tEe   -  The execution environment identifier
 * @param   mask  -  Pipe's mask (1 - Enable, 0 - Disable)
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_ClrTxHolbDropIrqStatePerPipe( VIDRV_ipa_EeType tEe, uint32 mask );

/**
 * @brief   Enables/Disables TX_HOLB_DROP irq per pipe
 *
 * @param   tEe   -  The execution environment identifier
 * @param   mask  -  Pipe's mask (1 - Enable, 0 - Disable)
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetTxHolbDropIrqCfgPerPipe( VIDRV_ipa_EeType tEe, uint32 mask );

/**
 * @brief   Gets HOLB_DROP irq info per pipe
 *
 * @param         tEe   - Execution environment identifier
 * @param[out]    info  - Endpoint HOLB_DROP bitmask
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetHeadOfLineBlockDropIrqInfo( VIDRV_ipa_EeType tEe, uint32* info );

/**
 * @brief   Gets SUSPEND irq info per pipe
 *
 * @param         tEe   - Execution environment identifier
 * @param[out]    info  - Endpoint SUSPEND bitmask
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetSuspendIrqInfo( VIDRV_ipa_EeType tEe, uint32* info );

/** @} */ /* Ends the IPA driver IF for IRQ's SWI */


/**
 * @brief   Enable head of line blocking mechanism with a given the timer. 
 *
 * @param   hPipe                The pipe handle. 
 * @param   enable               TRUE to enable, FALSE to disable. 
 * @param   headOfLineBlockTimer The head of line block timer value. Value is in 128 IPA-clk clock-cycles.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetHeadOfLineBlocking(VIDRV_ipa_PipeHandle hPipe, boolean enable, uint32 headOfLineBlockTimer);

/**
 * @brief   Enable downlink checksum offload for the given pipe(s).
 *
 * NULL parameters are ignored.
 *
 * @param   pipeSrc Consumer pipe. 
 * @param   pipeDst Producer pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeCsOffloadDownlink(VIDRV_ipa_PipeHandle pipeSrc, VIDRV_ipa_PipeHandle pipeDst);

/**
 * @brief   Enables uplink checksum offload for the given pipe(s).
 *
 * NULL parameters are ignored.
 *
 * @param   pipeSrc         Consumer pipe. 
 * @param   pipeDst         Producer pipe. 
 * @param   ulCsHdrOffset   Uplink checksum metadata header offset in words (4 byte). 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeCsOffloadUplink(VIDRV_ipa_PipeHandle pipeSrc, VIDRV_ipa_PipeHandle pipeDst, uint32 ulCsHdrOffset);

/**
 * @brief   Disable uplink/downlink checksum offload for specific pipe. 
 *
 * @param   pipe         pipe handle. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisablePipeCsOffload(VIDRV_ipa_PipeHandle pipe);

/**
 * @brief   Halt the pipe using the BAM mechanism. 
 *
 * @param   hPipe The pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
/* TODO: When the unified pipe interface is fully finished remove this definition and switch to the new function everywhere in the code. */
extern VIDRV_ipa_ResultType(*VIDRV_pipe_interface_PipePause)(VIDRV_ipa_PipeHandle hPipe, uint32 numOfAttempts);

#define VIDRV_ipa_PipeHalt(hPipe) VIDRV_pipe_interface_PipePause(hPipe, IPA_NUM_ATTEMPS_TO_PIPE_HALT)

/**
 * @brief   Resume the pipe after the BAM Halt. 
 *
 * @param   hPipe The pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
/* TODO: When the unified pipe interface is fully finished remove this definition and switch to the new function everywhere in the code. */
extern VIDRV_ipa_ResultType (*VIDRV_pipe_interface_PipeResume)(VIDRV_ipa_PipeHandle hPipe);

#define VIDRV_ipa_PipeResume(hPipe) VIDRV_pipe_interface_PipeResume(hPipe)

/**
 * @brief   Delay/resume the pipe - In that case the IPA will not read from the BAM. 
 *
 * @param   hPipe The pipe. 
 * @param   delay TRUE to delay, FALSE to resume. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeDelay(VIDRV_ipa_PipeHandle hPipe, boolean delay);

/**
 * @brief   Delay/resume the pipe from Second EE - In that case the IPA will not read from the BAM. 
 *
 * @param   hPipe The pipe. 
 * @param   delay TRUE to delay, FALSE to resume. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeDelaySecond(VIDRV_ipa_PipeHandle hPipe, boolean delay);

/**
 * @brief   Suspend/resume the pipe - In that case the IPA will not read from the BAM. 
 *
 * @param   hPipe The pipe. 
 * @param   suspend true to suspend. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeSuspend(VIDRV_ipa_PipeHandle hPipe, boolean suspend);

/**
 * @brief      Get the structure that represents the IPA_STATE register
 *
 * @param[out] ipaState pointer to structure that represents the IPA_STATE register
 *
 * @return     VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetState(VIDRV_ipa_StateType * ipaState);

/**
 * @brief      Get IPA accelerators state information.
 *
 * @param[out] ipaStateAcl pointer to structure that represents the IPA_STATE_ACL register
 *
 * @return     VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetStateAcl(VIDRV_ipa_StateAclType * ipaStateAcl);

/**
 * @brief      IPA fetcher state information.
 *
 * @param[out] ipaStateFetcher pointer to structure that represents the IPA_STATE_FETCHER register
 *
 * @return     VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetStateFetcher(VIDRV_ipa_StateFetcherType * ipaStateFetcher);

/** @} */ /* Ends the general interface documentation */

/* -----------------------------------------------------------------------
**                         FILTERING INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup Filtering
 *
 *  The following functions describe the Filtering (Firewall) functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Initialize the filter table and add it to the sorted list (according to the table number).
 *
 * @param [in]  filterTableParams   Pointer to information describing the filter table. 
 *
 * @return  The handle to the table, or null if not successful.  
*/
VIDRV_ipa_FilterTableHandle VIDRV_ipa_FilterTableInit(const VIDRV_ipa_FilterTableParamsType* filterTableParams);

/**
 * @brief   UnInitialize the filter table.
 *
 * @param   hTable  The table handle.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_FilterTableUninit(VIDRV_ipa_FilterTableHandle hTable);

/**
 * @brief   Add a new filter rule into the rules list of the specified table.
 *
 * @param   hTable         The table handle. 
 * @param   pRule          The pointer to Filter rule parameters.
 *                
 * @note    In order for changes to take place you must call VIDRV_ipa_FilterUpdate() after this function. 
 *
 * @return  An handle to the filter rule or NULL if not successful. 
*/
VIDRV_ipa_FilterRuleHandle VIDRV_ipa_FilterAddRule(VIDRV_ipa_FilterTableHandle hTable, const VIDRV_ipa_FilterRuleType * pRule);

/**
 * @brief   Remove a filter rule. 
 *
 * @param   hRule  Handle of the rule. 
 *
 * @note    In order for changes to take place you must call VIDRV_ipa_FilterUpdate() after this function
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_FilterRemoveRule(VIDRV_ipa_FilterRuleHandle hRule);

/**
 * @brief   Updates the filtering structure by sending the proper immediate command. The function
 * makes sure that each filter table is 128B aligned (and padded).
 *
 * This function should be called whenever we want to update the actual structure.
 * It should also be called when we want to un-init all the structure tables because it frees
 * The data structure that was created for the tables.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_FilterTableUpdate(void);

/**
 * @brief   Enable/disable IPA filter block. 
 *
 * @param   enable   TRUE to enable, FALSE to disable.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_FilterSet(boolean enable);

/**
 * @brief  Allows (yes/no) usage of Filter IPv4/IPv6 H-Tables.
 *
 * @param   ipVer      IP version, function do nothing if the version is invalid
 * @param   isEnabled  TRUE - enable, FALSE - disable
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetFilterHashedTableUsage(VIDRV_ipa_IpVersion ipVer, boolean isEnabled ); 

/**
 * @brief  Sets Filter Hashing Tuple "disable" mask.
 *
 * @param   hPipe    The pipe handle. 
 * @param   tuple    the Filter Hashing Tuple - "disable" mask
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetFilterTupleMask(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_HashingTuple tuple ); 

/**
 * @brief   Add new (or update) Filter Table with the rule with single equation.
 *          Each rule's parameter SHOULD be defined by user, exception rule's function alone,
 *          the rule's function will be created in accordance with pRule->hwRule.enRule :
 *           when  pEquation != NULL :  with parameters that are defined via pEquation.
 *                                      see section "Equations HW function Types" in file ipa_filter_routing_utils.h
 *                                      In this case in the user responsibility to release:
 *                                           pRule->hwRule.functions
 *           when  pEquation = NULL  :  with info from pRule only. 
 *                                      In this case in the user responsibility to set:
 *                                           pRule->functionSize    
 *                                           pRule->hwRule.functions
 *
 * @param   pFilterTableParams   Options for controlling the Filter table. 
 * @param   pRule                The pointer to Filter rule parameters.
 * @param   pEquation            The pointer to Equation ( should be in accordance with pRule->hwRule.enRule ).
 *                            
 * @return  A handle to the routing table, or null if not successful.  
*/
VIDRV_ipa_FilterTableHandle VIDRV_ipa_AddFilterTableRuleWithSingleEquation (
   const VIDRV_ipa_FilterTableParamsType  *pFilterTableParams              ,
   VIDRV_ipa_FilterRuleType               *pRule                           , 
   const void                             *pEquation                       );


/**
 * @brief  Sets Filter/Routing Hash Flushing
 *
 * @param   pHashFlushParameters    The pointer to the parameters of hash flushing
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetFiterRoutingHashFlush( VIDRV_ipa_FilterRoutingHashFlushParameters * pHashFlushParameters );


/**
 * @brief   Uninit all allocated filter tables
 *
 * @param   isVerbose     TRUE to print the information about all allocated filter tables
 */
void VIDRV_ipa_FreeAllFilterTables(boolean isVerbose);

/** @} */ /* Ends the filtering interface documentation */

/* -----------------------------------------------------------------------
**                         ROUTING INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup Routing
 *
 *  The following functions describe the Routing functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Initialize a Routing table.
 *
 * @param [in]   routeTableParams  Options for controlling the Routing table. 
 *
 * @return  An handle to the routing table, or null if not successful.  
*/
VIDRV_ipa_RouteTableHandle VIDRV_ipa_RouteTableInit(const VIDRV_ipa_RouteTableParamsType *routeTableParams);

/**
 * @brief   Uninitialize a Routing table. 
 *
 * @param   hTable   The table handle. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_RouteTableUninit(VIDRV_ipa_RouteTableHandle hTable);

/**
* @brief   Add a new Routing rule into the rules list of the specified table. 
 *
 * @param   hTable               The table handle. 
 * @param   pRule                The pointer to Routing rule parameters.
 * @param   pActionDescription   The pointer to the routing action description, 
 *                               if NULL then the values defined by pRule are used.
 *                            
 * @note    In order for changes to take place you must call VIDRV_ipa_RouteUpdate() after this function.  
 *
 * @return  An handle to the Routing rule or NULL if not successful. 
*/

VIDRV_ipa_RouteRuleHandle VIDRV_ipa_RouteAddRule(
   VIDRV_ipa_RouteTableHandle                hTable, 
   VIDRV_ipa_RouteRuleType                   *pRule, 
   VIDRV_ipa_RouteRuleActionDescriptionType  *pActionDescription);

/**
 * @brief   Remove a Routing rule. 
 *
 * @param   ruleHandle  Handle of the rule.
 *                      
 * @note    In order for changes to take place you must call VIDRV_ipa_RouteUpdate() after this function.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_RouteRemoveRule(VIDRV_ipa_RouteRuleHandle ruleHandle);

/**
 * @brief   Updates all the routing tables and routing structure by sending the proper immediate command. 
 *
 * The function makes sure that each routing table is 128B aligned (and padded)
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_RouteTableUpdate(void);

/**
 * @brief   Enable/disable the IPA Routing block and sets the default routing pipe number in case the routing is disabled. 
 *
 * @param   enable         TRUE to enable, FALSE to disable. 
 * @param   hDefaultPipe   The pipe handle, if NULL then the default destination pipe is not set. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_RouteSet(boolean enable, VIDRV_ipa_PipeHandle hDefaultPipe);

/**
 * @brief   Sets the default routing table for the specified pipe. 
 *
 * @param   hPipe    The pipe handle. 
 * @param   hTable   The table handle. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_RoutePipeDefaultTableInit(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_RouteTableHandle hTable);

/**
 * @brief  Allows (yes/no) usage of Routing IPv4/IPv6 H-Tables.
 *
 * @param   ipVer      IP version, function do nothing if the version is invalid
 * @param   isEnabled  TRUE - enable, FALSE - disable
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetRoutingHashedTableUsage(VIDRV_ipa_IpVersion ipVer, boolean isEnabled ); 

/**
 * @brief  Sets Routing Hashing Tuple "disable" mask.
 *
 * @param   hTable   The table handle.  
 * @param   tuple    the Routing Hashing Tuple - "disable" mask
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetRoutingTupleMask(VIDRV_ipa_RouteTableHandle hTable, VIDRV_ipa_HashingTuple tuple ); 

/**
 * @brief   Add new (or update) Routing Table with the rule with single equation.
 *          Each rule's parameter SHOULD be defined by user, exception rule's function alone,
 *          the rule's function will be created in accordance with pRule->hwRule.enRule :
 *           when  pEquation != NULL :  with parameters that are defined via pEquation.
 *                                      see section "Equations HW function Types" in file ipa_filter_routing_utils.h
 *                                      In this case in the user responsibility to release:
 *                                           pRule->hwRule.functions
 *           when  pEquation = NULL  :  with info from pRule only. 
 *                                      In this case in the user responsibility to set:
 *                                           pRule->functionSize    
 *                                           pRule->hwRule.functions
 *
 * @param   pRouteTableParams    Options for controlling the Routing table. 
 * @param   pRule                The pointer to Routing rule parameters.
 * @param   pEquation            The pointer to Equation ( should be in accordance with pRule->hwRule.enRule ).
 *                            
 * @return  A handle to the routing table, or null if not successful.  
*/
VIDRV_ipa_RouteTableHandle VIDRV_ipa_AddRoutingTableRuleWithSingleEquation (
   const VIDRV_ipa_RouteTableParamsType   *pRouteTableParams               ,
   VIDRV_ipa_RouteRuleType                *pRule                           , 
   const void                             *pEquation                       );


/**
 * @brief   Uninit all allocated route tables
 *
 * @param   isVerbose     TRUE to print the information about all allocated route tables
 */
void VIDRV_ipa_FreeAllRouteTables(boolean isVerbose);


/** @} */ /* Ends the routing interface documentation */

/* -----------------------------------------------------------------------
**                         HEADERING INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup Headering
 *
 *  The following functions describe the insertion and removal of headers functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Initialize the header tables. 
 *
 * @param   localTable  TRUE to local table. 
 * @param   systemTable TRUE to system table. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderTableInit(boolean localTable, boolean systemTable);

/**
 * @brief   Uninitialize the header tables. 
 *
 * @param   localTable  TRUE to local table. 
 * @param   systemTable TRUE to system table.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderTableUninit(boolean localTable, boolean systemTable);

/**
 * @brief   Add a new header to the header table. 
 *
 * @param   isLocal     TRUE if it is local, FALSE if it is in system memory. 
 * @param   headerParams  Information describing the header.
 *                
 * @note    In order for changes to take place you must call VIDRV_ipa_HeaderTableUpdate() after this function.  
 *
 * @return  A handler to the new header or NULL if not successful. 
*/
VIDRV_ipa_HeaderHandle VIDRV_ipa_HeaderAdd(boolean isLocal, VIDRV_ipa_HeaderParamsType headerParams);

/**
 * @brief   Remove a header. 
 *
 * @param   headerHandle   Handle of the header. 
 *
 * @note    In order for changes to take place you must call VIDRV_ipa_HeaderTableUpdate() after this function.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderRemove(VIDRV_ipa_HeaderHandle headerHandle);

/**
 * @brief   Update a header table if the corresponding argument is TRUE. 
 *
 * @param   localTable  TRUE to local table. 
 * @param   systemTable TRUE to system table. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderTableUpdate(boolean localTable, boolean systemTable);

/**
 * @brief   Set the default header to be inserted in case the routing block is skipped. 
 *
 * @param   headerHandle   Handle of the header. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 * 
 * @note This function can be called only after VIDRV_ipa_HeaderTableUpdate was called
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderSetDefault(VIDRV_ipa_HeaderHandle headerHandle);

/**
 * @brief   Configuer header insertion on a given pipe.
 *
 * @param   hPipe      The pipe handle. 
 * @param [in] params  Configuration parameters. See VIDRV_ipa_HeaderPipeInitParamsType.
 *
 * @note In case of generic aggregation user is expected to call to VIDRV_ipa_AggrPipeInit in order to configure the complementary aggregation parameters. <br>
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderPipeInit(VIDRV_ipa_PipeHandle hPipe, const VIDRV_ipa_HeaderPipeInitParamsType* params);

/**
 * @brief   Configure EXTENDED header register for a given pipe. 
 *
 * @param     hPipe     The pipe handle. 
 * @param     aggrType  The type of deaggregation/aggregation
 * @param     params    Configuration parameters. See VIDRV_ipa_RegHeaderExtType.
 *
 * @note Predefined parameters are used for known protocols (TLP/QMAP/RNDIS).
 * @note The parameter VIDRV_ipa_RegHeaderExtType.padToAlignment should be defined by user.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_HeaderExtPipeInit(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_GenericAggregationType aggrType, const VIDRV_ipa_RegHeaderExtType * params);


/** @} */ /* Ends the headering interface documentation */

/* -----------------------------------------------------------------------
**                         PPC INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup Packet Processing Context (PPC)
 *
 *  The following functions describe the PPC API of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Set the base address of the system packet processing context.
 *
 * @param   baseAddress    Base address to set.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetSystemPacketProcessingContextBaseAddress(uint64 baseAddress);

/**
 * @brief   Get the base address of the system packet processing context.
 *
 * @return  Currently configured base address, when a value of 0 indicates function failure or unconfigured address.
*/
uint64 VIDRV_ipa_GetSystemPacketProcessingContextBaseAddress(void);

/**
 * @brief   Set the base address of the local packet processing context.
 *
 * @param   baseAddress Base address to set.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetLocalPacketProcessingContextBaseAddress(uint32 baseAddress);

/**
 * @brief   Get the base address of the local packet processing context in IPA shared memory.
 *
 * @return  Currently configured base address, when a value of 0 indicates function failure or unconfigured address.
*/
uint32 VIDRV_ipa_GetLocalPacketProcessingContextBaseAddress(void);

/**
 * @brief   Initialize the PPC context. 
 *
 * @note    The local and the system contexts are initialized to an empty state.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextInit(void);

/**
 * @brief   Destroy the existing PPC context. 
 *
 * @note    The local and the system contexts are destroyed.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextUninit(void);

/**
 * @brief   Update the PPC context in in the memory and configure the IPA to use it. 
 *
 * @note    The local and the system contexts are stored in the memory in the format required by the IPA's HW.
 * @note    No configuration is done for the empty contexts. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextUpdate(void);

/**
 * @brief   Create a new packet processing command sequence.
 *
 * @param   addToLocalContext TRUE if the sequence should be stored in the local context, FALSE if its targeted to the system context.
 *
 * @return  A handle to the newly created sequence on success, NULL on failure.
*/
VIDRV_ipa_PacketProcessingCommandSequenceHandle VIDRV_ipa_PacketProcessingContextNewCommandSequence(boolean addToLocalContext);

/**
 * @brief   Remove a previously created packet processing command sequence.
 *
 * @param   sequenceHandle A handle to the sequence to be removed.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextRemoveCommandSequence(VIDRV_ipa_PacketProcessingCommandSequenceHandle sequenceHandle);

/**
 * @brief   Append Header Addition command to the packet processing command sequence.
 *
 * @param      sequenceHandle A handle to the command sequence.
 * @param [in] headerData     A pointer to the header data to be added.
 * @param      headerSize     A number of bytes in the header being added.
 * @param      address64bit   TRUE if the header address should be stored in 64-bit format in the command, otherwise 32-bit address format is used.
 *
 * @note    The header size must be between 1 and IPA_MAX_HEADER_SIZE - 1.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextAppendHeaderAdditionCommand(VIDRV_ipa_PacketProcessingCommandSequenceHandle sequenceHandle,
                                                                                  const uint8 *headerData,
                                                                                  uint32 headerSize,
                                                                                  boolean address64bit);

/**
 * @brief   Append Processing Command to the packet processing command sequence.
 *
 * @param   sequenceHandle     A handle to the command sequence.
 * @param   commandId          An ID of the command to add.
 * @param   parametersPresent  TRUE iff if the command has parameters.
 * @param   parameters         Command's parameters.
 *
 * @note    "parameters" argument is ignored if "parametersPresent" is FALSE.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failure.
*/
VIDRV_ipa_ResultType VIDRV_ipa_PacketProcessingContextAppendProcessingCommand(VIDRV_ipa_PacketProcessingCommandSequenceHandle sequenceHandle,
                                                                              uint16 commandId,
                                                                              boolean parametersPresent,
                                                                              uint32 parameters);

/** @} */ /* Ends the Packet Processing Context (PPC) interface documentation */


/* -----------------------------------------------------------------------
**                         NAT INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup NATing
 *
 *  The following functions describe the NAT functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Initialize the IPv4 NAT table. 
 *
 * @param   tableIndex   Zero-based index of the table - currently supporting only table number 0. 
 * @param   pTableParams Information describing the table. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.   
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatIPv4TableInit(uint8 tableIndex, VIDRV_ipa_NatIPv4TableParamsType *pTableParams);

/**
 * @brief   Get size of NAT's table entry. 
 *
 * @param   natDmaTablesType   type of DMA table for NAT's tables
 *                             see type VIDRV_ipa_DmaTablesType
 *
 * @return  size of NAT's table entry.   
*/
uint32 VIDRV_ipa_GetSizeOfNatTableEntryInBytes(VIDRV_ipa_DmaTablesType type);

/**
 * @brief   Uninitialize the IPv4 NAT table. 
 *
 * @param   tableIndex  Zero-based index of the table - currently only table zero is valid. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.   
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatIPv4TableUninit(uint8 tableIndex);

/**
 * @brief   Add a new NAT IPv4 entry and a corresponding NAT index entry. 
 *
 * @param   tableIndex The table number. 
 * @param   pNatEntry  The NAT entry information. 
 *
 * @return  An handle to the new Nat Entry object or NULL if not successful. 
*/
VIDRV_ipa_NatIPv4EntryHandle VIDRV_ipa_NatIPv4AddNat(uint8 tableIndex, VIDRV_ipa_NatIPv4SessionType *pNatEntry);

/**
 * @brief   Removes a NAT entry and the corresponding NAT index entry.  
 *
 * @param   entryHandle Handle of the entry.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatIPv4RemoveEntry(VIDRV_ipa_NatIPv4EntryHandle entryHandle);

/**
 * @brief   Configure the NAT type of the specified pipe. 
 *
 * @param   hPipe    The pipe handle. 
 * @param   natType  Type of the NAT. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatPipeSet(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_NatType natType);

/**
 * @brief   Return the base address of the NAT table.
 *
 * If in shared memory, returns the offset from the shared memory base.
 * If in system memory, returns the virtual base address.
 *
 * @param   tableIndex   Zero-based index of the NAT table. 
 * @param   ipv6         TRUE for IPv6 NAT, FALSE for IPv4 NAT. 
 * @param   tableType    Type of table for which to retrieve base address
 *
 * @return  pointer/offset to NAT table; NULL if illegal. 
*/
void* VIDRV_ipa_NatGetTableBase(uint8 tableIndex, boolean ipv6, VIDRV_ipa_DmaTablesType tableType);


/**
 * @brief   Read the timestamp for a given NAT entry. 
 *
 * @param   entryHandle Handle of the entry. 
 *
 * @return  value of the timestamp, 0 if failed. 
 */
uint32 VIDRV_ipa_NatGetTimeStamp(VIDRV_ipa_NatIPv4EntryHandle entryHandle);

/**
 * @brief   Read the FIN bit for a given NAT entry. 
 *
 * @param   entryHandle Handle of the entry.
 * @param [out] finValue The FIN bit value.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatGetFin(VIDRV_ipa_NatIPv4EntryHandle entryHandle, boolean* finValue);

/**
 * @brief   Write the FIN bit for a given NAT entry. 
 *
 * @param   entryHandle Handle of the entry.
 * @param   finValue    The FIN bit value.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatSetFin(VIDRV_ipa_NatIPv4EntryHandle entryHandle, boolean finValue);

/**
 * @brief   Return the index of the rule in the NAT rule table. 
 *
 * @param   entryHandle Handle of the entry. 
 *
 * @return  The index of the rule in the table, -1 if failed. 
*/
int32 VIDRV_ipa_NatGetEntryIndex(VIDRV_ipa_NatIPv4EntryHandle entryHandle);

/**
* @brief   Set the IPv4 NAT config entry.
*
* @param   tableIndex   Zero-based index of the table - currently supporting only table number 0.
* @param   pdnIndex     Index in the NAT config table.
* @param   pConfigEntry Config entry pointer.
*
* @return  VIDRV_IPA_SUCCESS if successful, else failed.   
*/
VIDRV_ipa_ResultType VIDRV_ipa_NatIPv4SetConfigEntry(uint8 tableIndex, 
                                                     uint8 pdnIndex,
                                                     const VIDRV_ipa_NatIpv4ConfigEntryHandle pConfigEntry);

/** @} */ /* Ends the NAT interface documentation */

/* -----------------------------------------------------------------------
**                    IPV6 CONNECTION TRACKING INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup ConnTrack
 *
 *  The following functions describe the IPv6 Conn Track functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Enables/Disables the Conn Track of the specified pipe. 
 *
 * @param   hPipe    The pipe handle. 
 * @param   isEnable TRUE to configure the Conn Track for the pipe, FALSE otherwise
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_Ipv6ConnTrackPipeSet(VIDRV_ipa_PipeHandle hPipe, boolean isEnable);

/**
 * @brief   Initialize the IPv6 Connection Tracking table. 
 *
 * @param   tableIndex   Zero-based index of the table - currently supporting only table number 0. 
 * @param   pTableParams Information describing the table. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.   
*/
VIDRV_ipa_ResultType VIDRV_ipa_Ipv6ConnTrackTableInit(uint8 tableIndex, VIDRV_ipa_Ipv6ConnTrackTableParamsType *pTableParams);

/**
 * @brief   Uninitialize the IPv6 Connection Tracking table. 
 *
 * @param   tableIndex  Zero-based index of the table - currently only table zero is valid. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.   
*/
VIDRV_ipa_ResultType VIDRV_ipa_Ipv6ConnTrackTableUninit(uint8 tableIndex);

/**
 * @brief   Add a new IPv6 Connection Tracking entry.
 *
 * @param   tableIndex  The table number. 
 * @param   pEntry      The Conn Track entry information. 
 *
 * @return  An handle to the new Conn Track Entry object or NULL if not successful. 
*/
VIDRV_ipa_Ipv6ConnTrackEntryHandle VIDRV_ipa_Ipv6ConnTrackAddEntry(uint8 tableIndex, const VIDRV_ipa_Ipv6ConnTrackEntryType *pEntry);

/**
 * @brief   Removes a Conn Track entry.
 *
 * @param   entryHandle Handle of the entry.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_Ipv6ConnTrackRemoveEntry(VIDRV_ipa_Ipv6ConnTrackEntryHandle entryHandle);

/**
 * @brief   Return the base address of the Conn Track table.
 *
 * If in shared memory, returns the offset from the shared memory base.
 * If in system memory, returns the virtual base address.
 *
 * @param   tableIndex   Zero-based index of the Conn Track table. 
 * @param   tableType    Type of table for which to retrieve base address
 *
 * @return  pointer/offset to Conn Track table; NULL if illegal. 
*/
void* VIDRV_ipa_Ipv6ConnTrackGetTableBase(uint8 tableIndex, VIDRV_ipa_DmaTablesType tableType);

/**
 * @brief   Return the index of the rule in the Conn Track rule table. 
 *
 * @param   entryHandle Handle of the entry. 
 *
 * @return  The index of the rule in the table, -1 if failed. 
*/
int32 VIDRV_ipa_Ipv6ConnTrackGetEntryIndex(VIDRV_ipa_Ipv6ConnTrackEntryHandle entryHandle);

/**
 * @brief   Read the timestamp for a given Conn Track entry. 
 *
 * @param   entryHandle Handle of the entry. 
 *
 * @return  value of the timestamp, 0 if failed. 
 */
uint32 VIDRV_ipa_Ipv6ConnTrackGetTimeStamp(VIDRV_ipa_Ipv6ConnTrackEntryHandle entryHandle);

/**
 * @brief   Updates a Conn Track entry using TABLE_DMA IC
 *
 * @param   entryHandle Handle of the entry.
 * @param   pIcBuffer   buffer for IC that holds the data, 2 bytes, to write
 * @param   offset      Offset in which to update the data, in uint16 units from the start of the entry.
 * @param   hPipe       Pipe on which to send the IC.
 * @param   flags       Flags for the IC.
 * @param   skipStart   Flag, skip pipe start.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_Ipv6ConnTrackUpdateEntry(VIDRV_ipa_Ipv6ConnTrackEntryHandle entryHandle, const VIDRV_ipa_BufferType* pIcBuffer, uint8 offset, 
                                                        VIDRV_ipa_PipeHandle hPipe, BAM_DRV_PipeDescFlags flags, boolean skipStart);

/** @} */ /* Ends the Conn Track interface documentation */

/* -----------------------------------------------------------------------
**                         HDLC INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup HDLC
 *
 *  The following functions describe the HDLC functionality of the IPA driver - Currently not supported
 *
 *  @{
 */

/**
 * @brief   Set the HDLC mode and the default destination pipe in case the HDLC is bypasses.
 *
 * @param   hPipe    The pipe handle. 
 * @param   hdlcMode The HDLC mode. 
 * @param   byteThreshold Byte threshold operation for DMA operation. To have this threshold ignored, set it to a large 
 *                        value beyond expected DMA chunk size (maximum value is IPA_MAX_BYTE_THRESHOLD). Valid only for DMA Mode. 
 * @param   dstPipe  Destination pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_HdlcPipeModeSet(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_HdlcModeType hdlcMode, uint32 byteThreshold, VIDRV_ipa_PipeHandle dstPipe);

/** @} */ /* Ends the HDLC interface documentation */

/* -----------------------------------------------------------------------
**               AGGREGATION / DEAGGREGATION INTERFACE
** ----------------------------------------------------------------------- */
/** @addtogroup Aggregation
 *
 *  The following functions describe the aggregation and deaggregation functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Configure the pipe aggregation and deaggregation parameters. 
 *
 * @param  hPipe The pipe handle.  
 * @param [in]  params   Configuration parameters. See VIDRV_ipa_AggrInitParamsType.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_AggrPipeInit(VIDRV_ipa_PipeHandle hPipe, const VIDRV_ipa_AggrInitParamsType* params); 

/**
 * @brief   Wait for the aggregation on the given pipes to be closed.
 *
 * @param  [in]   pipeArr           Array of pipes handles. 
 * @param         numPipes          Number of handles in array. 
 * @param         timeOut           The time out in ms to validate the aggregation closure (on all pipes that are declared in pipeArr).
 *                                  The function will return VIDRV_IPA_FAILURE if the aggregation isn't closed after the timeout.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_WaitForAggrClosure(VIDRV_ipa_PipeHandle* pipeArr, uint32 numPipes, uint32 timeOut);

/**
 * @brief   Wait for the aggregation on the given pipes to be started.
 *
 * @param  [in]   pipeArr           Array of pipes handles. 
 * @param         numPipes          Number of handles in array. 
 * @param         timeOut           The time out in ms to validate the aggregation begining (on all pipes that are declared in pipeArr).
 *                                  The function will return VIDRV_IPA_FAILURE if the aggregation isn't started 
 *                                  on some pipe in pipeArr after the timeout.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_WaitForAggrStart(VIDRV_ipa_PipeHandle* pipeArr, uint32 numPipes, uint32 timeOut);

/**
 * @brief   Force aggregation closure by SW. 
 *
 * @param  [in]   pipeArr           Array of pipes handles. 
 * @param         numPipes          Number of handles in array. 
 * @param         closeAtOnce       TRUE - if possible close all aggregations in single register write). FALSE - close aggregations for each pipe separately 
 * @param         timeOut           The time out in ms to validate the aggregation closure (on all pipes that are declared in pipeArr).
 *                                  The function will return VIDRV_IPA_FAILURE if the aggregation isn't closed after the timeout.
 *                                  Use 0 to prevent the aggregation closure validation. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_AggrForceClose(VIDRV_ipa_PipeHandle* pipeArr, uint32 numPipes, boolean closeAtOnce, uint32 timeOut); 


/**
 * @brief   Set aggregation timer granularity. 
 *
 * @param   granularity        The granularity. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_AggrSetTimerGranularity(VIDRV_ipa_TimerGranularityType granularity);

/**
 * @brief   Set QCNCM mode and the corresponding signature. 
 *
 * @param   enable      TRUE to enable, FALSE to disable. 
 * @param   signature   The signature. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetQcncmMode(boolean enable, uint32 signature); 

/**
 * @brief   Get the attributes of the First Error Captured, in the following cases
 *          -  general  (see VIDRV_ipa_IntSrcOpcodeType )       
 *          -  an MBIM De-Aggregation error. 
 *             (attr->opcode =  IPA_INT_SRC_OPCODE_DEAGGR_ERR_MBIM)
 *          -  an Generic De-Aggregation error. 
 *             (attr->opcode = IPA_INT_SRC_OPCODE_DEAGGR_ERR_GENERIC)
 *
 * @param            eeId   The execution environment identifier.
 * @param[in,out]    attr      The pointer to the FEC Attributes Parsing structure. 
 *                          
 * @note    The attr->opcode should be predefined to use this function. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed, but
 *          when attr->opcode is invalid the structure attr 
 *          will be updated as in general case ( including the attr->opcode as well )
 *          and the function will return VIDRV_IPA_BAD_PARAM. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetFecAttr(const VIDRV_ipa_EeType eeId, VIDRV_ipa_FecAttrParsingType * attr);

/**
 * @brief   Set pipe's maximal packet length value (for De-Aggregation). 
 *
 * @param   hPipe The pipe handle.  
 * @param   maxPacketLen    The maximal packet length
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeDeAggrMaxPacketLen(VIDRV_ipa_PipeHandle hPipe, uint32 maxPacketLen);

/**
 * @brief   Set ignoring (ON/OFF) packets smaller than header (for De-Aggregation pipes). 
 *          For example: this is intended for use in RNDIS De-Aggregation pipes, to silently ignore a redundant 1-byte trailer in MSFT implementation
 *
 * @param   hPipe    The pipe handle.  
 * @param   enable   true to enable, false to disable.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeDeAggrIgnoreMinPktErr(VIDRV_ipa_PipeHandle hPipe, boolean enable);

/** @} */ /* Ends the aggregation interface documentation */

/* -----------------------------------------------------------------------
**               FRAGMENT HANDLING INTERFACE
** ----------------------------------------------------------------------- */
/** @addtogroup Fragment
 *
 *  The following functions describe the IP packet fragment handling functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Configure default pipe for routing fragmented exception packets and new rule statues
 *          (if source pipe does not have a notification status pipe defined).
 *
 * @param   defaultPipe Default pipe to route fragmented exception packets and new rule statues. 
 *
 *
 * @code
 * 
 * IPA_HWIO_OUTF(IPA_ROUTE, FRAG_DEF_PIPE, defaultPipe->pipeId);
 *
 * @endcode
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_FragConfig(VIDRV_ipa_PipeHandle defaultPipe); 

/**
 * @brief   Clear fragment rule table.
 *
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_FragClearTable(void); 

/**
 * @brief   Configure IP packet fragment handling for specific pipe.  
 *
 * @param   hPipeConsumer  The pipe handle. 
 * @param   enable         TRUE to enable, FALSE to disable. 
 *
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_FragSetPipe(VIDRV_ipa_PipeHandle hPipeConsumer, boolean enable); 


/** @} */ /* Ends the Fragment interface documentation */

/* -----------------------------------------------------------------------
**               DECIPHERING INTERFACE
** ----------------------------------------------------------------------- */
/** @addtogroup Deciphering
 *
 *  The following functions describe the LTE deciphering functionality of the IPA driver
 *  
 *  @code
 *  Input buffer deciphering parameters
 *   -----------------------------------------------------------------------------------------
 *  |clear  | This region counted  |      This region is actually                   | clear   |
 *  |text(C)| as deciphered (SWD)  |      deciphered by IPA  (D)                    | tail(T) |
 *   -----------------------------------------------------------------------------------------
 *                                   ^=============================================^
 *                                    size
 *  Examples: 
 *  In all example C section size is 2 bytes, SWD is 3 bytes, D is 50 bytes and T is 4 bytes.
 *  All examples present the values of keystrmOffset, startOffset and size parameters.
 *  In addition example specify the expected buffer size.
 * 1.  ----------------------
 *    | D               | T  |
 *     ----------------------
 *  keystrmOffset=0, startOffset=0, size = 50. Buffer size is 54
 * 2.  ----------------------
 *    | C | D           | T  |
 *     ----------------------
 *  keystrmOffset=0, startOffset=2, size = 50. Buffer size is 56
 * 3.  ----------------------
 *    | SWD | D        | T   |
 *     ----------------------
 *  keystrmOffset=3, startOffset=3, size = 50. Buffer size is 57
 * 4.  ----------------------
 *    | C |SWD | D      | T  |
 *     ----------------------
 *  keystrmOffset=3, startOffset=5, size = 50. Buffer size is 59
 * 5. In this scenario part of the SWD was removed before it was sent to IPA. hence referred as RSWD (with size of 5 bytes)
 *     ----    ----------------------
 *    |RSWD|  | SWD | D         | T  |
 *     ----    ----------------------
 *  keystrmOffset=8, startOffset=3, size = 50. Buffer size is 57
 *  @endcode
 *
 *  @{
 */

/**
 *
 * @brief   Initialize LTE deciphering block. 
 *
 * @param [in] params  Initialization parameters. See VIDRV_ipa_DecipherSetParamsType. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_DecipherSet(const VIDRV_ipa_DecipherSetParamsType* params);

/**
 *
 * @brief   Initialize LTE deciphering block, with using of IC common parameters. 
 *
 * @param [in] commonParams   Immediate command common parameters. 
 *                            Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] cmdParams      Initialization parameters. See VIDRV_ipa_DecipherSetParamsType. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_DecipherSetEx(
   const VIDRV_ipa_ImmediateCommandCommonParamsType   * commonParams ,
   const VIDRV_ipa_DecipherSetParamsType              * cmdParams    );

/**
 * @brief   Load deciphering keys into a predefined location in IPA memory. 
 *
 * @param [in]   keys  Pointer to array of deciphering keys
 * @param        keysCount Number of keys. Value shall not exceed IPA_MAX_CIPHER_KEY_INDEX + 1.   
 *
 * 
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_DecipherLoadKeys(const VIDRV_ipa_DecipherKey* keys, uint32 keysCount);

/**
 * @brief   Configure deciphering parameters for the consecutive packet (by invoking LTE_DECIPH_CONFIG immediate command). 
 *
 * @param [in] commonParams Immediate command common parameters. Specify NULL to send command through system pipe with PIPE_DESC_FLAG_INT flag.
 * @param [in] cmdParams Deciphering parameters. See VIDRV_ipa_DecipherConfigPacketParamsType.   
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_ipa_DecipherConfigPacket(const VIDRV_ipa_ImmediateCommandCommonParamsType* commonParams,
                                                    const VIDRV_ipa_DecipherConfigPacketParamsType* cmdParams);
  
/** @} */ /* Ends the Deciphering interface documentation */

/* -----------------------------------------------------------------------
**               uC INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup uC
 *
 *  The following functions describe the uC interface of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Write to uC mailbox. 
 *
 * @param   block       The block of mailbox( 0 or 1). 
 * @param   mailboxId   Identifier for the mailbox (0 to 31). 
 * @param   value       The value to write. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ucWriteMailbox(uint32 block, uint32 mailboxId, uint32 value);

/**
 * @brief   Read from uC mailbox. 
 *
 * @param   [in]  block       The block of mailbox( 0 or 1). 
 * @param   [in]  mailboxId   Identifier for the mailbox (0 to 31). 
 * @param   [out] value       The value. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ucReadMailbox(uint32 block, uint32 mailboxId, uint32* value);

/**
 * @brief  Trigger interrupt to uC. 
 *
 * @param   tEe   The EE. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ucTriggerInterrupt(VIDRV_ipa_EeType tEe);

/**
 * @brief   Power up/down uC.
 *
 * @param   enable       TRUE to power up, FALSE to power down. 
 * @param   enableDsMode TRUE - enable deep-sleep mode, FALSE disable deep-sleep mode
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ucSet(boolean enable, boolean enableDsMode);

/**
 * @brief   Load uC FW.
 *
 * @param   image [in]  uC image to load. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ucLoad(const uint32 *image);

/**
 * @brief   Allocate buffer which meet QMB constraints (QMB transaction must not cross 1KB boundary in addresses). 
 *
 * @param [in,out]   buffer   buffer information. Caller shall initialize the size field 
 * @param   align             The alignment requirement. 
 * @param   flags             The flags. The flags (like M_ZERO, and M_PAGE_ALIGN).  
 *
 * @return  0 if successful or non-zero value on failure. 
*/
int32 VIDRV_ipa_ucAllocBufferForQmb(VIDRV_ipa_BufferType* buffer, uint16 align, uint16 flags);

 /** @} */ /* Ends the uC interface documentation */

/* -----------------------------------------------------------------------
**                         DEBUG INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup Debug
 *
 *  The following functions describe the debug capabilities of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Configure IPA to work in step mode and assert interrupt upon completion of specified steps in the IPA pipe. 
 *
 * @param   steps  step types for which step mode would be enabled. User may specify value as mask.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ConfigStepMode(VIDRV_ipa_StepModePipeStepType steps);

/**
 * @brief   Gets step mode configration
 *
 * @return  The current step mode configuration
*/
VIDRV_ipa_StepModePipeStepType VIDRV_ipa_GetStepModeConfig(void);

/**
 * @brief   Go to the next step while in step mode. 
 *
 * @return  The current pending step. 
 *
 * @note    In case STEP_MODE_IRQ is enabled no need to call to this service.
*/
VIDRV_ipa_StepModePipeStepType VIDRV_ipa_GoNextDebugStep(void);


/**
 * @brief   Set debug counter properties. 
 *
 * @param   counterIndex   Zero-based index of the counter. 
 * @param   enable         TRUE to enable, FALSE to disable. 
 * @param   ruleIndex      One-based index of the rule (in filter rule, asserting bit 8 specifies
 *                         a pipe rule). 
 * @param   srcPipe        Source pipe or NULL for all pipes. 
 * @param   product        False for counting bytes of incoming packets, otherwise all packets.
 * @param   type           The type of counting. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetDebugCounter(uint8 counterIndex, boolean enable, uint32 ruleIndex,
                                               VIDRV_ipa_PipeHandle srcPipe, boolean product, VIDRV_ipa_DebugCounterType type);

/**
 * @brief   IPA CMDQs (Rx_HPS/DPS_Tx) debug initialization and will stop the CMDQs from executing commands.
 *
 * @note    Current implementation supports only "default" Rx_HPS CMDQ.
 * @note    The function must be called before the queuing of the transfers.
 *
 * @param   rxHpsCmdqId       Rx_HPS CMDQ identifier
 * @param   dpsTxCmdqId       DPS_Tx CMDQ identifier
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_CmdqSetBlockEnable(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, boolean bEnBlockWrite, boolean bEnBlockRead);

/**
 * @brief   Read from one or more items CMDQ FIFO. 
 *
 * @param   cmdqType          Command queue type
 * @param   hPipe             Pipe handle
 * @param [out]  outInfo      Pointer to buffer that will receive CMDQ data. In case of NULL
 *                            the commands would be popped from fifo without storing them in buffer and counting them as fetched items.
 * @param   numOfItems        Number of items available in the outInfo buffer. 
 *
 * @return  Number of fetched items, 0 if failed. 
 */
uint32 VIDRV_ipa_PopCmdqFifoItem(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_CmdQueCommand* outInfo, uint32 numOfItems);

/**
 * @brief   Read Debug command from Rx_HPS Fifo. 
 *
 * @param         cmdqId         CMDQ identifier. 
 * @param [out]   pSrcAddr       If non-null, source address. 
 * @param [out]   pSrcLen        If non-null, length of the source. 
 * @param [out]   pMisc          If non-null, misc field. ( Where MISC[7:0] = { FLAGS[5:0], ORDER[1:0] }  )
 * @param [out]   pSrcPipeId     If non-null, identifier for the source pipe. 
 * @param [out]   pMbimMetadata  If non-null, the mbim metadata. 
 * @param [out]   pPacketLen     If non-null, total length of the packet. 
 * @param [out]   pOpcode        If non-null, the opcode. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_RxHpsReadDbgCmdData(
   VIDRV_ipa_RxHpsCmdqIds     cmdqId         ,
   uint64                     *pSrcAddr      ,
   uint32                     *pSrcLen       ,
   uint8                      *pMisc         ,
   uint32                     *pSrcPipeId    ,
   uint32                     *pMbimMetadata ,
   uint32                     *pPacketLen    ,
   uint32                     *pOpcode       );

/**
 * @brief   Executes the RX_SPLT debug bus command.
 *          A new command can be executed only after the status is not active.
 *
 * @param   cmdqId         CMDQ identifier. 
 * @param   srcAddr        Source address. 
 * @param   srcLen         Length of the source. 
 * @param   misc           The misc field. ( Where MISC[7:0] = { FLAGS[5:0], ORDER[1:0] }  ) 
 * @param   srcPipeId      Identifier for the pipe. 
 * @param   mbimMetadata   MBIM de-aggregation metadata. 
 * @param   packetLen      Packet length. 
 * @param   opcode         The command opcode field. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_RxSpltDebugCmdExecute(
   VIDRV_ipa_RxHpsCmdqIds  cmdqId         ,
   uint64                  srcAddr        , 
   uint32                  srcLen         , 
   uint8                   misc           , 
   uint32                  srcPipeId      , 
   uint32                  mbimMetadata   , 
   uint32                  packetLen      , 
   uint32                  opcode         );

/**
 * @brief   Sets debug command configuration register. 
 * @note    Both bEnBlockWrite and bEnBlockRead are used to configure, and no option to configure one of them.
 *
 * @param   cmdqType       Command queue type
 * @param   hPipe          Pipe handle
 * @param   bEnBlockWrite  TRUE for blocking write, FALSE for unblocking write.
 * @param   bEnBlockRead   TRUE for blocking read, FALSE for unblocking read.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetDbgCmdCfg(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, boolean bEnBlockWrite, boolean bEnBlockRead);

/**
 * @brief   Gets the status of the Rx_HPS/DPS_Tx debug command. 
 *          Only after the command is not active you can send another command.
 *
 * @param   cmdqType          Command queue type
 * @param   hPipe             Pipe handle
 * @param   pStatus [out]     If non-NULL, Fifo status
 * @param   pNumCmd [out]     If non-NULL, Number of pending commands in Fifo
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetDbgCmdStatus(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_DebugCmdStatusType *pStatus, uint32 *pNumCmd);

/**
 * @brief   Gets the Status.Empty of the Rx_HPS/DPS_Tx debug command.
 *
 * @param   cmdqType          CMDQ type
 * @param   hPipe             Pipe handle
 * @param   pCmdqEmpty [out]  the field "Empty" of the Status
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetDbgCmdStatusQueueIsEmpty(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, boolean * pCmdqEmpty);

/**
 * @brief   Gets the Status.Full of the Rx_HPS/DPS_Tx debug command.
 *
 * @param   hPipe             Pipe handle
 * @param   isRx              TRUE = RX Fifo, FALSE = TX Fifo. 
 * @param   pCmdqFull [out]   the field "Full" of the Status
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetDbgCmdStatusQueueIsFull(VIDRV_ipa_PipeHandle hPipe, boolean isRx, boolean  *pCmdqFull);

/**
 * @brief   Sets debug command register. 
 *
 * @param   cmdqType       Type of the cmdq. 
 * @param   hPipe             Pipe handle
 * @param   pop            TRUE to pop (remove top) . 
 * @param   releaseRd      TRUE to release one command (blocked by read client) from Fifo . 
 * @param   releaseWr      TRUE to release one command (blocked by write client) from Fifo . 
 * @param   releasePktRd   TRUE to release one packet (blocked by read client) from Fifo . 
 * @param   releasePktWr   TRUE to release one packet (blocked by write client) from Fifo . 
 * @param   signal         TRUE to signal (this parameter is deprecated since IPA 2.0). 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 *
 * ### param   isRx  TRUE for RX Fifo, FALSE for TX Fifo. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetDbgCmd(VIDRV_ipa_CmdqType cmdqType, VIDRV_ipa_PipeHandle hPipe, boolean pop, boolean releaseRd, boolean releaseWr, boolean releasePktRd, boolean releasePktWr, boolean signal);

/**
 * @brief   Configure IPA test bus and debug bus and then read the debug data. 
 *
 * @param [in] testBusParams if not NULL test bus configuration parameters.
 * @param enableTestBus indication whether to enable IPA test bus
 * @param [in] debugDataParams if not NULL debug bus configuration parameters.
 * @param [out] debugData The value read from IPA debug bus (debug data)
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_DebugBusRead(const VIDRV_ipa_DebugBusConfigType* testBusParams,
                                            boolean enableTestBus,
                                            const VIDRV_ipa_DebugBusConfigType* debugDataParams,
                                            uint32* debugData);

/**
 * @brief   Print IPA versions in INFO level.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_DebugPrintVersions(void);


/**
 * @brief   Configure HW events monitoring occur during IPA operation. 
 *
 * @param   select   Selects source of HW events to be monitored. 
 * @param   pipe     Selected pipe to monitor its Rx-related HW events. When configured to NULL all pipes are monitored.
 *                   Valid only when select=HW_EVENT_SELECT_IPA or HW_EVENT_SELECT_MIXED.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_DebugSetQdss(VIDRV_ipa_HwEventSelectType select, VIDRV_ipa_PipeHandle pipe);

/**
 * @brief   Sets Rd/Wr Block configuration of HPS-DPS CMDQ for a destination pipe
 *
 * @param   hPipe       IPA pipe handle. Note, it must be a Producer pipe. 
 * @param   rdBlockEn   TRUE/FALSE to block CMDQ read operation
 * @param   wrBlockEn   TRUE/FALSE to block CMDQ write operation
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetHpsDpsCmdqRdWrBlockConfiguration(VIDRV_ipa_PipeHandle hPipe, boolean rdBlockEn, boolean wrBlockEn ); 

/**
 * @brief   Clears Rd/Wr Block configuration of HPS-DPS CMDQ 
*/
void VIDRV_ipa_ClearHpsDpsCmdqRdWrBlockConfiguration( void );

/**
 * @brief   Releases one read command of HPS-DPS CMDQ for a destination pipe
 *          Applicable only to HW client, if BLOCK_WRITE is set.
 *
 * @param   hPipe       IPA pipe handle. Note, it must be a Producer pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ReleaseHpsDpsCmdqRdCommand(VIDRV_ipa_PipeHandle hPipe); 

/**
 * @brief   Releases one write command of HPS-DPS CMDQ for a destination pipe
 *          Applicable only to HW client, if BLOCK_WRITE is set.
 *
 * @param   hPipe       IPA pipe handle. Note, it must be a Producer pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ReleaseHpsDpsCmdqWrCommand(VIDRV_ipa_PipeHandle hPipe); 

/**
 * @brief   Checks if HPS-DPS CMDQ of a destination pipe is empty
 *
 * @param   hPipe       IPA pipe handle. Note, it must be a Producer pipe. 
 *
 * @return  TRUE - if empty, otherwise  - FALSE
*/
boolean VIDRV_ipa_IsHpsDpsCmdqEmpty( VIDRV_ipa_PipeHandle hPipe ); 

/**
 * @brief   Reads HPS-DPS CMDQ COUNT and DEPTH  for a destination pipe
 *
 * @param         hPipe       IPA pipe handle. Note, it must be a Producer pipe. 
 * @param   [out] pCount      if not NULL: Pointer to variable for CMDQ (FIFO) COUNT  
 * @param   [out] pDepth      if not NULL: Pointer to variable for CMDQ (FIFO) DEPTH
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetHpsDpsCmdqCountInfo(VIDRV_ipa_PipeHandle hPipe, uint32 *pCount, uint32* pDepth ); 


/** @} */ /* Ends the debug interface documentation */


/* -----------------------------------------------------------------------
**                         STATUS QUEUE INTERFACE
** ----------------------------------------------------------------------- */

/** @addtogroup STATUS_QUEUE
 *
 *  The following functions describe the Status Queue functionality of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Allocate IPA pipe as Status Queue pipe, memory buffers and other resources if needed. 
 *
 * @param   pipeNumForQueue     Pipe number dedicated for the status queue. 
 * @param   aggregTimeOutMs     Aggregation time out in milliseconds. Byte aggregation is always by IPA_SQ_BUF_SIZE. If set to
 *                              0, aggregation will not be used at all.
 * @param   enableSizeAggreg    TRUE to enable size aggregation. Relevant only in case aggregTimeOut is zero. 
 * @param   statMsgAlignBits    Size alignment of status message. Must be either 0 (no alignment) or 4 (32-bytes alignment). Other values are not intended usecase.
 *
 * @note    The Generic Aggregation is enabled OLNY if one of the following parameters has non-zero(or TRUE) value:
 *             aggregTimeOutMs
 *             enableSizeAggreg
 *
 * @return  Status Queue handle or NULL if function failed. 
 *
 */
VIDRV_ipa_StatusQueueHandle VIDRV_ipa_StatusQueueAlloc(uint32 pipeNumForQueue, uint32 aggregTimeOutMs, boolean enableSizeAggreg, uint32 statMsgAlignBits);

/**
 * @brief   Releases all internal resources and associated IPA pipe. 
 *
 * @param   hStatusQueue    Handle of Status Queue obtained from successful call to VIDRV_ipa_initStatusQueue. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_StatusQueueRelease(VIDRV_ipa_StatusQueueHandle hStatusQueue);

/**
 * @brief   Attach IPA pipe to Status Queue so that all statuses are redirected to the queue. 
 *
 * @param   hPipe           IPA pipe handle. Note, it must be Consumer pipe, because Producer pipe doesn't generate statuses. 
 * @param   hStatusQueue    Handle of Status Queue obtained from successful call to VIDRV_ipa_initStatusQueue. 
 *
 * @return  VIDRV_IPA_SUCCESS if call succeeded, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_StatusQueueAttachPipe(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_StatusQueueHandle hStatusQueue);


/**
 * @brief   Detach IPA pipe from Status Queue. Status generation will be disabled on the pipe.
 *
 * @param   hPipe           IPA pipe handle, that was previously attached to Status Queue. 
 * @param   hStatusQueue    Handle of Status Queue obtained from successful call to VIDRV_ipa_initStatusQueue. 
 *
 * @return  VIDRV_IPA_SUCCESS if call succeeded, else failed. Call will fail also if pipe was not attached to the specified status queue.
 */
VIDRV_ipa_ResultType VIDRV_ipa_StatusQueueDetachPipe(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_StatusQueueHandle hStatusQueue);

/**
 * @brief   Allocate descriptors for the status queue. 
 *
 * @param   hStatusQueue   Queue of status. 
 * @param   expectedCount  The minimum number of descriptors the caller is expecting to have in the fifo after this call. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_StatusQueueAllocDesc(VIDRV_ipa_StatusQueueHandle hStatusQueue, uint32 expectedCount);

/**
 * @brief   Wait for status element, that being masked by given filter has zero/non-zero fields (depending on bWaitZero parameter). 
 *
 * @param   hStatusQueue    Handle of Status Queue. 
 * @param   bWaitZero       TRUE to look for packet where masked fields are zeros. Otherwise look for packet where masked fields are not zero.
 *                          Note, opCode should always match exactly to corresponding field in filter.
 * @param   filter          The filter. Each non-zero field will be used as criteria mask. 
 * @param   lastRes         Previous result to look for next matching packet. Put NULL to search from start. 
 * @param   timeOut         The time out in ms for waiting. Let 0 for not waiting. 
 *
 * @return  NULL if wait has timed out, pointer to status element. 
 */
const VIDRV_ipa_StatusQueuePacketType* VIDRV_ipa_StatusQueueWait(VIDRV_ipa_StatusQueueHandle hStatusQueue,
                                                           boolean bWaitZero,
                                                           const VIDRV_ipa_StatusQueuePacketType * filter,
                                                           const VIDRV_ipa_StatusQueuePacketType * lastRes,
                                                           uint32 timeOut);

/**
 * @brief   Fills the inOrderList by Status Element Pointers that are relevant for pair {hConsumer, hProducer}. 
 *
 * @param   hStatusQueue    Handle of Status Queue. 
 * @param   lastRes         Previous result to look for next Statuses. Put NULL to search from start. 
 * @param   hConsumer       The consumer, if NULL then it means "any consumer". 
 * @param   hProducer       The producer, if NULL then it means "any producer". 
 * @param   inOrderList     The pointer to IN-ORDER List which the function should to fill. 
 *                          NULL-pointer is used to mark irrelevant element in this array.
 * @param   inOrderListSize The size of the array inOrderList. 
 * @param   timeOut         The time out in ms to create the list. Let 0 for not waiting. 
 *
 * @return  pointer to the last status element that was processed by this function. 
 *          Thus, the function returns NULL-pointer if no status element was detected.        
 */
const VIDRV_ipa_StatusQueuePacketType * VIDRV_ipa_StatusQueueCreateInOrderList(
   VIDRV_ipa_StatusQueueHandle               hStatusQueue      ,
   const VIDRV_ipa_StatusQueuePacketType     *lastRes          ,
   VIDRV_ipa_PipeHandle                      hConsumer         , 
   VIDRV_ipa_PipeHandle                      hProducer         ,
   const VIDRV_ipa_StatusQueuePacketType     *inOrderList[]    ,
   uint32                                    inOrderListSize   ,
   uint32                                    timeOut           );

/**
 * @brief   Close the aggregation of Status Queue pipe. 
 *
 * @param   hStatusQueue   Queue of status. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_StatusQueueAggrForceClose(VIDRV_ipa_StatusQueueHandle hStatusQueue);

/** @} */ /* Ends the status queue interface documentation */

/**
 * @brief   Enable status packet generation of consumer pipe (if not NULL) and direct them to specified producer pipe. 
 *
 * @param   hConsumer   The consumer. 
 * @param   hProducer   The producer. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, otherwise failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_StatusPathEnable(VIDRV_ipa_PipeHandle hConsumer, VIDRV_ipa_PipeHandle hProducer);

/**
 * @brief   Disable status packets generation on valid consumer and producer. 
 *
 * @param   hConsumer   The consumer, if not NULL. 
 * @param   hProducer   The producer, if not NULL. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, otherwise failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_StatusPathDisable(VIDRV_ipa_PipeHandle hConsumer, VIDRV_ipa_PipeHandle hProducer);

/**
 * @brief   Set status location to be before or after the packet (valid when status is directed to the same pipe as packet).
 *
 * @param   hProducer            The producer pipe to which the packet and status are directed. 
 * @param   isStatusAfterPacket  If TRUE, status will arrive after the packet, otherwise status will arrive before the packet. 
 *
 * @note    At the moment the option of getting status after packet is only for decompression case.
 * @return  VIDRV_IPA_SUCCESS if successful, otherwise failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetStatusLocation(VIDRV_ipa_PipeHandle hProducer, boolean isStatusAfterPacket);

/**
 * @brief   Set TX Threshold byte level for a specific marker for a specific pipe.
 *
 * @param   pipeNum        Pipe number.
 * @param   markerColor    The marker color - RED or YELLOW. 
 * @param   level          Marker level in bytes. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_TxThresholdIntSetLevel(uint32 pipeNum, VIDRV_ipa_TxThresholdType markerColor, uint16 level);

/**
 * @brief   Enable/disable interrupt triggering for specific pipes when TX level above or below corresponding marker threshold. 
 *
 * @param   markerColor       The marker color. 
 * @param   pipes             Array of pipes whose ABOVE threshold should be enabled or disable 
 * @param   pipeCount         Number of pipes. 
 * @param   enableWhenAbove   TRUE to enable event when BAM credit is above marker; FALSE to disable.
 * @param   enableWhenBelow   TRUE to enable event when BAM credit is below marker; FALSE to disable. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_TxThresholdIntEnable(VIDRV_ipa_TxThresholdType markerColor, const VIDRV_ipa_PipeHandle pipes[], uint32 pipeCount, boolean enableWhenAbove, boolean enableWhenBelow);

/**
 * @brief   Clear ABOVE/BELOW event status for specified pipes. Note, if pipe credit still below or above marker, its corresponding status will remain triggered.
 *
 * @param   markerColor The marker color. 
 * @param   pipes       Array of pipes whose ABOVE threshold should be enabled or disable. 
 * @param   pipeCount   Number of pipes. 
 * @param   clearAbove  TRUE to clear ABOVE event status. FALSE to skip clearing
 * @param   clearBelow  TRUE to clear BELOW event status. FALSE to skip clearing
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 *
 */
VIDRV_ipa_ResultType VIDRV_ipa_TxThresholdIntClearPipesStatus(VIDRV_ipa_TxThresholdType markerColor, const VIDRV_ipa_PipeHandle pipes[], uint32 pipeCount, boolean clearAbove, boolean clearBelow);

/**
 * @brief   Returns bitmask representing pipes that have their TX level below specified marker. 
 *
 * @param   markerColor The marker color. 
 * @param   getAbove    TRUE to get ABOVE status, FALSE to get BELOW status. 
 * @param [out] pipesState bit-mask value.         
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_TxThresholdIntGetPipesState(VIDRV_ipa_TxThresholdType markerColor, boolean getAbove, uint32* pipesState);

/**
 * @brief   Sets SW bypass clock gating for the IPA core. 
 *
 * @param   pSwBpCGC    The pointer to structure with configuration of the SW bypass clock-gating. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetSwBypassClkGating(VIDRV_ipa_SwBypassClkGatingConfigType *pSwBpCGC);

/**
 * @brief   Enable/disable Data Path Logging for a given pipe.
 *
 * @param   hPipe    Pipe handle.
 * @param   enable   The desired status of Data Path Logging enabling for the given pipe. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeConsumerDataPathLoggingEnable(VIDRV_ipa_PipeHandle hPipe, boolean enable);

/**
 * @brief   Set Data Path Logging configuration for a give pipe.
 *
 * @param   hPipe      Pipe handle for the producer pipe that will be used for Data Path Logging.
 * @param   maxLength  Length limit for the logged packets.
 * @param   enable     The desired status of Data Path Logging enabling. 
 *
 * @return  VIDRV_IPA_SUCCESS on success. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetDataPathLoggingConfiguration(VIDRV_ipa_PipeHandle hPipe, uint32 maxLength, boolean enable);

/**
 * @brief   Configure metadata extraction.
 *
 * @param   hPipe          Pipe handle for the consumer pipe that with Data Path Logging.
 * @param   fromRegister   TRUE iff the metadata must be taken from a register and not from the packet.
 * @param   metadataValue  Metadata value to put in the register, ignored if fromRegister is FALSE.
 * @param   metadataOffset An offset of the metadata within the packet, ignored if fromRegister is TRUE.
 *
 * @return  VIDRV_IPA_SUCCESS on success. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_ConfigureMetadataExtraction(VIDRV_ipa_PipeHandle hPipe, boolean fromRegister, uint32 metadataValue, uint32 metadataOffset);

/**
 * @brief  Get the first address of uC allocated area in SRAM
 *
 * @return Success - Address of uC allocated area in SRAM, 0 otherwise.
*/
uint32 VIDRV_ipa_GetUcSramBaseAddress(void);

/**
 * @brief  Get the size of the uC SRAM.
 *
 * @return Success - Size of the uC SRAM, 0 otherwise.
*/
uint32 VIDRV_ipa_GetUcSramSize(void);

/** @} */ /* Ends the VIDRV_ipa_Interface documentation */

/**
 * @brief   Load dictionary for RO decompression to a decidated address in SRAM.
 *
 * @param   pathToDictionary	Path of dictionary to load.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_LoadDecompressionDictionary(const char* pathToDictionary);

/**
 * @brief   Set sequence type of a pipe.
 *
 * @param   ipaPipeConsumer   The consumer pipe.
 * @param   seqType           The sequence type to set.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpInitSequence(VIDRV_ipa_PipeHandle ipaPipeConsumer, VIDRV_ipa_UserSequenceType seqType);

/**
 * @brief   Set sequence type of a pipe for the replication mechanism.
 *
 * @param   ipaPipeConsumer   The consumer pipe.
 * @param   seqType           The sequence type to set.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpInitRepSequence(VIDRV_ipa_PipeHandle ipaPipeConsumer, VIDRV_ipa_UserSequenceType seqType);

/**
 * @brief   Allocation of an ENDP (source or destination) to a resource-pipe-group (source-group or destination-group)
 *
 * @param   hPipe             The pipe (ENDP) handle
 * @param   resourceGroup     Identifier of the resource-pipe-group. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpResourceGroup(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_ResourceGroupType resourceGroup);

/**
 * @brief   Get endpoint resource group which is CMDQ ID for consumer. 
 *
 * @param   hPipe             The pipe (ENDP) handle 
 * @param   pResourceGroup    Pointer to identifier of the resource-pipe-group. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
 */
VIDRV_ipa_ResultType VIDRV_ipa_GetEndpResourceGroup(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_ResourceGroupType* pResourceGroup);

/**
 * @brief   Sets the HP (high priority) authorization of a resource-pipe-group
 *
 * @param   resourceGroup     Identifier of the resource-pipe-group. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetHighPriorityResourceGroup(VIDRV_ipa_ResourceGroupType resourceGroup);

/**
 * @brief   Configures a destination pipe to be high priority.
 *
 * @param   pipeHandle  Handler to the prosucer pipe to be set as high priority. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetHighPriorityDestinationPipe(VIDRV_ipa_PipeHandle pipeHandle);

/**
 * @brief   Reset the HP (high priority) resource-pipe-group
*/
void VIDRV_ipa_ResetHighPriorityResourceGroup(void);

/**
 * @brief   Reset the HP (high priority) destination pipe
*/
void VIDRV_ipa_ResetHighPriorityDestinationPipe(void);

/**
 * @brief   Set weight value for specified RX_HPS CMDQ. 
 *
 * @param   rxHpsCmdqId    The Id of RX_HPS CMDQ
 * @param   weight         The weight value
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetRxHpsCmdqWeight(VIDRV_ipa_RxHpsCmdqIds rxHpsCmdqId, uint32 weight);

/**
 * @brief   Set weight value for specified HPS_DPS CMDQ ( in relation to the producer pipe ). 
 *
 * @param   hPipe          The producer pipe (ENDP) handle
 * @param   weight         The weight value
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetHpsDpsCmdqWeight(VIDRV_ipa_PipeHandle hPipe, uint32 weight);

/**
 * @brief   Set weight value for specified DPS_TX CMDQ ( in relation to the producer pipe ). 
 *
 * @param   hPipe          The producer pipe (ENDP) handle
 * @param   weight         The weight value
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetDpsTxCmdqWeight(VIDRV_ipa_PipeHandle hPipe, uint32 weight);

/**
 * @brief  packed structures sizes test
 *
 * @note  Compilation time check
*/
void VIDRV_ipa_PpcStructSizeChecker(void);

/**
 * @brief  packed structures sizes test
 *
 * @note  Compilation time check
*/
void VIDRV_ipa_RouteStructSizeChecker(void);

/**
 * @brief  packed structures sizes test
 *
 * @note  Compilation time check
*/
void VIDRV_ipa_FilterStructSizeChecker(void);

/**
 * @brief  packed structures sizes test
 *
 * @note  Compilation time check
*/
void VIDRV_ipa_StructSizeChecker(void);

/**
 * @brief   Sets value to a requested field in the UC_QMB_MISC register.
 *
 * @param   field    Requested field
 * @param   value    Value of the field
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetUcQmbMisc(VIDRV_ipa_UcQmbMiscType field, uint32 value);

/**
 * @brief   Sets value to a requested field in the ENDP_INIT_MODE register.
 *
 * @param   hPipe    The pipe handle
 * @param   field    Requested field
 * @param   value    Value of the field
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpInitMode(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_EndpInitModeType field, uint32 value);

/**
 * @brief   Sets value to a requested field in the ENDP_INIT_DEAGGR register.
 *
 * @param   hPipe    The pipe handle
 * @param   field    Requested field
 * @param   value    Value of the field
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpInitDeaggr(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_EndpInitDeaggrType field, uint32 value);

/**
 * @brief   Sets TLV FIFO configuration parameters for a given pipe.
 *
 * @param   hPipe             The pipe handle
 * @param   fifoSize          FIFO size
 * @param   fifoStartAddress  FIFO start address
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeGsiCfgTlv(VIDRV_ipa_PipeHandle hPipe, uint32 fifoSize, uint32 fifoStartAddress);

/**
 * @brief   Get the depth of the TLV FIFO for a given pipe.
 *
 * @param   hPipe             The pipe handle. 
 *
 * @return  The depth of the TLV FIFO for a given pipe.
*/
uint32 VIDRV_ipa_GetPipeGsiCfgTlvFifoSize(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Get the depth of the AOS FIFO for a given pipe.
 *
 * @param   hPipe             The pipe handle. 
 *
 * @return  The depth of the AOS FIFO for a given pipe.
*/
uint32 VIDRV_ipa_GetPipeGsiCfgAosFifoSize(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Sets AOS FIFO configuration parameters for a given pipe.
 *
 * @param   hPipe             The pipe handle
 * @param   fifoSize          Fifo size
 * @param   fifoStartAddress  Fifo start address
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeGsiCfgAos(VIDRV_ipa_PipeHandle hPipe, uint32 fifoSize, uint32 fifoStartAddress);

/**
 * @brief   Sets value to a requested field in the ENDP_GSI_CFG2 register.
 *
 * @param   hPipe    The pipe handle
 * @param   field    Requested field
 * @param   value    Value of the field
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeGsiCfg2(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_EndpGsiCfg2Type field, uint32 value);

/**
 * @brief   Sets value to a requested field in the ENDP_GSI_CFG1 register.
 *
 * @param   hPipe    The pipe handle
 * @param   field    Requested field
 * @param   value    Value of the field
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeGsiCfg1(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_EndpGsiCfg1Type field, uint32 value);

/**
 * @brief   Reads 32 bit from the specified offset. 
 *
 * @param   offset   The offset from the core base address. 
 *
 * @return  The data read from the offset. 
*/
uint32 VIDRV_ipa_Read32(uint32 offset);

/**
 * @brief   Writes 32 bit to the specified offset. 
 *
 * @param   offset   The offset from the core base address. 
 * @param   data     The data to be written. 
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_Write32(uint32 offset, uint32 data);

/**
 * @brief   Gets value of decompression page size. 
 *
 * @return  On success - the size decompressed page in bytes, on failure - 0.
*/
uint32 VIDRV_ipa_GetDecompressionPageSize(void);

/**
 * @brief   Gets value of max compressed page size in RW mode (reads value from IPA_DCMP_CFG reg).
 *
 * @return  On success - the max size of RW compressed page in bytes, on failure - 0.
*/
uint32 VIDRV_ipa_GetCompressionRwPageMaxSize(void);

/**
 * @brief   Gets value of max compressed page size in RO mode (value is defined hard coded).
 *
 * @return  On success - the max size of RO compressed page in bytes, on failure - 0.
*/
uint32 VIDRV_ipa_GetCompressionRoPageMaxSize(void);

/**
 * @brief   Sets QSB configuration of Max outstanding requests (read/write) for IPAs QSB masters.
 *
 * @param   qmb0MaxWrites  Max number of outstanding writes for QMB 0
 * @param   qmb0MaxReads   Max number of outstanding reads for QMB 0
 * @param   qmb1MaxWrites  Max number of outstanding writes for QMB 1
 * @param   qmb1MaxReads   Max number of outstanding reads for QMB 1
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetMaxOutstandingRequests(uint8 qmb0MaxWrites, uint8 qmb0MaxReads, uint8 qmb1MaxWrites, uint8 qmb1MaxReads);

/**
 * @brief   Sets QSB configuration of Max outstanding requests (read/write) for IPAs QSB masters.
 *
 * @param[out]   pQmb0Writes  If non NULL, holds the actual number of outstanding writes for QMB 0
 * @param[out]   pQmb0Reads   If non NULL, holds the actual number of outstanding reads for QMB 0
 * @param[out]   pQmb1Writes  If non NULL, holds the actual number of outstanding writes for QMB 1
 * @param[out]   pQmb1Reads   If non NULL, holds the actual number of outstanding reads for QMB 1
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetQsbOutstandingCounter(uint8* pQmb0Writes, uint8* pQmb0Reads, uint8* pQmb1Writes, uint8* pQmb1Reads);

/**
 * @brief   Selects the QMB master for a specific pipe.
 *
 * @param   pipeId   Pipe ID
 * @param   qmbId    QMB master Id
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetPipeQmbMasterSelect(uint32 pipeId, uint8 qmbId);

/**
 * @brief   Gets the QMB master for a specific pipe.
 *
 * @param   pipeId   Pipe ID
 *
 * @return  QMB master Id for the given pipe
*/
uint8 VIDRV_ipa_GetPipeQmbMasterSelect(uint32 pipeId);

/**
 * @brief   Sets the producer configuration.
 *
 * @param   hPipe    The pipe handle
 * @param   txSelect TX selector Id - if bigger/equal than/to IPA_NUMBER_OF_TXS the value is randomized
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetProducerConfig(VIDRV_ipa_PipeHandle hPipe, uint8 txSelect);

/**
 * @brief   Gets the producer configuration.
 *
 * @param      pipeId   Pipe ID
 * @param[out] txSelect TX selector Id
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetProducerConfig(VIDRV_ipa_PipeHandle hPipe, uint8* txSelect);

/**
 * @brief   Sets the limits (min/max) for a resource-type of required resource-group
 *
 * @param   resourceGroupIndex      The index of a source resource-group
 * @param   resourceType            The index of resource-type
 * @param   minLimit                Min limit for resource-group "resourceGroupIndex" and
 *                                  resource-type "resourceType"
 * @param   maxLimit                Max limit for resource-group "resourceGroupIndex" and
 *                                  resource-type "resourceType"
 *
 * @note    The value IPA_RSRC_GRP_LIMIT_INVALID_VALUE can be used to prevent a limit (min/max) change.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_SetResourceTypeGroupLimits(
   VIDRV_ipa_ResourceGroupType               resourceGroupIndex      , 
   VIDRV_ipa_ResourceTypesType               resourceType            , 
   uint32                                    minLimit                , 
   uint32                                    maxLimit                );


/**
 * @brief   Gets the limits (min/max) for a resource-type of required resource-group
 *
 * @param   resourceGroupIndex      The index of a source resource-group
 * @param   resourceType            The index of resource-type
 * @param   minLimit                The pointer to min limit for resource-group "resourceGroupIndex" and
 *                                  resource-type "resourceType"
 * @param   maxLimit                The pointer to max limit for resource-group "resourceGroupIndex" and
 *                                  resource-type "resourceType"
 *
 * @note    The value IPA_RSRC_GRP_LIMIT_INVALID_VALUE can be used to prevent a limit (min/max) change.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_GetResourceTypeGroupLimits(
   VIDRV_ipa_ResourceGroupType               resourceGroupIndex      , 
   VIDRV_ipa_ResourceTypesType               resourceType            , 
   uint32*                                   minLimit                , 
   uint32*                                   maxLimit                );


/**
 * @brief   Gets the number of used resources for a specific resource-type of required resource-group
 *
 * @param   resourceGroupIndex      The index of a source resource-group
 * @param   resourceType            The index of resource-type
 * @param   counter                 The pointer to return value
 *
 * @note    The value IPA_RSRC_GRP_LIMIT_INVALID_VALUE can be used to prevent a limit (min/max) change.
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_GetResourceTypeGroupCounter(
   VIDRV_ipa_ResourceGroupType               resourceGroupIndex      , 
   VIDRV_ipa_ResourceTypesType               resourceType            , 
   uint32*                                   counter                 ); 

/**
 * @brief   Set the source and destination resource group/type configuration. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_LoadDefaultResourceConfiguration(void);

/**
 * @brief   Default IPA_SPARE_REG_1 configuration. 
 */
extern const uint32 ipaDefaultSpareReg1Configuration;

/**
 * @brief   Default IPA_SPARE_REG_2 configuration. 
 */
extern const uint32 ipaDefaultSpareReg2Configuration;

/**
 * @brief   State of all Aggr endpoints
 *
 * @return  Bitmap of states. Bit n represents pipe n.  
*/
uint32 VIDRV_ipa_GetStateAggrActiveBitmap(void);

/**
 * @brief   Sets the clock source for the time counter used for packet status 
 *
 * @param   disablePipelining  FALSE: 32Khz sleep clock (default). This configuration is meant for production code and IPAv1 compatibility, 
 *                             TRUE: IPA clock. This configuration is meant for performance testing (accurate time measurement)
 *
 * @return  VIDRV_IPA_SUCCESS if successful. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetTimeCntClkSrc(boolean clkSource);

/**
 * @brief   Configure uC timer. 
 *
 * @param   n         Timer identifier 0-3. Timers 0-1 are clocked by SLEEP_CLK (~32KHz) and timers 2-3 are clocked by the IPA clock. 
 * @param   count     Counter start value. 
 * @param   retrig    0: ONE_SHOT (If counter armed, on trigger event - count once and stop) 1: RETRIG (If counter armed, reload and wait for trigger again when counter expires). 
*/
void VIDRV_ipa_SetUcTimer(uint32 n, uint32 count, uint32 retrig);

/**
 * @brief   Read count value of uC timer. 
 *
 * @param   n         Timer identifier. 
 *
 * @return  The count value (start value - cycles number). 
*/
uint32 VIDRV_ipa_GetUcTimerCount(uint32 n);

/**
 * @brief   Check whether it's possible to configure header insertion in DMA mode. 
 *
 * @return  TRUE if header insertion in DMA mode is supported, FALSE otherwise. 
*/
boolean VIDRV_ipa_IsHeaderInsertionInDmaModeSupported(void);

/**
 * @brief   Configure header insertion in DMA mode if it's supported.
 *
 * @return  VIDRV_IPA_SUCCESS if header insertion in DMA mode is supported, VIDRV_IPA_NOT_SUPPORTED otherwise. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ConfigureHeaderInsertionInDmaMode(void);

/**
 * @brief   Configure threshold based DMA mode if it's supported.
 *
 * @return  VIDRV_IPA_SUCCESS if threshold based DMA mode is supported, VIDRV_IPA_NOT_SUPPORTED otherwise. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_ConfigureThresholdBasedDmaMode(void);

/**
 * @brief   Check whether a given deaggregation protocol is supported. 
 *
 * @param   aggregationProtocol         a protocol to check. 
 *
 * @return  TRUE if the protocol is supported, FALSE otherwise. 
*/
boolean VIDRV_ipa_IsDeaggregationSupported(VIDRV_ipa_AggrType aggregationProtocol);

/**
 * @brief   Check whether a given aggregation protocol is supported. 
 *
 * @param   aggregationProtocol         a protocol to check. 
 *
 * @return  TRUE if the protocol is supported, FALSE otherwise. 
*/
boolean VIDRV_ipa_IsAggregationSupported(VIDRV_ipa_AggrType aggregationProtocol);

/**
 * @brief   Get pipe interface (GSI/BAM). 
 *
 * @return  The pipe interface in use.
*/
VIDRV_ipa_PipeInterfaceType VIDRV_ipa_GetPipeInterface(void);

/**
 * @brief   Returns the physical base address of the currently used pipe interface (GSI/BAM).
 *
 * @return  The physical base address of the currently used pipe interface.
*/
uint64 VIDRV_ipa_GetPipeInterfacePhysicalBase(void);

/**
 * @brief   Gets the ipa_endp_gsi_cgf1_n register value. 
 *
 * @param[in]     pipeNum       The number of pipe to access register (n).
 * @param[out]    chid         The chid (channel id) field in ipa_endp_gsi_cgf1_n.
 * @param[out]    ee          The ee (execution environment) field in ipa_endp_gsi_cgf1_n.
 * @param[out]    endpEn       The endp_en (Endpoint enable) field in ipa_endp_gsi_cgf1_n.
 *
 * @return  VIDRV_IPA_SUCCESS on success, on fail otherwise.
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetEndpGsiCfg1(uint32 pipeNum, uint32* chid, uint32* ee, boolean * endpEn);

/**
 * @brief   Makes all initialization to be ready for power collapse (load uC SW image if needed)
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
 *
 * @note    #define IPA_POWER_COLLAPSE_SUPPORTED
 *          0   power collapse is not available on current target
 *          1   software power collapse. uC code is involved into power collapse sequence.
 *          2   hardware power collapse. uC code is not involved into power collapse sequence.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_InitPowerCollapse(void);

/**
 * @brief   Makes all uninitialization after power collapse
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_UninitPowerCollapse(void);

/**
 * @brief   Saves IPA state using uC and performs power down of the IPA and uC. (the part of PC sequence)
 *
 * @note    uC software team image should be loaded before usage
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_BackupAndPowerDown(void);

/**
 * @brief   Performs power up of the IPA and waits for uC response about
 *			   successful restoration of IPA state after PC.  (the part of PC sequence)
 *
 * @note    uC software team image should be loaded before usage
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType  VIDRV_ipa_PowerUpAndRestore(void);

/**
 * @brief   Read Debug command from RX_SPLT Fifo. 
 *
 * @param         cmdqId         CMDQ identifier. 
 * @param [out]   pSrcAddr       If non-null, source address. 
 * @param [out]   pSrcLen        If non-null, length of the source. 
 * @param [out]   pMisc          If non-null, misc field. ( Where MISC[7:0] = { FLAGS[5:0], ORDER[1:0] }  )
 * @param [out]   pSrcPipeId     If non-null, identifier for the source pipe. 
 * @param [out]   pMbimMetadata  If non-null, the mbim metadata. 
 * @param [out]   pPacketLen     If non-null, total length of the packet. 
 * @param [out]   pOpcode        If non-null, the opcode. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_RxSpltReadDbgCmdData(
   VIDRV_ipa_RxSpltCmdqIds    cmdqId         ,
   uint64                     *pSrcAddr      ,
   uint32                     *pSrcLen       ,
   uint8                      *pMisc         ,
   uint32                     *pSrcPipeId    ,
   uint32                     *pMbimMetadata ,
   uint32                     *pPacketLen    ,
   uint32                     *pOpcode       );

/**
 * @brief   Read Debug command from TX_COMMANDER Fifo. 
 *
 * @param [out]   pDstAddr       If non-null, internal address. 
 * @param [out]   pDstLen        If non-null, length of the destination. 
 * @param [out]   pPacketLen     If non-null, total length of the packet. 
 * @param [out]   pOrder         If non-null, order of command. 
 * @param [out]   pDstPipeId     If non-null, identifier for the destination pipe. 
 * @param [out]   pRsrcArg       If non-null, resource argument field of CMDQ field
 * @param [out]   pRsrcType      If non-null, resource type field of CMDQ field
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_TxCommanderReadDbgCmdData(
   uint32                        *pDstAddr,
   uint32                        *pDstLen,
   uint32                        *pPacketLen,
   VIDRV_ipa_DebugCmdOrderType   *pOrder,
   uint32                        *pDstPipeId,
   uint8                         *pRsrcArg,
   uint8                         *pRsrcType   );

/**
 * @brief   Writes data to uC SRAM. 
 *
 * @param      offset            The offset in bytes. 
 * @param [in] data              The data. 
 * @param      dataSizeInBytes   The size in bytes. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_WriteUcSram(uint32 offset, const uint8* data, uint32 dataSizeInBytes);

/**
 * @brief   reads data from uC SRAM. 
 *
 * @param         offset            The offset in bytes. 
 * @param [out]   data              The buffer to hold the data. 
 * @param         dataSizeInBytes   The size in bytes. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed.
*/
VIDRV_ipa_ResultType VIDRV_ipa_ReadUcSram(uint32 offset, uint8* data, uint32 dataSizeInBytes);

/**
 * @brief   Return a value from IPA HW config database
 *
 * @param   paramIndex     parameter index,
 *                         for more information look at IpaHwConfigParametersType enum
 * @return  -1 if failed, else value from IPA HW config data base.
*/
int32 VIDRV_ipa_GetHwParam(IpaHwConfigParametersType paramIndex);

/**
 * @brief   Enable uC mailbox interrupt
 *
 * @param   mailBoxBlock      mailbox block number
 * @param   mailBoxID         mailbox ID number
*/
VIDRV_ipa_ResultType VIDRV_ipa_EnableUcMboxInt(uint32 mailBoxBlock, uint32 mailBoxID);

/**
 * @brief   Disable uC mailbox interrupt
 *
 * @param   mailBoxBlock      mailbox block number
 * @param   mailBoxID         mailbox ID number

 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_DisableUcMboxInt(uint32 mailBoxBlock, uint32 mailBoxID);

/**
 * @brief   Clear uC mailbox interrupt
 *
 * @param   mailBoxBlock   mailbox block number
 * @param   mailBoxID      mailbox ID number

 * @return VIDRV_IPA_SUCCESS if successful, else failed

*/
VIDRV_ipa_ResultType VIDRV_ipa_ClearUcMboxInt(uint32 mailBoxBlock, uint32 mailBoxID);

/**
 * @brief   Get uC the status of a uC mailbox interrupt
 *
 * @param   mailBoxBlock   mailbox block number
 * @param   mailBoxID      mailbox ID number
 * @param [out]            whether the status bit of the corresponding mailbox is set

 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetUcMboxIntStts(uint32 mailBoxBlock, uint32 mailBoxID, boolean* isSet);

/**
 * @brief   Disable/Enable uC mailboxes
 *
 * @param   mask     mask of mailboxes to disable/enable

 * @return VIDRV_IPA_SUCCESS if successful, else failed

*/
VIDRV_ipa_ResultType VIDRV_ipa_SetUcMboxDis(uint8 mask);

/**
 * @brief   Disable/Enable Data Path Logging Timer.
 *
 * @param   isTimerEnable   TRUE - enable timer, FALSE otherwise
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetDataPathLoggingTimerEnable(boolean isTimerEnable);

/**
 * @brief   Sets the value of the Data Path Logging Timer.
 *
 * @param   timer   Value of the timer
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetDataPathLoggingTimer(uint64 timer);

/**
 * @brief   Disables DPL timer, sets new value and enables it again.
 *
 * @param   timer   Value of the timer
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetDataPathLoggingTimerSequence(uint64 timer);

/**
 * @brief   Returns IPA pipe interface handle.
 *
 * @return  IPA pipe interface handle
*/
void* VIDRV_ipa_GetPipeInterfaceHandle(void);

/**
 * @brief   Returns IPA pipe BAM/GSI channel.
 *
 * @param   hPipe The pipe handle. 
 *
 * @return  IPA pipe BAM/GSI channel.
*/
void* VIDRV_ipa_GetPipeChannel(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Returns IPA pipe id.
 *
 * @param   hPipe The pipe handle. 
 *
 * @return  IPA pipe id.
*/
uint32 VIDRV_ipa_GetPipeId(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Attaches a status queue to a consumer pipe.
 *
 * @param   Pipe handle of the consumer.
 * @param   Pipe handle of the status queue.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetEndpointStatusGen(VIDRV_ipa_PipeHandle hPipe, VIDRV_ipa_StatusQueueHandle hStatusQueue);

/**
 * @brief   Generate uC NMI
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_GenerateUcNmi(void);

/**
* @brief   Enables/Disables TX prefetch. 
*
* @param   txSelect       TX selector
* @param   enable         true to enable, false to disable. 
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetTxPrefetch(uint32 txSelect, boolean enable);

/**
* @brief   Returns index of current EE. 
*
* @return  Current EE index
*/
VIDRV_ipa_EeType VIDRV_ipa_GetCurrentEe(void);

/**
* @brief   Saves current command sniffer position. 
*
* @param   pPosition [out]      position
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_CmdSnifferGetCurrentPosition(VIDRV_ipa_SnifferCmdPositionType* pPosition);

/**
* @brief   Calculates the number of new elements in command sniffer buffer from previous pStartPosition
*
* @note    For correct calculation the number of new events should be less or equal to sniffer buffer size
*
* @param   pStartPosition [in]     Start position saved before using VIDRV_ipa_CmdSnifferGetCurrentPosition()
* @param   pNumOfElements [out]    number of new elements
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_CmdSnifferGetNumberOfNewElements(VIDRV_ipa_SnifferCmdPositionType* pStartPosition, uint32* pNumOfElements);

/**
* @brief   Returns pointer to command sniffer element from internal or external FIFO from previous pStartPosition
*
* @note    For correct operation the number of new events should be less or equal to sniffer buffer size
*
* @param   pStartPosition [in]     Start position saved before using VIDRV_ipa_SnifferGetCurrentPosition()
* @param   elementIndex   [in]     The index of the element
* @param   pSniffEntry    [out]    Pointer to command sniffer element
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_CmdSnifferGetElement(VIDRV_ipa_SnifferCmdPositionType* pStartPosition, uint32 elementIndex, const VIDRV_ipa_HwSniffCommand** pSniffEntry);

/*
* @brief   Configures DPL bandwidth reduction
*
* @param   isLogReduction [in]     TRUE to enable and FALSE to disable the feature
*
* @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetLogReduction(boolean isLogReduction);

/*
* @brief   Configures DPL L2 header removing
*
* @param   isLogL2Remove [in]     TRUE to enable and FALSE to disable the feature
*
* @return  VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetLogL2Remove(boolean isLogL2Remove);

/**
* @brief   Configure select QMB by address feature
* 
* @param   onProducerEn   TRUE to enable on producer
* @param   onConsumerEn   TRUE to enable on consumer
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetQmbSelectByAddress(boolean onProducerEn, boolean onConsumerEn);

/**
* @brief   Get value of metadata register of a certaion pipe
* 
* @param [in]  hPipe   pipe handle
* @param [out] metadataValue   metadata register value
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_GetEndpInitMetadata(VIDRV_ipa_PipeHandle hPipe, uint32* metadataValue);

/**
 * @brief   Configure EndPoint status suppression (valid only for consumer endpoints). 
 *
 * @param   hPipe         The pipe handle.
 * @param   set           If TRUE status will be suppressed for this endpoint. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, otherwise failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetStatusSuppression(VIDRV_ipa_PipeHandle hPipe, boolean set);

/**
 * @brief   Sets the ports arbiter mask.
 *
 * @param   enable           If TRUE ports arbiter is masked, FALSE unmasked. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, otherwise failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetTxCfgPaMaskEn(boolean enable);

/**
 * @brief   Enables quota statistic for required pipes
 *
 * @param [in]  config           list of pipes to configure statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableQuotaStatistic(const VIDRV_ipa_QuotaStatisticConfigType * config);

/**
 * @brief   Disables quota statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisableQuotaStatistic(void);

/**
 * @brief   Enables Tethering statistic for required pipes
 *
 * @param [in]  config           list of pipes to configure statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableTetheringStatistic(const VIDRV_ipa_TetheringStatisticConfigType * config);

/**
 * @brief   Disables Tethering statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisableTetheringStatistic(void);

/**
 * @brief   Enable FnR statistic
 *
 * @param [in]  config           list of rules to configure statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_EnableFnrStatistic(const VIDRV_ipa_FnrStatisticConfigType * config);

/**
 * @brief   Disables FnR statistic
 *
 * @return  VIDRV_IPA_SUCCESS in case of success
 */
VIDRV_ipa_ResultType VIDRV_ipa_DisableFnrStatistic(void);

/**
 * @brief   Configures relaxed ordering(RO) attributes on AXI bus(PCIe)
 *
 * @param   ipaWr   TRUE to enable RO attribute for IPA write transactions
 * @param   ipaRd   TRUE to enable RO attribute for IPA read transactions
 * @param   gsiWr   TRUE to enable RO attribute for GSI write transactions
 * @param   gsiRd   TRUE to enable RO attribute for GSI read transactions
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetRelaxedOrdering(boolean ipaWr, boolean ipaRd, boolean gsiWr, boolean gsiRd);

/**
* @brief   Saves current status sniffer position. 
*
* @param   pPosition [out]      position
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_StatusSnifferGetCurrentPosition(VIDRV_ipa_SnifferStatusPositionType* pPosition);

/**
* @brief   Calculates the number of new elements in status sniffer buffer from previous pStartPosition
*
* @note    For correct calculation the number of new events should be less or equal to sniffer buffer size
*
* @param   pStartPosition [in]     Start position saved before using VIDRV_ipa_CmdSnifferGetCurrentPosition()
* @param   pNumOfElements [out]    number of new elements
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_StatusSnifferGetNumberOfNewElements(VIDRV_ipa_SnifferStatusPositionType* pStartPosition, uint32* pNumOfElements);

/**
* @brief   Returns pointer to status sniffer element from internal or external FIFO from previous pStartPosition
*
* @note    For correct operation the number of new events should be less or equal to sniffer buffer size
*
* @param   pStartPosition [in]     Start position saved before using VIDRV_ipa_StatusSnifferGetCurrentPosition()
* @param   elementIndex   [in]     The index of the element
* @param   pSniffEntry    [out]    Pointer to status sniffer element
*
* @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_StatusSnifferGetElement(VIDRV_ipa_SnifferStatusPositionType* pStartPosition, uint32 elementIndex, const VIDRV_ipa_HwSniffStatusElementType** pSniffEntry);

/**
 * @brief   Configure pipe to be secured/non-secured endpoint
 *
 * @param   hPipe          The pipe handle.
 * @param   securedFlag    TRUE for secured, FALSE for non-secured endpoint. 
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetSecuredEndp(VIDRV_ipa_PipeHandle hPipe, boolean securedFlag);

/**
 * @brief   Returns TRUE for secured endpoint, otherwise returns FALSE in accordance with actual pipe configuration
 *
 * @param   hPipe          The pipe handle.
 *
 * @return  TRUE if the pipe is secured endpoint, otherwise FALSE. 
 */
boolean VIDRV_ipa_IsSecuredEndp(VIDRV_ipa_PipeHandle hPipe);

/**
 * @brief   Sets QSB configuration of Max read beats (data bus cycles)
 *
 * @param   qmb0MaxReadBeats  Max number of read beats for QMB 0
 * @param   qmb1MaxReadBeats  Max number of read beats for QMB 1
 *
 * @return VIDRV_IPA_SUCCESS if successful, else failed
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetMaxReadBeats(uint32 qmb0MaxReadBeats, uint32 qmb1MaxReadBeats);

/**
* @brief   Blocks/Unblocks producer ack manager.
*
* @param   state  TRUE - block, FALSE - unblock.
*
* @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_BlockProducerAckManager(boolean state);

/**
 * @brief   Set Coalescing group configuration.
 *
 * @param   hFirstEp   the first endpoint handle.
 * @param   vpNum      number of virtual pipes.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_ipa_SetCoalescingGroupConfig(VIDRV_ipa_PipeHandle hFirstEp, uint32 vpNum);

/**
 * @brief   Set Coalescing eviction LRU.
 *
 * @param   state       true to enable, false to disable. 
 * @param   threshold   LRU threshold.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetCoalescingEvictionLru(boolean state, uint32 threshold);

/**
 * @brief   Set Coalescing QMAP configuration.
 *
 * @param   muxIdSelect   Selects metadata bbyte to be use as muxId.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_ipa_SetCoalescingQmapConfig(uint32 muxIdSelect);

#ifdef __cplusplus
}
#endif

#endif /* VIDRV_IPA_H */
