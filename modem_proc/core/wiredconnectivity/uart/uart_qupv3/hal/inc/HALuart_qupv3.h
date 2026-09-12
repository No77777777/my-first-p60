#ifndef HALUART_QUPV3_H
#define HALUART_QUPV3_H
/*==================================================================================================

FILE: HALuart_qupv3.h

DESCRIPTION: UART HAL (HW abstraction layer)

                     Copyright (c) 2008-2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR

==================================================================================================*/
/*==================================================================================================

$Header: //components/rel/core.mpss/10.0/wiredconnectivity/uart/uart_qupv3/hal/inc/HALuart_qupv3.h#1 $

==================================================================================================*/
/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
/*==================================================================================================
                                             DATATYPES
==================================================================================================*/

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef _BOOLEAN_DEFINED
typedef unsigned char  boolean;  // must match definition in comdef.h
#define _BOOLEAN_DEFINED
#endif

#ifndef _UINT8_DEFINED
typedef unsigned char  uint8;    // must match definition in DALStdDef.h
#define _UINT8_DEFINED
#endif

#ifndef _UINT16_DEFINED
typedef unsigned short uint16;   // must match definition in DALStdDef.h
#define _UINT16_DEFINED
#endif

#ifndef _UINT32_DEFINED
typedef unsigned long  uint32;   // must match definition in DALStdDef.h
#define _UINT32_DEFINED
#endif

/*==================================================================================================
                                            ENUMERATIONS
==================================================================================================*/

typedef enum
{
   HAL_UART_QUPV3
} HALUartType;

typedef enum
{
   HAL_UART_5_BITS_PER_CHAR = (1<<0),
   HAL_UART_6_BITS_PER_CHAR = (1<<1),
   HAL_UART_7_BITS_PER_CHAR = (1<<2),
   HAL_UART_8_BITS_PER_CHAR = (1<<3),
} HALUartBitsPerCharType;

typedef enum
{
   HAL_UART_CXM_DISABLE,
   HAL_UART_CXM_BYPASS,
   HAL_UART_CXM_ENABLE,
} HALUartCxmConfigType;

typedef enum
{
   HAL_UART_0_5_STOP_BITS = (1<<0),
   HAL_UART_1_0_STOP_BITS = (1<<1),
   HAL_UART_1_5_STOP_BITS = (1<<2),
   HAL_UART_2_0_STOP_BITS = (1<<3),
} HALUartNumStopBitsType;

typedef enum
{
   HAL_UART_NO_PARITY    = (1<<0),
   HAL_UART_ODD_PARITY   = (1<<1),
   HAL_UART_EVEN_PARITY  = (1<<2),
   HAL_UART_SPACE_PARITY = (1<<3),
} HALUartParityModeType;

typedef enum
{
   HAL_UART_WWT_IMR_FLAG            = (1<<16),
   HAL_UART_TXCOMP_IMR_FLAG         = (1<<15),
   HAL_UART_RX_RD_ERROR_IMR_FLAG    = (1<<14),
   HAL_UART_TX_WR_ERROR_IMR_FLAG    = (1<<13),
   HAL_UART_PAR_FRAME_ERR_IMR_FLAG  = (1<<12),
   HAL_UART_RXBREAK_END_IMR_FLAG    = (1<<11),
   HAL_UART_RXBREAK_START_IMR_FLAG  = (1<<10),
   HAL_UART_TX_DONE_IMR_FLAG        = (1<<9),
   HAL_UART_TX_ERROR_IMR_FLAG       = (1<<8),
   HAL_UART_TX_READY_IMR_FLAG       = (1<<7),
   HAL_UART_CURRENT_CTS_IMR_FLAG    = (1<<6),
   HAL_UART_DELTA_CTS_IMR_FLAG      = (1<<5),
   HAL_UART_RXLEV_IMR_FLAG          = (1<<4),
   HAL_UART_RXSTALE_IMR_FLAG        = (1<<3),
   HAL_UART_RXBREAK_CHANGE_IMR_FLAG = (1<<2),
   HAL_UART_RXHUNT_IMR_FLAG         = (1<<1),
   HAL_UART_TXLEV_IMR_FLAG          = (1<<0),
} HALUartInterruptMaskRegisterFlags;

