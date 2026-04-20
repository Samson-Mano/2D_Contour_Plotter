#include "mesh_data_store.h"


mesh_data_store::mesh_data_store()
{
	// Empty constructor
}

void mesh_data_store::init(geom_parameters* geom_param_ptr)
{
	this->geom_param_ptr = geom_param_ptr;

	this->totalFrames = 0; // Total number of frames in the simulation
	this->time_points.clear(); // Time points for each frame
	this->step_i = 0; // Current step index

}



void mesh_data_store::load_simulation_data(std::ifstream& infile, float boundary_width)
{
	// Temporary variable to store the simulation data
	simulation_data sim_data;

	infile.read(reinterpret_cast<char*>(&sim_data.gridX), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.gridY), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.totalFrames), sizeof(int));
	infile.read(reinterpret_cast<char*>(&sim_data.simType), sizeof(int));

	sim_data.frames.resize(sim_data.totalFrames);

	int gridSize = sim_data.gridX * sim_data.gridY;

	// Store the total frames
	simType = sim_data.simType;
	totalFrames = sim_data.totalFrames;	
	time_points.clear();

	for (int i = 0; i < sim_data.totalFrames; ++i)
	{
		frame_data& frame = sim_data.frames[i];

		infile.read(reinterpret_cast<char*>(&frame.time), sizeof(float));
		infile.read(reinterpret_cast<char*>(&frame.z_max), sizeof(float));
		infile.read(reinterpret_cast<char*>(&frame.z_min), sizeof(float));

		frame.z_values.resize(gridSize);
		infile.read(reinterpret_cast<char*>(frame.z_values.data()),
			sizeof(uint16_t) * gridSize);

		// Store the time points
		time_points.push_back(frame.time);

	}

	// Generate points based on the grid dimensions and boundary width
	float boundary_width_buffered = boundary_width * 0.9f; // Add some buffer to ensure points are within the boundary

	mesh_data.init(geom_param_ptr); // Initialize the mesh data for points, lines and triangles


	int pt_id = 0; // give id to the points

	for (int j = 0; j < sim_data.gridY; ++j)
	{
		for (int i = 0; i < sim_data.gridX; ++i)
		{
			float x = (float)i / (sim_data.gridX - 1);
			float y = (float)j / (sim_data.gridY - 1);

			// Scale x and y to fit within the boundary width
			x = x * boundary_width_buffered - (boundary_width_buffered / 2.0f);
			y = y * boundary_width_buffered - (boundary_width_buffered / 2.0f);

			// Read the z values for this point across all frames
			std::vector<float> z_values(sim_data.totalFrames);
			for (int frame_idx = 0; frame_idx < sim_data.totalFrames; ++frame_idx)
			{
				int z_index = j * sim_data.gridX + i; // Calculate the index for z_values

				// Normalize the z value to the range [0, 1] based on z_min and z_max for the current frame
				float z_min = sim_data.frames[frame_idx].z_min;
				float z_max = sim_data.frames[frame_idx].z_max;
				float z_value = static_cast<float>(sim_data.frames[frame_idx].z_values[z_index]) / 65535.0f;

				//if (z_max > z_min) // Avoid division by zero
				//{
				//	z_value = (z_value - z_min) / (z_max - z_min); // Normalize to [0, 1]
				//}
				//else
				//{
				//	z_value = 0.0f; // If z_max and z_min are the same, set z_value to 0
				//}

				z_values[frame_idx] = z_value;
			}

			mesh_data.add_mesh_point(pt_id, x, y, z_values); // Add the point with normalized z values
			++pt_id;
		}
	}


	// Add the grid wireframe edges and grid triangles
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
			mesh_data.add_mesh_tris(i0, i1, i2);

			// Triangle 2 (CCW order)
			mesh_data.add_mesh_tris(i1, i3, i2);

			// Add the left end edges (to avoid duplicates)
			if (i == 0) { mesh_data.add_mesh_wireframe(i0, i2); } // Left edge
			if (j == 0) { mesh_data.add_mesh_wireframe(i0, i1); } // Bottom edge


			mesh_data.add_mesh_wireframe(i1, i3); // Triangle 2 edge 1
			mesh_data.add_mesh_wireframe(i3, i2); // Triangle 2 edge 2
			mesh_data.add_mesh_wireframe(i2, i1); // Triangle 2 edge 3

		}
	}


	// Create the buffers for the mesh data
	mesh_data.create_buffer();

	this->step_i = 0; // Reset the current step index to 0 after loading the data

}



void mesh_data_store::paint_mesh(bool is_showmesh, bool is_showwireframe, bool is_showpoint)
{
	// Paint the mesh
	mesh_data.paint_mesh(is_showmesh, is_showwireframe, is_showpoint);
}



void mesh_data_store::update_buffer(int timestep_i)
{
	// Update the buffer for the current time step
	mesh_data.update_buffer(timestep_i);
}	



void mesh_data_store::update_opengl_uniforms(bool set_modelmatrix, bool set_viewmatrix, bool set_transparency)
{
	// Update the openGl uniform matrices
	mesh_data.update_opengl_uniforms(set_modelmatrix, set_viewmatrix, set_transparency);	

}





