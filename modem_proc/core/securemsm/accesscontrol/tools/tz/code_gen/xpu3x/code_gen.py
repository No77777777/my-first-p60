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
generate_for_modem = False

vmids = {
  'ALL':'TZBSP_ALL_VMID',
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
  'ZAP_SHADER': 'TZBSP_VMID_ZAP_SHADER_BIT',
  'ALL_HYP': 'TZBSP_ALL_VMID',
  'IPA_WIFI': 'TZBSP_VMID_IPA_WIFI_BIT',
}


domains = {
  'ALL_ROT':'TZBSP_DOMAIN_TZ_BIT | TZBSP_DOMAIN_MSA_BIT | TZBSP_DOMAIN_SP_BIT | TZBSP_DOMAIN_HYP_BIT',
  'NONE': 'TZBSP_DOMAIN_NONE',
  'TZ': 'TZBSP_DOMAIN_TZ_BIT',
  'MSA': 'TZBSP_DOMAIN_MSA_BIT',
  'SP': 'TZBSP_DOMAIN_SP_BIT',
  'HYP':'TZBSP_DOMAIN_HYP_BIT',
}

TZ_RESOURCE = False
MSA_RESOURCE = False
SP_RESOURCE = False
HYP_RESOURCE = False
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
PROFILE = ""
READ_DOMAIN_STRING = ""
WRITE_DOMAIN_STRING = ""
UMR_TEXT = ""
INPUT_ADDR_MASK = ""
generate_for_modem = True

