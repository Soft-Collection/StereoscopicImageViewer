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
    public clsStereoImageManager(IntPtr hWnd, clsStereoImageManagerWrap.eFrequencies frequency, clsStereoImageManagerWrap.eSignalSources signalSource, string comPort, string leftImageFilePath, string rightImageFilePath)
    {
        try
        {
            mHandle = clsStereoImageManagerWrap.StereoImageManagerCreateNew(hWnd, frequency, signalSource, comPort, leftImageFilePath, rightImageFilePath);
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
            clsStereoImageManagerWrap.StereoImageManagerDispose(mHandle);
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
    public clsStereoImageManagerWrap.eStereoImageManagerErrors VideoRender()
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsStereoImageManagerWrap.StereoImageManagerVideoRender(mHandle);
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
        return clsStereoImageManagerWrap.eStereoImageManagerErrors.NullHandle;
    }
    public clsStereoImageManagerWrap.eStereoImageManagerErrors SetGlassesTimeOffset(int offset)
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsStereoImageManagerWrap.StereoImageManagerSetGlassesTimeOffset(mHandle, offset);
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
        return clsStereoImageManagerWrap.eStereoImageManagerErrors.NullHandle;
    }
    public clsStereoImageManagerWrap.eStereoImageManagerErrors SetTransparentTimePercent(int percent)
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsStereoImageManagerWrap.StereoImageManagerSetTransparentTimePercent(mHandle, percent);
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
        return clsStereoImageManagerWrap.eStereoImageManagerErrors.NullHandle;
    }
    public clsStereoImageManagerWrap.eStereoImageManagerErrors WindowSizeOrLocationChanged()
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsStereoImageManagerWrap.StereoImageManagerWindowSizeOrLocationChanged(mHandle);
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
        return clsStereoImageManagerWrap.eStereoImageManagerErrors.NullHandle;
    }
    #endregion
}
