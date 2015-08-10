// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GPS.h"
#include <math.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "application.h"
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


// Environmental sensor
Adafruit_BME280 bme; // I2C

// application
uint32_t timer;


void setup() {
  Serial.begin(115200);
  // SD Card
  // Wait for key, for debug reasons
  while (!Serial.available()) SPARK_WLAN_Loop();

  Serial.print("Initializing SD card...");


  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) Serial.println("Card failed, or not present");
  else Serial.println("Cart initialized :)");


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

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println("Ich bin das Log");
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }


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
