#include "systemutils/IDSystem.h"

#include "systemutils/Error.h"

IDSystem::IDSystem() noexcept:
    count(0),
    max(std::numeric_limits<GLuint>::max()),
    nextFreeID(1),
    className(nullptr) {}

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
            count++;
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
    count++;
    return ID;
}

auto IDSystem::deleteID(GLuint ID) noexcept -> void {
    toDelete.push(ID);
    count--;
}

auto IDSystem::setClassName(const char* name) noexcept -> void {
    className = name;
}

auto IDSystem::reset() noexcept -> void {
    count = 0;
    nextFreeID = 0;
    freeIDs.clear();
    resetQueue(toCreate);
    resetQueue(toDelete);
    resetQueue(toReuse);
}

auto IDSystem::getMax() const noexcept -> GLuint {
    return max;
}

auto IDSystem::resetQueue(std::queue<GLuint>& queue) noexcept -> void {
    while (!queue.empty()) {
        queue.pop();
    }
}
