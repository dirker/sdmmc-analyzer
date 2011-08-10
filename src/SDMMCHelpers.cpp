#include "SDMMCHelpers.h"

U8 SDMMCHelpers::crc7(const U8 *data, unsigned int size)
{
	U8 crc = 0;

	for (unsigned int i = 0; i < size; i++) {
		crc = __crc7(crc, *data);
		data++;
	}

	return __crc7_finalize(crc);
}

enum MMCResponse SDMMCHelpers::MMCCommandResponse(unsigned int index)
{
	switch (index) {
	case 0:
		return MMC_RSP_NONE;
	case 1:
		return MMC_RSP_R3;
	case 2:
		return MMC_RSP_R2;
	case 3:
		return MMC_RSP_R1;
	case 4:
		return MMC_RSP_NONE;
	case 5:
	case 6:
	case 7:
		return MMC_RSP_R1b;
	case 8:
		return MMC_RSP_R1;
	case 9:
	case 10:
		return MMC_RSP_R2;
	case 11:
	case 12:
	case 13:
	case 14:
		return MMC_RSP_R1;
	case 15:
		return MMC_RSP_NONE;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
		return MMC_RSP_R1;
	case 28:
	case 29:
		return MMC_RSP_R1b;
	case 30:
		return MMC_RSP_R1;
	case 35:
	case 36:
		return MMC_RSP_R1;
	case 38:
		return MMC_RSP_R1b;
	case 39:
		return MMC_RSP_R4;
	case 40:
		return MMC_RSP_R5;
	case 42:
	case 55:
	case 56:
		return MMC_RSP_R1;

	default:
		return MMC_RSP_NONE;
	}
}

/*
 * fast crc7 sd/mmc implementation taken from:
 *	 http://catid.mechafetus.com/news/news.php?view=253
 */

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
