/*
===========================================================================

FILE:         HALtlmmtilePlatform.c

DESCRIPTION:  
  This is the platform hardware abstraction layer implementation for the
  tlmm csr block.

===========================================================================

                             Edit History

$Header: //components/rel/core.mpss/10.0/systemdrivers/hal/tlmmtile/image/mpss/hw/kamorta/kernel/HALtlmmtilePlatform.c#4 $

when       who     what, where, why
--------   ---     --------------------------------------------------------
07/01/15   cpaulo  Created.

===========================================================================
       Copyright (c) 2015 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */

#include <HALtlmmtile.h>
#include "HALtlmmtileInternal.h"
#include <HALhwio.h>
#include "HALtlmmtileHWIO.h"
#include "ChipInfoDefs.h"


/* -----------------------------------------------------------------------
**                           MACROS
** ----------------------------------------------------------------------- */
#define ARR_SIZE(a) ((sizeof(a))/(sizeof(a[0])))


/* -----------------------------------------------------------------------
**                           FUNCTIONS
** ----------------------------------------------------------------------- */
HalTlmmPlatformDescriptionType HALTlmmTileBlockDescriptor[] =
{
  {
    .nMaxGpio = 113,
    .nNumTiles = 3,
    .aTiles = {
  TLMM_EAST_REG_BASE,
  TLMM_WEST_REG_BASE,
                TLMM_SOUTH_REG_BASE,
                0,
                0,
              },
    .nPinRegWidth = 0x1000,
    .nNumDirConnectBases = 1,
    .pHalTlmmTileDirConnArray = {
  HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_ADDR(0),
  HWIO_TLMM_WEST_DIR_CONN_INTRn_CFG_MSS_ADDR(0),
  HWIO_TLMM_SOUTH_DIR_CONN_INTRn_CFG_MSS_ADDR(0),
      0,
      0,
    },
    .eChipFamily = CHIPINFO_FAMILY_KAMORTA,
  },
  {
    .nMaxGpio = 127,
    .nNumTiles = 1,
    .aTiles = {
                TLMM_REG_REG_BASE,
                0,
                0,
                0,
                0,
              },
    .nPinRegWidth = 0x1000,
    .nNumDirConnectBases = 1,
    .pHalTlmmTileDirConnArray = {
      HWIO_TLMM_DIR_CONN_INTRn_CFG_MSS_ADDR(0),
      0,
      0,
      0,
      0,
    },
    .eChipFamily = CHIPINFO_FAMILY_AGATTI,
  },
};

/*
 * Gpio Structure
 */
#define HALTLMMTILEMAXGPIO 127
uint32 pHalTlmmGpioArray[HALTLMMTILEMAXGPIO];



/* ===========================================================================
**  HAL_tlmmtile_PlatformInit
**
** ======================================================================== */

static HalTlmmPlatformDescriptionType *pHALPlatformDesc;

