#include "tri_list_store.h"

tri_list_store::tri_list_store()
{
	// Empty constructor
}

tri_list_store::~tri_list_store()
{
	// Empty destructor
}

void tri_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the triangle shader
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	tri_shader.create_shader((shadersPath.string() + "/resources/shaders/default_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/default_frag_shader.frag").c_str());

	tri_shader.setUniform("vertexColor", geom_param_ptr->geom_colors.triangle_color);

	// Delete all the triangles
	clear_triangles();
}

void tri_list_store::add_tri(const int& tri_id, line_store* edge1, line_store* edge2, line_store* edge3)
{
	// Add to the list
	tri_store* temp_tri = new tri_store;

	temp_tri->tri_id = tri_id;
	temp_tri->edge1 = edge1;
	temp_tri->edge2 = edge2;
	temp_tri->edge3 = edge3;

	triMap.push_back(temp_tri);

	// Add to the tri id map
	triId_Map.insert({ tri_id, tri_count });

	// Iterate the triangle count
	tri_count++;
}


tri_store* tri_list_store::get_triangle(const int& tri_id)
{
	// Check whether tri_id exists?
	auto it = triId_Map.find(tri_id);

	if (it != triId_Map.end())
	{
		// tri id exists
		// return the address of the triangle
		return triMap[it->second];
	}
	else
	{
		// id not found
		return nullptr;
	}
}


void tri_list_store::set_buffer()
{

	// Set the buffer for index
	unsigned int tri_indices_count = 3 * tri_count; // 3 indices to form a triangle
	unsigned int* tri_vertex_indices = new unsigned int[tri_indices_count];

	unsigned int tri_i_index = 0;

	// Set the triangle index buffers
	for (auto& tri : triMap)
	{
		// Add index buffers
		get_tri_index_buffer(tri_vertex_indices, tri_i_index);
	}

	VertexBufferLayout tri_pt_layout;
	tri_pt_layout.AddFloat(2);  // Node center


	// Define the tri vertices of the model for a node (2 position) 
	const unsigned int tri_vertex_count = 2 * 3 * tri_count;
	unsigned int tri_vertex_size = tri_vertex_count * sizeof(float); // Size of the node_vertex

	// Create the triangle dynamic buffers
	tri_buffer.CreateDynamicBuffers(tri_vertex_size, tri_vertex_indices, tri_indices_count, tri_pt_layout);

	// Delete the dynamic index array
	delete[] tri_vertex_indices;

	update_buffer();

}

void tri_list_store::set_tri_color(const glm::vec3& tri_color)
{
	// Set the tri color
	tri_shader.setUniform("vertexColor", tri_color);

}

void tri_list_store::paint_static_triangles()
{
	// Paint all the triangles
	tri_shader.Bind();
	tri_buffer.Bind();
	glDrawElements(GL_TRIANGLES, (3 * tri_count), GL_UNSIGNED_INT, 0);
	tri_buffer.UnBind();
	tri_shader.UnBind();

}

void tri_list_store::paint_dynamic_triangles()
{
	// Paint all the triangles
	tri_shader.Bind();
	tri_buffer.Bind();

	// Update the tri buffer data for dynamic drawing
	update_buffer();

	glDrawElements(GL_TRIANGLES, (3 * tri_count), GL_UNSIGNED_INT, 0);
	tri_buffer.UnBind();
	tri_shader.UnBind();

}

void tri_list_store::update_buffer()
{

	// Define the tri vertices of the model for a point 3 * (2 position) 
	const unsigned int tri_vertex_count = 2 * 3 * tri_count;
	float* tri_vertices = new float[tri_vertex_count];

	unsigned int tri_v_index = 0;

	// Set the tri vertex buffers
	for (auto& tri : triMap)
	{
		// Add vertex buffers
		get_tri_vertex_buffer(tri, tri_vertices, tri_v_index);
	}

	unsigned int tri_vertex_size = tri_vertex_count * sizeof(float); // Size of the tri_vertex

	// Update the buffer
	tri_buffer.UpdateDynamicVertexBuffer(tri_vertices, tri_vertex_size);

	// Delete the dynamic vertices array
	delete[] tri_vertices;


}

void tri_list_store::clear_triangles()
{
	// Delete all the triangles
	tri_count = 0;
	triMap.clear();
	triId_Map.clear();
}

void tri_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency)
{
	if (set_modelmatrix == true)
	{
		// set the transparency
		tri_shader.setUniform("vertexTransparency", 1.0f);

		// set the model matrix
		tri_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);

	}

	if (set_viewmatrix == true)
	{
		glm::mat4 scalingMatrix = glm::mat4(1.0) * static_cast<float>(geom_param_ptr->zoom_scale);
		scalingMatrix[3][3] = 1.0f;

		glm::mat4 viewMatrix = glm::transpose(geom_param_ptr->panTranslation) * scalingMatrix;

		// set the pan translation
		tri_shader.setUniform("viewMatrix", viewMatrix, false);
	}


	if (set_transparency == true)
	{
		// set the alpha transparency
		tri_shader.setUniform("vertexTransparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}
}

void tri_list_store::get_tri_vertex_buffer(tri_store* tri, float* tri_vertices, unsigned int& tri_v_index)
{
	// Get the three node buffer for the shader
	// Point 1
	// Point location
	tri_vertices[tri_v_index + 0] = tri->edge1->start_pt->x_coord;
	tri_vertices[tri_v_index + 1] = tri->edge1->start_pt->y_coord;

	// Iterate
	tri_v_index = tri_v_index + 2;

	// Point 2
	// Point location
	tri_vertices[tri_v_index + 0] = tri->edge2->start_pt->x_coord;
	tri_vertices[tri_v_index + 1] = tri->edge2->start_pt->y_coord;

	// Iterate
	tri_v_index = tri_v_index + 2;

	// Point 3
	// Point location
	tri_vertices[tri_v_index + 0] = tri->edge3->start_pt->x_coord;
	tri_vertices[tri_v_index + 1] = tri->edge3->start_pt->y_coord;

	// Iterate
	tri_v_index = tri_v_index + 2;
}

void tri_list_store::get_tri_index_buffer(unsigned int* tri_vertex_indices, unsigned int& tri_i_index)
{
	//__________________________________________________________________________
	// Add the indices
	// Index 1
	tri_vertex_indices[tri_i_index] = tri_i_index;

	tri_i_index = tri_i_index + 1;

	// Index 2
	tri_vertex_indices[tri_i_index] = tri_i_index;

	tri_i_index = tri_i_index + 1;

	// Index 3
	tri_vertex_indices[tri_i_index] = tri_i_index;

	tri_i_index = tri_i_index + 1;

}



