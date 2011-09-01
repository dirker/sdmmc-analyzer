#ifndef __SDMMC_HELPERS
#define __SDMMC_HELPERS

#include <Analyzer.h>

enum MMCResponseType {
	MMC_RSP_NONE,
	MMC_RSP_R1,
	MMC_RSP_R2_CID,
	MMC_RSP_R2_CSD,
	MMC_RSP_R3,
	MMC_RSP_R4,
	MMC_RSP_R5,
};

struct MMCResponse
{
	enum MMCResponseType mType;
	unsigned int mBits;
	int mTimeout;
	bool mBusy;
};

class SDMMCHelpers
{
public:

public:
	static U8 crc7(const U8 *data, unsigned int size);
	static struct MMCResponse MMCCommandResponse(unsigned int index);

private:
	static U8 __crc7(U8 crc, U8 data);
	static U8 __crc7_finalize(U8 crc);
};

#endif
