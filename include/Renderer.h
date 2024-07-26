#pragma once

// DEPRECATED: remove all unnecessary scripts like Renderer
//             and replace them with Systems attached to Objects

class Renderer {
public:
    static auto getInstance() -> Renderer&;

    auto initialize() -> bool;
    auto shutdown() -> void;

private:
    Renderer();
    ~Renderer();
};
