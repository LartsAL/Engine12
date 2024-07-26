#pragma once

#include <format>

#define EMPTY_STR ""

auto PrintError(const char* file, unsigned int line, const char* function,
                const char* what, const char* extra_print) -> void;

#define PRINT_ERROR(what, ...) {\
    PrintError(__FILE__, __LINE__, __FUNCTION__, (what), std::format(EMPTY_STR __VA_ARGS__).c_str());\
}
