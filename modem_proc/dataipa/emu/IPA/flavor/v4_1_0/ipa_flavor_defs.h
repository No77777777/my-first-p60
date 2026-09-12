#ifndef IPA_FLAVOR_DEFS_H
#define IPA_FLAVOR_DEFS_H


/**---------------------------------------------------------------------------
 * @file     ipa_flavor_defs.h
 *
 * @brief    IPA definitions per specific IPA core flavor (version 4.0.0).
 * 
 * Copyright (c) 2016 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"
#include "ipa_flavor_customer.h"

/** The actual maximum transfer size-  Test "MAXIMAL CREDIT FOR BAM DESC" should be run to approve this value */
#define IPA_MAX_TRANSFER_SIZE       0x7FFF /* = 32KB - 1 = (32*1024 - 1)) */

/** The actual maximum frame size */
#define IPA_MAX_AGGR_FRAME_SIZE     0xFFFF /* = 64KB - 1 = (64*1024 - 1)) */

/**
 * @brief   maximum interrupt counter coalescing threshold. 
*/
#define IPA_MAX_INTERRUPT_COUNTER_COALESCING_THRESHOLD 255

/**
 * @brief   maximum interrupt timer coalescing threshold. 
*/
#define IPA_MAX_INTERRUPT_TIMER_COALESCING_THRESHOLD   255

/**
 * @brief   Maximum HLOB timer value. 
*/
#define IPA_MAX_HLOB_TIMER_VALUE 0xFFFFFFFF

/**
 * @brief   Maximum offset parameter for ciphering
*/
#define IPA_MAX_CIPHERING_OFFSET                       20

/**
 * @brief   Maximum offset parameter for deciphering
*/
#define IPA_MAX_DECIPHERING_OFFSET                     8

/**
 * @brief   IPA clock in MHz
*/
#define IPA_CLK   (250)

/**
 * @brief   The offset (for right shift purpose) of flags field in the entry of DPS sequencer table. 
*/
#define  IPA_ENTRY_FLAGS_OFFSET        4

/**
 * @brief   TLV FIFO size when working with GSI. 
*/
#define IPA_GSI_OVERALL_TLV_FIFO_SIZE 250

/**
 * @brief   AOS FIFO size when working with GSI. 
*/
#define IPA_GSI_OVERALL_AOS_FIFO_SIZE 340

/**
 * @brief   Size of RAM for the uC to use
*/
#define IPA_RAM_UC_MEM_SIZE    (0)

/** 
 * @brief   The number of source and destination resource groups
*/
#define IPA_SRC_RSRC_GRP_NUMBER  4
#define IPA_DST_RSRC_GRP_NUMBER  4
#define IPA_UC_RSRC_GRP_NUMBER   3

/** 
 * @brief   The max useful index of source/destination resource groups
 *
 * @note    These depend not only on IPA HW configuration but also on current resource group configuration
*/
#define IPA_MAX_USEFUL_SRC_RSRC_GRP_INDEX  1
#define IPA_MAX_USEFUL_DST_RSRC_GRP_INDEX  3

/**
 * @brief   Currently unutilized GSI shared RAM region that can be used for testing purposes (the offset is in dwords).
*/
#define GSI_UNUSED_SHRAM_OFFSET     (0xBA0/sizeof(uint32))

/** 
 * @brief   GSI peripheral base address mask (discards the lowest 19 bits of the address).
*/
#define GSI_PERIPH_BASE_ADDR_MASK  (0xfffffffffff80000L)

/**
 * @brief   Size of a Packet CONTEXT.
*/
#define IPA_PACKET_CONTEXT_SIZE_IN_BYTES        0x80

/**
 * @brief   Maximal ID of a Packet CONTEXT
*/
#define IPA_MAX_PACKET_CONTEXT_ID               9

/**
 * @brief   Offset (in RAM_HW) of the first Packet CONTEXT (offset from IPA_RAM_HW_FIRST)
*/
#define IPA_PACKET_CONTEXT_OFFSET_IN_RAM_HW     0x2B00                                                      