void HAL_tlmmtile_PlatformInit (HalTlmmGpioDescriptionType *pDesc)
{
  uint32 i,j;

   for(i =0 ; i < ARR_SIZE(HALTlmmTileBlockDescriptor) ; i++)
  {
    if( ChipInfo_GetChipFamily() == HALTlmmTileBlockDescriptor[i].eChipFamily)
    {
      pHALPlatformDesc = &HALTlmmTileBlockDescriptor[i];
    }
  }

  pDesc->nMaxGpio                                 = pHALPlatformDesc->nMaxGpio;
  pDesc->nNumTiles                                = pHALPlatformDesc->nNumTiles;
  pDesc->pGpio                                    = &pHalTlmmGpioArray[0];
  pDesc->GpioConfig.nOffset                       = (uint32)( HWIO_TLMM_GPIO_CFG0_ADDR - HWIO_TLMM_GPIO_CFG0_ADDR ); 
  pDesc->GpioConfig.nGpioPullShift                = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_PULL_SHFT ); 
  pDesc->GpioConfig.nGpioPullBmsk                 = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_PULL_BMSK ); 
  pDesc->GpioConfig.nGpioFuncSelShift             = (uint32)( HWIO_TLMM_GPIO_CFG0_FUNC_SEL_SHFT ); 
  pDesc->GpioConfig.nGpioFuncSelBmsk              = (uint32)( HWIO_TLMM_GPIO_CFG0_FUNC_SEL_BMSK ); 
  pDesc->GpioConfig.nGpioDrvStrShift              = (uint32)( HWIO_TLMM_GPIO_CFG0_DRV_STRENGTH_SHFT ); 
  pDesc->GpioConfig.nGpioDrvStrBmsk               = (uint32)( HWIO_TLMM_GPIO_CFG0_DRV_STRENGTH_BMSK ); 
  pDesc->GpioConfig.nGpioOEShift                  = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_OE_SHFT ); 
  pDesc->GpioConfig.nGpioOEBmsk                   = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_OE_BMSK ); 
  pDesc->GpioConfig.nGpioHiHysShift               = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_HIHYS_EN_SHFT ); 
  pDesc->GpioConfig.nGpioHiHysBmsk                = (uint32)( HWIO_TLMM_GPIO_CFG0_GPIO_HIHYS_EN_BMSK ); 
  //pDesc->GpioConfig.nGpioEgpioPresentShift        = (uint32)( HWIO_TLMM_GPIO_CFG0_EGPIO_PRESENT_SHFT ); 
  //pDesc->GpioConfig.nGpioEgpioPresentBmsk         = (uint32)( HWIO_TLMM_GPIO_CFG0_EGPIO_PRESENT_BMSK ); 
  //pDesc->GpioConfig.nGpioEgpioEnableShift         = (uint32)( HWIO_TLMM_GPIO_CFG0_EGPIO_ENABLE_SHFT ); 
  //pDesc->GpioConfig.nGpioEgpioEnableBmsk          = (uint32)( HWIO_TLMM_GPIO_CFG0_EGPIO_ENABLE_BMSK ); 
  pDesc->GpioInOut.nOffset                        = (uint32)( HWIO_TLMM_GPIO_IN_OUT0_ADDR - HWIO_TLMM_GPIO_CFG0_ADDR ); 
  pDesc->GpioInOut.nGpioInShift                   = (uint32)( HWIO_TLMM_GPIO_IN_OUT0_GPIO_IN_SHFT ); 
  pDesc->GpioInOut.nGpioInBmsk                    = (uint32)( HWIO_TLMM_GPIO_IN_OUT0_GPIO_IN_BMSK ); 
  pDesc->GpioInOut.nGpioOutShift                  = (uint32)( HWIO_TLMM_GPIO_IN_OUT0_GPIO_OUT_SHFT ); 
  pDesc->GpioInOut.nGpioOutBmsk                   = (uint32)( HWIO_TLMM_GPIO_IN_OUT0_GPIO_OUT_BMSK ); 
  pDesc->GpioIntrConfig.nOffset                   = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_ADDR - HWIO_TLMM_GPIO_CFG0_ADDR ); 
  pDesc->GpioIntrConfig.nGpioIntrEnShift          = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_ENABLE_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrEnBmsk           = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_ENABLE_BMSK ); 
  pDesc->GpioIntrConfig.nGpioIntrPolShift         = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_POL_CTL_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrPolBmsk          = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_POL_CTL_BMSK ); 
  pDesc->GpioIntrConfig.nGpioIntrDetShift         = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_DECT_CTL_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrDetBmsk          = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_DECT_CTL_BMSK ); 
  pDesc->GpioIntrConfig.nGpioIntrRawStatusShift   = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_RAW_STATUS_EN_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrRawStatusBmsk    = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_INTR_RAW_STATUS_EN_BMSK ); 
  pDesc->GpioIntrConfig.nGpioIntrTargetProcShift  = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_TARGET_PROC_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrTargetProcBmsk   = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_TARGET_PROC_BMSK ); 
  pDesc->GpioIntrConfig.nGpioIntrDirConnEnShift   = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_DIR_CONN_EN_SHFT ); 
  pDesc->GpioIntrConfig.nGpioIntrDirConnEnBmsk    = (uint32)( HWIO_TLMM_GPIO_INTR_CFG0_DIR_CONN_EN_BMSK ); 
  pDesc->GpioIntrStatus.nOffset                   = (uint32)( HWIO_TLMM_GPIO_INTR_STATUS0_ADDR - HWIO_TLMM_GPIO_CFG0_ADDR ); 
  pDesc->GpioIntrStatus.nGpioIntrStatusShift      = (uint32)( HWIO_TLMM_GPIO_INTR_STATUS0_INTR_STATUS_SHFT ); 
  pDesc->GpioIntrStatus.nGpioIntrStatusBmsk       = (uint32)( HWIO_TLMM_GPIO_INTR_STATUS0_INTR_STATUS_BMSK ); 

  for ( j=0 ; j < pHALPlatformDesc->nNumDirConnectBases ; j++)
  {
    pDesc->DirConnIntrCfg[j].nBase                     = &(pHALPlatformDesc->pHalTlmmTileDirConnArray[j][0]); 
    pDesc->DirConnIntrCfg[j].nMax                      =  HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_MAXn; 
    pDesc->DirConnIntrCfg[j].nDirConnIntrCfgSelShift   = (uint32)( HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_GPIO_SEL_SHFT ); 
    pDesc->DirConnIntrCfg[j].nDirConnIntrCfgSelBmsk    = (uint32)( HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_GPIO_SEL_BMSK ); 
    pDesc->DirConnIntrCfg[j].nDirConnIntrCfgPolShift   = (uint32)( HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_POLARITY_SHFT ); 
    pDesc->DirConnIntrCfg[j].nDirConnIntrCfgPolBmsk    = (uint32)( HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_POLARITY_BMSK );
    pDesc->DirConnIntrCfg[j].nOffset                   = (uint32)( HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_ADDR(1) - HWIO_TLMM_EAST_DIR_CONN_INTRn_CFG_MSS_ADDR(0) );  
  }
   
  pDesc->GpioIdStatus.nOffset                     = (uint32)( HWIO_TLMM_GPIO_ID_STATUS0_ADDR - HWIO_TLMM_GPIO_CFG0_ADDR );
  pDesc->GpioIdStatus.nGPIOIdStatusShift          = (uint32)( HWIO_TLMM_GPIO_ID_STATUS0_GPIO_ID_STATUS_SHFT );
  pDesc->GpioIdStatus.nGPIOIdStatusBmsk           = (uint32)( HWIO_TLMM_GPIO_ID_STATUS0_GPIO_ID_STATUS_BMSK );
  pDesc->nNumDirConnectBases                      =  pHALPlatformDesc->nNumDirConnectBases;
} /* END HAL_tlmmtile_PlatformInit */


/* ===========================================================================
**  HAL_tlmmtile_PlatformGetGpioBase
**
** ======================================================================== */

void HAL_tlmmtile_PlatformGetGpioBase ( HalTlmmGpioDescriptionType *pDesc, uint32 nGpio )
{
  uint32 nTileAddr, nTileIter;

  /*
   * Initialize the tile address to the first tile for the case where there is only 1 tile
   */
  pHalTlmmGpioArray[nGpio] = pHALPlatformDesc->aTiles[0] + ( nGpio * (pHALPlatformDesc->nPinRegWidth) );
  for ( nTileIter = 0; nTileIter < pDesc->nNumTiles; nTileIter++ )
  {
    /*
     * Read potential base address for this GPIO
     */
    nTileAddr = pHALPlatformDesc->aTiles[nTileIter] + ( nGpio * (pHALPlatformDesc->nPinRegWidth) );
    if ( in_dword(nTileAddr + pDesc->GpioIdStatus.nOffset) & pDesc->GpioIdStatus.nGPIOIdStatusBmsk )
    {
      /*
       * Found the tile, break out
       */
      pHalTlmmGpioArray[nGpio] = nTileAddr;
      break;
    }
  }
} /* END HAL_tlmmtile_PlatformGetGpioBase */
