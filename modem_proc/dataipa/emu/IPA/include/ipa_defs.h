/**---------------------------------------------------------------------------
   @file     ipa_defs.h

   @brief    IPA definitions per non-specific IPA core flavor (version).
   
   $Header: //components/rel/dataipa.mpss/3.2/emu/IPA/include/ipa_defs.h#1 $
   
   Copyright (c) 2012 - 2014 Qualcomm Technologies Incorporated.
   All Rights Reserved.
   Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/
#ifndef IPA_DEFS_H
#define IPA_DEFS_H

#include "comdef.h"
#include "compiler_int.h"
#include "ipa_flavor_defs.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   The amount of producers for RSC/RSB coalescing
 */
#define IPA_COALESCING_GROUP_PIPE_AMOUNT 2

/**
 * @brief   Given an argument we set all its bits
 *
*/
#define IPA_SETTING_ALL_BITS(arg)   arg = 0; arg = (~(arg));

/**
 * @brief      Used to disable the size field when deciphering
 */
#define IPA_DECIPHER_MAX_PACKET_DECIPHER_SIZE 0xFFFFFFFF

/**
 * @brief      MAX invalid value in order to disable deciphering, also MASK
 */
#define IPA_DECIPHER_BLOCK_OFF_CONTEXT_ID 0xF

/**
 * @brief      Due to Automatic counting feature we need to set the 36-39 bits 
 *             to with context-Id(index), 
 *
 */
#define IPA_DECIPHER_SET_CONTEXT_ID_IN_ADDR(addr, contextId)        addr &= ~(((uint64)IPA_DECIPHER_BLOCK_OFF_CONTEXT_ID) << 36); \
                                                                    addr |= ((uint64)contextId) << 36; 

/**
 * @brief   The number of ciphering/deciphering contexts
 *
 * @note    It is relevant since IPA3.1
*/
#define IPA_NUM_CIPHERING_DECIPHERING_CONTEXTS   4

/*
 * @brief   Filter/Routing "miss" is identified by this value.
*/
#define IPA_FNR_RULE_MISS_INDEX     0x3FF

/**
 * @brief   Invalid value of QMB id
 */
#define IPA_QMB_INVALID_ID     (0xFF)

/**
 * @brief   Invalid value of a resource group limit (min/max)
 */
#define IPA_RESOURCE_GROUP_LIMIT_INVALID_VALUE     (0xBAD)

/**
* @brief   Total number of sniffer elements in HW
*/
#define IPA_HWSNIFF_COUNT           3

/**
 * @brief   Maximal weight value of a CMDQ ( RX_HPS, HPS_DPS and DPS_TX )
 */
#define IPA_MAX_WEIGHT_FOR_CMDQ        (0xF)

/**
 * @brief   Maximal weight value of a CMDQ ( RX_HPS, HPS_DPS and DPS_TX )
 */
#define IPA_MIN_WEIGHT_FOR_CMDQ        (0x1)

/**
 * @brief   The maximal replication packet length.
 */
#define IPA_MAX_REPLICATION_LENGTH_IN_BYTES 256

/**
 * @brief   The maximum HPS header size
 */
#define IPA_MAX_HPS_HEADER_SIZE 256

   /**
 * @brief   Calculation the address (void *) of the an integer offset (in bytes). The offset can be any integer type (uint32, ... etc.)
 *
 * @note    As an address increment can be performed by this macro.
 * @note    The validation of the result correctness is in user responsibility, 
 *          because this macro performs only arithmetical operation and returns the result with type (void *). 
 *          
 *          Example: IPA_ADDR_PLUS_OFFSET( type = anyType, addr = (void*)0x0004), offset = 4 ) will return (anyType *)8
 *
 * @param type       The type of the returned pointer.
 * @param address    The basic address (any pointer).
 * @param offset     The offset in relation to the basic address. 
 */
#define IPA_ADDR_PLUS_OFFSET( type, address, offset )\
   ( (type *)( ((uint8 *)(address)) + (offset) ) )

/**
 * @brief   Calculation the address (void *) of the an integer negative offset (in bytes). The offset can be any integer type (uint32, ... etc.)
 *
 * @note    As an address decrement can be performed by this macro.
 * @note    The validation of the result correctness is in user responsibility, 
 *          because this macro performs only arithmetical operation and returns the result with type (void *). 
 *          
 *          Example: IPA_ADDR_MINUS_OFFSET( type = anyType, addr = (void*)0x0004), offset = 4 ) will return (anyType *)8
 *
 * @param type       The type of the returned pointer.
 * @param address    The basic address (any pointer).
 * @param offset     The offset in relation to the basic address. 
 */
#define IPA_ADDR_MINUS_OFFSET( type, address, offset )\
   ( (type *)( ((uint8 *)(address)) - (offset) ) )

/**
 * @brief   Convertion of the integer type to address (void *)
 *
 * @param   _addr_    The address represented as integer type
 */
#define IPA_UINT_TO_PTR(_addr_)           (void*)((size_t)(_addr_))

/**
 * @brief   Convertion of the address (void *) to unsigned type of required size
 *
 * @param   _addr_    The basic address (any pointer).
 */
#define IPA_PTR_TO_UINT(_addr_)           ((size_t)(_addr_))

/**
 * @brief   Return MSB part of the physical address 
 *
 * @param   _addr_    The address.
 */
#define IPA_PHYS_ADDR_TO_MSB(_ptr_)             ((uint32)(((_ptr_) >> 16) >> 16))

/**
 * @brief   Return LSB part of the physical address
 *
 * @param   _addr_    The address.
 */
#define IPA_PHYS_ADDR_TO_LSB(_ptr_)             ((uint32)(_ptr_))

/**
 * @brief   Construct physical address from LSB and MSB parts
 *
 * @param   _msb_    MSB
 * @param   _lsb_    LSB
 */
#define IPA_MSB_LSB_TO_PHYS_ADDR(_msb_, _lsb_)  (((((uint64)(_msb_))<<16)<<16)|(uint64)(_lsb_))

/**
* @brief   Calculation of the padding length for given size in accordance to scale of needed padding (paddingScale).
*          Thus : ( size + IPA_CALCULATE_PADDING( size, paddingScale ) ) is the multiple of paddingScale.
*          Example: IPA_CALCULATE_PADDING( 13, 4 ) will return 3, because ( 13 + 3 ) % 4 = 0
*
* @param size          The size of a data-block which should be padded.
* @param paddingScale  The scale of needed padding. 
*/
#define IPA_CALCULATE_PADDING( size, paddingScale )\
   ((paddingScale)?( ( (paddingScale) - ( (size)%(paddingScale) ) ) % (paddingScale) ) : 0)

/**
* @brief   Convert physical address to virtual address
*
* @param physBase          physical base address
* @param virtBase          virtual base address
* @param physAddr          physical address
*/
#define IPA_ADDR_PHYS_TO_VIRT(physBase, virtBase, physAddr) IPA_ADDR_PLUS_OFFSET(uint8, (size_t)(virtBase), (size_t)(physAddr) - (size_t)(physBase))

/**
* @brief   Number of TX blocks
*/
#define IPA_NUMBER_OF_TXS                 2

/**
* @brief   Number of endpoints
*/
#define IPA_MAX_PIPE_NUM                  (MAX(IPA_PIPE_PRODUCER_MAX_ID, IPA_PIPE_CONSUMER_MAX_ID) + 1)

/* Convex support (QUP defines it to something else)*/
#undef GSI_API
#define GSI_API(api_name)              VIDRV_ipa_##api_name
#undef GSI_PLATFORM_FILE
#define GSI_PLATFORM_FILE "VVHAL_platform.h"

/* Target specific number of GSI transfer channels and event channels */
#define GSI_MAX_NUMBER_OF_CHANNELS     IPA_MAX_NUM_OF_GSI_CHANNELS_PER_EE
#define GSI_MAX_NUMBER_OF_EVENTS       IPA_MAX_NUM_OF_GSI_EVENT_CHANNELS_PER_EE

/* This macro universally defines default size for BAM descriptor FIFO and GSI ring. 
 */
