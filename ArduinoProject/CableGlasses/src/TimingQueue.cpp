#include "TimingQueue.h"
#include <string.h>

#define increment_modulo(value, modulo) ((value) = (((value) < ((modulo) - 1)) ? (value) + 1 : 0))
#define minus_modulo(value, valueToSubtract, modulo) (((value) - (valueToSubtract)) >= 0) ? ((value) - (valueToSubtract)) : ((value) + (modulo) - (valueToSubtract))

TimingQueue::TimingQueue()
{
  Clear();
}

void TimingQueue::Clear()
{
  memset((void *)mBuffer, 0, sizeof(TimingQueueData) * BUFFER_SIZE);
  mHead = 0;
  mTail = 0;
  mSize = 0;
}

void TimingQueue::Enqueue(uint8_t value, uint32_t time)
{
  mBuffer[mHead] = { value, time };
  increment_modulo(mHead, BUFFER_SIZE);
}

void TimingQueue::Dequeue(uint8_t &value, uint32_t &time)
{
  Peek(value, time);
  Pop();
}

void TimingQueue::Peek(uint8_t &value, uint32_t &time)
{
  TimingQueueData tqd = mBuffer[mTail];
  value = tqd.Value;
  time = tqd.Time;
}

void TimingQueue::Pop()
{
  increment_modulo(mTail, BUFFER_SIZE);
}

uint8_t TimingQueue::Size()
{
  return minus_modulo(mHead, mTail, BUFFER_SIZE);
}