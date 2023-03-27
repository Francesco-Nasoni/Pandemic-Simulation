cmake_minimum_required(VERSION 3.16)

project(epidemic CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(epidemic main.cpp pandemic1.cpp  pandemicCM1.cpp axis.cpp graph.cpp person.cpp utilities.cpp)
target_link_libraries(epidemic PRIVATE sfml-graphics)

if (BUILD_TESTING)

  add_executable(epidemic.t pandemic.test.cpp pandemic1.cpp  pandemicCM1.cpp axis.cpp graph.cpp person.cpp utilities.cpp)
  target_link_libraries(epidemic.t PRIVATE sfml-graphics)
  add_test(NAME epidemic.t COMMAND epidemic.t)

endif()