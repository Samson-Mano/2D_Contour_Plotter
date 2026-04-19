#include "gBuffers.h"

gBuffers::gBuffers()
{
	// Empty constructor
}

void gBuffers::Bind() const
{
	// Bind the buffers
	vao.Bind();

}

void gBuffers::UnBind() const
{
	// Un Bind the buffers
	vao.UnBind();

}


void gBuffers::CreateBuffers(const float* vbstatic_data,
	unsigned int vbstatic_size,
	unsigned int vbdynamic_size,
	VertexBufferLayout vbstatic_layout,
	VertexBufferLayout vbdynamic_layout)
{
	vao.createVertexArray();

	//_____________________________________________________________________________
	// Static Vertex buffer (vertices and number of vertices * sizeof(float))
	// Data which never changes, such as vertices of a static model (x, y data)
	vbo_static.createVertexBuffer(vbstatic_data, vbstatic_size);

	// Add to the buffer
	vao.AddBuffer(vbo_static, vbstatic_layout);

	//_____________________________________________________________________________
	// Dynamic Vertex buffer (vertices and number of vertices * sizeof(float))
	vbo_dynamic.createDynamicVertexBuffer(vbdynamic_size);

	// Add to the buffer
	vao.AddBuffer(vbo_dynamic, vbdynamic_layout);


	//// Index buffer (indices and number of indices)
	// ibo.createIndexBuffer(ib_indices, ib_count);

}

void gBuffers::UpdateDynamicVertexBuffer(const float* vbdynamic_data,
	unsigned int& vbdynamic_size)
{
	// Dynamically update the vertex data to the Vertex Buffer
	vbo_dynamic.updateVertexBuffer(vbdynamic_data, vbdynamic_size);
}