#define IPA_DEFAULT_NUM_OF_RING_ITEMS  256
#define GSI_DEFAULT_RING_SIZE          (IPA_DEFAULT_NUM_OF_RING_ITEMS * 16)

/* The maximal sizes of GSI ring and the maximal number of ring elements that can be queued at once.
*/
#define GSI_MAX_RING_SIZE              0xfff0
#define GSI_MAX_NUM_OF_RING_ITEMS      (GSI_MAX_RING_SIZE / 16 - 1)

/**
 * @def  IPA_MAX_BAM_DESCRIPTOR_SIZE
 *
 * @brief   Maximum transfer size in bytes ( = 32KB - 1 ). 
 * It is max value of the field (!) size of a descriptor
*/
#define IPA_MAX_BAM_DESCRIPTOR_SIZE    0x7FFF

/**
* @brief Macros to make conditional compilation bug
*
* @param condition    Use true condition to cause the compilation bug
*
* @note Usage example: VIDRV_IPA_COMPILATION_BUG_ON(sizeof(stuct X)!=Y);
*/
#define VIDRV_IPA_COMPILATION_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/**
 * @brief   TX Threshold marker type
 * 
 * Red and Yellow IPA thresholds that allows SW / uC to receive notification when producer 
 * pipe buffer availability falls below or above a threshold.
 */
typedef enum
{
    IPA_TXTHRSHLD_RED, /**< Red Tx Threshold */
    IPA_TXTHRSHLD_YELLOW, /**< Yellow Tx Threshold */
} VIDRV_ipa_TxThresholdType;

/**
 * @brief The source of HW events to be monitored. 
 * @remark [IPA 2.0 SWI].HW_EVENTS_SELECT
*/
typedef enum
{
   IPA_HW_EVENT_SELECT_IPA      = 0, /**< IPA as the source for HW events*/
   IPA_HW_EVENT_SELECT_BAM_HW   = 1, /**< BAM HW as the source for HW events*/
   IPA_HW_EVENT_SELECT_UC       = 2, /**< uC as the source for HW events*/
   IPA_HW_EVENT_SELECT_MIXED    = 3, /**< mixed uC/IPA events ([31:18] => uC, [17:0] => IPA)*/
   IPA_HW_EVENT_SELECT_BAM_SW   = 4, /**< BAM SW as the source for HW events*/
} VIDRV_ipa_HwEventSelectType;

/**
 * @brief   Values that represent IPA statistics counters - for debugging.
 * Each counter may count one of the preprogramed events.
*/
typedef enum
{
   IPA_DEBUG_COUNTER_IPV4_FILTERING_RELATED = 0, /**< Ipv4 filtering related counter */
   IPA_DEBUG_COUNTER_IPV4_ROUTING_RELATED   = 1, /**< Ipv4 routing related counter */
   IPA_DEBUG_COUNTER_GENERAL                = 2, /**< General counter */
   IPA_DEBUG_COUNTER_IPV6_FILTERING_RELATED = 4, /**< Ipv6 filtering related counter */
   IPA_DEBUG_COUNTER_IPV6_ROUTING_RELATED   = 5, /**< Ipv6 routing related counter */
   IPA_DEBUG_COUNTER_HOLB_ACTIVATED         = 6, /**< Count HOLB activation on PKT on dest pipes */
} VIDRV_ipa_DebugCounterType;

/**
 * @brief   Values that represent timer granularity types. 
*/
typedef enum
{
   IPA_TIMER_GRANULARITY_0_125_MS    = 0x3,  /**< 1/8 ms */
   IPA_TIMER_GRANULARITY_0_25_MS     = 0x7,  /**< 1/4 ms */
   IPA_TIMER_GRANULARITY_0_5_MS      = 0xF,  /**< 1/2 ms */
   IPA_TIMER_GRANULARITY_1_MS        = 0x1F, /**< 1 ms */
}VIDRV_ipa_TimerGranularityType;

/**
 * @brief   Values that represent HDLC mode Type. 
*/
typedef enum
{
   IPA_BASIC_MODE = 0, /**< In the basic mode the HDLC block is working - currently nothing will be done */
   IPA_DCMP_RO    = 1, /**< RO decompression will be enabled */
   IPA_DMA_MODE   = 3, /**< In the DMA mode all the blocks are bypassed */
   IPA_DCMP_RW    = 5, /**< RW decompression will be enabled */
   IPA_CMP_RW     = 6, /**< RW compression will be enabled */
} VIDRV_ipa_HdlcModeType;

/**
 * @brief   Values that represent the NAT type. 
*/
typedef enum
{
   IPA_BYPASS_NAT = 0, /**< Bypass NAT */
   IPA_SRC_NAT    = 1, /**< Source NAT */
   IPA_DST_NAT    = 2, /**< Destination NAT */
} VIDRV_ipa_NatType;

/**
 * @brief   Values that represent the Conn Track type - reused of NAT types
*/
typedef enum
{
   IPA_BYPASS_CONN_TRACK   = 0,              /**< Bypass Conn Track */
   IPA_OUTBOUND_CONN_TRACK = 1,              /**< Outbound Conn Track */
   IPA_INBOUND_CONN_TRACK  = 2,              /**< Inbound Conn Track */
   IPA_OUTBOUND_AND_INBOUND_CONN_TRACK  = 3, /**< Outbound and Inbound Conn Track */
   
   IPA_CONN_TRACK_TYPE_NUMBER = IPA_OUTBOUND_AND_INBOUND_CONN_TRACK, /**< Amount of types */
} VIDRV_ipa_ConnTrackType;

/**
 * @brief   Values that represent the various execution environments.
*/
typedef enum
{
#if defined(IPA_FLAVOR_3_1_V2_Q) || defined(IPA_FLAVOR_3_1) || defined(IPA_FLAVOR_3_0)
   IPA_EE_A5      = 0,         /**< Application processor */
   IPA_EE_Q6      = 1,         /**< QDSP6 processor */
   IPA_EE_TZ      = 2,         /**< Trust Zone */
   IPA_EE_UC      = 3,         /**< micro controller, exception GSI */
   IPA_MAX_EE_NUM = 4,         /**< The maximum number of execution areas supported by IPA. */
   IPA_EE_UC_GSI  = 3,         /**< Index of GSI execution area for uC */
#else
   IPA_EE_A5      = 0,         /**< Application processor */
   IPA_EE_Q6      = 1,         /**< QDSP6 processor */
   IPA_EE_TZ      = 2,         /**< Trust Zone, Not in use since IPA 3.5 */
   IPA_EE_UC      = 3,         /**< micro controller, exception GSI */
   IPA_MAX_EE_NUM = 4,         /**< The maximum number of execution areas supported by IPA. */
   IPA_EE_UC_GSI  = 2,         /**< Index of GSI execution area for uC */
#endif

#ifdef COMPILING_MSS_PROC   
   IPA_EE_CURRENT = IPA_EE_Q6,
#else
   IPA_EE_CURRENT = IPA_EE_A5, /**< Current application is use */
#endif
} VIDRV_ipa_EeType; 

