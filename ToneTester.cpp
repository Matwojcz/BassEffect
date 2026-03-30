//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#include "ToneTester.h"
#include <numbers>
#include <__math/trigonometric_functions.h>

float ToneTester::bassPhaser(const int sampleRate, float frequency)
{
    phase +=  2.0f * std::numbers::pi * frequency / sampleRate;
    if (phase  >= 2.0f * std::numbers::pi) phase -= 2.0f * std::numbers::pi;
    return std::__math::sin(phase);
};