/*==================================================================================================

FILE: HALuart_bam.c

DESCRIPTION: HAL implementation for BAM-capable UARTs (UARTBAM)

                     Copyright (c) 2017-2018 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR

==================================================================================================*/
/*==================================================================================================

$Header: //components/rel/core.mpss/10.0/wiredconnectivity/uart/uart_qupv3/hal/src/HALuart_qupv3.c#1 $

==================================================================================================*/
/*==================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   (none)

==================================================================================================*/
/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include "HALhwio.h"
#include "HALcomdef.h"
#include "uart_hwio.h"
#include "HALuart_qupv3.h"
/*==================================================================================================
                                               MACROS
==================================================================================================*/

#define REG_IN(base, hwiosym) HWIO_##hwiosym##_IN(base)

#define REG_OUT(base, hwiosym, value) HWIO_##hwiosym##_OUT(base, value)

#define REG_INI(base, hwiosym, n) HWIO_##hwiosym##_INI(base, n)

#define REG_OUTI(base, hwiosym, n, value) HWIO_##hwiosym##_OUTI(base, n, value)

/*==================================================================================================
                                               CONSTANTS
==================================================================================================*/
// Different base addresses offset
#define GENI4_CFG           0x0
#define GENI4_IMAGE_REGS    0x100
#define GENI4_DATA          0x600
#define QUPV3_SE_DMA        0xC00
#define GENI4_IMAGE         0x1000

//SE
#define TX_FIFO_DEPTH_MASK      HWIO_SE_HW_PARAM_0_TX_FIFO_DEPTH_BMSK
#define TX_FIFO_DEPTH_SHIFT     HWIO_SE_HW_PARAM_0_TX_FIFO_DEPTH_SHFT

#define RX_FIFO_DEPTH_MASK      HWIO_SE_HW_PARAM_1_RX_FIFO_DEPTH_BMSK
#define RX_FIFO_DEPTH_SHIFT     HWIO_SE_HW_PARAM_1_RX_FIFO_DEPTH_SHFT


//FIFO
#define TX_FIFO_WC                   HWIO_GENI_TX_FIFO_STATUS_TX_FIFO_WC_BMSK

#define RX_LAST_VALID_BYTES_MASK     HWIO_GENI_RX_FIFO_STATUS_RX_LAST_BYTE_VALID_BMSK
#define RX_LAST_VALID_BYTES_SHIFT    HWIO_GENI_RX_FIFO_STATUS_RX_LAST_BYTE_VALID_SHFT
#define RX_FIFO_WC                   HWIO_GENI_RX_FIFO_STATUS_RX_FIFO_WC_BMSK
#define RX_FIFO_WC_SHIFT             HWIO_GENI_RX_FIFO_STATUS_RX_FIFO_WC_SHFT

//COMMANDS
#define M_CMD_SHIFT                  HWIO_GENI_M_CMD0_OPCODE_SHFT
#define S_CMD_SHIFT                  HWIO_GENI_S_CMD0_OPCODE_SHFT

// IRQ
#define M_IRQ_TF_FIFO_WATERMARK       HWIO_GENI_M_IRQ_STATUS_TX_FIFO_WATERMARK_BMSK
#define M_IRQ_CMD_DONE                HWIO_GENI_M_IRQ_STATUS_M_CMD_DONE_BMSK
#define M_IRQ_SEC                     HWIO_GENI_M_IRQ_STATUS_SEC_IRQ_BMSK
#define M_IRQ_TX_FIFO_WR_ERR          HWIO_GENI_M_IRQ_ENABLE_TX_FIFO_WR_ERR_EN_BMSK

#define S_IRQ_RX_LAST                 HWIO_GENI_S_IRQ_STATUS_RX_FIFO_LAST_BMSK
#define S_IRQ_RX_FIFO_WATERMARK       HWIO_GENI_S_IRQ_STATUS_RX_FIFO_WATERMARK_BMSK
#define S_IRQ_RX_BREAK_START          HWIO_GENI_S_IRQ_STATUS_S_GP_IRQ_2_BMSK
#define S_IRQ_RX_BREAK_END            HWIO_GENI_S_IRQ_STATUS_S_GP_IRQ_3_BMSK

#define TX_TRANS_CFG_CTS_MASK         HWIO_UART_TX_TRANS_CFG_UART_CTS_MASK_BMSK

