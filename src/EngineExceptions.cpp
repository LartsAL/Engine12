#include "EngineExceptions.h"

#include <stdexcept>

EngineException::EngineException(const char* msg, unsigned char code):
    message(msg),
    errnum(code) {}

auto EngineException::what() const noexcept -> const char * {
    return message;
}

auto EngineException::code() const noexcept -> unsigned char {
    return errnum;
}
