#include "SDMMCAnalyzer.h"

const char SDMMCAnalyzer::Name[] = "Name";

SDMMCAnalyzer::SDMMCAnalyzer()
:	Analyzer()
{
}

SDMMCAnalyzer::~SDMMCAnalyzer()
{
}

const char* SDMMCAnalyzer::GetAnalyzerName() const
{
	return Name;
}

void SDMMCAnalyzer::WorkerThread()
{
}

bool SDMMCAnalyzer::NeedsRerun()
{
    return false;
}

U32 SDMMCAnalyzer::GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels)
{
    return 0;
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