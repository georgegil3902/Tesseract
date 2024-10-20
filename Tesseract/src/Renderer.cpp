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

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the screen

}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();


	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));  // Draw call that uses the binded buffers to draw triangles

}
