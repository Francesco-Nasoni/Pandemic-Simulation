cmake_minimum_required(VERSION 3.16)

project(SIR CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(SIR main.cpp epidemic.cpp)

if (BUILD_TESTING)
  add_executable(SIR.t epidemic.test.cpp epidemic.cpp)
  add_test(NAME SIR.t COMMAND SIR.t)
endif()