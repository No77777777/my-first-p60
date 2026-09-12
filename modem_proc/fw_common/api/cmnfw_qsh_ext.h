/*!
  @file
  cmnfw_qsh_ext.h

  @brief
  QSH dump structure for common FW registers and states

*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

#ifndef CMNFW_QSH_EXT_H
#define CMNFW_QSH_EXT_H

/*! @brief Common FW QSH dump Major version */
#define CMNFW_QSH_MAJOR_VER 1

/*! @brief Common FW QSH dump Minor version */
#define CMNFW_QSH_MINOR_VER 0

/*! @brief QSH Dump size in bytes */
#define CMNFW_QSH_DUMP_NUM_BYTES             (1024)

/*! @brief Number of RXFE WB chains */
#define CMNFW_QSH_DUMP_NUM_RXFE_WB_CHAINS    (4)

/*! @brief Number of RXFE NB chains */
#define CMNFW_QSH_DUMP_NUM_RXFE_NB_CHAINS    (6)

/*! @brief Number of MCDMA HW channels */
#define CMNFW_QSH_DUMP_NUM_MCDMA_HW_CHANNELS (4)

/*! @brief Number of MCDMA SW channels */
#define CMNFW_QSH_DUMP_NUM_MCDMA_SW_CHANNELS (5)

/*! @brief Number of MTC DBG ports */
#define CMNFW_QSH_DUMP_NUM_MTC_DBG_PORTS     (16)

/*! @brief Number of DEMBACK */
#define CMNFW_QSH_DUMP_NUM_DEMBACK           (1)

/*! @brief Number of DEMBACK DBBUF */
#define CMNFW_QSH_DUMP_NUM_DEMBACK_DBBUF     (4)

/*! @brief Number of TX chains */
#define CMNFW_QSH_DUMP_NUM_TX_CHAINS         (1)

/*! @brief Number of Silvers */
#define CMNFW_QSH_DUMP_NUM_SILVER            (1)

/***************************************************************************/
/*                   RXFE QSH dump                                        */
/***************************************************************************/

/*! @brief RXFE PLL register dump */
typedef struct
{
  uint32 MSS_CC_MSS_MPLL1_MODE;
  uint32 MSS_CC_MSS_MPLL1_USER_CTL;
  uint32 MSS_CC_MSS_MPLL1_CONFIG_CTL;
  uint32 MSS_CC_MSS_MPLL1_STATUS;
} cmnfw_qsh_dump_rxfe_pll_t;

/*! @brief RXFE Top register dump */
typedef struct
{
  uint32 RXFE_ENABLE;
  uint32 RXFE_TOP_STATUS;
  uint32 RXFE_CMNNOTCH_FILT01_CFG;
  uint32 RXFE_CMNNOTCH_FILT23_CFG;
  uint32 RXFE_ACI_FILT0_CFG;
  uint32 RXFE_ACI_FILT1_CFG;
  uint32 RXFE_WBw_ADCMUX_CFG[CMNFW_QSH_DUMP_NUM_RXFE_WB_CHAINS];
  uint32 RXFE_NBn_WBMUX_CFG[CMNFW_QSH_DUMP_NUM_RXFE_NB_CHAINS];
} cmnfw_qsh_dump_rxfe_top_t;

/*! @brief RXFE bridge register dump */
typedef struct
{
  uint32 RXFE_WRTR_XFER_CTL;
  uint32 RXFE_BRDG_CTL;
  uint32 RXFE_BRDG_STATUS;
  uint32 RXFE_BRDG_CLEAR;
  uint32 RXFE_BRDG_ARB_PRI;
  uint32 RXFE_BRDG_AXI_PRI;
  uint32 RXFE_WRTRm_CFG0[CMNFW_QSH_DUMP_NUM_RXFE_NB_CHAINS];
  uint32 RXFE_WRTRm_CFG1[CMNFW_QSH_DUMP_NUM_RXFE_NB_CHAINS];
} cmnfw_qsh_dump_rxfe_brdg_t;

/*! @brief RXFE WB register dump  */
typedef struct
{
  uint32 RXFE_WB_TOP_CTL_WBw;
  uint32 RXFE_WB_TOP_STATUS_WBw;
  uint32 RXFE_WB_MISC_CFG_WBw;
  uint32 RXFE_WB_WBPWR_CFG_WBw;
  uint32 RXFE_WB_WBDC_CFG_WBw;
  uint32 RXFE_WB_VSRC_CFG_WBw;
  uint32 RXFE_WB_VSRC_STATUS_WBw;
  uint32 RXFE_WB_DEC_FILT_CFG_WBw;
} cmnfw_qsh_dump_rxfe_wb_t;

