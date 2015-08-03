// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GPS.h"
#include <math.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#define mySerial Serial1
#define APP_VERSION 10
#define GPSECHO  false  // Serial echo of raw gps data

Adafruit_BME280 bme; // I2C
Adafruit_GPS GPS(&mySerial);
boolean usingInterrupt = true;  // Use interrupf for serial data receive

byte bufferSize = 64;
byte bufferIndex = 0;
char buffer[65];
char c;

uint32_t timer;

void setup() {
  // Hardware Serial port for GPS module
  GPS.begin(9600);
  mySerial.begin(9600);
  // Own Serial Port for comunication
  Serial.begin(115200);

  // Environmental sensor test
  if (bme.begin()) Spark.publish("BME380", "Nice!", 60, PRIVATE);
  else Spark.publish("BME380", "Fuck!", 60, PRIVATE);
  /*int retryCounter = 0;
  while (1) {
    if (!bme.begin()) {
      retryCounter++;
      Spark.publish("BME380", "{ status: \"error: Could not find sensor, check wiring! " + String(APP_VERSION) + "\"}", 60, PRIVATE );
      delay(1000);
      if (retryCounter > 5) break;
    }
    else {
      Spark.publish("BME280", "{ status: \"started up! " + String(APP_VERSION) + "\"}", 60, PRIVATE );
      break;
    }
  }*/

  // GPS Module
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_NOANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
  timer = millis();
  Spark.publish("GPS", "{ status: \"started up! "+String(APP_VERSION)+"\"}", 60, PRIVATE );

  IPAddress myIP = WiFi.localIP();
  Spark.publish("MY_IP", String(myIP[0]) + "." + String(myIP[1]) + "." + String(myIP[2]) + "." + String(myIP[3]),
    60, PRIVATE );
}

void loop() {

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   {
      // this also sets the newNMEAreceived() flag to false
      if (millis() - timer > 10000) {

        Spark.publish("GPS", "{ last: \""+String(GPS.lastNMEA())+"\"}", 60, PRIVATE );
        Spark.publish("GPS", "{ error: \"failed to parse\"}", 60, PRIVATE );
      }
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 10000) {
    timer = millis(); // reset the timer
    
    // Environment
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure());
    Serial.println(" Pa");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    Serial.println();

    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);

    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4);
    Serial.print(GPS.lat);
    Serial.print(", ");

    Serial.print(GPS.longitude, 4);
    Serial.println(GPS.lon);

    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    Serial.print("Angle: "); Serial.println(GPS.angle);
    Serial.print("Altitude: "); Serial.println(GPS.altitude);
    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

    Spark.publish("GPS",
      "{lat: " + String(convertDegMinToDecDeg(GPS.latitude))
      + ", lon: -" + String(convertDegMinToDecDeg(GPS.longitude))
      + ", a: " + String(GPS.altitude)
      + " }",
      60, PRIVATE );

    Spark.publish("GPS",
      + "{ q: " + String(GPS.fixquality)
      + ", s: " + String((int)GPS.satellites)
      + " }",
      60, PRIVATE );

    Spark.publish("GPS_RAW", String(GPS.lastNMEA()), 60, PRIVATE );
  }
}


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
