#include "USART.h"

void *USART::msInstance = nullptr;
USART::dOnSync USART::msOnSync = nullptr;
USART::dOnDelayedSync USART::msOnDelayedSync = nullptr;
volatile uint16_t USART::msGlassesTimeOffset = 0;
volatile uint32_t USART::msTimer0TicksCounter = 0;
volatile uint32_t USART::msLastTimer0TicksCounter = 0;
volatile uint16_t USART::msTwoEyesPeriodInTT = 0;
TimingQueue USART::msTimingQueue;

USART::USART()
{
}

void USART::begin(void *instance, dOnSync onSync, dOnDelayedSync onDelayedSync, uint32_t baudRate)
{
    USART::msInstance = instance;
    USART::msOnSync = onSync;
    USART::msOnDelayedSync = onDelayedSync;
    SetupUSART(baudRate);
    SetupTimer0();
}

void USART::SetupUSART(uint32_t baudRate)
{
    cli();
    // ubrr = (F_CPU / (16 * Baudrate))
    uint16_t ubrr = (F_CPU / (16 * baudRate));
    // 8 is for 115200
    // Set baud rate
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    // Enable receiver and RX complete interrupt
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    // 8-bit data, no parity, 1 stop bit (default)
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    // Global interrupt enable
    sei();
}

// USART RX Complete Interrupt Service Routine
ISR(USART_RX_vect)
{
    uint8_t receivedByte = UDR0;                      // Read received byte from UART data register
    if ((0 <= receivedByte) && (receivedByte <= 100)) // Glasses Time Offset
    {
        uint8_t value = receivedByte;
        if (value < 0) value = 0;
        if (value > 99) value = 99;
        USART::msGlassesTimeOffset = value;
    }
    else if ((105 <= receivedByte) && (receivedByte <= 108)) // Sync
    {
        if (USART::msOnSync != nullptr)
        {
            uint8_t value = receivedByte - 105;
            if (value == 0)
            {
                uint32_t tcc = USART::msTimer0TicksCounter;
                USART::msTwoEyesPeriodInTT = tcc - USART::msLastTimer0TicksCounter;
                USART::msLastTimer0TicksCounter = tcc;
            }
            if (value < 0) value = 0;
            if (value > 3) value = 3;
            USART::msTimingQueue.Enqueue(value, USART::msTimer0TicksCounter);
            USART::msOnSync(USART::msInstance, value);
        }
    }
}

void USART::SetupTimer0()
{
    cli(); // Disable global interrupts
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01);              // Set CTC mode (WGM01 = 1)
    TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler 1024
    // OCRxA = (F_CPU / (Prescaler * Freq)) - 1
    // Freq = F_CPU / (Prescaler * (OCRxA + 1))
    OCR0A = 1;
    // Enable Timer0 compare match A interrupt
    TIMSK0 |= (1 << OCIE0A);
    sei(); // Enable global interrupts
}

ISR(TIMER0_COMPA_vect)
{
    USART::msTimer0TicksCounter++;
    if (USART::msTimingQueue.Size() > 0)
    {
        uint8_t value;
        uint32_t time;
        USART::msTimingQueue.Peek(value, time);
        uint16_t offset = (uint16_t)((double)USART::msTwoEyesPeriodInTT * (double)USART::msGlassesTimeOffset / 100.0);
        if (time + offset < USART::msTimer0TicksCounter)
        {
            if (USART::msOnDelayedSync != nullptr)
            {
                USART::msOnDelayedSync(USART::msInstance, value);
            }
            USART::msTimingQueue.Pop();
        }
    }
}