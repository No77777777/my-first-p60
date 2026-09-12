#include <stride.h>
#include <ipaIMEntry.h>

void ipa_stride_init(void)
{
    strideCreateIMThread(ipa);
}
