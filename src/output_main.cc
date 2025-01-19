#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "OutputService.hh"

// Для удобства использования
using json = nlohmann::json;

std::string getAudioFilePath(const std::string& configFilePath) {
    // Открываем файл конфигурации
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open config file.");
    }

    // Читаем JSON из файла
    json config;
    configFile >> config;

    // Получаем путь к аудиофайлу
    return config.at("audioFilePath").get<std::string>();
}

int main() {
    try {
        // Получаем путь к аудиофайлу из конфигурационного файла
        std::string audioFilePath = getAudioFilePath("config.json");

        // Создаем и инициализируем сервис вывода
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