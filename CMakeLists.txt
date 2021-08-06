cmake_minimum_required(VERSION 3.18)

project(FactAstra VERSION 0.0)

# Options
set(CONFIGURATION "Release" CACHE STRING "Defines what configuration to build the executable in.")

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Sources
set(SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(SRC_CPP
    ${SRCDIR}/main.cpp
    ${SRCDIR}/App.cpp
    ${SRCDIR}/util.cpp
)

set(SRC_HPP
    ${SRCDIR}/App.hpp
    ${SRCDIR}/util.hpp
)

source_group("Sources" FILES ${SRC_CPP})
source_group("Headers" FILES ${SRC_HPP})

add_executable(FactAstra ${SRC_CPP} ${SRC_HPP})

if(WIN32)
    set(OS "Win32")
elseif(UNIX)
    set(OS "Unix")
elseif(APPLE)
    set(OS "Apple")
endif()

set(LIBS
    Spectre2D
    glfw3
    glew32s
)

if(CONFIGURATION EQUAL Release)
    set(VS_CONFIGURATION_TYPE Release)
else()
    set(VS_CONFIGURATION_TYPE Debug)
endif()

foreach(LIB IN LISTS LIBS)
    target_include_directories(FactAstra PUBLIC "${CMAKE_SOURCE_DIR}/extlibs/${LIB}/include")
    target_link_libraries(FactAstra ${CMAKE_SOURCE_DIR}/extlibs/${LIB}/${OS}/${CONFIGURATION}/${LIB}.lib)
endforeach()