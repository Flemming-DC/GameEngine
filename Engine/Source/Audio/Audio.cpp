#define MINIAUDIO_IMPLEMENTATION
#include "Audio.h"
#include "ErrorChecker.h"
#include <filesystem>
Shorts;

static ma_engine engine;
static ma_engine_config engineConfig;
static bool initialized = false;


void Setup() // initialized on first use
{
    Deny(initialized,
        "Attempting to initialize audio twice");
    initialized = true;
    engineConfig = ma_engine_config_init();
    engineConfig.listenerCount = 1;
    ma_result result = ma_engine_init(&engineConfig, &engine);
    Assert(result == MA_SUCCESS,
        "Failed to initialize audio engine. ", result);

}

void Audio::Play(const string& filePath)
{
    if (!initialized)
        Setup();
    Assert(std::filesystem::exists(filePath),
        "Cannot play audioFile, since it does not exist. ", filePath);

    ma_result result = ma_engine_play_sound(&Audio::Engine(), filePath.c_str(), nullptr);

    Assert(result == MA_SUCCESS,
        "Failed to play sound. ", result);
}

ma_engine& Audio::Engine() 
{ 
    if (!initialized)
        Setup();
    return engine; 
}

namespace logger
{
    std::string to_string(ma_result result)
    {
        switch (result)
        {
        case MA_SUCCESS: return "MA_SUCCESS";
        case MA_ERROR: return "MA_ERROR";
        case MA_INVALID_ARGS: return "MA_INVALID_ARGS";
        case MA_INVALID_OPERATION: return "MA_INVALID_OPERATION";
        case MA_OUT_OF_MEMORY: return "MA_OUT_OF_MEMORY";
        case MA_OUT_OF_RANGE: return "MA_OUT_OF_RANGE";
        case MA_ACCESS_DENIED: return "MA_ACCESS_DENIED";
        case MA_DOES_NOT_EXIST: return "MA_DOES_NOT_EXIST";
        case MA_ALREADY_EXISTS: return "MA_ALREADY_EXISTS";
        case MA_TOO_MANY_OPEN_FILES: return "MA_TOO_MANY_OPEN_FILES";
        case MA_INVALID_FILE: return "MA_INVALID_FILE";
        case MA_TOO_BIG: return "MA_TOO_BIG";
        case MA_PATH_TOO_LONG: return "MA_PATH_TOO_LONG";
        case MA_NAME_TOO_LONG: return "MA_NAME_TOO_LONG";
        case MA_NOT_DIRECTORY: return "MA_NOT_DIRECTORY";
        case MA_IS_DIRECTORY: return "MA_IS_DIRECTORY";
        case MA_DIRECTORY_NOT_EMPTY: return "MA_DIRECTORY_NOT_EMPTY";
        case MA_AT_END: return "MA_AT_END";
        case MA_NO_SPACE: return "MA_NO_SPACE";
        case MA_BUSY: return "MA_BUSY";
        case MA_IO_ERROR: return "MA_IO_ERROR";
        case MA_INTERRUPT: return "MA_INTERRUPT";
        case MA_UNAVAILABLE: return "MA_UNAVAILABLE";
        case MA_ALREADY_IN_USE: return "MA_ALREADY_IN_USE";
        case MA_BAD_ADDRESS: return "MA_BAD_ADDRESS";
        case MA_BAD_SEEK: return "MA_BAD_SEEK";
        case MA_BAD_PIPE: return "MA_BAD_PIPE";
        case MA_DEADLOCK: return "MA_DEADLOCK";
        case MA_TOO_MANY_LINKS: return "MA_TOO_MANY_LINKS";
        case MA_NOT_IMPLEMENTED: return "MA_NOT_IMPLEMENTED";
        case MA_NO_MESSAGE: return "MA_NO_MESSAGE";
        case MA_BAD_MESSAGE: return "MA_BAD_MESSAGE";
        case MA_NO_DATA_AVAILABLE: return "MA_NO_DATA_AVAILABLE";
        case MA_INVALID_DATA: return "MA_INVALID_DATA";
        case MA_TIMEOUT: return "MA_TIMEOUT";
        case MA_NO_NETWORK: return "MA_NO_NETWORK";
        case MA_NOT_UNIQUE: return "MA_NOT_UNIQUE";
        case MA_NOT_SOCKET: return "MA_NOT_SOCKET";
        case MA_NO_ADDRESS: return "MA_NO_ADDRESS";
        case MA_BAD_PROTOCOL: return "MA_BAD_PROTOCOL";
        case MA_PROTOCOL_UNAVAILABLE: return "MA_PROTOCOL_UNAVAILABLE";
        case MA_PROTOCOL_NOT_SUPPORTED: return "MA_PROTOCOL_NOT_SUPPORTED";
        case MA_PROTOCOL_FAMILY_NOT_SUPPORTED: return "MA_PROTOCOL_FAMILY_NOT_SUPPORTED";
        case MA_ADDRESS_FAMILY_NOT_SUPPORTED: return "MA_ADDRESS_FAMILY_NOT_SUPPORTED";
        case MA_SOCKET_NOT_SUPPORTED: return "MA_SOCKET_NOT_SUPPORTED";
        case MA_CONNECTION_RESET: return "MA_CONNECTION_RESET";
        case MA_ALREADY_CONNECTED: return "MA_ALREADY_CONNECTED";
        case MA_NOT_CONNECTED: return "MA_NOT_CONNECTED";
        case MA_CONNECTION_REFUSED: return "MA_CONNECTION_REFUSED";
        case MA_NO_HOST: return "MA_NO_HOST";
        case MA_IN_PROGRESS: return "MA_IN_PROGRESS";
        case MA_CANCELLED: return "MA_CANCELLED";
        case MA_MEMORY_ALREADY_MAPPED: return "MA_MEMORY_ALREADY_MAPPED";
        case MA_CRC_MISMATCH: return "MA_CRC_MISMATCH";
        case MA_FORMAT_NOT_SUPPORTED: return "MA_FORMAT_NOT_SUPPORTED";
        case MA_DEVICE_TYPE_NOT_SUPPORTED: return "MA_DEVICE_TYPE_NOT_SUPPORTED";
        case MA_SHARE_MODE_NOT_SUPPORTED: return "MA_SHARE_MODE_NOT_SUPPORTED";
        case MA_NO_BACKEND: return "MA_NO_BACKEND";
        case MA_NO_DEVICE: return "MA_NO_DEVICE";
        case MA_API_NOT_FOUND: return "MA_API_NOT_FOUND";
        case MA_INVALID_DEVICE_CONFIG: return "MA_INVALID_DEVICE_CONFIG";
        case MA_LOOP: return "MA_LOOP";
        case MA_BACKEND_NOT_ENABLED: return "MA_BACKEND_NOT_ENABLED";
        case MA_DEVICE_NOT_INITIALIZED: return "MA_DEVICE_NOT_INITIALIZED";
        case MA_DEVICE_ALREADY_INITIALIZED: return "MA_DEVICE_ALREADY_INITIALIZED";
        case MA_DEVICE_NOT_STARTED: return "MA_DEVICE_NOT_STARTED";
        case MA_DEVICE_NOT_STOPPED: return "MA_DEVICE_NOT_STOPPED";
        case MA_FAILED_TO_INIT_BACKEND: return "MA_FAILED_TO_INIT_BACKEND";
        case MA_FAILED_TO_OPEN_BACKEND_DEVICE: return "MA_FAILED_TO_OPEN_BACKEND_DEVICE";
        case MA_FAILED_TO_START_BACKEND_DEVICE: return "MA_FAILED_TO_START_BACKEND_DEVICE";
        case MA_FAILED_TO_STOP_BACKEND_DEVICE: return "MA_FAILED_TO_STOP_BACKEND_DEVICE";
        default: return ma_result_description(result);
        }
    }
}


