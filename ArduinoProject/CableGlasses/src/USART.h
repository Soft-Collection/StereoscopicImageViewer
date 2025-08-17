#ifndef USART_h
#define USART_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TimingQueue.h"

#define SYNC_LEFT_TRANSPARENT 0
#define SYNC_LEFT_OPAQUE 1
#define SYNC_RIGHT_TRANSPARENT 2
#define SYNC_RIGHT_OPAQUE 3

class USART
{
public:
  typedef void (*dOnSync)(void *instance, uint8_t syncType);
  typedef void (*dOnDelayedSync)(void *instance, uint8_t syncType);

public:
  USART();
  static void begin(void *instance, dOnSync onSync, dOnDelayedSync onDelayedSync, uint32_t baudRate);

private:
  static void SetupUSART(uint32_t baudRate);
  static void SetupTimer0();

public:
  static void *msInstance;
  static dOnSync msOnSync;
  static dOnDelayedSync msOnDelayedSync;
  static volatile uint16_t msGlassesTimeOffset;
  static volatile uint32_t msTimer0TicksCounter;
  static volatile uint32_t msLastTimer0TicksCounter;
  static volatile uint16_t msTwoEyesPeriodInTT;
  static TimingQueue msTimingQueue;
};

#endif // USART_h