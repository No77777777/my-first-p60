#include "uimage_rpm_smd.h"
#include "uimage_rpm_assert.h"
#include "uimage_rpm_log.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DDIHWIO.h"

extern uImage_smd_shared_info_t uImage_rpm_smd_info;

/*******************************************************************************
 *
 * uImage_rpm_smd_init
 *
 * This function is used to initialize SMD shared info.
 *
 */
void uImage_rpm_smd_init(void)
{
    DalDeviceHandle* handle = NULL;
    uint8 *msg_ram_base     = NULL;
    DALResult result;

    result = DAL_DeviceAttach(DALDEVICEID_HWIO, &handle);

    UIMAGE_RPM_ASSERT((result == DAL_SUCCESS) && (handle != NULL), "failed to get DAL device handle");

    /*
     * Get the virtual address of MSG RAM base, which is mapped to 0x00060000
     */
    result = DalHWIO_MapRegion(handle, "RPM_SS_MSG_RAM_START_ADDRESS", &msg_ram_base);

    UIMAGE_RPM_ASSERT(result == DAL_SUCCESS, "failed to get MSG RAM vaddr");

    /*
     * Calculate the virtual addresses mapped to
     *
     *   0x00061000 (0x00060000 + 0x1000)
     *   0x0006102C (0x00060000 + 0x102C)
     *   0x00061800 (0x00060000 + 0x1800)
     *   0x00061C00 (0x00060000 + 0x1C00)
     */
    uImage_rpm_smd_info.tx_shared_info_ptr_algn = (uImage_smd_shared_stream_info_algn_t *)(msg_ram_base + 0x1000);
    uImage_rpm_smd_info.rx_shared_info_ptr_algn = (uImage_smd_shared_stream_info_algn_t *)(msg_ram_base + 0x102C);
    uImage_rpm_smd_info.tx_shared_fifo          = (uint32 *)(msg_ram_base + 0x1800);
    uImage_rpm_smd_info.rx_shared_fifo          = (uint32 *)(msg_ram_base + 0x1C00);

    UIMAGE_RPM_LOG("smd_init_done");
}

