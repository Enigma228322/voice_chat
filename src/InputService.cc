#include "InputService.hh"
#include <iostream>
#include <fstream>

InputService::InputService(const std::string& filePath, int sampleRate, int framesPerBuffer)
    : stream(nullptr), sampleRate(sampleRate), framesPerBuffer(framesPerBuffer), filePath(filePath) {}

InputService::~InputService() {
    terminate();
}

bool InputService::initialize() {
    // Open output file
    outputFile.open(filePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return false;
    }

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    // Open an audio input stream
    err = Pa_OpenDefaultStream(&stream,
                               1,          // Input channels
                               0,          // Output channels (0 means no output)
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

bool InputService::start() {
    if (!stream) {
        std::cerr << "Stream is not initialized." << std::endl;
        return false;
    }

    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    std::cout << "Recording... Press Enter to stop." << std::endl;
    std::cin.get();

    return true;
}

void InputService::stop() {
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

    if (outputFile.is_open()) {
        outputFile.close();
    }
}

void InputService::terminate() {
    stop();
    Pa_Terminate();
}

int InputService::audioCallback(const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData) {
    InputService *service = static_cast<InputService*>(userData);
    const float *in = (const float*)inputBuffer;

    if (inputBuffer == nullptr) {
        return paContinue;
    }

    // Write input data to file
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        service->outputFile.write(reinterpret_cast<const char*>(&in[i]), sizeof(in[i]));
    }

    return paContinue;
}