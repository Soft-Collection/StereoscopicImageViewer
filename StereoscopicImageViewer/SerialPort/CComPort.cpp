#include "stdafx.h"
#include "CComPort.h"
#include "CTools.h"

CComPort::CComPort(std::wstring comPortName)
{
    mHComMutex = new std::mutex();
    std::unique_lock<std::mutex> lock1(*mHComMutex); // Lock the mutex
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
    dcb.BaudRate = CBR_115200; // Set baud rate to 115200
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
    lock1.unlock();
}
CComPort::~CComPort()
{
    std::unique_lock<std::mutex> lock1(*mHComMutex); // Lock the mutex
    if (mHCom != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHCom);
        mHCom = INVALID_HANDLE_VALUE;
    }
    lock1.unlock();
    if (mHComMutex != nullptr)
    {
        delete mHComMutex;
        mHComMutex = nullptr;
    }
}
void CComPort::Send(BYTE* command, int length)
{
    std::unique_lock<std::mutex> lock1(*mHComMutex); // Lock the mutex
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
void CComPort::SendSync()
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::Frequency, 0x00 };
    command[3] = command[0] + command[1] + command[2];
    Send(command, 4);
}
void CComPort::SendGlassesTimeOffset(int offset)
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::GlassesTimeOffset, 0x00, 0x00 };
    command[3] = offset;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(command, 5);
}
void CComPort::SendTransparentTimePercent(int percent)
{
    BYTE command[] = { 0x54, 0xED, eCommandTypes::TransparentTimePercent, 0x00, 0x00 };
    command[3] = percent;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(command, 5);
}