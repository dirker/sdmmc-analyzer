#include "SDMMCAnalyzerResults.h"
#include "SDMMCAnalyzer.h"
#include "SDMMCAnalyzerSettings.h"
#include "SDMMCHelpers.h"

SDMMCAnalyzerResults::SDMMCAnalyzerResults(SDMMCAnalyzer* analyzer, SDMMCAnalyzerSettings* settings)
	: AnalyzerResults(),
	mSettings(settings),
	mAnalyzer(analyzer)
{
}

SDMMCAnalyzerResults::~SDMMCAnalyzerResults()
{
}

void SDMMCAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
	GenerateFrameText(frame_index, display_base, TEXTTYPE_BUBBLE);
}

void SDMMCAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
	ClearResultStrings();
	AddResultString("not supported");
}

void SDMMCAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
	GenerateFrameText(frame_index, display_base, TEXTTYPE_TABULAR);
}

void SDMMCAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
	ClearResultStrings();
	AddResultString("not supported");
}

void SDMMCAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
	ClearResultStrings();
	AddResultString("not supported");
}

void SDMMCAnalyzerResults::GenerateFrameText(U64 frame_index, DisplayBase display_base, TextType text_type) {

	if (text_type == TEXTTYPE_BUBBLE) {
		ClearResultStrings();
	}
	else if (text_type == TEXTTYPE_TABULAR) {
		ClearTabularText();
	}

	bool print_short = (text_type == TEXTTYPE_BUBBLE);  //add abbreviations for bubble text at different zoom levels
	Frame frame = GetFrame(frame_index);

	switch (frame.mType) {
	case FRAMETYPE_HEADER:
		if (frame.mData1 == 1)
			AddText(text_type, "Host sending");
		else
			AddText(text_type, "Card sending");
		break;

	case FRAMETYPE_COMMAND:
	{
		char str_cmd[4];
		char str_arg[33];

		AnalyzerHelpers::GetNumberString(frame.mData1, Decimal, 6, str_cmd, sizeof(str_cmd));
		AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 32, str_arg, sizeof(str_arg));
		if (print_short)
		{
			AddText(text_type, "CMD");
			AddText(text_type, "CMD", str_cmd);
		}
		AddText(text_type, "CMD", str_cmd, ", arg=", str_arg);
		break;
	}

	case FRAMETYPE_RESPONSE:
	{
		char str_32[33];

		switch (frame.mFlags) {
		case MMC_RSP_R1:
		{
			const char *str_state = "reserved";
			std::string str_flags("");

			switch ((frame.mData1 >> 9) & 0xf) {
			case 0: str_state = "Idle"; break;
			case 1: str_state = "Ready"; break;
			case 2: str_state = "Ident"; break;
			case 3: str_state = "Stby"; break;
			case 4: str_state = "Tran"; break;
			case 5: str_state = "Data"; break;
			case 6: str_state = "Rcv"; break;
			case 7: str_state = "Prg"; break;
			case 8: str_state = "Dis"; break;
			case 9: str_state = "Btst"; break;
			case 10: str_state = "Slp "; break;
			}

			AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 32, str_32, sizeof(str_32));

			if (print_short)
			{
				AddText(text_type, "R1");
				AddText(text_type, "R1, ", str_state);
			}
			AddText(text_type, "R1, ", str_state, ", rsp=", str_32);

			if (frame.mData1 & (1 << 31))
				str_flags += " ADDRESS_OUT_OF_RANGE";
			if (frame.mData1 & (1 << 30))
				str_flags += " ADDRESS_MISALIGN";
			if (frame.mData1 & (1 << 29))
				str_flags += " BLOCK_LEN_ERROR";
			if (frame.mData1 & (1 << 28))
				str_flags += " ERASE_SEQ_ERROR";
			if (frame.mData1 & (1 << 27))
				str_flags += " ERASE_PARAM";
			if (frame.mData1 & (1 << 26))
				str_flags += " WP_VIOLATION";
			if (frame.mData1 & (1 << 25))
				str_flags += " CARD_IS_LOCKED";
			if (frame.mData1 & (1 << 24))
				str_flags += " LOCK_UNLOCK_FAILED";
			if (frame.mData1 & (1 << 23))
				str_flags += " COM_CRC_ERROR";
			if (frame.mData1 & (1 << 22))
				str_flags += " ILLEGAL_COMMAND";
			if (frame.mData1 & (1 << 21))
				str_flags += " CARD_ECC_FAILED";
			if (frame.mData1 & (1 << 20))
				str_flags += " CC_ERROR";
			if (frame.mData1 & (1 << 19))
				str_flags += " ERROR";
			if (frame.mData1 & (1 << 18))
				str_flags += " UNDERRUN";
			if (frame.mData1 & (1 << 17))
				str_flags += " OVERRUN";
			if (frame.mData1 & (1 << 16))
				str_flags += " CID/CSD_OVERWRITE";
			if (frame.mData1 & (1 << 15))
				str_flags += " WP_ERASE_SKIP";
			if (frame.mData1 & (1 << 13))
				str_flags += " ERASE_RESET";
			if (frame.mData1 & (1 << 8))
				str_flags += " READY_FOR_DATA";
			if (frame.mData1 & (1 << 7))
				str_flags += " SWITCH_ERROR";
			if (frame.mData1 & (1 << 5))
				str_flags += " APP_CMD";

			if (str_flags.length() > 0)
				AddText(text_type, "R1, ", str_state, ", rsp=", str_32, str_flags.c_str());

			break;
		}
		case MMC_RSP_R2_CID:
		{
			std::string res("R2");
			char pname[7], prv_str[4], psn_str[12];
			char rsp_str[64];

			if (print_short) AddText(text_type, res.c_str());

			res += " [CID]";
			if (print_short) AddText(text_type, res.c_str());

			res += " rsp=";
			AnalyzerHelpers::GetNumberString(frame.mData1 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData1 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData2 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData2 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			if (print_short) AddText(text_type, res.c_str());

			pname[0] = (frame.mData1 >> 32) & 0xff;
			pname[1] = (frame.mData1 >> 24) & 0xff;
			pname[2] = (frame.mData1 >> 16) & 0xff;
			pname[3] = (frame.mData1 >> 8) & 0xff;
			pname[4] = (frame.mData1 >> 0) & 0xff;
			pname[5] = (frame.mData2 >> 56) & 0xff;
			pname[6] = 0;

			unsigned prv = (unsigned)((frame.mData2 >> 48) & 0xff);
			prv_str[0] = '0' + ((prv >> 4) & 0x0f);
			prv_str[1] = '.';
			prv_str[2] = '0' + (prv & 0x0f);
			prv_str[3] = 0;

			unsigned psn = (unsigned)((frame.mData2 >> 16) & 0xfffffffful);
			AnalyzerHelpers::GetNumberString(psn, Decimal, 32, psn_str, sizeof(psn_str));

			res += " pnm='";
			res += pname;
			res += "' prv=";
			res += prv_str;
			res += " psn=";
			res += psn_str;
			AddText(text_type, res.c_str());

			break;
		}
		case MMC_RSP_R2_CSD:
		{
			std::string res("R2");
			char rsp_str[64];

			if (print_short) AddText(text_type, res.c_str());

			res += " [CSD]";
			if (print_short) AddText(text_type, res.c_str());

			res += " rsp=";
			AnalyzerHelpers::GetNumberString(frame.mData1 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData1 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData2 >> 32, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			res += " ";
			AnalyzerHelpers::GetNumberString(frame.mData2 & 0xffffffffull, display_base, 32, rsp_str, sizeof(rsp_str));
			res += rsp_str;
			AddText(text_type, res.c_str());

			break;
		}
		case MMC_RSP_R3:
			AnalyzerHelpers::GetNumberString(frame.mData1, Hexadecimal, 32, str_32, sizeof(str_32));
			if (print_short) AddText(text_type, "R3");
			AddText(text_type, "R3, ocr=", str_32);
			break;
		case MMC_RSP_R4:
			AddText(text_type, "R4");
			break;
		case MMC_RSP_R5:
			AddText(text_type, "R5");
			break;
		}
		break;
	}

	case FRAMETYPE_CRC:
	{
		char str_crc[8];

		AnalyzerHelpers::GetNumberString(frame.mData1, Hexadecimal, 7, str_crc, sizeof(str_crc));

		if (print_short) AddText(text_type, "CRC");
		AddText(text_type, "CRC=", str_crc);
		break;
	}

	default:
		AddText(text_type, "error");
	}
}

void SDMMCAnalyzerResults::AddText(TextType text_type,
	const char * str1,
	const char * str2,
	const char * str3,
	const char * str4,
	const char * str5,
	const char * str6)
{
	switch (text_type)
	{
	case SDMMCAnalyzerResults::TEXTTYPE_BUBBLE:
		AddResultString(str1, str2, str3, str4, str5, str6);
		break;
	case SDMMCAnalyzerResults::TEXTTYPE_TABULAR:
		AddTabularText(str1, str2, str3, str4, str5, str6);
		break;
	case SDMMCAnalyzerResults::TEXTTYPE_FILE:
		//TODO
		break;
	default:
		break;
	}
}
