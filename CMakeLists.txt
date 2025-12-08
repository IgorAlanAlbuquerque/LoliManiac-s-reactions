# git submodule add -b ng https://github.com/alandtse/CommonLibVR.git extern/CommonLibVR-ng
# git submodule update --init --recursive

cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0002 NEW)

project(LoliManiacReactions VERSION 0.0.1 LANGUAGES CXX)

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/LoliManiacSingleReactions")
endif()

set(BUILD_TESTS OFF CACHE BOOL "Build unit tests for CommonLibVR." FORCE)
set(ENABLE_SKYRIM_VR OFF CACHE BOOL "Disable Skyrim VR in CommonLibVR-ng" FORCE)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibVR-ng
    ${CMAKE_BINARY_DIR}/_deps/CommonLibVR-ng-build
)

get_target_property(COMMONLIB_SRC_DIR CommonLibSSE SOURCE_DIR)

include(${COMMONLIB_SRC_DIR}/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(LoliManiacSingleReactions
    SOURCES
    src/plugin.cpp
    src/Elements.cpp
    src/single/FreezeFrost.cpp)

target_include_directories(LoliManiacSingleReactions
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(LoliManiacSingleReactions PRIVATE cxx_std_23) 
target_precompile_headers(LoliManiacSingleReactions
    PRIVATE
    src/PCH.h
    src/Elements.h
    src/single/FreezeFrost.h) 

if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET LoliManiacSingleReactions
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:LoliManiacSingleReactions>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:LoliManiacSingleReactions>"
        VERBATIM
    )
endif()
