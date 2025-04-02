using System;

public class clsStereoImageManager
{
    #region External Functions
    #endregion

    #region Variables
    private readonly object mLock = new object();
    private IntPtr mHandle = IntPtr.Zero;
    #endregion

    #region New / Dispose
    public clsStereoImageManager(IntPtr hWnd, clsSIMWrap.eFrequencies frequency, clsSIMWrap.eSignalSources signalSource, string comPort, string leftImageFilePath, string rightImageFilePath)
    {
        try
        {
            mHandle = clsSIMWrap.StereoImageManagerCreateNew(hWnd, frequency, signalSource, comPort, leftImageFilePath, rightImageFilePath);
        }
        catch (Exception ex)
        {
            ExceptionManager.Publish(ex);
        }
    }

    public void Dispose()
    {
        try
        {
            GC.SuppressFinalize(this);
            clsSIMWrap.StereoImageManagerDispose(mHandle);
        }
        catch (Exception ex)
        {
            ExceptionManager.Publish(ex);
        }
    }

    ~clsStereoImageManager()
    {
        Dispose();
    }
    #endregion

    #region Methods
    public clsSIMWrap.eStereoImageManagerErrors VideoRender()
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsSIMWrap.StereoImageManagerVideoRender(mHandle);
                }
            }
        }
        catch (Exception ex)
        {
            ExceptionManager.Publish(ex);
        }
        finally
        {
        }
        return clsSIMWrap.eStereoImageManagerErrors.NullHandle;
    }
    #endregion
}
