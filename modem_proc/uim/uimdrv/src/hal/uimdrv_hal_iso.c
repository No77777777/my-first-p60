/*===========================================================================

                            U I M D R V  H A L  I S O 

DESCRIPTION
  This file contains driver functions that are specific to ISO UIM

Copyright (c) 2015 - 2019 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimdrv_hal_iso.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------01/16/19   nr      Workaround to avoid glitch on UIM lines
12/26/18   ng      Fixed KW error
10/31/17   gm      Delay before clock stop optimization
03/13/17   ku      Common path for local and remote card
01/31/17   ks      Clear transaction timer upon clock off
11/24/16   ks      Adding support for spcom interface
10/14/16   ku      APDU Logging for extended log packets
06/17/16   gm      Stub out Hotswap related functionality
06/17/16   gm      F3 Optimization
07/15/16   sam     Avoid uart register access in force_Recovery when 
                   remote card is active
06/07/16   gm      F3 optimization
01/27/16   sam     clear any pending RX_STALE interrupts before intiating 
                   any T=1 block transfer to card
01/06/16   ks      Do not turn on CLK when remote SIM is connected
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "assert.h"
#include "intconv.h"
#include "uimi.h"
#include "uimdrv_clk.h"
#include "uimdrv_gpio.h"
#include "uimdrv_hal_iso.h"
#include "uimdrv_msg.h"
#include "uimdrv_pmic.h"
#include "uimdrv_uartdm.h"
#include "uimutil.h"
#include "uimsub_manager.h"
#include "uim_remote.h"
#include "uim_nvcfg.h"
#include "uim_logging.h"
#include "uim_rxtx.h"
#include "uimdrv.h"

#if defined( FEATURE_UIM_T_1_SUPPORT )
#include "crc.h"
#include "uimt1.h"                 /* T=1 protocol support */
#endif /* FEATURE_UIM_T_1_SUPPORT */

/*============================================================================= 
  INTERNAL FUNCTION PROTOTYPES
=============================================================================*/
void uim_send_apdu_command_hdr
(
  uim_instance_global_type *uim_ptr
);

void uim_send_pps_bytes
(
  uim_instance_global_type *uim_ptr
);

/*============================================================================= 
  FUNCTION UIMDRV_ISO_POWER_UP
=============================================================================*/
void uimdrv_iso_power_up
(
  uim_instance_global_type *uim_ptr
)
{

  /* Warm reset or cold reset depending on the voltage class */
  if(uim_hw_if.pmic[uim_ptr->id].m_voltage_class == UIM_VOLTAGE_CLASS_INVALID)
  {
    uim_log_put_reset_data(TRUE, uim_ptr);
  }
  else
  {
    uim_log_put_reset_data(FALSE, uim_ptr);
  }

  /* Program the currently selected voltage */
  switch (uim_ptr->state.current_voltage_class)
  {
    case UIM_VOLTAGE_CLASS_C:
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_43,uim_ptr->id, "uim power up @ 1.8 v");
        uim_program_voltage_class( uim_ptr, UIM_VOLTAGE_CLASS_C );
      } /* end case - UIM_VOLTAGE_1_8_V */
      break;

    case UIM_VOLTAGE_CLASS_B:
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_44,uim_ptr->id, "uim power up @ 3 v");
        uim_program_voltage_class( uim_ptr, UIM_VOLTAGE_CLASS_B );
      } /* end case - UIM_VOLTAGE_3V */
      break;

    default:
      {
        return;
      }
  }

  /* Turn on the clock for the R-UIM interface */
  UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

  /* First, turn on the UIM LDO. */
  uim_power_on_ldo_slot(uim_ptr);

  /* Next, place the I/O line in reception mode. */
  UIM_STOP_BREAK_SLOT(uim_ptr);

  UIM_CONFIGURE_DATA_FOR_UIM_CONTROLLER(uim_ptr);
  uim_clk_busy_wait(500);

  uim_uartdm_uim_controller_recover(uim_ptr);
  uim_clk_busy_wait(200);

  UIM_CONFIGURE_CLK_FOR_UIM_CONTROLLER(uim_ptr);
  uim_clk_busy_wait(200);

  /* Setup the UIM clock based on clock frequency set by HW enumeration */
  if (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq ==
      uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ])
  {
    uim_clock_control (uim_ptr->id, UIMDRV_CLK_FREQ_3_84_MHZ);
  }
  else if(uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq ==
          uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ])
  {
    uim_clock_control (uim_ptr->id, UIMDRV_CLK_FREQ_4_8_MHZ);
  }
  /* Reset the receiver so that the receiver does not process the stop break
     as a received byte */
  UIM_RESET_RX_SLOT (uim_ptr);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
} /* uimdrv_iso_power_up */


