#ifndef __CCOMPORT_H__
#define __CCOMPORT_H__

#include <windows.h>
#include <iostream>

class CComPort
{
private:
	HANDLE mHCom;
public:
	enum eTransparentLenses : int
	{
		None = 0,
		Left = 1,
		Right = 2
	};
private:
	void Begin(std::wstring comPortName);
	void End();
public:
	CComPort();
	~CComPort();
	void SendCommand(std::wstring comPortName, eTransparentLenses transparentLenses);
	void SendGlassesTimeOffset(std::wstring comPortName, int offset);
};

#endif // __CCOMPORT_H__