#pragma once

#include <filesystem>
#include <sstream>
#include <string>

// Enable DEBUG switches in DEBUG mode
// NDEBUG is defined in RELEASE mode
#ifndef NDEBUG
    // #define COMPOSE_SHADERS // Uncomment to compose shaders in DEBUG mode
    //                         // Currently disabled because the #line directive serves better for debugging
    #define MAKE_PATHS_RELATIVE_TO_SOURCE_DIR
#endif

#ifdef __APPLE__
    #include <unistd.h>
    #include <pwd.h>
#endif

/**
 * Contains context information for the application like the project name and source directory.
 * This is set using the CMake `configure_file` command.
 */
namespace Context {

// Fetch the directory for document files
#ifdef __APPLE__
    const std::filesystem::path HOME = getpwuid(getuid())->pw_dir; // Get home directory on Unix/Linux/MacOS
    const std::filesystem::path APP_DIR = HOME / "/Library/Application Support/cgintro_6/"; // Writing into the .app bundle is not allowed as it is signed
#else
    const std::filesystem::path APP_DIR = ""; // On other platforms just write into the application directory
#endif
const std::string PROJECT_NAME = "cgintro_6";


#ifndef NDEBUG
    /* Source directory, only available in DEBUG mode */
    const std::filesystem::path SOURCE_DIR = "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6";
    const std::filesystem::path COMPOSED_SHADER_DIR = "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/composed/";
#endif

/**
 * Overwrites the working directory to make relative paths relative to the source directory in DEBUG mode.
 */
inline void setWorkingDirectory() {
    #ifdef MAKE_PATHS_RELATIVE_TO_SOURCE_DIR
        std::filesystem::current_path(SOURCE_DIR);
    #endif
}

inline std::string getCWDWarning() {
    std::stringstream stream;
    const auto cwd = std::filesystem::current_path();
    if (std::filesystem::current_path() != SOURCE_DIR) {
        stream << "Warning: The current working directory " << cwd << " is not the source directory " << SOURCE_DIR
               << ". This can lead to relative paths not being resolved as expected, you can fix this by setting the working directory with `cd` before running your program." << std::endl;
    }
    return stream.str();
}

}
