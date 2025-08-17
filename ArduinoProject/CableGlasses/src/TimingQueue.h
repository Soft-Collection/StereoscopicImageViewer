#ifndef TimingQueue_h
#define TimingQueue_h

#include <avr/io.h>

#define BUFFER_SIZE 20

class TimingQueue
{
public:
  typedef struct
  {
    volatile uint8_t Value;
    volatile uint32_t Time;
  } TimingQueueData;

public:
  TimingQueue();
  void Clear();
  void Enqueue(uint8_t value, uint32_t time);
  void Dequeue(uint8_t &value, uint32_t &time);
  void Peek(uint8_t &value, uint32_t &time);
  void Pop();
  uint8_t Size();

private:
  TimingQueueData mBuffer[BUFFER_SIZE];
  volatile uint8_t mHead;
  volatile uint8_t mTail;
  volatile uint8_t mSize;
};

#endif // TimingQueue_h