cmake_minimum_required(VERSION 4.2)

project(Kainure LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEPS_DIR "${CMAKE_SOURCE_DIR}/../deps")
set(NODE_INCLUDES_DIR "${DEPS_DIR}/node/includes")
set(NODE_RUNTIME_DIR "${DEPS_DIR}/node/runtime")

file(GLOB_RECURSE SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE
    -m32
    -fPIC
    -O3
    -flto
    -msse2
    -funroll-loops
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    LINUX
    _LINUX
    __linux__
    BUILDING_NODE_EXTENSION
    USING_V8_SHARED
    USING_UV_SHARED
    NDEBUG
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    SYSTEM PRIVATE
        ${DEPS_DIR}
        ${DEPS_DIR}/json/single_include
        ${DEPS_DIR}/samp-sdk
        ${NODE_INCLUDES_DIR}/node
        ${NODE_INCLUDES_DIR}/uv
        ${NODE_INCLUDES_DIR}/v8
)

find_library(NODE_LIB
    NAMES libnode.so
    PATHS ${NODE_RUNTIME_DIR}
    NO_DEFAULT_PATH
    REQUIRED
)

target_link_options(${PROJECT_NAME} PRIVATE
    -m32
    -flto
    -Wl,-rpath,'$ORIGIN/..'
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${NODE_LIB}
    pthread
    dl
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".so"

    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON

    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../compiled/Linux"

    BUILD_RPATH "$ORIGIN/.."
    INSTALL_RPATH "$ORIGIN/.."
    
    BUILD_RPATH_USE_ORIGIN ON
)
