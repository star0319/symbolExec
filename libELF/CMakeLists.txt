cmake_minimum_required(VERSION 3.10)

project(ELF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Detecting SubModule libELF")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIBELF_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include LIBELF_HEAD_FILES)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(
    ${PROJECT_NAME}
    ${LIBELF_SOURCE_FILES}
        ${LIBELF_HEAD_FILES}
)

add_executable(elfTest ${LIBELF_SOURCE_FILES}
#        ${LIBCFG_HEAD_FILES}
)

#target_link_libraries(elfTest ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcapstone.a)

