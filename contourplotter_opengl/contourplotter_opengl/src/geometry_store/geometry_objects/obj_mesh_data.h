#pragma once
#include "../geometry_buffers/gBuffers.h"
#include "../geom_parameters.h"


struct point_store
{
	// store the individual point
	int point_id = -1; // Point ID
	float x_coord = 0.0; // x coordinate
	float y_coord = 0.0; // y coordinate
	// std::vector<float> z_values; // Z values for each time step (size = totalFrames)

};


struct line_store
{
	// store the individual point
	int startpt_id = -1; // Start point ID
	int endpt_id = -1; // End point ID

};


struct tri_store
{
	// store the individual Triangle
	int point_id1 = -1; // Point ID 1
	int point_id2 = -1; // Point ID 2
	int point_id3 = -1; // Point ID 3
};


class obj_mesh_data
{
public:


	obj_mesh_data();
	~obj_mesh_data() = default;

	void init(geom_parameters* geom_param_ptr);

	void add_mesh_point(int point_id,
		float x_coord,
		float y_coord, 
		const std::vector<float>& z_values);

	void add_mesh_wireframe(int startpt_id,
		int endpt_id);

	void add_mesh_tris(int point_id1,
		int point_id2,
		int point_id3);


	void update_buffer(int timestep_i);
	void create_buffer();
	void clear_mesh();
	void paint_mesh(bool is_showmesh, bool is_showwireframe, bool is_showpoint);


	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);

private:
	geom_parameters* geom_param_ptr = nullptr;
	
	// Mesh points, lines, and triangles count
	unsigned int point_count = 0;
	unsigned int line_count = 0;
	unsigned int tri_count = 0;

	// Point map and z values
	std::vector<point_store> pointMap;
	std::vector<std::vector<float>> point_Zvals; // [time step][point]

	// Line map	
	std::vector<line_store> lineMap;

	// Triangle map
	std::vector<tri_store> triMap;

	// Dynamic buffer to store the z values for each time step (size = point_count * totalFrames)
	std::vector<float> dynamic_buffer;

	gBuffers mesh_buffer;
	Shader mesh_shader;

	IndexBuffer ibo_points;
	IndexBuffer ibo_lines;
	IndexBuffer	ibo_tris;


	void create_point_index_buffer();
	void create_line_index_buffer();
	void create_tri_index_buffer();


};