#define GENI_IOS_CTS_MASK             HWIO_GENI_IOS_IO2_DATA_IN_BMSK
#define GENI_IOS_CTS_SHIFT            HWIO_GENI_IOS_IO2_DATA_IN_SHFT

/*==================================================================================================
                                             CONSTANTS
==================================================================================================*/
#define UART_PARITY_CFG_EVEN    0x0
#define UART_PARITY_CFG_ODD     0x2
#define UART_PARITY_CFG_SPACE   0x4
#define UART_PARITY_CFG_MARK    0x6
#define UART_PARITY_CFG_EN      0x1

#define UART_MANUAL_RFR_EN           0x80000000
#define UART_RFR_NOT_READY           0x1
#define UART_RFR_READY               0x0

#define UART_M_CMD_START_TRANSFER    0x1 << M_CMD_SHIFT
#define UART_M_CMD_START_BREAK       0x4 << M_CMD_SHIFT
#define UART_M_CMD_STOP_BREAK        0x5 << M_CMD_SHIFT

#define UART_S_CMD_START_TRANSFER    0x1 << S_CMD_SHIFT

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

static uint32 flags_to_intmask(uint32 flags);
static uint32 intmask_to_flags_m(uint32 intmask);
static uint32 intmask_to_flags_s(uint32 intmask);

static void   HALuart_qupv3_SetRxEnable                (HALUartObject *uart, uint32 enabled);
static void   HALuart_qupv3_SetTxEnable                (HALUartObject *uart, uint32 enabled);
static void   HALuart_qupv3_StartBreak                 (HALUartObject *uart);
static void   HALuart_qupv3_StopBreak                  (HALUartObject *uart);
static void   HALuart_qupv3_EnableInterrupts           (HALUartObject *uart, uint32 flags);
static void   HALuart_qupv3_DisableInterrupts          (HALUartObject *uart, uint32 flags);
static uint32 HALuart_qupv3_GetMaskedInterruptStatus   (HALUartObject *uart);
static uint32 HALuart_qupv3_GetInterruptStatus         (HALUartObject *uart);
static void   HALuart_qupv3_ClearInterrupts            (HALUartObject *uart, uint32 flags);
static void   HALuart_qupv3_SetDivider                 (HALUartObject *uart, uint32 divider);
static void   HALuart_qupv3_SetCharFormat              (HALUartObject *uart,
                                                         HALUartBitsPerCharType char_len,
                                                         HALUartNumStopBitsType stop_bits,
                                                         HALUartParityModeType parity);
static uint32 HALuart_qupv3_GetTxFifoSize              (HALUartObject *uart);
static uint32 HALuart_qupv3_GetRxFifoSize              (HALUartObject *uart);
static void   HALuart_qupv3_SetTxWatermark             (HALUartObject *uart, uint32 num_bytes);
static void   HALuart_qupv3_SetRxWatermark             (HALUartObject *uart, uint32 num_bytes);
static uint32 HALuart_qupv3_GetRxDataSize              (HALUartObject *uart);
static uint32 HALuart_qupv3_ReadRxFifo                 (HALUartObject *uart, uint8 *ptr,
                                                                           uint32 num_bytes);
static uint32 HALuart_qupv3_WriteTxFifo                (HALUartObject *uart, const uint8 *ptr,
                                                                           uint32 num_bytes);
static void   HALuart_qupv3_SetStaleTimeout            (HALUartObject *uart, uint32 stale_timeout);
static uint32 HALuart_qupv3_GetStatus                  (HALUartObject *uart);
static void   HALuart_qupv3_SetAutoRfrLevel            (HALUartObject *uart, uint32 num_bytes);
static void   HALuart_qupv3_SetRFR                     (HALUartObject *uart, uint32 on);
static void   HALuart_qupv3_SetAutoInboundFlowCtl      (HALUartObject *uart, uint32 enabled);
static void   HALuart_qupv3_SetOutboundFlowCtl         (HALUartObject *uart, uint32 enabled);
static void   HALuart_qupv3_SetDmaNumTxChars           (HALUartObject *uart, uint32 num_chars);
static void   HALuart_qupv3_GetRegisters               (HALUartObject *uart,
                                                         HALUartRegisters *registers);
