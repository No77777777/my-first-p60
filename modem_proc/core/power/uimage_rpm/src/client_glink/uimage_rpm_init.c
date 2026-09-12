#include "uimage_rpm_glink.h"
#include "uimage_rpm_log.h"

/*******************************************************************************
 *
 * uImage_rpm_init
 *
 */
void uImage_rpm_init(void)
{
    uImage_rpm_log_init();
    uImage_rpm_glink_init();

    UIMAGE_RPM_LOG("init_all_done");
}

