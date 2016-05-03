#ifndef WlanHandler_h
#define WlanHandler_h

#include "application.h"


extern "C" {
  typedef void (*callbackFunction)(void);
}


class WlanHandler
{
public:
  WlanHandler(int state);  // constructor
  bool scanRunning;
  bool cloudConnected;
  void tick(void);  // update fsm
  void startApScan(void);  // WarBike h4xx0r mode
  void startCloudConnect(void);
  static void wifiScanCallback(WiFiAccessPoint* wap, void* data);

private:
  int _apScanState;
  int _cloudConnectState;
  unsigned long _timestamp;
  void _apScanFsm(void);
  void _cloudConnectFsm(void);
  void _debugWriteLine(String Line);
};

#endif
