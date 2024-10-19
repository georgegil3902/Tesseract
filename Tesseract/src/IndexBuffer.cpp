#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
	// Generating Vertex Buffer
	GLCall(glGenBuffers(1, &m_RendererID));   // generate buffer and save its reference into buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));  // bind buffer as the ARRAY_BUFFER
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));    // uploads data to the currently bound buffer (buffer to which data will be sent which in this case is ARRAY_BUFFER which is bound to buffer at the moment, size of data being copied, pointer to our actual ie the vertices, usage )

}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));  // bind buffer as the ARRAY_BUFFER
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));  // bind buffer as the ARRAY_BUFFER

}
