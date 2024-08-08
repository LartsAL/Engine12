#pragma once

#include <stdexcept>

#define ENGINE_EXCEPT_UNKNOWN                   1
#define ENGINE_EXCEPT_WINDOW_CREATION_FAILED    2
#define ENGINE_EXCEPT_GLAD_INIT_FAILED          3
#define ENGINE_EXCEPT_SCENE_ALREADY_LINKED      4
#define ENGINE_EXCEPT_NULLPTR_RECEIVED          5

class EngineException : public std::exception {
public:
    explicit EngineException(const char* msg, unsigned char code = 1);
    auto what() const noexcept -> const char* override;
    auto code() const noexcept -> unsigned char;

private:
    const char* message;
    unsigned char errnum;
};
