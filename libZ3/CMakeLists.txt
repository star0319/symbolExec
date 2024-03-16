cmake_minimum_required(VERSION 3.10)

project(Z3Solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Detecting SubModule libz3")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIBZ3_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include LIBZ3_HEAD_FILES)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(
        ${PROJECT_NAME}
        ${LIBZ3_SOURCE_FILES}
#        src/z3Test.cpp
        #        ${LIBCFG_HEAD_FILES}
)

add_executable(z3Test ${LIBZ3_SOURCE_FILES}
        #        ${LIBCFG_HEAD_FILES}
)

set(Z3_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libz3.lib)
target_link_libraries(z3Test ${Z3_LIBRARIES})

