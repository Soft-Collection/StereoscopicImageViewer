using System;

public class clsStereoImageManager
{
    #region Variables
    private IntPtr mHandle = IntPtr.Zero;
    #endregion

    #region New / Dispose
    public clsStereoImageManager(IntPtr hWnd)
    {
        mHandle = clsStereoImageManagerWrap.StereoImageManagerCreateNew(hWnd);
    }
    public void Dispose()
    {
        GC.SuppressFinalize(this);
        clsStereoImageManagerWrap.StereoImageManagerDispose(mHandle);
    }
    ~clsStereoImageManager()
    {
        Dispose();
    }
    #endregion

    #region Methods
    public void DrawImage(string leftImageFilePath, string rightImageFilePath)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerDrawImage(mHandle, leftImageFilePath, rightImageFilePath);
        }
    }
    public void Start()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStart(mHandle);
        }
    }
    public void Stop()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStop(mHandle);
        }
    }
    public bool IsStarted()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerIsStarted(mHandle);
        }
        return false;
    }
    public int GetFrequency()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerGetFrequency(mHandle);
        }
        return 0;
    }
    public void SetCOMPort(string comPort)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerSetCOMPort(mHandle, comPort);
        }
    }
    public void SetGlassesTimeOffset(int offset)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerSetGlassesTimeOffset(mHandle, offset);
        }
    }
    public void SetTransparentTimePercent(int percent)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerSetTransparentTimePercent(mHandle, percent);
        }
    }
    #endregion
}
