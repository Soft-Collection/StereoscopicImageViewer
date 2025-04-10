#ifndef __CCOMPORT_H__
#define __CCOMPORT_H__

#include <windows.h>
#include <iostream>

class CComPort
{
private:
	HANDLE mHCom;
public:
	enum eCommandTypes : int
	{
		Frequency = 0,
		GlassesTimeOffset = 1,
		TransparentTimePercent = 2
	};
private:
	void Begin(std::wstring comPortName);
	void End();
	void Send(std::wstring comPortName, BYTE* command);
public:
	CComPort();
	~CComPort();
	void SendFrequency(std::wstring comPortName, int frequency);
	void SendGlassesTimeOffset(std::wstring comPortName, int offset);
	void SendTransparentTimePercent(std::wstring comPortName, int percent);
};

#endif // __CCOMPORT_H__