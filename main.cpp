#include <iostream>
#include <portaudio.h>
#include <numbers>
#include <atomic>
#include <termios.h>
#include <unistd.h>
#define SAMPLE_RATE (48000)

// ---  global flags for enabling effects --- /
std::atomic<bool> overdriveEnabled{false};
std::atomic<bool> compressorEnabled{false};

/* Old unix code to set mac terminal to react to keypresses without waiting for enter  */
void setRawMode(bool enable) {
    static struct termios oldSettings;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldSettings);
        struct termios newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    }
}


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

class Overdrive
{
public:
    float gain = 20.0f;
    float waveshaping(float inputBuffer)
    {
        float drive = inputBuffer * gain;
        return tanh(drive);
    }


};

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
    //gain = (1 - 0.01)* 1 + (0).01 * 1)
    //gain = 0.99 + 0.01
    float compression(float sample)
    {
        xrms = (1 - tav) * xrms  + tav * (sample * sample);
        if (xrms < compThreshold)
        {
            // gain = 1.0f;
            targetGain = 1.0f;
            gain = (1 - release) * gain + release * targetGain;
        }

        else
        {
            targetGain = compThreshold / xrms;
            gain = (1 - attack) * gain + attack * targetGain;
            // std::cout << "Compressing, rms " << xrms << " gain: "<< gain << std::endl;
        }
        return sample * gain;
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
        float phaser = toneTester.bassPhaser(SAMPLE_RATE, 80.0f);
        float sample = phaser;

        if (overdriveEnabled)
        {
            sample = overdrive.waveshaping(sample);
        }

        if (compressorEnabled)
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


        // ---- loop for enabling effects ------ //
        bool exitFlag = true;
        std::cout << "Press 'o' for Overdrive..." << std::endl;
        std::cout << "Press 'c' for Compressor..." << std::endl;
        std::cout << "Press 'q' to quit..." << std::endl;

        setRawMode(true);

        while (exitFlag)
        {
            char keyPress = std::cin.get();

            if (keyPress == 'q')
            {
                exitFlag = false;
            }

            // -- keypress for overdrive -- //
            if (keyPress == 'o')
            {
               overdriveEnabled =! overdriveEnabled;
                if (overdriveEnabled == true) std::cout << "Overdrive is on" << std::endl;
                else std::cout << "Overdrive is off" << std::endl;
            }

            // -- keypress for compressor -- //
            if (keyPress == 'c')
            {
              compressorEnabled =! compressorEnabled;
                if (compressorEnabled == true) std::cout << "Compressor is on" << std::endl;
                else std::cout << "Compressor is off" << std::endl;
            }
        }
        setRawMode(false);

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



