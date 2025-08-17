#ifndef StereoGlassesController_h
#define StereoGlassesController_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define LOW 0
#define HIGH 1
//-------------------------------------------------------
#define PIN_5 PD5
#define PIN_6 PD6
#define PIN_7 PD7
//-------------------------------------------------------
#define LEFT_SHUTTER_PIN PIN_5
#define COMMON_SHUTTER_PIN PIN_6
#define RIGHT_SHUTTER_PIN PIN_7
//-------------------------------------------------------
#define TRANSPARENT 0
#define OPAQUE 1
//-------------------------------------------------------
#define TICKS_TO_SET_TRANSPARENT (F_CPU / 1024 / 30)

class StereoGlassesController
{
public:
  StereoGlassesController();
  static void begin();
  static void digitalWrite(uint8_t leftPinState, uint8_t commonPinState, uint8_t rightPinState);
  static void setShutterState(bool isLeft, uint8_t state);
  static void heartBeat();

private:
  static void SetupPins();
  static void SetupTimer2();

public:
  static volatile uint32_t msTimer2TicksCounter;
  static volatile uint8_t msLeftShutterState;
  static volatile uint8_t msRightShutterState;
};

#endif // StereoGlassesController_h