#include "StereoGlassesController.h"

volatile uint32_t StereoGlassesController::msTimer2TicksCounter = 0;
volatile uint8_t StereoGlassesController::msLeftShutterState = TRANSPARENT;
volatile uint8_t StereoGlassesController::msRightShutterState = TRANSPARENT;

StereoGlassesController::StereoGlassesController()
{
}

void StereoGlassesController::begin()
{
    SetupPins();
    SetupTimer2();
}

void StereoGlassesController::setShutterState(bool isLeft, uint8_t state)
{
    if (isLeft)
    {
        msLeftShutterState = state;
    }
    else
    {
        msRightShutterState = state;
    }
}

void StereoGlassesController::heartBeat()
{
    msTimer2TicksCounter = 0;
}

void StereoGlassesController::digitalWrite(uint8_t leftPinState, uint8_t commonPinState, uint8_t rightPinState)
{
    uint8_t tempPortD = PORTD;
    if (leftPinState == HIGH) tempPortD |= (1 << LEFT_SHUTTER_PIN); else tempPortD &= ~(1 << LEFT_SHUTTER_PIN);
    if (commonPinState == HIGH) tempPortD |= (1 << COMMON_SHUTTER_PIN); else tempPortD &= ~(1 << COMMON_SHUTTER_PIN);
    if (rightPinState == HIGH) tempPortD |= (1 << RIGHT_SHUTTER_PIN); else tempPortD &= ~(1 << RIGHT_SHUTTER_PIN);
    PORTD = tempPortD;
}

void StereoGlassesController::SetupPins()
{
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7); // Set D5, D6 and D7 as outputs
    digitalWrite(LOW, LOW, LOW);
}

void StereoGlassesController::SetupTimer2()
{
    cli();      // Disable global interrupts
    TCCR2A = 0; // Normal mode (CTC set by WGM12)
    TCCR2B = 0;
    TCCR2A |= (1 << WGM21);                            // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler 1024
    // OCRxA = (F_CPU / (Prescaler * Freq)) - 1
    // Freq = F_CPU / (Prescaler * (OCRxA + 1))
    OCR2A = 1; // 7812.5 Hz
    // Enable Timer2 compare interrupt
    TIMSK2 |= (1 << OCIE2A);
    sei(); // Enable global interrupts
}

ISR(TIMER2_COMPA_vect)
{
    static uint8_t common = LOW;
    static uint8_t left = LOW;
    static uint8_t right = LOW;
    //----------------------------------------------------------------
    StereoGlassesController::msTimer2TicksCounter++;
    //----------------------------------------------------------------
    if (StereoGlassesController::msTimer2TicksCounter > TICKS_TO_SET_TRANSPARENT)
    {
        left = common;
        right = common;
    }
    else
    {
        left = (StereoGlassesController::msLeftShutterState == TRANSPARENT) ? common : !common;
        right = (StereoGlassesController::msRightShutterState == TRANSPARENT) ? common : !common;
    }
    StereoGlassesController::digitalWrite(left, common, right);
    //----------------------------------------------------------------
    common = !common;
}