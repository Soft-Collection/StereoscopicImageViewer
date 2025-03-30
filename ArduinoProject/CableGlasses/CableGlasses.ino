#include <Arduino.h>

//--------------------------------------------------------------------------------
#define LEFT_SHUTTER_PIN 6
#define COMMON_SHUTTER_PIN 7
#define RIGHT_SHUTTER_PIN 8
//--------------------------------------------------------------------------------

volatile bool isLeftTransparent = true;
volatile bool isRightTransparent = true;
volatile uint8_t leftOutput = LOW;
volatile uint8_t commonOutput = LOW;
volatile uint8_t rightOutput = LOW;
volatile uint32_t transparentTimeCounter = 0;
volatile uint32_t lowTransparencyBound = 0;
volatile uint32_t highTransparencyBound = 0;

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
  char inChar;
  while (!Serial.available()) {}
  while (Serial.available()) {
    inChar = (char)Serial.read();
  }
  lowTransparencyBound = transparentTimeCounter * 30 / 100;
  highTransparencyBound = transparentTimeCounter * 50 / 100;
  transparentTimeCounter = 0;
  isLeftTransparent = ((inChar == 'L') && (inChar != 'N'));
  isRightTransparent = ((inChar == 'R') && (inChar != 'N'));
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
  commonOutput = !commonOutput;
  leftOutput = (isLeftTransparent) ? commonOutput : !commonOutput;
  rightOutput = (isRightTransparent) ? commonOutput : !commonOutput;
  leftOutput = ((transparentTimeCounter > lowTransparencyBound) && (transparentTimeCounter < highTransparencyBound)) ? leftOutput : !commonOutput;
  rightOutput = ((transparentTimeCounter > lowTransparencyBound) && (transparentTimeCounter < highTransparencyBound)) ? rightOutput : !commonOutput;
  if (transparentTimeCounter > 40000) {
    leftOutput = commonOutput;
    rightOutput = commonOutput;
  }
  digitalWrite(LEFT_SHUTTER_PIN, leftOutput);
  digitalWrite(COMMON_SHUTTER_PIN, commonOutput);
  digitalWrite(RIGHT_SHUTTER_PIN, rightOutput);
  transparentTimeCounter++;
}
