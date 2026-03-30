//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#include "UserInterface.h"
#include <iostream>

void UserInterface::setRawMode(bool enable)
{
    static struct termios oldSettings;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldSettings);
        struct termios newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    }
}

void UserInterface::keyboardInput()
{
    std::cout << "Press 'o' for Overdrive..." << std::endl;
    std::cout << "Press 'c' for Compressor..." << std::endl;
    std::cout << "Press 'q' to quit..." << std::endl;
    while (exitFlag)
    {
        char keyPress = std::cin.get();

        if (keyPress == 'q')
        {
            exitFlag = false;
        }

        // -- keypress for overdrive -- //
        if (keyPress == 'o')
        {
            overdriveEnabled =! overdriveEnabled;
            if (overdriveEnabled == true) std::cout << "Overdrive is on" << std::endl;
            else std::cout << "Overdrive is off" << std::endl;
        }

        // -- keypress for compressor -- //
        if (keyPress == 'c')
        {
            compressorEnabled =! compressorEnabled;
            if (compressorEnabled == true) std::cout << "Compressor is on" << std::endl;
            else std::cout << "Compressor is off" << std::endl;
        }
    }
}
