#include "SDMMCAnalyzerResults.h"
#include "SDMMCAnalyzer.h"
#include "SDMMCAnalyzerSettings.h"
#include "SDMMCHelpers.h"

SDMMCAnalyzerResults::SDMMCAnalyzerResults(SDMMCAnalyzer* analyzer, SDMMCAnalyzerSettings* settings)
:	AnalyzerResults(),
	mSettings(settings),
	mAnalyzer(analyzer)
{
}

SDMMCAnalyzerResults::~SDMMCAnalyzerResults()
{
}

static const char* cmd_abbrev_from_number(unsigned int cmd_number)
{
	if (cmd_number >= 43 && cmd_number <= 47) return "[reserved]";
	switch (cmd_number) {
		case 0: return "GO_IDLE_STATE";
		case 1: return "[reserved]";
		case 2: return "ALL_SEND_CID";
		case 3: return "SEND_RELATIVE_ADDR";
		case 4: return "SET_DSR";
		case 5: return "[reserved for SDIO]";
		case 6: return "SWITCH_FUNC";
		case 7: return "SELECT/DESELECT_CARD";
		case 8: return "SEND_IF_COND";
		case 9: return "SEND_CSD";
		case 10: return "SEND_CID";
		case 11: return "VOLTAGE_SWITCH";
		case 12: return "STOP_TRANSMISSION";
		case 13: return "SEND_STATUS";
		case 14: return "[reserved]";
		case 15: return "GO_INACTIVE_STATE";
		case 16: return "SET_BLOCKLEN";
		case 17: return "READ_SINGLE_BLOCK";
		case 18: return "READ_MULTIPLE_BLOCK";
		case 19: return "SEND_TUNING_BLOCK";
		case 20: return "SPEED_CLASS_CONTROL";
		case 21: return "[reserved for DPS]";
		case 22: return "[reserved]";
		case 23: return "SET_BLOCK_COUNT";
		case 24: return "WRITE_BLOCK";
		case 25: return "WRITE_MULTIPLE_BLOCK";
		case 26: return "[reserved for manufacturer]";
		case 27: return "PROGRAM_CSD";
		case 28: return "SWT_WRITE_PROT";
		case 29: return "CLR_WRITE_PROT";
		case 30: return "SEND_WRITE_PROT";
		case 31: return "[reserved]";
		case 32: return "ERASE_WR_BLK_START";
		case 33: return "ERASE_WR_BLK_END";
		case 34: return "[function dependent]";
		case 35: return "[function dependent]";
		case 36: return "[function dependent]";
		case 37: return "[function dependent]";
		case 38: return "ERASE";
		case 39: return "[reserved]";
		case 40: return "[defined by DPS]";
		case 41: return "[reserved]";
		case 42: return "LOCK_UNLOCK";
		case 48: return "READ_EXTR_SINGLE";
		case 49: return "WRITE_EXTR_SINGLE";
		case 50: return "[function dependent]";
		case 51: return "[reserved]";
		case 52: return "[sdio]";
		case 53: return "[sdio]";
		case 54: return "[sdio]";
		case 55: return "APP_CMD";
		case 56: return "GEN_CMD";
		case 57: return "[function dependent]";
		case 58: return "READ_EXTR_MULTI";
		case 59: return "WRITE_EXTR_MULTI";
		default: return "[invalid command number]";
	}
}

void SDMMCAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
	ClearResultStrings();
	Frame frame = GetFrame(frame_index);

	switch (frame.mType) {
	case FRAMETYPE_HEADER:
		if (frame.mData1 == 1)
			AddResultString("Host sending");
		else
			AddResultString("Card sending");
		break;

	case FRAMETYPE_COMMAND:
	{
		char str_cmd[4];
		char str_arg[33];

		AnalyzerHelpers::GetNumberString(frame.mData1, Decimal, 6, str_cmd, sizeof(str_cmd));
		AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 32, str_arg, sizeof(str_arg));

		AddResultString("CMD");
		AddResultString("CMD", str_cmd);
		AddResultString("CMD", str_cmd, ", arg=", str_arg);
		AddResultString("CMD", str_cmd, " (", cmd_abbrev_from_number(frame.mData1), "), arg=", str_arg);
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
			
			AddResultString("R1");
			AddResultString("R1, ", str_state);
			AddResultString("R1, ", str_state, ", rsp=", str_32);

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
				AddResultString("R1, ", str_state, ", rsp=", str_32, str_flags.c_str());

			break;
		}
		case MMC_RSP_R2_CID:
		{
			std::string res("R2");
			char pname[7], prv_str[4], psn_str[12];
			char rsp_str[64];

			AddResultString(res.c_str());

			res += " [CID]";
			AddResultString(res.c_str());

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
			AddResultString(res.c_str());

			pname[0] = (frame.mData1 >> 32) & 0xff;
			pname[1] = (frame.mData1 >> 24) & 0xff;
			pname[2] = (frame.mData1 >> 16) & 0xff;
			pname[3] = (frame.mData1 >>  8) & 0xff;
			pname[4] = (frame.mData1 >>  0) & 0xff;
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
			AddResultString(res.c_str());

			break;
		}
		case MMC_RSP_R2_CSD:
		{
			std::string res("R2");
			char rsp_str[64];

			AddResultString(res.c_str());

			res += " [CSD]";
			AddResultString(res.c_str());

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
			AddResultString(res.c_str());

			break;
		}
		case MMC_RSP_R3:
			AnalyzerHelpers::GetNumberString(frame.mData1, Hexadecimal, 32, str_32, sizeof(str_32));
			AddResultString("R3");
			AddResultString("R3, ocr=", str_32);
			break;
		case MMC_RSP_R4:
			AddResultString("R4");
			break;
		case MMC_RSP_R5:
			AddResultString("R5");
			break;
		}
		break;
	}

	case FRAMETYPE_CRC:
	{
		char str_crc[8];

		AnalyzerHelpers::GetNumberString(frame.mData1, Hexadecimal, 7, str_crc, sizeof(str_crc));

		AddResultString("CRC");
		AddResultString("CRC=", str_crc);
		break;
	}

	default:
		AddResultString("error");
	}
}

void SDMMCAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
	ClearResultStrings();
	AddResultString("not supported");
}

void SDMMCAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
	ClearResultStrings();
	AddResultString("not supported");
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
