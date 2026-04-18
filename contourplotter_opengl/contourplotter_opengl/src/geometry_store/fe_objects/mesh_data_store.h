#pragma once
#include <vector>

// File system
#include <fstream>
#include <sstream>
#include <iomanip>

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

struct point_store
{
	int pt_id;
	float x, y;
	// std::vector<float> z_values; // size = totalFrames	
};

struct edge_store
{
	int start_pt_id, end_pt_id;
};

struct triangle_store
{
	int pt1_id, pt2_id, pt3_id;
};


class mesh_data_store
{
public:
	mesh_data_store();
	~mesh_data_store() = default;

	void load_simulation_data(std::ifstream& infile, float boundary_width);

private:
	// Main variable to store the simulation data
	simulation_data sim_data;


	std::vector<point_store> points;
	std::vector<edge_store> wireframe_edges;
	std::vector<triangle_store> triangles;

};

