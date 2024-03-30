#pragma once

#include <stdexcept>

class EngineException : public std::exception {
public:
    explicit EngineException(const char* msg);
    const char* what() const noexcept override;

private:
    const char* message;
};
