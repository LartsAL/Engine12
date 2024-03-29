#pragma once

#include <map>
#include <set>
#include <glad/glad.h>
#include "Object.h"

using namespace std;

class ObjectManager {
public:
    static auto getInstance() -> ObjectManager&;

    auto createObject() -> Object&;
    auto deleteObject(GLuint ID) -> void;

private:
    ObjectManager();

    GLuint maxObjects;             // Maximum existing objects
    GLuint nextFreeID;             // Next available ID for new Object
    set<GLuint> freeIDs;           // Reusable IDs freed from deleted Objects
    map<GLuint, Object*> objects;  // ID -> Object, map of all existing Objects
};
