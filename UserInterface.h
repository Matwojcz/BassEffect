//
// Created by Mateusz Wojczynski on 30/03/2026.
//
#ifndef BASSOGVERDRIVE_USERINTERFACE_H
#define BASSOGVERDRIVE_USERINTERFACE_H
#include <atomic>
#include <termios.h>
#include <unistd.h>

class UserInterface
{
public:
    std::atomic<bool> overdriveEnabled{false};
    std::atomic<bool> compressorEnabled{false};
    bool exitFlag = true;

    static void setRawMode(bool enable);
    // ---- logic for toggling effects ------
    void keyboardInput();


};


#endif //BASSOGVERDRIVE_USERINTERFACE_H