xpu_name_to_enum = {

  #     NAZGUL XPU

  "NAZGUL_RAMBLUR_PIMEM_RAMBLUR_PIMEM_APU_APU32Q2N7S1V1_2_CL36" : "HAL_XPU2_RAMBLUR_PIMEM_APU",
  "NAZGUL_OCIMEM_WRAPPER_CSR_APU" : "HAL_XPU2_IMEM_APU",
  "NAZGUL_SPDM_WRAPPER_TOP_SPDM_APU32Q2N7S1V1_1_CL36" : "HAL_XPU2_SPDM_APU",
  "NAZGUL_SECURITY_CONTROL_APU32Q2N7S1V0_25_CL36" : "HAL_XPU2_SEC_CTRL_APU",
  "NAZGUL_DDR_SS_BIMC_BIMC_CONFIG_APU" : "HAL_XPU2_BIMC_APU",
  "NAZGUL_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "NAZGUL_DCC_WRAPPER_DCC_XPU" : "HAL_XPU2_DCC",
  "NAZGUL_UFS_UFS_REGS_ICE_XPU3" : "HAL_XPU2_UFS_ICE",
  "NAZGUL_CRYPTO0_CRYPTO_TOP_XPU3_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "NAZGUL_MMSS_MDSS_XPU" : "HAL_XPU2_MMSS",
  "NAZGUL_TLMM_XW" : " HAL_XPU2_TLMM_XPU_WEST",
  "NAZGUL_SSC_SSC_BLSP_BLSP_XPU3_XPU3" : "HAL_XPU2_SSC_BLSP_BAM",
  "NAZGUL_PERIPH_SS_BLSP1_BLSP_XPU3_XPU3" : "HAL_XPU2_BAM_BLSP1_DMA",
  "NAZGUL_PERIPH_SS_BLSP2_BLSP_XPU3_XPU3" : "HAL_XPU2_BAM_BLSP2_DMA",
  "NAZGUL_CONFIG_NOC_WRAPPER_CNOC_A2_NOC_MPU_CFG" : "HAL_XPU2_CNOC_A2NOC_MPU",
  "NAZGUL_CONFIG_NOC_WRAPPER_CNOC_QDSS_MPU_CFG" : "HAL_XPU2_CNOC_QDSS_MPU",
  "NAZGUL_SSC_MPU_CFG_SSC_MPU_WRAPPER_SSC_MPU_CFG_MPU32Q2N7S1V0_10_CL36M22L11_AHB" : "HAL_XPU2_CFG_SSC",
  "NAZGUL_RAMBLUR_PIMEM_RAMBLUR_PIMEM_MPU_MPU32Q2N7S1V1_8_CL36M23L10_AXI" : "HAL_XPU2_RAMBLUR_PIMEM_MPU",
  "NAZGUL_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "NAZGUL_BOOT_ROM_BOOT_ROM_MPU32Q2N7S1V0_4_CL36M17L10_AHB" : "HAL_XPU2_BOOT_ROM",
  "NAZGUL_RPM_MPU" : "HAL_XPU2_RPM_MSTR_MPU",
  "NAZGUL_DDR_SS_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "NAZGUL_DDR_SS_BIMC_BIMC_S_DDR1" : "HAL_XPU2_BIMC_MPU1",
  "NAZGUL_MMSS_U_XPU3_AHB_WRAPPER_MNOC" : "HAL_XPU2_MNOC",
  "NAZGUL_SSC_SSC_Q6_MPU_MPU32Q2N7S1V0_12_CL36M22L11_AHB" : "HAL_XPU2_SSC_Q6_MPU",
  "NAZGUL_PMIC_ARB_PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "NAZGUL_LPASS_LPASS_Q6SS_MPU" : "HAL_XPU2_LPASS_Q6SS_MPU",
  "NAZGUL_CLK_CTL_GCC_RPU_RPU32Q2N7S1V0_176_CL36L12" : "HAL_XPU2_GCC_RPU",
  "NAZGUL_CORE_TOP_CSR_TCSR_MUTEX_RPU32Q2N7S1V0_64_CL36L12" : "HAL_XPU2_TCSR_MUTEX",
  "NAZGUL_SKL_WRAPPER_XPU3" : "HAL_XPU2_SKL",
  "NAZGUL_IPA_0_IPA_WRAPPER_BASE_BAM_NDP_XPU3": "HAL_XPU2_IPA",
  "ISTARI_PERIPH_SS_PERIPH_SS_SDC1_SDCC5_TOP_ICE_ICE_XPU2" : "HAL_XPU2_SDC1_SDCC_ICE",
  "NAZGUL_MSS_MPU_MSS_MPU_WRAPPER_MSS_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_MPU",
  "NAZGUL_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI_BASE_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_Q6_MPU",
  "NAZGUL_CONFIG_NOC_WRAPPER_CNOC_A1_NOC_MPU_CFG": "HAL_XPU2_CNOC_A1NOC_MPU",
  "NAZGUL_A2_NOC_AGGRE2_NOC_WRAPPER_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_ANOC2_MPU",
  "NAZGUL_CORE_TOP_CSR_TCSR_REGS_RPU32Q2N7S1V0_40_CL36L12" : "HAL_XPU2_TCSR_REGS",
  "NAZGUL_DDR_SS_MCCC_XPU_APU32Q2N7S1V0_2_CL36": "HAL_XPU2_MCCC_APU",
  "NAZGUL_SP_SCSR_XPU3" : "HAL_XPU2_SP_SCSR",
  "NAZGUL_TLMM_XE" : "HAL_XPU2_TLMM_XPU_EAST",
  "NAZGUL_TLMM_XN" : "HAL_XPU2_TLMM_XPU_NORTH",
  "NAZGUL_IPA_0_IPA_WRAPPER_BASE_GSI_TOP_XPU3" : "HAL_XPU2_IPA_0_GSI_TOP",
  "NAZGUL_MMSS_U_XPU3_MMCC_WRAPPER" : "HAL_XPU2_MMSS_CC",
  "NAZGUL_CONFIG_NOC_WRAPPER_CNOC_MS_NOC_MPU_CFG": "HAL_XPU2_CNOC_SNOC_MS_MPU",
  "NAZGUL_QM_MPU_CFG_QM_MPU_WRAPPER_QM_MPU_CFG_MPU32Q2N7S1V0_4_CL36M23L12_AHB" : "HAL_XPU2_QM_MPU_CFG",
  "NAZGUL_A53SS_APCS_XPU3" : "HAL_XPU2_APCS_MPU",
  "NAZGUL_A1_NOC_AGGRE1_NOC_WRAPPER_DV_MPU32Q2N7S1V0_16_CL36M35L12_AXI" : "HAL_XPU2_ANOC1_MPU",
  "NAZGUL_CONFIG_NOC_WRAPPER_CNOC_SNOC_MPU_CFG" : "HAL_XPU2_CNOC_SNOC_MPU",
  
   #     STARLORD XPU

  "STARLORD_RAMBLUR_PIMEM_RAMBLUR_PIMEM_APU_APU32Q2N7S1V1_2_CL36" : "HAL_XPU2_RAMBLUR_PIMEM_APU",
  "STARLORD_OCIMEM_WRAPPER_CSR_APU" : "HAL_XPU2_IMEM_APU",
  "STARLORD_SPDM_WRAPPER_TOP_SPDM_APU32Q2N7S1V1_1_CL36" : "HAL_XPU2_SPDM_APU",
  "STARLORD_SECURITY_CONTROL_APU32Q2N7S1V0_25_CL36" : "HAL_XPU2_SEC_CTRL_APU",
  "STARLORD_DDR_SS_BIMC_BIMC_CONFIG_APU" : "HAL_XPU2_BIMC_APU",
  "STARLORD_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "STARLORD_DCC_WRAPPER_DCC_XPU" : "HAL_XPU2_DCC",
  "STARLORD_UFS_UFS_REGS_ICE_XPU3" : "HAL_XPU2_UFS_ICE",
  "STARLORD_CRYPTO0_CRYPTO_TOP_XPU3_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "STARLORD_MMSS_MDSS_XPU" : "HAL_XPU2_MMSS",
  "STARLORD_PERIPH_SS_BLSP1_BLSP_XPU3_XPU3" : "HAL_XPU2_BAM_BLSP1_DMA",
  "STARLORD_PERIPH_SS_BLSP2_BLSP_XPU3_XPU3" : "HAL_XPU2_BAM_BLSP2_DMA",
  "STARLORD_CONFIG_NOC_WRAPPER_CNOC_A2_NOC_MPU_CFG" : "HAL_XPU2_CNOC_A2NOC_MPU",
  "STARLORD_CONFIG_NOC_WRAPPER_CNOC_QDSS_MPU_CFG" : "HAL_XPU2_CNOC_QDSS_MPU",
  "STARLORD_RAMBLUR_PIMEM_RAMBLUR_PIMEM_MPU_MPU32Q2N7S1V1_8_CL36M25L10_AXI" : "HAL_XPU2_RAMBLUR_PIMEM_MPU",
  "STARLORD_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "STARLORD_BOOT_ROM_BOOT_ROM_MPU32Q2N7S1V0_4_CL36M17L10_AHB" : "HAL_XPU2_BOOT_ROM",
  "STARLORD_RPM_MPU" : "HAL_XPU2_RPM_MSTR_MPU",
  "STARLORD_DDR_SS_BIMC_BIMC_S_DDR0" : "HAL_XPU2_BIMC_MPU0",
  "STARLORD_DDR_SS_BIMC_BIMC_S_DDR1" : "HAL_XPU2_BIMC_MPU1",
  "STARLORD_MMSS_U_XPU3_AHB_WRAPPER_MNOC" : "HAL_XPU2_MNOC",
  "STARLORD_PMIC_ARB_PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "STARLORD_LPASS_LPASS_Q6SS_MPU" : "HAL_XPU2_LPASS_Q6SS_MPU",
  "STARLORD_CLK_CTL_GCC_RPU_RPU32Q2N7S1V0_176_CL36L12" : "HAL_XPU2_GCC_RPU",
  "STARLORD_CORE_TOP_CSR_TCSR_MUTEX_RPU32Q2N7S1V0_64_CL36L12" : "HAL_XPU2_TCSR_MUTEX",
  "STARLORD_IPA_0_IPA_WRAPPER_BASE_BAM_NDP_XPU3": "HAL_XPU2_IPA",
  "STARLORD_MSS_MPU_MSS_MPU_WRAPPER_MSS_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_MPU",
  "STARLORD_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI_BASE_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_Q6_MPU",
  "STARLORD_CONFIG_NOC_WRAPPER_CNOC_A1_NOC_MPU_CFG": "HAL_XPU2_CNOC_A1NOC_MPU",
  "STARLORD_A2_NOC_AGGRE2_NOC_WRAPPER_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_ANOC2_MPU",
  "STARLORD_CORE_TOP_CSR_TCSR_REGS_RPU32Q2N7S1V0_40_CL36L12" : "HAL_XPU2_TCSR_REGS",
  "STARLORD_DDR_SS_MCCC_XPU_APU32Q2N7S1V0_2_CL36": "HAL_XPU2_MCCC_APU",
  "STARLORD_SP_SCSR_XPU3" : "HAL_XPU2_SP_SCSR",
  "STARLORD_TLMM_XN" : "HAL_XPU2_TLMM_XPU_NORTH",
  "STARLORD_IPA_0_IPA_WRAPPER_BASE_GSI_TOP_XPU3" : "HAL_XPU2_IPA_0_GSI_TOP",
  "STARLORD_MMSS_U_XPU3_MMCC_WRAPPER" : "HAL_XPU2_MMSS_CC",
  "STARLORD_CONFIG_NOC_WRAPPER_CNOC_MS_NOC_MPU_CFG": "HAL_XPU2_CNOC_SNOC_MS_MPU",
  "STARLORD_QM_MPU_CFG_QM_MPU_WRAPPER_QM_MPU_CFG_MPU32Q2N7S1V0_4_CL36M23L12_AHB" : "HAL_XPU2_QM_MPU_CFG",
  "STARLORD_A53SS_APCS_XPU3" : "HAL_XPU2_APCS_MPU",
  "STARLORD_CONFIG_NOC_WRAPPER_CNOC_SNOC_MPU_CFG" : "HAL_XPU2_CNOC_SNOC_MPU",
  "STARLORD_LPASS_LPASS_BLSP_XPU3" : "HAL_XPU2_BAM_BLSP_LPASS",
  "STARLORD_TLMM_XC" : "HAL_XPU2_TLMM_XPU_CENTER",
  "STARLORD_TLMM_XS" : "HAL_XPU2_TLMM_XPU_SOUTH",
  "STARLORD_PERIPH_SS_SDC1_SDCC5_TOP_ICE_XPU3" : "HAL_XPU2_SDC1_SDCC_ICE",
  "STARLORD_TURING_TURING_Q6SS_MPU" : "HAL_XPU2_TURING_Q6",
  "STARLORD_LPASS_CFG_MPU_MPU32Q2N7S1V0_10_CL36M22L12_AHB_BASE_LPASS_CFG_MPU_MPU32Q2N7S1V0_10_CL36M22L12_AHB" : "HAL_XPU2_LPASS_CFG_MPU",
  "STARLORD_MSS_NAV_MPU_MPU32Q2N7S1V1_16_CL36M35L12_AXI_BASE_MSS_NAV_MPU_MPU32Q2N7S1V1_16_CL36M35L12_AXI" : "HAL_XPU2_MSS_NAV_MPU",
  
  
  
    #     NAPALI XPU

  "NAPALI_RAMBLUR_PIMEM_RAMBLUR_PIMEM_APU_APU32Q2N7S1V1_2_CL36" : "HAL_XPU2_RAMBLUR_PIMEM_APU",
  "NAPALI_OCIMEM_WRAPPER_CSR_APU" : "HAL_XPU2_IMEM_APU",
  "NAPALI_SPDM_WRAPPER_TOP_SPDM_APU32Q2N7S1V1_1_CL36" : "HAL_XPU2_SPDM_APU",
  "NAPALI_SECURITY_CONTROL_APU32Q2N7S1V0_25_CL36" : "HAL_XPU2_SEC_CTRL_APU",
  "NAPALI_MPM2_MPM_APU" : "HAL_XPU2_MPM2",
  "NAPALI_QC_DCC_CSAFB9399A_APU" : "HAL_XPU2_DCC",
  "NAPALI_UFS_MEM_UFS_2LANE_REGS_ICE_XPU3" : "HAL_XPU2_UFS_ICE",
  "NAPALI_UFS_CARD_UFS_1LANE_REGS_ICE_XPU3" : "HAL_XPU2_SDC1_SDCC_ICE",
  "NAPALI_CRYPTO0_CRYPTO_TOP_XPU3_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "NAPALI_MDSS_MDSS_XPU" : "HAL_XPU2_MMSS",
  "NAPALI_TLMM_XW" : " HAL_XPU2_TLMM_XPU_WEST",
  "NAPALI_SSC_SSC_QUPV3_QUPV3_ID_0_GSI_TOP_XPU3" : "HAL_XPU2_SSC_BLSP_BAM",
  "NAPALI_QUPV3_0_QUPV3_ID_1_GSI_TOP_XPU3" : "HAL_XPU2_BAM_BLSP1_DMA",
  "NAPALI_QUPV3_1_QUPV3_ID_1_GSI_TOP_XPU3" : "HAL_XPU2_BAM_BLSP2_DMA",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_A2_NOC_MPU_CFG" : "HAL_XPU2_CNOC_A2NOC_MPU",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_QDSS_MPU_CFG" : "HAL_XPU2_CNOC_QDSS_MPU",
  "NAPALI_SSC_MPU_CFG_SSC_MPU_WRAPPER_SSC_MPU_CFG_MPU32Q2N7S1V0_10_CL36M22L11_AHB" : "HAL_XPU2_CFG_SSC",
  "NAPALI_SSC_MPU_CFG_SSC_MPU_WRAPPER_SSC_MPU_CFG_MPU32Q2N7S1V0_10_CL36M35L11_AHB" : "HAL_XPU2_CFG_SSC",
  "NAPALI_RAMBLUR_PIMEM_RAMBLUR_PIMEM_MPU_MPU32Q2N7S1V1_8_CL36M25L10_AXI" : "HAL_XPU2_RAMBLUR_PIMEM_MPU",
  "NAPALI_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "NAPALI_BOOT_ROM_BOOT_ROM_MPU32Q2N7S1V0_4_CL36M17L10_AHB" : "HAL_XPU2_BOOT_ROM",
  "NAPALI_RPM_MPU" : "HAL_XPU2_RPM_MSTR_MPU",
  "NAPALI_DDR_SS_LLCC0_LLCC_MPU" : "HAL_XPU2_BIMC_MPU0",
  "NAPALI_DDR_SS_LLCC1_LLCC_MPU" : "HAL_XPU2_BIMC_MPU1",
  "NAPALI_DDR_SS_LLCC2_LLCC_MPU" : "HAL_XPU2_BIMC_MPU2",
  "NAPALI_DDR_SS_LLCC3_LLCC_MPU" : "HAL_XPU2_BIMC_MPU3",
  "NAPALI_MMSS_U_XPU3_AHB_WRAPPER_MNOC" : "HAL_XPU2_MNOC",
  "NAPALI_SSC_SSC_Q6_MPU_MPU32Q2N7S1V0_12_CL36M35L11_AHB" : "HAL_XPU2_SSC_Q6_MPU",
  "NAPALI_PMIC_ARB_PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "NAPALI_LPASS_LPASS_Q6SS_MPU" : "HAL_XPU2_LPASS_Q6SS_MPU",
  "NAPALI_CLK_CTL_GCC_RPU_RPU32Q2N7S1V0_200_CL36L12" : "HAL_XPU2_GCC_RPU",
  "NAPALI_CORE_TOP_CSR_TCSR_MUTEX_RPU32Q2N7S1V0_64_CL36L12" : "HAL_XPU2_TCSR_MUTEX",
  "NAPALI_SKL_WRAPPER_XPU3" : "HAL_XPU2_SKL",
  "NAPALI_IPA_0_IPA_WRAPPER_BASE_BAM_NDP_XPU3": "HAL_XPU2_IPA",
  "NAPALI_MSS_MPU_MSS_MPU_WRAPPER_MSS_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_MPU",
  "NAPALI_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI_BASE_MSS_Q6_MPU_MPU32Q2N7S1V0_16_CL36M35L12_AXI": "HAL_XPU2_MSS_Q6_MPU",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_A1_NOC_MPU_CFG": "HAL_XPU2_CNOC_A1NOC_MPU",
  "NAPALI_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB_BASE_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB": "HAL_XPU2_ANOC2_MPU",
  "NAPALI_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB_BASE_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB": "HAL_XPU2_ANOC2_MPU",
  "NAPALI_CORE_TOP_CSR_TCSR_REGS_RPU32Q2N7S1V0_48_CL36L12" : "HAL_XPU2_TCSR_REGS",
  "NAPALI_DDR_SS_MCCC_XPU_APU32Q2N7S1V0_2_CL36": "HAL_XPU2_MCCC_APU",
  "NAPALI_SP_SCSR_XPU3" : "HAL_XPU2_SP_SCSR",
  "NAPALI_TLMM_XE" : "HAL_XPU2_TLMM_XPU_EAST",
  "NAPALI_TLMM_XN" : "HAL_XPU2_TLMM_XPU_NORTH",
  "NAPALI_TLMM_XS" : "HAL_XPU2_TLMM_XPU_SOUTH", 
  "NAPALI_IPA_0_IPA_WRAPPER_BASE_GSI_TOP_XPU3" : "HAL_XPU2_IPA_0_GSI_TOP",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_SS_MPU_CFG": "HAL_XPU2_CNOC_SNOC_MPU",
  "NAPALI_QM_MPU_CFG_QM_MPU_WRAPPER_QM_MPU_CFG_MPU32Q2N7S1V0_4_CL36M23L12_AHB" : "HAL_XPU2_QM_MPU_CFG",
  "NAPALI_A1_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB_BASE_A1_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB" : "HAL_XPU2_ANOC1_MPU",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_SNOC_MPU_CFG" : "HAL_XPU2_CNOC_SNOC_MS_MPU",
  "NAPALI_TITAN_SS_TITAN_XPU3" : "HAL_XPU2_TITAN_SS_APU",
  "NAPALI_DDR_SS_MPU32Q2N7S1V1_8_CL36M35L12_AHB" :  "HAL_MEMNOC_MS_MPU",
  "NAPALI_DDR_SS_MPU32Q2N7S1V0_16_CL36M35L12_AHB" :  "HAL_MEMNOC_MS_MPU",
  "NAPALI_CONFIG_NOC_WRAPPER_CNOC_AOSS_MPU_CFG"  :  "HAL_XPU2_CNOC_AOSS_MPU",
  "NAPALI_DDR_SS_LLCC_BROADCAST_LLCC_MPU"  :  "HAL_XPU2_LLCC_BROADCAST_MPU",
  "NAPALI_SSC_SSC_SDC_MPU_MPU32Q2N7S1V0_12_CL36M35L11_AHB"  :  "HAL_XPU2_SSC_SDC_MPU",
  "NAPALI_MSS_NAV_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB_BASE_MSS_NAV_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB"  :  "HAL_XPU2_MSS_NAV_MPU",
  "NAPALI_DDR_SS_DC_NOC_BROADCAST_MPU_MPU32Q2N7S1V1_40_CL36M24L12_AHB"  :  "HAL_DC_NOC_BROADCAST_MPU",
  "NAPALI_DDR_SS_DC_NOC_BROADCAST_MPU_MPU32Q2N7S1V0_40_CL36M24L12_AHB"  :  "HAL_DC_NOC_BROADCAST_MPU",
  "NAPALI_AOSS_AOSS_MPU_MPU32Q2N7S1V1_20_CL36M35L17_AHB"  :  "HAL_AOSS_MPU",
  "NAPALI_AOSS_AOSS_MPU_MPU32Q2N7S1V1_20_CL36M31L12_AHB" :  "HAL_AOSS_MPU",
  "NAPALI_AOSS_PMIC_ARB_PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB"  :  "HAL_XPU2_PMIC_ARB",
  "NAPALI_DDR_SS_DC_NOC_NON_BROADCAST_MPU_MPU32Q2N7S1V1_16_CL36M24L12_AHB"  :  "HAL_DC_NOC_NON_BROADCAST_MPU",
  "NAPALI_DDR_SS_DC_NOC_NON_BROADCAST_MPU_MPU32Q2N7S1V0_16_CL36M24L12_AHB"  :  "HAL_DC_NOC_NON_BROADCAST_MPU",
  "NAPALI_SPDM_WRAPPER_TOP_SPDM_APU32Q2N7S1V0_1_CL36": "HAL_XPU2_SPDM_APU",


  #     WARLOCK XPU
  "NAPALIQ_SSC_MPU_CFG_SSC_MPU_WRAPPER_SSC_MPU_CFG_MPU32Q2N7S1V0_10_CL36M35L11_AHB": "HAL_XPU2_CFG_SSC",
  "NAPALIQ_CLK_CTL_GCC_RPU_RPU32Q2N7S1V0_200_CL36L12" : "HAL_XPU2_GCC_RPU",
  "NAPALIQ_BOOT_ROM_BOOT_ROM_MPU32Q2N7S1V0_4_CL36M17L10_AHB" : "HAL_XPU2_BOOT_ROM",
  "NAPALIQ_RAMBLUR_PIMEM_RAMBLUR_PIMEM_APU_APU32Q2N7S1V1_2_CL36" : "HAL_XPU2_RAMBLUR_PIMEM_APU",
  "NAPALIQ_RAMBLUR_PIMEM_RAMBLUR_PIMEM_MPU_MPU32Q2N7S1V1_8_CL36M25L10_AXI" : "HAL_XPU2_RAMBLUR_PIMEM_MPU",
  "NAPALIQ_OCIMEM_WRAPPER_CSR_APU" : "HAL_XPU2_IMEM_APU",
  "NAPALIQ_OCIMEM_WRAPPER_CSR_MPU" : "HAL_XPU2_IMEM_MPU",
  "NAPALIQ_SPDM_WRAPPER_TOP_SPDM_APU32Q2N7S1V0_1_CL36" : "HAL_XPU2_SPDM_APU",
  "NAPALIQ_SECURITY_CONTROL_APU32Q2N7S1V0_25_CL36" : "HAL_XPU2_SEC_CTRL_APU",
  "NAPALIQ_PERIPH_SS_SDC1_SDCC5_TOP_ICE_XPU3" : "HAL_XPU2_SDC1_SDCC_ICE",
  "NAPALIQ_QUPV3_0_QUPV3_ID_1_GSI_TOP_XPU3" : "HAL_XPU2_BAM_BLSP1_DMA",
  "NAPALIQ_QUPV3_1_QUPV3_ID_1_GSI_TOP_XPU3" : "HAL_XPU2_BAM_BLSP2_DMA",
  "NAPALIQ_QC_DCC_CSAFB9399A_APU" : "HAL_XPU2_DCC",
  "NAPALIQ_QM_MPU_CFG_QM_MPU_WRAPPER_QM_MPU_CFG_MPU32Q2N7S1V0_4_CL36M23L12_AHB" : "HAL_XPU2_QM_MPU_CFG",
  "NAPALIQ_DDR_SS_LLCC0_LLCC_MPU" : "HAL_XPU2_BIMC_MPU0",
  "NAPALIQ_DDR_SS_LLCC1_LLCC_MPU" : "HAL_XPU2_BIMC_MPU1",
  "NAPALIQ_DDR_SS_LLCC_BROADCAST_LLCC_MPU" : "HAL_XPU2_LLCC_BROADCAST_MPU",
  "NAPALIQ_DDR_SS_MPU32Q2N7S1V0_16_CL36M35L12_AHB" : "HAL_MEMNOC_MS_MPU",
  "NAPALIQ_DDR_SS_DC_NOC_NON_BROADCAST_MPU_MPU32Q2N7S1V0_16_CL36M24L12_AHB" : "HAL_DC_NOC_NON_BROADCAST_MPU",
  "NAPALIQ_DDR_SS_DC_NOC_BROADCAST_MPU_MPU32Q2N7S1V0_40_CL36M24L12_AHB" : "HAL_DC_NOC_BROADCAST_MPU",
  "NAPALIQ_CONFIG_NOC_WRAPPER_CNOC_SNOC_MPU_CFG" : "HAL_XPU2_CNOC_SNOC_MS_MPU",
  "NAPALIQ_CONFIG_NOC_WRAPPER_CNOC_SS_MPU_CFG" : "HAL_XPU2_CNOC_SNOC_MPU",
  "NAPALIQ_CONFIG_NOC_WRAPPER_CNOC_AOSS_MPU_CFG" : "HAL_XPU2_CNOC_AOSS_MPU",
  "NAPALIQ_UFS_MEM_UFS_1LANE_REGS_ICE_XPU3" : "HAL_XPU2_UFS_ICE",
  "NAPALIQ_CRYPTO0_CRYPTO_TOP_XPU3_BAM" : "HAL_XPU2_CRYPTO0_BAM",
  "NAPALIQ_IPA_0_IPA_WRAPPER_BASE_GSI_TOP_XPU3" : "HAL_XPU2_IPA_0_GSI_TOP",
  "NAPALIQ_CORE_TOP_CSR_TCSR_MUTEX_RPU32Q2N7S1V0_64_CL36L12" : "HAL_XPU2_TCSR_MUTEX",
  "NAPALIQ_CORE_TOP_CSR_TCSR_REGS_RPU32Q2N7S1V0_48_CL36L12" : "HAL_XPU2_TCSR_REGS",
  "NAPALIQ_TLMM_XW" : "HAL_XPU2_TLMM_XPU_WEST",
  "NAPALIQ_TLMM_XN" : "HAL_XPU2_TLMM_XPU_NORTH",
  "NAPALIQ_TLMM_XS" : "HAL_XPU2_TLMM_XPU_SOUTH",
  "NAPALIQ_TITAN_SS_TITAN_XPU3" : "HAL_XPU2_TITAN_SS_APU",
  "NAPALIQ_MDSS_MDSS_XPU" : "HAL_XPU2_MMSS",
  "NAPALIQ_AOSS_AOSS_MPU_MPU32Q2N7S1V1_20_CL36M31L12_AHB" : "HAL_AOSS_MPU",
  "NAPALIQ_AOSS_PMIC_ARB_PMIC_ARB_MPU32Q2N7S1V1_25_CL36M27L12_AHB" : "HAL_XPU2_PMIC_ARB",
  "NAPALIQ_MSS_NAV_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB_BASE_MSS_NAV_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB" : "HAL_XPU2_MSS_NAV_MPU",
  "NAPALIQ_A1_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB_BASE_A1_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_8_CL36M35L12_AHB" : "HAL_XPU2_ANOC1_MPU",
  "NAPALIQ_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB_BASE_A2_NOC_MS_MPU_CFG_MPU32Q2N7S1V0_16_CL36M35L12_AHB" : "HAL_XPU2_ANOC2_MPU",
  "NAPALIQ_LPASS_SSC_QUPV3_QUPV3_ID_0_GSI_TOP_XPU3" : "HAL_XPU2_LPASS_SSC_BLSP_BAM",
  "NAPALIQ_LPASS_SSC_Q6_MPU_MPU32Q2N7S1V0_12_CL36M35L11_AHB" : "HAL_XPU2_LPASS_SSC_Q6_MPU",
  "NAPALIQ_LPASS_SSC_SDC_MPU_MPU32Q2N7S1V0_12_CL36M35L11_AHB" : "HAL_XPU2_LPASS_SSC_SDC_MPU",
}

