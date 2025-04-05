#include <Arduino.h>

//--------------------------------------------------------------------------------
#define LEFT_SHUTTER_PIN 6
#define COMMON_SHUTTER_PIN 7
#define RIGHT_SHUTTER_PIN 8

typedef struct
{
  volatile uint8_t Left;
  volatile uint8_t Common;
  volatile uint8_t Right;
} Output;
typedef struct
{
  volatile uint32_t Counter;
  volatile uint32_t Value;
} Duration;
typedef struct
{
  volatile uint32_t Begin;
  volatile uint32_t End;
} Transparancy;

volatile uint32_t timeCounter = 0;
//--------------------------------------------
Output output = {
  LOW,
};
//--------------------------------------------
Duration frameDuration = {
  0,
};
//--------------------------------------------
Transparancy leftTransparency = {
  0,
};
Transparancy rightTransparency = {
  0,
};
//--------------------------------------------
volatile uint32_t glassesTimeOffset = 0;
volatile uint32_t transparentTimePercent = 30;

void SetupSerial();
void WaitForLRSyncFromSerial();
void SetupExternalInt();
void SetupTimer1();

void setup() {
  pinMode(LEFT_SHUTTER_PIN, OUTPUT);
  pinMode(COMMON_SHUTTER_PIN, OUTPUT);
  pinMode(RIGHT_SHUTTER_PIN, OUTPUT);
  //-------------------------------------
  digitalWrite(LEFT_SHUTTER_PIN, LOW);
  digitalWrite(COMMON_SHUTTER_PIN, LOW);
  digitalWrite(RIGHT_SHUTTER_PIN, LOW);
  //-------------------------------------
  SetupSerial();
  SetupTimer1();
}

void loop() {
  WaitForLRSyncFromSerial();
}

void SetupSerial() {
  Serial.begin(115200);
  Serial.flush();
}

void WaitForLRSyncFromSerial() {
  uint8_t inByte;
  while (!Serial.available()) {}
  while (Serial.available()) {
    inByte = (uint8_t)Serial.read();
  }
  //-------------------------------------------------------
  if ((inByte == 0x4C) || (inByte == 0x52)) {
    uint32_t timeForTransparencyBegin = timeCounter + (frameDuration.Value * (50 - (transparentTimePercent / 2)) / 100);
    uint32_t timeForTransparencyEnd = timeCounter + (frameDuration.Value * (50 + (transparentTimePercent / 2)) / 100);
    if (inByte == 0x4C)  //Left Transparent
    {
      leftTransparency.Begin = timeForTransparencyBegin;
      leftTransparency.End = timeForTransparencyEnd;
      rightTransparency.Begin = timeForTransparencyBegin - frameDuration.Value;
      rightTransparency.End = timeForTransparencyEnd - frameDuration.Value;
    } else if (inByte == 0x52)  //Right Transparent
    {
      leftTransparency.Begin = timeForTransparencyBegin - frameDuration.Value;
      leftTransparency.End = timeForTransparencyEnd - frameDuration.Value;
      rightTransparency.Begin = timeForTransparencyBegin;
      rightTransparency.End = timeForTransparencyEnd;
    }
    frameDuration.Value = frameDuration.Counter;
    frameDuration.Counter = 0;
  } else if ((inByte >= 100) && (inByte <= 199)) {
    glassesTimeOffset = inByte - 100;
  } else if ((inByte >= 200) && (inByte <= 299)) {
    transparentTimePercent = inByte - 200;
  }
}

void SetupTimer1() {
  // Set digital pin 8 (PB0) as output
  DDRB |= (1 << DDB0);
  // Disable interrupts globally
  cli();
  // Configure Timer1
  TCCR1A = 0;  // Normal mode (CTC set by WGM12)
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);  // CTC mode
  //TCCR1B |= (1 << CS11);   // Prescaler = 16
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler = 1024
  // Calculate and set compare value for EACH_EYE_FREQUENCY_IN_HZ Hz
  // Compare value = (Clock frequency / (Prescaler * Frequency)) - 1
  // Clock frequency = 16,000,000 Hz, Prescaler = 1024, Frequency in Hz
  // OCR1A = (16000000 / (1024 * Frequency in Hz)) - 1;
  OCR1A = 1;  //3906.25 Hz
  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  // Enable interrupts globally
  sei();
}

// Interrupt Service Routine for Timer1 compare match
ISR(TIMER1_COMPA_vect) {
  output.Common = !output.Common;
  //----------------------------------------------------------------
  uint32_t timeCounterWithOffset = timeCounter - glassesTimeOffset;
  output.Left = ((leftTransparency.Begin < timeCounterWithOffset) && (timeCounterWithOffset < leftTransparency.End)) ? output.Common : !output.Common;
  output.Right = ((rightTransparency.Begin < timeCounterWithOffset) && (timeCounterWithOffset < rightTransparency.End)) ? output.Common : !output.Common;
  if (frameDuration.Counter > 2 * frameDuration.Value) {
    output.Left = output.Common;
    output.Right = output.Common;
  }
  //----------------------------------------------------------------
  digitalWrite(LEFT_SHUTTER_PIN, output.Left);
  digitalWrite(COMMON_SHUTTER_PIN, output.Common);
  digitalWrite(RIGHT_SHUTTER_PIN, output.Right);
  //----------------------------------------------------------------
  timeCounter++;
  frameDuration.Counter++;
}
