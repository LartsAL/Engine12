#include "systemutils/IDSystem.h"

#include "systemutils/Error.h"

IDSystem::IDSystem(const char* classname) noexcept:
    count(0),
    max(std::numeric_limits<GLuint>::max()),
    nextFreeID(1),
    className(classname) {}

IDSystem::~IDSystem() noexcept {
    reset();
}

auto IDSystem::createID() noexcept -> GLuint {
    GLuint ID;
    if (count == max) {
        if (className) {
            PRINT_ERROR("Can't create new ID.", "Maximum number of {}s reached: {}", className, max);
        } else {
            PRINT_ERROR("Can't create new ID.", "Maximum number of A-objects reached: {}", max);
        }
        return 0;
    }
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(freeIDs.begin());
    } else {
        ID = nextFreeID++;
    }
    count++;
    return ID;
}

auto IDSystem::deleteID(GLuint ID) noexcept -> void {
    freeIDs.insert(ID);
    count--;
}

auto IDSystem::setClassName(const char* name) noexcept -> void {
    className = name;
}

auto IDSystem::reset() noexcept -> void {
    count = 0;
    nextFreeID = 1;
    freeIDs.clear();
}

auto IDSystem::getMax() const noexcept -> GLuint {
    return max;
}

auto IDSystem::getIDCount() const noexcept -> GLuint {
    return count;
}
