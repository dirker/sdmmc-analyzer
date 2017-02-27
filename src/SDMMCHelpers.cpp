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

static struct MMCResponse sdapp_responses[64] = {
    /*  0 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  1 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  2 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  3 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  4 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  5 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  6 */{ MMC_RSP_R1, 32, 64, false },
    /*  7 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  8 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /*  9 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 10 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 11 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 12 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 13 */{ MMC_RSP_R1, 32, 64, false },
    /* 14 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 15 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 16 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 17 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 18 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 19 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 20 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 21 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 22 */{ MMC_RSP_R1, 32, 64, false },
    /* 23 */{ MMC_RSP_R1, 32, 64, false },
    /* 24 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 25 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 26 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 27 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 28 */{ MMC_RSP_NONE, 0, 0, false },  /* reserved */
    /* 29 */{ MMC_RSP_NONE, 0, 0, false },  /* reserved */
    /* 30 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 31 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 32 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 33 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 34 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 35 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 36 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 37 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 38 */{ MMC_RSP_NONE, 0, 0, false },  /* reserved */
    /* 39 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 40 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 41 */{ MMC_RSP_R3, 32, 64, false },
    /* 42 */{ MMC_RSP_R1, 32, 64, false },
    /* 43 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 44 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 45 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 46 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 47 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 48 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 49 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 50 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 51 */{ MMC_RSP_R1, 32, 64, false },
    /* 52 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 53 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 54 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 55 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 56 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 57 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 58 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 59 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 60 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 61 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 62 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
    /* 63 */{ MMC_RSP_NONE, 0, 0, false }, /* reserved */
};

static struct MMCResponse invalid_response = {
	MMC_RSP_NONE,  0,   0, false
};

U8 SDMMCHelpers::crc7(const U8 *data, unsigned int size)
{
	U8 crc = 0;

	for (unsigned int i = 0; i < size; i++) {
		crc = __crc7(crc, *data);
		data++;
	}

	return __crc7_finalize(crc);
}

struct MMCResponse SDMMCHelpers::MMCCommandResponse(unsigned int index)
{
	if (index > 63)
		return invalid_response;

	return responses[index];
}

struct MMCResponse SDMMCHelpers::SDAppCommandResponse(unsigned int index)
{
    if (index > 63)
        return invalid_response;

    return sdapp_responses[index];
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
