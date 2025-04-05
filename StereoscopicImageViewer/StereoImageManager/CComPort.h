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
		Left = 0,
		Right = 1
	};
private:
	void Begin(std::wstring comPortName);
	void End();
	void Send(std::wstring comPortName, BYTE byteToSend);
public:
	CComPort();
	~CComPort();
	void SendCommand(std::wstring comPortName, eTransparentLenses transparentLenses);
	void SendGlassesTimeOffset(std::wstring comPortName, int offset);
	void SendTransparentTimePercent(std::wstring comPortName, int percent);
};

#endif // __CCOMPORT_H__