#ifndef __CTOOLS_H__
#define __CTOOLS_H__

#include <iostream>
#include <string>
#include <windows.h>

class CTools
{
public:
	static std::string ConvertUnicodeToMultibyte(const std::wstring& wstr);
};

#endif // __CTOOLS_H__
