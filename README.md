# BassEffect

A real-time bass guitar effects processor built in C++ from the ground up — no plugin frameworks, no walled gardens. Just PortAudio, signal processing maths, and C++.

## What it is

A standalone cross-platform application that processes audio from any connected audio interface in real time. Plug in your bass, run the program, hear the effect. Built as a learning project and portfolio piece, with the goal of understanding the full stack from audio I/O to DSP to UI — without hiding any of it behind a framework.

## Current state

- Real-time audio passthrough via PortAudio
- Automatic detection of Focusrite Scarlett interfaces, with graceful fallback to system default
- Overdrive effect using tanh waveshaping
- Compressor with RMS level detection, attack/release envelope smoothing, and configurable threshold
- Keyboard-driven effect toggling at runtime (`o` for overdrive, `c` for compressor, `q` to quit)
- Thread-safe effect switching using `std::atomic<bool>` across audio and main threads
- Clean class architecture — `AudioEngine`, `Overdrive`, `Compressor`, `ToneTester`, `UserInterface` as separate modules
- Built-in sine wave test tone generator for development without an instrument

## Roadmap

### Effects
- [x] Passthrough
- [x] Overdrive (tanh waveshaping)
- [x] Compressor (RMS detection, attack/release envelope)
- [ ] Reverb (Schroeder — comb filters + allpass filters)
- [ ] Wah-wah (time-varying bandpass filter)

### UI (Qt)
- [ ] Audio device selection at runtime
- [ ] Effect toggle switches
- [ ] Parameter knobs — gain, threshold, attack, release, reverb size
- [ ] Signal level visualisation

### Platform
- [x] macOS
- [ ] Windows
- [ ] Linux

## Building

```bash
brew install portaudio pkg-config   # macOS
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew ..
cmake --build .
```

## Usage

Run the executable. Available commands while running:

```
o — toggle overdrive
c — toggle compressor
q — quit
```

## Built with

- [PortAudio](http://www.portaudio.com/) — cross-platform audio I/O
- C++20
- CMake