/**
* @brief   Start address of first sector, offset from the start of HW RAM (IPA_RAM_HW_FIRST)
*/
#define IPA_START_ADDRS_FIRST_SECTOR_IN_SRAM 0x3000

/**
* @brief   First index of sector list in the resource id selector (48-63 for sector lists and 0-47 for descriptor lists) 
*/
#define IPA_RESOURCE_ID_SELECTOR_SECTOR_LIST_INDEX_FIRST 48

/**
* @brief   The size of a full sector
*/
#define IPA_FULL_SECTOR_SIZE  512

/**
* @brief   The maximum context index for ciphering/deciphering immediate commands
*/
#define IPA_MAX_CIPHER_DECIPHER_CONTEXT_INDEX 3

/*
 * @brief   Size of uc IRAM size.
*/
#define IPA_UC_IRAM_SIZE 0x8000

/*
 * @brief   Size of uc DRAM size.
*/
#define IPA_UC_DRAM_SIZE 0x1E00

/** 
* @brief	Address offset in UC QMB space.
*/
#define RAM_UC_IRAM           0x10000

/**
* @brief BUTTOM value for IRAM reading is its first address.
*/
#define IRAM_BOTTOM_READ_OFFSET  0x0

/**
* @brief TOP value for IRAM reading is its last address minus 0x100 because of DMA buffer size.
*/
#define IRAM_TOP_READ_OFFSET     0x7FFC

/**
* @brief BOTTOM value for DRAM reading is its first address.
*/
#define DRAM_BOTTOM_READ_OFFSET  0x8300

/**
* @brief TOP value for DRAM reading is its last address minus 0x100 for DMA minus 0x200 because of the stack.
*/
#define DRAM_TOP_READ_OFFSET     0x9DFC

/**
* @brief BUTTOM value for IRAM writing is its first address after the code plus some extra (QMB address).
*/
#define IRAM_BOTTOM_WRITE_OFFSET 0x3600

/**
* @brief TOP value for IRAM writing is its last address minus 0x100 because of DMA buffer size (QMB address).
*/
#define IRAM_TOP_WRITE_OFFSET    0x7FFC

/**
* @brief BOTTOM value for DRAM writing is its first address after the code and data plus some extra.
*/
#define DRAM_BOTTOM_WRITE_OFFSET  0x9800

/**
* @brief TOP value for DRAM writing is its last address minus 0x100 for DMA minus 0x200 because of the stack.
*/
#define DRAM_TOP_WRITE_OFFSET     0x9DFC

/*
* @brief   Maximum allowed timer id.
*/
#define IPA_UC_MAX_TIMER_ID 0x4

/*
 * @brief   The default configuration of BCR bits
*/
#define IPA_DEFAULT_BCR_CONFIGURATION (HAL_IPA_BCR_CMDQ_L_LACK_ONE_ENTRY | HAL_IPA_BCR_SUSPEND_L2_IRQ | HAL_IPA_BCR_HOLB_DROP_L2_IRQ | HAL_IPA_BCR_DUAL_TX)

/**
* @brief   the maximum Number of pipes per direction available.
*
* @note    The number of useful pipes is limited by number of GSI channels per EE
*          Please use IPA_CONFIG_GSI_CH_ID_MAPPING_MODE_BALANCED
*/
#define IPA_NUM_OF_PIPES_PER_DIRECTION MIN(MIN(IPA_PIPE_CONSUMER_MAX_ID-IPA_PIPE_CONSUMER_MIN_ID, IPA_PIPE_PRODUCER_MAX_ID-IPA_PIPE_PRODUCER_MIN_ID+1), (IPA_MAX_NUM_OF_GSI_CHANNELS_PER_EE - 1)/2)

/**
* @brief   the maximal number of MBIM aggregation pipes.
*/
#define IPA_NUM_OF_MBIM_AGGREGATION_PIPES 4

/**
* @brief   the maximal number of generic aggregation pipes.
*/
#define IPA_NUM_OF_GENERIC_AGGREGATION_PIPES 13

/**
 * @brief Num of cmd words
 */
#define IPA_FIFO_CMD_NUM_OF_WORDS 4