/*! @brief RXFE NB register dump */
typedef struct
{
  uint32 RXFE_NB_TOP_CTL_NBn;
  uint32 RXFE_NB_FILTERS_CFG_NBn;
  uint32 RXFE_NB_GDA_NBn;
  uint32 RXFE_NB_NBDC_COMPVAL_I_NBn;
  uint32 RXFE_NB_NBDC_COMPVAL_Q_NBn;
  uint32 RXFE_NB_NBPWR_CFG_NBn;
  uint32 RXFE_NB_WTR_CFG_NBn;
  uint32 RXFE_NB_WTR_FUNC_CFG_NBn;
} cmnfw_qsh_dump_rxfe_nb_t;


/*! @brief RXFE QSH dump structure */
typedef struct
{
  /*! @brief RXFE PLL register dump */
  cmnfw_qsh_dump_rxfe_pll_t pll_dump;

  /*! @brief RXFE top register dump */
  cmnfw_qsh_dump_rxfe_top_t rxfe_top_dump;

  /*! @brief RXFE bridge register dump */
  cmnfw_qsh_dump_rxfe_brdg_t brdg_dump;

  /*! @brief RXFE WB register dump */
  cmnfw_qsh_dump_rxfe_wb_t wb_dump[CMNFW_QSH_DUMP_NUM_RXFE_WB_CHAINS];

  /*! @brief RXFE NB register dump */
  cmnfw_qsh_dump_rxfe_nb_t nb_dump[CMNFW_QSH_DUMP_NUM_RXFE_NB_CHAINS];
} cmnfw_qsh_dump_rxfe_t;


/***************************************************************************/
/*                   MCDMA QSH dump                                       */
/***************************************************************************/

/*! @brief MCDMA QSH dump structure */
typedef struct
{
  uint32 MCDMA_ENABLE;
  uint32 MCDMA_LOG_CH_BUSY;
  uint32 MCDMA_PHY_CH_CTL;
  uint32 MCDMA_TS_STATUS_WORD0;
  uint32 MCDMA_LOG_HW_CHn_DBG[CMNFW_QSH_DUMP_NUM_MCDMA_HW_CHANNELS];
  uint32 MCDMA_LOG_SW_CHn_DBG[CMNFW_QSH_DUMP_NUM_MCDMA_SW_CHANNELS];
  uint32 MCDMA_TS_DMA_CHn_WORD0[CMNFW_QSH_DUMP_NUM_MCDMA_SW_CHANNELS];
  uint32 MCDMA_TS_DMA_CHn_WORD1[CMNFW_QSH_DUMP_NUM_MCDMA_SW_CHANNELS];
  uint32 MCDMA_TS_DMA_CHn_WORD2[CMNFW_QSH_DUMP_NUM_MCDMA_SW_CHANNELS];

} cmnfw_qsh_dump_mcdma_t;


/***************************************************************************/
/*                   MTC DBG QSH dump                                     */
/***************************************************************************/

/*! @brief MTC DBG QSH dump structure */
typedef struct
{
  uint32 DBG_ENABLE;
  uint32 DBG_LOG_STATUS;
  uint32 DBG_MUX_MASK_n[CMNFW_QSH_DUMP_NUM_MTC_DBG_PORTS];
  uint32 DBG_ERR_IRQ_STATUS0;
  uint32 DBG_ERR_IRQ_STATUS1;
} cmnfw_qsh_dump_mtc_dbg_t;


/***************************************************************************/
/*                   CCS QSH dump                                         */
/***************************************************************************/

/*! @brief CCS QSH dump structure */
typedef struct
{
  uint32 CCS_MSTS;
  uint32 CCS_MCTL;
  uint32 CCS_DMAC;

} cmnfw_qsh_dump_ccs_t;

/***************************************************************************/
/*                   DEMBACK QSH dump                                      */
/***************************************************************************/

/*! @brief DEMBACK common registers */
typedef struct
{
  uint32 DEMBACK_EN;
  uint32 DEMBACK_MODE_SEL;
} cmnfw_qsh_demback_common_register_dump_t;

