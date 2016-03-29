#include "WlanHandler.h"

WlanHandler::WlanHandler(int state)
{
  _state = 0;
  _timestamp = millis();
}

void WlanHandler::tick(void)
{
  switch (_state) {
    case 1:  // turn wifi on
      _debugWriteLine("Turn WiFi on");
      WiFi.on();
      delay(100);  // blocking -- dont do this
      _state++;
      break;

    case 2:  // start scan
      _debugWriteLine("Start Scan");
      WiFi.scan(WlanHandler::_wifi_scan_callback);
      _state++;
      break;

    case 3:
      _debugWriteLine("Scan finished");
      _state = 0;
      break;

    default:
      break;
  }
}

void WlanHandler::start_ap_scan(void)
{
  if (_state == 0) _state++;;
}

// EXAMPLE using a callback
void WlanHandler::_wifi_scan_callback(WiFiAccessPoint* wap, void* data)
{
    WiFiAccessPoint& ap = *wap;
    Serial.print("SSID: ");
    Serial.print(ap.ssid);
    Serial.print("\t      Security: ");
    Serial.print(ap.security);
    Serial.print("\tChannel: ");
    Serial.print(ap.channel);
    Serial.print("\tRSSI: ");
    Serial.println(ap.rssi);
}

void WlanHandler::_debugWriteLine(String Line)
{
  Serial.println("WlanHandler: " + Line);
}
