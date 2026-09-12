#TBD 
#TZBSP_VMIDCLROE support for No/ACCESS FULL ACCESS type xpu 
#Add #endif for #ifdef  for static structs 

import sys, os
import xml.etree.ElementTree as ET
from optparse import OptionParser

# Declare global variables
root = None 
XPUs = []
config_file_handle = None
enable_file_handle = None

vmids = {
  'ALL':'~TZBSP_VMID_NOACCESS',
  'NO_ACCESS': 'TZBSP_VMID_NOACCESS',
  'RPM': 'TZBSP_VMID_RPM_BIT',
  'TZ': 'TZBSP_VMID_TZ_BIT',
  'AP': 'TZBSP_VMID_AP_BIT',
  'MSS':'TZBSP_VMID_MSS_BIT',
  'LPASS':'TZBSP_VMID_LPASS_BIT',
  'WLAN':'TZBSP_VMID_WLAN_BIT',
  'CP':'TZBSP_VMID_CP_BIT',
  'VIDEO':'TZBSP_VMID_VIDEO_BIT',
  'IPA_UC':'TZBSP_VMID_IPA_UC_BIT',
  'IPA_AP_ALIAS':'TZBSP_VMID_IPA_AP_ALIAS_BIT',
  'IPA_MSS_ALIAS':'TZBSP_VMID_IPA_MSS_ALIAS_BIT',
  'SSC':'TZBSP_VMID_SSC_BIT',
  'IPA_AP': 'TZBSP_VMID_IPA_AP_BIT',
  'VMID_0':'TZBSP_VMID_VMID_0_BIT',
  'AP_CPU':'TZBSP_VMID_AP_BIT',  # Mapped it to AP_VMID for Bear. HB don't use it.
  'ZAP_SHADER': 'TZBSP_VMID_ZAP_SHADER_BIT'
}

vmids_single = {
  'NO_ACCESS': 'TZBSP_VMID_NOACCESS',
  'RPM': 'TZBSP_VMID_RPM',
  'TZ': 'TZBSP_VMID_TZ',
  'AP': 'TZBSP_VMID_AP',
  'MSS':'TZBSP_VMID_MSS',
  'LPASS':'TZBSP_VMID_LPASS',
  'WLAN':'TZBSP_VMID_WLAN',
  'CP':'TZBSP_VMID_CP',
  'VIDEO':'TZBSP_VMID_VIDEO',
  'IPA_UC':'TZBSP_VMID_IPA_UC',
  'IPA_AP_ALIAS':'TZBSP_VMID_IPA_AP_ALIAS',
  'IPA_MSS_ALIAS':'TZBSP_VMID_IPA_MSS_ALIAS',
  'SSC':'TZBSP_VMID_SSC',
  'IPA_AP': 'TZBSP_VMID_IPA_AP',
  'VMID_0':'TZBSP_VMID_VMID_0',
  'AP_CPU':'TZBSP_VMID_AP',  # Mapped it to AP_VMID for Bear. HB don't use it.
  'ZAP_SHADER': 'TZBSP_VMID_ZAP_SHADER'
}

SECURE_RESOURCE = False
MSA_RESOURCE = False
START_ADDRESS = "0"
END_ADDRESS = "0"
READ_VMID_STRING = ""
WRITE_VMID_STRING = ""
GLOBAL_READ = False
GLOBAL_WRITE = False
STATIC_TAG1 = ""
STATIC_TAG2 = None
DEFAULT_TAGNAME = ""
XPU_GLOBAL_MSA = False
DEFAULT_COMMENTED = False
VMIDCLROE = False
VMIDCLRWE = False
MSACLROE = False
MSACLRWE = False
SCLROE = False
SCLRWE = False
VERSION_SUFFIX = "_V1"

