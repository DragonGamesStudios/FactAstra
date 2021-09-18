cmake_minimum_required(VERSION 3.18)

project(FactAstra VERSION 0.0)

# Options
set(CONFIGURATION "Release" CACHE STRING "Defines what configuration to build the executable in.")

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Sources
set(SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(SRC_PROTOTYPE_CPP
    ${SRCDIR}/LuaPrototype.cpp
    ${SRCDIR}/LuaControllerPrototype.cpp
)

set(SRC_CPP
    ${SRCDIR}/main.cpp
    ${SRCDIR}/App.cpp
    ${SRCDIR}/util.cpp
    ${SRCDIR}/ModManager.cpp
    ${SRCDIR}/json.cpp
    ${SRCDIR}/Version.cpp
    ${SRCDIR}/LuaStateManager.cpp
    ${SRCDIR}/LuaObject.cpp
    ${SRCDIR}/LuaStorage.cpp
)

set(SRC_PROTOTYPE_HPP
    ${SRCDIR}/LuaPrototype.hpp
    ${SRCDIR}/LuaControllerPrototype.hpp
)

set(SRC_HPP
    ${SRCDIR}/App.hpp
    ${SRCDIR}/util.hpp
    ${SRCDIR}/ModManager.hpp
    ${SRCDIR}/json.hpp
    ${SRCDIR}/Version.hpp
    ${SRCDIR}/errors.hpp
    ${SRCDIR}/LuaStateManager.hpp
    ${SRCDIR}/LuaObject.hpp
    ${SRCDIR}/LuaStorage.hpp
)

source_group("Sources" FILES ${SRC_CPP})
source_group("Headers" FILES ${SRC_HPP})
source_group("Prototypes - Sources" FILES ${SRC_PROTOTYPE_CPP})
source_group("Prototypes - Headers" FILES ${SRC_PROTOTYPE_HPP})

add_executable(FactAstra ${SRC_CPP} ${SRC_PROTOTYPE_CPP} ${SRC_HPP} ${SRC_PROTOTYPE_HPP})

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
    zlibstaticd
    liblzma
    zstd_static
    bzip2
    libminizip
    lua
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/licenses.txt ${CMAKE_CURRENT_BINARY_DIR}/licenses.txt COPYONLY)

set(FACTASTRA_BUILD_LOCATION "")

if(CONFIGURATION EQUAL Release)
    set(VS_CONFIGURATION_TYPE Release)
else()
    set(VS_CONFIGURATION_TYPE Debug)
endif()

add_custom_command(TARGET FactAstra PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:FactAstra>/data)

foreach(LIB IN LISTS LIBS)
    target_include_directories(FactAstra PUBLIC "${CMAKE_SOURCE_DIR}/extlibs/${LIB}/include")
    target_link_libraries(FactAstra ${CMAKE_SOURCE_DIR}/extlibs/${LIB}/${OS}/${CONFIGURATION}/${LIB}.lib)
endforeach()