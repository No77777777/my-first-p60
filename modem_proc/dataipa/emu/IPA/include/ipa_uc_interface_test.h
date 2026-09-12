#ifndef IPA_UC_INTERFACE_TEST_H_
#define IPA_UC_INTERFACE_TEST_H_

#include "comdef.h"


#define IPA_UC_TEST_NUM_OF_PARAMS                  10
#define IPA_UC_TEST_HEADER_INSERATION_PATTERN      0xAB /**< Pattern to write in the inserted header. */
#define IPA_UC_TEST_MAX_HEADER_REMOVAL_SIZE        0x20 /**< Header insertion maximum size in bytes. */
#define IPA_UC_TEST_MAX_HEADER_INSERTION_SIZE      0x20 /**< Header removal maximum size in bytes. */
#define IPA_UC_TEST_MAX_COMMANDS                   0x10 /**< Maximum commands in Rx CMDQ test */
#define IPA_UC_MIN_TIMER_COUNT                     0x100  /**< Minimum allowed timer count */
#define IPA_UC_MAX_TIMER_COUNT                     0xFFFF  /**< Maximum allowed timer count */
#define IPA_UC_MAX_PC_PIPE_COUNT                   6
#define IPA_UC_INVALID_PIPE_ID                     0xFF

typedef enum
{
   IPA_UC_TEST_BACKUP_BAM_PIPES,       /**< Backup 2 bam pipes before power collapse. */
   IPA_UC_TEST_READ_ADDRESS,           /**< Read from uC RAM address. */
   IPA_UC_TEST_WRITE_ADDRESS,          /**< Write to uC RAM address. */
   IPA_UC_TEST_DMA_READ,               /**< Read from System NOC to local memory by QMB DMA engine. */
   IPA_UC_TEST_DMA_WRITE,                /**< Write to System NOC from local memory by QMB DMA engine. */
   IPA_UC_TEST_TIMER,                  /**< Test uC timers */
   IPA_UC_TEST_IMMEDIATE_CMD,          /**< Test uC ability to send immediate command to PROC*/
   IPA_UC_TEST_INT_TO_CPU,             /**< Test uC ability to generate IPA interrupt toward EE*/
   IPA_UC_TEST_INT_TO_UC,              /**< Test uC ability to get IPA interrupt */
   IPA_UC_TEST_RESTORE_BAM_PIPES,      /**< Restore 2 bam pipes after power collapse. */
   IPA_UC_TEST_FILTER_PACKET,          /**< Send packets by filters to be manipulated by uC */
   IPA_UC_TEST_RX_CMDQ,                /**< uC "generates" data and sent it to TX */
   IPA_UC_TEST_BACKUP_IPA,             /**< Backup IPA configuration before power collapse. */
   IPA_UC_TEST_RESTORE_IPA,            /**< Restore IPA configuration after power collapse. */
   IPA_UC_TEST_BACKUP_IPA_SECURITY,    /**< Backup IPA/XPU/VMIDMT configuration before power collapse. */
   IPA_UC_TEST_RESTORE_IPA_SECURITY,   /**< Restore IPA/XPU/VMIDMT configuration after power collapse. */
   IPA_UC_TEST_QMB_COAL,               /**< Test interrupt coalescing in QMB */
   IPA_UC_TEST_UNALIGNED_ACCESS,       /**< Test uC capability to access unaligned address (following CR QCTDD01117742)*/
   IPA_UC_TEST_REGISTER_READ_CR,       /**< Test CR QCTDD00945399 fix for uC*/
   IPA_UC_TEST_LATENCY_MEASURE,        /**< Measure latency to PCIe, IPA SRAM, DDR, IMEM and etc. */
   IPA_UC_TEST_DUAL_TX,                /**< Simulate dual TX work, with IPA and uC */
   IPA_UC_TEST_VUIC_ERROR,             /**< Test IU NMI invocation at the VUIC error */
   IPA_UC_TEST_PPB_REGISTERS_ACCESS,   /**< Test uC registers access */
   IPA_UC_TEST_SET_UCP_PARAMS,         /**< Set ucp parameters */
   IPA_UC_TEST_PROD_ACK_MGR_WA,        /**< Test uC producer ack manager_wa */
}IPA_UC_TESTS;

