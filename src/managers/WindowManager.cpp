#include "managers/WindowManager.h"

WindowManager::WindowManager() = default;

WindowManager::~WindowManager() = default;

auto WindowManager::getInstance() -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize() -> void {

}

auto WindowManager::shutdown() -> void {

}
