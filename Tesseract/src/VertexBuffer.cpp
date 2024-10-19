#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// Generating Vertex Buffer
	GLCall(glGenBuffers(1, &m_RendererID));   // generate buffer and save its reference into buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));  // bind buffer as the ARRAY_BUFFER
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));    // uploads data to the currently bound buffer (buffer to which data will be sent which in this case is ARRAY_BUFFER which is bound to buffer at the moment, size of data being copied, pointer to our actual ie the vertices, usage )

}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));  // bind buffer as the ARRAY_BUFFER
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));  // bind buffer as the ARRAY_BUFFER

}
