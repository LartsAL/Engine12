#pragma once

class WindowManager {
public:
    static auto getInstance() -> WindowManager&;

    auto initialize() -> void;
    // auto update() -> void;  // Maybe unused
    auto shutdown() -> void;

private:
    WindowManager();
    ~WindowManager();
};
