#ifndef ZDI_ANALYZER_SETTINGS
#define ZDI_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class ZDIAnalyzerSettings : public AnalyzerSettings
{
public:
	ZDIAnalyzerSettings();
	virtual ~ZDIAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mZCL;
	Channel mZDA;

protected:
	std::unique_ptr< AnalyzerSettingInterfaceChannel >	mZCLInterface;
	std::unique_ptr< AnalyzerSettingInterfaceChannel >	mZDAInterface;
};

#endif //ZDI_ANALYZER_SETTINGS