/*! @brief DEMBACK bridge registers */
typedef struct
{
  uint32 DEMBACK_RDR0_XFER_CTL;
  uint32 DEMBACK_RDR1_XFER_CTL;
  uint32 DEMBACK_RDR2_XFER_CTL;
  uint32 DEMBACK_RDR3_XFER_CTL;
  uint32 DEMBACK_BRDG_CTL;
  uint32 DEMBACK_BRDG_STATUS;
} cmnfw_qsh_demback_brdg_register_dump_t;

/*! @brief DEMBACK DBBUF registers */
typedef struct
{

  uint32 DEMBACK_DBBUF_BRDG_CTL_DBd;
  uint32 DEMBACK_DBBUF_BRDG_STATUS_DBd;
  uint32 DEMBACK_DBBUF_RDRi_XFER_CTL_DBd[CMNFW_QSH_DUMP_NUM_DEMBACK_DBBUF];
  uint32 DEMBACK_DBBUF_WRTRi_XFER_CTL_DBd[CMNFW_QSH_DUMP_NUM_DEMBACK_DBBUF];
} cmnfw_qsh_demback_db_buf_register_dump_t;

/*! @brief Structure to store DEMBACK register information on crash */
typedef struct
{
  /*! @brief DEMBACK_COMMON register dump structure */
  cmnfw_qsh_demback_common_register_dump_t common_dump;
  /*! @brief DEMBACK_BRDG register dump structure */
  cmnfw_qsh_demback_brdg_register_dump_t brdg_dump;
  /*! @brief DB_BUF register dump structure */
  cmnfw_qsh_demback_db_buf_register_dump_t db_buf_dump[CMNFW_QSH_DUMP_NUM_DEMBACK];

} cmnfw_qsh_dump_demback_t;

/***************************************************************************/
/*                   MTC CLK QSH dump                                         */
/***************************************************************************/

/*! @brief MTC CLK register dump at crash*/
typedef struct
{
  uint32 MTC_ENABLE;
  uint32 MODEM_PWRUP;
  uint32 MODEM_PWRON;
} cmnfw_qsh_dump_mtc_clk_t;

/***************************************************************************/
/*                   TXFE QSH dump                                        */
/***************************************************************************/

/*! @brief TXFE TXC register dump at crash */
typedef struct
{
  uint32 TXC_DP_CFG_An;
  uint32 TXC_SAMPLE_COUNT_An;
} cmnfw_qsh_dump_txfe_txc_reg_t;

/*! @brief TXFE TXR register dump at crash */
typedef struct
{
  uint32 TXR_IQ_DAC_An;
} cmnfw_qsh_dump_txfe_txr_reg_t;

/*! @brief TXFE TOP register dump at crash */
typedef struct
{
  uint32 TX_ENABLE;
  uint32 TX_MODE_COMMON;
  uint32 TX_MODE_c[CMNFW_QSH_DUMP_NUM_TX_CHAINS];
} cmnfw_qsh_dump_txfe_top_reg_t;

/*! @brief TXFE COMBODAC register dump at crash */
typedef struct
{
  uint32 COMBODAC0_CLK_CFG;//Tabasco register
  uint32 COMBODAC1_CLK_CFG; // tabasco register
} cmnfw_qsh_dump_txfe_combodac_reg_t;

/*! @brief TXFE COMBODAC register dump at crash */
typedef struct
{
  uint32 DAC_CLK_CFG;//jacala register
} cmnfw_qsh_dump_txfe_txdac_reg_t;

typedef union
{
 cmnfw_qsh_dump_txfe_combodac_reg_t tabasco_dac;
 cmnfw_qsh_dump_txfe_txdac_reg_t jacala_dac;
}cmnfw_qsh_dump_txfe_dac_reg_t;

/*! @brief TXFE PLL register dump at crash */
typedef struct
{
  uint32 MSS_QDSP6SS_PLL_STATUS;
} cmnfw_qsh_dump_txfe_pll_reg_t;