/**
* @brief   Total number of bits in client ID 
*/
#define IPA_HWSNIFF_CLIENT_BITSIZE  4
#define IPA_HWSNIFF_RX_CMD_L_BITSIZE   56
#define IPA_HWSNIFF_RX_CMD_H_BITSIZE   40
#define IPA_HWSNIFF_TX_CMD_L_BITSIZE   55
#define IPA_HWSNIFF_TX_CMD_H_BITSIZE   41

/**
* @brief   Number of QMB engines
*/
#define IPA_NUMBER_OF_QMBS                            1

/* GSI FW */
#define GSI_FW_IPA_IMAGE_FILE "firmware/fw_ipa_gsi_2.0.inc"
#define GSI_MAX_FW_SIZE (3072 * sizeof(uint64))

/**
 * @brief   Maximal number of virtual contexts per coalescing group.
 */
#define IPA_COALESCING_VCS_MAX 0

/**
 * @brief   Values that represent VIDRV_ipa_HwSniffClients. 
 */
typedef enum VIDRV_ipa_HwSniffClients
{
    IPA_HWSNIFF_CLIENT_RX_LEGACY   = 0, /**< */
    IPA_HWSNIFF_CLIENT_RX_HPS      = 1, /**< */
    IPA_HWSNIFF_CLIENT_HPS_DPS     = 2, /**< */
    IPA_HWSNIFF_CLIENT_DPS_TX      = 3, /**< */
    IPA_HWSNIFF_CLIENT_TX_VRTL     = 4, /**< */
    IPA_HWSNIFF_CLIENT_ACK_MGR     = 5, /**< */
    IPA_HWSNIFF_CLIENT_EMPTY       = 7, /**< */
    IPA_HWSNIFF_CLIENT_PROD_ACKMNGR= 10, /**< */
    IPA_HWSNIFF_CLIENT_TESTBUS     = 11, /**< */
} VIDRV_ipa_HwSniffClients;

/**
* @brief   Values that represent the IPA Resource-Types
*/
typedef enum
{
   VIDRV_IPA_RSRC_TYPE_SRC_PKT_CONTEXTS               = 0,
   VIDRV_IPA_RSRC_TYPE_SRC_DATA_DESCRIPTOR_LISTS      = 1,
   VIDRV_IPA_RSRC_TYPE_SRC_DATA_DESCRIPTOR_BUFFERS    = 2,
   VIDRV_IPA_RSRC_TYPE_SRC_HPS_DMARS                  = 3,
   VIDRV_IPA_RSRC_TYPE_SRC_ACK_ENTRIES                = 4,
   VIDRV_IPA_RSRC_TYPE_SRC_NUMBER                     = 5,    /* The total number of the IPA Source Resources types */
   VIDRV_IPA_RSRC_TYPE_DST_DATA_SECTORS               = 5,
   VIDRV_IPA_RSRC_TYPE_DST_DPS_DMARS                  = 6,
   VIDRV_IPA_RSRC_TYPE_DST_NUMBER                     = 7,  /* The total number of the IPA Destination Resources types */
   VIDRV_IPA_RSRC_TYPE_RX_HPS_CMDQ                    = 7,
   VIDRV_IPA_RSRC_TYPE_NUMBER
} VIDRV_ipa_ResourceTypesType;

