#pragma once

#include <set>
#include <queue>
#include <GLFW/glfw3.h>

class IDSystem {
public:
    IDSystem() noexcept;
    ~IDSystem() noexcept;

    auto createID() noexcept -> GLuint;
    auto deleteID(GLuint ID) noexcept -> void;
    auto setClassName(const char* name) noexcept -> void;

private:
    GLuint count;
    GLuint max;
    GLuint nextFreeID;
    std::set<GLuint> freeIDs;
    std::queue<GLuint> toDelete;
    std::queue<GLuint> toReuse;
    std::queue<GLuint> toCreate;
    const char* className;
};
