#include "WindowManager.h"

WindowManager::WindowManager() = default;

auto WindowManager::getInstance() -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize(int width, int height, const char* title) -> bool {

}

auto WindowManager::shutdown() -> void {

}
