#pragma once

#include <set>
#include <queue>
#include <GLFW/glfw3.h>

class IDSystem {
public:
    explicit IDSystem(const char* classname = nullptr) noexcept;
    ~IDSystem() noexcept;

    auto createID() noexcept -> GLuint;
    auto deleteID(GLuint ID) noexcept -> void;
    auto setClassName(const char* name) noexcept -> void;
    auto reset() noexcept -> void;      // reverts IDSystem state back to when it's constructor have been just called
    auto getMax() const noexcept -> GLuint;

    std::queue<GLuint> toDelete;
    std::queue<GLuint> toCreate;
    std::set<GLuint> freeIDs;
private:
    auto resetQueue(std::queue<GLuint>& queue) noexcept -> void;

    GLuint max;
    GLuint count;               // Number of given ID's
    GLuint nextFreeID;
    const char* className;
};
