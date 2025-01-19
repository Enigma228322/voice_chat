cmake_minimum_required(VERSION 3.10)

project(AudioTransfer)

# Add PortAudio
add_subdirectory(libs/portaudio)
add_subdirectory(libs/json)

# Include directories
include_directories(libs/portaudio/include)
include_directories(include)

# Add library for InputService
add_library(InputService src/InputService.cc)
# Add library for OutputService
add_library(OutputService src/OutputService.cc)

# Add executable
add_executable(audio_input src/input_main.cc)
# Add executable
add_executable(audio_output src/output_main.cc)

# Link libraries
target_link_libraries(audio_input PRIVATE InputService portaudio nlohmann_json::nlohmann_json)
# Link libraries
target_link_libraries(audio_output PRIVATE OutputService portaudio nlohmann_json::nlohmann_json)
