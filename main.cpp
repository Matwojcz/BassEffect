#include <iostream>
#include <portaudio.h>
#define SAMPLE_RATE (48000)

static int paBassOverdriveCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    auto* outB = static_cast<float*>(outputBuffer);
    auto* inB = static_cast<const float*>(inputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; i++ )
    {
        *outB++ = *inB++;
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

        for(unsigned int i=0; i<numDevices; i++ )
        {
            const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
            std::cout << "Index " << i << " " <<  deviceInfo->name << std::endl;
            std::cout << deviceInfo->defaultSampleRate << std::endl;

        }

        // Parameters needed for the correct device to be used
        PaStreamParameters outputParameters;
        PaStreamParameters inputParameters;
        constexpr int ioDeviceIndex = 2;
        // input parameters
        inputParameters.device = 2;
        inputParameters.channelCount = 1;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(ioDeviceIndex)->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;
        // output parameters
        outputParameters.device = 2;
        outputParameters.channelCount = 1;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(ioDeviceIndex)->defaultLowOutputLatency;
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
                             nullptr)
     );

        /* Start Streaming*/
        checkPaErrors(Pa_StartStream( stream ));
        /* Sleep and keep stream alive until enter clicked */
        std::cout << "Press Enter to stop.." << std::endl;
        std::cin.get();
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



