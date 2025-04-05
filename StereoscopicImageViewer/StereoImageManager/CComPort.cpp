#include "stdafx.h"
#include "CComPort.h"

#include "../Common/CTools.h"

CComPort::CComPort()
{
    mHCom = NULL;
}
CComPort::~CComPort()
{
    End();
}
void CComPort::Begin(std::wstring comPortName)
{
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
        CloseHandle(mHCom);
        return;
    }
    dcb.BaudRate = CBR_115200; // Set baud rate to 9600
    dcb.ByteSize = 8;        // 8 data bits
    dcb.Parity = NOPARITY;   // No parity
    dcb.StopBits = ONESTOPBIT; // 1 stop bit
    if (!SetCommState(mHCom, &dcb)) {
        std::cerr << "Error: Unable to configure COM port." << std::endl;
        CloseHandle(mHCom);
        return;
    }
}
void CComPort::End()
{
    if (mHCom != NULL)
    {
        CloseHandle(mHCom);
        mHCom = NULL;
    }
}
void CComPort::Send(std::wstring comPortName, BYTE byteToSend)
{
    if (mHCom == NULL) Begin(comPortName);
    if (mHCom != NULL)
    {
        DWORD bytesWritten;
        if (!WriteFile(mHCom, &byteToSend, 1, &bytesWritten, NULL)) {
            std::cerr << "Error: Unable to write to COM port." << std::endl;
            End();
            return;
        }
        if (!FlushFileBuffers(mHCom)) {
            std::cerr << "Error: Unable to flush COM port." << std::endl;
            End();
            return;
        }
    }
}
void CComPort::SendCommand(std::wstring comPortName, eTransparentLenses transparentLenses)
{
    BYTE byteToSend = 76; // 76 = L (Left).
    if (transparentLenses == eTransparentLenses::Left)
    {
        byteToSend = 76; // 76 = L (Left).
    }
    else if (transparentLenses == eTransparentLenses::Right)
    {
        byteToSend = 82; // 82 = R (Right).
    }
    Send(comPortName, byteToSend);
}
void CComPort::SendGlassesTimeOffset(std::wstring comPortName, int offset)
{
    Send(comPortName, (BYTE)(offset + 100));
}
void CComPort::SendTransparentTimePercent(std::wstring comPortName, int percent)
{
    Send(comPortName, (BYTE)(percent + 180));
}