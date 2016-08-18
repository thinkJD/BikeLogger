#include "TrackHandler.h"


TrackHandler::TrackHandler() {
	_state = 0;
	_timestamp = millis();
}

void TrackHandler::test_sd(void) {
	// Setup SD Card
	if (SD.begin(chipSelect)) Serial.println("SD Card initialized");
	else Serial.println("SD Card failed or not present");
}

void TrackHandler::tick(void) {
	switch (_state) {

		case 0:
			break;

		default:
			break;
	}
}

void TrackHandler::startNewTrack(void) {
	return;
}

void TrackHandler::finishTrack(void) {
	return;
}

