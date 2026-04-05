//
// Created by Mateusz Wojczynski on 02/04/2026.
//

#ifndef BASSOGVERDRIVE_AUDIOENGINE_H
#define BASSOGVERDRIVE_AUDIOENGINE_H
#include <iostream>
#include <portaudio.h>
#include "Compressor.h"
#include "ToneTester.h"
#include "Overdrive.h"
#include "UserInterface.h"
#define SAMPLE_RATE (48000)
class AudioEngine
{

private:
    PaStream *stream;
    ToneTester toneTester;
    Overdrive overdrive;
    Compressor compressor;
    UserInterface userInterface;
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
public:
    AudioEngine(): stream(nullptr){}
    // AudioEngine* engine = static_cast<AudioEngine*>(userData);

    static void checkPaErrors(const PaError errorHandling);

    void run();

    static int EffectCallBack( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );


};


#endif //BASSOGVERDRIVE_AUDIOENGINE_H