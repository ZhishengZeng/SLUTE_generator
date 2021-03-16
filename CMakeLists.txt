project(slute)
cmake_minimum_required(VERSION 3.11)

add_library(slute
src/Comb.cpp
src/Generator.cpp
src/POST.cpp
src/POWV.cpp
src/PreTree.cpp
src/Util.cpp)

SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fopenmp")
# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -fopenmp")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(generateST ${PROJECT_SOURCE_DIR}/generateST.cpp)
target_link_libraries(generateST slute)



