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
    public void StereoStart()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoStart(mHandle);
        }
    }
    public void StereoStop()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoStop(mHandle);
        }
    }
    public bool StereoIsStarted()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerStereoIsStarted(mHandle);
        }
        return false;
    }
    public int StereoGetFrequency()
    {
        if (mHandle != IntPtr.Zero)
        {
            return clsStereoImageManagerWrap.StereoImageManagerStereoGetFrequency(mHandle);
        }
        return 0;
    }
    public void StereoSetCOMPort(string comPort)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSetCOMPort(mHandle, comPort);
        }
    }
    public void StereoSetGlassesTimeOffset(int offset)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSetGlassesTimeOffset(mHandle, offset);
        }
    }
    public void StereoLRBoth(int lrboth)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoLRBoth(mHandle, lrboth);
        }
    }
    public void StereoSwapLR(bool swaplr)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoSwapLR(mHandle, swaplr);
        }
    }
    public void StereoWindowSizeChanged()
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerStereoWindowSizeChanged(mHandle);
        }
    }
    public void ImagerProvideImages(string leftImageFilePath, string rightImageFilePath)
    {
        if (mHandle != IntPtr.Zero)
        {
            clsStereoImageManagerWrap.StereoImageManagerImagerProvideImages(mHandle, leftImageFilePath, rightImageFilePath);
        }
    }    
    #endregion
}