xpu_name_to_enum = {

  #     ISTARI XPU

  "ISTARI_XPU_CFG_CPR_APU1132_2_BASE_XPU_CFG_CPR_APU1132_2" : "HAL_XPU2_CFG_CPR",
  "ISTARI_RAMBLUR_PIMEM_RAMBLUR_PIMEM_APU_APU1132_8" : "HAL_XPU2_RAMBLUR_PIMEM_APU",
  "ISTARI_OCIMEM_WRAPPER_CSR_APU" : "HAL_XPU2_IMEM_APU",
  "ISTARI_SPDM_WRAPPER_TOP_SPDM_APU0132_1" : "HAL_XPU2_SPDM_APU",
  "ISTARI_SECURITY_CONTROL_APU1132_37" : "HAL_XPU2_SEC_CTRL_APU",
  "ISTARI_BIMC_BIMC_CONFIG_APU" : "HAL_XPU2_BIMC_APU",
  "ISTARI_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "ISTARI_DCC_WRAPPER_DCC_XPU" : "HAL_XPU2_DCC",
  "ISTARI_UFS_UFS_REGS_UFS_APU" : "HAL_XPU2_UFS",
  "ISTARI_UFS_UFS_REGS_ICE_XPU2" : "HAL_XPU2_UFS_ICE",
  "ISTARI_CRYPTO0_CRYPTO_TOP_XPU2_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "ISTARI_MMSS_MDSS_MDSS_XPU" : "HAL_XPU2_MDSS_APU",
  "ISTARI_MMSS_VENUS0_VENUS_WRAPPER_BASE_VENUS_APU1132_3_QR" : "HAL_XPU2_MMSS_VENUS",
  "ISTARI_TLMM_TLMM_APU1032_175" : "HAL_XPU2_TLMM",
  "ISTARI_SSC_SSC_BLSP_BLSP_BAM_XPU2" : "HAL_XPU2_SSC_BLSP_BAM",
  "ISTARI_PERIPH_SS_PERIPH_SS_SDC1_SDCC5_TOP_XPU2" : "HAL_XPU2_BAM_SDCC1",
  "ISTARI_PERIPH_SS_PERIPH_SS_SDC2_SDCC5_TOP_XPU2" : "HAL_XPU2_BAM_SDCC2",
  "ISTARI_PERIPH_SS_PERIPH_SS_BLSP1_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP1_DMA",
  "ISTARI_PERIPH_SS_PERIPH_SS_BLSP2_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP2_DMA",
  "ISTARI_XPU_CFG_SNOC_CFG_MPU1032_4_M16L12_AHB_BASE_XPU_CFG_SNOC_CFG_MPU1032_4_M16L12_AHB" : "HAL_XPU2_SNOC_CFG",
  "ISTARI_XPU_CFG_ANOC0_CFG_MPU1032_4_M16L12_AHB_BASE_XPU_CFG_ANOC0_CFG_MPU1032_4_M16L12_AHB" : "HAL_XPU2_CFG_ANOC0_CFG",
  "ISTARI_XPU_CFG_ANOC1_CFG_MPU1032_4_M16L12_AHB_BASE_XPU_CFG_ANOC1_CFG_MPU1032_4_M16L12_AHB" : "HAL_XPU2_CFG_ANOC1_CFG",
  "ISTARI_XPU_CFG_ANOC2_CFG_MPU1032_4_M16L12_AHB_BASE_XPU_CFG_ANOC2_CFG_MPU1032_4_M16L12_AHB" : "HAL_XPU2_CFG_ANOC2_CFG",
  "ISTARI_XPU_CFG_QDSS_MPU1132_3_M23L12_AHB_BASE_XPU_CFG_QDSS_MPU1132_3_M23L12_AHB" : "HAL_XPU2_RBCR_QDSS",
  "ISTARI_XPU_CFG_SSC_MPU1032_10_M22L11_AHB_BASE_XPU_CFG_SSC_MPU1032_10_M22L11_AHB" : "HAL_XPU2_CFG_SSC",
  "ISTARI_RAMBLUR_PIMEM_RAMBLUR_PIMEM_MPU_MPU1132A_8_M23L10_AXI_36" : "HAL_XPU2_RAMBLUR_PIMEM_MPU",
  "ISTARI_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "ISTARI_BOOT_ROM_BOOT_ROM_MPU1032_4_M17L10_AHB" : "HAL_XPU2_BOOT_ROM",
  "ISTARI_RPM_MPU" : "HAL_XPU2_RPM_MSTR_MPU",
  "ISTARI_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "ISTARI_BIMC_BIMC_S_DDR1" : "HAL_XPU2_BIMC_MPU1",
  "ISTARI_MMSS_U_XPU2_AHB_WRAPPER_MNOC" : "HAL_XPU2_MMAGIC_XPU2_MNOC",
  "ISTARI_MMSS_U_XPU2_AHB_WRAPPER" : "HAL_XPU2_MMAGIC_DSA_MPU",
  "ISTARI_SSC_SSC_Q6_MPU_MPU1032_12_M22L11_AHB" : "HAL_XPU2_SSC_Q6_MPU",
  "ISTARI_PMIC_ARB_PMIC_ARB_MPU1132_18_M25L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "ISTARI_PERIPH_SS_PERIPH_SS_MPU_MPU1132A_4_M15L7_AHB" : "HAL_XPU2_PNOC_MPU",
  "ISTARI_LPASS_LPASS_Q6SS_MPU" : "HAL_XPU2_LPASS_QDSP6SS",
  "ISTARI_CLK_CTL_GCC_RPU_RPU0032_144_L12" : "HAL_XPU2_CLK_CTL",
  "ISTARI_CORE_TOP_CSR_TCSR_MUTEX_RPU1132_64_L12" : "HAL_XPU2_TCSR_MUTEX",

  "ISTARI_ISTARI_SKL_WRAPPER_XPU2" : "HAL_XPU2_SKL",
  "ISTARI_IPA_0_IPA_WRAPPER_BASE_BAM_NDP_XPU2": "HAL_XPU2_IPA",
  "ISTARI_PERIPH_SS_PERIPH_SS_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "ISTARI_MSS_MPU_MPU1032A_16_M35L12_AXI_36_BASE_MSS_MPU_MPU1032A_16_M35L12_AXI_36": "HAL_XPU2_MSS_MPU",
  "ISTARI_MSS_Q6_MPU_MPU1032A_16_M35L12_AXI_36_BASE_MSS_Q6_MPU_MPU1032A_16_M35L12_AXI_36": "HAL_XPU2_MSS_Q6_MPU",
  "ISTARI_ANOC1_MPU_MPU1032A_16_M35L12_AXI_36_BASE_ANOC1_MPU_MPU1032A_16_M35L12_AXI_36": "HAL_XPU2_ANOC1_MPU",
  "ISTARI_ANOC2_MPU_MPU1032A_16_M35L12_AXI_36_BASE_ANOC2_MPU_MPU1032A_16_M35L12_AXI_36": "HAL_XPU2_ANOC2_MPU",
  "ISTARI_CORE_TOP_CSR_TCSR_REGS_RPU1132_32_L12" : "HAL_XPU2_TCSR_REGS",
  # Below is the v1 entry
  "ISTARI_CORE_TOP_CSR_TCSR_REGS_RPU1132_24_L12" : "HAL_XPU2_TCSR_REGS",
  "ISTARI_MMSS_U_XPU2_MMCC_WRAPPER": "HAL_XPU2_MMSS_CC",
  "ISTARI_UFS_UFS_REGS_ICE_ICE_XPU2": "HAL_XPU2_UFS_ICE",

  #     SAHI XPU

  "SAHI_XPU_CFG_PRNG_CFG_APU1132_1_BASE_XPU_CFG_PRNG_CFG_APU1132_1" : "HAL_XPU2_PRNG_CFG",
  "SAHI_SECURITY_CONTROL_APU1132_10" : "HAL_XPU2_SEC_CTRL_APU",
  "SAHI_RPM_APU" : "HAL_XPU2_RPM_APU",
  "SAHI_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "SAHI_DEHR_BIMC_WRAPPER_DEHR_XPU" : "HAL_XPU2_DEHR",
  "SAHI_CRYPTO0_CRYPTO_TOP_XPU2_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "SAHI_CORE_TOP_CSR_TCSR_REGS_APU1132_16" : "HAL_XPU2_TCSR_REGS",
  "SAHI_VENUS0_VENUS_VENUS_WRAPPER_BASE_VENUS_APU1132_4_QR_12" : "HAL_XPU2_MMSS_VENUS",
  "SAHI_MSS_TOP_MSS_APU0132_5" : "HAL_XPU2_MSS",
  "SAHI_SDC1_SDCC5_TOP_XPU2" : "HAL_XPU2_SDC1_SDCC",
  "SAHI_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "SAHI_SDC2_SDCC5_TOP_XPU2" : "HAL_XPU2_SDC2_SDCC",
  "SAHI_BLSP1_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP1_DMA",
  "SAHI_IPA_WRAPPER_BASE_BAM_NDP_XPU2" : "HAL_XPU2_IPA_BAM_NDP",
  "SAHI_BLSP2_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP2_DMA",
  "SAHI_WCSS_WCSS_APU" : "HAL_XPU2_PRONTO",
  "SAHI_XPU_CFG_SNOC_CFG_MPU1132_4_M19L12_AHB_BASE_XPU_CFG_SNOC_CFG_MPU1132_4_M19L12_AHB" : "HAL_XPU2_SNOC_CFG",
  "SAHI_XPU_CFG_PCNOC_CFG_MPU1132_4_M19L12_AHB_BASE_XPU_CFG_PCNOC_CFG_MPU1132_4_M19L12_AHB" : "HAL_XPU2_PCNOC_CFG",
  "SAHI_XPU_CFG_RPM_CFG_MPU1132_2_M21L12_AHB_BASE_XPU_CFG_RPM_CFG_MPU1132_2_M21L12_AHB" : "HAL_XPU2_RPM_CFG",
  "SAHI_SMMU_500_MPU_WRAPPER_SMMU_SS_TCU_MPU" : "HAL_XPU2_SMMU_500_TCU_MPU",
  "SAHI_SMMU_500_MPU_WRAPPER_SMMU_SS_DIME_MPU" : "HAL_XPU2_SMMU_500_DIME_MPU",
  "SAHI_SMMU_500_MPU_WRAPPER_SMMU_SS_GPS_MPU" : "HAL_XPU2_SMMU_500_GPS_MPU",
  "SAHI_SMMU_500_MPU_WRAPPER_SMMU_SS_QDSP_MPU" : "HAL_XPU2_SMMU_500_QDSP_MPU",
  "SAHI_SMMU_500_MPU_WRAPPER_SMMU_SS_IPA_MPU" : "HAL_XPU2_IPA" ,
  "SAHI_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "SAHI_BOOT_ROM_BOOT_ROM_MPU1032_3_M20L12_AHB" : "HAL_XPU2_BOOT_ROM",
  "SAHI_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "SAHI_TLMM_TLMM_MPU1132_16_M22L12_AHB" : "HAL_XPU2_TLMM",
  "SAHI_PMIC_ARB_PMIC_ARB_MPU1132_25_M25L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "SAHI_LPASS_LPASS_TCSR_XPU" : "HAL_XPU2_LPASS_TOP",
  "SAHI_CLK_CTL_GCC_RPU_RPU1132_32_L12" : "HAL_XPU2_CLK_CTL",

  #     ELDARION XPU

  "ELDARION_XPU_CFG_PRNG_CFG_APU1132_1_BASE_XPU_CFG_PRNG_CFG_APU1132_1" : "HAL_XPU2_PRNG_CFG",
  "ELDARION_SECURITY_CONTROL_APU1132_19" : "HAL_XPU2_SEC_CTRL_APU",
  "ELDARION_RPM_APU" : "HAL_XPU2_RPM_APU",
  "ELDARION_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "ELDARION_DEHR_BIMC_WRAPPER_DEHR_XPU" : "HAL_XPU2_DEHR",
  "ELDARION_CRYPTO0_CRYPTO_TOP_XPU2_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "ELDARION_CORE_TOP_CSR_TCSR_REGS_APU1132_16" : "HAL_XPU2_TCSR_REGS",
  "ELDARION_VENUS0_VENUS_VENUS_WRAPPER_BASE_VENUS_APU1132_3_QR" : "HAL_XPU2_MMSS_VENUS",
  "ELDARION_MSS_TOP_MSS_APU0132_5" : "HAL_XPU2_MSS",
  "ELDARION_SDC1_SDCC5_TOP_XPU2" : "HAL_XPU2_SDC1_SDCC",
  "ELDARION_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "ELDARION_SDC2_SDCC5_TOP_XPU2" : "HAL_XPU2_SDC2_SDCC",
  "ELDARION_BLSP1_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP1_DMA",
  "ELDARION_IPA_WRAPPER_BASE_BAM_NDP_XPU2" : "HAL_XPU2_IPA_BAM_NDP",
  "ELDARION_BLSP2_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP2_DMA",
  "ELDARION_WCSS_WCSS_APU" : "HAL_XPU2_PRONTO",
  "ELDARION_XPU_CFG_SNOC_CFG_MPU1132_4_M19L12_AHB_BASE_XPU_CFG_SNOC_CFG_MPU1132_4_M19L12_AHB" : "HAL_XPU2_SNOC_CFG",
  "ELDARION_XPU_CFG_PCNOC_CFG_MPU1132_4_M19L12_AHB_BASE_XPU_CFG_PCNOC_CFG_MPU1132_4_M19L12_AHB" : "HAL_XPU2_PCNOC_CFG",
  "ELDARION_XPU_CFG_RPM_CFG_MPU1132_2_M21L12_AHB_BASE_XPU_CFG_RPM_CFG_MPU1132_2_M21L12_AHB" : "HAL_XPU2_RPM_CFG",
  "ELDARION_SMMU_500_MPU_WRAPPER_SMMU_SS_TCU_MPU" : "HAL_XPU2_SMMU_500_TCU_MPU",
  "ELDARION_SMMU_500_MPU_WRAPPER_SMMU_SS_DIME_MPU" : "HAL_XPU2_SMMU_500_DIME_MPU",
  "ELDARION_SMMU_500_MPU_WRAPPER_SMMU_SS_GPS_MPU" : "HAL_XPU2_SMMU_500_GPS_MPU",
  "ELDARION_SMMU_500_MPU_WRAPPER_SMMU_SS_QDSP_MPU" : "HAL_XPU2_SMMU_500_QDSP_MPU",
  "ELDARION_SMMU_500_MPU_WRAPPER_SMMU_SS_IPA_MPU" : "HAL_XPU2_IPA",
  "ELDARION_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "ELDARION_XPU_CFG_DSA_CFG_MPU1132_3_M23L12_AHB_BASE_XPU_CFG_DSA_CFG_MPU1132_3_M23L12_AHB" : "HAL_XPU2_CFG_DSA",
  "ELDARION_BOOT_ROM_BOOT_ROM_MPU1032_3_M20L12_AHB" : "HAL_XPU2_BOOT_ROM",
  "ELDARION_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "ELDARION_BIMC_BIMC_S_DDR1" : "HAL_XPU2_BIMC_MPU1",
  "ELDARION_TLMM_TLMM_MPU1132_16_M22L12_AHB" : "HAL_XPU2_TLMM",
  "ELDARION_PMIC_ARB_PMIC_ARB_MPU1132_25_M25L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "ELDARION_LPASS_LPASS_TCSR_XPU" : "HAL_XPU2_LPASS_TOP",
  "ELDARION_CLK_CTL_GCC_RPU_RPU1132_32_L12" : "HAL_XPU2_CLK_CTL",

  #      JACALA XPU 
  
  "JACALA_SECURITY_CONTROL_APU1132_19" : "HAL_XPU2_SEC_CTRL_APU",
  "JACALA_RPM_APU" : "HAL_XPU2_RPM_APU",
  "JACALA_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "JACALA_DEHR_BIMC_WRAPPER_DEHR_XPU" : "HAL_XPU2_DEHR",
  "JACALA_CRYPTO0_CRYPTO_TOP_XPU2_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "JACALA_CORE_TOP_CSR_TCSR_APU1132_16" : "HAL_XPU2_TCSR_REGS",
  "JACALA_A5X_A5X_XPU2" : "HAL_XPU2_A5X",
  "JACALA_VENUS0_VENUS_VENUS_WRAPPER_BASE_VENUS_APU1132_3_QR" : "HAL_XPU2_MMSS_VENUS",
  "JACALA_MSS_TOP_MSS_APU0132_5" : "HAL_XPU2_MSS",
  "JACALA_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "JACALA_BLSP1_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP1_DMA",
  "JACALA_IPA_WRAPPER_BASE_BAM_NDP_XPU2" : "HAL_XPU2_IPA_BAM_NDP",
  "JACALA_BLSP2_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP2_DMA",
  "JACALA_WCSS_WCSS_APU" : "HAL_XPU2_PRONTO",
  "JACALA_XPU_CFG_SNOC_CFG_MPU1132_4_M18L12_AHB_BASE_XPU_CFG_SNOC_CFG_MPU1132_4_M18L12_AHB" : "HAL_XPU2_SNOC_CFG",
  "JACALA_XPU_CFG_PCNOC_CFG_MPU1132_4_M18L12_AHB_BASE_XPU_CFG_PCNOC_CFG_MPU1132_4_M18L12_AHB" : "HAL_XPU2_PCNOC_CFG",
  "JACALA_XPU_CFG_Q6PCNOC_CFG_MPU0032_20_M31L10_AXI_BASE_XPU_CFG_Q6PCNOC_CFG_MPU0032_20_M31L10_AXI" : "HAL_XPU2_Q6PCNOC_CFG",
  "JACALA_XPU_CFG_RPM_CFG_MPU1132_2_M19L12_AHB_BASE_XPU_CFG_RPM_CFG_MPU1132_2_M19L12_AHB" : "HAL_XPU2_RPM_CFG",
  "JACALA_SMMU_500_MPU_WRAPPER_SMMU_SS_TCU_MPU" : "HAL_XPU2_SMMU_500_TCU_MPU",
  "JACALA_SMMU_500_MPU_WRAPPER_SMMU_SS_MCDMA_MPU" : "HAL_XPU2_SMMU_500_DIME_MPU",
  "JACALA_SMMU_500_MPU_WRAPPER_SMMU_SS_GPS_MPU" : "HAL_XPU2_SMMU_500_GPS_MPU",
  "JACALA_SMMU_500_MPU_WRAPPER_SMMU_SS_QDSP_MPU" : "HAL_XPU2_SMMU_500_QDSP_MPU",
  "JACALA_SMMU_500_MPU_WRAPPER_SMMU_SS_IPA_MPU" : "HAL_XPU2_IPA",
  "JACALA_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "JACALA_BOOT_ROM_BOOT_ROM_MPU1032_3_M19L12_AHB" : "HAL_XPU2_BOOT_ROM",
  "JACALA_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "JACALA_TLMM_TLMM_MPU1132_16_M21L12_AHB" : "HAL_XPU2_TLMM",
  "JACALA_PMIC_ARB_PMIC_ARB_MPU1132_25_M24L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "JACALA_LPASS_LPASS_TCSR_XPU" : "HAL_XPU2_LPASS_TOP",
  "JACALA_CLK_CTL_GCC_RPU_RPU1132_32_L12" : "HAL_XPU2_CLK_CTL",

  #      FEERO XPU 
  
  "FEERO_SECURITY_CONTROL_XPU" : "HAL_XPU2_SEC_CTRL_APU",
  "FEERO_RPM_APU" : "HAL_XPU2_RPM_APU",
  "FEERO_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "FEERO_DEHR_BIMC_WRAPPER_DEHR_XPU" : "HAL_XPU2_DEHR",
  "FEERO_CRYPTO0_CRYPTO_TOP_XPU2_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "FEERO_CORE_TOP_CSR_TCSR_REGS_APU1132_16" : "HAL_XPU2_TCSR_REGS",
  "FEERO_A5X_A5X_XPU2" : "HAL_XPU2_A5X",
  "FEERO_VENUS0_VENUS_VENUS_WRAPPER_BASE_VENUS_APU1132_3_QR" : "HAL_XPU2_MMSS_VENUS",
  "FEERO_MSS_TOP_MSS_APU0132_5" : "HAL_XPU2_MSS",
  "FEERO_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "FEERO_BLSP1_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP1_DMA",
  "FEERO_BLSP2_BLSP_BAM_XPU2" : "HAL_XPU2_BAM_BLSP2_DMA",
  "FEERO_WCSS_WCSS_APU" : "HAL_XPU2_PRONTO",
  "FEERO_XPU_CFG_SNOC_CFG_MPU1132_4_M18L12_AHB_BASE_XPU_CFG_SNOC_CFG_MPU1132_4_M18L12_AHB" : "HAL_XPU2_SNOC_CFG",
  "FEERO_XPU_CFG_PCNOC_CFG_MPU1132_4_M18L12_AHB_BASE_XPU_CFG_PCNOC_CFG_MPU1132_4_M18L12_AHB" : "HAL_XPU2_PCNOC_CFG",
  "FEERO_XPU_CFG_Q6PCNOC_CFG_MPU0032A_20_M31L10_AXI_FEERO_BASE_XPU_CFG_Q6PCNOC_CFG_MPU0032A_20_M31L10_AXI_FEERO" : "HAL_XPU2_Q6PCNOC_CFG",
  "FEERO_XPU_CFG_RPM_CFG_MPU1132_2_M19L12_AHB_BASE_XPU_CFG_RPM_CFG_MPU1132_2_M19L12_AHB" : "HAL_XPU2_RPM_CFG",
  "FEERO_SMMU_500_MPU_WRAPPER_SMMU_SS_TCU_MPU" : "HAL_XPU2_SMMU_500_TCU_MPU",
  "FEERO_SMMU_500_MPU_WRAPPER_SMMU_SS_MCDMA_MPU" : "HAL_XPU2_SMMU_500_DIME_MPU",
  "FEERO_SMMU_500_MPU_WRAPPER_SMMU_SS_GPS_MPU" : "HAL_XPU2_SMMU_500_GPS_MPU",
  "FEERO_SMMU_500_MPU_WRAPPER_SMMU_SS_QDSP_MPU" : "HAL_XPU2_SMMU_500_QDSP_MPU",
  "FEERO_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "FEERO_BOOT_ROM_BOOT_ROM_MPU1032_3_M20L12_AHB" : "HAL_XPU2_BOOT_ROM",
  "FEERO_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "FEERO_TLMM_TLMM_MPU1132_16_M22L12_AHB" : "HAL_XPU2_TLMM",
  "FEERO_PMIC_ARB_PMIC_ARB_MPU1132_25_M25L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "FEERO_LPASS_LPASS_TCSR_XPU" : "HAL_XPU2_LPASS_TOP",
  "FEERO_CLK_CTL_GCC_RPU_RPU1132_32_L12" : "HAL_XPU2_CLK_CTL",
}

