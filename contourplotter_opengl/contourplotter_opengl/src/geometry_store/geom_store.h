#pragma once
#include "geom_parameters.h"

//// File system
//#include <fstream>
//#include <sstream>
//#include <iomanip>

// Tool Windows
#include "../tool_window/options_window.h"

//// FE Objects
//#include "fe_objects/elementfixedend_list_store.h"
//#include "fe_objects/elementmass_list_store.h"
//#include "fe_objects/elementspring_list_store.h"

// Geometry Objects
#include "geometry_objects/obj_mesh_data.h"
#include "geometry_objects/text_list_store.h"
// #include "geometry_objects/graph_data_store.h"

// Solver
//#include "../solver/shm_response_solver.h"
//#include "../solver/respsolver.h"





class geom_store
{
public: 
	const double m_pi = 3.14159265358979323846;
	bool is_geometry_set = false;

	// Main Variable to strore the geometry parameters
	geom_parameters geom_param;

	geom_store();
	~geom_store();

	void init(options_window* op_window);
	void fini();


	void initialize_model(std::ifstream& infile); // Initialize the model

	// Functions to control the drawing area
	void update_WindowDimension(const int& window_width, const int& window_height);
	void update_model_matrix();
	void update_model_zoomfit();
	void update_model_pan(glm::vec2& transl);
	void update_model_zoom(double& z_scale);
	void update_model_transperency(bool is_transparent);


	// Functions to paint the geometry and results
	void paint_geometry();
private:
	const double zero_disply = 200.0;

	// Declare these globally or in your class
	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
	double accumulatedTime = 0.0f;
	
	double simulation_time_t = 0.0; // total simualation time
	int step_i = 0;


	// Window pointers
	options_window* op_window = nullptr;

	// Other geometry objects
	obj_mesh_data boundary_lines;



	// Text data
	text_list_store text_timedata;

	SimulationData load_simulation_data(std::ifstream& infile);

	void update_simulation(); // Update the simuation
	void paint_model(); // Paint the model


};

