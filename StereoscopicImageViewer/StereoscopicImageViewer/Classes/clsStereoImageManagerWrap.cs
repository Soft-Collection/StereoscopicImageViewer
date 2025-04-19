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
    public static extern void StereoImageManagerDrawImage(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string leftImageFilePath, [MarshalAs(UnmanagedType.LPWStr)] string rightImageFilePath);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStart(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerStop(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool StereoImageManagerIsStarted(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int StereoImageManagerGetFrequency(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerSetCOMPort(IntPtr StereoImageManagerHandle, [MarshalAs(UnmanagedType.LPWStr)] string comPort);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerSetGlassesTimeOffset(IntPtr StereoImageManagerHandle, int offset);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerSetTransparentTimePercent(IntPtr StereoImageManagerHandle, int percent);

    #endregion

    #endregion
}
