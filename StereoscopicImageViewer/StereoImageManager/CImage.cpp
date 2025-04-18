#include "stdafx.h"
#include "CImage.h"

CImage::CImage()
{
    this->Width = 0;
    this->Height = 0;
    this->Channels = 0;
    this->IsLeft = false;
}

CImage::~CImage()
{
}

//BGRA
void CImage::LoadImage(std::wstring filePath, int& width, int& height, int& channels, std::vector<BYTE>& pixelData, bool isLeft)
{
    // Initialize COM library
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return;
    // Create WIC factory
    IWICImagingFactory* pFactory = NULL;
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory);
    if (FAILED(hr)) return;
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
        return;
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
}

void CImage::LoadImage(CImage* img)
{
    CImage::LoadImage(img->FilePath, img->Width, img->Height, img->Channels, img->PixelData, img->IsLeft);
}