using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

public class clsAutoBufferWithPriority<T>
{
    #region Structures
    private struct ItemStruct
    {
        public eDelayTypes DelayType;
        public int DelayInMilliseconds;
        public T Item;
    }
    #endregion

    #region Enums
    public enum ePriorities
    {
        Priority01 = 0,
        Priority02 = 1,
        Priority03 = 2,
        Priority04 = 3,
        Priority05 = 4,
        Priority06 = 5,
        Priority07 = 6,
        Priority08 = 7,
        Priority09 = 8,
        Priority10 = 9
    }

    public enum eDelayTypes
    {
        PreDelay,
        PostDelay
    }

    public enum eQueuePolicies
    {
        DoNothing,
        Ignore,
        Clear
    }
    #endregion

    #region Delegates
    public delegate void PerformActionDelegate(T item);
    #endregion

    #region Variables
    private Task mTask = null;
    private Queue<ItemStruct>[] mQueues = new Queue<ItemStruct>[10];
    private readonly object mQueuesLock = new object();
    private PerformActionDelegate mPerformActionDelegate = null;
    private readonly object mPerformActionDelegateLock = new object();
    #endregion

    #region Constructor / Dispose
    public clsAutoBufferWithPriority(PerformActionDelegate performActionDelegate)
    {
        lock (mPerformActionDelegateLock)
        {
            mPerformActionDelegate = performActionDelegate;
        }
        lock (mQueuesLock)
        {
            for (int i = 0; i < mQueues.Length; i++)
            {
                mQueues[i] = new Queue<ItemStruct>();
            }
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
                    ItemStruct tempItemStruct = default(ItemStruct);

                    lock (mQueuesLock)
                    {
                        if (mQueues != null)
                        {
                            for (int i = 0; i < mQueues.Length; i++)
                            {
                                if (mQueues[i] != null && mQueues[i].Count > 0)
                                {
                                    tempItemStruct = mQueues[i].Dequeue();
                                    break;
                                }
                            }
                        }
                    }

                    PerformActionDelegate tempPerformActionDelegate = null;
                    lock (mPerformActionDelegateLock)
                    {
                        tempPerformActionDelegate = mPerformActionDelegate;
                    }

                    if (tempPerformActionDelegate != null)
                    {
                        if (tempItemStruct.DelayType == eDelayTypes.PreDelay)
                        {
                            if (tempItemStruct.DelayInMilliseconds > 0)
                            {
                                Thread.Sleep(tempItemStruct.DelayInMilliseconds);
                            }
                        }

                        tempPerformActionDelegate(tempItemStruct.Item);

                        if (tempItemStruct.DelayType == eDelayTypes.PostDelay)
                        {
                            if (tempItemStruct.DelayInMilliseconds > 0)
                            {
                                Thread.Sleep(tempItemStruct.DelayInMilliseconds);
                            }
                        }
                    }

                    lock (mQueuesLock)
                    {
                        bool allQueuesAreEmpty = true;
                        if (mQueues != null)
                        {
                            for (int i = 0; i < mQueues.Length; i++)
                            {
                                if (mQueues[i] != null && mQueues[i].Count > 0)
                                {
                                    allQueuesAreEmpty = false;
                                    break;
                                }
                            }
                        }

                        if (allQueuesAreEmpty)
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

    public void Enqueue(T item, ePriorities priority, eDelayTypes delayType, int delayInMilliseconds, eQueuePolicies queuePolicyIfCurrentPriorityQueueNotEmpty)
    {
        lock (mQueuesLock)
        {
            if (mQueues != null && mQueues[(int)priority] != null)
            {
                ItemStruct tempItemStruct = new ItemStruct
                {
                    Item = item,
                    DelayType = delayType,
                    DelayInMilliseconds = delayInMilliseconds
                };

                switch (queuePolicyIfCurrentPriorityQueueNotEmpty)
                {
                    case eQueuePolicies.DoNothing:
                        mQueues[(int)priority].Enqueue(tempItemStruct);
                        break;
                    case eQueuePolicies.Ignore:
                        if (mQueues[(int)priority].Count == 0)
                        {
                            mQueues[(int)priority].Enqueue(tempItemStruct);
                        }
                        break;
                    case eQueuePolicies.Clear:
                        if (mQueues[(int)priority].Count > 0)
                        {
                            mQueues[(int)priority].Clear();
                        }
                        mQueues[(int)priority].Enqueue(tempItemStruct);
                        break;
                }
                Perform();
            }
        }
    }
    #endregion
}
