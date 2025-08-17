using System;
using System.Runtime.InteropServices;

public class clsStereoImageManagerWrap
{
    #region Common

    #region Constants
    private const string DllFileName = "StereoImageManager.dll";
    #endregion

    #endregion

    #region Stereo Image Manager

    #region External Functions

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr StereoImageManagerCreateNew(IntPtr hWnd);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerDispose(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoStart(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoStop(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerStereoIsStarted(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int StereoImageManagerStereoGetFrequency(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSetCOMPort(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string comPort);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSetGlassesTimeOffset(IntPtr StereoImageManagerHandle, int offset);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoLRBoth(IntPtr StereoImageManagerHandle, int lrboth);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoSwapLR(IntPtr StereoImageManagerHandle, bool swaplr);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStereoWindowSizeChanged(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerImagerProvideImages(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string leftImageFilePath, [MarshalAs(UnmanagedType.LPWStr)] string rightImageFilePath);

    #endregion

    #endregion
}
