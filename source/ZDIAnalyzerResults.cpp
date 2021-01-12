#include "ZDIAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "ZDIAnalyzer.h"
#include "ZDIAnalyzerSettings.h"
#include <iostream>
#include <fstream>

ZDIAnalyzerResults::ZDIAnalyzerResults( ZDIAnalyzer* analyzer, ZDIAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

ZDIAnalyzerResults::~ZDIAnalyzerResults()
{
}

void ZDIAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char reg_str[20];
        char data_str[20];

	AnalyzerHelpers::GetNumberString( frame.mData1 >> 1, display_base, 8, reg_str, std::size(reg_str) );
	AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, data_str, std::size(data_str) );

        AddResultString( frame.mData1 & 1 ? "R" : "W" );
        auto mid = std::string(reg_str) + (frame.mData1 & 1 ? "<=" : "=>") + data_str;
        auto big = std::string(reg_str) + (frame.mData1 & 1 ? " <= " : " => ") + data_str;
        AddResultString ( mid.c_str() );
        AddResultString ( big.c_str() );
}

void ZDIAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void ZDIAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
    ClearResultStrings();
    AddResultString( "not supported" );
#if 0
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddTabularText( number_str );
#endif
#endif
}

void ZDIAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}

void ZDIAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}
