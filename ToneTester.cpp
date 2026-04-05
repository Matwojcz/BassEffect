//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#include "ToneTester.h"
#include <numbers>
#include <cmath>

float ToneTester::bassPhaser(const int sampleRate, float frequency)
{
    phase +=  2.0f * std::numbers::pi * frequency / sampleRate;
    if (phase  >= 2.0f * std::numbers::pi) phase -= 2.0f * std::numbers::pi;
    return std::sin(phase);
};