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
