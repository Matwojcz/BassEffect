# BassEffect

A real-time bass guitar effects processor built in C++ from the ground up — no plugin frameworks, no walled gardens. Just PortAudio, signal processing maths, and C++.

## What it is

A standalone cross-platform application that processes audio from any connected audio interface in real time. Plug in your bass, run the program, hear the effect. Built as a learning project and portfolio piece, with the goal of understanding the full stack from audio I/O to DSP to UI.

## Current state

- Real-time audio passthrough via PortAudio
- Automatic detection of Focusrite Scarlett interfaces, with graceful fallback to system default
- Overdrive effect using tanh waveshaping with gain and output level control

## Roadmap

### Effects

- [x] Passthrough
- [x] Overdrive (tanh waveshaping)
- [ ] Compressor (RMS level detection, attack/release envelope)
- [ ] Wah-wah (time-varying filter)
- [ ] UI with Qt

### Architecture

- [ ] Effect chain — combine effects in series
- [ ] Per-effect enable/disable at runtime

### UI (Qt)

- [ ] Audio device selection at runtime
- [ ] Effect toggle switches
- [ ] Parameter knobs — gain, threshold, attack, release
- [ ] Signal level visualisation

### Platform

- [x] macOS
- [ ] Windows
- [ ] Linux

## Built with

- [PortAudio](http://www.portaudio.com/) — cross-platform audio I/O
- C++17
- CMake
- Qt (planned)


## Why

Built to understand real-time audio processing from first principles — callbacks, sample buffers, DSP maths, signal chains. Every line written and reasoned through, not generated or copied from a framework tutorial.

