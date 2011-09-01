#include "SDMMCSimulationDataGenerator.h"
#include "SDMMCAnalyzerSettings.h"
#include "SDMMCHelpers.h"

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
		CreateCommand(0, 0);
		CreateIdle(0.01);
		CreateCommand(1, 0);
		CreateResponse48(1, 0x00FF8000);
		CreateIdle(0.01);
		CreateCommand(1, 0x40300000);
		CreateResponse48(1, 0x80FF8000);
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
	U64 samples = (U64)(400000.0 * seconds);
	U64 orig = mClock->GetCurrentSampleNumber();

	while ((mClock->GetCurrentSampleNumber() - orig) < samples) {
		CreateClockPeriod();
	}
}

void SDMMCSimulationDataGenerator::CreateCommand(unsigned int index, unsigned long arg)
{
	U64 data =
		(0ull       << 39) | /* start bit */
		(1ull       << 38) | /* transmission bit: host */
		((U64)index << 32) | /* command index */
		((U64)arg   <<  0);  /* command argument */

	/* make sure we continue with clock as high */
	if (mClock->GetCurrentBitState() != BIT_HIGH) {
		mClock->Transition();
		mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
	}

	BitExtractor bits(data, AnalyzerEnums::MsbFirst, 40);

	for (int i = 0; i < 40; i++) {
		mCommand->TransitionIfNeeded(bits.GetNextBit());
		CreateClockPeriod();
	}

	/* FIXME: ugly, only works on little-endian */
	U8 crc = SDMMCHelpers::crc7((U8 *)&data, 5);

	for (int i = 0; i < 7; i++) {
		crc <<= 1;

		mCommand->TransitionIfNeeded(crc & 0x80 ? BIT_HIGH : BIT_LOW);
		CreateClockPeriod();
	}

	/* stop bit */
	mCommand->TransitionIfNeeded(BIT_HIGH);
	CreateClockPeriod();
}

void SDMMCSimulationDataGenerator::CreateResponse48(unsigned int index, unsigned long arg)
{
	U64 data =
		(0ull       << 39) | /* start bit */
		(0ull       << 38) | /* transmission bit: host */
		((U64)index << 32) | /* command index */
		((U64)arg   <<  0);  /* command argument */

	/* make sure we continue with clock as high */
	if (mClock->GetCurrentBitState() != BIT_HIGH) {
		mClock->Transition();
		mChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(.5));
	}

	CreateClockPeriod();
	CreateClockPeriod();

	BitExtractor bits(data, AnalyzerEnums::MsbFirst, 40);

	for (int i = 0; i < 40; i++) {
		mCommand->TransitionIfNeeded(bits.GetNextBit());
		CreateClockPeriod();
	}

	/* FIXME: ugly, only works on little-endian */
	U8 crc = SDMMCHelpers::crc7((U8 *)&data, 5);

	for (int i = 0; i < 7; i++) {
		crc <<= 1;

		mCommand->TransitionIfNeeded(crc & 0x80 ? BIT_HIGH : BIT_LOW);
		CreateClockPeriod();
	}

	/* stop bit */
	mCommand->TransitionIfNeeded(BIT_HIGH);
	CreateClockPeriod();
}