def get_read_vmid_string(xpu, RG):
  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)

  vmid_tag = get_vmid_tag_string(xpu, "READ")
  read_vmid_string = RG.attrib.get(vmid_tag)
  return read_vmid_string

def get_write_vmid_string(xpu, RG):
  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)

  vmid_tag = get_vmid_tag_string(xpu, "WRITE")
  write_vmid_string = RG.attrib.get(vmid_tag)
  return write_vmid_string

def get_vmid_tag_string(xpu, read_write_type):

  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)

  if(xpu_type == "SingleVmid") and (permission_type == "FULLACCESS_NOACCESS"):
    if(read_write_type == "READ"):
      read_vmid_tag_string = "rwvmid"
    elif(read_write_type == "WRITE"):
      write_vmid_tag_string = "rwvmid"
  elif(xpu_type == "SingleVmid") and (permission_type != "FULLACCESS_NOACCESS"):
    if(read_write_type == "READ"):
      read_vmid_tag_string = "rvmid"
    elif(read_write_type == "WRITE"):
      write_vmid_tag_string = "wvmid"
  elif(xpu_type != "SingleVmid") and (permission_type == "FULLACCESS_NOACCESS"):
    if(read_write_type == "READ"):
      read_vmid_tag_string = "rwvmids"
    elif(read_write_type == "WRITE"):
      write_vmid_tag_string = "rwvmids"
  elif(xpu_type != "SingleVmid") and (permission_type != "FULLACCESS_NOACCESS"):
    if(read_write_type == "READ"):
      read_vmid_tag_string = "rvmids"
    elif(read_write_type == "WRITE"):
      write_vmid_tag_string = "wvmids"

  if(read_write_type == "READ"):
    return read_vmid_tag_string
  elif(read_write_type == "WRITE"):
    return write_vmid_tag_string
  
  
