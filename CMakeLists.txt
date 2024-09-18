cmake_minimum_required(VERSION 3.15)

project(DrugLib LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add options to enable or disable Boost and Qt5
option(USE_BOOST "Enable Boost library" ON)
option(USE_QT5 "Enable Qt5 library" ON)

# Global compiler options (if necessary)
# It's better to set these per-target
# add_compile_options(-pthread -pipe -Wall -fPIC -Werror=return-type)

# Find necessary packages
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL 3.0.0 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(absl REQUIRED)
find_package(libpqxx REQUIRED)

# Conditionally find Boost if enabled
if(USE_BOOST)
    # Boost settings (adjust as needed)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED OFF)
    set(Boost_USE_STATIC_RUNTIME ON)
    set(Boost_NO_SYSTEM_PATHS ON)
    # set(BOOST_ROOT /opt/vcpkg/installed/x64-linux) # Adjust if necessary

    find_package(Boost REQUIRED COMPONENTS log_setup log filesystem program_options)
    if(NOT Boost_FOUND)
        message(FATAL_ERROR "Boost libraries not found, but USE_BOOST is ON.")
    endif()
endif()

# Conditionally find Qt5 if enabled
if(USE_QT5)
    find_package(Qt5 COMPONENTS Core Quick REQUIRED)
    if(NOT Qt5_FOUND)
        message(FATAL_ERROR "Qt5 libraries not found, but USE_QT5 is ON.")
    endif()
endif()

# Include subdirectories
add_subdirectory(common)

add_subdirectory(data)
