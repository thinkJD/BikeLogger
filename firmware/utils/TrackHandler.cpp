#include "TrackHandler.h"

File _myFile;

// constructor
TrackHandler::TrackHandler() { 
	_state = 0;
	_timestamp = millis();
	_myFile = File();
}

// Public methodes
void TrackHandler::test_sd(void) {
	// Setup SD Card
	if (SD.begin(chipSelect)) Serial.println("SD Card initialized");
	else Serial.println("SD Card failed or not present");
}

void TrackHandler::tick(void) {
	log("HELLO WORLD");
	_read_from_file();
	switch (_state) {

		case 0:
			break;

		default:
			break;
	}
}

void TrackHandler::log(String message) { 
  _write_to_file(message);
}

void TrackHandler::startNewTrack(void) {
	return;
}

void TrackHandler::finishTrack(void) {
	return;
}

// private methodes

int TrackHandler::_write_to_file(String data) { 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  _myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (_myFile) {
    _myFile.println(data);
    // close the file:
    _myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
	return 0;
}

int TrackHandler::_read_from_file() {
  // re-open the file for reading:
  _myFile = SD.open("test.txt");
  if (_myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (_myFile.available()) {
      Serial.write(_myFile.read());
    }
    // close the file:
    _myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

	return 0;
 }