typedef enum
{
   IPA_UC_NO_ACK,       /**<Do not request ACK */
   IPA_UC_FLAGS_ACK,    /**<Request ACK by flags */
   IPA_UC_LOOPBACK_ACK, /**<Request ACK by loopback command */
}IPA_UC_ACK_MODES;

#pragma pack(push,1) /*Save previous, and turn on 1 byte alignment*/


typedef PACK(struct) IPA_UC_TEST_CMD_DATA
{
   uint32 testId;
   uint32 isTestFinished;
   uint32 isTestFailed;
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];
}IPA_UC_TEST_CMD_DATA;

typedef PACK(union) IPA_UC_TEST_ADDRESS_ACCESS_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_ADDRESS_ACCESS_DATA_SPECIFIC
   {
      uint32 address;
      uint32 value; /*For read operation: expected value in read. For write operation: value to write */
   }specific;
} IPA_UC_TEST_ADDRESS_ACCESS_DATA;

typedef PACK(union) IPA_UC_TEST_INT_TO_CPU_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_INT_TO_CPU_DATA_SPECIFIC
   {
      uint32 intId;
      uint32 allEEs;
   }specific;
} IPA_UC_TEST_INT_TO_CPU_DATA;


typedef PACK(union) IPA_UC_TEST_INT_TO_UC_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_INT_TO_UC_DATA_SPECIFIC
   {
      uint32 mask;
   }specific;
} IPA_UC_TEST_INT_TO_UC_DATA;

typedef PACK(union) IPA_UC_TEST_FILTER_PACKET_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_FILTER_PACKET_DATA_SPECIFIC
   {
      uint32 discardPacket;            /*drop the packet. This field override all other fields*/
      uint32 headerInsertionSize;      /*number of byte to fill with the value IPA_UC_TEST_HEADER_INSERATION_PATTERN before the original packet*/
      uint32 headerRemovalSize;        /*number of byte to remove from the original packet*/
      uint32 destinationPipeId;        /*destination pipe to route the packet*/
      uint32 numPackets;               /*number of expected packets*/
   }specific;
} IPA_UC_TEST_FILTER_PACKET_DATA;

typedef PACK(union) IPA_UC_TEST_RX_CMDQ_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_RX_CMDQ_DATA_SPECIFIC
   {
      uint32 numOfTransfers;      /*number of transfers to generate*/
      uint32 transferSize;        /*data size in bytes*/
      uint32 numOfCmds;           /*number of commands to generate per transfer*/
      uint32 requestAck;          /*Indication whether ACK is needed. 0 - don't request ACK, 1 - request ACK by flags, 2 - request ACK by loopback*/
      uint32 bytePattern;         /*pattern to draw in data. Only 8 LSB are used*/
      uint32 srcPipeId;           /*source pipe for the transfer*/
   }specific;
} IPA_UC_TEST_RX_CMDQ_DATA;

typedef PACK(struct) IPA_UC_DMA_TRANSFER_DATA
{
   uint64 systemAddress   :64;
   uint32 localAddress    :32;
   uint8  sizeInBytes     :8;
   uint8  completionMode  :1;    /**< 0: post, 1: complete */
   uint8  blockMode       :1;    /**< 0: off, 1: on */
   uint8  reserved        :6;
} IPA_UC_DMA_TRANSFER_DATA;

typedef PACK(union) IPA_UC_TEST_DMA_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_DMA_DATA_SPECIFIC
   {
      uint32 numOfTransfers;
      uint32 errorType;
      uint64 pData; /*64 bit pointer to IPA_UC_DMA_TRANSFER_DATA*/
   }specific;
} IPA_UC_TEST_DMA_DATA;

typedef PACK(union) IPA_UC_TEST_TIMER_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_TIMER_DATA_SPECIFIC
   {
      uint32 id;             /**< The timer id */
      uint32 numReloads;     /**< 1: count once and stop, > 1 reload and wait for trigger again untill numReloads expires */
      uint32 count;          /**< Count value, 0 - counter disabled */
      uint32 putToSleep;     /**< Put uC to sleep before waiting to interrupt */
   }specific;
} IPA_UC_TEST_TIMER_DATA;