def dissect_RG(xpu, RG):
  global SECURE_RESOURCE
  global MSA_RESOURCE
  global START_ADDRESS
  global END_ADDRESS
  global READ_VMID_STRING
  global WRITE_VMID_STRING
  global GLOBAL_READ
  global GLOBAL_WRITE
  global ROE 
  global RWE
  global XPU_GLOBAL_MSA
  global VMIDCLROE
  global VMIDCLRWE
  global MSACLROE
  global MSACLRWE
  global SCLROE
  global SCLRWE
  
  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)

  if(xpu_type == "SingleVmid"):
    single_vmid = True
  else:
    single_vmid = False
  
  SECURE_RESOURCE = False
  MSA_RESOURCE = False
  START_ADDRESS = "0"
  END_ADDRESS = "0"
  READ_VMID_LIST = []
  READ_VMID_STRING = ""
  WRITE_VMID_STRING = ""
  WRITE_VMID_LIST = []
  GLOBAL_READ = False
  GLOBAL_WRITE = False
  ROE = False
  RWE = False
  VMIDCLROE = False
  VMIDCLRWE = False
  MSACLROE = False
  MSACLRWE = False
  SCLROE = False
  SCLRWE = False
  
  if(RG.attrib.get('sec') == "true"):
    SECURE_RESOURCE = True
    rg_scr_flags = RG.getiterator('XPU_RGn_SCR')
    for rg_scr_flag in rg_scr_flags:
      RGSCR_MSACLROE = rg_scr_flag.attrib.get("MSACLROE")
      if(RGSCR_MSACLROE):
        MSACLROE= True
      RGSCR_MSACLRWE = rg_scr_flag.attrib.get("MSACLRWE")
      if(RGSCR_MSACLRWE):
        MSACLRWE= True
      RGSCR_VMIDCLROE = rg_scr_flag.attrib.get("VMIDCLROE")
      if(RGSCR_VMIDCLROE):
        VMIDCLROE= True
      RGSCR_VMIDCLRWE = rg_scr_flag.attrib.get("VMIDCLRWE")
      if(RGSCR_VMIDCLRWE):
        VMIDCLRWE= True
      break # we want only the first one
  if(RG.attrib.get('msa') == "true"):
    MSA_RESOURCE = True
    XPU_GLOBAL_MSA = True
    rg_mcr_flags = RG.getiterator('XPU_RGn_MCR')
    for rg_mcr_flag in rg_mcr_flags:
      RGMCR_SCLROE = rg_mcr_flag.attrib.get("SCLROE")
      if(RGMCR_SCLROE):
        SCLROE= True
      RGMCR_SCLRWE = rg_mcr_flag.attrib.get("SCLRWE")
      if(RGMCR_SCLRWE):
        SCLRWE= True
      RGMCR_VMIDCLROE = rg_mcr_flag.attrib.get("VMIDCLROE")
      if(RGMCR_VMIDCLROE):
        VMIDCLROE= True
      RGMCR_VMIDCLRWE = rg_mcr_flag.attrib.get("VMIDCLRWE")
      if(RGMCR_VMIDCLRWE):
        VMIDCLRWE= True
      break # we want only the first one
  if(RG.tag == "PRTn"):
    START_ADDRESS = RG.attrib.get('start')
    END_ADDRESS = RG.attrib.get('end')
    
  read_vmid = get_read_vmid_string(xpu,RG)
  READ_VMID_LIST = get_vmids(xpu, read_vmid)
  
  write_vmid = get_write_vmid_string(xpu, RG)
  WRITE_VMID_LIST = get_vmids(xpu, write_vmid)

    
  if ('ALL' in READ_VMID_LIST) or (RG.attrib.get('roge') == "true"):
    GLOBAL_READ = True
  if ('ALL' in WRITE_VMID_LIST) or (RG.attrib.get('rwge') == "true"):
    GLOBAL_WRITE = True
  
  if(xpu_type == "SingleVmid"):
    if 'ALL' in READ_VMID_LIST:
      READ_VMID_LIST = []
    if 'ALL' in WRITE_VMID_LIST:
      WRITE_VMID_LIST = []
  
  READ_VMID_STRING = get_vmid_string(READ_VMID_LIST, single_vmid)
  WRITE_VMID_STRING = get_vmid_string(WRITE_VMID_LIST, single_vmid)
      

  if(get_xpu_permission_type(xpu) == "FULLACCESS_NOACCESS"):
    if (READ_VMID_STRING != WRITE_VMID_STRING):
      if(xpu_type == "SingleVmid"):
        if (GLOBAL_READ == True):
          READ_VMID_STRING = WRITE_VMID_STRING
        elif (GLOBAL_WRITE == True):
          WRITE_VMID_STRING = READ_VMID_STRING
        else:
          READ_VMID_STRING = "***ERROR, this is a Full access/ No access type xPU, but R/W VMIDs are specified different for Non Secure/ Non MSA partition"
          WRITE_VMID_STRING = "***ERROR, this is a Full access/ No access type xPU, but R/W VMIDs are specified different for Non Secure/ Non MSA partition"
      else:
        READ_VMID_STRING = "***ERROR, this is a Full access/ No access type xPU, but R/W VMIDs are specified different for Non Secure/ Non MSA partition"
        WRITE_VMID_STRING = "***ERROR, this is a Full access/ No access type xPU, but R/W VMIDs are specified different for Non Secure/ Non MSA partition"
        
  if(xpu_type == "SingleVmid"):
    #print "************************" + str(WRITE_VMID_LIST)
    if (not GLOBAL_WRITE) and (not "NO_ACCESS" in WRITE_VMID_LIST) and ((WRITE_VMID_LIST != []) and (WRITE_VMID_LIST[0] != '')):
      RWE = True
    if(get_xpu_permission_type(xpu) == "READ_WRITE"):
      if (not GLOBAL_READ) and (not "NO_ACCESS" in READ_VMID_LIST) and ((READ_VMID_LIST != []) and (READ_VMID_LIST[0] != '')):
        ROE = True
    if(GLOBAL_WRITE == True):
      GLOBAL_READ = False