def get_read_vmid_string(xpu, RG):
  read_vmid_string = RG.attrib.get("rvmids")
  return read_vmid_string

def get_write_vmid_string(xpu, RG):
  write_vmid_string = RG.attrib.get("wvmids")
  return write_vmid_string

  
def dissect_RG(xpu, RG):
  global TZ_RESOURCE
  global MSA_RESOURCE
  global SP_RESOURCE
  global HYP_RESOURCE
  global START_ADDRESS
  global END_ADDRESS
  global READ_VMID_STRING
  global WRITE_VMID_STRING
  global READ_DOMAIN_STRING
  global WRITE_DOMAIN_STRING

  
  TZ_RESOURCE = False
  MSA_RESOURCE = False
  SP_RESOURCE = False
  HYP_RESOURCE = False
  
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
  READ_DOMAIN_STRING = ""
  WRITE_DOMAIN_STRING = ""
  
  if(RG.attrib.get('owner') == "TZ"):  
    TZ_RESOURCE = True
  elif(RG.attrib.get('owner') == "MSA"):  
    MSA_RESOURCE = True
  elif(RG.attrib.get('owner') == "SP"):  
    SP_RESOURCE = True
  elif(RG.attrib.get('owner') == "HYP"):  
    HYP_RESOURCE = True

  if(RG.tag == "PRTn"):
    START_ADDRESS = RG.attrib.get('start')
    END_ADDRESS = RG.attrib.get('end')
    
  read_vmid = get_read_vmid_string(xpu,RG)
  READ_VMID_LIST = get_vmids(xpu, read_vmid)
  READ_VMID_STRING = get_vmid_string(READ_VMID_LIST)
  
  write_vmid = get_write_vmid_string(xpu, RG)
  WRITE_VMID_LIST = get_vmids(xpu, write_vmid)
  WRITE_VMID_STRING = get_vmid_string(WRITE_VMID_LIST)

  WRITE_DOMAIN_STRING = get_domain_string(RG.attrib.get('wdomains'))
  READ_DOMAIN_STRING = get_domain_string(RG.attrib.get('rdomains'))

  