/*============================================================================= 
  FUNCTION UIMDRV_ISO_POWER_DOWN
=============================================================================*/
void uimdrv_iso_power_down
(
  uim_instance_global_type *uim_ptr
)
{
  /* Program the currently selected voltage */
  switch (uim_ptr->state.current_voltage_class)
  {
    case UIM_VOLTAGE_CLASS_C:
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_50,uim_ptr->id, "uim power down @ 1.8 v");	  
      break;

    case UIM_VOLTAGE_CLASS_B:
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_51,uim_ptr->id, "uim power down @ 3 v");
      break;

    default:
      break;
  }

  /* Make sure the clock is on for the R-UIM interface */
  UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

  /* Mask all interrupts from UART. */
  UIM_PRG_IMR_SLOT(uim_ptr, MSMU_IMR_MASK_ALL );

  /* Reset the UART receiver and disable */
  UIM_RESET_RX_SLOT (uim_ptr);

  UIMDRV_MSG_LOW_0(uim_ptr->id, "uim_power_down" );

  /* Switch the user of the UIM GPIOs to TLMM and set to PULL_DOWN */
  UIM_CONFIGURE_RESET_DEFAULT_GPIO(uim_ptr);

  /* Set the reset active */
  UIM_ASSERT_RESET_SLOT (uim_ptr);

  /* Wait for 100 clock cycles before turning the clock off. */
  uim_clk_busy_wait ((100*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  /* Turn the clock to the UIM off */
  uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_LOW);

  UIM_CONFIGURE_CLK_DEFAULT_GPIO(uim_ptr);

  /* Wait for 100 clock cycles before setting I/O line low. */
  uim_clk_busy_wait ((100*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  /* Configure data to TLMM */
  UIM_CONFIGURE_DATA_DEFAULT_GPIO(uim_ptr);

   /* Wait for 100 clock cycles before setting IO line low. */
  uim_clk_busy_wait ((100*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  /* Set the I/O line Low */
  UIM_START_BREAK_SLOT (uim_ptr);

  UIM_CONFIGURE_DATA_DEFAULT_GPIO(uim_ptr);

  /* Wait for 100 clock cycles before turning power off. */
  uim_clk_busy_wait ((100*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  /* Turn off the UIM LDO */
  uim_power_off_ldo_slot (uim_ptr);

  /* Voltage rest to INVALID on power down */
  uim_program_voltage_class( uim_ptr, UIM_VOLTAGE_CLASS_INVALID );

  /* Wait an extra 2msecs for the PMIC to power down.
  If this causes problems in the build, remove the code and get the phone
  changed to power down the Vcc quicker. */
  uim_clk_busy_wait ( UIM_MICROSECONDS_PER_TWO_MILLISECONDS );

  /* clear the timer and the signal just in case
   * there is a dangling timer/signal
   */

  /* Clear the timer first */
  uim_clear_cmd_rsp_timer(uim_ptr);

  /* Now clear the signal */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  /* Allow the clock for the R-UIM interface to be turned off */
  UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);

  /*
    Setting the GPIO's to default after we power down, during power up
    we can directly drive these gpio's and assign it to uim controller
  */

  /* Switch the user of the UIM GPIOs to TLMM and set to PULL_DOWN */
  UIM_CONFIGURE_RESET_DEFAULT_GPIO(uim_ptr);

  UIM_CONFIGURE_CLK_DEFAULT_GPIO(uim_ptr);

  UIM_CONFIGURE_DATA_DEFAULT_GPIO(uim_ptr);

  uim_clk_busy_wait(200);

  /* Flush the remianing bytes in the log buffer */
  uim_log_put_last_byte(uim_ptr);

  /* Logging for extended packets */
  uim_log_put_power_off_data(uim_ptr);
} /* uimdrv_iso_power_down */


/*============================================================================= 
  FUNCTION UIMDRV_ISO_RESET
=============================================================================*/
void uimdrv_iso_reset
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  /* Stores the maximum number of UIM clock cycles to wait for ATR */
  int32                       max_count       = 0;

  UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_reset" );
  /* Mask off all interrupts */
  UIM_PRG_IMR_SLOT(uim_ptr, MSMU_IMR_MASK_ALL);

  if(uim_ptr->atr.is_parity_in_atr)
  {
    /* enable the parity error work-around as ATR has been received once*/
    UIM_PRG_IPR_SLOT(uim_ptr, MSMU_IPR_DEF_STALE_TIMEOUT);
    UIM_PRG_MR2_SLOT(uim_ptr, (MSMU_MR2_8BPC | MSMU_MR2_2SB | MSMU_MR2_EPAR| MSMU_MR2_RX_ERROR_CHAR_OFF | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF));
  }
  else
  {
    /* Set the stale timeout: Do not enable the parity error workaround in hardware */
    UIM_PRG_IPR_SLOT(uim_ptr, MSMU_IPR_DEF_STALE_TIMEOUT);
  }

  if( uim_nv_is_feature_enabled(UIMDRV_FEATURE_ENABLE_BOOT_UP_IN_INVERSE_CONVENTION, uim_ptr) == TRUE )
  {
    if(uim_ptr->state.convention == UIM_INVERSE_CONVENTION)
    {
      /* Set the convention back to "inverse" */
      UIM_PROGRAM_INVERSE_CONVENTION_SLOT(uim_ptr);
    }
    else
    {
      UIM_PROGRAM_DIRECT_CONVENTION_SLOT(uim_ptr);
    }
  }

  /* Turn OFF the ability to echo back sent bytes.
     Set SIM_CFG: MASK_RX to 1 */
  UIM_UART_CNFG_SLOT(uim_ptr,UART_SIM_CFG__MASK_RX, 8);

  /* Reset/flush the UART just in case there was something going on. */
  uim_reset_uart_slot (uim_ptr);

/*
   Configuring the reset gpio in uim_reset to be a tlmm gpio.
   In deassert reset we give control back to uim controller.
   We do not pull the gpio high and give control to uim controller,
   as uim controller does that for us.
*/

  UIM_CONFIGURE_RESET_DEFAULT_GPIO(uim_ptr);

  /* Assert the reset line */
  UIM_ASSERT_RESET_SLOT (uim_ptr);

  /* Enable the UART transmitter */
  UIM_ENABLE_UART_TX_SLOT (uim_ptr);

  /* Enable the UART receiver */
  UIM_ENABLE_UART_RX_SLOT (uim_ptr);

  uim_ptr->card_cmd.resp_buf_ptr = rsp_ptr;
  uim_ptr->card_cmd.resp_buf_byte_ptr = (byte *)&rsp_ptr->rsp;

  /* Set up all the driver variables to expect an ATR. */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_RECEIVE_ATR;
  uim_ptr->rxtx_state_machine.rx_sub_state = UIM_RX_WAIT_FOR_RXSTALE;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;
  uim_ptr->card_cmd.num_resp_bytes_rcvd = 0;
  uim_ptr->atr.check_byte_expected = FALSE;
  uim_ptr->atr.check_byte = 0;
  uim_ptr->atr.num_historical_chars = 0;
  uim_ptr->atr.num_bytes_to_skip = 2;
  uim_ptr->atr.receive_mode = UIM_WAITING_FOR_T0;

  /* This status is returned in the Normal end SW processing.
     It distinguishes between data download error and all other
     results. */
  uim_ptr->flag.processing_download_error = FALSE;

  /* Set the byte response delay to the default */
  uim_ptr->setting.response_byte_delay = UIM_RESPONSE_BYTE_DELAY_DEFAULT;

  /* Set the UART clock to the default values. */
  SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1);

  /* Wait for 200 clock cycles before enabling the receiver. */
  uim_clk_busy_wait ((200*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  /* Wait another 200 clock cycles (at 2.46Mhz) to deassert the reset. */
  uim_clk_busy_wait ((200*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_NO_ATR_IN_40000_CLK_CYCLES,
                               uim_ptr) == TRUE)
  {
    /* reset the flag right before releasing the reset line */
    uim_ptr->atr.atr_byte_received = FALSE;
  }

  /* Initialize a new RX transfer */
  /* The ATR size cannot exceed 0x30 */
  UIM_DM_RX_INIT_SLOT(uim_ptr, 0x30);

  /* Enable the stale character event */
  UIM_ENABLE_RX_STALE_INT_SLOT(uim_ptr);

  /* Some how this delay is needed for LCU UART.
     Will need to find out ways to remove this delay. NEEDED FOR ULC? */
  uim_clk_busy_wait (500);

  /* Enable the receive interrupts - stale character timeout,
     receive break condition, parity frame error */
  UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_WWT) );

  /* De-assert the reset line */
  UIM_DEASSERT_RESET_SLOT (uim_ptr);

  /* Enable the receive interrupts - stale character timeout,
     receive break condition, parity frame error */
  UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_WWT) );

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_NO_ATR_IN_40000_CLK_CYCLES,
                               uim_ptr) == TRUE)
  {
    /* Set the maximum UIM clock cycles to wait */
    max_count = UIM_ISO7816_MAX_ATRWAIT_UIMCYCLES + UIM_BYTE_TIME \
                + uim_ptr->setting.config_params.additional_delay_for_atr;

    /* Start the 40K UIM clock cycles equivalent SCLK timer */
    timer_set(&uim_ptr->atr.atr_40k_timer,
              (timetick_type)
              ((max_count*SLEEP_CLK_FREQ)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq),
              0, T_SCLK);
  }
  else
  {
    if((uim_ptr->state.DI_current < UIM_BRAF_SIZE) &&
        (uim_ptr->state.FI_current < UIM_CRCF_SIZE))
    {

        uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);

      uim_start_cmd_rsp_timer( uim_ptr->card_cmd.work_waiting_time +
                               UIM_UART_DM_WAITING_TIME_CORRECTION,
                               uim_ptr);
    }
    else
    {
      UIMDRV_PUP_MSG_ERR_2(UIMLOG_MSG_45,uim_ptr->id,"Wrong value of uim_DI_current = 0x%x/uim_FI_current = 0x%x",
                       uim_ptr->state.DI_current,
                       uim_ptr->state.FI_current);
    }
  }

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
} /* uimdrv_iso_reset */


