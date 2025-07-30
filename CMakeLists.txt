cmake_minimum_required(VERSION 3.10)
project(TechinicianAgent)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(techinician_main_agent src/main.cpp)

target_link_libraries(techinician_main_agent ${OpenCV_LIBS})
