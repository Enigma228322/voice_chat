#include "InputService.hh"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// ��� �������� �������������
using json = nlohmann::json;

std::string getAudioFilePath(const std::string& configFilePath) {
    // ��������� ���� ������������
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open config file.");
    }

    // ������ JSON �� �����
    json config;
    configFile >> config;

    // �������� ���� � ����������
    return config.at("audioFilePath").get<std::string>();
}

int main() {
    try {
        // �������� ���� � ���������� �� ����������������� �����
        std::string audioFilePath = getAudioFilePath("config.json");

        // ������� � �������������� ������ �����
        InputService inputService(audioFilePath);

        if (!inputService.initialize()) {
            return 1;
        }

        if (!inputService.start()) {
            inputService.terminate();
            return 1;
        }

        inputService.stop();
        inputService.terminate();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}