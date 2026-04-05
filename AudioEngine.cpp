//
// Created by Mateusz Wojczynski on 02/04/2026.
//

#include "AudioEngine.h"




void AudioEngine::checkPaErrors(const PaError errorHandling)
{

    if (errorHandling != paNoError)
    {
        throw std::runtime_error(std::string("PortAudio error: \n") + Pa_GetErrorText(errorHandling));
    }
}

int AudioEngine::EffectCallBack( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    // Compressor *state = static_cast<Compressor*>(userData);
    auto *engine = static_cast<AudioEngine*>(userData);
    auto* outB = static_cast<float*>(outputBuffer);
    for (unsigned int i = 0; i < framesPerBuffer; i++ )
    {

        float phaser = engine->toneTester.bassPhaser(SAMPLE_RATE, 80.0f);
        float sample = phaser;
        if (engine->userInterface.overdriveEnabled)
        {
            sample = engine->overdrive.waveshaping(sample);
        }

        if (engine->userInterface.compressorEnabled)
        {
            sample = engine->compressor.compression(sample);
        }
        // std::cout << "callback running, sample: " << sample << std::endl;
        // *outB++ = sample;o
        *outB++ = sample;
    }

    return 0;
}

void AudioEngine::run()
{
    checkPaErrors(Pa_Initialize());
    const int numDevices = Pa_GetDeviceCount();
    int inputDeviceIndex = Pa_GetDefaultInputDevice();
    int outputDeviceIndex = Pa_GetDefaultOutputDevice();


    if( numDevices < 0 )
    {
        checkPaErrors(numDevices);
    }

    for(unsigned int i=0; i<numDevices; i++ )
    {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "Index " << i << " " <<  deviceInfo->name << std::endl;
        std::cout << deviceInfo->defaultSampleRate << std::endl;

        const std::string nameFound(deviceInfo->name);
        if (nameFound.find("Scarlett") != std::string::npos)
        {
            std::cout << "Index of Scarlett is = " << i << std::endl;
            inputDeviceIndex = i;
            outputDeviceIndex = i;
        }
    }
    std::cout << outputDeviceIndex << " test printout " << std::endl;

    // input parameters
    inputParameters.device = inputDeviceIndex;
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDeviceIndex)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    // output parameters
    outputParameters.device = outputDeviceIndex;
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDeviceIndex)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    checkPaErrors(Pa_OpenStream(&stream,
                           &inputParameters,
                           &outputParameters,
                           SAMPLE_RATE,
                           64,
                           paNoFlag,
                           EffectCallBack,
                           this)
   );

    /* Start Streaming*/
    checkPaErrors(Pa_StartStream( stream ));
    UserInterface::setRawMode(true);
    userInterface.keyboardInput();
    UserInterface::setRawMode(false);

    /* Stop Streaming*/
    checkPaErrors(Pa_StopStream( stream ));
    /* Close Stream*/
    checkPaErrors(Pa_CloseStream( stream ));
    /* Terminating PortAudio*/
    checkPaErrors(Pa_Terminate());

}