typedef enum
{
   HAL_UART_STATUS_RX_EVENT_HIGH      = (1<<11),
   HAL_UART_STATUS_RX_EVENT_LOW       = (1<<10),
   HAL_UART_STATUS_TRANS_ACTIVE       = (1<<9),
   HAL_UART_STATUS_RX_BREAK_START     = (1<<8),
   HAL_UART_STATUS_HUNT_CHAR          = (1<<7),
   HAL_UART_STATUS_RX_BREAK           = (1<<6),
   HAL_UART_STATUS_PARITY_FRAME_ERROR = (1<<5),
   HAL_UART_STATUS_OVERRUN            = (1<<4),
   HAL_UART_STATUS_TX_EMPTY           = (1<<3),
   HAL_UART_STATUS_TX_READY           = (1<<2),
   HAL_UART_STATUS_RX_FULL            = (1<<1),
   HAL_UART_STATUS_RX_READY           = (1<<0),
} HALUartStatusType;

typedef enum
{
   HAL_UART_SUCCESS,
   HAL_UART_NOT_SUPPORTED,
   HAL_UART_BAD_PARAMETER,
} HALUartErrorType;

/*==================================================================================================
                                             STRUCTURES
==================================================================================================*/

typedef struct HALUartFcnTable HALUartFcnTable;  // forward declaration due to circular reference
typedef struct HALUartObject HALUartObject;

struct HALUartObject
{
   const HALUartFcnTable *fcn_table;   // table of function pointers specific to UART type
   uint8                 *base;        // base of UART register block (virtual address)
   uint8                 *physical;    // base of UART register block (physical address)
   HALUartErrorType      last_error;   // error status of most recent HAL API call
};

typedef struct
{
   uint32  mr1;
   uint32  mr2;
   uint32  sr;
   uint32  misr;
   uint32  isr;
   uint32  ipr;
   uint32  tfwr;
   uint32  rfwr;
   uint32  hcr;
   uint32  dmrx;
   uint32  snap;
   uint32  dmen;
   uint32  ncft;
   uint32  badr;
   uint32  txfs;
   uint32  rxfs;
   uint32  sim;
   uint32  csr;   // BAM only
   uint32  imr;   // BAM only
   uint32  irda;  // BAM only
   uint32  cctl;  // BAM only
   uint32  bcr;   // BAM only
   uint32  rctl;  // BAM only
   uint32  ddbg;  // BAM only
   uint32  fsm;   // BAM only
   uint32  ver;   // BAM only
   uint32  gen;   // BAM only
} HALUartRegisters;

struct HALUartFcnTable
{
   void   (*SetRxEnable)                (HALUartObject *uart, uint32 enabled);
   void   (*SetTxEnable)                (HALUartObject *uart, uint32 enabled);
   void   (*StartBreak)                 (HALUartObject *uart);
   void   (*StopBreak)                  (HALUartObject *uart);
   void   (*EnableInterrupts)           (HALUartObject *uart, uint32 flags);
   void   (*DisableInterrupts)          (HALUartObject *uart, uint32 flags);
   uint32 (*GetMaskedInterruptStatus)   (HALUartObject *uart);
   uint32 (*GetInterruptStatus)         (HALUartObject *uart);
   void   (*ClearInterrupts)             (HALUartObject *uart, uint32 flags);
   void   (*SetDivider)                 (HALUartObject *uart, uint32 divider);
   void   (*SetCharFormat)              (HALUartObject *uart, HALUartBitsPerCharType char_len,
                                                              HALUartNumStopBitsType stop_bits,
                                                              HALUartParityModeType parity);
   uint32 (*GetTxFifoSize)              (HALUartObject *uart);
   uint32 (*GetRxFifoSize)              (HALUartObject *uart);
   void   (*SetTxWatermark)             (HALUartObject *uart, uint32 num_bytes);
   void   (*SetRxWatermark)             (HALUartObject *uart, uint32 num_bytes);
   uint32 (*GetRxDataSize)              (HALUartObject *uart);
   uint32 (*ReadRxFifo)                 (HALUartObject *uart, uint8 *ptr,
                                                              uint32 num_bytes);
   uint32 (*WriteTxFifo)                (HALUartObject *uart, const uint8 *ptr,
                                                              uint32 num_bytes);
   void   (*SetStaleTimeout)            (HALUartObject *uart, uint32 stale_timeout);
   uint32 (*GetStatus)                  (HALUartObject *uart);
   void   (*SetAutoRfrLevel)            (HALUartObject *uart, uint32 num_bytes);
   void   (*SetRFR)                     (HALUartObject *uart, uint32 on);
   void   (*SetAutoInboundFlowCtl)      (HALUartObject *uart, uint32 enabled);
   void   (*SetOutboundFlowCtl)         (HALUartObject *uart, uint32 enabled);
   void   (*SetDmaNumTxChars)           (HALUartObject *uart, uint32 num_chars);
   void   (*GetRegisters)               (HALUartObject *uart, HALUartRegisters *registers);
   void   (*LoopbackSet)                (HALUartObject *uart, uint32 enabled);
   uint32 (*GetCTSStatus)               (HALUartObject *uart);
};

