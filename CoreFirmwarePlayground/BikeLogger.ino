// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GPS.h"
#include <math.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "application.h"
#include "OneButton.h"
#include "SD.h"

// GPS
#define mySerial Serial1
#define APP_VERSION 10
#define GPSECHO  false  // Serial echo of raw gps data ... slow
Adafruit_GPS GPS(&mySerial);

byte bufferSize = 64;
byte bufferIndex = 0;
char buffer[65];
char c;

// SD Card
// SOFTWARE SPI pin configuration - modify as required
// The default pins are the same as HARDWARE SPI
const uint8_t chipSelect = A2;    // Also used for HARDWARE SPI setup
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin = A3;

char dataString[100];
char sdata[10];
char logLine[300];

// Environmental sensor
Adafruit_BME280 bme; // I2C

OneButton button(D7, true);

// application
uint32_t timer;
volatile bool tracking = true;
volatile bool writing = false;

//SYSTEM_MODE(MANUAL);  // allows controll over the cloud functions
void setup() {
//  Spark.connect();
//  button.attachPress(longClickHandler);
//  button.attachDoubleClick(doubleClickHandler);
//  button.attachClick(singleClickHandler);
  Serial.begin(115200);
  // Setup SD Card
  if (!SD.begin(chipSelect))
    Spark.publish("SD", "Card failed, or not present", 60, PRIVATE);
  else Spark.publish("SD", "Cart initialized :)", 60, PRIVATE);

  // Setup GPS Module
  GPS.begin(9600);  // Initialize GPS lib
  mySerial.begin(9600);  // Hardware serial port
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // RMC and GGA Data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  GPS.sendCommand(PGCMD_NOANTENNA);  // no updates about the antenna status
  delay(1000);  // some init time for the gps chipset
  mySerial.println(PMTK_Q_RELEASE);  // Ask for firmware version

  // Setup Environmental sensor
  if (bme.begin()) Spark.publish("BME280", "Sensor initialized", 60, PRIVATE);
  else Spark.publish("BME280", "No Senosor found", 60, PRIVATE);

  // other
  timer = millis();
  pinMode(D4, INPUT_PULLDOWN);
  attachInterrupt(D4, TrackUpdate, RISING);  // D4 = GPS Pulse per sec if fix
  Spark.publish("App", "{ status: \"started up! "+String(APP_VERSION)+"\"}", 60, PRIVATE );
  IPAddress myIP = WiFi.localIP();
  Spark.publish("IP", String(myIP[0]) + "."
    + String(myIP[1]) + "."
    + String(myIP[2]) + "."
    + String(myIP[3]), 60, PRIVATE );
}


void loop() {
  // reset timer at overflow
  if (timer > millis())  timer = millis();
  // opdate button object
//  button.tick();

//  if (Spark.connected()) {
//    Spark.process();
//  }

  // execute every 10 sec
  if (millis() - timer > 10000) {
    timer = millis();
    publishStatus();
  }

  // Get and parse last GPS sentence. If it fails, wait for the next one
  while (mySerial.available() > 0) char c = GPS.read();
  if (GPS.newNMEAreceived())
    if (!GPS.parse(GPS.lastNMEA())) return;
}

void startNewTrack() {
  // get file count
  // create new file
  // write first record
}

void publishStatus() {
  if (GPS.fix) {
    Spark.publish("GPS",
      "{ quality: " + String(GPS.fixquality)
      + ", satelites: " + String((int)GPS.satellites)
      + " }", 60, PRIVATE );

    Spark.publish("GPS",
      "{lat: " + String(convertDegMinToDecDeg(GPS.latitude))
      + ", lon: -" + String(convertDegMinToDecDeg(GPS.longitude))
      + ", a: " + String(GPS.altitude)
      + ", s: " + String(GPS.speed)
      + " }", 60, PRIVATE );
    }
  else {
    Spark.publish("GPS", "{ Error: No GPS Fix }", 60, PRIVATE );
    Spark.publish("GPS", "{ satellites: " + String(GPS.satellites) + " }", 60, PRIVATE);
  }

  Spark.publish("ENV",
    "{Temperature: " + String(bme.readTemperature()) + " *C"
    + ", Humidity: " + String(bme.readHumidity()) + " %"
    + ", Pressure: " + String(bme.readPressure()) + " hp"
    +  "}", 60, PRIVATE );
}

void TrackUpdate() {

  if (tracking) {
    noInterrupts();
    File trackLog = SD.open("trackLog.log", FILE_WRITE);
    if (trackLog) {
      // Timestamp
      trackLog.print(GPS.day, DEC); trackLog.print('.');
      trackLog.print(GPS.month, DEC); trackLog.print('.');
      trackLog.print(GPS.year, DEC); trackLog.print(' ');
      trackLog.print(GPS.hour, DEC); trackLog.print(':');
      trackLog.print(GPS.minute, DEC); trackLog.print(':');
      trackLog.print(GPS.seconds, DEC); trackLog.print('.');
      trackLog.print(GPS.milliseconds); trackLog.print(';');
      // GPS data
      trackLog.print(GPS.fixquality, DEC); trackLog.print(';');
      trackLog.print(GPS.latitude, 4); trackLog.print(GPS.lat);
      trackLog.print(GPS.longitude, 4); trackLog.print(GPS.lon);
      trackLog.print(GPS.speed); trackLog.print(';');
      trackLog.print(GPS.altitude); trackLog.print(';');
      trackLog.print((int)GPS.satellites); trackLog.print(';');
      // Environmelntal data
      trackLog.print(bme.readTemperature()); trackLog.print(';');
      trackLog.print(bme.readHumidity()); trackLog.print(';');
      trackLog.print(bme.readPressure()); trackLog.println(';');
      trackLog.close();
      interrupts();
    }
  }
}

// Event Handling
// single click: pause tracking
// double click: tracking on / off
//               off -> on: start new track
//               on -> off: save track
// long Click: connect to cloud service if possible
void singleClickHandler() {
    //track = !track;
}
void doubleClickHandler() {
}
void longClickHandler() {
}

// Cloud functions
void StartTracking() { }
void StopTracking() { }
void GoSleep() { }

// Utils
//http://arduinodev.woofex.net/2013/02/06/adafruit_gps_forma/
double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
  return decDeg;
}