/*! @brief fatal error register dump structure */
typedef struct
{
  /*! @brief TXFE top register dump */
  cmnfw_qsh_dump_txfe_top_reg_t top_dump;

  cmnfw_qsh_dump_txfe_dac_reg_t dac_dump;

  /*! @brief TXFE pll register dump */
  cmnfw_qsh_dump_txfe_pll_reg_t pll_dump;

  /*! @brief TXFE TXC register dump */
  cmnfw_qsh_dump_txfe_txc_reg_t txc_dump[CMNFW_QSH_DUMP_NUM_TX_CHAINS];

  /*! @brief TXFE TXR register dump */
  cmnfw_qsh_dump_txfe_txr_reg_t txr_dump[CMNFW_QSH_DUMP_NUM_TX_CHAINS];
} cmnfw_qsh_dump_txfe_t;

/***************************************************************************/
/*                   COPROC QSH dump                                      */
/***************************************************************************/

/*! @brief Coproc registers */
typedef struct cmnfw_qsh_dump_coproc_crash_dump_s
{
  /*! @brief Registers related to Q6_CP boot-up, wakeup, and sleep */
  struct cmnfw_qsh_dump_coproc_reg_dump_s 
  {
    uint32 RST_EVB;
    uint32 PWR_CTL;
    uint32 MEM_PWR_CTL;
    uint32 GFMUX_CTL;
    uint32 SAW2_SPM_STS;
    uint32 SAW2_SPM_STS2;
    uint32 SLPC_CFG;
    uint32 BLOCK_INTR;
  } reg;
  
  /*! @brief Registers related to Silver engines */
  struct cmnfw_qsh_dump_coproc_silver_crash_dump_s 
  {
    uint32 RESET;
    uint32 PWR_CTL;
    uint32 PWR_STATUS;
    uint32 CLK_CTL;
  } silver[CMNFW_QSH_DUMP_NUM_SILVER];
} cmnfw_qsh_dump_coproc_t;

/***************************************************************************/
/*                   L1S QSH dump                                         */
/***************************************************************************/

/*! @brief L1S QSH dump structure */
typedef struct
{
  uint32 CP_L1S_PWR_CTL;
  uint32 CP_L1S2_PWR_CTL;
} cmnfw_qsh_dump_l1s_t;

/***************************************************************************/
/*                   Overall QSH dump structure                            */
/***************************************************************************/

/*! @brief QSH dump structure */
typedef struct
{
  cmnfw_qsh_dump_rxfe_t         rxfe_dump;                         /*!< RXFE register dump */
  cmnfw_qsh_dump_mcdma_t        mcdma_dump;                        /*!< MCDMA register dump */
  cmnfw_qsh_dump_mtc_dbg_t      mtc_dbg_dump;                      /*!< MTC DBG register dump */
  cmnfw_qsh_dump_ccs_t          ccs_dump;                          /*!< CCS register dump */
  cmnfw_qsh_dump_demback_t      demback_dump;                      /*!< DEMBACK register dump */
  cmnfw_qsh_dump_mtc_clk_t      mtc_clk_dump;                      /*!< MTC CLK register dump */
  cmnfw_qsh_dump_txfe_t         txfe_dump;                         /*!< TXFE register dump */
  cmnfw_qsh_dump_coproc_t       coproc_dump;                       /*!< COPROC register dump */
  cmnfw_qsh_dump_l1s_t          l1s_dump;                          /*!< L1S register dump */

} cmnfw_qsh_dump_int_s;

/*! @brief QSH dump structure definition */
typedef struct
{
  char                 version[64]; /*!< Common FW version */
  boolean              valid;       /*!< If the dump structure is valid */
  cmnfw_qsh_dump_int_s dumps;       /*!< QSH dumps */

} cmnfw_qsh_dump_s;

/***************************************************************************/
/*                   TLV structure                                         */
/***************************************************************************/

/*! @brief Common FW QSH dump enum */
typedef enum
{
  CMNFW_QSH_DUMP_TAG_0 = 0

} cmnfw_qsh_dump_tag_e;

/*! @brief Define the header locally. This should match QSH def "qsh_dump_tag_hdr_s" */
typedef struct
{
  /*! set using dump_tag enum exported by client */
  uint16  dump_tag;
  
  /*! size of dump struct in bytes */
  uint16  dump_struct_size_bytes;
} cmnfw_qsh_dump_tag_hdr_s;

/*! @brief Common FW QSH dump TLV structure */
typedef struct
{
  cmnfw_qsh_dump_tag_hdr_s hdr;             /*!< QSH dump Header */
  cmnfw_qsh_dump_s         cmnfw_qsh_dump;  /*!< QSH dump */

} cmnfw_qsh_dump_tag_0_s;

#endif // CMN_QSH_EXT_H
