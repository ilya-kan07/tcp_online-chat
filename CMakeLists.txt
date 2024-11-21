cmake_minimum_required(VERSION 3.10)

project(tcp_online_chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Boost REQUIRED COMPONENTS system)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_definitions(-D_WIN32_WINNT=0x0601)

include_directories(include)

add_executable(server src/server/server.cpp)
add_executable(client src/client/window.cpp src/client/client.cpp src/client/main.cpp include/window.hpp)

target_link_libraries(server Boost::system)
target_link_libraries(client Boost::system)
target_link_libraries(client Qt6::Core Qt6::Gui Qt6::Widgets)

if (MSVC)
    target_compile_options(client PRIVATE /Zc:__cplusplus /permissive-)
endif()