/**
 * @brief Blocks in IPA which are source for debug information. 
 * @remark [IPA 2.0 SWI].IPA_TESTBUS_SEL.EXTERNAL_BLOCK_SELECT (4 bits, TESTBUS) <br>
 *         [IPA 2.0 SWI].IPA_DEBUG_DATA_SEL.EXTERNAL_BLOCK_SELECT (4 bits, DEBUGBUS)
*/
typedef enum
{
   IPA_DEBUG_BUS_RX                = 0x0,  /**< RX FIFO */
   IPA_DEBUG_BUS_TX                = 0x1,  /**< TX FIFO */
   IPA_DEBUG_BUS_PROC_IPH_FR_RT    = 0x2,  /**< IPH block */
   IPA_DEBUG_BUS_PROC_IPH_HDRI     = 0x3,  /**< Header insertion block */
   IPA_DEBUG_BUS_PROC_IPH_NAT      = 0x4,  /**< NAT block */
   IPA_DEBUG_BUS_PROC              = 0x5,  /**< PROC block */
   IPA_DEBUG_BUS_PROC_IPH          = 0x6,  /**< IPH block */
   IPA_DEBUG_BUS_MISC              = 0x10, /**< misc block */
   IPA_DEBUG_BUS_AHB2AHB           = 0x9,  /**< AHB2AHB wrapper */
   IPA_DEBUG_BUS_MAXI2AXI          = 0xA,  /**< MAXI2AXI wrapper */
   IPA_PROC_DCPH_TESTBUS           = 0x0B, /**< Proc DCHP testbus */
   IPA_CMDQ_ACKQ_TESTBUS           = 0x0C, /**< CMDQ ACKQ testbus */
   IPA_PROC_AGGR_TESTBUS           = 0x0D, /**< Proc Aggr testbus */
   IPA_GSI_TESTBUS                 = 0x0E, /**< GSI testbus */
   IPA_DEADBEAF                    = 0x0F, /**< Deadbeaf */
   IPA_STTS_SNIFFER_TESTBUS        = 0x10, /**< STTS sniffer testbus */
   IPA_PROC_IPH_SCND_FR_RT_TESTBUS = 0x11, /**< Proc IPH SCND FR RT testbus */
   IPA_PROC_IPH_SCND_HDRI_TESTBUS  = 0x12, /**< Proc IPH SCND HDRI testbus */
   IPA_PROC_IPH_SCND_NAT_TESTBUS   = 0x13, /**< Proc IPH SCND NAT testbus */ 
   IPA_PROC_IPH_SCND_TESTBUS       = 0x14, /**< Proc IPH SCND testbus */
   IPA_PROC_IPH_SCND_FRAG_TESTBUS  = 0x15, /**< Proc IPH SCND Frag testbus */
   IPA_PROC_UCP_TESTBUS            = 0x16, /**< Proc UCP testbus */
} VIDRV_ipa_DebugBusBlockSelectType;

/**
 * @brief   Values that represent aggregation types. 
*/
typedef enum
{
   /* 
   *  non-Generic aggregation / de-aggregation 
   */
   IPA_MBIM16    = 0x0, /**< 000 - MBIM 16-bit aggregation / deaggregation 
                                 (deaggregation uses hard coded values - backwards compatible with IPAv1) 
                            */
   IPA_MBIM32    = 0x1, /**< Deprecated. Not supported in any spec  
                            */
   IPA_TLP       = 0x2, /**< 010 - TLP aggregation / deaggregation 
                                 (deaggregation uses hard coded values - backwards compatible with IPAv1)
                            */
   /* 
   *  non-Generic de-aggregation 
   */
   IPA_RNDIS     = 0x3, /**< 011 - RNDIS deaggregation 
                                 (deaggregation uses hard coded values)
                            */
   IPA_QMAP      = 0x6,/**< 110 - QMAP deaggregation 
                                 (deaggregation uses hard coded values), was generic, which for deaggregation meant QMAP.
                            */

   /* 
   *  Generic aggregation / de-aggregation 
   */
   IPA_GENERIC   = 0x4, /**< 100 - Generic aggregation/deaggregation 
                                 (reads register parameters) - new config 
                                 (applicable for TLP, QMAP, RNDIS)
                            */
   IPA_COALESCING = 0x5, /**< 101 - Coalescing aggregation
                                 (applicable starting from IPA 4.5)
                            */
   IPA_NO_AGGR  = 0xFFFF, /**< value to define no aggregation/deaggregation */
}VIDRV_ipa_AggrType;

/**
 * @brief   Values that represent aggregation enabling types. 
*/
typedef enum
{
   IPA_BYPASS_AGGREGATION   = 0, /**< Bypass Aggregation */
   IPA_ENABLE_AGGREGATION   = 1, /**< Enable Aggregation */
   IPA_ENABLE_DEAGGREGATION = 2, /**< Enable Deaggregation */
}VIDRV_ipa_AggrEnType;

/**
 * @brief   The type represents value of different Opcode identifying 
 *          which error took place, per interrupt source.
*/
typedef enum 
{  
   /* for reg IPA_FEC_ATTR_EE_n */
   IPA_INT_SRC_OPCODE_RX_ERR_WRONG_OPCODE_RECEIVED       = 0x02,  /**< 000010 - wrong_opcode_received (RX_ERR)           */
   IPA_INT_SRC_OPCODE_RX_ERR_DE_AGGR_CONFIG_ERROR        = 0x04,  /**< 000100 - de-aggregation config error (RX_ERR)     */
   IPA_INT_SRC_OPCODE_RX_ERR_DIRECT_CONFIG_ERROR         = 0x08,  /**< 001000 - direct config error (RX_ERR)             */
   IPA_INT_SRC_OPCODE_PROC_ERR_REG_WR_IMM_CMD            = 0x0C,  /**< 001100 - REG_WR_IMM_CMD (PROC_ERR)                */
   IPA_INT_SRC_OPCODE_PROC_ERR_REG_RD_IMM_CMD            = 0x0D,  /**< 001101 - REG_RD_IMM_CMD (PROC_ERR)                */
   IPA_INT_SRC_OPCODE_PROC_ERR_SHARED_MEM_DMA_IMM_CMD    = 0x13,  /**< 010011 - SHARED_MEM_DMA_IMM_CMD (PROC_ERR)        */
   IPA_INT_SRC_OPCODE_PROC_ERR_AGGR_BYTE_LIMIT_ERROR     = 0x14,  /**< 010100 - AGGR_BYTE_LIMIT_ERROR (TX_ERR)           */
   IPA_INT_SRC_OPCODE_PROC_ERR_SECURITY_ERROR            = 0x15,  /**< 010101 - SECURITY_ERROR (TX_ERR)                */

   /* for reg IPA_MBIM_DEAGGR_FEC_ATTR_EE_n */
   IPA_INT_SRC_OPCODE_DEAGGR_ERR_MBIM                    = 0x01,  /**< 000001 - MBIM De-Aggregation error(DEAGGR_ERR)    */

   /* for reg IPA_GEN_DEAGGR_FEC_ATTR_EE_n */
   IPA_INT_SRC_OPCODE_DEAGGR_ERR_GENERIC                 = 0x21,  /**< 100001 - Generic De-Aggregation error(DEAGGR_ERR) */
}  VIDRV_ipa_IntSrcOpcodeType;

/**
 * @brief   The type represents the different fields of register IPA_UC_QMB_MISC 
*/
typedef enum
{
   IPA_UC_QMB_MISC_POSTED_STALL,
   IPA_UC_QMB_MISC_IRQ_COAL    ,
   IPA_UC_QMB_MISC_SWAP        ,
   IPA_UC_QMB_MISC_OOOWR       ,
   IPA_UC_QMB_MISC_OOORD       ,
   IPA_UC_QMB_MISC_RD_PRIORITY ,
   IPA_UC_QMB_MISC_WR_PRIORITY ,
   IPA_UC_QMB_MISC_USER        ,
} VIDRV_ipa_UcQmbMiscType;

/**
 * @brief   The type represents the different fields of register IPA_ENDP_INIT_MODE_n
*/
typedef enum
{
   IPA_ENDP_INIT_MODE_PAD_EN,
   IPA_ENDP_INIT_MODE_PIPE_REPLICATE_EN,
   IPA_ENDP_INIT_MODE_BYTE_THRESHOLD,
   IPA_ENDP_INIT_MODE_DEST_PIPE_INDEX,
   IPA_ENDP_INIT_MODE_MODE,
   IPA_ENDP_INIT_MODE_DCPH_EN
} VIDRV_ipa_EndpInitModeType;

/**
 * @brief   The type represents the different fields of register IPA_ENDP_INIT_DEAGGR_n
*/
typedef enum
{
   IPA_ENDP_INIT_DEAGGR_MAX_PACKET_LEN,
   IPA_ENDP_INIT_DEAGGR_PACKET_OFFSET_LOCATION,
   IPA_ENDP_INIT_DEAGGR_PACKET_OFFSET_VALID,
   IPA_ENDP_INIT_DEAGGR_SYSPIPE_ERR_DETECTION,
   IPA_ENDP_INIT_DEAGGR_DEAGGR_HDR_LEN
} VIDRV_ipa_EndpInitDeaggrType;

