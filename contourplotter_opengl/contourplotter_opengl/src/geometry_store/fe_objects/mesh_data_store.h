#pragma once
#include <vector>

// File system
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../geometry_objects/obj_mesh_data.h"


struct frame_data
{
	float time;
	float z_min;
	float z_max;

	std::vector<uint16_t> z_values; // size = gridX * gridY
};

struct simulation_data
{
	int gridX;
	int gridY;
	int totalFrames;
	int simType;

	std::vector<frame_data> frames;
};


class mesh_data_store
{
public:
	int totalFrames = 0; // Total number of frames in the simulation
	std::vector<float> time_points; // Time points for each frame
	int step_i = 0; // Current step index
	int simType = 0; // Simulation type (0 for static plot, 1 for dynamic plot)

	mesh_data_store();
	~mesh_data_store() = default;

	void init(geom_parameters* geom_param_ptr);
	void load_simulation_data(std::ifstream& infile, float boundary_width);

	void paint_mesh(bool is_showmesh, bool is_showwireframe, bool is_showpoint);

	void update_buffer(int timestep_i);
	void update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency);

private:
	geom_parameters* geom_param_ptr = nullptr;

	obj_mesh_data mesh_data; // Store the mesh data


};

