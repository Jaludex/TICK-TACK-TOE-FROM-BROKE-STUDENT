#include <headers/basics.hpp>

void clearConsole()
{
    #ifdef _WIN32
        // Windows
        std::system("cls");
    #else
        // Linux, macOS
        std::system("clear");
    #endif
}
