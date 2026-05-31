#pragma once

#ifdef _WIN32
#include <windows.h>

inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif