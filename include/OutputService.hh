#pragma once

#include <vector>
#include <portaudio.h>

class OutputService {
public:
    OutputService(const std::string& filePath, int sampleRate = 96000, int framesPerBuffer = 256);
    ~OutputService();

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

    std::vector<float> audioData;
    size_t currentFrame;
    PaStream *stream;
    int sampleRate;
    int framesPerBuffer;
    std::string filePath;
};
