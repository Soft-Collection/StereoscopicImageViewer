#include "CStereoImageManager.h"

extern "C" 
{
	__declspec(dllexport) void* StereoImageManagerCreateNew(HWND hWnd);
	__declspec(dllexport) void StereoImageManagerDispose(void* handle);
	__declspec(dllexport) void StereoImageManagerDrawImage(void* handle, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
	__declspec(dllexport) void StereoImageManagerStart(void* handle);
	__declspec(dllexport) void StereoImageManagerStop(void* handle);
	__declspec(dllexport) BOOL StereoImageManagerIsStarted(void* handle);
	__declspec(dllexport) int StereoImageManagerGetFrequency(void* handle);
	__declspec(dllexport) void StereoImageManagerSetCOMPort(void* handle, LPCWSTR comPort);
	__declspec(dllexport) void StereoImageManagerSetGlassesTimeOffset(void* handle, int offset);
	__declspec(dllexport) void StereoImageManagerSetTransparentTimePercent(void* handle, int percent);
}