def get_vmids(xpu, vmid_string):
  
  if(vmid_string == None):
    return []
  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)

  vmid_list  = vmid_string.split(",")
  vmid_string1 = ""
  num_vmids = len(vmid_list)
  
  if(xpu_type == "SingleVmid"): 
    if (num_vmids > 2):
      config_file_handle.write("***Error, this is a Single VMID resource, but more than 2 VMIDs are specified")
      vmid_list = []
    elif (num_vmids == 2):
      if(SECURE_RESOURCE == True) and ('MSA=1' in vmid_list):
        do_nothing = 1
      elif(MSA_RESOURCE == True) and ('NS=0' in vmid_list):
        do_nothing = 1
      else:
        config_file_handle.write("***Error, this is a Single VMID resource, but more than 1 VMIDs are specified")
    elif(num_vmids == 1):
      #do nothing
      do_nothing = 1
    elif (num_vmids == 0):
      vmid_list = []
  else:
    if (num_vmids == 0):
      vmid_list = []
  return vmid_list


def get_vmid_string(vmid_list, single_vmid):
  vmidlist_touse = []
  if(single_vmid == True):
    vmidlist_touse = vmids_single
  else:
    vmidlist_touse = vmids

  num_vmids = len(vmid_list)
  vmid_string1 = ""
  if( num_vmids != 0):
    for vmid in vmid_list:
      if(vmid_string1 is ''):
        if(vmid != ''):
          vmid_string1 += vmidlist_touse[vmid]
      else:
        if(vmid != ''):
          vmid_string1 += " | "
          vmid_string1 += vmidlist_touse[vmid]
  if(vmid_string1 is ''):
    vmid_string1 += vmidlist_touse['NO_ACCESS'];
  return vmid_string1
  
  
