#ifndef TrackHandler_h
#define TrackHandler_h

#include "application.h"
#include "GPS.h"

extern "C" {
  typedef void (*callbackFunction)(void);
}

class TrackHandler {
public:
  TrackHandler(int state);  // constructor
  void tick(void);  // update fsm
  void startNewTrack(void);
  void finishTrack(void);
  
private:
  int _state;
  unsigned long _timestamp;
  void _debugWriteLine(String Line);
};

#endif
