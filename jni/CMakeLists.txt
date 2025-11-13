# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.10.2)

# OpenCV setup
# IMPORTANT: Set the OpenCV_DIR environment variable or hardcode the path here
# Example: set(OpenCV_DIR /path/to/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)

# Project name
project("Flamapp")

# Add the native library
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp
             Processor.cpp
             ../gl/renderer.cpp
             )

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../gl
    ${OpenCV_INCLUDE_DIRS}
)


# Link libraries
target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # and EGL/GLES2 libraries.
                       android
                       log
                       EGL
                       GLESv2
                       ${OpenCV_LIBS}
                       )
