#ifndef __SDMMC_ANALYZER_SETTINGS
#define __SDMMC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SDMMCAnalyzerSettings : public AnalyzerSettings
{
public:
    SDMMCAnalyzerSettings();
    virtual ~SDMMCAnalyzerSettings();
    
    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();
    
    Channel mClockChannel;
    Channel mCommandChannel;
    
protected:
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mClockChannelInterface;
    std::auto_ptr<AnalyzerSettingInterfaceChannel> mCommandChannelInterface;
};

#endif