/**
 * @brief   The type represents the different fields of register IPA_ENDP_GSI_CFG2_n
*/
typedef enum
{
   IPA_ENDP_GSI_CFG2_INIT_ENDP,
   IPA_ENDP_GSI_CFG2_PUBLISH_THRESHOLD
} VIDRV_ipa_EndpGsiCfg2Type;

/**
 * @brief   The type represents the different fields of register IPA_ENDP_GSI_CFG1_n
*/
typedef enum
{
   IPA_ENDP_GSI_CFG1_ENDP_EN,
   IPA_ENDP_GSI_CFG1_CHID,
   IPA_ENDP_GSI_CFG1_EE
} VIDRV_ipa_EndpGsiCfg1Type;

/**
 * @brief   The type represents the values of a CMDQ STATUS. 
*/
typedef enum
{
   IPA_DEBUG_CMD_STATUS_ACTIVE     = 0, 
   IPA_DEBUG_CMD_STATUS_NOT_ACTIVE = 1
} VIDRV_ipa_DebugCmdStatusType;

/**
 * @brief   The type represents all the currently supported pipe interfaces.
*/
typedef enum
{
   IPA_PIPE_INTERFACE_BAM,
   IPA_PIPE_INTERFACE_GSI
} VIDRV_ipa_PipeInterfaceType;

/**
 * @brief IP version.
 */
typedef enum VIDRV_ipa_IpVersion
{
    IP_VER_INVALID = 0, /**< Invalid version */
    IP_VER_4       = 4,       /**< IPv4 */
    IP_VER_6       = 6        /**< IPv6 */
} VIDRV_ipa_IpVersion;

/**
 * @brief   List of IP protocol numbers (prefix IP_PROTO_, suffix in accordance to the protocol's keyword)
 * @note    See http://en.wikipedia.org/wiki/List_of_IP_protocol_numbers
*/
typedef enum VIDRV_ipa_IpProtocol
{
   IP_PROTO_HOPOPT      = 0x0,   /**< 0     - IPv6 Hop-by-Hop Option                              */ 
   IP_PROTO_ICMP        = 0x01,  /**< 1     - Internet Control Message Protocol                   */
   IP_PROTO_IGMP        = 0x02,  /**< 2     - Internet Group Management Protocol                  */
   IP_PROTO_IPV4        = 0x04,  /**< 4     - IPv4 (encapsulation)                                */
   IP_PROTO_TCP         = 0x06,  /**< 6     - Transmission Control Protocol                       */
   IP_PROTO_UDP         = 0x11,  /**< 17    - User Datagram Protocol                              */
   IP_PROTO_IPV6        = 0x29,  /**< 41    - IPv6 encapsulation                                  */
   IP_PROTO_IPV6_ROUTE  = 0x2B,  /**< 43    - Routing Header for IPv6                             */
   IP_PROTO_IPV6_FRAG   = 0x2C,  /**< 44    - Fragment Header for IPv6                            */
   IP_PROTO_ESP         = 0x32,  /**< 50    - Encapsulating Security Payload                      */
   IP_PROTO_AH          = 0x33,  /**< 51    - Authentication Header                               */
   IP_PROTO_IPV6_ICMP   = 0x3A,  /**< 58    - Internet Control Message Protocol for IPv6          */
   IP_PROTO_IPV6_NONXT  = 0x3B,  /**< 59    - No Next Header for IPv6                             */
   IP_PROTO_IPV6_OPTS   = 0x3C,  /**< 60    - Destination Options for IPv6                        */
   IP_PROTO_OSPF        = 0x59,  /**< 89    - Open Shortest Path First                            */
   IP_PROTO_SCTP        = 0x84,  /**< 132   - Stream Control Transmission Protocol                */
   IP_PROTO_MOBILITY    = 0x87,  /**< 135   - Mobility Extension Header for IPv6                  */

   IP_PROTO_INVALID     = 0xFF,  /**< Special key that used to sign "NON-SPECIFIED" IP Protocol,
                                    and should not be present in a IP Header                    */ 
   IP_PROTO_RESERVED    = 0xFF   /**< 255   - Reserved                                            */
} VIDRV_ipa_IpProtocol;

/**
* @brief   Values that represent type of CMDQ existing in IPA3
*/
typedef enum VIDRV_ipa_CmdqType
{
   VIDRV_IPA_CMDQ_RX_HPS,
   VIDRV_IPA_CMDQ_FIRST = VIDRV_IPA_CMDQ_RX_HPS, 
   VIDRV_IPA_CMDQ_DPS_TX,
   VIDRV_IPA_CMDQ_HPS_DPS,
   VIDRV_IPA_CMDQ_RX_SPLT,
   VIDRV_IPA_CMDQ_TX_COMMANDER,
   VIDRV_IPA_CMDQ_RX_ACKQ,
   VIDRV_IPA_CMDQ_ACKMNGR,
   VIDRV_IPA_CMDQ_PROD_ACKMNGR,
   VIDRV_IPA_CMDQ_LAST = VIDRV_IPA_CMDQ_PROD_ACKMNGR,
   /* these are not yet supported in HAL properly */
   VIDRV_IPA_CMDQ_UC_RX_HND,
   VIDRV_IPA_CMDQ_INVALID
} VIDRV_ipa_CmdqType;

/**
 * @brief   Values that represent the Rx_HPS CMDQs. 
*/
typedef enum VIDRV_ipa_RxHpsCmdqIds
{
   VIDRV_IPA_RX_HPS_CMDQ_0     = 0,        /**< Rx_HPS Command Queue 0 */
   VIDRV_IPA_RX_HPS_CMDQ_1     = 1,        /**< Rx_HPS Command Queue 1 */
   VIDRV_IPA_RX_HPS_CMDQ_2     = 2,        /**< Rx_HPS Command Queue 2 */
   VIDRV_IPA_RX_HPS_CMDQ_3     = 3,        /**< Rx_HPS Command Queue 3 */
   VIDRV_IPA_RX_HPS_CMDQ_4     = 4,        /**< Rx_HPS Command Queue 4 */
   VIDRV_IPA_RX_HPS_UC_CMDQ    = 5,        /**< Rx_HPS uC Command Queue 5 */
   VIDRV_IPA_RX_HPS_CMDQS_TOTAL_NUMBER,    /* The total number of the Rx_HPS CMDQs */          
   /* special values */
   VIDRV_IPA_RX_HPS_CMDQ_DEFAULT = VIDRV_IPA_RX_HPS_CMDQ_0 /* The default Rx_HPS CMDQ */          
} VIDRV_ipa_RxHpsCmdqIds;

/**
 * @brief   Values that represent the Rx_SPLT CMDQs. 
*/
typedef enum VIDRV_ipa_RxSpltCmdqIds
{
   VIDRV_IPA_RX_SPLT_CMDQ_0               = 0,        /**< Rx_HPS Command Queue 0 */
   VIDRV_IPA_RX_SPLT_CMDQ_1               = 1,        /**< Rx_HPS Command Queue 1 */
   VIDRV_IPA_RX_SPLT_CMDQ_2               = 2,        /**< Rx_HPS Command Queue 2 */
   VIDRV_IPA_RX_SPLT_CMDQ_3               = 3,        /**< Rx_HPS Command Queue 3 */
   VIDRV_IPA_RX_SPLT_CMDQ_4               = 4,        /**< Rx_HPS Command Queue 4 */
   VIDRV_IPA_RX_SPLT_CMDQS_TOTAL_NUMBER   = (IPA_SRC_RSRC_GRP_NUMBER - 1),    /* The total number of the Rx_SPLT CMDQs */          
   /* special values */
   VIDRV_IPA_RX_SPLT_CMDQ_DEFAULT         = VIDRV_IPA_RX_SPLT_CMDQ_0 /* The default Rx_SPLT CMDQ */          
} VIDRV_ipa_RxSpltCmdqIds;

