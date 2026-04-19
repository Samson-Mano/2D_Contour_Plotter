#include "VertexArray.h"

VertexArray::VertexArray()
{
	// Empty constructor
}

VertexArray::~VertexArray()
{
	// Destructor: deletes the vertex array object identified by the ID.
	glDeleteVertexArrays(1, &va_id);
}

void VertexArray::createVertexArray()
{
	// Main Constructor: generates a unique vertex array object ID.
	glGenVertexArrays(1, &va_id);
	m_AttribIndex = 0; // Reset the attribute index to 0 when creating a new vertex array
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	// Associates a vertex buffer object with the vertex array object.
	// Bind the vertex array object.
	Bind();

	// Bind the vertex buffer object.
	vbo.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		// Enable the vertex attribute array for the specified buffer index.
		glEnableVertexAttribArray(m_AttribIndex);

		// Set up the vertex attribute pointer for the specified buffer index.
		// This specifies how to interpret the vertex data in the vertex buffer object.
		glVertexAttribPointer(m_AttribIndex, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)(uintptr_t)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

		m_AttribIndex++; // Increment the attribute index for the next buffer
	}

	// Unbind the vertex buffer object.
	vbo.UnBind();

	// Unbind the vertex array object.
	UnBind();
}

void VertexArray::Bind() const
{
	// Binds the vertex array object for use with subsequent OpenGL calls.
	glBindVertexArray(va_id);
}

void VertexArray::UnBind() const
{
	// Unbinds the currently bound vertex array object.
	glBindVertexArray(0);
}