typedef PACK(union) IPA_UC_TEST_IMMEDIATE_CMD_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_IMMEDIATE_CMD_DATA_SPECIFIC
   {
      uint32 opcode;            /**< The immediate command opcode */
      uint32 pipeId;            /**< The immediate command pipe id */
      uint32 numDoubleWords;    /**< Number of double words (64 bits) to write */
      uint32 words[IPA_UC_TEST_NUM_OF_PARAMS-3];   /**< Words (32 bit) to write */
   }specific;
} IPA_UC_TEST_IMMEDIATE_CMD_DATA;

typedef PACK(union) IPA_UC_TEST_PC_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_PC_DATA_SPECIFIC
   {
      uint32 pipe[IPA_UC_MAX_PC_PIPE_COUNT];       /**< The pipes id to backup/restore */
   }specific;
} IPA_UC_TEST_PC_DATA;

typedef PACK(union) IPA_UC_TEST_QMB_COAL_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_QMB_COAL_DATA_SPECIFIC
   {
      uint32 localAddr           ;
      uint64 systemAddr          ;
      uint32 sizeInBytes         ;
      uint32 numOfTransfers      ;
      uint32 direction           ;    /**< 0: Read from System NOC to local memory. 1: Write to System NOC from local memory */
   }specific;
} IPA_UC_TEST_QMB_COAL_DATA;

/**
 * @brief   Values that represent the type of latency measurement 
*/
typedef enum
{  
   IPA_LATENCY_STRUCTURAL_QMB = 0,
   IPA_LATENCY_STEADY_STATE_QMB,
   IPA_LATENCY_STRUCTURAL_MEMCPY,
   IPA_LATENCY_NUM_OF_TYPES
}IpaLatencyMeasurementType;

typedef PACK(union) IPA_UC_TEST_LATENCY_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_LATENCY_DATA_SPECIFIC
   {
      uint32    latencyType                     ; /**< Use memcpy or QMB for the measurements */
      uint32    measureAddress                  ; /**< Measure read/write latency from/to this address */
      uint32    measureSize                     ; /**< Size of transfer to measure */
      uint32    readMeasurement                 ; /**< Measurement of read transfer from measureAddress in size of measureSize */
      uint32    writeMeasurement                ; /**< Measurement of write transfer to measureAddress in size of measureSize */
      uint32    totalBytes                      ; /**< Total sent bytes for bandwidth calculation */
      uint32    totalReadCycles                 ; /**< Total time for all read transfers */
      uint32    totalWriteCycles                ; /**< Total time for all write transfers */
      uint32    isSramUsedAsIpaMemory           ; /**< TRUE  - IPA SRAM is used instead of uC DRAM
                                                       FALSE - uC DRAM is used for measurements */
   }specific;
} IPA_UC_TEST_LATENCY_DATA;

typedef PACK(union) IPA_UC_TEST_PROD_ACK_MGR_WA_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct)
   {
      uint32 t1;        /**< T1 value in msec, 0 - WA disabled */
      uint32 t2;        /**< T2 value in msec */
   } specific;
} IPA_UC_TEST_PROD_ACK_MGR_WA_DATA;

/**
 * @brief   Values that represent the type of dual tx measurement 
*/
typedef enum
{  
   IPA_DUAL_TX_QMB_NO_TRANSACTIONS = 0,
   IPA_DUAL_TX_QMB_WRITE_ONLY = 1,
   IPA_DUAL_TX_QMB_READ_ONLY = 2,
   IPA_DUAL_TX_QMB_READ_WRITE = 3,
   IPA_DUAL_TX_QMB_NUM_OF_TYPES
}IpaDualTxMeasurementType;

typedef PACK(union) IPA_UC_TEST_DUAL_TX_DATA
{
   uint32 params[IPA_UC_TEST_NUM_OF_PARAMS];

   PACK(struct) IPA_UC_TEST_DUAL_TX_DATA_SPECIFIC
   {
      uint32 dualTxType       :32; /**< Type of traffic QMB Reads or Writes or Reads and Writes */
      uint32 qmb0PipeNumber   :32; /**< Pipe number that is connected to QMB0 */
      uint32 qmb1PipeNumber   :32; /**< Pipe number that is connected to QMB1 */
      uint32 deviceDdrAddress :32; /**< Address in device DDR to read from */
      uint32 hostPcieMemory   :32; /**< Address in PCIe space to write to */
      uint32 sramAddressOffset:32; /**< Address offset in IPA SW SRAM for uC use */      
   }specific;
} IPA_UC_TEST_DUAL_TX_DATA;

