#ifndef CONFIGPATH_H
#define CONFIGPATH_H

#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

inline fs::path getConfigPath(const std::string& appName, const fs::path& androidAppFilesDir = "") {
    fs::path configDir;

#if defined(_WIN32)
    const char* appData = std::getenv("APPDATA");
    if (appData) {
        configDir = fs::path(appData) / appName
    }

// Needs a simple android plugin to retrieve filesDir

#elif defined(__ANDROID__)
    if (!androidAppFilesDir.empty()) {
        configDir = androidAppFilesDir / appName;
    } else {
        configDir = fs::path("/data/data/unknown_package/files") / appName;
    }
#else // Linux/Unix
        const char* home = std::getenv("HOME");
        if (home) configDir = fs::path(home) / ".config" / appName;
#endif
    fs::create_directories(configDir);
    return configDir / "config.json";
}
#endif