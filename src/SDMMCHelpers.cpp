#include "SDMMCHelpers.h"

static struct MMCResponse responses[64] = {
	/*  0 */ {MMC_RSP_NONE,     0,   0, false},
	/*  1 */ {MMC_RSP_R3,      32,   5, false},
	/*  2 */ {MMC_RSP_R2_CID, 128,   5, false},
	/*  3 */ {MMC_RSP_R1,      32,  64, false},
	/*  4 */ {MMC_RSP_NONE,     0,   0, false},
	/*  5 */ {MMC_RSP_R1,      32,  64, true},
	/*  6 */ {MMC_RSP_R1,      32,  64, true},
	/*  7 */ {MMC_RSP_R1,      32,  64, true},
	/*  8 */ {MMC_RSP_R1,      32,  64, false},
	/*  9 */ {MMC_RSP_R2_CSD, 128,  64, false},
	/* 10 */ {MMC_RSP_R2_CID, 128,  64, false},
	/* 11 */ {MMC_RSP_R1,      32,  64, false},
	/* 12 */ {MMC_RSP_R1,      32,  64, false},
	/* 13 */ {MMC_RSP_R1,      32,  64, false},
	/* 14 */ {MMC_RSP_R1,      32,  64, false},
	/* 15 */ {MMC_RSP_NONE,     0,   0, false},
	/* 16 */ {MMC_RSP_R1,      32,  64, false},
	/* 17 */ {MMC_RSP_R1,      32,  64, false},
	/* 18 */ {MMC_RSP_R1,      32,  64, false},
	/* 19 */ {MMC_RSP_R1,      32,  64, false},
	/* 20 */ {MMC_RSP_R1,      32,  64, false},
	/* 21 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 22 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 23 */ {MMC_RSP_R1,      32,  64, false},
	/* 24 */ {MMC_RSP_R1,      32,  64, false},
	/* 25 */ {MMC_RSP_R1,      32,  64, false},
	/* 26 */ {MMC_RSP_R1,      32,  64, false},
	/* 27 */ {MMC_RSP_R1,      32,  64, false},
	/* 28 */ {MMC_RSP_R1,      32,  64, true},
	/* 29 */ {MMC_RSP_R1,      32,  64, true},
	/* 30 */ {MMC_RSP_R1,      32,  64, false},
	/* 31 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 32 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 33 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 34 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 35 */ {MMC_RSP_R1,      32,  64, false},
	/* 36 */ {MMC_RSP_R1,      32,  64, false},
	/* 37 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 38 */ {MMC_RSP_R1,      32,  64, true},
	/* 39 */ {MMC_RSP_R4,      32,  64, false},
	/* 40 */ {MMC_RSP_R5,      32,  64, false},
	/* 41 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 42 */ {MMC_RSP_R1,      32,  64, false},
	/* 43 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 44 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 45 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 46 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 47 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 48 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 49 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 50 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 51 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 52 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 53 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 54 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 55 */ {MMC_RSP_R1,      32,  64, false},
	/* 56 */ {MMC_RSP_R1,      32,  64, false},
	/* 57 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 58 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 59 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 60 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 61 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 62 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
	/* 63 */ {MMC_RSP_NONE,     0,   0, false}, /* reserved */
};

static struct MMCResponse invalid_response = {
	MMC_RSP_NONE,  0,   0, false
};

struct MMCResponse SDMMCHelpers::MMCCommandResponse(unsigned int index)
{
	if (index > 63)
		return invalid_response;

	return responses[index];
}

/*
  Polynomial = 0x89 (2^7 + 2^3 + 1)
  width      = 7 bit
*/
static unsigned char crc7_table[256] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

U8 SDMMCHelpers::crc7(const U8 *data, unsigned int size)
{
	U8 crc7_accum = 0;
	int i;

	for (i=0;  i < size; i++) {
		crc7_accum = crc7_table[(crc7_accum << 1) ^ data[i]];
	}
	return crc7_accum;
}