/**
 * @brief   Values that represent an index of the IPA Source/Destination Resource Groups.
*/
typedef enum
{
   VIDRV_IPA_RSRC_GRP_0       = 0,
   VIDRV_IPA_RSRC_GRP_1       = 1,
   VIDRV_IPA_RSRC_GRP_2       = 2,
   VIDRV_IPA_RSRC_GRP_3       = 3,
   VIDRV_IPA_RSRC_GRP_4       = 4,
   VIDRV_IPA_RSRC_GRP_5       = 5,
   VIDRV_IPA_RSRC_GRP_6       = 6,
   VIDRV_IPA_RSRC_GRP_7       = 7,
   VIDRV_IPA_RSRC_GRP_NUMBER  = IPA_SRC_RSRC_GRP_NUMBER,                         /* The total number of the IPA Source/Destination Resource Groups */          
   VIDRV_IPA_RSRC_GRP_UC      = IPA_UC_RSRC_GRP_NUMBER , 
   VIDRV_IPA_RSRC_MAX_DST_GRP = IPA_MAX_USEFUL_DST_RSRC_GRP_INDEX, /* The maximum destination group index for general usage. */
   VIDRV_IPA_RSRC_MAX_SRC_GRP = IPA_MAX_USEFUL_SRC_RSRC_GRP_INDEX, /* The maximum source group index for general usage. */
   VIDRV_IPA_RSRC_MAX_GRP     = MIN(VIDRV_IPA_RSRC_MAX_SRC_GRP,VIDRV_IPA_RSRC_MAX_DST_GRP)    /* The maximum resource group index for general usage 
                                                         for tests which are designed to use the same number of source and destination groups. */
} VIDRV_ipa_ResourceGroupType;

/**
 *
 * @brief   Values that represent the commands order in CMDQ 
*/
typedef enum
{
   VIDRV_IPA_DEBUG_CMD_MISC_MID   = 0,
   VIDRV_IPA_DEBUG_CMD_MISC_FIRST = 1,
   VIDRV_IPA_DEBUG_CMD_MISC_LAST  = 2,
   VIDRV_IPA_DEBUG_CMD_MISC_ONLY  = 3
} VIDRV_ipa_DebugCmdOrderType;

/**
 * @brief   Values that represent the possible error types for a IPA driver. 
*/
typedef enum
{
  VIDRV_IPA_SUCCESS,        /**<  No errors. */
  VIDRV_IPA_NOT_SUPPORTED,  /**<  Function is not supported by the IPA core. */
  VIDRV_IPA_BAD_PARAM,      /**<  Invalid parameter value. */
  VIDRV_IPA_FAILURE         /**<  Failure occurred. */
} VIDRV_ipa_ResultType;

/*
 * Max QMB read beats parameter, since 4.0.0
 * Currently defined according to 9x75 endpoint configuration
 */
#define IPA_QMB_0_MAX_READ_BEATS 120
#define IPA_QMB_1_MAX_READ_BEATS 0

#ifdef WIN32
#pragma pack(push,1) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   WDI peer metadata format
 */
typedef PACK(union)
{
   uint32 raw;
   PACK(struct) peerMetaData
   {
      uint32 sw_peer_id : 16;
      uint32 vap_id     : 3;
      uint32 reserved   : 13;
   } peerMetaData;
} VIDRV_WDI_PEER_META_DATA;

#ifdef WIN32
#pragma pack(pop) /* Save previous, and turn on 1 byte alignment */
#endif

/**
 * @brief   Data transfer information. 
*/
typedef struct 
{
    void      *vaddr;       /**< Buffer virtual address */
    uint64     dma_addr;    /**< Buffer physical address */
    uint32     size;        /**< Buffer size */
    void      *cookie;      /**< Buffer internal information */
    /* WDI specific data */
    uint32 sw_buffer_cookie : 21; /**< The Sw_buffer_cookie */
    uint32 da_is_MCBC : 1;  /**< Indicates the DA address was a Multicast or Broadcast address for this MSDU */
    VIDRV_WDI_PEER_META_DATA peer_meta_data;  /**< vdev_id /  vap_id is 3 bit value within this field */
#ifdef  IPA_DBG_MEMORY_CORRUPTIONS
    /* private for memory corruption sanity check functions */
    uint32    *pMarkerBeforeBuffer;
    uint32    *pMarkerAfterBuffer;
    uint32    originalSize;
    void      *originalVaddr;
#endif
} VIDRV_ipa_BufferType;

/** 
  * @brief Handle of a pipe 
  */
typedef struct IpaPipeType                                     * VIDRV_ipa_PipeHandle;

/** 
  * @brief Handle to an header entry
  */
typedef struct VIDRV_ipa_HeaderEntryStruct                     * VIDRV_ipa_HeaderHandle;


/** 
  * For compilation with USB project without BAM driver
  */
#ifndef _VVDRV_BAM_H_

#define BAM_PIPE_NUMBER_AUTO    (0xFFu)

#define HAL_ENUM_32BITS(x)              HAL_##x##_FORCE32BITS = 0x7FFFFFFF

/* offset of flags field in the descriptor */
#define HAL_BAM_DESC_FLAG_OFFST         24

/** 
 * @brief   Pipe Descriptor Flags.   
 */ 
typedef enum
{
    HAL_BAM_PIPE_DESC_FLAG_IMM  = 0x01000000,    /* Immediate. Only BAM-NDP and cannot be used with CMD */
    HAL_BAM_PIPE_DESC_FLAG_ULK  = 0x02000000,    /* Unlock */
    HAL_BAM_PIPE_DESC_FLAG_LCK  = 0x04000000,    /* Lock */
    HAL_BAM_PIPE_DESC_FLAG_CMD  = 0x08000000,    /* Command descriptor */
    HAL_BAM_PIPE_DESC_FLAG_NWD  = 0x10000000,    /* Notify-when-done */
    HAL_BAM_PIPE_DESC_FLAG_EOB  = 0x20000000,    /* End-Of-Block Notification */
    HAL_BAM_PIPE_DESC_FLAG_EOT  = 0x40000000,    /* End-Of-Transfer Notification */
    HAL_BAM_PIPE_DESC_FLAG_INT  = (int32)0x80000000,    /* Interrupt on Notification */
    HAL_ENUM_32BITS(BAM_PIPE_DESC_FLAG)
} HAL_bam_PipeDescFlagType;
 
/** 
 * @brief   Pipe direction .  
 */ 
typedef enum
{
   HAL_BAM_PIPE_CONSUMER = 0,          /* The Pipe Reads data from data-fifo or system-memory */
   HAL_BAM_PIPE_PRODUCER = 0x00000008, /* The Pipe Writes data to data-fifo or system-memory */
   HAL_ENUM_32BITS(BAM_PIPE_DIR)
} HAL_bam_PipeDirType;

/**
 * @enum BAM_DRV_PipeStatusType
 *
 * @brief   Values that represent BAM_DRV_PipeStatusType. 
*/
typedef enum BAM_DRV_PipeStatusType
{
    PIPE_HRESP_ERR_STATUS        = 0x80 ,   // Same as BAM_IRQ_HRESP_ERR_IRQ, only now the interrupts is per pipe - for security reasons.  
    PIPE_RESET_ERR_STATUS        = 0x40 ,   // Same as BAM_IRQ_ERROR_IRQ, only now the interrupts is per pipe - for security reasons. 
    PIPE_TRANSFER_END_STATUS     = 0x20,    // For producer this interrupt happens whenever the last AHB burst of a transfer happens 
                                            // For Consumer when ack_on_success closes a transfer.
    PIPE_ERR_STATUS              = 0x10,    // Pipe Error interrupt indicates that an error has happened. TBD: Error reason is not yet defined. Interrupt not in use.
    PIPE_OUT_OF_DESC_STATUS      = 0x8,     // When no descriptors is available for pipe to process (consumer - to read, producer - to store data)
    PIPE_WAKE_STATUS             = 0x4,     // Wake peripheral interrupt signals the driver to wake up the peripheral (USB for example) for transmission via the current pipe. 
                                            // This interrupt is issued whenever an Event comes to the specific pipe.
    PIPE_TIMER_STATUS            = 0x2,     // Inactivity timer has reached its threshold value Interrupt
    PIPE_DESC_STATUS             = 0x1,     // This Interrupt rises when BAM finishes processing an IO Vector which has INT bit selected
    PIPE_STATUS_ALL              = (PIPE_HRESP_ERR_STATUS | PIPE_RESET_ERR_STATUS | PIPE_TRANSFER_END_STATUS| PIPE_ERR_STATUS | PIPE_OUT_OF_DESC_STATUS | PIPE_WAKE_STATUS | PIPE_TIMER_STATUS| PIPE_DESC_STATUS),
    PIPE_STATUS_NONE             = 0
} BAM_DRV_PipeStatusType;

