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
        End();
        return;
    }
    dcb.BaudRate = CBR_115200; // Set baud rate to 115200
    dcb.ByteSize = 8;          // 8 data bits
    dcb.Parity = NOPARITY;     // No parity
    dcb.StopBits = ONESTOPBIT; // 1 stop bit
    if (!SetCommState(mHCom, &dcb)) {
        std::cerr << "Error: Unable to configure COM port." << std::endl;
        End();
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
void CComPort::Send(std::wstring comPortName, BYTE* command)
{
    if (mHCom == NULL) Begin(comPortName);
    if (mHCom != NULL)
    {
        DWORD bytesWritten;
        if (!WriteFile(mHCom, command, 5, &bytesWritten, NULL)) {
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
void CComPort::SendFrequency(std::wstring comPortName, int frequency)
{
    BYTE command[] = { 0x54, 0xED, 0x00, 0x00, 0x00 };
    command[2] = eCommandTypes::Frequency;
    command[3] = frequency;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(comPortName, command);
}
void CComPort::SendGlassesTimeOffset(std::wstring comPortName, int offset)
{
    BYTE command[] = { 0x54, 0xED, 0x00, 0x00, 0x00 };
    command[2] = eCommandTypes::GlassesTimeOffset;
    command[3] = offset;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(comPortName, command);
}
void CComPort::SendTransparentTimePercent(std::wstring comPortName, int percent)
{
    BYTE command[] = { 0x54, 0xED, 0x00, 0x00, 0x00 };
    command[2] = eCommandTypes::TransparentTimePercent;
    command[3] = percent;
    command[4] = command[0] + command[1] + command[2] + command[3];
    Send(comPortName, command);
}