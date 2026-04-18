#pragma once
#include "point_list_store.h"

struct line_store
{
	// store the individual point
	int line_id = -1;

	point_store* start_pt = nullptr;
	point_store* end_pt = nullptr;

	line_store* next_line = nullptr;        // Next half-edge in the same face
	line_store* twin_line = nullptr;        // Opposite half-edge
	tri_store* face = nullptr;    // Face to the left of this half-edge
};


class line_list_store
{
public:

	unsigned int line_count = 0;
	std::unordered_map<int, int> lineId_Map;
	std::vector<line_store> lineMap;

	line_list_store();
	~line_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_line(const int& line_id, point_store* start_pt, point_store* end_pt);
	line_store* get_line(const int& line_id);

	void set_buffer();
	void update_buffer();
	void set_line_color(const glm::vec3& line_color);

	void paint_static_lines();
	void paint_dynamic_lines();

	void clear_lines();
	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);

private:
	geom_parameters* geom_param_ptr = nullptr;

	gBuffers line_buffer;
	Shader line_shader;

	void get_line_vertex_buffer(line_store& ln, float* line_vertices, unsigned int& line_v_index);
	void get_line_index_buffer(unsigned int* line_vertex_indices, unsigned int& line_i_index);

};



