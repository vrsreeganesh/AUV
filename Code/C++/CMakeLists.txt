# setting architecture
set(CMAKE_OSX_ARCHITECTURES arm64)

# version of the cmake required
cmake_minimum_required(VERSION 4.1.1)

# name of the project/executable
project(auv)

# adding executables
add_executable(auv main.cpp)

# request C++23
set_property(TARGET auv PROPERTY CXX_STANDARD 23)
set_property(TARGET auv PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET auv PROPERTY CXX_EXTENSIONS OFF)  # optional, avoids GNU extensions

# adding logging
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(auv PRIVATE spdlog::spdlog fmt::fmt)


# compiler optimization flags
# target_compile_options(auv PRIVATE -Ofast)
# target_compile_options(auv PRIVATE -O3 -ffast-math)
# target_compile_options(auv PRIVATE -O3)
target_compile_options(auv PRIVATE -O2)
# target_compile_options(auv PRIVATE -O1)
# target_compile_options(auv PRIVATE -O0)

# finding and linking fftw3
find_library(FFTW3_LIB fftw3 REQUIRED)
find_path(FFTW3_INCLUDE_DIR fftw3.h REQUIRED)
target_include_directories(auv PRIVATE ${FFTW3_INCLUDE_DIR})
target_link_libraries(auv PRIVATE ${FFTW3_LIB})