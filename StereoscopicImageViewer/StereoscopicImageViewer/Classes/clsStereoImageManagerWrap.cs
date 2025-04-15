using System;
using System.Runtime.InteropServices;

public class clsStereoImageManagerWrap
{
    #region Common

    #region Constants
    private const string DllFileName = "StereoImageManager.dll";
    #endregion

    #region Enums
    #endregion

    #region Structures
    #endregion

    #endregion

    #region Stereo Image Manager

    #region External Functions

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr StereoImageManagerCreateNew(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] string comPort, [MarshalAs(UnmanagedType.LPWStr)] string leftImageFilePath, [MarshalAs(UnmanagedType.LPWStr)] string rightImageFilePath);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerDispose(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern eStereoImageManagerErrors StereoImageManagerVideoRender(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern eStereoImageManagerErrors StereoImageManagerSetGlassesTimeOffset(IntPtr StereoImageManagerHandle, int offset);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern eStereoImageManagerErrors StereoImageManagerSetTransparentTimePercent(IntPtr StereoImageManagerHandle, int percent);

    #endregion

    #region Enums

    public enum eStereoImageManagerErrors : int
    {
        NoError = 0,
        NullHandle = 1,
        CouldNotCreateInstance = 2,
        CouldNotOpenVideoDecompressor = 3,
        CouldNotCloseVideoCodec = 4,
        CouldNotSetVideoDecompressParam = 5,
        CouldNotVideoDecompress = 6,
        ExceptionInVideoReInit = 7,
        ExceptionInVideoRender = 8,
        DibIsNull = 9,
        Direct2DIsNull = 10,
        Direct3DIsNull = 11,
        DecodedVideoIsNull = 12,
        DecodedVideoNotReady = 13,
        ExceptionInAudioReInit = 14,
        CouldNotOpenWaveOut = 15,
        CouldNotCloseWaveOut = 16,
        CouldNotWriteToWaveOut = 17,
        WaveIsNull = 18,
        UUCoreIsNull = 19,
        DifferentLeftRightImageDimensions = 20
    }

    #endregion

    #endregion
}
