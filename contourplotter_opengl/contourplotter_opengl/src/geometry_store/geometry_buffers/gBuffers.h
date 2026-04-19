#pragma once
#include "indexBuffer.h"
#include "VertexArray.h"

// Below header files are not used in this class 
#include "Shader.h"
#include "Texture.h"


class gBuffers
{
public:
	gBuffers();
	~gBuffers() = default;
	void Bind()const;
	void UnBind()const;

	void CreateBuffers(const float* vbstatic_data, 
		unsigned int vbstatic_size,
		unsigned int vbdynamic_size,
		VertexBufferLayout vbstatic_layout,
		VertexBufferLayout vbdynamic_layout);

	void UpdateDynamicVertexBuffer(const float* vbdynamic_data,
		unsigned int& vbdynamic_size);

	VertexBuffer vbo_static;
	VertexBuffer vbo_dynamic;
	VertexArray vao;

private:

};