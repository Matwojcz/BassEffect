//
// Created by Mateusz Wojczynski on 30/03/2026.
//
#include "Overdrive.h"
#include <cmath>

float Overdrive::waveshaping(float inputBuffer)
{
    float drive = inputBuffer * gain;
    return std::tanh(drive);
}
