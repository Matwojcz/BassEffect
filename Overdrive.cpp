//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#include "Overdrive.h"

#include <__math/hyperbolic_functions.h>

float Overdrive::waveshaping(float inputBuffer)
{
    float drive = inputBuffer * gain;
    return std::__math::tanh(drive);
}
