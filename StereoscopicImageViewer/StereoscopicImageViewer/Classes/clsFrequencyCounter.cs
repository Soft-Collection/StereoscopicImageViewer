using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StereoscopicImageViewer
{
    class clsFrequencyCounter : IDisposable
    {
        #region Variables
        private Task mTask = null;
        private bool mTaskQuit = false;
        private int mFrameCounter = 0;
        private int mFPSValue = 0;
        private bool mDisposed = false;
        #endregion

        #region New / Dispose
        public clsFrequencyCounter()
        {
            if (mTask == null)
            {
                mTask = new Task(() =>
                {
                    mTaskQuit = false;
                    while (!mTaskQuit)
                    {
                        System.Threading.Interlocked.Exchange(ref mFPSValue, mFrameCounter);
                        System.Threading.Interlocked.Exchange(ref mFrameCounter, 0);
                        System.Threading.Thread.Sleep(1000);
                    }
                });
                mTask.Start();
            }
        }
        ~clsFrequencyCounter()
        {
            Dispose(false);
        }
        public void Dispose()
        {
            try
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            catch
            {
            }
        }
        protected virtual void Dispose(bool disposing)
        {
            try
            {
                if (!this.mDisposed)
                {
                    if (disposing)
                    {
                    }
                    //-------------------------------------------------------
                    mTaskQuit = true;
                    if (mTask != null)
                    {
                        mTask.Wait();
                        mTask.Dispose();
                        mTask = null;
                    }
                    //-------------------------------------------------------
                    mDisposed = true;
                }
            }
            catch
            {
            }
        }
        #endregion

        #region Methods
        public void Increment()
        {
            System.Threading.Interlocked.Increment(ref mFrameCounter);
        }
        public int GetFrequencyInHzValue()
        {
            return mFPSValue;
        }
        #endregion
    }
}