def getIndex(name):
  RGIndices = root.getiterator('RGIndex')
  for RGIndex in RGIndices:
    if(RGIndex.attrib.get('name') == name):
      return RGIndex.attrib.get('index')
      
      
def process_xpu_rg(xpu, RG, tag):
  xpu_type = get_xpu_multivmid_type(xpu)
  permission_type = get_xpu_permission_type(xpu)
  
  rg_flags = RG.getiterator('XPU_RGn_FLAGS')
  for rg_flag in rg_flags:
    RGtag = rg_flag.attrib.get("hint1")
    break # we want only the first one
  if(RGtag != tag):
    return
    
  
  RGindex = str(RG.attrib.get('index'))
  
  index = getIndex(RGindex)
  
  rg_static_enabled = True
  if 'true' != RG.find('XPU_RGn_FLAGS').attrib.get('static'):
    rg_static_enabled = False

  text = ""
  if False == rg_static_enabled:
    text += "/*\n"

  text += '{ ' + index + ', '
  dissect_RG(xpu, RG)
  if(SECURE_RESOURCE == True):
    text += 'TZBSP_XPU_SEC'
  elif (MSA_RESOURCE == True):
    text += 'TZBSP_XPU_MODEM_PROT'
  else:
    text += 'TZBSP_XPU_NON_SEC'
  
  if(GLOBAL_WRITE == True) and (xpu_type == "SingleVmid"):
    text += "| TZBSP_RWGE"
  elif (GLOBAL_READ == True) and (xpu_type == "SingleVmid"):
    text += "| TZBSP_ROGE"

  if(ROE == True):
    text += "| TZBSP_ROE"
  if(RWE == True):
    text += "| TZBSP_RWE"

  #if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
  text += "| TZBSP_XPU_ENABLE"

  if(MSACLROE):
    text += "| TZBSP_MSACLROE"
  if(MSACLRWE):
    text += "| TZBSP_MSACLRWE"
  if(VMIDCLROE):
    text += "| TZBSP_VMIDCLROE"
  if(VMIDCLRWE):
    text += "| TZBSP_VMIDCLRWE"
  if(SCLROE):
    text += "| TZBSP_SCLROE"
  if(SCLRWE):
    text += "| TZBSP_SCLRWE"

  
  text += ", \n  "
  text += (READ_VMID_STRING + ", " + WRITE_VMID_STRING  ) 
  
  if (get_xpu_type(xpu) == "MPU"):
    text += (",\n  " + START_ADDRESS + ", " + END_ADDRESS)
  text +=  "},\n"

  if False == rg_static_enabled:
    text += "*/\n"

  config_file_handle.write(text)


def get_xpu_type(xpu):
  parameters = xpu.getiterator("Parameter")
  for parameter in parameters:
    if(parameter.attrib.get("name") == "XPUT"):
      xpu = parameter.attrib.get("value")
      if xpu == "1":
        return "APU"
      elif xpu == "2":
        return "MPU"
      elif xpu == "0":
        return "RPU"

def get_xpu_multivmid_type(xpu):
  parameters = xpu.getiterator("Parameter")
  for parameter in parameters:
    if(parameter.attrib.get("name") == "MV"):
      mv = parameter.attrib.get("value")
      if mv == "1":
        return "MultiVmid"
      elif mv == "0":
        return "SingleVmid"

def get_xpu_permission_type(xpu):
  parameters = xpu.getiterator("Parameter")
  for parameter in parameters:
    if(parameter.attrib.get("name") == "PT"):
      pt = parameter.attrib.get("value")
      if pt == "0":
        return "FULLACCESS_NOACCESS"
      elif pt == "1":
        return "READ_WRITE" 