/*==================================================================================================
                                        EXTERN DECLARATIONS
==================================================================================================*/

// These external symbols will be defined in the UART type specific modules but they need
// to be declared here before using them in the inline functions below.

extern const HALUartFcnTable functions_uart_qupv3;

/*==================================================================================================
                                          INLINE FUNCTIONS
==================================================================================================*/
/*==================================================================================================

FUNCTION: HALuart_SetRxEnable

DESCRIPTION:
   Enables or disables the channel receiver according to the boolean "enabled"
   by writing the proper value to the UART command register bits 0 and 1.

==================================================================================================*/
static __inline
void HALuart_SetRxEnable(HALUartObject *uart, uint32 enabled)
{
   uart->fcn_table->SetRxEnable(uart, enabled);
}

/*==================================================================================================

FUNCTION: HALuart_SetTxEnable

DESCRIPTION:
   Enables or disables the channel transmitter according to boolean "enabled"
   by writing the proper value to the UART command register bits 2 and 3.
   On disabling terminates the operation of the transmitter after any character
   in the transmit shift register is sent.

==================================================================================================*/
static __inline
void HALuart_SetTxEnable(HALUartObject *uart, uint32 enabled)
{
   uart->fcn_table->SetTxEnable(uart, enabled);
}

/*==================================================================================================

FUNCTION: HALuart_StartBreak

DESCRIPTION:
   This API forces the transmitter signal low. The transmitter must be enabled.
   If the transmitter is busy, the break is started when all characters in the
   transmit FIFO and the transmit shift register have been completely sent.

==================================================================================================*/
static __inline
void HALuart_StartBreak(HALUartObject *uart)
{
   uart->fcn_table->StartBreak(uart);
}

/*==================================================================================================

FUNCTION: HALuart_StopBreak

DESCRIPTION:
   If executed while the transmit channel is breaking, this command causes the
   transmitter signal to go high. The signal remains high for at least one bit
   time before sending out a new character.

==================================================================================================*/
static __inline
void HALuart_StopBreak(HALUartObject *uart)
{
   uart->fcn_table->StopBreak(uart);
}

/*==================================================================================================

FUNCTION: HALuart_EnableInterrupts

DESCRIPTION:
   This function turns the input bits on and preserves the other bits in the mask.

==================================================================================================*/
static __inline
void HALuart_EnableInterrupts(HALUartObject *uart, uint32 flags)
{
   uart->fcn_table->EnableInterrupts(uart, flags);
}

/*==================================================================================================

FUNCTION: HALuart_DisableInterrupts

DESCRIPTION:
   The analog of EnableInterrupts.  The input bits are turned off, all other bits are preserved.

==================================================================================================*/
static __inline
void HALuart_DisableInterrupts(HALUartObject *uart, uint32 flags)
{
   uart->fcn_table->DisableInterrupts(uart, flags);
}

