using System;

public class clsAudioVideoRender
{
    #region External Functions
    #endregion

    #region Variables
    private readonly object mLock = new object();
    private IntPtr mHandle = IntPtr.Zero;
    #endregion

    #region New / Dispose
    public clsAudioVideoRender(IntPtr hWnd, clsAudioVideoWrap.eVideoRenderTargets videoRenderTarget, clsAudioVideoWrap.eFrequencies frequency, clsAudioVideoWrap.eSignalSources signalSource, string comPort, string leftImageFilePath, string rightImageFilePath)
    {
        try
        {
            mHandle = clsAudioVideoWrap.AudioVideoRenderCreateNew(hWnd, videoRenderTarget, frequency, signalSource, comPort, leftImageFilePath, rightImageFilePath);
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
            clsAudioVideoWrap.AudioVideoRenderDispose(mHandle);
        }
        catch (Exception ex)
        {
            ExceptionManager.Publish(ex);
        }
    }

    ~clsAudioVideoRender()
    {
        Dispose();
    }
    #endregion

    #region Methods
    public clsAudioVideoWrap.eAudioVideoRenderErrors VideoRender()
    {
        try
        {
            if (mHandle != IntPtr.Zero)
            {
                lock (mLock)
                {
                    return clsAudioVideoWrap.AudioVideoRenderVideoRender(mHandle);
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
        return clsAudioVideoWrap.eAudioVideoRenderErrors.NullHandle;
    }
    #endregion
}
