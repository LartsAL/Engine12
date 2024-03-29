#pragma once

class WindowManager {
public:
    static auto getInstance() -> WindowManager&;

    auto initialize(int width, int height, const char* title) -> bool;
    auto shutdown() -> void;

    // А надо ли? И не только тут
    WindowManager(const WindowManager&) = delete;
    auto operator=(const WindowManager&) -> WindowManager& = delete;

private:
    WindowManager();
};