/* *
* @brief  Values to represent possible cases of SNOC_FEC.CLIENT
*/
typedef enum
{ 
   /* The following value should be used to mark Not Available cases */
   VIDRV_IPA_SNOC_FEC_CLIENT_NOT_AVAILABLE   = 0x1B,
   /* List of SNOC_FEC possible client's values */
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_MBIM         = 0x00,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_RNDIS        = 0x01,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_SW0          = 0x02,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_SW1          = 0x03,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_SW2          = 0x04,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_SW3          = VIDRV_IPA_SNOC_FEC_CLIENT_NOT_AVAILABLE,
   VIDRV_IPA_SNOC_FEC_CLIENT_RX_SW4          = VIDRV_IPA_SNOC_FEC_CLIENT_NOT_AVAILABLE,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR0           = 0x05,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR1           = 0x06,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR2           = 0x07,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR3           = 0x08,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR4           = 0x09,
   VIDRV_IPA_SNOC_FEC_CLIENT_DMAR5           = 0x0A,
   VIDRV_IPA_SNOC_FEC_CLIENT_IMM_CMD         = 0x0B,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_HDRI        = 0x0C,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_FILTER      = 0x0D,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_NAT_E       = 0x0E,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_NAT_I_F     = 0x0F,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_NAT_F       = 0x10,
   VIDRV_IPA_SNOC_FEC_CLIENT_HPS_ROUTER      = 0x11,
   VIDRV_IPA_SNOC_FEC_CLIENT_DDMAR0          = 0x12,
   VIDRV_IPA_SNOC_FEC_CLIENT_DDMAR1          = 0x13,
   VIDRV_IPA_SNOC_FEC_CLIENT_DDMAR2          = 0x14,
   VIDRV_IPA_SNOC_FEC_CLIENT_DDMAR3          = 0x15,
   VIDRV_IPA_SNOC_FEC_CLIENT_DDMAR4          = 0x16,
   VIDRV_IPA_SNOC_FEC_CLIENT_TX_0            = 0x17,
   VIDRV_IPA_SNOC_FEC_CLIENT_TX_1            = 0x18,
   VIDRV_IPA_SNOC_FEC_CLIENT_UC              = 0x19,
   VIDRV_IPA_SNOC_FEC_CLIENT_SNIF            = 0x1A,
} VIDRV_ipa_SnocFecClientType;

/**
 * @brief   Structure parsing for HW Command Sniffer block. 
 */
typedef union VIDRV_ipa_HwSniffCommand
{
    uint64  raw[IPA_FIFO_CMD_NUM_OF_WORDS / 2];
    VIDRV_ipa_HwSniffClients type : IPA_HWSNIFF_CLIENT_BITSIZE;
        
    /** @brief Rx CMD word. Type=0,1 */
    struct RX_RULE
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;   
        uint64 rxSrcGroup           : 3;   
        uint64 addrMsb              : 1;   
        uint64 cmdWordLsb           : IPA_HWSNIFF_RX_CMD_L_BITSIZE; 
        uint64 cmdWordMsb           : IPA_HWSNIFF_RX_CMD_H_BITSIZE; 
        uint64 rxAddrEx             : 8;   
        uint64 count                : 16;  
    } rx;  
        
    /** @brief Tx CMD word. Type=4 for TX 0 and Type=8 for TX 1*/
    struct TX_RULE
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;
        uint64 destPipe             : 5;   
        uint64 cmdWordLsb           : IPA_HWSNIFF_TX_CMD_L_BITSIZE; 
        uint64 cmdWordMsb           : IPA_HWSNIFF_TX_CMD_H_BITSIZE; 
        uint64 _res                 : 7;   
        uint64 count                : 16;  
    } tx; 
    struct EMPTY_RULE /** Type=7 */
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;   
        uint64 snifferElement       : 2;   
        uint64 queueType            : IPA_HWSNIFF_CLIENT_BITSIZE;   
        uint64 _res1                : 54;  
        uint64 _res2                : 32;  
        uint64 missCount            : 16;  
        uint64 _res3                : 16;  
    } empty; 
    struct CTX_RULE /** Type=2,3 */
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;   
        uint64 cmdWord              : 20;  
        uint64 _res1                : 40;    
        uint64 _res2                : 48;   
        uint64 count                : 16;   
    } ctx; 
    struct ACK_RULE /** Type=5 */
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;    
        uint64 _res1                : 60;
        uint64 _res2                : 48;
        uint64 count                : 16;
    } ack;
    struct DEBUG_RULE /** Type=11 */
    {
        uint64 type                 : IPA_HWSNIFF_CLIENT_BITSIZE;    
        uint64 select               : 4;
        uint64 data0                : 32;
        uint64 data1Lsb             : 24;
        uint64 data1Msb             : 8;
        uint64 _res                 : 40;
        uint64 count                : 16;
    } debug;
} VIDRV_ipa_HwSniffCommand;

#ifdef __cplusplus
}
#endif

#endif /* IPA_FLAVOR_DEFS_H */
