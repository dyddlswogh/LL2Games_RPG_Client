#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include "stbNetworkConfig.h"

namespace stb
{
    class Logger
    {
    private:
        static std::ofstream& GetLogFile()
        {
            static std::ofstream logFile;
            return logFile;
        }

        static std::mutex& GetMutex()
        {
            static std::mutex logMutex;
            return logMutex;
        }

        static bool& GetInitialized()
        {
            static bool initialized = false;
            return initialized;
        }

    public:
        static void Init()
        {
            if (!GetInitialized())
            {
                std::string charId = NetworkConfig::GetCharacterId();
                std::string filename = "client_" + charId + ".log";
                GetLogFile().open(filename, std::ios::out | std::ios::trunc);
                GetInitialized() = true;
            }
        }

        static void Log(const std::string& msg)
        {
            std::lock_guard<std::mutex> lock(GetMutex());
            if (GetLogFile().is_open())
            {
                GetLogFile() << msg;
                GetLogFile().flush();
            }
            OutputDebugStringA(msg.c_str());
        }

        static void Close()
        {
            if (GetLogFile().is_open())
            {
                GetLogFile().close();
            }
        }
    };

    #define LOG(msg) do { \
        std::stringstream ss; \
        ss << msg; \
        stb::Logger::Log(ss.str()); \
    } while(0)
}
