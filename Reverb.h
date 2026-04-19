//
// Created by Mateusz Wojczynski on 06/04/2026.
//

#ifndef BASSOGVERDRIVE_REVERB_H
#define BASSOGVERDRIVE_REVERB_H


class Reverb
{
public:
    // y(n) = cx(n) + gy(n-M) // comb filter equation
    // y(n) = b_0 * x(n) + x(n-M) - a_m * y(n-M) // equation for allpass comb filter
    float delayTime = 0; //in miliseconds
    float delay(float sample);


};


#endif //BASSOGVERDRIVE_REVERB_H