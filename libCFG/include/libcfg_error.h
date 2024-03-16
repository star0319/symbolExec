#ifndef LIBCFG_ERROR_H
#define LIBCFG_ERROR_H

#include <iostream>

template <typename... Args>
void libcfg_error_print(Args&&... args) {
    [[maybe_unused]]
    int _[]{(std::cerr << "\033[31m" << std::forward<Args>(args) << "\033[0m", 0)...};
}

#define libcfg_error(...) do { \
    libcfg_error_print(__FILE__, ":"); \
    libcfg_error_print(__LINE__, "\n"); \
    libcfg_error_print("ERROR:", "\t"); \
    libcfg_error_print(__VA_ARGS__); \
    std::cerr << std::endl; \
    exit(-1); \
} while (0)

#endif