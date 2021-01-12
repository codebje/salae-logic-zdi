#ifndef ZDI_SIMULATION_DATA_GENERATOR
#define ZDI_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>

class ZDIAnalyzerSettings;
struct SimulationData;

class ZDISimulationDataGenerator
{
public:
	ZDISimulationDataGenerator();
	~ZDISimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, ZDIAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	ZDIAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	ClockGenerator mClockGenerator;
	U32 mCurrentSample;

	void AdvanceAllBySec( double sec )
	{
		mZDISimulationChannels.AdvanceAll( mClockGenerator.AdvanceByTimeS( sec ) );
	}

	void OutputTransaction( const SimulationData &data );

	SimulationChannelDescriptorGroup mZDISimulationChannels;
	SimulationChannelDescriptor* mZCL;
	SimulationChannelDescriptor* mZDA;

};
#endif //ZDI_SIMULATION_DATA_GENERATOR