def get_domain_string(domains1):
  domain_string = ""
  if( domains1 == ""):
    return ""
  domain_list = domains1.split(",")
  num_domains = len(domain_list)
  if( num_domains != 0):
    for domain in domain_list:
      if(domain_string is ''):
        if(domain != ''):
          domain_string += domains[domain]
      else:
        if(domain != ''):
          domain_string += " | "
          domain_string += domains[domain]
  if(domain_string is ''):
    domain_string += domains['NONE'];
  return domain_string
  
  
def get_vmids(xpu, vmid_string):
  
  if(vmid_string == None):
    return []

  vmid_list  = vmid_string.split(",")
  vmid_string1 = ""
  num_vmids = len(vmid_list)
  
  if (num_vmids == 0):
    vmid_list = []
  return vmid_list


def get_vmid_string(vmid_list):
  vmidlist_touse = []
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
      

def process_xpu_unmapped_rg(xpu, RG, tag, umr, do_print):
  global READ_DOMAIN_STRING
  global WRITE_DOMAIN_STRING
  global UMR_TEXT 

  index = "TZBSP_UNMAPPED_PARTITION"
  rg_static_enabled = True
  
  dissect_RG(xpu, RG)
  if(TZ_RESOURCE == True):
    UMR_TEXT += 'TZBSP_XPU_TZ_OWNER'
  elif (MSA_RESOURCE == True):
    UMR_TEXT += 'TZBSP_XPU_MSA_OWNER'
  elif (SP_RESOURCE == True):
    UMR_TEXT += 'TZBSP_XPU_SP_OWNER'
  else:
    UMR_TEXT += 'TZBSP_XPU_HYP_OWNER'
  
  #if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
  UMR_TEXT += "| TZBSP_XPU_ENABLE"
  UMR_TEXT += ", \n  "  
  
  if READ_DOMAIN_STRING != "":
    READ_DOMAIN_STRING += " | "
  if WRITE_DOMAIN_STRING != "":
    WRITE_DOMAIN_STRING += " | "

  UMR_TEXT = READ_DOMAIN_STRING + READ_VMID_STRING + ", " + WRITE_DOMAIN_STRING + WRITE_VMID_STRING
  
  if do_print == True:
    config_file_handle.write(text)

  
