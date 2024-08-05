#include "objects/Object.h"

#include "glad/glad.h"

Object::Object(ObjectID ID){
    ID(ID);
}

auto Object::getID() const -> ObjectID {
    return ID;
}
