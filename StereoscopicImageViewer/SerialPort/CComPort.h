#ifndef __CCOMPORT_H__
#define __CCOMPORT_H__

#include <windows.h>
#include <iostream>
#include <mutex>

#define SYNC_LEFT_TRANSPARENT 0
#define SYNC_LEFT_OPAQUE 1
#define SYNC_RIGHT_TRANSPARENT 2
#define SYNC_RIGHT_OPAQUE 3

class CComPort
{
private:
	HANDLE mHCom;
	std::mutex* mMutexSend;
private:
	void Send(BYTE* command, int length);
public:
	CComPort(std::wstring comPortName);
	~CComPort();
	void SendSync(int syncType);
	void SendGlassesTimeOffset(int offset);
};

#endif // __CCOMPORT_H__