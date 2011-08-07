#ifndef __SDMMC_SIMULATION_DATA_GENERATOR
#define __SDMMC_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>

class SDMMCAnalyzerSettings;
struct SDMMCRequest;

class SDMMCSimulationDataGenerator
{
public:
    SDMMCSimulationDataGenerator();
    ~SDMMCSimulationDataGenerator();
    
    void Initialize(U32 simulation_sample_rate, SDMMCAnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **channels);

protected:
    void CreateClockPeriod();
    void CreateIdle(double seconds);
    void CreateCommand();
    
protected:
    SDMMCAnalyzerSettings *mSettings;
    
    U32 mSimulationSampleRateHz;
    ClockGenerator mClockGenerator;
    
    SimulationChannelDescriptorGroup mChannels;
    SimulationChannelDescriptor *mClock;
    SimulationChannelDescriptor *mCommand;
};

#endif