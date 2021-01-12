#ifndef ZDI_ANALYZER_H
#define ZDI_ANALYZER_H

#include <Analyzer.h>
#include "ZDIAnalyzerResults.h"
#include "ZDISimulationDataGenerator.h"

class ZDIAnalyzerSettings;
class ANALYZER_EXPORT ZDIAnalyzer : public Analyzer2
{
public:
	ZDIAnalyzer();
	virtual ~ZDIAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::unique_ptr< ZDIAnalyzerSettings > mSettings;
	std::unique_ptr< ZDIAnalyzerResults > mResults;

	AnalyzerChannelData* mZCL;
	AnalyzerChannelData* mZDA;

	ZDISimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

private:
        U8 GetTransferredByte();
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //ZDI_ANALYZER_H
