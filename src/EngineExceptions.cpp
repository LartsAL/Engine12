#include "EngineExceptions.h"

#include <stdexcept>

EngineException::EngineException(const char *msg): message(msg) {}

const char *EngineException::what() const noexcept {
    return message;
}
