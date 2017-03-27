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
	virtual void GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base);
	virtual void GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id);
	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
	virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
	virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

public:
	enum FrameType {
		FRAMETYPE_HEADER,
		FRAMETYPE_COMMAND,
		FRAMETYPE_RESPONSE,
		FRAMETYPE_CRC,
	};

protected:
	SDMMCAnalyzerSettings *mSettings;
	SDMMCAnalyzer *mAnalyzer;

	enum TextType {
		TEXTTYPE_BUBBLE,
		TEXTTYPE_TABULAR,
		TEXTTYPE_FILE
	};

	void GenerateFrameText(U64 frame_index, DisplayBase display_base, TextType text_type);
	void AddText(TextType text_type, const char* str1,
		const char* str2 = NULL,
		const char* str3 = NULL,
		const char* str4 = NULL,
		const char* str5 = NULL,
		const char* str6 = NULL);
};

#endif
