// This #include statement was automatically added by the Spark IDE.
#include "application.h"
#include "OneButton.h"
#include "WlanHandler.h"

SYSTEM_MODE(MANUAL);  // allows controll over the cloud functions

OneButton button(D7, true);
WlanHandler wlan_handler(0);

// start tracking
void click()
{
	Serial.println("Click event");
	wlan_handler.start_ap_scan();
}

// try cloud connect
void doubleclick() {
	Serial.println("Double Click");
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
	button.attachDoubleClick(doubleclick);
	button.attachClick(click);
  button.attachPress(press);
}


void loop() {
  button.tick();
	wlan_handler.tick();
  delay(100);
}
