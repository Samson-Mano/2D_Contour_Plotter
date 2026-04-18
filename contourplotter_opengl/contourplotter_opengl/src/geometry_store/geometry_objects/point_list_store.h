#pragma once
#include "../geometry_buffers/gBuffers.h"
#include "../geom_parameters.h"

struct tri_store; // forward declaration

struct point_store
{
	// store the individual point
	int point_id = -1; // Point ID
	double x_coord = 0.0; // x coordinate
	double y_coord = 0.0; // y coordinate

	glm::vec2 pt_coord() const
	{
		return glm::vec2(x_coord, y_coord);
	}
};

class point_list_store
{
	// Store all the points
public:
	geom_parameters* geom_param_ptr = nullptr;
	unsigned int point_count = 0;
	std::unordered_map<int, int> pointId_Map;
	std::vector<point_store> pointMap;

	point_list_store();
	~point_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_point(const int& point_id, const double& x_coord, const double& y_coord );
	point_store* get_point(const int& point_id);

	void update_point(const int& point_id, const double& x_coord, const double& y_coord);

	void set_buffer();
	void update_buffer();
	void set_point_color(const glm::vec3& point_color);

	void paint_static_points();
	void paint_dynamic_points();

	void clear_points();
	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);

private:
	gBuffers point_buffer;
	Shader point_shader;

	void get_point_vertex_buffer(point_store& pt, float* point_vertices, unsigned int& point_v_index);
	void get_point_index_buffer(unsigned int* point_indices, unsigned int& point_i_index);
};



