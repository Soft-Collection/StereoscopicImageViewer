#include "stdafx.h"
#include "CComPort.h"
#include "../Common/CTools.h"

CComPort::CComPort(std::wstring comPortName)
{
    mMutexSend = new std::mutex();
    //-------------------------------------------------------------------
    std::wstring portNameW = std::wstring(L"\\\\.\\") + comPortName;
    std::string portNameA = CTools::ConvertUnicodeToMultibyte(portNameW);
    // Open the COM port
    mHCom = CreateFile(portNameA.c_str(),
        GENERIC_WRITE,
        0, // No sharing
        NULL, // No security
        OPEN_EXISTING,
        0, // Non-overlapped mode
        NULL);
    if (mHCom == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open COM port." << std::endl;
        return;
    }
    // Configure the COM port
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(mHCom, &dcb)) {
        std::cerr << "Error: Unable to get COM port state." << std::endl;
        if (mHCom != INVALID_HANDLE_VALUE)
        {
            CloseHandle(mHCom);
            mHCom = INVALID_HANDLE_VALUE;
        }
        return;
    }
    dcb.BaudRate = CBR_9600; // Set baud rate to 115200
    dcb.ByteSize = 8;          // 8 data bits
    dcb.Parity = NOPARITY;     // No parity
    dcb.StopBits = ONESTOPBIT; // 1 stop bit
    if (!SetCommState(mHCom, &dcb)) {
        std::cerr << "Error: Unable to configure COM port." << std::endl;
        if (mHCom != INVALID_HANDLE_VALUE)
        {
            CloseHandle(mHCom);
            mHCom = INVALID_HANDLE_VALUE;
        }
        return;
    }
}
CComPort::~CComPort()
{
    if (mHCom != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHCom);
        mHCom = INVALID_HANDLE_VALUE;
    }
    if (mMutexSend != nullptr)
    {
        delete mMutexSend;
        mMutexSend = nullptr;
    }
}
void CComPort::Send(BYTE* command, int length)
{
    std::unique_lock<std::mutex> lock1(*mMutexSend); // Lock the mutex
    if (mHCom != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten;
        if (!WriteFile(mHCom, command, length, &bytesWritten, NULL)) {
            std::cerr << "Error: Unable to write to COM port." << std::endl;
            if (mHCom != INVALID_HANDLE_VALUE)
            {
                CloseHandle(mHCom);
                mHCom = INVALID_HANDLE_VALUE;
            }
            return;
        }
        if (!FlushFileBuffers(mHCom)) {
            std::cerr << "Error: Unable to flush COM port." << std::endl;
            if (mHCom != INVALID_HANDLE_VALUE)
            {
                CloseHandle(mHCom);
                mHCom = INVALID_HANDLE_VALUE;
            }
            return;
        }
    }
    lock1.unlock();
}
void CComPort::SendSync(int syncType)
{
    BYTE command[1];
    command[0] = syncType + 105;
    Send(command, 1);
}
void CComPort::SendGlassesTimeOffset(int offset)
{
    BYTE command[1];
    command[0] = offset;
    Send(command, 1);
}