def process_xpu_rg(xpu, RG, tag, umr, do_print):
  global READ_DOMAIN_STRING
  global WRITE_DOMAIN_STRING
  global UMR_TEXT

# All profiles with MSA as owner and R/W access apart from MSA has to be configured in MBA  
#  RGtag = RG.attrib.get("profile")
#  if(RGtag != tag):
#    return
  
  index = "TZBSP_UNMAPPED_PARTITION"
  rg_static_enabled = True
  if(umr != True):  
  
    RGindex = str(RG.attrib.get('index'))
    index = RGindex

    rg_static_enabled = True
    if 'true' != RG.find('XPU_RGn_FLAGS').attrib.get('static'):
      rg_static_enabled = False

  text = ""
  if False == rg_static_enabled:
    text += "/*\n"

  text += '{ ' + index + ', '
  dissect_RG(xpu, RG)
  if(TZ_RESOURCE == True):
    text += 'TZBSP_XPU_TZ_OWNER'
  elif (MSA_RESOURCE == True):
    text += 'TZBSP_XPU_MSA_OWNER'
  elif (SP_RESOURCE == True):
    text += 'TZBSP_XPU_SP_OWNER'
  else:
    text += 'TZBSP_XPU_HYP_OWNER'
  
  #if(tag == STATIC_TAG1)  or (tag == STATIC_TAG2):
  text += "| TZBSP_XPU_ENABLE"
  
  text += ", \n  "
  
  if READ_DOMAIN_STRING != "":
    READ_DOMAIN_STRING += " | "
  if WRITE_DOMAIN_STRING != "":
    WRITE_DOMAIN_STRING += " | "
    
  
  text += (READ_DOMAIN_STRING + READ_VMID_STRING + ", " + WRITE_DOMAIN_STRING + WRITE_VMID_STRING  ) 
  
  UMR_TEXT = READ_DOMAIN_STRING + READ_VMID_STRING + ", " + WRITE_DOMAIN_STRING + WRITE_VMID_STRING
  
  
  if (get_xpu_type(xpu) == "MPU"):
    text += (",\n  " + START_ADDRESS + INPUT_ADDR_MASK + ", " + END_ADDRESS + INPUT_ADDR_MASK)
  text +=  "},\n"

  if False == rg_static_enabled:
    text += "*/\n"
  
  if do_print == True:
  	if(generate_for_modem == True):
		if(MSA_RESOURCE == True):
			if(not(((READ_DOMAIN_STRING + READ_VMID_STRING == "TZBSP_DOMAIN_MSA_BIT | TZBSP_VMID_NOACCESS") or  
			  (READ_DOMAIN_STRING + READ_VMID_STRING == "TZBSP_DOMAIN_MSA_BIT") or
			  (READ_DOMAIN_STRING + READ_VMID_STRING == "TZBSP_VMID_NOACCESS")) and 			  
			  
			  ((WRITE_DOMAIN_STRING + WRITE_VMID_STRING == "TZBSP_DOMAIN_MSA_BIT | TZBSP_VMID_NOACCESS") or 
			  (WRITE_DOMAIN_STRING + WRITE_VMID_STRING == "TZBSP_DOMAIN_MSA_BIT") or
			  (WRITE_DOMAIN_STRING + WRITE_VMID_STRING == "TZBSP_VMID_NOACCESS")))):
			  
				config_file_handle.write(text)
	else:
		config_file_handle.write(text)  


