#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto element = elements[i];
		// Binding Vertex attribute object (vao) to Vertex Buffer (buffer)
		GLCall(glEnableVertexAttribArray(i));   // Enables the use of a vertex attribute array with an index of 0
		// Associate currently bound vertex buffer with vertex attribute array
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));  // attributes associated with vertex buffer with index 0, no of components per vertex attribute (in this case 2), data type of each component, whether data to be normalised, stride(byte offset between consecutive vertex attributes), offset(initial offset of where the data for this attribute starts within the buffer 0 in this case)
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));

}
