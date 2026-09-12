#ifndef IPA_FLAVOR_CUSTOMER_H
#define IPA_FLAVOR_CUSTOMER_H
/*
===========================================================================

FILE:         ipa_flavor_customer.h

DESCRIPTION:  
IPA customizations per specific IPA core flavor (version 3.5.1).

$Header: 

===========================================================================

===========================================================================
Copyright © 2016 Qualcomm Technologies Incorporated.
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
   {"IPA_CONFIG_INTERRUPT_POLLING",                1 }, \
   {"IPA_CONFIG_PUBLISH_THRESHOLD",                1 }, \
   {"IPA_CONFIG_USE_GSI",                          1 }, \
   {"IPA_CONFIG_FAST_CLOCK",                       1 }, \
   {"IPA_CONFIG_RESOURCE_CONFIG",                  2 }, \
   {"IPA_CONFIG_STATIC_FIFO_SIZE_TLV_AOS",         0 }, \
   {"IPA_CONFIG_STATIC_SEQUENCE_TYPE",             0 }, \
   {"IPA_CONFIG_STATIC_RESOURCE",                  0 }, \
   {"IPA_CONFIG_OVERRIDE_IPA_CGC",                 0 }, \
   {"IPA_CONFIG_OVERRIDE_DEFAULT_EE_NUM",          IPA_DEFINE_MANAGER_UNDEFINED}, \
   {"IPA_CONFIG_SRAM_DYNAMIC_ALLOCATION",          0 }, \
   {"IPA_CONFIG_GSI_CH_ID_MAPPING_MODE",           0 }, \
   {"IPA_CONFIG_ENDP_STATIC_CONFIGURATION_INDEX",  IPA_DEFINE_MANAGER_UNDEFINED}, \
   {"IPA_CONFIG_DEFAULT_TRE_SIZE",                 16}, \
}

/**
* @brief   Current IPA Flavor
*/
#define IPA_FLAVOR_3_5_1


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

/* Workaround for -------------------------------------------------------- CR QCTDD02928301
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02928301 -  HW-State: Opened
                                                                           VI-State: Pass w WA
   Issue Description:
   NDMA Mode test + Immd cmd with WaitRsrcGrp_clear is sent. In this test DMA_RSRC_GRP blocked.
*/
#define IPA_HOLB_PLUS_EXECUTABLE_IMMEDIATE_COMMAND_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03195028
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03195028 -  HW-State: Opened
                                                                           VI-State: Pass w WA (Test 639)
   Issue Description:
   issue is in comp_regs ORRED selection, GRP_0 is always used due to typo
*/
#define IPA_RESOURCE_COUNTERS_WORK_ON_SOURCE_GROUP_0_ONLY_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03527717
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03527717 -  HW-State: Opened
                                                                           VI-State: Pass w WA (Test 410)
   Issue Description:
   the value of IPA_TX_COMMANDER_CMDQ_DATA_RD_1 register is incorrect
*/
#define IPA_TX_COMMANDER_CMDQ_ISSUE_WORKAROUND

/* Workaround for -------------------------------------------------------- Defined by HPG
                                                                           HW-State: There is known issue
                                                                           VI-State: Pass w WA (test 543)
   Issue Description:
   [IPA 3.0]   Checksum value depends on the addresses of producer's descriptors :
               even or odd space between descriptors (The WA guaranties that each spacew will have even number of bytes)
*/
#define IPA_CHECKSUM_OFFLOAD_CORNER_CASE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03525873
                                                                           HW-State: Opened
                                                                           VI-State: Pass w WA (test 337)
   Issue Description:
   [IPA 3.x] A route rule index is calculated incorrectly when Destination NAT routes to a hash Route table.
*/
#define IPA_NAT_AND_HASH_ROUTE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03571575
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03571575 -  HW-State: Opened
                                                                           VI-State: Pass w WA (test 337)
   Issue Description:
   [IPA 3.x] Partially disabling hash-tables of an accelerator (FILTER/ROUTER) is causing the accelerator 
   to pick the results of the previous hash-enabled packet when processing a hash-disabled packet.
*/
#define IPA_NON_HASH_AND_HASH_FILTER_ROUTE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03067490
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03067490 -  HW-State: IFR
                                                                           VI-State: Pass w WA (tests 260)
   Issue Description:
   IPA3.5:CMDQ:RD/WR BLOCK/RELEASE testing of PROD/CONS ACKMNGR CMDQ results in hang
*/
#define IPA_CMDQ_ACTION_ACKMNGR_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03610896
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03610896 -  HW-State: Opened
                                                                           VI-State: Pass w WA (test 225)
   Issue Description:
   [IPA 3.5] traffic with sizes of 8 bytes and less can cause IPA stall
*/
#define IPA_PREFETCH_BUFFER_MISSES_A_POP_AND_CAUSES_DEADLOCK_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD02216972
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02216972 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 534)
   Issue Description:
   IPA Producer Reset does not clear aggreagtion state
*/
#define IPA_STOP_PRODUCER_OPEN_AGGR_FRAME_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03059770
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03059770 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 40)
   Issue Description:
   The field exceptionType of a packet Status can contain incorrect vallue:  
   IPA_SQEXP_PACKET_LENGTH instesd of IPA_SQEXP_IPH_PARSER_THRESHOLD for IPv6 packets
*/
#define IPA_EXCEPTIONTYPE_FOR_IPH_PARSER_THRESHOLD_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD02793367
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD02793367 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 549)
   Issue Description:
   [IPA 3.0, 3.1] Notification status for data sent directly to the Logging producer is Log status instead of Packet Status
*/
#define IPA_DPL_WRONG_STATUS_TYPE_WORKAROUND

/* Workaround for -------------------------------------------------------- CR QCTDD03025804
   https://tardis.qualcomm.com/cqweb/#/QCTDD/QCTDD/RECORD/QCTDD03025804 -  HW-State: Opened
                                                                           VI-State: Pass w WA (tests 122)
   Issue Description:
   In case DMA TASK FLUSH is enabled for a pkt, then it gets dropped.
   Corresponding dropped status is pushed to NOTIF pipe and SNIFFER queue.
 
   If SRC pipe has DPL enabled, then DPL PKT is not pushed to DPL producer pipe but corresponding DPL
   status is getting pushed to sniffer, which is unexpected.

*/
#define IPA_INCORRECT_SNIFFER_DPL_STATUS_WORKAROUND

#endif /* IPA_FLAVOR_CUSTOMER_H */
