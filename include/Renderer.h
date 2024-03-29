#pragma once

class Renderer {
public:
    static auto getInstance() -> Renderer&;

    auto initialize() -> bool;
    auto shutdown() -> void;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

private:
    Renderer();
    ~Renderer();
};
