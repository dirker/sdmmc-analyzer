#ifndef __SDMMC_HELPERS
#define __SDMMC_HELPERS

#include <Analyzer.h>

class SDMMCHelpers
{
public:
    static U8 crc7(const U8 *data, unsigned int size);
    
private:
    static U8 __crc7(U8 crc, U8 data);
    static U8 __crc7_finalize(U8 crc);
};

#endif