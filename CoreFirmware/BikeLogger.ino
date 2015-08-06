// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GPS.h"
#include <math.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// GPS
#define mySerial Serial1
#define APP_VERSION 10
#define GPSECHO  false  // Serial echo of raw gps data ... slow
Adafruit_GPS GPS(&mySerial);

byte bufferSize = 64;
byte bufferIndex = 0;
char buffer[65];
char c;

// Environmental sensor
Adafruit_BME280 bme; // I2C

// application
uint32_t timer;


void setup() {
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
  Spark.publish("App", "{ status: \"started up! "+String(APP_VERSION)+"\"}", 60, PRIVATE );
  IPAddress myIP = WiFi.localIP();
  Spark.publish("IP", String(myIP[0]) + "."
    + String(myIP[1]) + "."
    + String(myIP[2]) + "."
    + String(myIP[3]), 60, PRIVATE );
}


void loop() {
  while (mySerial.available() > 0) char c = GPS.read();

  // Get and parse last GPS sentence. If it fails, wait for the next one
  if (GPS.newNMEAreceived())
    if (!GPS.parse(GPS.lastNMEA())) return;

  // approximately every 2 seconds or so, print out the current stats
  if (timer > millis())  timer = millis();
  if (millis() - timer > 10000) {
    timer = millis();

    if (GPS.fix) {
      Spark.publish("GPS",
          + "{ quality: " + String(GPS.fixquality)
          + ", satelites: " + String((int)GPS.satellites)
          + " }",
        60, PRIVATE );

      Spark.publish("GPS",
        "{lat: " + String(convertDegMinToDecDeg(GPS.latitude))
          + ", lon: -" + String(convertDegMinToDecDeg(GPS.longitude))
          + ", a: " + String(GPS.altitude)
          + ", s: " + String(GPS.speed)
          + " }",
        60, PRIVATE );
    }
    else {
      Spark.publish("GPS", "{ Error: No GPS Fix }", 60, PRIVATE );
      Spark.publish("GPS", "{Status: }", 60, PRIVATE);
    }

    Spark.publish("ENV",
      "{Temperature: " + String(bme.readTemperature()) + " *C"
        + ", Humidity: " + String(bme.readHumidity()) + " %"
        + ", Pressure: " + String(bme.readPressure()) + " hp"
        +  "}",
      60, PRIVATE );
  }
}

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