def get_xpu_type(xpu):
  parameters = xpu.getiterator("Parameter")
  for parameter in parameters:
    if(parameter.attrib.get("name") == "XPU3_IDR0:XPUTYPE"):
      xpu1 = parameter.attrib.get("value")
      if xpu1 == "1":
        return "APU"
      elif xpu1 == "2":
        return "MPU"
      elif xpu1 == "0":
        return "RPU"


def find_tags_in_xpus(XPUs):
  RGtag_list = []
  for xpu in XPUs:
    if(get_xpu_type(xpu) == "MPU"):
      RGs = xpu.getiterator('PRTn')
    else:
      RGs = xpu.getiterator('RGn')
    for RG in RGs:
      RGtag = RG.attrib.get("profile")
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
    RGtag = RG.attrib.get("profile")
    RGtag_list.append(RGtag) 
  RGtag_set = set(RGtag_list)
  return RGtag_set
  

def find_xpu_from_name(XPUs, tag, stringname):
  for xpu in XPUs:
    xpu_name = xpu.attrib.get('fqname')
    #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tag
    xpu_name = xpu_name.replace('.', '_')
    xpu_array_name = xpu_name + tag + VERSION_SUFFIX
    if(xpu_array_name == stringname):
      return xpu

def find_xpu_input_addr_mask(xpu):
    ret = None
    hw = xpu.find('HWConfig')
    hw_int = hw.find('HWIntegrationParameters')

    if hw_int is not None:
      parameters = hw_int.getiterator("Parameter")
      if parameters is not None:
        for parameter in parameters :
          if(parameter.attrib.get("name") == "InputMSB"):
            ret = parameter.attrib.get("value")
    
    if ret is not None :
        if (int(ret) >= 32):
          ret = "&" + str(hex(0xFFFFFFFFFFFFFFF >> (64 - int(ret))))
        else:
          ret = "&" + str(hex(0xFFFFFFFF >> (32 - int(ret))))
    else:
        ret= ""
    return ret

