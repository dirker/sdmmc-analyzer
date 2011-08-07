#include <AnalyzerHelpers.h>
#include "SDMMCAnalyzerSettings.h"

SDMMCAnalyzerSettings::SDMMCAnalyzerSettings()
:   mClockChannel(UNDEFINED_CHANNEL),
    mCommandChannel(UNDEFINED_CHANNEL)
{
    mClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel);
    mClockChannelInterface->SetTitleAndTooltip("Clock", "Clock (CLK)");
    mClockChannelInterface->SetChannel(mClockChannel);
    
    mCommandChannelInterface.reset(new AnalyzerSettingInterfaceChannel);
    mCommandChannelInterface->SetTitleAndTooltip("Command", "Command (CMD)");
    mCommandChannelInterface->SetChannel(mCommandChannel);
    
    AddInterface(mClockChannelInterface.get());
    AddInterface(mCommandChannelInterface.get());
    
    ClearChannels();
    AddChannel(mClockChannel, "Clock", false);
    AddChannel(mCommandChannel, "Command", false);
}

SDMMCAnalyzerSettings::~SDMMCAnalyzerSettings()
{
}

bool SDMMCAnalyzerSettings::SetSettingsFromInterfaces()
{
    Channel clk = mClockChannelInterface->GetChannel();
    Channel cmd = mCommandChannelInterface->GetChannel();
    
    if (clk == cmd) {
        SetErrorText("Please select different channels for each input");
        return false;
    }
    
    mClockChannel = clk;
    mCommandChannel = cmd;
    
    ClearChannels();
    AddChannel(mClockChannel, "Clock", true);
    AddChannel(mCommandChannel, "Command", true);
    
    return true;
}

void SDMMCAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mClockChannelInterface->SetChannel(mClockChannel);
    mCommandChannelInterface->SetChannel(mCommandChannel);
}

void SDMMCAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive archive;
    
    archive.SetString(settings);
    
    archive >> mClockChannel;
    archive >> mCommandChannel;
    
    ClearChannels();
    AddChannel(mClockChannel, "Clock", true);
    AddChannel(mCommandChannel, "Command", true);
    
    UpdateInterfacesFromSettings();
}

const char *SDMMCAnalyzerSettings::SaveSettings()
{
    SimpleArchive archive;
    
    archive << mClockChannel;
    archive << mCommandChannel;
    
    return SetReturnString(archive.GetString());
}