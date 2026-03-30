//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#ifndef BASSOGVERDRIVE_OVERDRIVE_H
#define BASSOGVERDRIVE_OVERDRIVE_H

class Overdrive
{
public:
    float gain = 20.0f;
    float waveshaping(float inputBuffer);
};


#endif //BASSOGVERDRIVE_OVERDRIVE_H