def find_tags_in_xpus(XPUs):
  RGtag_list = []
  for xpu in XPUs:
    if(get_xpu_type(xpu) == "MPU"):
      RGs = xpu.getiterator('PRTn')
    else:
      RGs = xpu.getiterator('RGn')
    for RG in RGs:
      rg_flags = RG.getiterator('XPU_RGn_FLAGS')
      for rg_flag in rg_flags:
        RGtag = rg_flag.attrib.get("hint1")
        break # we want only the first one
      RGtag_list.append(RGtag) 
  RGtag_set = set(RGtag_list)
  return RGtag_set
  

def find_tags_in_xpu(XPU):
  RGtag_list = []
  if(get_xpu_type(XPU) == "MPU"):
    RGs = XPU.getiterator('PRTn')
  else:
    RGs = XPU.getiterator('RGn')
  for RG in RGs:
    rg_flags = RG.getiterator('XPU_RGn_FLAGS')
    for rg_flag in rg_flags:
      RGtag = rg_flag.attrib.get("hint1")
      break # we want only the first one
    RGtag_list.append(RGtag) 
  RGtag_set = set(RGtag_list)
  return RGtag_set
  

def find_xpu_from_name(XPUs, tag, stringname):
  for xpu in XPUs:
    xpu_name = xpu.attrib.get('name')
    #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tag
    xpu_name = xpu_name.replace('.', '_')
    xpu_array_name = xpu_name + tag + VERSION_SUFFIX
    if(xpu_array_name == stringname):
      return xpu

def get_qrib_en_type(xpu):
  parameters = xpu.getiterator("Parameter")
  for parameter in parameters:
    if(parameter.attrib.get("name") == "QRIB_EN"):
      qrib = parameter.attrib.get("value")
      if qrib == "0":
        return "QRIB_NOT_ENABLED" 
      elif qrib == "1":
        return "QRIB_ENABLED" 

def process_xpus():
  global SECURE_RESOURCE, MSA_RESOURCE
  global START_ADDRESS, END_ADDRESS
  global READ_VMID_STRING, WRITE_VMID_STRING
  global GLOBAL_READ, GLOBAL_WRITE
  global ROE, RWE
  global XPU_GLOBAL_MSA

  #find all the tags in the whole xml
  tags = find_tags_in_xpus(XPUs)
  #create a list per tag
  xpu_list = dict()
  xpu_msa = dict()
  for tag in tags: 
    xpu_list[tag] = []

  for tag in tags: 
    xpu_msa[tag] = []

  if(DEFAULT_COMMENTED== True):
    comment = "// "
  else:
    comment = ""

  for xpu in XPUs:
    xpu_name = str(xpu.attrib.get('name'))
    xpu_tag_list = find_tags_in_xpu(xpu)
    for tag in xpu_tag_list:
      if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
        tagname = DEFAULT_TAGNAME
      else:
        tagname = "__" + tag
        #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tagname
      xpu_name = xpu_name.replace('.', '_')
      xpu_array_name = xpu_name + tagname + VERSION_SUFFIX
      enable_file_handle.write(comment + "#define " +  "__XPU__" + xpu_array_name + "__ENABLE__\n")
  enable_file_handle.write("\n\n")

  for xpu in XPUs:
    xpu_name = str(xpu.attrib.get('name'))
    config_file_handle.write('\n/* ' + xpu_name + ' */\n')
    if(get_xpu_permission_type(xpu) == "READ_WRITE"):
      text = "Read/Write access type. "
    else: 
      text = "Full Access/No Access type. "
    if(get_xpu_multivmid_type(xpu) == "SingleVmid"):
      text += "Single VMID type. "
    else: 
      text += "Multi VMID type. "
    config_file_handle.write('/* ' + text + ' */\n\n')

    xpu_tag_list = find_tags_in_xpu(xpu)
    for tag in xpu_tag_list:
      if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
        tagname = DEFAULT_TAGNAME
      else:
        tagname = "__" + tag
      if(get_xpu_type(xpu) == "MPU"):
        RGs = xpu.getiterator('PRTn')
        #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tagname
        xpu_name = xpu_name.replace('.', '_')
        xpu_array_name = xpu_name + tagname + VERSION_SUFFIX
        #print "#ifdef " + "__XPU__"+ xpu_name +"__ENABLE__" 
        config_file_handle.write("static const tzbsp_mpu_rg_t " + xpu_array_name + "[] = {\n")
      else:
        RGs = xpu.getiterator('RGn')
        #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tagname
        xpu_name = xpu_name.replace('.', '_')
        xpu_array_name = xpu_name + tagname + VERSION_SUFFIX
        #print "#ifdef " + "__XPU__"+ xpu_name +"__ENABLE__"
        config_file_handle.write("static const tzbsp_rpu_rg_t " + xpu_array_name + "[] = {\n")
  
      xpu_list[tag].append(xpu_array_name)

      for RG in RGs:
        process_xpu_rg(xpu,RG,tag)
      xpu_msa[tag].append(XPU_GLOBAL_MSA)
      XPU_GLOBAL_MSA = False
              
      config_file_handle.write("};\n\n")
      config_file_handle.write("\n") #endif

  for tag in tags:
  
    if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
      tagname = DEFAULT_TAGNAME
    else:
      tagname = "__" + tag
    config_file_handle.write("\n/*******************************************************/\n")
    config_file_handle.write("/* " + tagname  + " xPUs global array */\n")
    config_file_handle.write("/*******************************************************/\n")
    config_file_handle.write("const tzbsp_xpu_cfg_t g_tzbsp_xpu_cfg" + tagname + VERSION_SUFFIX +"[] = {\n")
  
    for xpu_name in xpu_list[tag]:
      xpu =  find_xpu_from_name(XPUs, tagname, xpu_name)
      umr_read_vmid = "TZBSP_VMID_NOACCESS"
      umr_write_vmid = "TZBSP_VMID_NOACCESS"
      suvmid = "TZBSP_VMID_NOACCESS"

      #get physical address 
      physical_address = xpu.attrib.get('xpuPhysicalAddress')

      #get SU now
      xpu_rpu_acr = xpu.getiterator('XPU_RPU_ACR0')
      for su in xpu_rpu_acr:
        suvmid = su.attrib.get("RWE")
        break #only one SU so break
      suvmid = get_vmid_string(get_vmids(xpu, str(suvmid)), False)

      #get unmapped information now
      if(get_xpu_type(xpu) == "MPU"):
        xpu_umr_acr = xpu.getiterator('XPU_UMR_ACR')
        for umr in xpu_umr_acr:
          umr_read_vmid = umr.attrib.get("rvmids")
          umr_write_vmid = umr.attrib.get("wvmids")
          break # only one umr is there so break
        umr_read_vmid = get_vmid_string(get_vmids(xpu, str(umr_read_vmid)), False)
        umr_write_vmid = get_vmid_string(get_vmids(xpu, str(umr_write_vmid)), False)
      # find config hint
      xpu_flags = xpu.getiterator('XPU_FLAGS')
      for xpu_flag in xpu_flags:
        config_hint = xpu_flag.attrib.get("hint")
        break #only one SU so break
      if config_hint == "static":
        enable_text = "TZBSP_XPU_ENABLE"
      else:
        enable_text = ""
    
      if("true" == xpu.find('SWConfig').find('XPU_MCR').attrib.get('XPUMSAE')):
        msa_text = "TZBSP_XPU_MODEM_PROT"
      else:
        msa_text = ""

      if("true" == xpu.find('SWConfig').find('XPU_CR').attrib.get('MSAE')):
        if msa_text == "":
          msa_text = "TZBSP_XPU_CR_MSAE"
        else:
          msa_text += " | TZBSP_XPU_CR_MSAE"

      if enable_text == "" and msa_text =="":
        flag_text = "0"
      elif msa_text == "":
        flag_text = enable_text
      elif enable_text == "":
        flag_text = msa_text
      else:
        flag_text = enable_text + " | " + msa_text

      XPU_UMR_CNTL = xpu.find('SWConfig').find('XPU_UMR_CNTL') 
      if XPU_UMR_CNTL is not None:
        if "true" == XPU_UMR_CNTL.attrib.get('MSACLROE'):
          flag_text = flag_text + " | TZBSP_XPU_UMR_MSACLROE"
        if "true" == XPU_UMR_CNTL.attrib.get('MSACLRWE'):
          flag_text = flag_text + " | TZBSP_XPU_UMR_MSACLRWE"

      config_file_handle.write("#ifdef " + "__XPU__"+ xpu_name +"__ENABLE__\n")
      config_file_handle.write("/* " + xpu_name + " */\n")
      if VERSION_SUFFIX == "":
        xpu_name_enum = xpu_name_to_enum[xpu_name]
      else:
        xpu_name_enum = xpu_name_to_enum[xpu_name[:(-1 * len(VERSION_SUFFIX))]]
      config_file_handle.write('{ ' + physical_address + ", " + xpu_name_enum + ", " + flag_text +', \n  ' + str(umr_read_vmid) + ", " + str(umr_write_vmid) + ", " + str(suvmid) + ", \n  " + "ARR_SIZE(" + xpu_name + "), " + xpu_name + " },\n")
      config_file_handle.write("#endif\n\n")

    config_file_handle.write("};\n\n")

    config_file_handle.write("const uint32 g_tzbsp_xpu_cfg_count" + tagname + VERSION_SUFFIX + "=\n")
    config_file_handle.write("    sizeof(g_tzbsp_xpu_cfg" + tagname + VERSION_SUFFIX + ") / sizeof(g_tzbsp_xpu_cfg" + tagname + VERSION_SUFFIX + "[0]);\n\n\n")