def process_xpus():
  global TZ_RESOURCE, MSA_RESOURCE
  global START_ADDRESS, END_ADDRESS
  global READ_VMID_STRING, WRITE_VMID_STRING
  global GLOBAL_READ, GLOBAL_WRITE
  global ROE, RWE
  global XPU_GLOBAL_MSA
  global UMR_TEXT
  global INPUT_ADDR_MASK

  #find all the tags in the whole xml
  tags = find_tags_in_xpus(XPUs)
  #create a list per tag
  xpu_list = dict()

  for tag in tags: 
    xpu_list[tag] = []

  if(DEFAULT_COMMENTED== True):
    comment = "// "
  else:
    comment = ""

  for xpu in XPUs:
    xpu_name = str(xpu.attrib.get('fqname'))
    xpu_tag_list = find_tags_in_xpu(xpu)
    for tag in xpu_tag_list:
      if(tag != PROFILE and PROFILE != "all"):
        continue
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
    xpu_name = str(xpu.attrib.get('fqname'))

    xpu_tag_list = find_tags_in_xpu(xpu)
    INPUT_ADDR_MASK = find_xpu_input_addr_mask(xpu)
    for tag in xpu_tag_list:
      if(tag != PROFILE and PROFILE != "all"):
        continue
      config_file_handle.write('\n/* ' + xpu_name + ' */\n')

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
        config_file_handle.write("#ifdef " + "__XPU__"+ xpu_array_name +"__ENABLE__\n")
        config_file_handle.write("static const tzbsp_mpu_rg_t " + xpu_array_name + "[] = {\n")
      else:
        RGs = xpu.getiterator('RGn')
        #xpu_array_name = xpu_name.split('.')[1] + '_' + xpu_name.split('.')[2] + tagname
        xpu_name = xpu_name.replace('.', '_')
        xpu_array_name = xpu_name + tagname + VERSION_SUFFIX
        config_file_handle.write("#ifdef " + "__XPU__"+ xpu_array_name +"__ENABLE__\n")
        #print "#ifdef " + "__XPU__"+ xpu_name +"__ENABLE__"
        config_file_handle.write("static const tzbsp_rpu_rg_t " + xpu_array_name + "[] = {\n")
  
      xpu_list[tag].append(xpu_array_name)

      for RG in RGs:
        process_xpu_rg(xpu,RG,tag, False, True)
      
      # process unmapped partition info now
 #     if (get_xpu_type(xpu) == "MPU"):
 #       UMRs = xpu.getiterator('Unmapped')
 #       for UMR in UMRs:
 #         process_xpu_rg(xpu,UMR,tag, True, True)

      config_file_handle.write("};\n")
      config_file_handle.write("#endif\n")
      config_file_handle.write("\n") #endif

  for tag in tags:
    
    if(tag != PROFILE and PROFILE != "all"):
      continue
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
      physical_address_iterator = xpu.getiterator('Parameter')
      for phy_addr in physical_address_iterator:
        if(phy_addr.attrib.get("name") == "FF_ADDRESS"):
          physical_address = phy_addr.attrib.get("value")

      
	  # process unmapped partition info now
      if (get_xpu_type(xpu) == "MPU"):
        UMRs = xpu.getiterator('Unmapped')
        for UMR in UMRs:
               process_xpu_unmapped_rg(xpu,UMR,tag, True, False)
        if(UMR_TEXT == ""):
          UMR_TEXT = "TZBSP_VMID_NOACCESS, TZBSP_VMID_NOACCESS" 
      else:
        UMR_TEXT = "TZBSP_VMID_NOACCESS, TZBSP_VMID_NOACCESS"

	  #get SU now
      #xpu_rpu_acr = xpu.getiterator('XPU_RPU_ACR0')
      #for su in xpu_rpu_acr:
        #suvmid = su.attrib.get("RWE")
        #break #only one SU so break
      #suvmid = get_vmid_string(get_vmids(xpu, str(suvmid)))

      # find config hint
      enable_text = "TZBSP_XPU_ENABLE"
      msa_text =""
      #xpu_flags = xpu.getiterator('XPU_FLAGS')
      #for xpu_flag in xpu_flags:
        #config_hint = xpu_flag.attrib.get("hint")
        #break #only one SU so break
      #if config_hint == "static":
        #enable_text = "TZBSP_XPU_ENABLE"
      #else:
        #enable_text = ""
    
      #if("true" == xpu.find('SWConfig').find('XPU_MCR').attrib.get('XPUMSAE')):
        #msa_text = "TZBSP_XPU_MODEM_PROT"
      #else:
        #msa_text = ""

      #if("true" == xpu.find('SWConfig').find('XPU_CR').attrib.get('MSAE')):
        #if msa_text == "":
          #msa_text = "TZBSP_XPU_CR_MSAE"
        #else:
          #msa_text += " | TZBSP_XPU_CR_MSAE"

      if enable_text == "" and msa_text =="":
        flag_text = "0"
      elif msa_text == "":
        flag_text = enable_text
      elif enable_text == "":
        flag_text = msa_text
      else:
        flag_text = enable_text + " | " + msa_text


      config_file_handle.write("#ifdef " + "__XPU__"+ xpu_name +"__ENABLE__\n")
      config_file_handle.write("/* " + xpu_name + " */\n")
      
      if(tag == ""):
        if VERSION_SUFFIX == "":
          xpu_name_enum = xpu_name_to_enum[xpu_name]
        else:
          xpu_name_enum = xpu_name_to_enum[xpu_name[:(-1 * len(VERSION_SUFFIX))]]
      else:
        if VERSION_SUFFIX == "":
          xpu_name_enum = xpu_name_to_enum[xpu_name[:(-1 * (2 + len(tag)))]]
        else:
          temp = xpu_name[:(-1 * len(VERSION_SUFFIX))]
          temp = xpu_name[:(-1 * (2 + len(tag)))]
          xpu_name_enum = xpu_name_to_enum[temp]

      config_file_handle.write('{ ' + physical_address + ", " + xpu_name_enum + ", " + flag_text +', \n  ' + UMR_TEXT + ", " + str(suvmid) + ", \n  " + "ARR_SIZE(" + xpu_name + "), {" + xpu_name + "}},\n")
      config_file_handle.write("#endif\n\n")
      UMR_TEXT = ""

    config_file_handle.write("};\n\n")

    config_file_handle.write("const uint32 g_tzbsp_xpu_cfg_count" + tagname + VERSION_SUFFIX + "=\n")
    config_file_handle.write("    sizeof(g_tzbsp_xpu_cfg" + tagname + VERSION_SUFFIX + ") / sizeof(g_tzbsp_xpu_cfg" + tagname + VERSION_SUFFIX + "[0]);\n\n\n")


