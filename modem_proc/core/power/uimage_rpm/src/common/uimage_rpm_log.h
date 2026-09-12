#ifndef UIMAGE_RPM_LOG_H
#define UIMAGE_RPM_LOG_H

#include <stdarg.h>
#include "DALStdDef.h"
#include "micro_ULog.h"

#define GET_COUNT_MACRO(_0,_1,_2,_3,_4,_5,_6,COUNT,...) COUNT
#define DO_ULOG(...) micro_ULog_RealTimePrintf(uImage_rpm_log_hnd, GET_COUNT_MACRO(__VA_ARGS__,6,5,4,3,2,1,0), __VA_ARGS__)

/*
 * NOTE:
 * If crashscrope does not parse micro ulog, load the dump and run below script on Modem T32 Sim to parse micro ulog manually:
 * CD.DO <MODEM_BUILD>\modem_proc\core\services\diag\micro_diagbuffer\scripts\micro_diagbuffer_dump.cmm <OUTPUT_DIR>
 *
 * To merge ulogs, use go/umerge.
 */
#define UIMAGE_RPM_LOG(msg, args...) DO_ULOG(msg, ##args)

#if MEMORY_OPTIMIZATION
#   define UIMAGE_RPM_LOG_DEBUG(msg, args...)
#   define UIMAGE_RPM_LOG_SIZE     MICRO_ULOG_MEM_NEEDED(256)
#else
#   define UIMAGE_RPM_LOG_DEBUG(msg, args...) DO_ULOG(msg, ##args)
#   define UIMAGE_RPM_LOG_SIZE     MICRO_ULOG_MEM_NEEDED(1024)
#endif /* MEMORY_OPTIMIZATION */

#define UIMAGE_RPM_LOG_NAME     "uimage_rpm_master_log" /* max 23 chars + null */


extern micro_ULogHandle uImage_rpm_log_hnd;

/*******************************************************************************
 *
 * uImage_rpm_log_init
 *
 * This function is used to initialize log handler for uImage RPM.
 *
 */
void uImage_rpm_log_init(void);

#endif /* UIMAGE_RPM_LOG_H */
