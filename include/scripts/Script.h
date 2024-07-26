#pragma once

class Script {
public:
    virtual ~Script() = 0;

    virtual auto update() -> void = 0;
    virtual auto fixedUpdate() -> void = 0;
};