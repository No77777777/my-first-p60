#ifndef VVHAL_PLATFORM_H
#define VVHAL_PLATFORM_H

#include "ipa_customer.h"
#include "ipa_flavor_defs.h"
#include "ipa_defs.h"
#include "ipa_port.h"
#include "HALhwio_ipa.h"

#if 0 /* change to 1 to enable GSI driver print its internal errors and warnings */
   #define GSI_LOG_ERROR(fmt, ...)     ipa_os_print(IPA_DRV,    IPA_ERROR, __FUNCTION__ ": ERR - " fmt "\n", ##__VA_ARGS__)
   #define GSI_LOG_WARN(fmt, ...)      ipa_os_print(IPA_DRV,    IPA_WARNING, __FUNCTION__ ": WARN - " fmt "\n", ##__VA_ARGS__)
#endif

#define GSI_DISABLE_TODO_WARNINGS   1

#endif
