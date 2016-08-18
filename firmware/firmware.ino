// This #include statement was automatically added by the Spark IDE.
#include "application.h"
#include "OneButton.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "WlanHandler.h"
#include "TrackHandler.h"
#include "SD.h"
#include <math.h>
#include "Adafruit_GPS.h"

SYSTEM_MODE(MANUAL);  // allows controll over the cloud functions

OneButton button(D7, true);
WlanHandler wlan_handler(0);
Adafruit_BME280 bme;  // i2c hardware port
TrackHandler track_handler = TrackHandler();

// GPS
#define APP_VERSION 10
#define GPSECHO  false  // Serial echo of raw gps data ... slow
#define GPS_ENABLE D5
#define GPS_FIXSIGNAL D4
Adafruit_GPS GPS(100);

char dataString[100];
char sdata[10];
char logLine[300];

void printEnvironmentalSensorData() {
  _debugWriteLine("Temperature: " + String(bme.readTemperature()) + " *C");
	_debugWriteLine("Humidity: " + String(bme.readHumidity()) + " %");
	_debugWriteLine("Pressure: " + String(bme.readPressure()) + " hp");
}

void printGpsLastNema() {
	_debugWriteLine("Fix Quality: " + String(GPS.fixquality));
  _debugWriteLine("Time: " + String(GPS.hour) +  ':' + String(GPS.minute) + ':' + String(GPS.seconds) + '.' + String(GPS.milliseconds));
  _debugWriteLine("Date: " + GPS.day + '.' + GPS.month + '.' + String(GPS.year));
	_debugWriteLine("Fix: " + String(GPS.fix) + "Quality: " + String(GPS.fixquality)); 
	if (GPS.fix) {
		_debugWriteLine("Sattelites: " + String(GPS.satellites));
		_debugWriteLine("Location: " + String(GPS.latitude) + String(GPS.lat) + ", " + String(GPS.longitude) + String(GPS.lon));
		_debugWriteLine("Speed (knots): " + String(GPS.speed) + " Altitude: " + String(GPS.altitude));
	}
	else {
		_debugWriteLine("No GPS fix");
	}
}

void click() {
	Serial.println("Click event");
	//wlan_handler.start_ap_scan();
	wlan_handler.startCloudConnect();
}

void doubleclick() {
	printEnvironmentalSensorData();
	printGpsLastNema();
}

// Enter DFU mode. DFU mode is active until system reset
void press() {
	Serial.println("System: Enter DFU mode");
	delay(200);
	System.dfu(true);
}

void _debugWriteLine(String Line) {
	if (wlan_handler.cloudConnected) {
		Particle.publish("Debug", "Main: " + Line);
	}
	Serial.println("Main: " + Line);
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Serial Connected");

	// setup environmental sensor
	if (bme.begin()) Serial.println("Env Sensor initialized");
 	else Serial.println("Env Sensor not found");

	// Setup GPS Module
	pinMode(GPS_ENABLE, OUTPUT);
	digitalWrite(GPS_ENABLE, HIGH);  // enable GPS Module
	Serial1.begin(9600);  // Initialize hw serial
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // RMC and GGA Data
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	GPS.sendCommand(PGCMD_NOANTENNA);  // no updates about the antenna status
	delay(1000);  // some init time for the gps chipset
	Serial.println("GPS initialized");

	track_handler.test_sd();

	// all the other stuff
	button.attachDoubleClick(doubleclick);
	button.attachClick(click);
	button.attachPress(press);
}

// called from application loot if serial data in buffer
void serialEvent1() {
	char c = Serial1.read();
  GPS.newChar(c);
  // debug
	//Serial.print(c);
}

void loop() {
  button.tick();
	wlan_handler.tick();


	// Get and parse last GPS sentence.
	if (GPS.newNMEAreceived()) {
		if (!GPS.parse(GPS.lastNMEA())) {
			//printGpsLastNema();
		}
	}
}
