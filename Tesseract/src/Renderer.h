#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Macros
#define ASSERT(x) if (!(x)) __debugbreak(); // debug pause in visual studio in the case of assertion not possible
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))  // debug macro that gets errors and prints them

// Clear all errors
void GLClearError();

// Log out the errors read
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};