add_library(MatrixGameClientInternal STATIC)
add_library(MatrixGameServerInternal STATIC)

#find_package(Winsock REQUIRED)
#include_directories(${Winsock_INCLUDE_DIRS})
#target_link_libraries(YourTarget ${Winsock_LIBRARIES})

macro(add_matrix_directory DIRECTORY)
    file(GLOB DIR_SOURCES ${DIRECTORY}/*.cpp ${DIRECTORY}/*.c)
    file(GLOB DIR_HEADERS ${DIRECTORY}/*.hpp ${DIRECTORY}/*.h)
    target_sources(MatrixGameClientInternal PRIVATE ${DIR_SOURCES} ${DIR_HEADERS})
    target_sources(MatrixGameServerInternal PRIVATE ${DIR_SOURCES} ${DIR_HEADERS})
    get_filename_component (GROUP ${DIRECTORY} NAME)
    source_group ("${GROUP}\\Source Files" FILES ${DIR_SOURCES})
    source_group ("${GROUP}\\Header Files" FILES ${DIR_HEADERS})
endmacro()

file(GLOB MATRIX_SOURCES src/*.cpp src/*.c)
file(GLOB MATRIX_HEADERS src/*.hpp src/*.h)

set(MATRIX_RESOURCES
    MatrixGame.ico
    MatrixGame.rc
    Resource.h)

# Client

target_sources(
    MatrixGameClientInternal
    PRIVATE
        ${MATRIX_SOURCES}
        ${MATRIX_HEADERS}
        ${MATRIX_RESOURCES})

target_include_directories(
    MatrixGameClientInternal
    PUBLIC
        .
        src
	${ENET_INCLUDE_DIRS}
        "$<$<C_COMPILER_ID:MSVC>:${DIRECTX9_INCLUDE_DIRS}>")

target_compile_options(
    MatrixGameClientInternal
    PUBLIC
        ${COMPILE_OPTIONS})

target_compile_definitions(
    MatrixGameClientInternal
    PUBLIC
	WIN32_LEAN_AND_MEAN
        MATRIXGAME_EXPORTS
        $<$<CONFIG:Release>:ASSERT_OFF>
        _ALLOW_RTCc_IN_STL
        WINDOWS_IGNORE_PACKING_MISMATCH
        ${COMPILE_DEFINITIONS}
        CLIENT_ON)

target_link_options(
    MatrixGameClientInternal
    PUBLIC
        "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>"
        ${LINK_OPTIONS})

target_link_libraries(
    MatrixGameClientInternal
    PUBLIC
        winmm
        ${DIRECTX9_LIBRARIES}
        MatrixLib
	Ws2_32.lib
	winmm.lib 
	ENET
        "$<$<C_COMPILER_ID:MSVC>:legacy_stdio_definitions>"
)


# Server

target_sources(
    MatrixGameServerInternal
    PRIVATE
        ${MATRIX_SOURCES}
        ${MATRIX_HEADERS}
        ${MATRIX_RESOURCES})

target_include_directories(
    MatrixGameServerInternal
    PUBLIC
        .
        src
	${ENET_INCLUDE_DIRS}
        "$<$<C_COMPILER_ID:MSVC>:${DIRECTX9_INCLUDE_DIRS}>")

target_compile_options(
    MatrixGameServerInternal
    PUBLIC
        ${COMPILE_OPTIONS})

target_compile_definitions(
    MatrixGameServerInternal
    PUBLIC
	WIN32_LEAN_AND_MEAN
        MATRIXGAME_EXPORTS
        $<$<CONFIG:Release>:ASSERT_OFF>
        _ALLOW_RTCc_IN_STL
        WINDOWS_IGNORE_PACKING_MISMATCH
        ${COMPILE_DEFINITIONS}
        SERVER_ON)

target_link_options(
    MatrixGameServerInternal
    PUBLIC
        "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>"
        ${LINK_OPTIONS})

target_link_libraries(
    MatrixGameServerInternal
    PUBLIC
        winmm
        ${DIRECTX9_LIBRARIES}
        MatrixLib
	ws2_32.lib
	winmm.lib 
	ENET
        "$<$<C_COMPILER_ID:MSVC>:legacy_stdio_definitions>"
)


# if(NOT MATRIXGAME_BUILD_DLL)
    set_source_files_properties(
        src/MatrixFormGame.cpp
        PROPERTIES
            COMPILE_DEFINITIONS $<$<CONFIG:Release>:EXE_VERSION>)
# endif()

#if(MATRIXGAME_CHEATS)
    set_source_files_properties(
        src/MatrixFormGame.cpp
        PROPERTIES
            COMPILE_DEFINITIONS CHEATS_ON)
# endif()

add_matrix_directory(src/Effects)
add_matrix_directory(src/Interface)
add_matrix_directory(src/Logic)
add_matrix_directory(src/Network)

source_group("Resources" FILES ${MATRIX_RESOURCES})

# if(MATRIXGAME_BUILD_DLL)
#     add_library(MatrixGame SHARED)
#     # remove lib prefix from the DLL name (for GCC build)
#     set_target_properties(MatrixGame PROPERTIES PREFIX "")
# else()
#     add_executable(MatrixGame)
# endif()

# target_sources(MatrixGame PRIVATE
#     src/MatrixGame.cpp
# )

# target_link_libraries(MatrixGame MatrixGameInternal)


add_executable(MatrixGameClient)
target_sources(MatrixGameClient PRIVATE src/MatrixGame.cpp)
target_compile_definitions(MatrixGameClient PRIVATE CLIENT_ON)
target_link_libraries(MatrixGameClient MatrixGameClientInternal)

add_executable(MatrixGameServer)
target_sources(MatrixGameServer PRIVATE src/MatrixGame.cpp)
target_compile_definitions(MatrixGameServer PRIVATE SERVER_ON)
target_link_libraries(MatrixGameServer MatrixGameServerInternal)
