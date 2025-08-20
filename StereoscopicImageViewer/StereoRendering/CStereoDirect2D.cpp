#include "stdafx.h"
#include "CStereoDirect2D.h"

#define BOTH 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

CStereoDirect2D::CStereoDirect2D(HWND hWnd)
{
	m_HWnd = hWnd;
	m_D3DDevice = nullptr;
	m_D3DContext = nullptr;
	m_SwapChain = nullptr;
	m_D2DFactory = nullptr;
	m_D2DDevice = nullptr;
	m_D2DContext = nullptr;
	m_LRInBitmap = nullptr;
	m_LROutBitmap = nullptr;
	m_SourceRect = { 0.0, 0.0, 0.0, 0.0 };
	m_D2DClientRect = { 0.0, 0.0, 0.0, 0.0 };
	m_LastImageDimensions = { 0, 0, 0 };
	m_LeftImage = nullptr;
	m_RightImage = nullptr;
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	m_LRBoth.store(0);
	m_SwapLR.store(FALSE);
	m_ImageDataUpdated.store(FALSE);
	//--------------------------------------------------------
	mMutexDrawBlt = new std::mutex();
	//--------------------------------------------------------
	CreateDevice();
}
CStereoDirect2D::~CStereoDirect2D()
{
	ReleaseDevice();
	//--------------------------------------------------------
	if (mMutexDrawBlt != nullptr)
	{
		delete mMutexDrawBlt;
		mMutexDrawBlt = nullptr;
	}
}
void CStereoDirect2D::CreateDevice()
{
	InitDevice();
	InitSwapChain();
	CreateRenderTarget();
	m_LastImageDimensions = { 0, 0, 0 };
}
void CStereoDirect2D::InitDevice()
{
	HRESULT hr;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, nullptr, &context);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return;	}
	hr = device.As(&m_D3DDevice);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	hr = context.As(&m_D3DContext);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	ComPtr<IDXGIDevice> dxgiDevice;
	hr = m_D3DDevice.As(&dxgiDevice);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	ComPtr<ID2D1Factory1> baseFactory;
	D2D1_FACTORY_OPTIONS options = {};
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, &baseFactory);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	if (SUCCEEDED(hr))
	{
		baseFactory.As(&m_D2DFactory); // m_D2DFactory is ID2D1Factory7
	}
	else
	{
		ShowErrorInformation(hr);
		return;
	}
	hr = m_D2DFactory->CreateDevice(dxgiDevice.Get(), &m_D2DDevice);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	hr = m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_D2DContext);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
}
void CStereoDirect2D::InitSwapChain()
{
	HRESULT hr;
	ComPtr<IDXGIDevice> dxgiDevice;
	hr = m_D3DDevice.As(&dxgiDevice);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	ComPtr<IDXGIAdapter> adapter;
	hr = dxgiDevice->GetAdapter(&adapter);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	ComPtr<IDXGIFactory2> factory;
	hr = adapter->GetParent(IID_PPV_ARGS(&factory));
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = 2;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Width = 0;
	desc.Height = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	hr = factory->CreateSwapChainForHwnd(m_D3DDevice.Get(), m_HWnd, &desc, nullptr, nullptr, &m_SwapChain);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
}
void CStereoDirect2D::CreateRenderTarget()
{
	HRESULT hr;
	ComPtr<IDXGISurface> dxgiSurface;
	hr = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiSurface));
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	D2D1_BITMAP_PROPERTIES1 props = { { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE }, 96.0f, 96.0f, D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, nullptr };
	hr = m_D2DContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &props, &m_LROutBitmap);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
	m_D2DContext->SetTarget(m_LROutBitmap.Get());
}
void CStereoDirect2D::CreateSourceBitmap(UINT width, UINT height)
{
	HRESULT hr;
	D2D1_BITMAP_PROPERTIES1 props = {
		{ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE },
		96.0f,
		96.0f,
		D2D1_BITMAP_OPTIONS_NONE, //instead of CPU_WRITE
		nullptr
	};
	hr = m_D2DContext->CreateBitmap(D2D1_SIZE_U{ width, height * 2 }, nullptr, 0, &props, &m_LRInBitmap);
	if (FAILED(hr)) { ShowErrorInformation(hr);	return; }
}
void CStereoDirect2D::ReleaseDevice()
{
	if (m_LRInBitmap)  m_LRInBitmap.Reset();
	if (m_LROutBitmap) m_LROutBitmap.Reset();
	if (m_D2DContext)  m_D2DContext.Reset();
	if (m_D2DDevice)   m_D2DDevice.Reset();
	if (m_D2DFactory)  m_D2DFactory.Reset();
	if (m_SwapChain)   m_SwapChain.Reset();
	if (m_D3DContext)  m_D3DContext.Reset();
	if (m_D3DDevice)   m_D3DDevice.Reset();
}
void CStereoDirect2D::ReInitBitmap(ImageDimensions imageDimensions)
{
	if ((m_LastImageDimensions.Width != imageDimensions.Width) || 
		(m_LastImageDimensions.Height != imageDimensions.Height) || 
		(m_LastImageDimensions.Channels != imageDimensions.Channels))
	{
		if (m_LRInBitmap) m_LRInBitmap.Reset();
		CreateSourceBitmap((UINT)imageDimensions.Width, (UINT)imageDimensions.Height);
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_ImageDataUpdated.store(FALSE);
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_LastImageDimensions = { imageDimensions.Width, imageDimensions.Height, imageDimensions.Channels };
	}
}
void CStereoDirect2D::ResetDevice()
{
	ReleaseDevice();
	CreateDevice();
}
void CStereoDirect2D::SelectSourceRect(BOOL isLeft)
{
	D2D1_RECT_F sourceRectLeftVertical = D2D1::RectF(0.0, 0.0, (float)m_LastImageDimensions.Width, (float)m_LastImageDimensions.Height);
	D2D1_RECT_F sourceRectRightVertical = D2D1::RectF(0.0, (float)m_LastImageDimensions.Height, (float)m_LastImageDimensions.Width, (float)m_LastImageDimensions.Height * 2.0);
	if (isLeft)
	{
		if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == LEFT_ONLY))
		{
			if (m_SwapLR.load())
			{
				m_SourceRect = sourceRectRightVertical;
			}
			else
			{
				m_SourceRect = sourceRectLeftVertical;
			}
		}
	}
	else //IsRight
	{
		if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == RIGHT_ONLY))
		{
			if (m_SwapLR.load())
			{
				m_SourceRect = sourceRectLeftVertical;
			}
			else
			{
				m_SourceRect = sourceRectRightVertical;
			}
		}
	}
}
void CStereoDirect2D::DrawOnLRBitmap(CImage* leftImage, CImage* rightImage)
{
	HRESULT hr;
	if (!(leftImage && rightImage)) return;
	UINT width = leftImage->Width;
	UINT height = leftImage->Height;
	UINT channels = leftImage->Channels;
	D2D1_MAPPED_RECT mapped = {};
	D2D1_RECT_U leftRect = { 0, 0, width, height };
	D2D1_RECT_U rightRect = { 0, height, width, 2 * height };
	hr = m_LRInBitmap->CopyFromMemory(&leftRect, leftImage->PixelData.data(), width * channels);
	hr = m_LRInBitmap->CopyFromMemory(&rightRect, rightImage->PixelData.data(), width * channels);
}
void CStereoDirect2D::DrawImageRGB(CImage* leftImage, CImage* rightImage)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (!m_ImageDataUpdated.load())
	{
		if (m_LeftImage != NULL)
		{
			delete m_LeftImage;
			m_LeftImage = NULL;
		}
		if (m_RightImage != NULL)
		{
			delete m_RightImage;
			m_RightImage = NULL;
		}
		m_LeftImage = leftImage;
		m_RightImage = rightImage;
		m_ImageDataUpdated.store(TRUE);
	}
	lock1.unlock();
}
void CStereoDirect2D::Blt(BOOL isLeft, void* user, dSendSync sendSync)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (m_ImageDataUpdated.load())
	{
		if (m_LeftImage && m_RightImage)
		{
			ReInitBitmap({ m_LeftImage->Width, m_LeftImage->Height, m_LeftImage->Channels });
			DrawOnLRBitmap(m_LeftImage, m_RightImage);
		}
		m_ImageDataUpdated.store(FALSE);
	}
	lock1.unlock();
	m_D2DContext->BeginDraw();
	if (sendSync)
	{
		m_D2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		if (!((isLeft && m_LRBoth.load() == RIGHT_ONLY) || (!isLeft && m_LRBoth.load() == LEFT_ONLY)))
		{
			SelectSourceRect(isLeft);
			if (m_LRInBitmap)
			{
				m_D2DContext->DrawBitmap(m_LRInBitmap.Get(), m_D2DClientRect, 1.0f, D2D1_INTERPOLATION_MODE_LINEAR, &m_SourceRect);
			}
		}
	}
	else
	{
		m_D2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	}
	m_D2DContext->EndDraw();
	m_SwapChain->Present(1, 0);
	if (sendSync) sendSync(user, isLeft);
	MeasureFrequencyInHz();
}
INT CStereoDirect2D::GetFrequency()
{
	return m_FrequencyInHz.load();
}
void CStereoDirect2D::StereoLRBoth(int lrboth)
{
	m_LRBoth.store(lrboth);
}
void CStereoDirect2D::StereoSwapLR(BOOL swaplr)
{
	m_SwapLR.store(swaplr);
}
void CStereoDirect2D::StereoWindowSizeChanged()
{
	RECT clientRect;
	GetClientRect(m_HWnd, &clientRect);
	m_D2DClientRect = D2D1::RectF(static_cast<FLOAT>(clientRect.left), static_cast<FLOAT>(clientRect.top), static_cast<FLOAT>(clientRect.right), static_cast<FLOAT>(clientRect.bottom));
	ResizeSwapChain(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}
void CStereoDirect2D::ResizeSwapChain(UINT width, UINT height)
{
	if (!m_SwapChain) return;
	m_D2DContext->SetTarget(nullptr); //Unbind any D2D render target
	m_LROutBitmap.Reset();
	HRESULT hr = m_SwapChain->ResizeBuffers(
		0,                   // buffer count, 0 = keep
		width, height,       // new size
		DXGI_FORMAT_UNKNOWN, // keep format
		0                    // flags
	);
	if (FAILED(hr)) return;
	CreateRenderTarget();
}
void CStereoDirect2D::MeasureFrequencyInHz()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
	if (duration.count() > 0)
	{
		m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
	}
	m_LastTimeMeasuring = now;
}
void CStereoDirect2D::ShowErrorInformation(HRESULT hr)
{
	char msg[512] = { 0 }; //ANSI multibyte buffer
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		0, //Language neutral
		msg,
		sizeof(msg),
		nullptr
	);
	char buf[1024] = { 0 };
	sprintf_s(buf, sizeof(buf), "HRESULT = 0x%08X\n%s", hr, msg);
	MessageBoxA(nullptr, buf, "DirectX Error", MB_OK | MB_ICONERROR);
}