/**
* @brief   Values that represent UCP commands Id. 
*/
typedef enum
{
   IPA_CLAT_UL_ID                      = 0,
   IPA_CLAT_DL_ID                      = 1,
   IPA_MAP_T_UL_FMR_BMR_ID             = 2,
   IPA_MAP_T_DL_FMR_BMR_ID             = 3,
   IPA_MAP_T_UL_DMR_ID                 = 4,
   IPA_MAP_T_DL_DMR_ID                 = 5,
   IPA_BRIDGING_802_3_TO_802_3_ID      = 6,
   IPA_BRIDGING_802_3_TO_ETH_II_ID     = 7,
   IPA_BRIDGING_ETH_II_TO_802_3_ID     = 8,
   IPA_BRIDGING_ETH_II_To_ETH_II_ID    = 9,
   IPA_UC_HEADER_MANIPULATION_TEST_ID  = 0xFF
} VIDRV_ipa_UcpCommandId;

/**
* @brief   A struct that is copied to the uC in case a Packet Processing Command is issued.
*/
typedef PACK(struct) 
{
   /*Using bit 11:8 of 16 bit CommandID to pass UCP_RESUME checksum control information*/
   uint8 commandId;                           /* uC processing command id*/
   uint8 commandIdUcpResumeChksmCtrl;         /* UCP_RESUME checksum control information*/
   uint16 commandIdUnused;                    /* unused top 16 bits of CommandId Word*/
   uint8* pPacket;          /* Input + Output. Pointer to start of the packet. */
   /* Starts from IP offset / offset after header removal.*/
   /* Packet buffer changes, such as header removal, will be reflected by changes this pointer.*/
   uint8* pNewHdr;          /* Input + Output. Pointer to the start of the inserted header. */
   /* Header buffer changes, such as removing the start of the inserted header, will be reflected by changes this pointer.*/
   uint32 procParams;       /* Input only.*/
   /* For MAPT FMR/BMR UL will contain: PSID_PortOffset, PSID_BitSize, EA_Start_BitOffset, EA_V4_Suffix_BitSize.*/
   /* For MAPT FMR/BMR DL will contain: PSID_PortOffset, PSID_BitSize (for performing PSID consistency check verification).*/
   /* For MAPT DMR or CLAT UL&DL will contain: BRPrefix_LengthInBytes.*/
   /* For Ethernet Bridging will not be used. */
   uint8* pPacketPayload;   /* Input only. */
   /* Pointer to packet's payload/transport header.*/
   uint8* pPacketL2;        /* Input only.*/
   /* Pointer to L2 header of the packet / start of the original packet buffer as if no header removal took place.*/
   uint8* pPacketFrag;     /* Input only. 0 - not a fragment. Any non zero value - fragment*/
   /* For IPv6 packet it is a pointer to IPv6 fragment ext header of the packet*/
   /* For IPv4 packet, any non zero value may be set for a fragment*/
   uint32 protocol;         /* Input only. */
   /* Packet's v4 protocol or last IPv6 next header's protocol, ideally the transport layer protocol.*/
   uint32 newHdrLen;        /* Input + Output. Size of the inserted header.*/
   /* May be used to reduce the size of the inserted header.*/
   uint8* pPacketStatus;    /* Start address for packet status in the packet context */
   uint8* pPacketOrig;      /* Absolute address of packet in IPA RAM */
} PacketHeaderProcessingContextType;

typedef PACK(union)
{
   uint32 params;

   PACK(struct)
   {
      uint32 exception           :1;
      uint32 destPipeOverride    :1;
      uint32 destPipeId          :5;
   } specific;
} IPA_UC_TEST_UCP_PARAMS;

#pragma pack(pop) /* Revert alignment to what it was previously*/

#endif /* IPA_UC_INTERFACE_TEST_H_ */
