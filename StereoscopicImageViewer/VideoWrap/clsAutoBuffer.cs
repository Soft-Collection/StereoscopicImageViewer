using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

public class clsAutoBuffer<T>
{
    #region Delegates
    public delegate void PerformActionDelegate(T item);
    #endregion

    #region Variables
    private Task mTask = null;
    private Queue<T> mQueue = null;
    private readonly object mQueueLock = new object();
    private PerformActionDelegate mPerformActionDelegate = null;
    private readonly object mPerformActionDelegateLock = new object();
    private int mDelay = 0;
    #endregion

    #region Properties
    public int Delay
    {
        get { return mDelay; }
        set { Interlocked.Exchange(ref mDelay, value); }
    }
    #endregion

    #region Constructor / Dispose
    public clsAutoBuffer(PerformActionDelegate performActionDelegate, int delay)
    {
        lock (mPerformActionDelegateLock)
        {
            mPerformActionDelegate = performActionDelegate;
        }
        mDelay = delay;
        lock (mQueueLock)
        {
            mQueue = new Queue<T>();
        }
    }

    public void Dispose()
    {
        lock (mPerformActionDelegateLock)
        {
            mPerformActionDelegate = null;
        }
    }
    #endregion

    #region Methods
    private void Perform()
    {
        if (mTask == null)
        {
            mTask = new Task(() =>
            {
                while (true)
                {
                    T tempItem = default(T);
                    lock (mQueueLock)
                    {
                        if (mQueue != null && mQueue.Count > 0)
                        {
                            tempItem = mQueue.Dequeue();
                        }
                    }

                    PerformActionDelegate tempPerformActionDelegate = null;
                    lock (mPerformActionDelegateLock)
                    {
                        tempPerformActionDelegate = mPerformActionDelegate;
                    }

                    if (tempPerformActionDelegate != null)
                    {
                        tempPerformActionDelegate(tempItem);
                    }

                    if (mDelay > 0)
                    {
                        Thread.Sleep(mDelay);
                    }

                    lock (mQueueLock)
                    {
                        if (mQueue != null && mQueue.Count == 0)
                        {
                            mTask = null;
                            return;
                        }
                    }
                }
            });
            mTask.Start();
        }
    }

    public void Enqueue(T item)
    {
        lock (mQueueLock)
        {
            if (mQueue != null)
            {
                mQueue.Enqueue(item);
                Perform();
            }
        }
    }

    public void UrgentEnqueue(T item)
    {
        lock (mQueueLock)
        {
            if (mQueue != null)
            {
                mQueue.Clear();
                mQueue.Enqueue(item);
                Perform();
            }
        }
    }

    public void EnqueueIfEmpty(T item)
    {
        lock (mQueueLock)
        {
            if (mQueue != null && mQueue.Count == 0)
            {
                mQueue.Enqueue(item);
                Perform();
            }
        }
    }

    public void EnqueueIfDifferent(T item)
    {
        lock (mQueueLock)
        {
            if (mQueue != null)
            {
                if (mQueue.Count == 0)
                {
                    mQueue.Enqueue(item);
                    Perform();
                }
                else
                {
                    T prevItem = mQueue.Peek();
                    if (!item.Equals(prevItem))
                    {
                        mQueue.Enqueue(item);
                        Perform();
                    }
                }
            }
        }
    }
    #endregion
}