def process_qrib():
  config_file_handle.write("const tzbsp_qrib_cfg_t g_tzbsp_qrib_cfg[] = {\n")
  for xpu in XPUs:
    xpu_name = str(xpu.attrib.get('name'))
    xpu_tag_list = find_tags_in_xpu(xpu)
    for tag in xpu_tag_list:
      if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
        tagname = DEFAULT_TAGNAME
      else:
        tagname = "__" + tag
      xpu_name = xpu_name.replace('.', '_')
      xpu_array_name = xpu_name + tagname + VERSION_SUFFIX
      if(get_qrib_en_type(xpu)== "QRIB_ENABLED"):
        config_file_handle.write("#ifdef " +  "__XPU__"+ xpu_array_name +"__ENABLE__\n")
        qrib_entry = "{ 0xDEADBEEF, /* QRIB_XPU2_INIT_ADDR */ 0xDEADBEEF, /* QRIB_XPU2_ACR_ADDR */ 0xDEADBEEF, /* QRIB_XPU2_VMID_EN_ADDR */\n 0x1, /* NSEN_INIT */\n 0x1, /* EN_TZ */\n TZBSP_VMID_TZ_BIT, /* XPU_ACR_VMID */\n 0x1, /* VMIDEN_INIT */\n 0x1, /* VMIDEN_INIT_EN_HV */ }," 
        config_file_handle.write(qrib_entry + "\n")
        config_file_handle.write("#endif\n")
  config_file_handle.write("};\n\n\n")
  config_file_handle.write("const uint32 g_tzbsp_qrib_cfg_count =\n")
  config_file_handle.write("    sizeof(g_tzbsp_qrib_cfg) / sizeof(g_tzbsp_qrib_cfg[0]);\n\n\n")


def main(arglist):
  global root, XPUs, enable_file_handle, config_file_handle, VERSION_SUFFIX 

  if (len(arglist) == 0 or arglist[0][0] != "-"):
    print "Invalid argument(s). Use -h or --help for more info"
    sys.exit(1)

  parser = OptionParser()
  parser.add_option("-x", "--xml", dest="xmlfile",
                    help="Access control XML file", metavar="XMLFILE")
  parser.add_option("-e", "--enable_file",
                    dest="enable_file",
                    help="header file containing #defines for each XPU",
                    metavar="ENABLEHEADER")
  parser.add_option("-c", "--config_file",
                    dest="config_file",
                    help="config file containing XPU static config",
                    metavar="CONFIGFILE",
                    default="")
  parser.add_option("-s", "--version_suffix",
                    dest="version_suffix",
                    help="Version suffix",
                    metavar="VERSION",
                    default="")
  parser.add_option("-r", "--header",
                    dest="header_file",
                    help="Header file",
                    metavar="HEADER",
                    default="")

  (options, args) = parser.parse_args(arglist)

  tree = ET.parse(options.xmlfile)
  config_file_handle = open(options.config_file, 'w')
  enable_file_handle = open(options.enable_file, 'w')
  VERSION_SUFFIX = options.version_suffix

  enable_file_name = os.path.basename(options.enable_file)
  enable_file_define = "__%s__" % enable_file_name.replace(".", "_").upper()
  enable_file_handle.write("#ifndef %s\n\n" % enable_file_define)

  if len(options.header_file) > 0:
    header_file_handle = open(options.header_file, 'r')
    config_file_handle.write(header_file_handle.read())
    header_file_handle.close()

  config_file_handle.write("#include \"%s\"\n" % enable_file_name)

  root = tree.getroot()
  XPUs = []
  XPUs += root.findall('APU')
  XPUs += root.findall('MPU')
  XPUs += root.findall('RPU')

  process_xpus()

  enable_file_handle.write("#endif\n")
  enable_file_handle.close()
  config_file_handle.close()


if __name__ == "__main__":
  main(sys.argv[1:])