/*============================================================================= 
  FUNCTION UIMDRV_ISO_UPDATE_OP_PARAM
=============================================================================*/
boolean uimdrv_iso_update_op_param
(
  uim_op_params_type const *op_params,
  uim_instance_global_type *uim_ptr
)
{
  /* Check if the UIM clock frequency is to change. */
  if (op_params->change_clk_freq)
  {
    /* Change the UIM clock frequency based on the selected rate */
    switch (op_params->clock_frequency)
    {
      case UIMDRV_CLK_FREQ_3_84_MHZ:
        {
          /*-------------------------------------------------------------------------
           Turn on the clock for the R-UIM interface
          -------------------------------------------------------------------------*/
          UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

          /* Set the clock to this rate. */
          uim_clock_control (uim_ptr->id, UIMDRV_CLK_FREQ_3_84_MHZ);
          if((uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_AFTER_CLK_RESTART] > 0)&&
             (uim_ptr->debug.tot_recovery_count > 0) &&
             (uim_ptr->command.mode ==  UIM_NORMAL_MODE))
          {
            /* Add delay after clock restart */
            uim_timed_sleep(
               uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_AFTER_CLK_RESTART],
               uim_ptr,
               UIM_ALL_ZERO_SIG);
          }
        } /* end case - UIMDRV_CLK_FREQ_3_84_MHZ */
        break;

      case UIMDRV_CLK_FREQ_4_8_MHZ:
        {
          /*-------------------------------------------------------------------------
           Turn on the clock for the R-UIM interface
          -------------------------------------------------------------------------*/
          UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

          /* Set the clock to this rate. */
          uim_clock_control (uim_ptr->id, UIMDRV_CLK_FREQ_4_8_MHZ);
          if((uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_AFTER_CLK_RESTART] > 0)&&
             (uim_ptr->debug.tot_recovery_count > 0) &&
             (uim_ptr->command.mode ==  UIM_NORMAL_MODE))
          {
            /* Add delay after clock restart */
            uim_timed_sleep(
               uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_AFTER_CLK_RESTART],
               uim_ptr,
               UIM_ALL_ZERO_SIG);
          }
        } /* end case - UIMDRV_CLK_FREQ_4_8_MHZ */
        break;

      case UIMDRV_CLK_STOP_HIGH:
        {
          if(uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_BEFORE_CLK_STOP] > 0)
          {
            /* Add a delay only in case of multiple recoveries and skip delay in case of internal 
			   poll command. This is to ensure we do not sleep for longer times on recovery but only
			   in incremental time, to minimize power consumption due to long wakeup. */
            if(uim_ptr->debug.tot_recovery_count > UIM_MIN_RECOVERY_COUNT_FOR_DELAY &&
              (uim_ptr->command.static_cmd_buf.hdr.command != UIM_INTERNAL_POLL_F))
            {
              /* We increment the timer value according to the number of recoveries */
              uint16 delay = uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_BEFORE_CLK_STOP] + 
				             (5 * (uim_ptr->debug.tot_recovery_count - (UIM_SLEEP_TIMER_INCREMENT - 1)));

			  /* Fallback to the max limit of 30ms in case more recoveries lead to higher delay */
			  if(delay > UIM_DELAY_BEFORE_CLOCK_STOP_MAX_LIMIT)
			  {
			    delay = UIM_DELAY_BEFORE_CLOCK_STOP_MAX_LIMIT;
			  }

              /* Add delay before clock stop */
              uim_timed_sleep(
                 delay,
                 uim_ptr,
                 UIM_CMD_Q_SIG);
            }
            /* Do clock stop only if it is normal mode and there is no command
               in command queue */
            if(( q_cnt( &uim_ptr->task.cmd_q ) == 0 ) &&
               (uim_ptr->command.mode !=  UIM_RECOVERY_MODE)
              )
            {

              UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

              /* Set the clock to high state. */
              uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_HIGH);

              /*-------------------------------------------------------------------------
               Allow the clock for the R-UIM interface to be turned off
              -------------------------------------------------------------------------*/
              UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);
            }
            else
            {
              /* CLK didn't stop. This only may happen in the case of delay before clk stop.
                 NOTE: the return value of uim_update_op_params
                 currently only represent whether CLK stop action is performed.
                 In future, it can be expanded to some update_op_params status enum
                 to indication whether some action in update_op_params is perfromed
                 correctly */
              return FALSE;
            }
          }
          else
          {
            /* Set the clock to high state. */
            uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_HIGH);

            /*-------------------------------------------------------------------------
             Allow the clock for the R-UIM interface to be turned off
            -------------------------------------------------------------------------*/
            UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);
          }
        } /* end case - UIM_CLK_STOP_HIGH */
        break;

      case UIMDRV_CLK_STOP_LOW:
        {
          if(uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_BEFORE_CLK_STOP] > 0)
          {
            /* Add a delay only in case of multiple recoveries and skip delay in case of internal 
			   poll command. This is to ensure we do not sleep for longer times on recovery but only
			   in incremental time, to minimize power consumption due to long wakeup. */
            if(uim_ptr->debug.tot_recovery_count > UIM_MIN_RECOVERY_COUNT_FOR_DELAY &&
              (uim_ptr->command.static_cmd_buf.hdr.command != UIM_INTERNAL_POLL_F))
            {
              /* We increment the timer value according to the number of recoveries */
              uint16 delay = uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_DELAY_BEFORE_CLK_STOP] + 
				             (5 * (uim_ptr->debug.tot_recovery_count - (UIM_SLEEP_TIMER_INCREMENT - 1)));

			  /* Fallback to the max limit of 30ms in case more recoveries lead to higher delay */
			  if(delay > UIM_DELAY_BEFORE_CLOCK_STOP_MAX_LIMIT)
			  {
			    delay = UIM_DELAY_BEFORE_CLOCK_STOP_MAX_LIMIT;
			  }

              /* Add delay before clock stop */
              uim_timed_sleep(
                 delay,
                 uim_ptr,
                 UIM_CMD_Q_SIG);
            }
            /* Do clock stop only if it is normal mode and there is no command
               in command queue */
            if(( q_cnt( &uim_ptr->task.cmd_q ) == 0 ) &&
               (uim_ptr->command.mode !=  UIM_RECOVERY_MODE)
              )
            {
              UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

              /* Set the clock to high state. */
              uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_LOW);

              /*-------------------------------------------------------------------------
               Allow the clock for the R-UIM interface to be turned off
              -------------------------------------------------------------------------*/
              UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);
            }
            else
            {
              /* CLK didn't stop. This only may happen in the case of delay before clk stop.
                 NOTE: the return value of uim_update_op_params
                 currently only represent whether CLK stop action is performed.
                 In future, it can be expanded to some update_op_params status enum
                 to indication whether some action in update_op_params is perfromed
                 correctly */
              return FALSE;
            }
          }
          else
          {
            /* Set the clock to low state. */
            uim_clock_control (uim_ptr->id, UIMDRV_CLK_STOP_LOW);

            /*-------------------------------------------------------------------------
             Allow the clock for the R-UIM interface to be turned off
            -------------------------------------------------------------------------*/
            UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT(uim_ptr);
          }
        } /* end case - UIM_CLK_STOP_LOW */
        break;

      default: /* do nothing. */
        {
        }
        break;
    } /* end switch - clock frequency */

  } /* end if - change the UIM clock frequency */

  /* Determine if the baud rate is to change */
  if (op_params->change_baud_rate)
  {
    /* Change the baudrate with the supplied parameters. */
    SET_UIM_BAUD_RATE_SLOT (uim_ptr->id, op_params->FI, op_params->DI);
  } /* end if - the baud rate is to change */

  /* Determine if the guardtime is to change. */
  if (op_params->change_guardtime)
  {
    /* Set the stop bit number */
    if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == FALSE)
    {
#if defined( FEATURE_UIM_T_1_SUPPORT )
    if( UIM_T_1_PROTOCOL == uim_ptr->state.operating_protocol )
    {
      UIM_UART_CNFG_SLOT(uim_ptr, UART_SIM_CFG__STOP_BIT_LEN_MASK,
                    (op_params->guardtime_bits)
                     <<UART_SIM_CFG__STOP_BIT_LEN_SHFT);
    }
    else
#endif /* FEATURE_UIM_T_1_SUPPORT */
    {
      UIM_UART_CNFG_SLOT(uim_ptr, UART_SIM_CFG__STOP_BIT_LEN_MASK,
                    (op_params->guardtime_bits + UIM_UART_DM_EXTRA_GUARD_TIME)
                     <<UART_SIM_CFG__STOP_BIT_LEN_SHFT);
    }
    }
    else
    {
      UIM_UART_CNFG_SLOT(uim_ptr, UART_SIM_CFG__STOP_BIT_LEN_MASK,
                   op_params->guardtime_bits<<UART_SIM_CFG__STOP_BIT_LEN_SHFT);
    }