/*==================================================================================================

FUNCTION: HALuart_GetMaskedInterruptStatus

DESCRIPTION:
   Returns the "AND" of the interrupt status and the set interrupt mask

==================================================================================================*/
static __inline
uint32 HALuart_GetMaskedInterruptStatus(HALUartObject *uart)
{
   return uart->fcn_table->GetMaskedInterruptStatus(uart);
}

/*==================================================================================================

FUNCTION: HALuart_GetInterruptStatus

DESCRIPTION:
   Provides the current status of the possible interrupt conditions.

==================================================================================================*/
static __inline
uint32 HALuart_GetInterruptStatus(HALUartObject *uart)
{
   return uart->fcn_table->GetInterruptStatus(uart);
}

/*==================================================================================================

FUNCTION: HALuart_ClearInterrupts

DESCRIPTION:
    The interrupt bits mentioned in the 'flags' will be cleared.

==================================================================================================*/
static __inline
void HALuart_ClearInterrupts(HALUartObject *uart, uint32 flags)
{
   uart->fcn_table->ClearInterrupts(uart, flags);
}

/*==================================================================================================

FUNCTION: HALuart_SetDivider

DESCRIPTION:
   Sets the divider for the transmit/receive baud rates.  The hardware provides
   the ability to set the transmit and receive baud rates differently but this
   API currently forces you to set them to be the same.

==================================================================================================*/
static __inline
void HALuart_SetDivider(HALUartObject *uart, uint32 divider)
{
   uart->fcn_table->SetDivider(uart, divider);
}

/*==================================================================================================

FUNCTION: HALuart_SetCharFormat

DESCRIPTION:
   Sets how many bits are transmitted or received per character,
   the duration of the stop bit that is appended to each transmitted character,
   and which parity mode is used.

==================================================================================================*/
static __inline
void HALuart_SetCharFormat(HALUartObject *uart,
                           HALUartBitsPerCharType char_len,
                           HALUartNumStopBitsType stop_bits,
                           HALUartParityModeType parity)
{
   uart->fcn_table->SetCharFormat(uart, char_len, stop_bits, parity);
}

/*==================================================================================================

FUNCTION: HALuart_GetTxFifoSize

DESCRIPTION:
   Returns the size (in bytes) of the TX FIFO

==================================================================================================*/
static __inline
uint32 HALuart_GetTxFifoSize(HALUartObject *uart)
{
   return uart->fcn_table->GetTxFifoSize(uart);
}

/*==================================================================================================

FUNCTION: HALuart_GetRxFifoSize

DESCRIPTION:
   Returns the size (in bytes) of the RX FIFO

==================================================================================================*/
static __inline
uint32 HALuart_GetRxFifoSize(HALUartObject *uart)
{
   return uart->fcn_table->GetRxFifoSize(uart);
}

