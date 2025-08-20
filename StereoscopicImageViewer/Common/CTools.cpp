#include "stdafx.h"
#include "CTools.h"

std::string CTools::ConvertUnicodeToMultibyte(const std::wstring& wstr)
{
    // Get the required buffer size
    int bufferSize = WideCharToMultiByte(
        CP_ACP,                // Code page (ANSI)
        0,                     // Flags
        wstr.c_str(),          // Input wide string
        -1,                    // Null-terminated string
        NULL,                  // No output buffer yet
        0,                     // Calculate buffer size
        NULL, NULL             // Default for unmapped characters
    );
    if (bufferSize == 0) {
        throw std::runtime_error("Failed to calculate buffer size for conversion.");
    }
    // Allocate buffer for multibyte string
    char* buffer = new char[bufferSize];
    // Perform the conversion
    WideCharToMultiByte(
        CP_ACP,                // Code page (ANSI)
        0,                     // Flags
        wstr.c_str(),          // Input wide string
        -1,                    // Null-terminated string
        buffer,                // Output buffer
        bufferSize,            // Buffer size
        NULL, NULL             // Default for unmapped characters
    );
    // Create a std::string from the buffer and free memory
    std::string result(buffer);
    delete[] buffer;
    return result;
}

INT CTools::GetRefreshmentRateOfMainMonitorInHz()
{
    DISPLAY_DEVICE displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    for (DWORD deviceIndex = 0; EnumDisplayDevices(NULL, deviceIndex, &displayDevice, 0); deviceIndex++)
    {
        if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
        {
            std::wcout << L"Primary monitor: " << displayDevice.DeviceString << std::endl;
            DEVMODE devMode;
            ZeroMemory(&devMode, sizeof(devMode));
            devMode.dmSize = sizeof(devMode);
            if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
            {
                if (devMode.dmDisplayFrequency > 1)
                {
                    return devMode.dmDisplayFrequency;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return -2;
            }
            break;
        }
    }
    return -3;
}
