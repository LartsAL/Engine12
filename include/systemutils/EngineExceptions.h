#pragma once

#include <stdexcept>

class EngineException : public std::exception {
public:
    explicit EngineException(const char* msg, unsigned char code = 1);
    auto what() const noexcept -> const char* override;
    auto code() const noexcept -> unsigned char;

private:
    const char* message;
    unsigned char errnum;
};
