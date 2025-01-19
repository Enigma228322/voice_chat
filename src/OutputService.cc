#include "OutputService.hh"
#include <iostream>
#include <fstream>
#include <algorithm>

OutputService::OutputService(const std::string& filePath, int sampleRate, int framesPerBuffer)
    : currentFrame(0), stream(nullptr), sampleRate(sampleRate), framesPerBuffer(framesPerBuffer), filePath(filePath) {}

OutputService::~OutputService() {
    terminate();
}

bool OutputService::initialize() {
    // Open input file
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return false;
    }

    // Read audio data from file
    float sample;
    while (inputFile.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
        audioData.push_back(sample);
    }
    inputFile.close();

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    // Open an audio output stream
    err = Pa_OpenDefaultStream(&stream,
                               0,          // Input channels (0 means no input)
                               1,          // Output channels
                               paFloat32,  // Sample format
                               sampleRate,
                               framesPerBuffer,
                               audioCallback,
                               this);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return false;
    }

    return true;
}

bool OutputService::start() {
    if (!stream) {
        std::cerr << "Stream is not initialized." << std::endl;
        return false;
    }

    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    std::cout << "Playing audio... Press Enter to stop." << std::endl;
    std::cin.get();

    return true;
}

void OutputService::stop() {
    if (stream) {
        PaError err = Pa_StopStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        }

        err = Pa_CloseStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        }

        stream = nullptr;
    }
}

void OutputService::terminate() {
    stop();
    Pa_Terminate();
}

int OutputService::audioCallback(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData) {
    OutputService *service = static_cast<OutputService*>(userData);
    float *out = (float*)outputBuffer;
    size_t framesLeft = service->audioData.size() - service->currentFrame;

    if (framesLeft < framesPerBuffer) {
        std::fill(out, out + framesPerBuffer, 0);
        return paComplete;
    }

    std::copy(service->audioData.begin() + service->currentFrame, service->audioData.begin() + service->currentFrame + framesPerBuffer, out);
    service->currentFrame += framesPerBuffer;

    return paContinue;
}