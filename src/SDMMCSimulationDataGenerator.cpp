#include "SDMMCSimulationDataGenerator.h"
#include "SDMMCAnalyzerSettings.h"

SDMMCSimulationDataGenerator::SDMMCSimulationDataGenerator()
{
}

SDMMCSimulationDataGenerator::~SDMMCSimulationDataGenerator()
{
}

void SDMMCSimulationDataGenerator::Initialize(U32 simulation_sample_rate, SDMMCAnalyzerSettings *settings)
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;
    
    mClockGenerator.Init(400000, mSimulationSampleRateHz);
    
    mClock = mChannels.Add(mSettings->mClockChannel, mSimulationSampleRateHz, BIT_HIGH);
    mCommand = mChannels.Add(mSettings->mCommandChannel, mSimulationSampleRateHz, BIT_HIGH);
    
    /* introduce a bit of idle time */
    mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(10.0));
}

U32 SDMMCSimulationDataGenerator::GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **channels)
{
    U64 adjusted_newest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( newest_sample_requested, sample_rate, mSimulationSampleRateHz );

    /* FIXME: also generate traffic on the CMD line */
    while (mClock->GetCurrentSampleNumber() < adjusted_newest_sample_requested) {
        mClock->Transition();
        mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
    }
    
    *channels = mChannels.GetArray();
    return mChannels.GetCount();
}