#if defined( FEATURE_UIM_T_1_SUPPORT )
  /* Check if the protocol is T=1 */
  if ( UIM_T_1_PROTOCOL == uim_ptr->state.operating_protocol )
  {
    /* Program the interface to operate in block mode */
    UIM_UART_CNFG_SLOT(uim_ptr, UART_SIM_CFG_TX_RX_MODE_MASK,UART_SIM_CFG_TX_RX_MODE_MASK);

    /* Determine if the stop bits are 1 */
    if (UIM_MINIMUM_T_1_GUARDTIME == op_params->guardtime_bits )
    {
      if ( UIM_DIRECT_CONVENTION == uim_ptr->state.convention)
      {
        /* Use only one stop bit */
        UIM_PRG_MR2_SLOT(uim_ptr, MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_8BPC |
                         MSMU_MR2_1SB | MSMU_MR2_EPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF );
      }
      else /* inverse convention */
      {
        /* Use only one stop bit */
        UIM_PRG_MR2_SLOT(uim_ptr, MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_8BPC |
                         MSMU_MR2_1SB | MSMU_MR2_OPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF);
      } /* end if - direct convention */
    } /* end if - guardtime bits are 1 */
    else /* we only use 2 stop bits for T=1 at maximum */
    {
      if ( UIM_DIRECT_CONVENTION == uim_ptr->state.convention)
      {
        /* Use two stop bits */
        UIM_PRG_MR2_SLOT(uim_ptr, MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_8BPC |
                         MSMU_MR2_2SB | MSMU_MR2_EPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF);
      }
      else /* inverse convention */
      {
        /* Use two stop bits */
        UIM_PRG_MR2_SLOT(uim_ptr, MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_8BPC |
                         MSMU_MR2_2SB | MSMU_MR2_OPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF);
      } /* end if - direct convention */
    }
  }
  else if (UIM_T_0_PROTOCOL == uim_ptr->state.operating_protocol)
#endif /* FEATURE_UIM_T_1_SUPPORT */
  {
    /* Set the RX error char off bit in MR2 to let UART_DM ignore the
       bytes with parity error in T=0 mode */
    if( uim_ptr->state.convention == UIM_INVERSE_CONVENTION)
    {
      UIM_PRG_MR2_SLOT(uim_ptr,  MSMU_MR2_RX_ERROR_CHAR_OFF | MSMU_MR2_8BPC | MSMU_MR2_2SB
                       | MSMU_MR2_OPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF);
    }
    else
    {
      UIM_PRG_MR2_SLOT(uim_ptr, MSMU_MR2_RX_ERROR_CHAR_OFF | MSMU_MR2_8BPC | MSMU_MR2_2SB
                       | MSMU_MR2_EPAR | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF);
    }
  }

  } /* end if - change the guardtime. */
  /* Currently this TRUE return only means CLK stop performed */
  return TRUE;
} /* uimdrv_iso_update_op_param */


/*============================================================================= 
  FUNCTION UIMDRV_ISO_SEND_COMMAND
=============================================================================*/
void uimdrv_iso_send_command
(
  uim_req_buf_type         *uim_req,
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_49,uim_ptr->id, "uim_send_command" );

  /* Reset the error counts */
  uim_ptr->card_cmd.parity_error_count = 0;
  uim_ptr->card_cmd.overrun_error_count = 0;

#if defined( FEATURE_UIM_T_1_SUPPORT )
  if (UIM_T_1_PROTOCOL == uim_ptr->state.current_protocol)
  {
    /* Initialize the RX sub-state for T-1 protocol */
    uim_ptr->rxtx_state_machine.rx_t1_block_sub_state = UIM_RX_T_1_RX_PROLOGUE_ST;

    /* Process the card response for the T=1 protocol */
    uim_process_t_1_command( uim_req, uim_ptr);
  }
  else
#endif /* FEATURE_UIM_T_1_SUPPORT */
  {
    uimdrv_set_card_cmd_data(uim_req , uim_ptr);

    /* Send the APDU */
    uimdrv_iso_send_apdu (uim_req, uim_ptr);
  } /* end if - the protocol is T=1 */
} /* uimdrv_iso_send_command */


/*===========================================================================
  FUNCTION UIMDRV_ISO_SEND_PPS
===========================================================================*/
void uimdrv_iso_send_pps
(
  uim_pps_req_buf_type const *pps_request,  /* This points to the PPS request and
                                               the PPS response buffer         */
  uim_instance_global_type   *uim_ptr
)
{
  if (pps_request == NULL  || uim_ptr==NULL)
  {
    return;
  }

  /* Turn OFF the ability to echo-back receive bytes.
     Set SIM_CFG: MASK_RX to 1 */
  UIM_UART_CNFG_SLOT(uim_ptr,UART_SIM_CFG__MASK_RX, 8);


  /* Reset/flush the UART just in case there was something going on. */
  uim_reset_uart_slot (uim_ptr);

  /* Enable the UART transmitter */
  UIM_ENABLE_UART_TX_SLOT (uim_ptr);

  /* Enable the UART receiver */
  UIM_ENABLE_UART_RX_SLOT (uim_ptr);

  /* Some how this delay is needed for LCU UART.
     Will need to find out ways to remove this delay. */
  uim_clk_busy_wait (500);

  UIMDRV_MSG_LOW_0(uim_ptr->id, "uim_send_pps" );

  /* Save the number of bytes to send of the request. */
  uim_ptr->card_cmd.num_bytes_to_send = pps_request->size_of_pps_request;

  /* Save the PPS request and response buffer pointers. */
  uim_ptr->card_cmd.request_buffer_ptr = pps_request->request_buf;
  uim_ptr->card_cmd.resp_buf_ptr = pps_request->rsp_ptr;
  uim_ptr->card_cmd.resp_buf_byte_ptr = (byte *)&pps_request->rsp_ptr->rsp;


  /* Set up the driver variables to expect a PPS response */
  /* Indicate the remaining bytes are to be sent */
  uim_ptr->flag.send_all_remaining_bytes = TRUE;
  uim_ptr->rxtx_state_machine.tx_state = UIM_TX_SEND_PPS;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;
  uim_ptr->card_cmd.num_resp_bytes_rcvd = 0;
  uim_ptr->pps.check_byte = 0;
  uim_ptr->pps.num_bytes_to_skip = 2;
  uim_ptr->pps.receive_mode = UIM_WAITING_FOR_T0;

  /* Program the receive level to allow fast processing of tx'd bytes */
  UIM_RX_WATERLEVEL_SLOT (uim_ptr, UIM_RX_WATER_MARK_LEVEL_ZERO );

  /* This is when we use the word mode UART */
  uim_send_pps_bytes(uim_ptr);

  /* starting hw wwt with 2* wwt after writing to TX fifo */
  uim_uartdm_set_wwt_val(uim_ptr, 2 * uim_ptr->card_cmd.work_waiting_time_etus);
  uim_uartdm_enable_wwt_counter(uim_ptr);
  /* Also start the software wwt timer with a value 100ms more than the hw wwt */
  (void)rex_set_timer(&uim_ptr->command.cmd_rsp_timer, (2 * uim_ptr->card_cmd.work_waiting_time) + UIM_ADDITIONAL_DELAY_FOR_SWT );
  uim_ptr->flag.active_cmd_rsp_timer = TRUE;
} /* uimdrv_iso_send_pps */


