#include "mesh_data_store.h"


mesh_data_store::mesh_data_store()
{
	// Empty constructor
}



void mesh_data_store::load_simulation_data(std::ifstream& infile, float boundary_width)
{
	
	infile.read(reinterpret_cast<char*>(&sim_data.gridX), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.gridY), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.totalFrames), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.simType), sizeof(int));

	sim_data.frames.resize(sim_data.totalFrames);

	int gridSize = sim_data.gridX * sim_data.gridY;

	for (int i = 0; i < sim_data.totalFrames; ++i)
	{
		frame_data& frame = sim_data.frames[i];

		infile.read(reinterpret_cast<char*>(&frame.time), sizeof(float));
		infile.read(reinterpret_cast<char*>(&frame.z_max), sizeof(float));
		infile.read(reinterpret_cast<char*>(&frame.z_min), sizeof(float));

		frame.z_values.resize(gridSize);
		infile.read(reinterpret_cast<char*>(frame.z_values.data()),
			sizeof(uint16_t) * gridSize);
	}

	// Generate points based on the grid dimensions and boundary width
	float boundary_width_buffered = boundary_width * 0.9f; // Add some buffer to ensure points are within the boundary

	int pt_id = 0; // give id to the points
	this->points.clear(); // Clear any existing points

	for (int j = 0; j < sim_data.gridY; ++j)
	{
		for (int i = 0; i < sim_data.gridX; ++i)
		{
			float x = (float)i / (sim_data.gridX - 1);
			float y = (float)j / (sim_data.gridY - 1);

			// Scale x and y to fit within the boundary width
			x = x * boundary_width_buffered - (boundary_width_buffered / 2.0f);
			y = y * boundary_width_buffered - (boundary_width_buffered / 2.0f);

			//// Read the z values for this point across all frames
			//std::vector<float> z_values(sim_data.totalFrames);
			//for (int frame_idx = 0; frame_idx < sim_data.totalFrames; ++frame_idx)
			//{
			//	int z_index = j * sim_data.gridX + i; // Calculate the index for z_values
			//	z_values[frame_idx] = static_cast<float>(sim_data.frames[frame_idx].z_values[z_index]);
			//}

			this->points.push_back({pt_id, x, y });
			++pt_id;
		}
	}

	// Add the grid wireframe edges and grid triangles
	this->wireframe_edges.clear(); // Clear any existing edges
	this->triangles.clear(); // Clear any existing triangles

	for (int j = 0; j < sim_data.gridY - 1; ++j)
	{
		for (int i = 0; i < sim_data.gridX - 1; ++i)
		{
			//   i2___i3
			//   | \   |
			//   |  \  |
			//   |   \ |
			//   i0___i1

			int i0 = j * sim_data.gridX + i;
			int i1 = i0 + 1;
			int i2 = i0 + sim_data.gridX;
			int i3 = i2 + 1;

			// Triangle 1 (CCW order)
			triangles.push_back({ i0, i1, i2 });

			// Triangle 2 (CCW order)
			triangles.push_back({ i1, i3, i2 });

			// Add the left end edges (to avoid duplicates)
			if (i == 0) { wireframe_edges.push_back({ i0, i2 }); } // Left edge
			if (j == 0) { wireframe_edges.push_back({ i0, i1 }); } // Bottom edge


			wireframe_edges.push_back({ i1, i3 }); // Triangle 2 edge 1
			wireframe_edges.push_back({ i3, i2 }); // Triangle 2 edge 2
			wireframe_edges.push_back({ i2, i1 }); // Triangle 2 edge 3

		}
	}



}
