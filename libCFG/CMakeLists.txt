cmake_minimum_required(VERSION 3.10)

project(CFG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Detecting SubModule libCFG")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIBCFG_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include LIBCFG_HEAD_FILES)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/capstone
)

add_library(
    ${PROJECT_NAME}
    ${LIBCFG_SOURCE_FILES}
        #        ${LIBCFG_HEAD_FILES}
)

add_executable(cfgTest ${LIBCFG_SOURCE_FILES}
#        ${LIBCFG_HEAD_FILES}
)
set(Capstone_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcapstone.a)
target_link_libraries(cfgTest ${Capstone_LIBRARIES})

