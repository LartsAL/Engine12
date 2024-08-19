#pragma once

#include <set>
#include <queue>
#include <glad/glad.h>

class IDSystem {
public:
    explicit IDSystem(const char* classname = nullptr) noexcept;
    ~IDSystem() noexcept;

    auto createID() noexcept -> GLuint;
    auto deleteID(GLuint ID) noexcept -> void;
    auto setClassName(const char* name) noexcept -> void;
    auto reset() noexcept -> void;      // Reverts IDSystem state back to when its constructor have been just called
    auto getMax() const noexcept -> GLuint;
    auto getIDCount() const noexcept -> GLuint;

    std::set<GLuint> freeIDs;
private:
    GLuint max;
    GLuint count;               // Number of given ID's
    GLuint nextFreeID;
    const char* className;
};
