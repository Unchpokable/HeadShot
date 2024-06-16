#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>

#include "Configs.h"
#include "Input.h"
#include "ScreenScanner.h"

std::string JoinArgs(int argc, char* argv[])
{
    std::ostringstream oss;
    for(int i = 1; i < argc; i++) {
        if(i > 1) {
            oss << " ";
        }
        oss << argv[i];
    }
    return oss.str();
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }

    const std::string args = JoinArgs(argc, argv);
    const Configuration config = ParseCommandLineArgs(args);

    if(config.ShowConsole)
    {
        AllocConsole();
        std::cout << "Console allocated";
    }

    ScreenScanner scanner(config);

#ifndef _DEBUG
    scanner.AddCallback(MouseLeft);
#endif
    scanner.AddCallback([]() {
        std::cout << "[Info] Pixels detected" << "\n";
    });
    scanner.Start();

    while(true)
    {
        if(GetAsyncKeyState(VK_MENU) & 0x8000) {
            scanner.SetNeedScanning(true);
        } else
            scanner.SetNeedScanning(false);

        if (GetAsyncKeyState(VK_INSERT) & 0x8000)
            break;

        Sleep(1);
    }

    return 0;
}
