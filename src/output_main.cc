#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "OutputService.hh"

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

        // ������� � �������������� ������ ������
        OutputService outputService(audioFilePath);

        if (!outputService.initialize()) {
            return 1;
        }

        if (!outputService.start()) {
            outputService.terminate();
            return 1;
        }

        outputService.stop();
        outputService.terminate();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}