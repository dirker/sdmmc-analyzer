#include "SDMMCHelpers.h"

/*
 * fast crc7 sd/mmc implementation taken from:
 *   http://catid.mechafetus.com/news/news.php?view=253
 */

U8 SDMMCHelpers::crc7(const U8 *data, unsigned int size)
{
    U8 crc = 0;
    
    for (int i = 0; i < size; i++) {
        crc = __crc7(crc, *data);
        data++;
    }
    
    return __crc7_finalize(crc);
}

U8 SDMMCHelpers::__crc7(U8 crc, U8 data)
{
    data ^= crc << 1;
    
    if (data & 0x80)
        data ^= 9;
    
    crc = data ^ (crc & 0x78) ^ (crc << 4) ^ ((crc >> 3) & 0x0f);
    
    return crc & 0x7f;
}

U8 SDMMCHelpers::__crc7_finalize(U8 crc)
{
    crc = (crc << 1) ^ (crc << 4) ^ (crc & 0x70) ^ ((crc >> 3) & 0x0f);
    return crc | 1;
}