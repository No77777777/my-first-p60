/*===========================================================================

            GLink SMEM transport XPORT Configuration Structures

=============================================================================

  @file
    xport_smem_config.c

    Contains structures to be used in Glink SMEM trasnport configuration.

  Copyright (c) 2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies, Inc. Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/settings/mproc/glink/xport_smem/config/670/xport_smem_spss.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/25/17   vr       Initial version
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "com_dtypes.h"
#include "DALSys.h"
#include "DALDeviceId.h"
#include "DDIHWIO.h"
#include "err.h"

/*===========================================================================
                    CONSTANT / MACRO DECLARATIONS
===========================================================================*/
#define XPORT_SMEM_SP_SCSR_CLR_REG (0xe380a008)

/*===========================================================================
                      LOCAL DATA DEFINATION
===========================================================================*/
static uint8           *sp_scsr_base_ptr = NULL;
/*===========================================================================
FUNCTION      xport_smem_notify_spss_ctrl_info
===========================================================================*/
/**
  Notify spss allocated control info via sp_scsr register.
  This function will also map sp_scsr register to virtual address

  @param[in]  ctrl_info_addr address of allocated control info
  @param[in]  ctrl_info_size size of allocated control info

  @return     TRUE if allocation was successful.
              FALSE otherwise

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_notify_spss_ctrl_info
(
  uint32 ctrl_info_addr,
  uint32 ctrl_info_size
)
{
  DalDeviceHandle *sp_scsr_handle;  
  DALResult        result;
  uint32           sp_scsr_smem_reg = 0;

  result = DAL_DeviceAttach(DALDEVICEID_HWIO, &sp_scsr_handle);

  if ( result == DAL_SUCCESS && sp_scsr_handle != NULL )
  {
    result = DalHWIO_MapRegion( sp_scsr_handle, "+SP_CNOC_SP_SCSR_REGS", &sp_scsr_base_ptr );

    if ( result != DAL_SUCCESS || sp_scsr_base_ptr == NULL )
    {
      ERR_FATAL("xport_smem_notify_spss_ctrl_info_alloc:"
                " Failed to map sp_scsr region[result: %d][pBasePtr: 0x%x].",
                result, sp_scsr_base_ptr, 0);
    }
  }
  else
  {
    ERR_FATAL("xport_smem_notify_spss_ctrl_info_alloc:"
              " Failed to attach HWIO DalDevice[result: %d]",
              result, 0, 0);
  }

  /* Secure this mailbox to make sure only owner can access this resource */
  outpdw( sp_scsr_base_ptr + 0x00009004, 1);

  /* Populate sp scsr smem registers */
  /* Write to SP_SCSR_MBn_SMEM_ADDR_L,
   * base + 0x00005008 + 0x2000*n, n=2 for modem */
  sp_scsr_smem_reg = (uint32)(sp_scsr_base_ptr + 0x00009008);
  outpdw( sp_scsr_smem_reg, ctrl_info_addr );

  /* Write to SP_SCSR_MBn_SMEM_ADDR_U,
   * base + 0x0000500c + 0x2000*n, n=2 for modem */
  sp_scsr_smem_reg = (uint32)(sp_scsr_base_ptr + 0x0000900c);

  /* upper address register can only contain 16 bit
   * But 32 bit is enough to represent smem ctrl info address so we just set 0 */
  outpdw( sp_scsr_smem_reg, 0 );

  /* Write to SP_SCSR_MBn_SMEM_SIZE,
   * base + 0x00005010 + 0x2000*n, n=2 for modem */
  sp_scsr_smem_reg = (uint32)(sp_scsr_base_ptr + 0x00009010);
  outpdw( sp_scsr_smem_reg, ctrl_info_size );
}

/*===========================================================================
FUNCTION      xport_smem_clear_spss_interrupt
===========================================================================*/
/**
  Clear spss interrupt source. This is only for spss since interrupt from spss
  is level triggered

  @param[in]  ctx_ptr   Pointer to secure processor edge context

  @return       None.

  @sideeffects  None.
*/
/*=========================================================================*/
void xport_smem_clear_spss_interrupt(void)
{
  uint32           sp_scsr_clr_interrupt_reg = 0;

  sp_scsr_clr_interrupt_reg = (uint32)(sp_scsr_base_ptr + 0x0000A008);
  outpdw( sp_scsr_clr_interrupt_reg, 1 );
}
