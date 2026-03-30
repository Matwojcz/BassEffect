//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#include "Compressor.h"


float Compressor::compression(float sample)
{
    xrms = (1 - tav) * xrms  + tav * (sample * sample);
    if (xrms < compThreshold)
    {
        // gain = 1.0f;
        targetGain = 1.0f;
        gain = (1 - release) * gain + release * targetGain;
    }

    else
    {
        targetGain = compThreshold / xrms;
        gain = (1 - attack) * gain + attack * targetGain;
        // std::cout << "Compressing, rms " << xrms << " gain: "<< gain << std::endl;
    }
    return sample * gain;
}