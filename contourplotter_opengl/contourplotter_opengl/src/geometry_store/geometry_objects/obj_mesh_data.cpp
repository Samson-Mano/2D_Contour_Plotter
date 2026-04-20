#include "obj_mesh_data.h"

obj_mesh_data::obj_mesh_data()
{
	// Empty constructor
}


void obj_mesh_data::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the mesh shader
	auto shaderSrc = ShaderLibrary::Get(ShaderLibrary::ShaderType::MeshShader);

	mesh_shader.create_shader(shaderSrc.vertex.c_str(), shaderSrc.fragment.c_str());

	// Delete all the points, lines and triangles
	clear_mesh();

}



void obj_mesh_data::add_mesh_point(int point_id, float x_coord, float y_coord,
	const std::vector<float>& z_values)
{
	// Add to the list
	pointMap.push_back({ point_id, x_coord, y_coord });

	// Add to the z value list in the [point][time step] format
	point_Zvals.push_back(z_values);

	// Iterate the point count
	point_count++;

}

void obj_mesh_data::add_mesh_wireframe(int startpt_id, int endpt_id)
{
	// add the lines
	lineMap.push_back({ startpt_id, endpt_id });

	// Iterate the line count
	line_count++;

}

void obj_mesh_data::add_mesh_tris(int point_id1, int point_id2, int point_id3)
{
	// add the triangles
	triMap.push_back({ point_id1, point_id2, point_id3 });

	// Iterate the triangle count
	tri_count++;
}




void obj_mesh_data::update_buffer(int timestep_i)
{
	// Define the node z value for a point dynamic buffer

	const auto& frame = point_Zvals[timestep_i];
	// dynamic_buffer = frame; // faster (memcpy)

	std::memcpy(dynamic_buffer.data(), frame.data(), point_count * sizeof(float));

	const unsigned int pointdynamic_vertex_count = 1 * point_count;
	unsigned int pointdynamic_vertex_size = pointdynamic_vertex_count * sizeof(float); // Size of the node_vertex

	// Update the buffer
	mesh_buffer.UpdateDynamicVertexBuffer(dynamic_buffer.data(), pointdynamic_vertex_size);

}



void obj_mesh_data::create_buffer()
{
	// Create the index buffer for the points
	create_point_index_buffer();
	create_line_index_buffer();
	create_tri_index_buffer();	

	//_______________________________________________________________________________________________________________
	// Define the node vertices of the model for a node (2 position) static buffer
	const unsigned int pointstatic_vertex_count = 2 * point_count;
	unsigned int pointstatic_vertex_size = pointstatic_vertex_count * sizeof(float); // Size of the node_vertex

	std::vector<float> pointstatic_vertices;
	
	// Set the point vertex buffers
	for (auto& pt : pointMap)
	{
		// Add vertex buffers
		pointstatic_vertices.push_back(pt.x_coord);
		pointstatic_vertices.push_back(pt.y_coord);

	}

	VertexBufferLayout nodestatic_layout;
	nodestatic_layout.AddFloat(2);  // Node position (x, y)


	//_______________________________________________________________________________________________________________
	// Initialize the dynamic vertext size
	const unsigned int pointdynamic_vertex_count = 1 * point_count; // 1 vertex attribute (z value) for a point
	unsigned int pointdynamic_vertex_size = pointdynamic_vertex_count * sizeof(float); // Size of the mode z value


	VertexBufferLayout nodedynamic_layout;
	nodedynamic_layout.AddFloat(1);  // Node z value (Z)

	dynamic_buffer.resize(point_count);

	// Flip the point_Zvals to [time step][point] format for faster access during buffer updates
	int totalFrames = static_cast<int>(point_Zvals[0].size()); // Total frame is the size of the inner vector (time steps)
	std::vector<std::vector<float>> flipped_point_Zvals(totalFrames, std::vector<float>(point_count));

	for (int p = 0; p < point_count; ++p)
	{
		for (int t = 0; t < totalFrames; ++t)
		{
			flipped_point_Zvals[t][p] = point_Zvals[p][t];
		}
	}

	point_Zvals = std::move(flipped_point_Zvals);

	// Create the point dynamic buffers
	mesh_buffer.CreateBuffers(pointstatic_vertices.data(),
		pointstatic_vertex_size,
		pointdynamic_vertex_size,
		nodestatic_layout,
		nodedynamic_layout);

	//
}


