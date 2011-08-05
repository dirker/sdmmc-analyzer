SD/MMC Analyzer for Logic
=========================

This implements an SD/MMC protocol analyzer for Logic (http://www.saleae.com).

Compiling
---------

In order to compile the code the Saleae Analyzer SDK is needed which can be download via the Saleae Community pages (http://www.saleae.com/community/). The code is tested against version 1.1.9 of the SDK.

Prior to compiling the SDK zip file should be unpacked into the directory containing the cloned sdmmc-analyzer repository, e.g.:

    +-rootdir/
      +-SaleaeAnalyzerSdk-1.1.9/
      +-sdmmc-analyzer/

### Mac OS X

Use the Xode project in xcode4/.

### Linux

Not yet supported

### Windows

Not yet supported

Debugging
---------

### Mac OS X

* Configure Xcode to place build products in locations specified by targets
  * Xcode -> Preferences -> Locations Tab -> Build Location
* Edit SDMMCAnalyzer Scheme to launch Logic upon debugging
  * Product -> Edit Scheme -> Debug -> Info -> Executable
  * Browse for Logic.app (e.g. /Applications/Logic.app)
* Configure Logic to look for the Analyzer Plugin
  * Launch Logic manually
  * Options -> Preferences
  * Under [For Developers], "Search this path for Analyzer Plugins"
  * Browse for the ../sdmmc-analyzer/xcode4/build/Debug directory
  * Click "Save" and close Logic

