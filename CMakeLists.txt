cmake_minimum_required(VERSION 3.10)
project(TechinicianAgent)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/local/include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(OpenSSL REQUIRED)

# Include all cpp's in the src directory and its subdirectories
file(GLOB MAIN_SRC "src/*.cpp")
file(GLOB MODULES_SRC "src/preproc/modules/*.cpp")
file(GLOB MAIN_MODULES_SRC "src/preproc/*.cpp")
file(GLOB UTILS_SRC "src/utils/*.cpp")
file(GLOB CONNECTORS_SRC "src/connectors/*.cpp")

add_executable(ai-preproccessor
    ${MAIN_SRC}
    ${MAIN_MODULES_SRC}
    ${MODULES_SRC}
    ${UTILS_SRC}
    ${CONNECTORS_SRC}
)

target_link_libraries(ai-preproccessor
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    OpenSSL::Crypto
)
