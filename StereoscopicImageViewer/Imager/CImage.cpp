#include "stdafx.h"
#include "CImage.h"

CImage::CImage(std::wstring FilePath, int Width, int Height, int Channels, std::vector<BYTE> PixelData, bool IsLeft)
{
    this->FilePath = FilePath;
    this->Width = Width;
    this->Height = Height;
    this->Channels = Channels;
    this->PixelData = PixelData;
    this->IsLeft = IsLeft;
}

CImage::~CImage()
{
}

//BGRA
CImage* CImage::LoadImage(std::wstring filePath, bool isLeft)
{
    CImage* receivedImage = NULL;
    CImage* convertedImage = NULL;
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<BYTE> pixelData;
    // Initialize COM library
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return NULL;
    // Create WIC factory
    IWICImagingFactory* pFactory = NULL;
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory);
    if (FAILED(hr)) return NULL;
    // Create a decoder to read the image
    IWICBitmapDecoder* pDecoder = NULL;
    pFactory->CreateDecoderFromFilename(filePath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    // Get frame from the image
    IWICBitmapFrameDecode* pFrame = NULL;
    pDecoder->GetFrame(0, &pFrame);
    // Get image size
    pFrame->GetSize((UINT*)&width, (UINT*)&height);
    // Get pixel format
    WICPixelFormatGUID pixelFormat;
    pFrame->GetPixelFormat(&pixelFormat);
    // Determine number of channels
    if (IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppBGRA)) {
        channels = 4;
    }
    else if (IsEqualGUID(pixelFormat, GUID_WICPixelFormat24bppBGR)) {
        channels = 3;
    }
    else {
        std::cerr << "Unsupported pixel format!" << std::endl;
        pFrame->Release();
        pDecoder->Release();
        pFactory->Release();
        CoUninitialize();
        return NULL;
    }
    // Allocate memory for image data
    pixelData.resize(width * height * channels);
    // Copy image data to pixelData vector
    pFrame->CopyPixels(NULL, width * channels, width * height * channels, pixelData.data());
    // Release resources
    pFrame->Release();
    pDecoder->Release();
    pFactory->Release();
    CoUninitialize();
    receivedImage = new CImage(filePath, width, height, channels, pixelData, isLeft);
    if (receivedImage->Channels == 3)
    {
        convertedImage = ConvertBGR24ToBGRA32(receivedImage);
        if (receivedImage)
        {
            delete receivedImage;
            receivedImage = NULL;
            return convertedImage;
        }
    }
    return receivedImage;
}

CImage* CImage::ConvertRGB24ToBGRA32(CImage* srcImage)
{
    int srcPitch = srcImage->Width * 3;
    int dstPitch = srcImage->Width * 4;
    std::vector<BYTE> dst;
    // Allocate memory for image data
    dst.resize(srcImage->Width * srcImage->Height * 4);
    for (int y = 0; y < srcImage->Height; y++)
    {
        const uint8_t* s = srcImage->PixelData.data() + y * srcPitch;
        uint8_t* d = dst.data() + y * dstPitch;
        for (int x = 0; x < srcImage->Width; x++)
        {
            uint8_t r = s[0];
            uint8_t g = s[1];
            uint8_t b = s[2];
            d[0] = b;     // B
            d[1] = g;     // G
            d[2] = r;     // R
            d[3] = 255;   // A = opaque
            s += 3;
            d += 4;
        }
    }
    return new CImage(L"", srcImage->Width, srcImage->Height, 4, dst, true);
}

CImage* CImage::ConvertBGR24ToBGRA32(CImage* srcImage)
{
    int srcPitch = srcImage->Width * 3; // 3 bytes per pixel
    int dstPitch = srcImage->Width * 4; // 4 bytes per pixel
    std::vector<BYTE> dst;
    // Allocate memory for image data
    dst.resize(srcImage->Width * srcImage->Height * 4);
    for (int y = 0; y < srcImage->Height; y++)
    {
        const BYTE* s = srcImage->PixelData.data() + y * srcPitch;
        BYTE* d = dst.data() + y * dstPitch;
        for (int x = 0; x < srcImage->Width; x++)
        {
            d[0] = s[0]; // B
            d[1] = s[1]; // G
            d[2] = s[2]; // R
            d[3] = 255;  // A = opaque
            s += 3;
            d += 4;
        }
    }
    return new CImage(L"", srcImage->Width, srcImage->Height, 4, dst, true);
}