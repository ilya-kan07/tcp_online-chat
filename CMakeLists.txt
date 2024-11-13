cmake_minimum_required(VERSION 3.10)

project(tcp_online_chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(BOOST_ROOT "C:/boost_1_86_0")
set(BOOST_LIBRARYDIR "C:/boost_1_86_0/libs")

find_package(Boost 1.86 REQUIRED COMPONENTS system)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_definitions(-D_WIN32_WINNT=0x0601)

include_directories(include)

add_executable(server src/server/server.cpp)
add_executable(client src/client/client.cpp src/client/main.cpp)

target_link_libraries(server Boost::system)
target_link_libraries(client Boost::system Qt6::Core Qt6::Gui Qt6::Widgets)

if(UNIX)
    target_link_libraries(server pthread)
    target_link_libraries(client pthread)
endif()

if (MSVC)
    target_compile_options(client PRIVATE /Zc:__cplusplus /permissive-)
endif()

# cmake .. -G "MinGW Makefiles"
