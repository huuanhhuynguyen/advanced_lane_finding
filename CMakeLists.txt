cmake_minimum_required(VERSION 3.10)

set(THIS lane_finding_advanced_cpp)
project(${THIS})

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

include_directories(include)
add_executable(${THIS} ${HEADERS} ${SOURCES})

target_link_libraries(${THIS} ${OpenCV_LIBS})
target_link_libraries( ${THIS} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})