#if defined( FEATURE_UIM_T_1_SUPPORT )
/*===========================================================================
  FUNCTION UIMDRV_ISO_T_1_SEND_I_BLOCK
===========================================================================*/
void uimdrv_iso_t_1_send_i_block
(
  byte                      node_address,
  uim_instance_global_type *uim_ptr
)
{
  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);
  /* Start UIM BUSY Response timers if is in not started state*/
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_I_BLOCK);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_I_BLOCK);

  /* wait here for the BGT time */
  uim_timed_sleep(uim_ptr->t1.t_1_bgt,uim_ptr,UIM_ALL_ZERO_SIG);

  /* Clear the UART RX FIFO by reading byte/word one by one and
     by doing a UART RX reset to clear the error flags */
  uimdrv_iso_flush_uart(uim_ptr);

  /* Build the Prologue field */
  uim_ptr->t1.tx_prologue_field.nad = node_address;
  uim_ptr->t1.tx_prologue_field.pcb = UIM_T_1_PCB_I_BLOCK;
  /* Set the send sequence number */
  if (uim_ptr->t1.me_send_sequence_num != UIM_T_1_INIT_SEQ_NUM)
  {
    uim_ptr->t1.tx_prologue_field.pcb |= UIM_T_1_PCB_SEND_SEQ_NUM;
  }
  /* Determine the information field length and more bit */
  if (uim_ptr->t1.apdu_request_bytes_to_send > uim_ptr->t1.t_1_ifsc)
  {
    /* Send the maximum I-block size */
    uim_ptr->card_cmd.num_cmd_data_bytes_to_send = uint16touint8(uim_ptr->t1.t_1_ifsc);
    uim_ptr->t1.tx_prologue_field.len = uint16touint8(uim_ptr->t1.t_1_ifsc);
    uim_ptr->t1.tx_prologue_field.pcb |= UIM_T_1_PCB_MORE_DATA_BIT;

    /* Save the length of the I block being sent.  There might
    be some more blocks (R & S) between sending the blocks of the
    chained I blocks */
    uim_ptr->t1.last_tx_i_block_length = uim_ptr->t1.tx_prologue_field.len;

    /* Change the T=1 protocol state */
    uim_ptr->t1.t_1_protocol_state = UIM_T_1_SENT_CHAINED_APDU_ST;
  }
  else
  {
    /* The APDU fits within the I-block */
    uim_ptr->card_cmd.num_cmd_data_bytes_to_send = uint16touint8(uim_ptr->t1.apdu_request_bytes_to_send);
    uim_ptr->t1.tx_prologue_field.len = (byte)uim_ptr->t1.apdu_request_bytes_to_send;

    /* Change the T=1 protocol state */
    uim_ptr->t1.t_1_protocol_state = UIM_T_1_SENT_APDU_ST;
  } /* end if - the APDU is larger than the block size */

  /* Initialize the request pointer to send out the prologue field */
  uim_ptr->card_cmd.request_buffer_ptr = (byte *)&uim_ptr->t1.tx_prologue_field;
  /* Number of bytes to send of the prologue. */
  uim_ptr->card_cmd.num_bytes_to_send = UIM_T_1_NUM_BYTES_PROLOGUE;

  /* Save the APDU and response buffer pointers. */
  uim_ptr->card_cmd.apdu_command_data_ptr = uim_ptr->t1.apdu_req_buffer_ptr;
  uim_ptr->card_cmd.resp_buf_ptr = uim_ptr->t1.t_1_apdu_resp_buf;
  uim_ptr->card_cmd.resp_buf_byte_ptr = uim_ptr->t1.apdu_rsp_buffer_ptr;
  uim_ptr->t1.rx_prologue_field_buffer_ptr = (byte *)&uim_ptr->t1.rx_prologue_field_buffer;

  /* Zero the command response size */
  uim_ptr->card_cmd.resp_buf_ptr->cmd_rsp_size = 0;
  /* Initialize the EDC fields */

  if (UIM_T_1_EDC_LRC == uim_ptr->t1.t_1_edc)
  {
    /* Initialize the crc field as it initializes both bytes of the EDC
       field including the lrc field */
    uim_ptr->t1.tx_epilogue_field.crc = 0;
  }
  else /* The EDC is CRC */
  {
    uim_ptr->t1.tx_epilogue_field.crc = CRC_16_L_SEED;
  } /* The EDC is LRC */

  /* Start sending the prologue */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_T_1_SEND_PROLOGUE_ST;
  uim_ptr->rxtx_state_machine.rx_sub_state  = UIM_RX_WAIT_FOR_TXLEV;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;

  /* Clear if there was a pending stale interrupt */
  if (UIM_READ_MISR_SLOT(uim_ptr) & MSMU_IMR_RXSTALE)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "This condition should not happen");
    /* clear the stale char event */
    UIM_CLEAR_RX_STALE_INT_SLOT(uim_ptr);
  }

  uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);

  /* Enable Tx level interrupt to cause T=1 block to be sent out.
     TX READY interrupt tells us when the FIFO is empty (last word moved
     to the packing buffer) and NO_CHARS_FOR_TX characters were written
     into the FIFO. As per the UART_DM design, it is forbidden to write
     the next TX transfer size into NO_CHARS_FOR_TX register before TX_READY
     interrupt is triggered. */

    /* Enable the receive interrupts - stale character timeout,
       receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
    UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_RXLEV | \
                     MSMU_IMR_TX_COMP | MSMU_IMR_WWT | MSMU_IMR_TXLEV | MSMU_IMR_TX_READY) );
} /* uimdrv_iso_t_1_send_i_block */


/*===========================================================================
  FUNCTION UIMDRV_ISO_T_1_SEND_R_BLOCK
===========================================================================*/
void uimdrv_iso_t_1_send_r_block
(
  byte                      node_address, /* Node address byte */
  byte                      r_block_code, /* R-block response code */
  uim_instance_global_type *uim_ptr
)
{
  /* Start UIM BUSY Response timers if is in not started state*/
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_R_BLOCK);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_R_BLOCK);

  /* When BWT times-out, there is no need waiting for BGT */
  if( uim_ptr->t1.t_1_error_count==0 )
  {
    /* wait here for the BGT time */
    uim_timed_sleep(uim_ptr->t1.t_1_bgt,uim_ptr,UIM_ALL_ZERO_SIG);
  }

  /* Clear the UART RX FIFO by reading byte/word one by one and
     by doing a UART RX reset to clear the error flags */
  uimdrv_iso_flush_uart(uim_ptr);

  /* Build the Prologue field */
  uim_ptr->t1.tx_prologue_field.nad = node_address;
  uim_ptr->t1.tx_prologue_field.pcb = UIM_T_1_PCB_R_BLOCK | r_block_code;
  /* Set the send sequence number */
  if (uim_ptr->t1.card_send_sequence_num != 0)
  {
    uim_ptr->t1.tx_prologue_field.pcb |= UIM_T_1_PCB_NEXT_SEQ_NUM;
  }

  /* There is no information field in an R-block */
  uim_ptr->card_cmd.num_cmd_data_bytes_to_send = 0;
  uim_ptr->t1.tx_prologue_field.len = 0;

  /* Initialize the request pointer to send out the prologue field */
  uim_ptr->card_cmd.request_buffer_ptr = (byte *)&uim_ptr->t1.tx_prologue_field;
  /* Number of bytes to send of the prologue. */
  uim_ptr->card_cmd.num_bytes_to_send = UIM_T_1_NUM_BYTES_PROLOGUE;

  /* Save the APDU and response buffer pointers. */
  uim_ptr->card_cmd.apdu_command_data_ptr = uim_ptr->t1.apdu_req_buffer_ptr;
  uim_ptr->card_cmd.resp_buf_ptr = uim_ptr->t1.t_1_apdu_resp_buf;
  uim_ptr->card_cmd.resp_buf_byte_ptr = uim_ptr->t1.apdu_rsp_buffer_ptr;
  uim_ptr->t1.rx_prologue_field_buffer_ptr = (byte *)&uim_ptr->t1.rx_prologue_field_buffer;

  /* Zero the command response size */
  uim_ptr->card_cmd.resp_buf_ptr->cmd_rsp_size = 0;
  /* Initialize the EDC fields */
  if (UIM_T_1_EDC_LRC == uim_ptr->t1.t_1_edc)
  {
    /* Initialize the crc field as it initializes both bytes of the EDC
       field including the lrc field */
    uim_ptr->t1.tx_epilogue_field.crc = 0;
  }
  else /* The EDC is CRC */
  {
    uim_ptr->t1.tx_epilogue_field.crc = CRC_16_L_SEED;
  } /* The EDC is LRC */

  /* Start sending the prologue */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_T_1_SEND_PROLOGUE_ST;
  uim_ptr->rxtx_state_machine.rx_sub_state  = UIM_RX_WAIT_FOR_TXLEV;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;

  /* Clear if there was a pending stale interrupt */
  if (UIM_READ_MISR_SLOT(uim_ptr) & MSMU_IMR_RXSTALE)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "This condition should not happen");
    /* clear the stale char event */
    UIM_CLEAR_RX_STALE_INT_SLOT(uim_ptr);
  }

  uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);

  /* Enable Tx level interrupt to cause T=1 block to be sent out.
     TX READY interrupt tells us when the FIFO is empty (last word moved
     to the packing buffer) and NO_CHARS_FOR_TX characters were written
     into the FIFO. As per the UART_DM design, it is forbidden to write
     the next TX transfer size into NO_CHARS_FOR_TX register before TX_READY
     interrupt is triggered. */

    /* Enable the receive interrupts - stale character timeout,
       receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
    UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_RXLEV | \
                     MSMU_IMR_TX_COMP | MSMU_IMR_WWT | MSMU_IMR_TXLEV | MSMU_IMR_TX_READY) );
} /* uimdrv_iso_t_1_send_r_block */