/*==================================================================================================

FUNCTION: HALuart_SetTxWatermark

DESCRIPTION:
   This API sets a value that determines the level of the transmit FIFO at
   which an interrupt is generated. The interrupt is generated when the
   number of bytes in the TX FIFO is less than or equal to num_bytes.

==================================================================================================*/
static __inline
void HALuart_SetTxWatermark(HALUartObject *uart, uint32 num_bytes)
{
   uart->fcn_table->SetTxWatermark(uart, num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_SetRxWatermark

DESCRIPTION:
   This API sets a value that determines the level of the receive FIFO at
   which an interrupt is generated. The interrupt is generated when the
   number of bytes in the RX FIFO is greater than or equal to num_bytes.

==================================================================================================*/
static __inline
void HALuart_SetRxWatermark(HALUartObject *uart, uint32 num_bytes)
{
   uart->fcn_table->SetRxWatermark(uart, num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_GetRxDataSize

DESCRIPTION:
   This API is used to calculate the number of bytes available in the RX fifo.

==================================================================================================*/

static __inline
uint32 HALuart_GetRxDataSize(HALUartObject *uart)
{
   return uart->fcn_table->GetRxDataSize(uart);
}

/*==================================================================================================

FUNCTION: HALuart_ReadRxFifo

DESCRIPTION:
   This API is used to copy data from the RX FIFO to a RAM buffer.
   This function will copy as much data as possible without overflowing
   the RAM buffer.

==================================================================================================*/
static __inline
uint32 HALuart_ReadRxFifo(HALUartObject *uart, uint8 *ptr, uint32 num_bytes)
{
   return uart->fcn_table->ReadRxFifo(uart, ptr, num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_WriteTxFifo

DESCRIPTION:
   This API is used to copy data from a RAM buffer to the TX FIFO.
   This function will copy as much data as possible without overrunning
   the FIFO.  It will return the number of bytes copied.

==================================================================================================*/
static __inline
uint32 HALuart_WriteTxFifo(HALUartObject *uart, const uint8 *ptr, uint32 num_bytes)
{
   return uart->fcn_table->WriteTxFifo(uart, ptr, num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_SetStaleTimeout

DESCRIPTION:
   This API sets the number of character times that may elapse without data
   received before a 'stale event' is generated. The character time is defined
   as 10 times the bit rate. "stale_timeout" may assume a value of 1 to
   2^(RAM_ADDR_WIDTH + 2) -1.

==================================================================================================*/
static __inline
void HALuart_SetStaleTimeout(HALUartObject *uart, uint32 stale_timeout)
{
   uart->fcn_table->SetStaleTimeout(uart, stale_timeout);
}

/*==================================================================================================

FUNCTION: HALuart_GetStatus

DESCRIPTION:
   Gets the state of the UART status register (SR).  This register is used to
   obtain the current state of the UART subsystem.  This register is updated
   asynchronously.

==================================================================================================*/
static __inline
uint32 HALuart_GetStatus(HALUartObject *uart)
{
   return uart->fcn_table->GetStatus(uart);
}

/*==================================================================================================

FUNCTION: HALuart_SetAutoRfrLevel

DESCRIPTION:
   Programs the level in the receive FIFO at which the RFR_N signal will be
   deasserted, if programmed to do so (see RX_RDY_CTL field of this register).
   The level counts the number of bytes inside the RX FIFO. It doesn't count
   the character that is being received (shift register) or characters in the
   packing buffer. This value is programmed from 1 to 2^RAM_ADDR_WIDTH.
   The RFR_N signal is deasserted when the RX FIFO level (the number of
   characters remaining in the RX FIFO) is greater than the level that is
   programmed into this register.

==================================================================================================*/
static __inline
void HALuart_SetAutoRfrLevel(HALUartObject *uart, uint32 num_bytes)
{
   uart->fcn_table->SetAutoRfrLevel(uart, num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_SetRFR

DESCRIPTION:
   When the automatic ready-for-receive feature is turned off by
   HALuart_SetAutoInboundFlowCtl, the RFR_N signal can be used by normal
   signal port bit manipulation via this API.

==================================================================================================*/
static __inline
void HALuart_SetRFR(HALUartObject *uart, uint32 on)
{
   uart->fcn_table->SetRFR(uart, on);
}

/*==================================================================================================

FUNCTION: HALuart_SetAutoInboundFlowCtl

DESCRIPTION:
   This API sets the current setting for the automatic ready-for-receiving
   (RFR_N) control for the receiver. If RFR_N is set (1), due to a valid
   start bit received when the channel FIFO is at the level programmed by
   HALuart_SetAutoRfrLevel then TRUE is returned. Else, if the FIFO level
   has fallen below the programmed level, the RFR_N signal is turned on, or
   cleared (0) and FALSE is returned. When this feature is off, the
   RFR_N signal can be used by normal signal port bit manipulation by
   HALuart_SetRFR.

==================================================================================================*/
static __inline
void HALuart_SetAutoInboundFlowCtl(HALUartObject *uart, uint32 enabled)
{
   uart->fcn_table->SetAutoInboundFlowCtl(uart, enabled);
}

/*==================================================================================================

FUNCTION: HALuart_SetOutboundFlowCtl

DESCRIPTION:
   When set bFlowEnabled TRUE), the transmitter checks the CTS_N input to
   determine whether to begin transmission of a new character. If CTS_N is
   low, the character is sent. Otherwise the transmitter continues marking
   until CTS_N goes low, then the next character is transmitted. A change on
   CTS_N during the transmission of a character has no effect on that
   character. When clear (bFlowEnabled FALSE), the CTS_N input for the channel
   has no effect on the transmitter.

==================================================================================================*/
static __inline
void HALuart_SetOutboundFlowCtl(HALUartObject *uart, uint32 enabled)
{
   uart->fcn_table->SetOutboundFlowCtl(uart, enabled);
}

/*==================================================================================================

FUNCTION: HALuart_SetDmaNumTxChars

DESCRIPTION:
   This API sets the total number of characters for transmission. Before
   writing a new value, the TX FIFO must be empty (as indicated by
   HAL_UART_TX_READY_IMR_FLAG set in result of
   HALuart_GetInterruptStatus or HALuart_GetMaskedInterruptStatus or
   after a reset). It is used by the transmitter to calculate how many
   characters to transmit in the last word. In DM mode it is also used for the
   CRCI mechanism. Any additional writes to the TX FIFO above the quantity set
   by this API will be discarded. The transmit-ready interrupt indicated by
   HAL_UART_TX_READY_IMR_FLAG is triggered after the quantity of characters
   set by this API is moved from the TX FIFO to the unpacking register.

==================================================================================================*/
static __inline
void HALuart_SetDmaNumTxChars(HALUartObject *uart, uint32 num_chars)
{
   uart->fcn_table->SetDmaNumTxChars(uart, num_chars);
}

/*==================================================================================================

FUNCTION: HALuart_GetRegisters

DESCRIPTION:
   Debug function.  Get the raw value from the registers without any processing.

==================================================================================================*/
static __inline
void HALuart_GetRegisters(HALUartObject *uart, HALUartRegisters *registers)
{
   uart->fcn_table->GetRegisters(uart, registers);
}

/*==================================================================================================

FUNCTION: HALuart_LoopbackSet

DESCRIPTION:
   Enable or disable loopback mode. This mode routes the TX line back to the RX line.
   For BAM UARTs, this mode also routes the RTS line back to the CTS line.
   The reason routing the RTS to CTS is only done on BAM UARTs is because it's only
   supported in HW on BAM UARTs. Loopbacks are done internally (inside the UART block).

==================================================================================================*/
static __inline
void HALuart_LoopbackSet(HALUartObject *uart, uint32 enabled)
{
   uart->fcn_table->LoopbackSet(uart, enabled);
}

/*==================================================================================================

FUNCTION: HALuart_GetStatus

DESCRIPTION:
   Gets the state of the UART status register (SR).  This register is used to
   obtain the current state of the UART subsystem.  This register is updated
   asynchronously.

==================================================================================================*/
static __inline
uint32 HALuart_GetCTSStatus(HALUartObject *uart)
{
   return uart->fcn_table->GetCTSStatus(uart);
}


/*==================================================================================================

FUNCTION: HALuart_InitObject

DESCRIPTION:
   Initializes the HALUart object.  Must be called first before any other HAL APIs.

==================================================================================================*/
static __inline
void HALuart_InitObject(HALUartObject *uart, HALUartType type, void *virt_addr, void *phys_addr)
{
   // The default case should be impossible to hit (unless HALUartType is extended without
   // updating this switch statement).  In this unlikely scenario a NULL pointer should
   // cause a data abort which will quickly alert the programmer to his mistake.

   switch (type)
   {
      case HAL_UART_QUPV3: uart->fcn_table = &functions_uart_qupv3; break;
      default:             uart->fcn_table = NULL;                  break;
   }

   uart->base        = virt_addr;
   uart->physical    = phys_addr;
   uart->last_error  = HAL_UART_SUCCESS;
}

/*==================================================================================================

FUNCTION: HALuart_GetLastError

DESCRIPTION:
   Returns the last error that occurred with this UART, clearing it.

==================================================================================================*/
static __inline
HALUartErrorType HALuart_GetLastError(HALUartObject *uart)
{
   HALUartErrorType last_error;

   last_error = uart->last_error;
   uart->last_error = HAL_UART_SUCCESS;
   return last_error;
}

#endif  // HALUART_QUPV3_H