void obj_mesh_data::clear_mesh()
{
	// Clear the mesh
	point_count = 0;
	line_count = 0;
	tri_count = 0;

	point_Zvals.clear();
	pointMap.clear();
	lineMap.clear();
	triMap.clear();	
}


void obj_mesh_data::paint_mesh(bool is_showmesh, bool is_showwireframe, bool is_showpoint)
{
	// Paint the mesh
	mesh_shader.Bind();
	mesh_buffer.Bind();

	if (tri_count != 0 && is_showmesh == true)
	{
		// Paint the triangles
		ibo_tris.Bind();
		glDrawElements(GL_TRIANGLES, (3 * tri_count), GL_UNSIGNED_INT, 0);
		ibo_tris.UnBind();
	}

	if (line_count != 0 && is_showwireframe == true)
	{
		// Paint the wireframe
		ibo_lines.Bind();
		glDrawElements(GL_LINES, (2 * line_count), GL_UNSIGNED_INT, 0);
		ibo_lines.UnBind();
	}

	if (point_count != 0 && is_showpoint == true)
	{
		// Paint the points
		ibo_points.Bind();
		glDrawElements(GL_POINTS, point_count, GL_UNSIGNED_INT, 0);
		ibo_points.UnBind();
	}

	mesh_buffer.UnBind();
	mesh_shader.UnBind();
	//
}





void obj_mesh_data::update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency)
{
	// Update the openGl uniform matrices
	if (set_modelmatrix == true)
	{
		// set the transparency
		mesh_shader.setUniform("vertexTransparency", 1.0f);

		// set the model matrix
		mesh_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);

		// set the model matrix
		mesh_shader.setUniform("projectionMatrix", geom_param_ptr->projectionMatrix, false);

		// Update the Cmap options
		mesh_shader.setUniform("CmapOption", geom_param_ptr->CmapOption);
	}

	if (set_viewmatrix == true)
	{
		glm::mat4 scalingMatrix = glm::mat4(1.0) * static_cast<float>(geom_param_ptr->zoom_scale);
		scalingMatrix[3][3] = 1.0f;

		glm::mat4 viewMatrix = glm::transpose(geom_param_ptr->panTranslation) * scalingMatrix;

		// set the view matrix
		mesh_shader.setUniform("viewMatrix", viewMatrix, false);
	}

	if (set_transparency == true)
	{
		// set the alpha transparency  static_cast<float>(geom_param_ptr->geom_transparency)
		mesh_shader.setUniform("vertexTransparency", static_cast<float>(geom_param_ptr->geom_transparency));

	}
	//
}


void obj_mesh_data::create_point_index_buffer()
{
	// Create the Point buffer
	// Set the buffer for index
	unsigned int point_indices_count = 1 * point_count; // 1 indices to form a point
	std::vector<unsigned int> point_indices;

	unsigned int point_i_index = 0;

	// Set the point index buffers
	for (auto& pt : pointMap)
	{
		//__________________________________________________________________________
		// Add the indices
		point_indices.push_back(point_i_index);

		point_i_index = point_i_index + 1;

	}

	ibo_points.createIndexBuffer(point_indices.data(), point_indices_count);

}



void obj_mesh_data::create_line_index_buffer()
{
	if(line_count == 0)
	{
		return; // No lines to create index buffer for
	}

	// Create the Line buffer
	// Set the buffer for index
	unsigned int line_indices_count = 2 * line_count; // Each line contributes 2 indices
	std::vector<unsigned int> line_indices;

	// Set the line index buffers
	for (auto& ln : lineMap)
	{
		//__________________________________________________________________________
		// Add the indices
		// Index 1
		line_indices.push_back(ln.startpt_id);

		// Index 2
		line_indices.push_back(ln.endpt_id);

	}

	ibo_lines.createIndexBuffer(line_indices.data(), line_indices_count);
//
}



void obj_mesh_data::create_tri_index_buffer()
{
	if (tri_count == 0)
	{
		return; // No triangles to create index buffer for
	}

	// Set the buffer for index
	unsigned int tri_indices_count = 3 * tri_count; // 3 indices to form a triangle
	std::vector<unsigned int> tri_indices;


	// Set the triangle index buffers
	for (auto& tri : triMap)
	{
		// Add index buffers
		// Index 1
		tri_indices.push_back(tri.point_id1);

		// Index 2
		tri_indices.push_back(tri.point_id2);

		// Index 3
		tri_indices.push_back(tri.point_id3);
	}

	ibo_tris.createIndexBuffer(tri_indices.data(), tri_indices_count);
	//
}


