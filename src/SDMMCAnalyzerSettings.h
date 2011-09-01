#ifndef __SDMMC_ANALYZER_SETTINGS
#define __SDMMC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

enum SDMMCProtocol {
	PROTOCOL_MMC,
	PROTOCOL_SD,
};

enum SDMMCSampleEdge {
	SAMPLE_EDGE_RISING,
	SAMPLE_EDGE_FALLING
};

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
	enum SDMMCProtocol mProtocol;
	enum SDMMCSampleEdge mSampleEdge;

protected:
	std::auto_ptr<AnalyzerSettingInterfaceChannel> mClockChannelInterface;
	std::auto_ptr<AnalyzerSettingInterfaceChannel> mCommandChannelInterface;
	std::auto_ptr<AnalyzerSettingInterfaceNumberList> mProtocolInterface;
	std::auto_ptr<AnalyzerSettingInterfaceNumberList> mSampleEdgeInterface;
};

#endif
