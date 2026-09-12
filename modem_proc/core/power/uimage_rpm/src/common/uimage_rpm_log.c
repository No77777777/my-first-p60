#include "uimage_rpm_log.h"


/* a handle to the uImage RPM micro_ULog */
micro_ULogHandle uImage_rpm_log_hnd = NULL;

/* Pre-allocated log buffers required for island mode */
char uImage_rpm_log_buffer[UIMAGE_RPM_LOG_SIZE];

