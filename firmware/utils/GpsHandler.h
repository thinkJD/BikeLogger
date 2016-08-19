#ifndef GpsHandler_h
#define GpsHandler_h

#include "application.h"

// SD Card
const uint8_t chipSelect = A2;
const uint8_t mosiPin = A5;
const uint8_t misoPin = A4;
const uint8_t clockPin = A3;

class TrackHandler {
	public:
  		TrackHandler();  // constructor
  		void test_sd(void);
  		void tick(void);  // update fsm
  		void startNewTrack(void);
  		void finishTrack(void);
      void log(String message);
	private:
  		int _state;
  		unsigned long _timestamp;
  		void _debugWriteLine(String Line);
      int _write_to_file(String message);
      int _read_from_file();
};

#endif
