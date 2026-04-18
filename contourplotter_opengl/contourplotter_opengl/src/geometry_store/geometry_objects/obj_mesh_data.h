#pragma once
#include "point_list_store.h"
#include "line_list_store.h"
#include "tri_list_store.h"

class obj_mesh_data
{
public:


	obj_mesh_data();
	~obj_mesh_data();

	void init(geom_parameters* geom_param_ptr, 
			  bool is_paint_geom_pts,
			  bool is_paint_geom_lines,
			  bool is_paint_geom_tris);

	void add_mesh_point(const int& point_id,
		const double& x_coord,
		const double& y_coord);

	void add_mesh_lines(const int& line_id,
		const int& start_pt_id,
		const int& end_pt_id);

	void add_mesh_tris(const int& tri_id,
		const int& point_id1,
		const int& point_id2,
		const int& point_id3);

	void update_mesh_point(const int& point_id,
		const double& x_coord,
		const double& y_coord);

	void update_mesh_buffer();

	void update_mesh_color(const glm::vec3& point_color, const glm::vec3& line_color, const glm::vec3& tri_color);

	void set_buffer();

	void clear_mesh();

	void paint_static_mesh();

	void paint_dynamic_mesh();

	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);

private:
	bool is_paint_geom_pts = false;
	bool is_paint_geom_lines = false;
	bool is_paint_geom_tris = false;

	geom_parameters* geom_param_ptr = nullptr;

	point_list_store mesh_points;
	line_list_store mesh_lines;
	tri_list_store mesh_tris;

	int half_edge_count = 0;
	std::vector<line_store*> mesh_half_edges; // All the Half edge data

	int add_half_edge(const int& startPt_id, const int& endPt_id);
};