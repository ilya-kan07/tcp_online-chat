cmake_minimum_required(VERSION 3.10)

project(tcp_online_chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BOOST_ROOT "C:/local/boost_1_86_0")

find_package(Boost REQUIRED COMPONENTS system)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

add_definitions(-D_WIN32_WINNT=0x0601)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(server ${SOURCE_DIR}/server/server.cpp)

add_executable(client ${SOURCE_DIR}/client/client.cpp ${SOURCE_DIR}/client/main.cpp)

target_link_libraries(server Boost::system)
target_link_libraries(client Boost::system)

target_link_libraries(client Qt6::Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(UNIX)
    target_link_libraries(server pthread)
    target_link_libraries(client pthread)
endif()

if (MSVC)
    target_compile_options(client PRIVATE /Zc:__cplusplus /permissive-)
    target_compile_options(server PRIVATE /Zc:__cplusplus /permissive-)
endif()
