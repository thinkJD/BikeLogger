#include "WlanHandler.h"
/*
  Power up / down wifi
  connect to cloud if connected send updates
  close cloud connection
  start ap scan
*/

WlanHandler::WlanHandler(int state) {
  _apScanState = 0;
  _cloudConnectState = 0;
  _timestamp = millis();
}

void WlanHandler::tick(void) {
  Particle.process();  // we are in manual mode
  _apScanFsm();
  _cloudConnectFsm();
}

void WlanHandler::startApScan(void) {
  if (!scanRunning) {
    _apScanState = 1;
  }
}

void WlanHandler::startCloudConnect(void) {
  if (!cloudConnected && _cloudConnectState == 0) {
    _cloudConnectState = 1;
  }
}

void WlanHandler::_apScanFsm(void) {
  switch (_apScanState) {
    case 1:  // turn wifi on
      _debugWriteLine("Turn WiFi on");
      scanRunning = true;
      WiFi.on();
      _apScanState++;
      break;

    case 2:  // start scan
      _debugWriteLine("Start Scan");
      WiFi.scan(WlanHandler::wifiScanCallback);
      _apScanState++;
      break;

    case 3:
      _debugWriteLine("Scan finished");
      _apScanState = 0;
      scanRunning = false;
      break;

    default:
      break;
  }
}

void WlanHandler::_cloudConnectFsm(void) {
  switch (_cloudConnectState) {
    case 1:
      WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
      _cloudConnectState++;
      break;

    case 2:
      // set credentials if necessary then try a reconnect
      if (! WiFi.hasCredentials()) {
        _debugWriteLine("No WiFi Credentials found. Try reset.");
        WiFi.setCredentials("BrumBrumFeuerwehr", "MuffinLand1337");
        WiFi.connect();
      }
      _cloudConnectState++;
      break;

      case 3:
        if (WiFi.ready()) {
          _cloudConnectState++;
        }
        break;

      case 4:
        Particle.connect();
        _cloudConnectState++;
        break;

      case 5:
        if (Particle.connected()) {
          _debugWriteLine("Particle Connected");
          cloudConnected = true;
          _cloudConnectState++;
        }
        break;

      case 6:
        //_debugWriteLine("Cloud Connected");
        break;
  }
}

void WlanHandler::wifiScanCallback(WiFiAccessPoint* wap, void* data) {
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

void WlanHandler::_debugWriteLine(String Line) {
  if (Particle.connected()) Particle.publish("Debug", "WlanHandler: "  + Line);
  Serial.println("WlanHandler: " + Line);
}