/**
 * @brief   Descriptor flags. 
 */
typedef enum BAM_DRV_PipeDescFlags
{
    PIPE_DESC_FLAG_NONE     = 0,
    PIPE_DESC_FLAG_NDP_IMM  = HAL_BAM_PIPE_DESC_FLAG_IMM >> HAL_BAM_DESC_FLAG_OFFST,    /* Immediate. Only BAM-NDP and cannot be used with CMD */
    PIPE_DESC_FLAG_LOCK     = HAL_BAM_PIPE_DESC_FLAG_LCK >> HAL_BAM_DESC_FLAG_OFFST,    /* Lock BAM */
    PIPE_DESC_FLAG_UNLOCK   = HAL_BAM_PIPE_DESC_FLAG_ULK >> HAL_BAM_DESC_FLAG_OFFST,    /* Unlock BAM */
    PIPE_DESC_FLAG_CMD      = HAL_BAM_PIPE_DESC_FLAG_CMD >> HAL_BAM_DESC_FLAG_OFFST,    /* Command descriptor */
    PIPE_DESC_FLAG_EOB      = HAL_BAM_PIPE_DESC_FLAG_EOB >> HAL_BAM_DESC_FLAG_OFFST,    /* End-Of-Block Notification */
    PIPE_DESC_FLAG_EOT      = HAL_BAM_PIPE_DESC_FLAG_EOT >> HAL_BAM_DESC_FLAG_OFFST,    /* End-Of-Transfer Notification */
    PIPE_DESC_FLAG_INT      = ((uint32)HAL_BAM_PIPE_DESC_FLAG_INT) >> HAL_BAM_DESC_FLAG_OFFST, 
    PIPE_DESC_FLAG_EOT_NWD = (HAL_BAM_PIPE_DESC_FLAG_NWD | HAL_BAM_PIPE_DESC_FLAG_EOT) >> HAL_BAM_DESC_FLAG_OFFST,        /* Notify-when-done */
    PIPE_DESC_FLAG_CMD_NWD = (PIPE_DESC_FLAG_EOT_NWD | PIPE_DESC_FLAG_CMD),
} BAM_DRV_PipeDescFlags;

/**
 * @brief   Pipe direction. 
 */
typedef enum BAM_DRV_PipeDir
{
    PIPE_DIR_CONSUMER = HAL_BAM_PIPE_CONSUMER,    /* The Pipe Reads data from data-fifo or system-memory */
    PIPE_DIR_PRODUCER = HAL_BAM_PIPE_PRODUCER,    /* The Pipe Writes data to data-fifo or system-memory */
    PIPE_DIR_UNCONFIGURED                         /* special case, when PipeAlloc must allocate pipes for BAM-2-BAM mode */
} BAM_DRV_PipeDir;
#endif // _VVDRV_BAM_H_

/**
 * @brief   The type represents inline IC mode.
*/
typedef enum
{
   VIDRV_IPA_INLINE_IMM_CMD_RANDOM       = 0x0, /* Random usage of inline ICs, if declared */   
   VIDRV_IPA_INLINE_IMM_CMD_DISABLE      = 0x2, /* Disabling the usage of inline ICs */   
   VIDRV_IPA_INLINE_IMM_CMD_ENABLE       = 0x1, /* Enabling the usage of inline ICs, if declared */   
} VIDRV_ipa_InlineImmCmdModeType;

/**
 * @brief   Immediate command common parameters. 
*/
typedef  struct
{
   VIDRV_ipa_PipeHandle             hPipe;               /**< The consumer pipe used for sending the command. 
                                                               Specify NULL to send command through system pipe. */
   BAM_DRV_PipeDescFlags            flags;               /**<  flags The BAM descriptor flags */
   boolean                          skipPipeStart;       /**<  0 - Start the transfer by the Imm.Cmd., 
                                                               1 - Skip the start of the transfer by the Imm.Cmd. */
   boolean                          skipWait;            /**<  0 - Wait for completion
                                                               1 - Skip waiting for completion */
   VIDRV_ipa_InlineImmCmdModeType   enableInlineImmCmd;  /**<  in accordance with VIDRV_ipa_InlineImmCmdModeType */
} VIDRV_ipa_ImmediateCommandCommonParamsType;


/**
 * @brief   This value is used as base-Opcode to declare INLINE Immediate Commands. 
            This value MUST be a power of 2
*/
#define IPA_INLINE_IMM_CMD_BASE      (128)

/**
 * @brief   Width of address 32 bit or 64 bit. 
 */
typedef enum VIDRV_ipa_AddressWidth
{
   IPA_ADDR_32BIT = 0,
   IPA_ADDR_64BIT = 1,   
} VIDRV_ipa_AddressWidth;

/**
 * @brief   Pipeline clear options
 */
typedef enum VIDRV_ipa_PipelineClearOptions
{
   IPA_PIPELINE_CLEAR_HPS           = 0,        /**<    HPS - no packets inside HPS (not group specific)            */
   IPA_PIPELINE_CLEAR_SOURCE_GROUP  = 1,        /**<    Source Group's no packet contexts - The IC's source group  
                                                        does not use any packet contexts 
                                                        (no packets of the IC's source group 
                                                        are inside HPS + HPS-DPS queues + DPS + DPS-TX queues + TX) */
   IPA_PIPELINE_CLEAR_HPS_DPS       = 2,        /**<    Wait until no packets reside inside IPA pipeline
                                                        (HPS, HPS-DPS queues, DPS, DPS-TX queues, TX).              */
   IPA_PIPELINE_CLEAR_RESERVED      = 3         /**<    Reserved                                                    */
} VIDRV_ipa_PipelineClearOptions;

/**
 * @brief Values that represent the IPA DMA_TASK immediate command flags. Values may be used as mask.
 * @sa VIDRV_ipa_StartDmaTask
 * @remark [Immediate Commands.xlsx]."DMA_TASK"
 * @note values of enumerator shall not be changed since it was taken from arch. document 
*/
typedef enum VIDRV_ipa_DmaTaskFlagsType
{
   VIDRV_IPA_DMA_TASK_FLAG_NONE   = 0x00, /**< No flag */
   VIDRV_IPA_DMA_TASK_FLAG_CMPLT  = 0x01, /**< When asserted IPA generates EOT on the source, closing aggregation */
   VIDRV_IPA_DMA_TASK_FLAG_EOF    = 0x02, /**< When set IPA will assert the BAM EOT to the destination client. This is used for SW based aggregation */
   VIDRV_IPA_DMA_TASK_FLAG_FLSH   = 0x04, /**< When asserted, packet will go through the IPA blocks but will not be transferred to destination client */
} VIDRV_ipa_DmaTaskFlagsType;

/**
 * @brief   The type represents value of the field "direction" in IPA Immediate Commands.
*/
typedef enum 
{  
   VIDRV_IPA_IMM_CMD_DIRECTION_WRITE = 0, /**< 0 - IPA Write, Write to   Local_Addr from System_Addr (IC DMA_SHARED_MEM ) */
   VIDRV_IPA_IMM_CMD_DIRECTION_READ  = 1  /**< 1 - IPA Read , Read  from Local_Addr to   System_Addr (IC DMA_SHARED_MEM ) */
}  VIDRV_ipa_ImmCmdDirectionType;

/**
 * @brief Values that represent available algorithms for LTE ciphering. 
 * @sa VIDRV_ipa_DecipherSet
 * @remark [Immediate Commands.xlsx]."LTE_DECIPH_INIT"."Algorithm" (3 bits)
*/
typedef enum VIDRV_ipa_CipherAlgorithmType
{
   VIDRV_IPA_DECIPHER_ALGO_AES      = 0x0,   /**< AES decryption */
   VIDRV_IPA_DECIPHER_ALGO_ZUC      = 0x1,   /**< ZUC decryption */
   VIDRV_IPA_DECIPHER_ALGO_SNOW_3G  = 0x2,    /**< SNOW 3G decryption */
   VIDRV_IPA_DECIHPER_ALGO_END      = VIDRV_IPA_DECIPHER_ALGO_SNOW_3G   /**< Max type for algorithms */
} VIDRV_ipa_CipherAlgorithmType;