/*===========================================================================
  FUNCTION UIMDRV_ISO_T_1_SEND_S_BLOCK
===========================================================================*/
void uimdrv_iso_t_1_send_s_block
(
  byte                      node_address,        /* Node address byte */
  byte                      s_block_code,        /* S-block code */
  byte                     *s_block_data,       /* S-block data */
  byte                      s_block_data_size,    /* S-block data size */
  uim_instance_global_type *uim_ptr
)
{
  /* Start UIM BUSY Response timers if is in not started state*/
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_S_BLOCK);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_T_1_SEND_S_BLOCK);

  /* When BWT times-out, there is no need waiting for BGT */
  if( uim_ptr->t1.t_1_error_count==0 )
  {
    /* wait here for the BGT time */
    uim_timed_sleep(uim_ptr->t1.t_1_bgt,uim_ptr,UIM_ALL_ZERO_SIG);
  }

  /* Clear the UART RX FIFO by reading byte/word one by one and
     by doing a UART RX reset to clear the error flags */
  uimdrv_iso_flush_uart(uim_ptr);

  /* Build the Prologue field */
  uim_ptr->t1.tx_prologue_field.nad = node_address;
  uim_ptr->t1.tx_prologue_field.pcb = UIM_T_1_PCB_S_BLOCK | s_block_code;

  /* There is no information field in an R-block */
  uim_ptr->card_cmd.num_cmd_data_bytes_to_send = s_block_data_size;
  uim_ptr->t1.tx_prologue_field.len  = s_block_data_size;

  /* Initialize the request pointer to send out the prologue field */
  uim_ptr->card_cmd.request_buffer_ptr = (byte *)&uim_ptr->t1.tx_prologue_field;
  /* Number of bytes to send of the prologue. */
  uim_ptr->card_cmd.num_bytes_to_send = UIM_T_1_NUM_BYTES_PROLOGUE;

  /* Save the APDU and response buffer pointers. */
  uim_ptr->card_cmd.apdu_command_data_ptr = s_block_data;
  uim_ptr->card_cmd.resp_buf_ptr = uim_ptr->t1.t_1_apdu_resp_buf;
  uim_ptr->card_cmd.resp_buf_byte_ptr = uim_ptr->t1.apdu_rsp_buffer_ptr;
  uim_ptr->t1.rx_prologue_field_buffer_ptr = (byte *)&uim_ptr->t1.rx_prologue_field_buffer;

  /* Zero the command response size */
  uim_ptr->card_cmd.resp_buf_ptr->cmd_rsp_size = 0;
  /* Initialize the EDC fields */
  if (UIM_T_1_EDC_LRC == uim_ptr->t1.t_1_edc)
  {
    /* Initialize the crc field as it initializes both bytes of the EDC
       field including the lrc field */
    uim_ptr->t1.tx_epilogue_field.crc = 0;
  }
  else /* The EDC is CRC */
  {
    uim_ptr->t1.tx_epilogue_field.crc = CRC_16_L_SEED;
  } /* The EDC is LRC */

  /* Start sending the prologue */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_T_1_SEND_PROLOGUE_ST;
  uim_ptr->rxtx_state_machine.rx_sub_state  = UIM_RX_WAIT_FOR_TXLEV;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;

  /* Clear if there was a pending stale interrupt */
  if (UIM_READ_MISR_SLOT(uim_ptr) & MSMU_IMR_RXSTALE)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "This condition should not happen");
    /* clear the stale char event */
    UIM_CLEAR_RX_STALE_INT_SLOT(uim_ptr);
  }

  uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);

  /* Enable Tx level interrupt to cause T=1 block to be sent out.
     TX READY interrupt tells us when the FIFO is empty (last word moved
     to the packing buffer) and NO_CHARS_FOR_TX characters were written
     into the FIFO. As per the UART_DM design, it is forbidden to write
     the next TX transfer size into NO_CHARS_FOR_TX register before TX_READY
     interrupt is triggered. */

    /* Enable the receive interrupts - stale character timeout,
       receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
    UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_RXLEV | \
                     MSMU_IMR_TX_COMP | MSMU_IMR_WWT | MSMU_IMR_TXLEV | MSMU_IMR_TX_READY) );
} /* uimdrv_iso_t_1_send_s_block */
#endif /* FEATURE_UIM_T_1_SUPPORT */


/*===========================================================================
  FUNCTION uim_read_status
===========================================================================*/
dword uim_read_status
(
  uim_instance_global_type *uim_ptr
)
{
  return UIM_READ_STATUS_SLOT(uim_ptr);
} /* uim_read_status */


/*===========================================================================
  FUNCTION UIMDRV_ISO_FLUSH_UART
===========================================================================*/
void uimdrv_iso_flush_uart
(
  uim_instance_global_type *uim_ptr
)
{
  dword uart_status;  /* Holds the UART status */

  volatile uint32 rx_value;   /* Holds the received word from the receive FIFO. */

  /* Make sure there is a byte in the receive FIFO */
  for (uart_status = UIM_READ_STATUS_SLOT(uim_ptr);
       (uart_status & MSMU_SR_RXRDY);
       uart_status = UIM_READ_STATUS_SLOT(uim_ptr))
  {
    /* Get the next received word. */
    rx_value = UIM_GET_RX_WORD_SLOT(uim_ptr);
  }

  /* reset all the error flags in the UART RX */
  uim_reset_receiver(uim_ptr);
  /* setting rx_value gets a word from the UART but causes a compile warning */
  (void)rx_value;
} /* uimdrv_iso_flush_uart */


/*===========================================================================
  FUNCTION UIMDRV_ISO_RECONFIG_UARTDM
===========================================================================*/
void uimdrv_iso_reconfig_uartdm
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer != UIM_ISO)
  {
    return;
  }
  /* Enable the UARTDM clocks before accessing any UARTDM register */
  UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT(uim_ptr);

  /* Set the convention back to "direct" */
  UIM_PROGRAM_DIRECT_CONVENTION_SLOT(uim_ptr);

  /* Note that the RX_CHAR_OFF bit must be disabled during ATR state so that
   * bytes with parity errors enter into the UARTDM, which shall be used to
   * switch the inverse convention.
   */
  UIM_PRG_MR2_SLOT(uim_ptr, (MSMU_MR2_8BPC | MSMU_MR2_2SB | MSMU_MR2_EPAR| MSMU_MR2_ERRMODE_BLOCK | MSMU_MR2_RX_ERROR_ZERO_CHAR_OFF));

  /* Reset the UARTDM after programming the MR2 register */
  uim_reset_uart_slot (uim_ptr);
} /* uimdrv_iso_reconfig_uartdm */


