#include "uimage_rpm_log.h"
#include "uimage_rpm_assert.h"
#include "micro_diagbuffer_i.h"

/*
 * NOTE:
 * If crashscrope does not parse micro ulog, load the dump and run below script on Modem T32 Sim to parse micro ulog manually:
 * CD.DO <MODEM_BUILD>\modem_proc\core\services\diag\micro_diagbuffer\scripts\micro_diagbuffer_dump.cmm <OUTPUT_DIR>
 *
 * To merge ulogs, use go/umerge.
 */
extern micro_ULogHandle uImage_rpm_log_hnd;
extern char uImage_rpm_log_buffer[UIMAGE_RPM_LOG_SIZE];

/*******************************************************************************
 *
 * uImage_rpm_log_init
 *
 * This function is used to initialize log handler for uImage RPM.
 *
 */
void uImage_rpm_log_init(void)
{
    uint32            effective_buf_size;
    micro_ULogResult  status;

    status = micro_ULog_CreateLog(&uImage_rpm_log_hnd,
                                  UIMAGE_RPM_LOG_NAME,
                                  uImage_rpm_log_buffer,
                                  &effective_buf_size,
                                  sizeof(uImage_rpm_log_buffer));

    CORE_VERIFY(MICRO_ULOG_SUCCESS == status); /* cannot log yet */

    /* Enabling the log */
    micro_ULog_Enable(uImage_rpm_log_hnd);

    UIMAGE_RPM_LOG("log_init_done"); 
}

