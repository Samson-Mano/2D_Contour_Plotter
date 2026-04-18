#include "obj_mesh_data.h"

obj_mesh_data::obj_mesh_data()
{
	// Empty constructor
}

obj_mesh_data::~obj_mesh_data()
{
	// Empty destructor
}

void obj_mesh_data::init(geom_parameters* geom_param_ptr, bool is_paint_geom_pts, bool is_paint_geom_lines, bool is_paint_geom_tris)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the view state (whether the model is only surface, only lines or only pts or paint all?
	this->is_paint_geom_pts = is_paint_geom_pts;
	this->is_paint_geom_lines = is_paint_geom_lines;
	this->is_paint_geom_tris = is_paint_geom_tris;

	// Initialize the mesh objects (points, lines, surfaces)
	this->mesh_points.init(geom_param_ptr);
	this->mesh_lines.init(geom_param_ptr);
	this->mesh_tris.init(geom_param_ptr);

}

void obj_mesh_data::add_mesh_point(const int& point_id, const double& x_coord, const double& y_coord)
{
	// add the points
	this->mesh_points.add_point(point_id, x_coord, y_coord);

}

void obj_mesh_data::add_mesh_lines(const int& line_id, const int& start_pt_id, const int& end_pt_id)
{
	// add the lines
	// Get the start pt and end point
	point_store* start_pt = this->mesh_points.get_point(start_pt_id);
	point_store* end_pt = this->mesh_points.get_point(end_pt_id);

	// If points are not valid... exit
	if (start_pt == nullptr || end_pt == nullptr)
		return;

	this->mesh_lines.add_line(line_id, start_pt, end_pt);

}

void obj_mesh_data::add_mesh_tris(const int& tri_id, const int& point_id1, const int& point_id2, const int& point_id3)
{

	//    2____3 
	//    |   /  
	//    | /    
	//    1      

	// Add the half triangle of the quadrilaterals
	// Add three half edges
	int line_id1, line_id2, line_id3;

	// Add edge 1
	line_id1 = add_half_edge(point_id1, point_id2);

	// Point 1 or point 2 not found
	if (line_id1 == -1)
		return;

	// Add edge 2
	line_id2 = add_half_edge(point_id2, point_id3);

	// Point 3 not found
	if(line_id2 == -1)
	{
		mesh_half_edges.pop_back(); // remove the last item which is edge 1
		half_edge_count--;
		return;
	}


	// Add edge 3
	line_id3 = add_half_edge(point_id3, point_id1);


	//________________________________________
	// Add the mesh triangles
	this->mesh_tris.add_tri(tri_id, mesh_half_edges[line_id1],
		mesh_half_edges[line_id2],
		mesh_half_edges[line_id3]);


	// Set the half edges next line
	mesh_half_edges[line_id1]->next_line = mesh_half_edges[line_id2];
	mesh_half_edges[line_id2]->next_line = mesh_half_edges[line_id3];
	mesh_half_edges[line_id3]->next_line = mesh_half_edges[line_id1];

	// Set the half edge face data
	tri_store* temp_tri = this->mesh_tris.get_triangle(tri_id);

	mesh_half_edges[line_id1]->face = temp_tri;
	mesh_half_edges[line_id2]->face = temp_tri;
	mesh_half_edges[line_id3]->face = temp_tri;

}

void obj_mesh_data::update_mesh_point(const int& point_id, const double& x_coord, const double& y_coord)
{
	// Update the point with new - coordinates
	this->mesh_points.update_point(point_id, x_coord, y_coord);

}

void obj_mesh_data::update_mesh_buffer()
{
	// Update the mesh point buffer
	this->mesh_points.update_buffer();
	this->mesh_lines.update_buffer();

}

void obj_mesh_data::update_mesh_color(const glm::vec3& point_color, const glm::vec3& line_color, const glm::vec3& tri_color)
{
	// Set the color of the mesh
	this->mesh_points.set_point_color(point_color);
	this->mesh_lines.set_line_color(line_color);
	this->mesh_tris.set_tri_color(tri_color);

}

void obj_mesh_data::set_buffer()
{
	// Set the buffer
	this->mesh_tris.set_buffer();
	this->mesh_lines.set_buffer();
	this->mesh_points.set_buffer();

}

void obj_mesh_data::clear_mesh()
{
	// Clear the mesh
	this->mesh_tris.clear_triangles();
	this->mesh_half_edges.clear();
	this->mesh_lines.clear_lines();
	this->mesh_points.clear_points();

}

void obj_mesh_data::paint_static_mesh()
{
	// Paint the static mesh (mesh which are fixed)
	if (is_paint_geom_tris == true)
	{
		// Paint the mesh triangles
		this->mesh_tris.paint_static_triangles();

	}

	if (is_paint_geom_lines == true)
	{
		// Paint the mesh lines
		this->mesh_lines.paint_static_lines();

	}

	if (is_paint_geom_pts == true)
	{
		// Paint the mesh points
		this->mesh_points.paint_static_points();

	}

}

void obj_mesh_data::paint_dynamic_mesh()
{
	// Paint the dynamic mesh (mesh which are not-fixed but variable)
	if (is_paint_geom_tris == true)
	{
		// Paint the mesh triangles
		this->mesh_tris.paint_dynamic_triangles();

	}

	if (is_paint_geom_lines == true)
	{
		// Paint the mesh lines
		this->mesh_lines.paint_dynamic_lines();

	}

	if (is_paint_geom_pts == true)
	{
		// Paint the mesh points
		this->mesh_points.paint_dynamic_points();

	}

}

void obj_mesh_data::update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency)
{
	// Update the openGl uniform matrices
	this->mesh_tris.update_opengl_uniforms(set_modelmatrix, set_viewmatrix, set_transparency);
	this->mesh_lines.update_opengl_uniforms(set_modelmatrix, set_viewmatrix, set_transparency);
	this->mesh_points.update_opengl_uniforms(set_modelmatrix, set_viewmatrix, set_transparency);

}


int obj_mesh_data::add_half_edge(const int& startPt_id, const int& endPt_id)
{
	// Add the Half edge
	line_store* temp_edge = new line_store;
	temp_edge->line_id = half_edge_count;
	temp_edge->start_pt = this->mesh_points.get_point(startPt_id);
	temp_edge->end_pt = this->mesh_points.get_point(endPt_id);

	// Add to the Half edge list
	mesh_half_edges.push_back(temp_edge);

	// Iterate
	half_edge_count++;

	return (half_edge_count - 1); // return the index of last addition
}
