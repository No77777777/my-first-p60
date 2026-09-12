/**
  @file gpi_tgt.c
  @brief
 * This file contains implementation of target-specific APIs used by the GPI driver.  
*/

/*
===============================================================================

                             Edit History


when       who     what, where, why
--------   ---     ------------------------------------------------------------
06/10/21   bn      Ported for Divar
05/30/18   pc      File modified for Talos.
01/24/18   ah      File modified for Hana.
===============================================================================
                   Copyright (c) 2017-2018, 2021 Qualcomm Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

#include "gpi_utils.h"


static DalDeviceHandle *clock_handle = NULL;

static const char *qup_common_clocks[MAX_NUM_QUP_BLOCKS][MAX_NUM_QUP_COMMON_CLOCKS] =
{
   { "gcc_qupv3_wrap0_core_2x_clk",
      "gcc_qupv3_wrap0_core_clk",
      "gcc_qupv3_wrap_0_s_ahb_clk",
      "gcc_qupv3_wrap_0_m_ahb_clk"
   },
   { "scc_qupv3_2xcore_clk",
      "scc_qupv3_core_clk",
      "scc_qupv3_s_hclk_clk",
      "scc_qupv3_m_hclk_clk"
   }
};

/**
 * Get clock handle.
 *
 * @param[in] None
 *
 * @return   TRUE if clock handle asigned, FALSE otherwise
 */
boolean get_clock_handle(void)
{
   if (clock_handle == NULL)
   {
      if (DAL_SUCCESS != DAL_ClockDeviceAttach(DALDEVICEID_CLOCK, &clock_handle))
      {
         gpi_log(GPI_WARNING, 0, "clock_attach - WARN - DAL_ClockDeviceAttach fail!\n");
         return FALSE;
      }
   }

   return TRUE;
}

/**
 * Check GSI clocks.
 *
 * @param[in] gpii_ctxt   GPII
 *
 * @return   1 if clocks are ON, 0 if some clocks are OFF.
 */
boolean gpi_clocks_on(void *gpii_ctxt)
{
   gpii_info *gpii = (gpii_info *)gpii_ctxt;
   uint32 clk_idx;
   ClockIdType clock_id;

   if (!get_clock_handle())
   {
      gpi_log(GPI_WARNING, 0, "clocks_on - WARN - clock_handle fail!\n");
      return FALSE;
   }

   for (clk_idx = 0; clk_idx < MAX_NUM_QUP_COMMON_CLOCKS; clk_idx++)
   {
      if (DAL_SUCCESS == DalClock_GetClockId(clock_handle, qup_common_clocks[gpii->qup_type][clk_idx], &clock_id))
      {
         if (TRUE != DalClock_IsClockOn(clock_handle, clock_id))
         {
            return FALSE;
         }
      }
      else
      {
         return FALSE;
      }
   }

   return TRUE;
}

/**
 * Enable GSI clocks.
 *
 * @param[in] gpii_ctxt   GPII
 *
 * @return   GPI_RETURN_STATUS    Return status.
 */
boolean switch_clocks(QUPV3_BLOCK_TYPE qup, boolean on)
{
   uint32 clk_idx;
   ClockIdType clock_id;

   if (!get_clock_handle())
   {
      gpi_log(GPI_WARNING, 0, "clocks_on - WARN - clock_handle fail!\n");
      return FALSE;
   }

   for (clk_idx = 0; clk_idx < MAX_NUM_QUP_COMMON_CLOCKS; clk_idx++)
   {
      if (DAL_SUCCESS == DalClock_GetClockId(clock_handle, qup_common_clocks[qup][clk_idx], &clock_id))
      {
         if (DAL_SUCCESS != (on? DalClock_EnableClock(clock_handle, clock_id): DalClock_DisableClock(clock_handle, clock_id)))
         {
            return FALSE;
         }
      }
      else
      {
         return FALSE;
      }
   }

   return TRUE;
}

/**
 * Enable GSI clocks.
 *
 * @param[in] gpii_ctxt   GPII
 *
 * @return   GPI_RETURN_STATUS    Return status.
 */
boolean gpi_clock_enable(void *gpii_ctxt)
{
   gpii_info *gpii = (gpii_info *)gpii_ctxt;

   return switch_clocks(gpii->qup_type, TRUE);
}

/**
 * Disable GSI clocks.
 *
 * @param[in] gpii_ctxt   GPII
 *
 * @return   GPI_RETURN_STATUS    Return status.
 */
boolean gpi_clock_disable(void *gpii_ctxt)
{
   gpii_info *gpii = (gpii_info *)gpii_ctxt;

   return switch_clocks(gpii->qup_type, FALSE);
}
