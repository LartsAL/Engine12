#pragma once

class SystemVars {
public:
    static int screenWidth;
    static int screenHeight;

    static int openGLVersionMajor;
    static int openGLVersionMinor;

    SystemVars() = delete;
    ~SystemVars() = delete;
};