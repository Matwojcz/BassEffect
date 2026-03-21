cmake_minimum_required(VERSION 4.1)
project(bassOGverdrive)

set(CMAKE_CXX_STANDARD 20)

add_executable(bassOGverdrive main.cpp)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

target_link_libraries(bassOGverdrive ${PORTAUDIO_LIBRARIES})
target_include_directories(bassOGverdrive PRIVATE ${PORTAUDIO_INCLUDE_DIRS})