def main(arglist):
  global root, XPUs, enable_file_handle, config_file_handle, VERSION_SUFFIX, PROFILE , generate_for_modem

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
  parser.add_option("-p", "--profile",
                    dest="profile",
                    help="Profile Configuration. Do not use this option for default profile config. For all profile use -p all",
                    metavar="PROFILE",
                    default="")
  parser.add_option("-m", "--modem",
                  action="store_true", dest="modem", default=False,
                  help="Use this option to just get Modem owned resources")

  (options, args) = parser.parse_args(arglist)

  if(options.modem):
	generate_for_modem = True
  
  print generate_for_modem
  
  tree = ET.parse(options.xmlfile)
  config_file_handle = open(options.config_file, 'w')
  enable_file_handle = open(options.enable_file, 'w')
  
  PROFILE = options.profile
  
  VERSION_SUFFIX = options.version_suffix

  enable_file_name = os.path.basename(options.enable_file)
  enable_file_define = "__%s__" % enable_file_name.replace(".", "_").upper()
  enable_file_handle.write("#ifndef %s\n\n" % enable_file_define)
  enable_file_handle.write("#define %s\n\n" % enable_file_define) 
  
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
  
  #for xpu in XPUs:
	#config_file_handle.write(ET.tostring(xpu))


  enable_file_handle.write("#endif\n")
  enable_file_handle.close()
  config_file_handle.close()


if __name__ == "__main__":
  main(sys.argv[1:])

