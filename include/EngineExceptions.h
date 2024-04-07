#pragma once

#include <stdexcept>

class EngineException : public std::exception {
public:
    explicit EngineException(const char* msg);
    auto what() const noexcept -> const char* override;

private:
    const char* message;
};
