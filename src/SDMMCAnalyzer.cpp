#include <AnalyzerChannelData.h>
#include "SDMMCAnalyzer.h"
#include "SDMMCAnalyzerResults.h"
#include "SDMMCHelpers.h"

const char SDMMCAnalyzer::Name[] = "SDMMC";

SDMMCAnalyzer::SDMMCAnalyzer()
:	Analyzer(),
	mSettings(new SDMMCAnalyzerSettings),
	mSimulationInitialized(false)
{
	SetAnalyzerSettings(mSettings.get());
}

SDMMCAnalyzer::~SDMMCAnalyzer()
{
	KillThread();
}

const char* SDMMCAnalyzer::GetAnalyzerName() const
{
	return Name;
}

void SDMMCAnalyzer::WorkerThread()
{
	mResults.reset(new SDMMCAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());

	mResults->AddChannelBubblesWillAppearOn(mSettings->mCommandChannel);

	mClock = GetAnalyzerChannelData(mSettings->mClockChannel);
	mCommand = GetAnalyzerChannelData(mSettings->mCommandChannel);

	while (true) {
		int cmdindex;

		ReportProgress(mClock->GetSampleNumber());
		AdvanceToNextClockRising();
		
		cmdindex = TryReadCommand();
		if (cmdindex < 0) {
			/* continue if parsing the command failed */
			continue;
		}

		if (mSettings->mProtocol == PROTOCOL_MMC) {
			MMCResponse response = SDMMCHelpers::MMCCommandResponse(cmdindex);
			if (response != MMC_RSP_NONE)
				/* FIXME: handle timeout */
				WaitForAndReadMMCResponse(response);
		} else {
			/* FIXME: implement SD response handling */
		}
	}
}

bool SDMMCAnalyzer::NeedsRerun()
{
	return false;
}

U32 SDMMCAnalyzer::GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels)
{
	if (!mSimulationInitialized) {
	mDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
	mSimulationInitialized = true;
	}

	return mDataGenerator.GenerateSimulationData(newest_sample_requested, sample_rate, simulation_channels);
}

U32 SDMMCAnalyzer::GetMinimumSampleRateHz()
{
	return 400000 * 4;
}

void SDMMCAnalyzer::AdvanceToNextClockRising()
{
	do {
		mClock->AdvanceToNextEdge();
	} while (mClock->GetBitState() != BIT_HIGH);

	mCommand->AdvanceToAbsPosition(mClock->GetSampleNumber());
}

int SDMMCAnalyzer::TryReadCommand()
{
	int index;
	
	/* check for start bit */
	if (mCommand->GetBitState() != BIT_LOW)
		return -1;

	mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::Start, mSettings->mCommandChannel);
	AdvanceToNextClockRising();
	
	/* transfer bit */
	if (mCommand->GetBitState() != BIT_HIGH) {
		/* if host is not transferring this is no command */
		mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::X, mSettings->mCommandChannel);
		return -1;
	}
	AdvanceToNextClockRising();

	/* command index and argument */
	{
		Frame frame;

		frame.mStartingSampleInclusive = mClock->GetSampleNumber();
		frame.mData1 = 0;
		frame.mData2 = 0;
		frame.mType = SDMMCAnalyzerResults::FRAMETYPE_COMMAND;

		for (int i = 0; i < 6; i++) {
			frame.mData1 = (frame.mData1 << 1) | mCommand->GetBitState();
			AdvanceToNextClockRising();
		}

		for (int i = 0; i < 32; i++) {
			frame.mData2 = (frame.mData2 << 1) | mCommand->GetBitState();
			AdvanceToNextClockRising();
		}

		frame.mEndingSampleInclusive = mClock->GetSampleNumber() - 1;
		mResults->AddFrame(frame);
		
		/* save index for returning */
		index = (int)frame.mData1;
	}

	/* crc */
	{
		Frame frame;

		frame.mStartingSampleInclusive = mClock->GetSampleNumber();
		frame.mData1 = 0;
		frame.mType = SDMMCAnalyzerResults::FRAMETYPE_CRC;

		for (int i = 0; i < 7; i++) {
			frame.mData1 = (frame.mData1 << 1) | mCommand->GetBitState();
			AdvanceToNextClockRising();
		}

		frame.mEndingSampleInclusive = mClock->GetSampleNumber() - 1;
		mResults->AddFrame(frame);
	}

	/* stop bit */
	mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mCommandChannel);

	mResults->CommitResults();
	ReportProgress(mClock->GetSampleNumber());
	
	return index;
}

int SDMMCAnalyzer::WaitForAndReadMMCResponse(enum MMCResponse response)
{
	return -1;
}

/*
 * loader hooks
 */

const char* GetAnalyzerName()
{
	return SDMMCAnalyzer::Name;
}

Analyzer* CreateAnalyzer()
{
	return new SDMMCAnalyzer();
}

void DestroyAnalyzer(Analyzer* analyzer)
{
	delete analyzer;
}
