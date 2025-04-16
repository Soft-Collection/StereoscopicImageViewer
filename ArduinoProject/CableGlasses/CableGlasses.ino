#include <Arduino.h>

//--------------------------------------------------------------------------------
#define LEFT_SHUTTER_PIN 6
#define COMMON_SHUTTER_PIN 7
#define RIGHT_SHUTTER_PIN 8

#define COMMAND_TYPE_SYNC 0
#define COMMAND_TYPE_GLASSES_TIME_OFFSET 1
#define COMMAND_TYPE_TRANSPARENT_TIME_PERCENT 2

typedef struct
{
  volatile uint8_t Left;
  volatile uint8_t Common;
  volatile uint8_t Right;
} Output;
typedef struct
{
  volatile uint32_t Begin;
  volatile uint32_t End;
} Transparency;
typedef struct
{
  Transparency Left;
  Transparency Right;
} Shutters;

Output output = {
  LOW,
};
Shutters shutters = {
  0.0,
};
//-------------------------------------------------------
volatile uint8_t glassesTimeOffset = (uint8_t)(-1);
volatile uint8_t transparentTimePercent = (uint8_t)(-1);
//-------------------------------------------------------
volatile uint32_t lastSyncTimeInUS = 0;
volatile uint32_t insideTwoEyeCycleInUS = 0;
//-------------------------------------------------------
volatile uint32_t twoEyePeriodInUS = 0;
volatile uint32_t glassesTimeOffsetInUS = 0;

void UpdateVariables();
uint8_t GetTransparencyTime(Transparency transparency, uint8_t common);
void SetupSerial();
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
  uint8_t inByte;
  //-------------------------------
  while (!Serial.available());
  inByte = (uint8_t)Serial.read();
  if (inByte != 0x54) return;
  //-------------------------------
  while (!Serial.available());
  inByte = (uint8_t)Serial.read();
  if (inByte != 0xED) return;
  //-------------------------------
  while (!Serial.available());
  uint8_t command_type = (uint8_t)Serial.read();
  if (command_type == COMMAND_TYPE_SYNC) 
  {
    while (!Serial.available());
    uint8_t checkSum = (uint8_t)Serial.read();
    if (checkSum != (uint8_t)(0x54 + 0xED + command_type)) return;
    //-------------------------------------------------------------
    uint32_t timeInUS = micros();
    twoEyePeriodInUS = timeInUS - lastSyncTimeInUS;
    lastSyncTimeInUS = timeInUS;
  } 
  else if (command_type == COMMAND_TYPE_GLASSES_TIME_OFFSET) 
  {
    while (Serial.available() < 2);
    uint8_t value = (uint8_t)Serial.read();
    uint8_t checkSum = (uint8_t)Serial.read();
    if (checkSum != (uint8_t)(0x54 + 0xED + command_type + value)) return;
    glassesTimeOffset = value;
  } 
  else if (command_type == COMMAND_TYPE_TRANSPARENT_TIME_PERCENT) 
  {
    while (Serial.available() < 2);
    uint8_t value = (uint8_t)Serial.read();
    uint8_t checkSum = (uint8_t)Serial.read();
    if (checkSum != (uint8_t)(0x54 + 0xED + command_type + value)) return;
    transparentTimePercent = value;
  }
  //-------------------------------
  UpdateVariables();
}

void UpdateVariables() {
  if (twoEyePeriodInUS == 0) return;
  //----------------------------------------------------------------
  glassesTimeOffsetInUS = (int32_t)(twoEyePeriodInUS * (double)glassesTimeOffset / 100.0);
  //----------------------------------------------------------------
  shutters.Left.Begin = (int32_t)((double)twoEyePeriodInUS * (0.25 - ((double)transparentTimePercent / 400.0)));
  shutters.Left.End = (int32_t)((double)twoEyePeriodInUS * (0.25 + ((double)transparentTimePercent / 400.0)));
  shutters.Right.Begin = (int32_t)((double)shutters.Left.Begin + ((double)twoEyePeriodInUS / 2.0));
  shutters.Right.End = (int32_t)((double)shutters.Left.End + ((double)twoEyePeriodInUS / 2.0));
  //----------------------------------------------------------------
  shutters.Left.Begin = (shutters.Left.Begin + glassesTimeOffsetInUS) % twoEyePeriodInUS;
  shutters.Left.End = (shutters.Left.End + glassesTimeOffsetInUS) % twoEyePeriodInUS;
  shutters.Right.Begin = (shutters.Right.Begin + glassesTimeOffsetInUS) % twoEyePeriodInUS;
  shutters.Right.End = (shutters.Right.End + glassesTimeOffsetInUS) % twoEyePeriodInUS;
}

uint8_t GetTransparencyTime(Transparency transparency, uint8_t common) {
  if (transparency.Begin <= transparency.End) {
    if ((transparency.Begin < insideTwoEyeCycleInUS) && (insideTwoEyeCycleInUS < transparency.End)) {
      return common;
    } 
    else {
      return !common;
    }
  } 
  else {
    if ((transparency.Begin < insideTwoEyeCycleInUS) || (insideTwoEyeCycleInUS < transparency.End)) {
      return common;
    } 
    else {
      return !common;
    }
  }
}

void SetupSerial() {
  Serial.begin(250000);
  Serial.flush();
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
  OCR1A = 1;  //7812.5 Hz
  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  // Enable interrupts globally
  sei();
}

// Interrupt Service Routine for Timer1 compare match
ISR(TIMER1_COMPA_vect) {
  if (glassesTimeOffset == (uint8_t)(-1)) return;
  if (transparentTimePercent == (uint8_t)(-1)) return;
  //----------------------------------------------------------------
  output.Common = !output.Common;
  //----------------------------------------------------------------
  insideTwoEyeCycleInUS = micros() - lastSyncTimeInUS;
  //----------------------------------------------------------------
  output.Left = GetTransparencyTime(shutters.Left, output.Common);
  output.Right = GetTransparencyTime(shutters.Right, output.Common);
  if (insideTwoEyeCycleInUS > twoEyePeriodInUS * 2) {
    output.Left = output.Common;
    output.Right = output.Common;
  }
  //----------------------------------------------------------------
  digitalWrite(LEFT_SHUTTER_PIN, output.Left);
  digitalWrite(COMMON_SHUTTER_PIN, output.Common);
  digitalWrite(RIGHT_SHUTTER_PIN, output.Right);
}
