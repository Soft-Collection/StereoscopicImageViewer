#include "stdafx.h"
#include "CImage.h"

CImage::CImage()
{
    this->Width = 0;
    this->Height = 0;
    this->Channels = 0;
    this->IsLeft = false;
    this->RectangleHeight = 0;
    this->RectanglesMustBeDrawn = false;
}

CImage::~CImage()
{
}

//BGRA
void CImage::LoadPNG(std::wstring filePath, int& width, int& height, int& channels, std::vector<BYTE>& pixelData, bool isLeft, int rectangleHeight, bool rectanglesMustBeDrawn)
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
    // Draw Rectangles
    if (!rectanglesMustBeDrawn) return;
    if (height < rectangleHeight) return;
    int middleWidth = width / 2;
    int rectangleTop = height - rectangleHeight;
    for (int y = rectangleTop; y < height; y++)
    {
        for (int x = 0; x < middleWidth; x++)
        {
            for (int i = 0; i < channels; i++)
            {
                pixelData[((y * width + x) * channels) + i] = isLeft ? 0 : 255;
            }
        }
        for (int x = middleWidth; x < width; x++)
        {
            for (int i = 0; i < channels; i++)
            {
                pixelData[((y * width + x) * channels) + i] = isLeft ? 255 : 0;
            }
        }
    }
}

void CImage::LoadPNG(CImage* img) 
{
    CImage::LoadPNG(img->FilePath, img->Width, img->Height, img->Channels, img->PixelData, img->IsLeft, img->RectangleHeight, img->RectanglesMustBeDrawn);
}