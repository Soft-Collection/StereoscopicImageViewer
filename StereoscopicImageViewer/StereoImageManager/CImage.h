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
	CImage();
	~CImage();
	static void LoadImage(std::wstring filePath, int& width, int& height, int& channels, std::vector<BYTE>& pixelData, bool isLeft);
	static void LoadImage(CImage* img);
};

#endif // __CIMAGE_H__