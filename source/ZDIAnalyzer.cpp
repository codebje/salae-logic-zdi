#include "ZDIAnalyzer.h"
#include "ZDIAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

ZDIAnalyzer::ZDIAnalyzer()
:   Analyzer2(),  
    mSettings( new ZDIAnalyzerSettings() ),
    mSimulationInitilized( false )
{
    SetAnalyzerSettings( mSettings.get() );
}

ZDIAnalyzer::~ZDIAnalyzer()
{
    KillThread();
}

void ZDIAnalyzer::SetupResults()
{
    mResults.reset( new ZDIAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );

    //mResults->AddChannelBubblesWillAppearOn( mSettings->mZCL );
    mResults->AddChannelBubblesWillAppearOn( mSettings->mZDA );
}

void ZDIAnalyzer::WorkerThread()
{
    mZCL = GetAnalyzerChannelData( mSettings->mZCL );
    mZDA = GetAnalyzerChannelData( mSettings->mZDA );

    for (;;) {

        bool found_start = false;

        while (!found_start) {
            /* Look for a start condition: ZDA should be high */
            if (mZDA->GetBitState() != BIT_HIGH) {
                mZDA->AdvanceToNextEdge();
                mZCL->AdvanceToAbsPosition( mZDA->GetSampleNumber() );
            }

            mZDA->AdvanceToNextEdge();
            mZCL->AdvanceToAbsPosition( mZDA->GetSampleNumber() );

            /* At the transition, ZCL should be high */
            if (mZCL->GetBitState() != BIT_HIGH) {
                continue;
            }

            found_start = true;
        }

        // ZDA just transitioned low while ZCL was high, start recognising a Frame
        Frame frame;

        // Start the frame where the start condition occurred
        frame.mStartingSampleInclusive = mZCL->GetSampleNumber();

        // read ZDA 7 times to get the address and 1 to get the direction
        U8 address = GetTransferredByte();

        // Read the single-bit separator
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );

        BitState separator1 = mZDA->GetBitState();

        U8 data = GetTransferredByte();

        // Read the single-bit separator
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );

        BitState separator2 = mZDA->GetBitState();

        frame.mEndingSampleInclusive = mZCL->GetSampleNumber();
        frame.mData1 = address;
        frame.mData2 = data;
        frame.mFlags = 0;

        mResults->AddFrame(frame);
        mResults->CommitResults();
        ReportProgress(frame.mEndingSampleInclusive);

    }

}

U8 ZDIAnalyzer::GetTransferredByte()
{
    U8 result = 0;

    for (U8 i = 0; i < 8; i++) {
        // advance to the next falling clock edge
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );

        // advance to the next rising clock edge
        mZCL->AdvanceToNextEdge();
        mZDA->AdvanceToAbsPosition( mZCL->GetSampleNumber() );

        // Sample mZDA
        result = (result << 1) | mZDA->GetBitState();
    }

    return result;
}

bool ZDIAnalyzer::NeedsRerun()
{
	return false;
}

U32 ZDIAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

/* The minimum sample rate needed to get good results.
 *
 * For ZDI at 8MHz this would be 32MHz, but ZDI at 2MHz is only 8MHz. Without knowing how fast the
 * protocol is running, we'll just hope for the best.
 *
 */
U32 ZDIAnalyzer::GetMinimumSampleRateHz()
{
    return 25000;       // The Docs say: if the bit rate is unknown, just return 25000
}

const char* ZDIAnalyzer::GetAnalyzerName() const
{
	return "ZDI";
}

const char* GetAnalyzerName()
{
	return "ZDI";
}

Analyzer* CreateAnalyzer()
{
	return new ZDIAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
