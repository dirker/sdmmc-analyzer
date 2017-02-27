#ifndef __SDMMC_ANALYZER_RESULTS
#define __SDMMC_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class SDMMCAnalyzer;
class SDMMCAnalyzerSettings;

class SDMMCAnalyzerResults : public AnalyzerResults
{
public:
	SDMMCAnalyzerResults(SDMMCAnalyzer* analyzer, SDMMCAnalyzerSettings* settings);
	virtual ~SDMMCAnalyzerResults();
	virtual void GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id);
	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
	virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
	virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

public:
	enum FrameType {
		FRAMETYPE_HEADER,
		FRAMETYPE_COMMAND,
        FRAMETYPE_ACOMMAND,
		FRAMETYPE_RESPONSE,
		FRAMETYPE_CRC,
	};

protected:
	SDMMCAnalyzerSettings *mSettings;
	SDMMCAnalyzer *mAnalyzer;
};

#endif
