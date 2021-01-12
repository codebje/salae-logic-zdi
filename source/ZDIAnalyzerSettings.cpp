#include "ZDIAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


ZDIAnalyzerSettings::ZDIAnalyzerSettings()
:	mZCL( UNDEFINED_CHANNEL ),
	mZDA( UNDEFINED_CHANNEL )
{
	mZCLInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mZCLInterface->SetTitleAndTooltip( "ZCL", "Zilog Debug Interface clock" );
	mZCLInterface->SetChannel( mZCL );

	mZDAInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mZDAInterface->SetTitleAndTooltip( "ZDA", "Zilog Debug Interface data" );
	mZDAInterface->SetChannel( mZDA );

	AddInterface( mZCLInterface.get() );
	AddInterface( mZDAInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mZCL, "ZCL", false );
	AddChannel( mZDA, "ZDA", false );
}

ZDIAnalyzerSettings::~ZDIAnalyzerSettings()
{
}

bool ZDIAnalyzerSettings::SetSettingsFromInterfaces()
{
	if( mZCLInterface->GetChannel() == UNDEFINED_CHANNEL )
	{
		SetErrorText( "Please select an input for channel 1." );
		return false;
	}

	if( mZDAInterface->GetChannel() == UNDEFINED_CHANNEL )
	{
		SetErrorText( "Please select an input for channel 2." );
		return false;
	}

	mZCL = mZCLInterface->GetChannel();
	mZDA = mZDAInterface->GetChannel();

	if( mZCL == mZDA )
	{
		SetErrorText( "Please select different inputs for the channels." );
		return false;
	}

	ClearChannels();
	AddChannel( mZCL, "ZCL", false );
	AddChannel( mZDA, "ZDA", false );

	return true;
}

void ZDIAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mZCLInterface->SetChannel( mZCL );
	mZDAInterface->SetChannel( mZDA );
}

void ZDIAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mZCL;
	text_archive >> mZDA;

	ClearChannels();
	AddChannel( mZCL, "ZCL", false );
	AddChannel( mZDA, "ZDA", false );

	UpdateInterfacesFromSettings();
}

const char* ZDIAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mZCL;
	text_archive << mZDA;

	return SetReturnString( text_archive.GetString() );
}
