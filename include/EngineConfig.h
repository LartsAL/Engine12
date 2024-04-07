#pragma once

#include <any>
#include <string>
#include <unordered_map>

class EngineConfig {
public:
    EngineConfig() = delete;
    ~EngineConfig() = delete;

    template<class T>
    static auto get(std::string&& key) -> T;
    template<class T>
    static auto set(std::string&& key, T&& value) -> void;

private:
    static std::unordered_map<std::string, std::any> vars;
};

#include "EngineConfig.tpp"
