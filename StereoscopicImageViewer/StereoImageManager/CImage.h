#ifndef __CIMAGE_H__
#define __CIMAGE_H__

#include <windows.h>
#include <iostream>
#include <vector>
#include <wincodec.h>
#include <cwchar>
#include <fstream>

class CImage
{
public:
	std::wstring FilePath;
	int Width;
	int Height;
	int Channels;
	std::vector<BYTE> PixelData;
	bool IsLeft;
public:
	CImage(std::wstring FilePath, int Width, int Height, int Channels, std::vector<BYTE> PixelData, bool IsLeft);
	~CImage();
	static CImage* LoadImage(std::wstring filePath, bool isLeft);
};

#endif // __CIMAGE_H__