/**
 * @brief Values that represent the stages in IPA pipe that packet, following IP_PACKET_INIT_EX command, shall skip. <br>
 * Values may be used as mask.
 * @sa VIDRV_ipa_IpPacketInitEx
 *  
*/
typedef enum VIDRV_ipa_IpPacketInitExActionsType
{
   VIDRV_IPA_PACKET_INIT_ACTION_EMPTY           = 0x00, /**< there is no skipped blocks */
   VIDRV_IPA_PACKET_INIT_ACTION_FRAG_DISABLE    = 0x01, /**< Skip fragment handling block */
   VIDRV_IPA_PACKET_INIT_ACTION_FILTER_DISABLE  = 0x02, /**< Skip filtering block */
   VIDRV_IPA_PACKET_INIT_ACTION_NAT_DISABLE     = 0x04, /**< Skip NAT block */
   VIDRV_IPA_PACKET_INIT_ACTION_ROUTE_DISABLE   = 0x08, /**< Skip Routing block */
   VIDRV_IPA_PACKET_INIT_ACTION_HDR_RI_DISABLE  = 0x10, /**< Skip Header Removal, Insertion and Retaining. */
   VIDRV_IPA_PACKET_INIT_ACTION_CS_DISABLE      = 0x20, /**< Skip Checksum offload block. Checksum trailer will not be added to the packet. */
   VIDRV_IPA_PACKET_INIT_ACTION_STAT_DISABLE    = 0x40, /**< Skip Quota/Tethering Statistics collection. */
   VIDRV_IPA_PACKET_INIT_ACTION_DISABLE_ALL     = 0x7F, /**< all blocks are skipped */
} VIDRV_ipa_IpPacketInitExActionsType;

/**
 * @brief   Values that represent the filtering actions. 
*/
typedef enum VIDRV_ipa_FilterActionType
{
   VIDRV_IPA_FILTER_ACTION_PASS_TO_ROUTING               = 0x0,   /**< Pass to routing action */
   VIDRV_IPA_FILTER_ACTION_PASS_TO_SRC_NAT               = 0x1,   /**< Pass to src NAT action */
   VIDRV_IPA_FILTER_ACTION_PASS_TO_OUTBOUND_CONN_TRACK   = VIDRV_IPA_FILTER_ACTION_PASS_TO_SRC_NAT,   /**< Pass to outbound connection tracking action */
   VIDRV_IPA_FILTER_ACTION_PASS_TO_DST_NAT               = 0x2,   /**< Pass to dst NAT action */
   VIDRV_IPA_FILTER_ACTION_PASS_TO_INBOUND_CONN_TRACK    = VIDRV_IPA_FILTER_ACTION_PASS_TO_DST_NAT,   /**< Pass to inbound connection tracking action */
   VIDRV_IPA_FILTER_ACTION_EXCEPTION_PACKET              = 0x3,   /**< Pass to exception (notification) pipe */
   VIDRV_IPA_FILTER_ACTION_NUMBERS                       /**< the number of IPA Filter ACTION options */
} VIDRV_ipa_FilterActionType;


/**
 * @brief   DMA_TASK and DMA_TASK_32BIT_ADDR Immediate command parameters. 
*/
typedef  struct
{
   VIDRV_ipa_BufferType*      bufferArray;      /**< array of buffers to send with current dma task command         */
   uint32                     bufferArraySize;  /**< number of buffers to send with current dma task command        */
   boolean                    isFirstCommand;   /**< indication whether current dma task command is the first in a raw*/
   boolean                    isLastCommand;    /**< indication whether current dma task command is the last in a raw. 
                                                     In such case EOT would be sent                                 */
   boolean                    setSctBit;        /**< indication whether current dma task command should be issued with SCT bit
                                                     set (relevant only for GSI mode, in BAM mode this setting will have no effect. */
   uint32                     packetSize;       /**< Total packet size. If a packet is sent using multiple DMA_TASKs, 
                                                     this field shall summarize transfer size and remain the same for all dma tasks.
                                                     Value must not exceed IPA_MAX_PACKET_SIZE_IN_DMA_TASK          */
   VIDRV_ipa_DmaTaskFlagsType flags;            /**< The flags used for DMA_TASK immediate command. 
                                                     User may specify value as mask. 
                                                     If a packet is sent using multiple DMA_TASKs, 
                                                     this field shall remain the same for all dma tasks             */
   VIDRV_ipa_BufferType       cmdBuffer;        /**< Buffer allocated by user used for holding the immediate command. 
                                                     Buffer must be aligned to IPA_IM_ALIGN and its size shall be IPA_DMA_TASK_FRAME_HEADER_SIZE*bufferArraySize bytes. 
                                                     Buffer can be released once IPA consumed the dma task command  */
   boolean                    skipPipeStart;    /**< 0 - Start the transfer by the Imm.Cmd., 
                                                     1 - Skiping the start of the transfer by the Imm.Cmd.          */
   VIDRV_ipa_AddressWidth     addrWidth;        /**< Address width. DMA_TASK Imm.Cmd. is for 64bit address width
                                                     and DMA_TASK_32B_ADDR is for 32bit address width               */

} VIDRV_ipa_DmaTaskParamsType;

/**
 * @brief   IP PACKET INIT Immediate command parameters. 
 * @sa VIDRV_ipa_IpPacketInit
*/
typedef  struct
{
   VIDRV_ipa_PipeHandle hDstPipe;   /**< Handle to the destination pipe */
   VIDRV_ipa_BufferType cmdBuffer;  /**< Buffer allocated by user used for holding the immediate command. Buffer must be aligned to IPA_IM_ALIGN and its size shall be IPA_IP_PACKET_INIT_IMM_CMD_SIZE. 
                                         Buffer can be released once IPA consumed the dma task command */
} VIDRV_ipa_IpPacketInitParamsType;


/**
 * @brief   LTE DECIPH CONFIG Immediate command parameters. 
 * @sa VIDRV_ipa_DecipherConfigPacket
*/
typedef  struct
{
   uint32 contextIndex;             /**<  A key index to use. It is relevant since IPA3.1 */
   uint32 size;                     /**<  Number of bytes to decrypt */
   uint32 count;                    /**<  Cipher related parameter */
   VIDRV_ipa_BufferType cmdBuffer;  /**< Buffer allocated by user used for holding the immediate command. Buffer must be aligned to IPA_IM_ALIGN and its size shall be IPA_LTE_DECIPH_CONFIG_IMM_CMD_SIZE. 
                                         Buffer can be released once IPA consumed the dma task command */
} VIDRV_ipa_DecipherConfigPacketParamsType;

/**
 * @brief   IP PACKET TAG Status Immediate command  
*/
typedef  struct
{
   uint16 tag[3];                      /**< The tag */
   VIDRV_ipa_BufferType cmdBuffer;  /**< Buffer allocated by user used for holding the immediate command. Buffer must be aligned to IPA_IM_ALIGN and its size shall be IPA_IP_PACKET_TAG_IMM_CMD_SIZE.
                                         Buffer can be released once IPA consumed the dma task command */
} VIDRV_ipa_IpPacketTagStatusParamsType;

/**
 * @brief   REGISTER_WRITE Immediate command parameters. 
 * @sa      VIDRV_ipa_ImmCmdRegisterWrite
*/
typedef  struct
{
   uint16                           reservedForSw;          /**<    Ignored by HW. May be used by SW.                              */
   boolean                          skipPipelineClear;      /**<    0: Wait until IPA pipeline is clear (default), 
                                                                    1: Don't wait until IPA pipeline is clear.                     */
   VIDRV_ipa_PipelineClearOptions   pipelineClearOptions;   /**<    Pipeline clear options. For more details please, 
                                                                    look at VIDRV_ipa_PipelineClearOptions enum definition         */
   uint16                           offset;                 /**<    Offset from IPA base address - 
                                                                    lower 16 bits of the IPA Register address.                     */
   uint32                           value;                  /**<    Value to write to register.                                    */
   uint32                           bitEnable;              /**<    Mask specifying which value bits to write to the register.     
                                                                    Only enabled bits (set to 1) will be written.                  */
   VIDRV_ipa_BufferType             cmdBuffer;              /**<    Buffer should be allocated by user and is used to hold the Immediate Command,
                                                                    when descriptor's flag EOT is NOT set for the pipe. 
                                                                    Buffer MUST be aligned to IPA_IM_ALIGN and its size shall be IPA_REGISTER_WRITE_IMM_CMD_SIZE. 
                                                                    Buffer can be released once IPA consumed the immediate command sequence with EOT at end.
                                                                                                                                   */
   uint16                           offsetHigh;             /**<    High bits of the Offset field - 
                                                                    bits 17-20 of the IPA register address. 4 bits.                */
} VIDRV_ipa_RegisterWriteParamsType;

