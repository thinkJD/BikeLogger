#include "TrackHandler.h"

TrackHandler::TrackHandler(int state) {
	_state = 0;
	_timestamp = millis();
}

TrackHandler::Tick(void) {
	switch (_state) {

		case 0:
			break;

		default:
			break;
	}
}

TrackHandler::startNewTrack(void) {
	return;
}

TrackHandler::finishTrack(void) {
	return;
}

