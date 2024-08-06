#include "systemutils/IDSystem.h"

#include "systemutils/Error.h"

IDSystem::IDSystem() noexcept:
    count(0),
    max(std::numeric_limits<GLuint>::max()),
    nextFreeID(1),
    className(nullptr) {}

IDSystem::~IDSystem() noexcept {
    freeIDs.clear();
    while (!toDelete.empty()) {
        toDelete.pop();
    }
    while (!toReuse.empty()) {
        toReuse.pop();
    }
    while (!toCreate.empty()) {
        toCreate.pop();
    }
}

auto IDSystem::createID() noexcept -> GLuint {
    GLuint ID;
    if (count == max) {
        if (toDelete.empty()) {
            if (className) {
                PRINT_ERROR("Can't create new ID.", "Maximum number of {}s reached: {}", className, max);
            } else {
                PRINT_ERROR("Can't create new ID.", "Maximum number of A-objects reached: {}", max);
            }
            return 0;
        } else {
            ID = toDelete.front();
            toDelete.pop();
            toReuse.push(ID);
            toCreate.push(ID);
            return ID;
        }
    }
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(freeIDs.begin());
    } else {
        ID = nextFreeID++;
    }
    toCreate.push(ID);
    return ID;
}

auto IDSystem::deleteID(GLuint ID) noexcept -> void {
    toDelete.push(ID);
}

auto IDSystem::setClassName(const char* name) noexcept -> void {
    className = name;
}
