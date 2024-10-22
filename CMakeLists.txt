cmake_minimum_required(VERSION 3.10)
project(symbol_exec)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libCFG)
add_subdirectory(libZ3)
add_subdirectory(libELF)
add_subdirectory(symbolEXEC)




add_executable(symmainStack symex.cpp
        #        ${LIBCFG_HEAD_FILES}
)


add_executable(symGlobalDemo global_demo.cpp
        #        ${LIBCFG_HEAD_FILES}
)