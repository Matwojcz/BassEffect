//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#ifndef BASSOGVERDRIVE_TONETESTER_H
#define BASSOGVERDRIVE_TONETESTER_H


class ToneTester
{
public:
    float phase = 0.0f;
    float bassPhaser(int sampleRate, float frequency);
};


#endif //BASSOGVERDRIVE_TONETESTER_H