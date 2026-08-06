cmake_minimum_required(VERSION 3.15)
project(EnergylandiaCollector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(collector
    src/main.cpp
)

target_include_directories(collector PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(collector PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

if (MSVC)
    target_compile_options(collector PRIVATE /W4)
else()
    target_compile_options(collector PRIVATE -Wall -Wextra -Wpedantic)
endif()