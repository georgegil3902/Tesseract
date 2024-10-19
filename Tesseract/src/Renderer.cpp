#include "Renderer.h"
#include <iostream>

// Clear all errors
void GLClearError()
{
    while (glGetError());
}

// Log out the errors read
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] ( " << error << " ) : " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}