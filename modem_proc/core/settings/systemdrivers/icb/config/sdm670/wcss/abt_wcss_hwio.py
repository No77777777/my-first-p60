
# \\ben\corebsp_labdata_0001\sysdrv\hwio\HWIOGen.py --cfg=abt_hwio.py --flat=..\..\..\..\..\api\systemdrivers\hwio\sdm845\ARM_ADDRESS_FILE.FLAT
HWIO_REGISTER_FILES = [
  {
    'filename': 'ABTimeout_HWIO_WCSS.h',
    'modules': [
      'WCSS_ECAHB_TSLV',
      'WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV',
      'WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV',
      'WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG',
      'WCSS_SR',
      'TCSR_TCSR_REGS',
      'WCSS_HM_A_NOC_CFG_WIFI_NOC',
    ],
    'module-filter-include': {
      'WCSS_ECAHB_TSLV': ['BASE'],
      'WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV': ['BASE'],
      'WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV': ['BASE'],
      'WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG': ['QDSP_ERROR_INTR_ENABLES_SET', 'QDSP_ERROR_INTR_MASKED_STATUS', 'WAKEUP_INTR_ENABLES_SET'],
      'WCSS_SR': ['WCSS_SR_SR_INTERRUPT_ENABLE', 'WCSS_SR_SR_INTERRUPT_STATUS'],
      'TCSR_TCSR_REGS': ['TIMEOUT_SLAVE_GLB_EN'],
      'WCSS_HM_A_NOC_CFG_WIFI_NOC': ['WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID', 'WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID'],
    },
    'module-filter-exclude': {
    },
    'header': '''#include "msmhwiobase.h"
''',
  },
]

if __name__ == "__main__":
  from subprocess import Popen
  hwiogen = Popen([r"python", r"\\ben\corebsp_labdata_0001\sysdrv\hwio\HWIOGen.py", r"--cfg="+__file__, r"--flat=..\..\..\..\..\..\api\systemdrivers\hwio\sdm845\ARM_ADDRESS_FILE.FLAT"], shell=True)
  hwiogen.wait()

