using System;
using System.Runtime.InteropServices;

public class clsStereoImageManagerWrap
{
    #region Common

    #region Constants
    private const string DllFileName = "StereoImageManager.dll";
    private const int MMSYSERR_BASE = 0;
    private const int WAVERR_BASE = 32;
    #endregion

    #region Enums
    public enum eVideoFrameTypes : uint
    {
        undefine = 0,
        JPEG = 1,
        I = 2,
        P = 3,
        B = 4,
        I_X264 = 23,
        P_X264 = 17
    }

    public enum eMMRESULT : int
    {
        MMSYSERR_NOERROR = 0,
        MMSYSERR_ERROR = MMSYSERR_BASE + 1,
        MMSYSERR_BADDEVICEID = MMSYSERR_BASE + 2,
        MMSYSERR_NOTENABLED = MMSYSERR_BASE + 3,
        MMSYSERR_ALLOCATED = MMSYSERR_BASE + 4,
        MMSYSERR_INVALHANDLE = MMSYSERR_BASE + 5,
        MMSYSERR_NODRIVER = MMSYSERR_BASE + 6,
        MMSYSERR_NOMEM = MMSYSERR_BASE + 7,
        MMSYSERR_NOTSUPPORTED = MMSYSERR_BASE + 8,
        MMSYSERR_BADERRNUM = MMSYSERR_BASE + 9,
        MMSYSERR_INVALFLAG = MMSYSERR_BASE + 10,
        MMSYSERR_INVALPARAM = MMSYSERR_BASE + 11,
        MMSYSERR_HANDLEBUSY = MMSYSERR_BASE + 12,
        MMSYSERR_INVALIDALIAS = MMSYSERR_BASE + 13,
        MMSYSERR_BADDB = MMSYSERR_BASE + 14,
        MMSYSERR_KEYNOTFOUND = MMSYSERR_BASE + 15,
        MMSYSERR_READERROR = MMSYSERR_BASE + 16,
        MMSYSERR_WRITEERROR = MMSYSERR_BASE + 17,
        MMSYSERR_DELETEERROR = MMSYSERR_BASE + 18,
        MMSYSERR_VALNOTFOUND = MMSYSERR_BASE + 19,
        MMSYSERR_NODRIVERCB = MMSYSERR_BASE + 20,
        MMSYSERR_MOREDATA = MMSYSERR_BASE + 21,
        MMSYSERR_LASTERROR = MMSYSERR_BASE + 21,
        WAVERR_BADFORMAT = WAVERR_BASE + 0,
        WAVERR_STILLPLAYING = WAVERR_BASE + 1,
        WAVERR_UNPREPARED = WAVERR_BASE + 2,
        WAVERR_SYNC = WAVERR_BASE + 3,
        WAVERR_LASTERROR = WAVERR_BASE + 3
    }
    #endregion

    #region Structures
    [StructLayout(LayoutKind.Sequential)]
    public struct tWAVEFORMATEX
    {
        public ushort wFormatTag;
        public ushort nChannels;
        public uint nSamplesPerSec;
        public uint nAvgBytesPerSec;
        public ushort nBlockAlign;
        public ushort wBitsPerSample;
        public ushort cbSize;

