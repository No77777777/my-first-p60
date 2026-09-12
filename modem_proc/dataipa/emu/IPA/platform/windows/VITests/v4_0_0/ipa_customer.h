#ifndef IPA_CUSTOMER_H
#define IPA_CUSTOMER_H

#include "ipa_debug.h"
#include "ipa_flavor_customer.h"



/* Workarounds related defines */
#define IPA_WINDOWS_GRANULARITY_WORKAROUND 15

#define CORE_LEVEL_GE

/* DUT to which we flash the core */
#define IPA_COPSS_DUT   0

/* define the memory pool to be in the FPGA internal memory 
 * and not in the system memory of the windows machine 
 */
#define IPA_DMA_INTERNAL_MEMORY

/** 
 * @brief	General info to manage generic pipe's allocation and randomization:
 *          1. number of pipes enabled in the IPA
 *          2. special consumer pipe for IPA's system needs
 *          3. minimal and maximal pipe's ID for consumers and producers 
 *          4. minimal and maximal pipe's ID for de-aggregation consumers
 *
 * @note    Only following values should be changed to adjust the pipe management for a target:
 *             > IPA_PIPE_CONSUMER_MIN_ID
 *             > IPA_PIPE_CONSUMER_MAX_ID
 *             > IPA_PIPE_CONSUMER_DEAGGR_MIN_ID
 *             > IPA_PIPE_CONSUMER_DEAGGR_MAX_ID
 *             > IPA_PIPE_PRODUCER_MIN_ID
 *             > IPA_PIPE_PRODUCER_MAX_ID
 */
/* consumers : */
#define IPA_PIPE_CONSUMER_MIN_ID          0
#define IPA_PIPE_CONSUMER_MAX_ID          9
/* The value of IPA_PIPE_SYSTEM_CONSUMER_ID is the maximal possible ID of consumers */
#define IPA_PIPE_SYSTEM_CONSUMER_ID       (IPA_PIPE_CONSUMER_MAX_ID)
#define IPA_PIPE_CONSUMER_DEAGGR_MIN_ID   0
#define IPA_PIPE_CONSUMER_DEAGGR_MAX_ID   2
/* producers : */
#define IPA_PIPE_PRODUCER_MIN_ID          10
#define IPA_PIPE_PRODUCER_MAX_ID          22

/**
 *  @brief The number of pipes available for allocation, it could be less then IPA_MAX_PIPE_NUM.
 *
 *  @note  It is not a final version. But it is a good marker of tests which should be 
 *         updated after adding of GSI channel randomization.
 */
#define IPA_MAX_ALLOC_PIPE_NUM            16

/**
 *  @brief The number of GSI channels available per EE.
 */
#define IPA_MAX_NUM_OF_GSI_CHANNELS_PER_EE 17

/**
 *  @brief The number of GSI event channels available per EE.
 */
#define IPA_MAX_NUM_OF_GSI_EVENT_CHANNELS_PER_EE 12

/*
 * @brief   The minimal expected performance value for E2E performance test
*/
#define IPA_MIN_EXPECTED_PERFORMANCE_IN_MB 55.0

/** 
 * @brief	Address in PCIe space.
 * @note	Irrelevant in R3PC.
*/
#define IPA_UC_PCIE_ADDRESS   (0xFFFFFFFF)

/** 
 * @brief	Address in IMEM space.
 * @note	Irrelevant in R3PC.
*/
#define IPA_UC_IMEM_ADDRESS   (0xFFFFFFFF)

/**
 * @brief   Address to initiate BAD_SNOC_ACCESS_IRQ
*/
#define IPA_BAD_SNOC_ADDRESS  (0xF0000000)

/*
 * @brief   A flag that indicates if power collapse is supported in current target
*/
/* TODO: Should be non-zero, but currently crashes on COPSS and doesn't allow regression in CI */
#define IPA_POWER_COLLAPSE_SUPPORTED   0

/*
 * @brief   Memory ranges for test ipa_test_run_bit_40_via_gsi_test (physical addresses).
*/
#define IPA_BIT_40_TEST_IPA_ADDRESSES { 0 }

/*
 * @brief   Memory ranges for test ipa_test_run_bit_40_via_gsi_test (A7 virtual addresses mapped to PCIe).
*/
#define IPA_BIT_40_TEST_A7_ADDRESSES  { 0 }

/*
 * @brief   Type of XPU that is present in chip.
 *          Possible defines: IPA_XPU2, IPA_XPU3.
*/
#define IPA_XPU3

/*
 * @brief   Security architecture. The VMID that APPS access IPA rgs is a constant 0 - Honeybadger Security Architecture.
 *          Possible defines: IPA_SECURITY_APPS_VMID_CHANGEABLE, IPA_SECURITY_APPS_VMID_0.
*/
#define IPA_SECURITY_APPS_VMID_0

#endif /* IPA_CUSTOMER_H */
 
