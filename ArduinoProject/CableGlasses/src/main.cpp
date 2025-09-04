#include <avr/io.h>
#include <avr/interrupt.h>
#include "StereoGlassesController.h"
#include "USART.h"

void setup();
void loop();
void OnSync(void *instance, uint8_t syncType);
void OnDelayedSync(void *instance, uint8_t syncType);

int main(void)
{
    setup();
    while (true)
    {
        loop();
    }
    return 0;
}

void setup()
{
    StereoGlassesController::begin();
    USART::begin(nullptr, OnSync, OnDelayedSync, 19200);
}

void loop()
{
}

void OnSync(void *instance, uint8_t syncType)
{
    StereoGlassesController::heartBeat();
}

void OnDelayedSync(void *instance, uint8_t syncType)
{
    switch (syncType)
    {
    case SYNC_LEFT_TRANSPARENT:
        StereoGlassesController::setShutterState(true, TRANSPARENT);
        break;
    case SYNC_LEFT_OPAQUE:
        StereoGlassesController::setShutterState(true, OPAQUE);
        break;
    case SYNC_RIGHT_TRANSPARENT:
        StereoGlassesController::setShutterState(false, TRANSPARENT);
        break;
    case SYNC_RIGHT_OPAQUE:
        StereoGlassesController::setShutterState(false, OPAQUE);
        break;
    }
}