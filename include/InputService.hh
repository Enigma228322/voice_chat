#pragma once

#include <string>
#include <fstream>
#include <portaudio.h>

class InputService {
public:
    InputService(const std::string& filePath, int sampleRate = 96000, int framesPerBuffer = 256);
    ~InputService();

    bool initialize();
    bool start();
    void stop();
    void terminate();

private:
    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);

    std::ofstream outputFile;
    PaStream *stream;
    int sampleRate;
    int framesPerBuffer;
    std::string filePath;
};