/**
 * @brief   REGISTER_READ and REGISTER_READ_32BIT_ADDR Immediate commands parameters. 
 * @sa      VIDRV_ipa_ImmCmdRegisterRead
*/
typedef  struct
{
    uint16                          reservedForSw;          /**<    Ignored by HW. May be used by SW.                               */
    boolean                         skipPipelineClear;      /**<    0: Wait until IPA pipeline is clear (default), 
                                                                    1: Don't wait until IPA pipeline is clear.                      */
    VIDRV_ipa_PipelineClearOptions  pipelineClearOptions;   /**<    Pipeline clear options. For more details please, 
                                                                    look at VIDRV_ipa_PipelineClearOptions enum definition          */
    uint16                          offset;                 /**<    Offset from IPA base address - 
                                                                    lower 16 bits of the IPA Register address.                      */
    uint64                          systemAddress;          /**<    Address in system memory for storing register value.            */
    VIDRV_ipa_BufferType            cmdBuffer;              /**<    Buffer should be allocated by user and is used to hold the Immediate Command,
                                                                    when descriptor's flag EOT is NOT set for the pipe. 
                                                                    Buffer MUST be aligned to IPA_IM_ALIGN and its size shall be IPA_REGISTER_READ_IMM_CMD_SIZE. 
                                                                    Buffer can be released once IPA consumed the immediate command sequence with EOT at end.*/
    VIDRV_ipa_AddressWidth          addrWidth;              /**<    Address width. DMA_TASK Imm.Cmd. is for 64bit address width
                                                                    and DMA_TASK_32B_ADDR is for 32bit address width                */
    uint16                          offsetHigh;             /**<    High bits of the Offset field - 
                                                                    bits 17-20 of the IPA register address. 4 bits.                 */
} VIDRV_ipa_RegisterReadParamsType;


/**
 * @brief   DMA_SHARED_MEM Immediate command parameters. 
 * @sa      VIDRV_ipa_ImmCmdDmaSharedMem
*/
typedef  struct
{
   uint16                           reservedForSw;          /**<    Ignored by HW. May be used by SW.                              */
   boolean                          clearAfterRead;         /**<    Since IPA 4.0, Clear local memory at the end of a read operation
                                                                    allows atomic read and clear if HPS is clear. Ignore for writes.
                                                                                                                                   */
   uint16                           size;                   /**<    Size in bytes of data to copy. 
                                                                    For IPAv2, expected size is up to 2K bytes.                    */
   uint64                           systemAddr;             /**<    Address in system memory                                       */
   uint16                           localAddr;              /**<    Address in IPA local memory                                    */
   VIDRV_ipa_ImmCmdDirectionType    direction;              /**<    0 - IPA Write, Write to   Local_Addr from System_Addr
                                                                    1 - IPA Read , Read  from Local_Addr to   System_Addr          */
   boolean                          skipPipelineClear;      /**<    0: Wait until IPA pipeline is clear (default), 
                                                                    1: Don't wait until IPA pipeline is clear.                     */
   VIDRV_ipa_PipelineClearOptions   pipelineClearOptions;   /**<    Pipeline clear options. For more details please, 
                                                                    look at VIDRV_ipa_PipelineClearOptions enum definition         */
   VIDRV_ipa_BufferType             cmdBuffer;              /**<    Buffer should be allocated by user and is used to hold the 
                                                                    Immediate Command, when descriptor's flag EOT is NOT set for the pipe. 
                                                                    Buffer MUST be aligned to IPA_IM_ALIGN and its size shall be IPA_REGISTER_WRITE_IMM_CMD_SIZE. 
                                                                    Buffer can be released once IPA consume the immediate command sequence with EOT at end.
                                                                                                                                   */
} VIDRV_ipa_DmaSharedMemParamsType;


/**
 * @brief   IPA decipher initialization parameters. 
 * @sa VIDRV_ipa_DecipherSet
*/
typedef  struct
{
   uint32 contextIndex; /**< Cipher/Decipher configuration index. It is relevant since IPA3.1 */
   uint16 keystrmOffset;/**< Generate and ignore keystream up to this value in bytes before starting deciphering.
                             0 means start from the first keystream-byte. Bytes before this offset are assumed to be 
                             deciphered outside of IPA (for example: SW)thus the key is manipulated as those bytes where deciphered. */ 
   uint16 startOffset;  /**< Offset from which to start packet buffer deciphering.  
                             IPA would produced clear output starting from this offset. Value shall not exceed IPA_MAX_CIPHER_OFFSET_START*/
   VIDRV_ipa_CipherAlgorithmType algorithm; /**< The ciphering algorithm*/
   uint32 keyIndex;     /**< Index to 128-bit (AES, ZUC) /32-bit (Snow 3G)
                             confidentiality key stored in the confidentiality key cache. 0 means first key in LTE keys table. 
                             Value shall not exceed IPA_MAX_CIPHER_KEY_INDEX. */
   uint8 bearer;        /**< The bearer - this is ciphering related parameter*/

   VIDRV_ipa_BufferType cmdBuffer;  /**< Buffer allocated by user used for holding the immediate command. Buffer must be aligned to IPA_IM_ALIGN and its size shall be IPA_LTE_DECIPH_INIT_IMM_CMD_SIZE. 
                                         Buffer can be released once IPA consumed the dma task command */
} VIDRV_ipa_DecipherSetParamsType;

/**
 * @brief   IP_PACKET_INIT_EX immediate command parameters. 
 * @sa VIDRV_ipa_IpPacketInitEx
*/
typedef  struct
{
   VIDRV_ipa_IpPacketInitExActionsType actionType; /**< specify the stages in IPA pipe that the consecutive packet shall skip. Values may be used as mask */

   /**
    * @brief   output of filtering, applicable only if the filtering is disabled 
   */
   struct filterOutputStruct{                           
      VIDRV_ipa_FilterActionType          action;        /**< filter action as would indicated from filtering rule */
      uint16                              routeTableId;  /**< index of Routing table as would indicated from filtering rule   */
      boolean                             retainHeader;  /**< is used to add back to the packet the header removed as part of header removal. This will be done as part of header insertion block. */
      uint16                              priority;      /**< the rule priority */
      uint16                              ruleId;        /**< the ruke ID */      

   } filterOutput;                                       /**< Valid only if FILTER_DISABLE=1 */

   /**
    * @brief   output of routing block, applicable only if the routing block is disabled.
   */
   struct routeOutputStruct{                  
      VIDRV_ipa_PipeHandle                hDstPipe;      /**< destination pipe as would indicated from Routing rule */
      VIDRV_ipa_HeaderHandle              hHdr;          /**< Header for insertion as would indicated from Routing rule */
      uint16                              priority;      /**< the rule priority */
      uint16                              ruleId;        /**< the ruke ID */      
   } routeOutput;                                        /**< Valid only if ROUTE_DISABLE=1 */

   VIDRV_ipa_BufferType cmdBuffer;     /**<  Buffer allocated by user and is used to hold the immediate command.
                                             Buffer must be aligned to IPA_IM_ALIGN and 
                                             its size shall be IPA_IP_PACKET_INIT_EX_IMM_CMD_SIZE. 
                                             Buffer can be released once IPA consumed the immediate command sequence with EOT at end.
                                       */
}VIDRV_ipa_IpPacketInitExParamsType;



#ifdef __cplusplus
}
#endif

#endif /* IPA_DEFS_H */

