#include <iostream>

#include "AudioEngine.h"
#define SAMPLE_RATE (48000)

int main()
{
    try
    {
        // Initializes portAudio
        AudioEngine engine;
        engine.run();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;

}
