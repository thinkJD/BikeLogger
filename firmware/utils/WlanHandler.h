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
  void tick(void);  // update fsm
  void start_ap_scan(void);  // WarBike h4xx0r mode
  static void _wifi_scan_callback(WiFiAccessPoint* wap, void* data);

private:
  int _state;
  unsigned long _timestamp;
  void _debugWriteLine(String Line);

};

#endif