static void   HALuart_qupv3_LoopbackSet                (HALUartObject *uart, uint32 enabled);
static uint32 HALuart_qupv3_GetCTSStatus               (HALUartObject *uart);

/*==================================================================================================
                                          GLOBAL VARIABLES
==================================================================================================*/

const HALUartFcnTable functions_uart_qupv3 =
{
   HALuart_qupv3_SetRxEnable,
   HALuart_qupv3_SetTxEnable,
   HALuart_qupv3_StartBreak,
   HALuart_qupv3_StopBreak,
   HALuart_qupv3_EnableInterrupts,
   HALuart_qupv3_DisableInterrupts,
   HALuart_qupv3_GetMaskedInterruptStatus,
   HALuart_qupv3_GetInterruptStatus,
   HALuart_qupv3_ClearInterrupts,
   HALuart_qupv3_SetDivider,
   HALuart_qupv3_SetCharFormat,
   HALuart_qupv3_GetTxFifoSize,
   HALuart_qupv3_GetRxFifoSize,
   HALuart_qupv3_SetTxWatermark,
   HALuart_qupv3_SetRxWatermark,
   HALuart_qupv3_GetRxDataSize,
   HALuart_qupv3_ReadRxFifo,
   HALuart_qupv3_WriteTxFifo,
   HALuart_qupv3_SetStaleTimeout,
   HALuart_qupv3_GetStatus,
   HALuart_qupv3_SetAutoRfrLevel,
   HALuart_qupv3_SetRFR,
   HALuart_qupv3_SetAutoInboundFlowCtl,
   HALuart_qupv3_SetOutboundFlowCtl,
   HALuart_qupv3_SetDmaNumTxChars,
   HALuart_qupv3_GetRegisters,
   HALuart_qupv3_LoopbackSet,
   HALuart_qupv3_GetCTSStatus,
};

/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================

FUNCTION: flags_to_intmask

DESCRIPTION:

==================================================================================================*/
static uint32 flags_to_intmask(uint32 flags)
{
   uint32 intmask = 0;

   if ( flags & HAL_UART_RXBREAK_END_IMR_FLAG    ) { intmask |= S_IRQ_RX_BREAK_END;           }
   if ( flags & HAL_UART_RXBREAK_START_IMR_FLAG  ) { intmask |= S_IRQ_RX_BREAK_START;         }
   if ( flags & HAL_UART_TX_READY_IMR_FLAG       ) { intmask |= M_IRQ_CMD_DONE;               }
   if ( flags & HAL_UART_RXLEV_IMR_FLAG          ) { intmask |= S_IRQ_RX_FIFO_WATERMARK;      }
   if ( flags & HAL_UART_RXSTALE_IMR_FLAG        ) { intmask |= S_IRQ_RX_LAST;                }
   if ( flags & HAL_UART_TXLEV_IMR_FLAG          ) { intmask |= M_IRQ_TF_FIFO_WATERMARK;      }

   return intmask;
}

/*==================================================================================================

FUNCTION: intmask_to_flags

DESCRIPTION:

==================================================================================================*/
static uint32 intmask_to_flags_m(uint32 intmask)
{
   uint32 flags = 0;

   if ( intmask & M_IRQ_CMD_DONE             ) { flags |= HAL_UART_TX_READY_IMR_FLAG;       }
   if ( intmask & M_IRQ_TF_FIFO_WATERMARK    ) { flags |= HAL_UART_TXLEV_IMR_FLAG;          }

   return flags;
}

