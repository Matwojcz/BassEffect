//
// Created by Mateusz Wojczynski on 30/03/2026.
//

#ifndef BASSOGVERDRIVE_COMPRESSOR_H
#define BASSOGVERDRIVE_COMPRESSOR_H

class Compressor
{
public:
    float gain = 1.0f;
    float rms = 0.0f;
    float tav = 0.1f;
    float xrms = 0.0f;
    float compThreshold = 0.3f;
    float attack{0.01f};
    float release{0.001f};
    float targetGain{0.0f};
    float compression(float sample);
};


#endif //BASSOGVERDRIVE_COMPRESSOR_H