/*===========================================================================
  FUNCTION UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT
===========================================================================*/
void UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->id >= UIM_INSTANCE_INVALID)
  {
    return;
  }
  /* If the clocks were turned ON already, then ignore */
  if(TRUE == uim_ptr->state.clock_is_on)
  {
    return;
  }

  if(uim_ptr->hardware.sleep_latency_voting_handle)
  {
    /* Vote against Q6 power collapse */
    uim_vote_against_sleep(uim_ptr);
  }
  else
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Core Cpu handle invalid");
  }

  uim_mss_bus_clk_config(uim_ptr->hardware.mss_bus_clk_voting_handle,
                         UIMDRV_CLK_CONFIG_BUS_FREQ);

  uimdrv_pmic_vote_for_ldo_powermode(uim_ptr, PMIC_UIM_LDO_HIGH_POWER_MODE);

  UIM_CLOCK_REGIME_ENABLE_SLOT(uim_ptr->id);

  UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);
  uim_ptr->state.clock_is_on = TRUE;
  UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);

  /* Disable the clock gating to the respective module.
     Please note that we are doing this after clocks are enabled */
  UIM_SET_UARTDM_ACTIVE_MODE(uim_ptr);
} /* UIMDRV_ISO_TCXO_MUST_BE_ON_SLOT */


/*===========================================================================
  FUNCTION UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT
===========================================================================*/
void UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->id >= UIM_INSTANCE_INVALID)
  {
    return;
  }

  /* If the clocks were shut-off already, then ignore */
  if(FALSE == uim_ptr->state.clock_is_on)
  {
    return;
  }

  /* Clear busy and teransaction timers if they are active */
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CLOCK_OFF);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CLOCK_OFF);

  uim_clk_busy_wait(100);

  /* Enable the clock gating to the respective modules.
     Please note that we are doing this after clocks are enabled */
  UIM_SET_UARTDM_INACTIVE_MODE(uim_ptr);

  UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);
  uim_ptr->state.clock_is_on = FALSE;

  UIM_CLOCK_REGIME_DISABLE_SLOT(uim_ptr->id);

  uimdrv_pmic_vote_for_ldo_powermode(uim_ptr, PMIC_UIM_LDO_LOW_POWER_MODE);

  uim_mss_bus_clk_config(uim_ptr->hardware.mss_bus_clk_voting_handle,
                         UIMDRV_CLK_CONFIG_BUS_SLEEP_FREQ);

  UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.clk_ctl, uim_ptr);

  if(uim_ptr->hardware.sleep_latency_voting_handle)
  {
    uim_vote_for_sleep(uim_ptr);
  }
} /* UIMDRV_ISO_TCXO_CAN_BE_OFF_SLOT */


/**
 *  This function sends the APDU command in the case of UART DM.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_send_apdu_command_hdr
(
  uim_instance_global_type *uim_ptr
)
{
  /* Initialize these globals to FALSE for every APDU send */
  uim_ptr->flag.sw1_byte_rcvd  = FALSE;
  uim_ptr->flag.sw2_byte_rcvd  = FALSE;
  uim_ptr->flag.resp_bytes_being_rcvd = FALSE;
  uim_ptr->card_cmd.num_bytes_received = 0;
  uim_ptr->card_cmd.num_words_received = 0;
  uim_ptr->card_cmd.total_bytes_read = 0;

  /* Initialize the RXLEV count for every APDU transaction */
  uim_ptr->debug.rxlev_count = 0;

  /* Reset the RX byte count to zero */
  uim_ptr->card_cmd.num_bytes_received = 0;

  /* set the rx state to wait for procedure bytes */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_PROC_PRCDR_BYTES;

  /* Clear if there was a pending stale interrupt */
  if (UIM_READ_MISR_SLOT(uim_ptr) & MSMU_IMR_RXSTALE)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "This condition should not happen");
    /* clear the stale char event */
    UIM_CLEAR_RX_STALE_INT_SLOT(uim_ptr);
  }

  /* RAM_ADDR_WIDTH is 7 bits in our version of LCU MSM chip
     - This RAM is used as both RX FIFO and TX FIFO
     - 128 words is the total size of RAM. Which means, the size of RX and
       TX FIFO is 128 words.
     - Default configuration:  (we use this)
       RX FIFO size = TX FIFO size, that means 64 words (256 bytes) each.
   */

  /* If the response size (ack byte + data + status bytes) is greater than
     RX FIFO size, then enable RXLEV interrupt */
  if ((uim_ptr->card_cmd.num_resp_data_bytes_expected > 0) &&
      (uim_ptr->card_cmd.num_resp_data_bytes_expected +
       NUM_ACK_BYTES + NUM_STATUS_BYTES > MAX_RXFIFO_SIZE) )
  {
    UIM_RX_WATERLEVEL_SLOT(uim_ptr, MSMU_DEF_RFWR_VAL );

      /* Enable the receive interrupts - stale character timeout,
         receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
      UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | MSMU_IMR_RXLEV | \
                      MSMU_IMR_TX_COMP | MSMU_IMR_WWT) );
    }
    else
    {
      /* Enable the receive interrupts - stale character timeout,
         receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
      UIM_PRG_IMR_SLOT(uim_ptr, ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK |  \
                      MSMU_IMR_TX_COMP | MSMU_IMR_WWT) );
    }

  /* Initialize a new RX transfer
    UART_DM SWI says: number of bytes needs to be multipled by 16 */
  UIM_DM_RX_INIT_SLOT(uim_ptr, MSMU_DEF_RX_INIT_VAL);

  /* Program the default stale timeout value */
  UIM_PRG_IPR_SLOT(uim_ptr, MSMU_IPR_DEF_STALE_TIMEOUT);

   /* Wait for TXCOMP (transfer completion) event to make sure the APDU
      command bytes have been seen successfully */
  uim_ptr->rxtx_state_machine.rx_sub_state = UIM_RX_WAIT_FOR_TXCOMP;

  /* Enable Stale event */
  UIM_ENABLE_RX_STALE_INT_SLOT(uim_ptr);

  /* Program the number of bytes to be sent as APDU command header */
  UIM_NUM_CHARS_TO_SEND_SLOT(uim_ptr,uim_ptr->card_cmd.num_bytes_to_send);

  /* Clear the work waiting timer when transmitting bytes */
  uim_clear_cmd_rsp_timer(uim_ptr);

  /* Write the words to the UART */
  uim_tx(uim_ptr->card_cmd.request_buffer_ptr,
                    uim_ptr->card_cmd.num_bytes_to_send,
                    uim_ptr);

  /* starting hw wwt with 2* wwt after writing to TX fifo */
  uim_uartdm_set_wwt_val(uim_ptr, 2 * uim_ptr->card_cmd.work_waiting_time_etus);
  uim_uartdm_enable_wwt_counter(uim_ptr);
  /* Also start the software wwt timer with a value 100ms more than the hw wwt */
  (void)rex_set_timer(&uim_ptr->command.cmd_rsp_timer, (2 * uim_ptr->card_cmd.work_waiting_time) + UIM_ADDITIONAL_DELAY_FOR_SWT );
  uim_ptr->flag.active_cmd_rsp_timer = TRUE;
} /* uim_send_apdu_command_hdr */


