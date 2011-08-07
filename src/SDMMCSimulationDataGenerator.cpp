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
    
    /* introduce a few periods of no clock */
    mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(10.0));
}

U32 SDMMCSimulationDataGenerator::GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **channels)
{
    U64 adjusted_newest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( newest_sample_requested, sample_rate, mSimulationSampleRateHz );

    while (mClock->GetCurrentSampleNumber() < adjusted_newest_sample_requested) {
        CreateIdle(0.1);
        CreateCommand();
    }
    
    *channels = mChannels.GetArray();
    return mChannels.GetCount();
}

void SDMMCSimulationDataGenerator::CreateClockPeriod()
{
    mClock->Transition();
    mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
    
    mClock->Transition();
    mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
}

void SDMMCSimulationDataGenerator::CreateIdle(double seconds)
{
    U64 samples = 400000.0 * seconds;
    U64 orig = mClock->GetCurrentSampleNumber();
    
    while ((mClock->GetCurrentSampleNumber() - orig) < samples) {
        CreateClockPeriod();
    }    
}

void SDMMCSimulationDataGenerator::CreateCommand()
{
    /* make sure we continue with clock as high */
    if (mClock->GetCurrentBitState() != BIT_HIGH) {
        mClock->Transition();
        mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
    }
    
    /* start bit */
    mCommand->TransitionIfNeeded(BIT_LOW);
    CreateClockPeriod();
    
    /* host transfers */
    mCommand->TransitionIfNeeded(BIT_HIGH);
    CreateClockPeriod();
    
    /* index: 0 */
    for (int i = 0; i < 6; i++) {
        mCommand->TransitionIfNeeded(BIT_LOW);
        CreateClockPeriod();
    }
    
    /* argument: 0 */
    for (int i = 0; i < 32; i++) {
        mCommand->TransitionIfNeeded(BIT_LOW);
        CreateClockPeriod();
    }
    
    /* FIXME: calculate real crc7 */
    for (int i = 0; i < 7; i++) {
        mCommand->Transition();
        CreateClockPeriod();
    }
    
    /* stop bit */
    mCommand->TransitionIfNeeded(BIT_HIGH);
    CreateClockPeriod();
}