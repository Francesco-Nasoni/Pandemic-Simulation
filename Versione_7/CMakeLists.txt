cmake_minimum_required(VERSION 3.16)

project(pandemic CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)

include_directories("header")

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -O2")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer -O2")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer -O2")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(pandemic main.cpp source/pandemic.cpp  source/pandemicCM.cpp source/axis.cpp source/graph.cpp source/person.cpp source/m_functions.cpp)
target_link_libraries(pandemic PRIVATE sfml-graphics)

if (BUILD_TESTING)
  string(REGEX REPLACE  " " "-O2" CMAKE_EXE_LINKER_FLAGS_DEBUG "")
  string(REGEX REPLACE  " " "-O2" CMAKE_CXX_FLAGS_DEBUG "")
  add_executable(pandemic.t pandemic.test.cpp source/pandemic.cpp  source/pandemicCM.cpp source/axis.cpp source/graph.cpp source/person.cpp source/m_functions.cpp)
  target_link_libraries(pandemic.t PRIVATE sfml-graphics)
  add_test(NAME pandemic.t COMMAND pandemic.t)
endif()