        public tWAVEFORMATEX(ushort wFormatTag, ushort nChannels, uint nSamplesPerSec, uint nAvgBytesPerSec, ushort nBlockAlign, ushort wBitsPerSample, ushort cbSize)
        {
            this.wFormatTag = wFormatTag;
            this.nChannels = nChannels;
            this.nSamplesPerSec = nSamplesPerSec;
            this.nAvgBytesPerSec = nAvgBytesPerSec;
            this.nBlockAlign = nBlockAlign;
            this.wBitsPerSample = wBitsPerSample;
            this.cbSize = cbSize;
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct tagWAVEOUTCAPSW
    {
        public ushort wMid;
        public ushort wPid;
        public uint vDriverVersion;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string szPname;
        public uint dwFormats;
        public ushort wChannels;
        public ushort wReserved1;
        public uint dwSupport;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct tagWAVEINCAPSA
    {
        public ushort wMid;
        public ushort wPid;
        public uint vDriverVersion;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string szPname;
        public uint dwFormats;
        public ushort wChannels;
        public ushort wReserved1;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct mmtime_tag
    {
        public uint wType;
        public Anonymous_191604fe_770e_4688_aef5_8c113498eb25 u;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct Anonymous_191604fe_770e_4688_aef5_8c113498eb25
    {
        [FieldOffset(0)]
        public uint ms;
        [FieldOffset(0)]
        public uint sample;
        [FieldOffset(0)]
        public uint cb;
        [FieldOffset(0)]
        public uint ticks;
        [FieldOffset(0)]
        public Anonymous_9bd9399e_c332_40ac_ad3c_8155b5ea7388 smpte;
        [FieldOffset(0)]
        public Anonymous_cb1eb549_6c81_48bd_b5bf_228b52412176 midi;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Anonymous_9bd9399e_c332_40ac_ad3c_8155b5ea7388
    {
        public byte hour;
        public byte min;
        public byte sec;
        public byte frame;
        public byte fps;
        public byte dummy;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] pad;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Anonymous_cb1eb549_6c81_48bd_b5bf_228b52412176
    {
        public uint songptrpos;
    }

    [CLSCompliant(false)]
    [StructLayout(LayoutKind.Sequential)]
    public struct STREAM_INFO
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
        public byte[] FileIdString;
        public uint VideoFormat;
        public uint GOPSize;
        public uint FrameWidth;
        public uint FrameHeight;
    }

    [CLSCompliant(false)]
    [StructLayout(LayoutKind.Sequential)]
    public struct GOP_INFO
    {
        public uint Index;
        public uint GOPHeaderOffset;
        public uint GOPDataOffset;
        public uint Length;
        public uint NumberOfFrames;
    }

    [CLSCompliant(false)]
    [StructLayout(LayoutKind.Sequential)]
    public struct FRAME_INFO
    {
        public int Index;
        public uint FrameHeaderOffset;
        public uint FrameDataOffset;
        public eVideoFrameTypes Type;
        public int Length;
        public long Time;
    }

    [CLSCompliant(false)]
    public struct FrameInfoAndData
    {
        public bool IsManaged;
        public FRAME_INFO FrameInfo;
        public IntPtr UnmanagedData;
        public byte[] ManagedData;
        public int GOPIndex;
        public IntPtr FrameHeaderPtr;
        public IntPtr FrameDataPtr;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct tagBITMAPINFOHEADER
    {
        public uint biSize;
        public int biWidth;
        public int biHeight;
        public ushort biPlanes;
        public ushort biBitCount;
        public uint biCompression;
        public uint biSizeImage;
        public int biXPelsPerMeter;
        public int biYPelsPerMeter;
        public uint biClrUsed;
        public uint biClrImportant;
    }
    #endregion

    #endregion

    #region Stereo Image Manager

    #region External Functions

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr StereoImageManagerCreateNew(IntPtr hWnd, eFrequencies frequency, clsStereoImageManagerWrap.eSignalSources signalSource, [MarshalAs(UnmanagedType.LPWStr)] string comPort, [MarshalAs(UnmanagedType.LPWStr)] string leftImageFilePath, [MarshalAs(UnmanagedType.LPWStr)] string rightImageFilePath);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void StereoImageManagerDispose(IntPtr StereoImageManagerHandle);

    [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
    public static extern eStereoImageManagerErrors StereoImageManagerVideoRender(IntPtr StereoImageManagerHandle);

    #endregion

    #region Enums

    public enum eFrequencies : int
    {
        Default = 0,
        F1Hz = 1,
        F60Hz = 2,
        F75Hz = 3,
        F100Hz = 4,
        F120Hz = 5,
        F144Hz = 6
    }

    public enum eSignalSources : int
    {
        ScreenSensor = 0,
        COMPort = 1
    }

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
