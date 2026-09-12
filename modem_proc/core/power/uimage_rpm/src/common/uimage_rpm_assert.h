#ifndef UIMAGE_RPM_ASSERT_H
#define UIMAGE_RPM_ASSERT_H

#include "uimage_rpm_log.h"
#include "CoreVerify.h"

#if MEMORY_OPTIMIZATION
#   define UIMAGE_RPM_ASSERT(cond, err_msg, args...)   CORE_VERIFY(cond)
#else
#   define UIMAGE_RPM_ASSERT(cond, err_msg, args...)         \
       do {                                                  \
           if(!(cond)) {                                     \
               if(err_msg != NULL)                           \
                   UIMAGE_RPM_LOG("ERROR: "err_msg, ##args); \
               CORE_VERIFY(0);                               \
           }                                                 \
       } while(0)
#endif /* MEMORY_OPTIMIZATION */

#endif /* UIMAGE_RPM_ASSERT_H */

