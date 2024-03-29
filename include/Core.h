#pragma once

class Core {
public:
    Core();
    ~Core();

    auto initialize() -> bool;
    auto run() -> void;
    auto shutdown() -> void;

private:
    auto update() -> void;
};
