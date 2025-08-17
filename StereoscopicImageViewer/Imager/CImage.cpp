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
    return new CImage(filePath, width, height, channels, pixelData, isLeft);
}
