cmake_minimum_required(VERSION 3.10)

project(SYMBOLEXEC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Detecting SubModule symbolExec")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SYMBOLEXEC_SOURCE_FILES)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/src LIBCFG_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include SYMBOLEXEC_HEAD_FILES)
#aux_source_directory(${PROJECT_SOURCE_DIR}/libCFG/include libCFG_HEAD_FILE)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../libELF/src LIBELF_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../libELF/include LIBELF_HEAD_FILES)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/include/capstone
        ${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/include/nlohmann
        ${CMAKE_CURRENT_SOURCE_DIR}/../libELF/include/elfio
        ${CMAKE_CURRENT_SOURCE_DIR}/../libELF/include/

)

add_library(
        ${PROJECT_NAME}
        ${SYMBOLEXEC_SOURCE_FILES}
        ${LIBELF_HEAD_FILES}
#        ${LIBCFG_SOURCE_FILES}
#        src/z3Test.cpp
        #        ${LIBCFG_HEAD_FILES}
)

add_executable(symbolExec ${SYMBOLEXEC_SOURCE_FILES}
        #        ${LIBCFG_HEAD_FILES}
)


#set(Z3_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libz3.lib)
#target_link_libraries(symbolExec ${Z3_LIBRARIES})

#message(STATUS "Detecting SubModule symbolExec  ${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/lib/")
set(Capstone_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../libCFG/lib/libcapstone.a)
target_link_libraries(symbolExec ${Capstone_LIBRARIES})

