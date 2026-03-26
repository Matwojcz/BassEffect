#include <iostream>
#include <portaudio.h>
#include <numbers>
#define SAMPLE_RATE (48000)

class ToneTester
{
public:
    float phase;
    float bassPhaser(const int sampleRate, float frequency)
    {
        phase +=  2.0f * std::numbers::pi * frequency / sampleRate;
        if (phase  >= 2.0f * std::numbers::pi) phase -= 2.0f * std::numbers::pi;
        return sin(phase);
    };
};

class Compressor // change to class Compressor and refactor
{
public:
    float gain = 1.0f;
    float rms = 0.0f;
    float tav = 0.1f;
    float xrms = 0.0f;
    float compThreshold = 0.3f;

    float compression(float sample)
    {
        xrms = (1 - tav) * xrms  + tav * (sample * sample);
        if (xrms < compThreshold)
        {
            gain = 1.0f;
        }
        else
        {
            gain = compThreshold / xrms;
            std::cout << "Compressing, rms " << xrms << " gain: "<< gain << std::endl;
        }
        return sample * gain;
    }
};

class Overdrive
{
public:
    float gain = 20.0f;
    float waveshaping(float inputBuffer)
    {
        float drive = inputBuffer * gain;
        // return tanh(drive) * (1.0f/gain);
        return tanh(drive);
    }

};
ToneTester toneTester;
Overdrive overdrive;
Compressor compressor;

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
        float phaser;
        std::cout << compressor.xrms << std::endl;
        // *outB++ = overdrive.waveshaping(*inB++);
        // *outB++ = overdrive.waveshaping(toneTester.bassPhaser(SAMPLE_RATE, 80.0f));
        phaser = overdrive.waveshaping(toneTester.bassPhaser(SAMPLE_RATE, 80.0f));
        *outB++ = state->compression(phaser);

        // *outB++ = *inB++;
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



