#include <iostream>
#include <portaudio.h>
#include <atomic>
#include "Compressor.h"
#include "Overdrive.h"
#include "ToneTester.h"
#include "UserInterface.h"
#define SAMPLE_RATE (48000)

ToneTester toneTester;
Overdrive overdrive;
Compressor compressor;
UserInterface userInterface;

static int paBassOverdriveCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    Compressor *state = static_cast<Compressor*>(userData);
    auto* outB = static_cast<float*>(outputBuffer);
    auto* inB = static_cast<const float*>(inputBuffer);
    for (unsigned int i = 0; i < framesPerBuffer; i++ )
    {
        float phaser = toneTester.bassPhaser(SAMPLE_RATE, 80.0f);
        float sample = phaser;

        if (userInterface.overdriveEnabled)
        {
            sample = overdrive.waveshaping(sample);
        }

        if (userInterface.compressorEnabled)
        {
            sample = compressor.compression(sample);
        }

        *outB++ = sample;
    }

    return 0;
}

void checkPaErrors(const PaError errorHandling)
{

    if (errorHandling != paNoError)
    {
        throw std::runtime_error(std::string("PortAudio error: \n") + Pa_GetErrorText(errorHandling));
    }
}

int main()
{
    try
    {
        // Initializes portAudio
        checkPaErrors(Pa_Initialize());
        const int numDevices = Pa_GetDeviceCount();
        if( numDevices < 0 )
        {
            checkPaErrors(numDevices);
        }

        int inputDeviceIndex = Pa_GetDefaultInputDevice();
        int outputDeviceIndex = Pa_GetDefaultOutputDevice();
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

        // Parameters needed for the correct device to be used
        PaStreamParameters outputParameters;
        PaStreamParameters inputParameters;


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

        PaStream *stream;
        /* Open an audio I/O stream. */
        checkPaErrors(Pa_OpenStream(&stream,
                             &inputParameters,
                             &outputParameters,
                             SAMPLE_RATE,
                             64,
                             paNoFlag,
                             paBassOverdriveCallback,
                             &compressor)
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
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}