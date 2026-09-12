#ifndef IPA_FLAVOR_CUSTOMER_H
#define IPA_FLAVOR_CUSTOMER_H
/*
===========================================================================

FILE:         ipa_flavor_customer.h

DESCRIPTION:  
IPA customizations per specific IPA core flavor (version 4.5.0).

$Header: 

===========================================================================

===========================================================================
Copyright © 2017 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================*/

/* When IPA_CONFIG_INTERRUPT_POLLING is 1 then the interrupts are ignored 
   and polling mode is used 
   When IPA_CONFIG_PUBLISH_THRESHOLD is 1 then the IPA_PUBLISH_THRESHOLD is ignored
   and BAM_DESC_CNT_TRSHLD.CNT_TRSHLD is set at 1 (usable for BAM2BAM PureDMA tests )
*/
#define IPA_CUSTOMER_DEFAULT_DEFINE_VECTOR \
{ \
   {"IPA_CONFIG_INTERRUPT_POLLING",           1 }, \
   {"IPA_CONFIG_PUBLISH_THRESHOLD",           1 }, \
   {"IPA_CONFIG_USE_GSI",                     1 }, \
   {"IPA_CONFIG_FAST_CLOCK",                  1 }, \
   {"IPA_CONFIG_RESOURCE_CONFIG",             0 }, \
   {"IPA_CONFIG_STATIC_FIFO_SIZE_TLV_AOS",    0 }, \
   {"IPA_CONFIG_STATIC_SEQUENCE_TYPE",        0 }, \
   {"IPA_CONFIG_STATIC_RESOURCE",             0 }, \
   {"IPA_CONFIG_OVERRIDE_IPA_CGC",            0 }, \
   {"IPA_CONFIG_OVERRIDE_DEFAULT_EE_NUM",     IPA_DEFINE_MANAGER_UNDEFINED}, \
   {"IPA_CONFIG_SRAM_DYNAMIC_ALLOCATION",     0 }, \
   {"IPA_CONFIG_GSI_CH_ID_MAPPING_MODE",      0 }, \
   {"IPA_CONFIG_ENDP_STATIC_CONFIGURATION_INDEX",  IPA_DEFINE_MANAGER_UNDEFINED}, \
   {"IPA_CONFIG_DEFAULT_TRE_SIZE",            16}, \
}

/**
* @brief   Current IPA Flavor
*/
#define IPA_FLAVOR_4_5_0 


/* Comment out this define in case there are some missing registers definitions in the shared code */
/* #define IPA_RUMI_SHARED_CODE_VERSION_OF_MSMHWIOREG_WORKAROUND */

/* Workaround for -------------------------------------------------------- CR QCTDD02430817
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02430817 -  HW-State: Opened
                                                                           VI-State: Pass w WA (147)
   Issue Description:
      IPA is stalled when there is invalid QMAP+UL CS header (PAD_GT_PCK_LEN_WITH_PAD) and the header is divided due to pipe’s Data FIFO  wraparound event.
   Reproduction of the problem : test 147 qmap_gen_deaggr_invalid_traffic (osRandState=0xce189205):
      1. Allocate pipes for setup consumer->B2B producer->B2B consumer->producer
      2. Set QMAP Gen.DeAggr for B2B consumer
      3. Send packet with size 0x7ffD to adjust the DATA FIFO:
         B2B Data FIFO : [0x697c : 0xe97b] , size 0x8000
         QMAP Packet start address in DATA FIFO : 0xe979 
      4. Send the QMAP frame (see attached files)
   Recommended work around:
	   DATA FIFO size and QMAP padding should be a multiple of 8
      Note:
      Kumar Vaibhav : " Checksum offload hasn’t been advertised to any customers other than MAV and is not being used over USB
                        in a commercial setting. So this limitation doesn’t impact existing customers. 
                         Also, as the issue is limited to a scenario where header gets corrupted due to link errors, 
                         this is a low probability case and acceptable limitation for test purposes. 
                       "
*/
#define IPA_INVALID_QMAP_UL_CS_HEADER_AND_DATA_FIFO_WRAPAROUND_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD02454550
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02454550 -  HW-State: Opened
                                                                           VI-State: Pass w WA (test 267)
                                                                           VI-State: Pass w WA (test 538)
   Issue Description:
      ‘H’ bits of Status have incorrect values after flushing Filter/Routing hash tables.
      It occur when a hash-table was not empty before flushing, and after the flushing 
      there is the matching on such "hit"-rule which was present in hash-table before flushing. 
      The ‘H’ bit is wrong after flushing for each FIRST “hit” on a rule that was present in hash before the flushing.
   Reproduction of the problem : test 538/267 concurrent_fnr_table_update_...
   Recommended work around : avoid 'H'-bits checking
