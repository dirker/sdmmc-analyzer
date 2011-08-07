#include <AnalyzerChannelData.h>
#include "SDMMCAnalyzer.h"

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
        mClock->AdvanceToNextEdge();
        ReportProgress(mClock->GetSampleNumber());
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