/*===========================================================================
 FUNCTION UIMDRV_ISO_SEND_APDU 
===========================================================================*/
void uimdrv_iso_send_apdu
(
  uim_req_buf_type         *uim_req, /* This points to the APDU command buffer, the
                                        command data buffer, the response buffer
                                        the APDU status and the callback function */
  uim_instance_global_type *uim_ptr
)
{
  if (uim_req == NULL  || uim_ptr ==NULL)
  {
    return;
  }

  /* Initialize the byte count */
  uim_ptr->card_cmd.gcf_byte_count = 0;

  /* Save the number of bytes to send of the APDU header. */
  uim_ptr->card_cmd.num_bytes_to_send = NUM_BYTES_APDU_HDR;

  /* Save the request buffer pointer. */
  uim_ptr->card_cmd.request_buffer_ptr = (byte *)&uim_req->apdu_hdr;

  /* Copy the contents of APDU header into last sent APDU header */
  uim_ptr->debug.last_sent_apdu = uim_req->apdu_hdr;

  /* Copy the instruction case */
  uim_ptr->state.last_instrn_case = uim_req->instrn_case;

  /* Determine which direction the data flows */
  switch (uim_req->instrn_case)
  {
    case UIM_INSTRN_CASE_2:   /* Case 2 - Response data only */
      {
        /* The data flows from the UIM to the ME or not at all. */
        uim_ptr->card_cmd.num_cmd_data_bytes_to_send = 0;
        /* Determine if 256 bytes are expected */
        if (UIM_MAX_TRANSACTION_P3 == uim_req->apdu_hdr.p3)
        {
          /* Wait for 256 bytes */
          uim_ptr->card_cmd.num_resp_data_bytes_expected = UIM_MAX_TRANSACTION_SIZE;
        }
        else
        {
          uim_ptr->card_cmd.num_resp_data_bytes_expected = uim_req->apdu_hdr.p3;
        } /* end if - check if max number of bytes are expected */
      } /* UIM_INSTRN_CASE_2 */
      break;

    case UIM_INSTRN_CASE_1:   /* Case 1 - No data exchanged */
    case UIM_INSTRN_CASE_3:   /* Case 3 - Command data only */
    case UIM_INSTRN_CASE_4:   /* Case 4 - Response and command data */
      {
        uim_ptr->card_cmd.num_cmd_data_bytes_to_send = uim_req->apdu_hdr.p3;
        uim_ptr->card_cmd.num_resp_data_bytes_expected = 0;
      } /* UIM_INSTRN_CASE_1 or UIM_INSTRN_CASE_3 or UIM_INSTRN_CASE_4 */
      break;
    default:
      {
        /* The instruction case is not valid.  Cannot process command.
           Cause a failure through the time out mechanism. */
        UIMDRV_MSG_HIGH_1(uim_ptr->id,"Invalid instrn case 0x%x",
                          uim_req->instrn_case);
        /* Clear the cmd_rsp_timer_ptr first */
        uim_clear_cmd_rsp_timer(uim_ptr);
        /* Set the command response timeout signal */
        (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
        return;
      } /* default */
  } /* end switch - instruction case */

  /* Set up the driver variables to send the APDU header */
  /* Indicate the remaining bytes are to be sent */
  uim_ptr->flag.send_all_remaining_bytes = TRUE;
  uim_ptr->rxtx_state_machine.tx_state = UIM_TX_SEND_CMD_HDR;
  uim_ptr->card_cmd.num_resp_bytes_rcvd = 0;
  /* Reset the null procedure byte count for every APDU being sent
     This was guarded by a call to uim_nv_is_feature_enabled - but
     I think it's cheaper to set it to 0 always*/
  uim_ptr->card_cmd.infinite_null_count = 0;

  /* Program the receive level to allow fast processing of tx'd bytes */
  UIM_RX_WATERLEVEL_SLOT (uim_ptr, UIM_RX_WATER_MARK_LEVEL_ZERO );

    uim_uartdm_set_wwt_val(uim_ptr, uim_ptr->card_cmd.work_waiting_time_etus);

  /* Send the APDU command header in the case of word mode UART */
  uim_send_apdu_command_hdr(uim_ptr);

  /* Start UIM BUSY Response timers if in not_started state*/
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_SEND_APDU);
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_SEND_APDU);
} /* uimdrv_iso_send_apdu */


/**
 *  This function resets the receiver and places the rx ISR to
 *  the idle state.
 *  
 *  Dependencies: This function inactivates the UART receiver.
 *  Any data received while in idle are processed as errors.
 *  
 *  Side Effect: After this function is executed, the UART receiver
 *  is disabled and the Rx ISR is in the idle state.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_reset_receiver
(
  uim_instance_global_type *uim_ptr
)
{
  /*Return for remote card so as not to break the existing path */
  UIM_RETURN_IF_REMOTE_CARD(uim_ptr);
  /* Reset the receiver and flush the FIFO. */
  UIM_RESET_RX_SLOT (uim_ptr);

  /* Reset the error status */
  UIM_RESET_ERR_SLOT (uim_ptr);

  /* Reset the break change interrupt */
  UIM_RESET_BREAK_SLOT (uim_ptr);

  /* Re-enable the receiver */
  UIM_ENABLE_UART_RX_SLOT (uim_ptr);
} /* uim_reset_receiver */


/**
 *  API to mask all UART interrupts.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_prg_imr_mask_all
(
  uim_instance_global_type *uim_ptr
)
{
  UIM_PRG_IMR_SLOT(uim_ptr, MSMU_IMR_MASK_ALL);
}/* uim_prg_imr_mask_all */


/**
 *  This procedure will force UIM recovery
 *  
 *  Side Effect: UART is shutdown, ALL interrupts are masked,
 *  and the CMD RSP SIG is set
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_force_recovery
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->setting.phy_layer != UIM_ISO)
  {
    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_56,uim_ptr->id,"Force Recovery");
    /* Set the command response timeout signal */
    (void) rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
    return;
  }
  uim_reset_receiver (uim_ptr);
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_PROCESS_IDLE;
  UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_56,uim_ptr->id,"Force Recovery");

  /* Disable the receiver to avoid interrupts */
  uim_reset_uart_slot(uim_ptr);

  /* Clear the WWT timer */
  (void) rex_clr_timer(&uim_ptr->command.cmd_rsp_timer);

  /* Set the command response timeout signal */
  (void) rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );

  /* Mask all interrupts from UART and stop processing
  any further bytes/interrupts in a loop.  We do not
  want to do this time and again... We have decided
  do power down the interface */
  uim_prg_imr_mask_all(uim_ptr);
} /* uim_force_recovery */


/**
 *  This function sends the PPS bytes in the case of UART DM.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_send_pps_bytes
(
  uim_instance_global_type *uim_ptr
)
{
  /* Wait for the PPS response in UIM_RX_RECEIVE_PPS state */
  uim_ptr->rxtx_state_machine.rx_state = UIM_RX_RECEIVE_PPS;
  uim_ptr->rxtx_state_machine.rx_sub_state = UIM_RX_WAIT_FOR_TXCOMP;

  /* Enable the receive interrupts - stale character timeout,
     receive break condition, parity frame error , TX complete IRQ, WWT IRQ */
  UIM_PRG_IMR_SLOT (uim_ptr,  ( MSMU_IMR_RXSTALE | MSMU_IMR_RXBREAK | \
                    MSMU_IMR_TX_COMP | MSMU_IMR_WWT ) );

  /* Wait for 4-byte PPS response */
  /* UART_DM SWI: number of bytes needs to be multipled by 16 */
  if (uim_ptr->card_cmd.num_bytes_to_send > 4 &&
      uim_ptr->card_cmd.num_bytes_to_send < 9)
  {
    UIM_DM_RX_INIT_SLOT(uim_ptr, 32);
  }
  else
  {
    UIM_DM_RX_INIT_SLOT(uim_ptr, 16);
  }

  /* Enable the stale char event */
  UIM_ENABLE_RX_STALE_INT_SLOT(uim_ptr);

  /* Program the number of bytes to be sent as PPS request */
  UIM_NUM_CHARS_TO_SEND_SLOT(uim_ptr, uim_ptr->card_cmd.num_bytes_to_send);

  uim_clk_busy_wait(100);

  /* Send the PPS*/
  uim_tx(uim_ptr->card_cmd.request_buffer_ptr,
         uim_ptr->card_cmd.num_bytes_to_send,
         uim_ptr);
} /* uim_send_pps_bytes */


/**
 *  This function resets the UART used to communicate with the
 *  UIM.
 *  
 *  Side Effect: The UART will no longer be transmitting nor
 *  receiving data. The receive and transmit FIFOs will be
 *  flushed.
 *  
 *  @param uim_ptr[IN] Pointer to the current
 *                     instance of the UIM global structure
 */
void uim_reset_uart_slot
(
  uim_instance_global_type *uim_ptr
)
{
  UIM_RESET_RX_SLOT (uim_ptr);
  UIM_RESET_TX_SLOT (uim_ptr);
  UIM_RESET_UART_DM_SLOT (uim_ptr);
} /* uim_reset_uart_slot */



