// This #include statement was automatically added by the Spark IDE.
#include "application.h"
#include "OneButton.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "WlanHandler.h"
#include "SD.h"
#include <math.h>
#include "Adafruit_GPS.h"

SYSTEM_MODE(MANUAL);  // allows controll over the cloud functions

OneButton button(D7, true);
WlanHandler wlan_handler(0);
Adafruit_BME280 bme;  // i2c hardware port

// GPS
#define mySerial Serial1
#define APP_VERSION 10
#define GPSECHO  false  // Serial echo of raw gps data ... slow
Adafruit_GPS GPS(&mySerial);

// SD Card
const uint8_t chipSelect = A2;
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin = A3;

char dataString[100];
char sdata[10];
char logLine[300];

// start tracking
void click()
{
	Serial.println("Click event");
	wlan_handler.start_ap_scan();
}

// try cloud connect
void doubleclick() {
	Serial.println("Double Click");
	Serial.println("Temperature:" + String(bme.readTemperature()) + " *C");
	Serial.println("Humidity:" + String(bme.readHumidity()) + " %");
	Serial.println("Pressure:" + String(bme.readPressure()) + " hp");
}

// Enter DFU mode. DFU mode is active until system reset
void press() {
	Serial.println("System: Enter DFU mode");
	delay(200);
	System.dfu(true);
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Serial Connected");

	// setup environmental sensor
	if (bme.begin()) Serial.println("Env Sensor initialized");
 	else Serial.println("Env Sensor not found");

	// Setup SD Card
	if (SD.begin(chipSelect)) Serial.println("SD Card initialized");
	else Serial.println("SD Card failed or not present");

	// Setup GPS Module
	GPS.begin(9600);  // Initialize GPS lib
	mySerial.begin(9600);  // Hardware serial port
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // RMC and GGA Data
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	GPS.sendCommand(PGCMD_NOANTENNA);  // no updates about the antenna status
	delay(1000);  // some init time for the gps chipset
	mySerial.println(PMTK_Q_RELEASE);  // Ask for firmware version

	// init all the other stuff
	button.attachDoubleClick(doubleclick);
	button.attachClick(click);
	button.attachPress(press);
}

void loop() {
  button.tick();
	wlan_handler.tick();

	// Get and parse last GPS sentence. If it fails, wait for the next one
	while (mySerial.available() > 0) char c = GPS.read();
	if (GPS.newNMEAreceived())
		if (!GPS.parse(GPS.lastNMEA())) return;
}