*/
#define IPA_INVALID_H_BITS_OF_STATUS_AFTER_FNR_HASH_FLUSHING_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD02231093
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02206283 -  HW-State: Completed
                                                                           VI-State: Pass w WA (test 532)
   Issue Description:
   IPA PROD GSI channel reset fails when more than 64KB transfers were previously started on this PROD

   Note:
   The CR is not planned to be fixed and marked as Completed for 3.0 without any mention of future targets.
   However, it cannot be removed from the GSI driver since QUP uses the same driver and doesn't have this issue.
*/
#define GSI_WORKAROUND_CR_QCTDD02231093 (1)

/* Workaround for -------------------------------------------------------- CR QCTDD02511572
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02511572 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 268, 546)
   Issue Description:
   uC_QMB: Slaveway+SNOC access with OOO Wr and OOO Rd causing flushed transaction resulting in SIM hang
*/
#define IPA_OOO_SLAVEWAY_WORKAROUND


/* Workaround for -------------------------------------------------------- CR QCTDD02789721
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02511572 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 260, 410)
   Issue Description:
   CMDQ debug features and fields are not fully supported
*/
#define IPA_CMDQ_DEBUG_FEATURES_NOT_FULLY_SUPPORTED_WORKAROUND

/* Workaround for -------------------------------------------------------- Defined by HPG
                                                                           HW-State: There is known issue
                                                                           VI-State: Pass w WA (test 543)
   Issue Description:
   [IPA 3.0]   Checksum value depends on the addresses of producer's descriptors :
               even or odd space between descriptors (The WA guaranties that each spacew will have even number of bytes)
*/
#define IPA_CHECKSUM_OFFLOAD_CORNER_CASE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03527717
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03527717 -  HW-State: Opened
                                                                           VI-State: Pass w WA (Test 410)
   Issue Description:
   the value of IPA_TX_COMMANDER_CMDQ_DATA_RD_1 register is incorrect
*/
#define IPA_TX_COMMANDER_CMDQ_ISSUE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03571575
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03571575 -  HW-State: Opened
                                                                           VI-State: Pass w WA (test 337)
   Issue Description:
   [IPA 3.x] Partially disabling hash-tables of an accelerator (FILTER/ROUTER) is causing the accelerator 
   to pick the results of the previous hash-enabled packet when processing a hash-disabled packet.
*/
#define IPA_NON_HASH_AND_HASH_FILTER_ROUTE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03984732
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03984732 -  HW-State: Opened
                                                                           VI-State: Pass w WA (test 342, 645)
   Issue Description:
   [IPA 4.0] MBIM Pipe reset not happening due to CGC issue – simple SW WA – enable CGC before reset, and restore after the resetting
*/
#define IPA_MBIM_PIPE_RESET_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD04255223
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD04255223 -  HW-State: Opened
                                                                           VI-State: No test fails with that
   Issue Description:
   IPA_4.0.0::HOLB: packet is not dropped if HOLB_EN was enabled after the packet was already processed in TX
*/
#define IPA_HOLB_EN_AND_PACKET_NOT_DROPPED_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD04149145
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD04149145 -  HW-State: Opened
                                                                           VI-State: No test fails with that
   Issue Description:
   IPA4.0:CGC: enabling global CGC causes incorrect ram_rd_ack occasionally and results in failure
*/
#define IPA_GLOBAL_CGC_WORKAROUND

/* Workaround for -------------------------------------------------------- CR tbd
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/tbd           -  HW-State: Opened
                                                                           VI-State: 263
   Issue Description:
   IPA4.x: RAM_SNIFFER_BASE_OFFSET points to incorrect region
*/
#define IPA_WRONG_RAM_SNIFFER_BASE_OFFSET_WORKAROUND

#endif /* IPA_FLAVOR_CUSTOMER_H */
