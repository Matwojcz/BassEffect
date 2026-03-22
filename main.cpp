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


int main()
{
    PaError err = Pa_Initialize();
    if(err != paNoError)
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }

    int numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
        err = numDevices;
        return -1;
    }

    for(unsigned int i=0; i<numDevices; i++ )
    {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "Index " << i << " " <<  deviceInfo->name << std::endl;
        std::cout << deviceInfo->defaultSampleRate << std::endl;

    }

    PaStream *stream;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                1,          /* no input channels */
                                1,          /* mono output */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                256,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                paBassOverdriveCallback, /* this is your callback function */
                                nullptr ); /*This is a pointer that will be passed to
                                                   your callback*/
    if(err != paNoError)
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }

    /* Start Streaming*/
    err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }

    /* Sleep and keep stream alive until enter clicked */
    std::cout << "Press Enter to stop.." << std::endl;
    std::cin.get();

    /* Start Streaming*/
    err = Pa_StopStream( stream );
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }


    err = Pa_Terminate();
    if( err != paNoError )
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

}