/*==================================================================================================

FUNCTION: intmask_to_flags

DESCRIPTION:

==================================================================================================*/
static uint32 intmask_to_flags_s(uint32 intmask)
{
   uint32 flags = 0;

   if ( intmask & S_IRQ_RX_BREAK_END             ) { flags |= HAL_UART_RXBREAK_END_IMR_FLAG;    }
   if ( intmask & S_IRQ_RX_BREAK_START           ) { flags |= HAL_UART_RXBREAK_START_IMR_FLAG;  }
   if ( intmask & S_IRQ_RX_FIFO_WATERMARK        ) { flags |= HAL_UART_RXLEV_IMR_FLAG;          }
   if ( intmask & S_IRQ_RX_LAST                  ) { flags |= HAL_UART_RXSTALE_IMR_FLAG;        }

   return flags;
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetRxEnable

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetRxEnable(HALUartObject *uart, uint32 enabled)
{
   REG_OUT(uart->base + GENI4_DATA, GENI_S_CMD0, UART_S_CMD_START_TRANSFER);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetTxEnable

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetTxEnable(HALUartObject *uart, uint32 enabled)
{
   return;
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_StartBreak

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_StartBreak(HALUartObject *uart)
{
   REG_OUT(uart->base + GENI4_DATA, GENI_M_CMD0, UART_M_CMD_START_BREAK);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_StopBreak

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_StopBreak(HALUartObject *uart)
{
   REG_OUT(uart->base + GENI4_DATA, GENI_M_CMD0, UART_M_CMD_STOP_BREAK);
}


/*==================================================================================================

FUNCTION: HALuart_qupv3_EnableInterrupts

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_EnableInterrupts(HALUartObject *uart, uint32 flags)
{
   uint32 int_mask = flags_to_intmask(flags);

   uint32 rx_masks = HAL_UART_RXSTALE_IMR_FLAG        |
                     HAL_UART_RXLEV_IMR_FLAG          |
                     HAL_UART_RXBREAK_START_IMR_FLAG  |
                     HAL_UART_RXBREAK_END_IMR_FLAG;

   if ( flags & rx_masks)
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_S_IRQ_EN_SET, int_mask);
      REG_OUT(uart->base + GENI4_DATA, GENI_M_IRQ_EN_SET, M_IRQ_SEC);
   }
   else
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_M_IRQ_EN_SET, int_mask);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_DisableInterrupts

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_DisableInterrupts(HALUartObject *uart, uint32 flags)
{
   uint32 int_mask = flags_to_intmask(flags);

   uint32 rx_masks = HAL_UART_RXSTALE_IMR_FLAG        |
                     HAL_UART_RXLEV_IMR_FLAG          |
                     HAL_UART_RXBREAK_START_IMR_FLAG  |
                     HAL_UART_RXBREAK_END_IMR_FLAG;

   if ( flags & rx_masks)
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_S_IRQ_EN_CLEAR, int_mask);
   }
   else
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_M_IRQ_EN_CLEAR, int_mask);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetMaskedInterruptStatus

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetMaskedInterruptStatus(HALUartObject *uart)
{
   uint32 s_irq_status;
   uint32 m_irq_status;

   m_irq_status = REG_IN(uart->base + GENI4_DATA, GENI_M_IRQ_STATUS);
   s_irq_status = REG_IN(uart->base + GENI4_DATA, GENI_S_IRQ_STATUS);

   m_irq_status = m_irq_status & REG_IN(uart->base + GENI4_DATA, GENI_M_IRQ_ENABLE);
   s_irq_status = s_irq_status & REG_IN(uart->base + GENI4_DATA, GENI_S_IRQ_ENABLE);

   return (intmask_to_flags_m(m_irq_status) | intmask_to_flags_s(s_irq_status));
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetInterruptStatus

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetInterruptStatus(HALUartObject *uart)
{
   uint32 s_irq_status;
   uint32 m_irq_status;

   m_irq_status = REG_IN(uart->base + GENI4_DATA, GENI_M_IRQ_STATUS);
   s_irq_status = REG_IN(uart->base + GENI4_DATA, GENI_S_IRQ_STATUS);

   return (intmask_to_flags_m(m_irq_status) | intmask_to_flags_s(s_irq_status));
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_ClearInterrupts

DESCRIPTION:

==================================================================================================*/
static void   HALuart_qupv3_ClearInterrupts (HALUartObject *uart, uint32 flags)
{
uint32 int_mask = flags_to_intmask(flags);

   uint32 rx_masks = HAL_UART_RXSTALE_IMR_FLAG        |
                     HAL_UART_RXLEV_IMR_FLAG          |
                     HAL_UART_RXBREAK_START_IMR_FLAG  |
                     HAL_UART_RXBREAK_END_IMR_FLAG;

   if ( flags & rx_masks)
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_S_IRQ_CLEAR, int_mask);
   }
   else
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_M_IRQ_CLEAR, int_mask);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetDivider

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetDivider(HALUartObject *uart, uint32 divider)
{
   uint32 reg_val = 0;
   if ((divider % 32) != 0)
   {
      uart->last_error = HAL_UART_NOT_SUPPORTED;
   }
   else
   {
      reg_val = ((divider/32) << 4) | 0x1 ;
      REG_OUT(uart->base + GENI4_CFG, GENI_SER_M_CLK_CFG, reg_val);
      REG_OUT(uart->base + GENI4_CFG, GENI_SER_S_CLK_CFG, reg_val);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetCharFormat

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetCharFormat(HALUartObject *uart,
                                      HALUartBitsPerCharType char_len,
                                      HALUartNumStopBitsType stop_bits,
                                      HALUartParityModeType parity)
{
   uint32 reg_val = 0;

   switch (char_len)
   {
      case HAL_UART_5_BITS_PER_CHAR: reg_val = 5; break;
      case HAL_UART_6_BITS_PER_CHAR: reg_val = 6; break;
      case HAL_UART_7_BITS_PER_CHAR: reg_val = 7; break;
      default:
      case HAL_UART_8_BITS_PER_CHAR: reg_val = 8; break;
   }
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_WORD_LEN, reg_val);
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_RX_WORD_LEN, reg_val);

   switch (stop_bits)
   {
      case HAL_UART_0_5_STOP_BITS: reg_val = 0; break;
      default:
      case HAL_UART_1_0_STOP_BITS: reg_val = 0;  break;
      case HAL_UART_1_5_STOP_BITS: reg_val = 1;  break;
      case HAL_UART_2_0_STOP_BITS: reg_val = 2;  break;
   }
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_STOP_BIT_LEN, reg_val);

   switch (parity)
   {
      default:
      case HAL_UART_NO_PARITY:
         reg_val = 0;
         break;

      case HAL_UART_EVEN_PARITY:
         reg_val = UART_PARITY_CFG_EVEN  | UART_PARITY_CFG_EN;
         break;

      case HAL_UART_ODD_PARITY:
         reg_val = UART_PARITY_CFG_ODD   | UART_PARITY_CFG_EN;
         break;

      case HAL_UART_SPACE_PARITY:
         reg_val = UART_PARITY_CFG_SPACE | UART_PARITY_CFG_EN;
         break;
   }

   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_PARITY_CFG, reg_val);
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_RX_PARITY_CFG, reg_val);

   if (reg_val != 0)
   {
      reg_val = REG_IN(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG) | 0x1;

      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG, reg_val );
      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_RX_TRANS_CFG, 0x10);
   }
   else
   {
      reg_val = (REG_IN(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG) & ~0x1);

      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG, reg_val);
      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_RX_TRANS_CFG, 0x0);
   }

   REG_OUT(uart->base + GENI4_IMAGE_REGS, GENI_TX_PACKING_CFG0, 0x4380E);
   REG_OUT(uart->base + GENI4_IMAGE_REGS, GENI_TX_PACKING_CFG1, 0xC3E0E);
   REG_OUT(uart->base + GENI4_IMAGE_REGS, GENI_RX_PACKING_CFG0, 0x4380E);
   REG_OUT(uart->base + GENI4_IMAGE_REGS, GENI_RX_PACKING_CFG1, 0xC3E0E);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetTxFifoSize

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetTxFifoSize(HALUartObject *uart)
{
   uint32 temp;

   temp = REG_IN(uart->base + QUPV3_SE_DMA, SE_HW_PARAM_0); // to get TXFIFO depth
   temp = (temp & TX_FIFO_DEPTH_MASK) >> TX_FIFO_DEPTH_SHIFT;

   return temp << 2;
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetRxFifoSize

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetRxFifoSize(HALUartObject *uart)
{
   uint32 temp;

   temp = REG_IN(uart->base + QUPV3_SE_DMA, SE_HW_PARAM_1);// to get RXFIFO depth
   temp = (temp & RX_FIFO_DEPTH_MASK) >> RX_FIFO_DEPTH_SHIFT;

   return temp << 2;
}
/*==================================================================================================

FUNCTION: HALuart_qupv3_SetTxWatermark

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetTxWatermark(HALUartObject *uart, uint32 num_bytes)
{
   if ( num_bytes & 0x03 )  // must be a multiple of 4 bytes
   {
      uart->last_error = HAL_UART_BAD_PARAMETER;
   }
   else
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_TX_WATERMARK_REG, num_bytes >> 2);  // TFWR expressed in 4-byte words
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetRxWatermark

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetRxWatermark(HALUartObject *uart, uint32 num_bytes)
{

   if ( num_bytes == 0 || num_bytes & 0x03 )  // must be nonzero and a multiple of 4 bytes
   {
      uart->last_error = HAL_UART_BAD_PARAMETER;
   }
   else
   {
      REG_OUT(uart->base + GENI4_DATA, GENI_RX_WATERMARK_REG, (num_bytes >> 2) - 1);
                                                        // RX watermark expressed in 4-byte words;
                                                        // subtract 1 because the UART HW sets
                                                        // the RXLEV bit when the number of words
                                                        // in the RXFIFO is *greater* than the
                                                        // value in RX_WATERMARK_REG
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetRxDataSize

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetRxDataSize(HALUartObject *uart)
{
   uint32 rx_fifo_status;
   uint32 words_to_read;
   uint32 partial_bytes_to_read;

   rx_fifo_status = REG_IN(uart->base + GENI4_DATA, GENI_RX_FIFO_STATUS);

   partial_bytes_to_read = (rx_fifo_status & RX_LAST_VALID_BYTES_MASK) >> RX_LAST_VALID_BYTES_SHIFT;
   words_to_read = rx_fifo_status & RX_FIFO_WC;
   if (partial_bytes_to_read != 0 && partial_bytes_to_read != 4)
   {
      words_to_read -= 1;
   }

   return( (words_to_read * 4) + partial_bytes_to_read );
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_ReadRxFifo

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_ReadRxFifo(HALUartObject *uart, uint8 *ptr, uint32 num_bytes)
{
   uint32  bytes_in_rxfifo = 0;                           
   uint32  partial_bytes_to_read = num_bytes & 0x3;
   uint32  words_to_read = num_bytes >> 2;
   uint32  rx_word;
   uint32 i;
   
   bytes_in_rxfifo = HALuart_qupv3_GetRxDataSize(uart);
   
   if(num_bytes > bytes_in_rxfifo)
   {
	   num_bytes = bytes_in_rxfifo;
   }
   
   if ( (ptr - (uint8 *)NULL) & 0x03 )  // ptr is not 32-bit word aligned
   {
      for (i = 0; i < words_to_read; i++)
      {
         rx_word = REG_INI(uart->base + GENI4_DATA, GENI_RX_FIFOn, 0);
         ptr[0] = (uint8)(rx_word >>  0);
         ptr[1] = (uint8)(rx_word >>  8);
         ptr[2] = (uint8)(rx_word >> 16);
         ptr[3] = (uint8)(rx_word >> 24);
         ptr += 4;
      }
   }
   else  // ptr is 32-bit word aligned
   {
      for (i = 0; i < words_to_read; i++)
      {
         rx_word = REG_INI(uart->base + GENI4_DATA, GENI_RX_FIFOn, 0);
         *(uint32 *)ptr = rx_word;
         ptr += 4;
      }
   }

   if (partial_bytes_to_read)
   {
      rx_word = REG_INI(uart->base + GENI4_DATA, GENI_RX_FIFOn, 0);
      for (i = 0; i < partial_bytes_to_read; i++) { ptr[i] = (uint8)(rx_word >> i * 8); }
   }

   return(num_bytes);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_WriteTxFifo

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_WriteTxFifo(HALUartObject *uart, const uint8 *ptr, uint32 num_bytes)
{
   uint32 full_words = num_bytes >> 2;
   uint32 partial_bytes = num_bytes & 0x03;
   uint32 empty_slots;
   uint32 words_in_fifo;
   uint32 tx_fifo_size;
   uint32 word_value;
   uint32 words_to_send;
   uint32 bytes_to_send;
   uint32 i;

   words_in_fifo = REG_IN(uart->base + GENI4_DATA, GENI_TX_FIFO_STATUS) & TX_FIFO_WC;

   tx_fifo_size = (REG_IN(uart->base + QUPV3_SE_DMA, SE_HW_PARAM_0) & TX_FIFO_DEPTH_MASK) >> TX_FIFO_DEPTH_SHIFT;

   empty_slots = tx_fifo_size - words_in_fifo;

   words_to_send = (full_words >  empty_slots) ? (empty_slots) : (full_words);
   bytes_to_send = (full_words >= empty_slots) ? (0) : (partial_bytes);

   if ( (ptr - (uint8 *)NULL) & 0x03 )  // buffer is not 32-bit word aligned
   {
      for (i = 0; i < words_to_send; i++)
      {
         word_value = ptr[0] << 0 | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;
         REG_OUTI(uart->base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
         ptr += 4;
      }
   }
   else  // buffer is 32-bit word aligned
   {
      for (i = 0; i < words_to_send; i++)
      {
         word_value = *(uint32 *)ptr;
         REG_OUTI(uart->base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
         ptr += 4;
      }
   }

   if (bytes_to_send)
   {
      word_value = 0;
      for (i = 0; i < bytes_to_send; i++) { word_value |= ptr[i] << i * 8; }
      REG_OUTI(uart->base + GENI4_DATA, GENI_TX_FIFOn, 0, word_value);
   }

   return(words_to_send * 4 + bytes_to_send);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetStaleTimeout

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetStaleTimeout(HALUartObject *uart, uint32 stale_timeout)
{
   // Stale timeout is in bit times, so hard coding it for 10 bits per byte here.
   // TODO: Check the stop bit length and parity config and decide bit length.
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_RX_STALE_CNT, stale_timeout*10);

}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetStatus

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetStatus(HALUartObject *uart)
{
   return( 0 );
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetAutoRfrLevel

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetAutoRfrLevel(HALUartObject *uart, uint32 num_bytes)
{
   if ( num_bytes == 0 || num_bytes & 0x03 )  // must be nonzero and a multiple of 4 bytes
   {
      uart->last_error = HAL_UART_BAD_PARAMETER;
   }
   else
   {
      uint32 num_words = (num_bytes >> 2) - 1;  // subtract 1 because the UART HW deasserts
                                                // RTS when the number of words in the RXFIFO
                                                // is *greater* than the value in RFR_AUTO_LEVEL

      REG_OUT(uart->base + GENI4_DATA, GENI_RX_RFR_WATERMARK_REG, num_words);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetRFR

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetRFR(HALUartObject *uart, uint32 on)
{
   uint32 reg_val = UART_MANUAL_RFR_EN;

   (on) ? (reg_val |= UART_RFR_NOT_READY) : (reg_val |= UART_RFR_READY);

   REG_OUT(uart->base + GENI4_DATA, UART_MANUAL_RFR, reg_val);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetAutoInboundFlowCtl

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetAutoInboundFlowCtl(HALUartObject *uart, uint32 enabled)
{
   uint32 reg_val = 0;

   reg_val = (enabled) ?  0 : 1;
   REG_OUT(uart->base + GENI4_DATA, UART_MANUAL_RFR, reg_val);

}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetOutboundFlowCtl

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetOutboundFlowCtl(HALUartObject *uart, uint32 enabled)
{
   uint32 reg_val = 0;

   reg_val = REG_IN(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG);

   if (enabled) { reg_val &= ~TX_TRANS_CFG_CTS_MASK; }
   else         { reg_val |=  TX_TRANS_CFG_CTS_MASK; }

   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_CFG, reg_val);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_SetDmaNumTxChars

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_SetDmaNumTxChars(HALUartObject *uart, uint32 num_chars)
{
   REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_TX_TRANS_LEN, num_chars);
   REG_OUT(uart->base + GENI4_DATA, GENI_M_CMD0, UART_M_CMD_START_TRANSFER);
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetRegisters

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_GetRegisters(HALUartObject *uart, HALUartRegisters *registers)
{
   return;
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_LoopbackSet

DESCRIPTION:

==================================================================================================*/
static void HALuart_qupv3_LoopbackSet(HALUartObject *uart, uint32 enabled)
{
   if (enabled)
   {
      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_LOOPBACK_CFG, 0x3);
   }
   else
   {
      REG_OUT(uart->base + GENI4_IMAGE_REGS, UART_LOOPBACK_CFG, 0x0);
   }
}

/*==================================================================================================

FUNCTION: HALuart_qupv3_GetCTSStatus

DESCRIPTION:

==================================================================================================*/
static uint32 HALuart_qupv3_GetCTSStatus(HALUartObject *uart)
{
   uint32 geni_ios;
   geni_ios = REG_IN(uart->base + GENI4_DATA, GENI_IOS);
   return ( geni_ios & GENI_IOS_CTS_MASK ) >> GENI_IOS_CTS_SHIFT;
}

