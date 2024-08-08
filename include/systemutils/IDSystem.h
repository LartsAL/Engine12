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

    auto reset() -> void; // returns IDSystem state back to when it's conctructor have been just called

    GLuint max;

    std::queue<GLuint> toDelete;
    std::queue<GLuint> toReuse;
    std::queue<GLuint> toCreate;
    std::set<GLuint> freeIDs;

private:
    GLuint count;               //number of given ID's
    GLuint nextFreeID;
    const char* className;

    static auto resetQueue(std::queue<GLuint>& queue) -> void;
};
