#include "geom_store.h"

geom_store::geom_store()
{
	// Empty Constructor
}

geom_store::~geom_store()
{
	// Empty Destructor
}

void geom_store::init(options_window* op_window)
{
	// Initialize
	// Initialize the geometry parameters
	geom_param.init();


	is_geometry_set = false;


	// Add the window pointers
	this->op_window = op_window; // Option window


}


void geom_store::initialize_model(std::ifstream& infile)
{
	// Intialize the model
	std::vector< glm::vec3> bndry_pts_list;

	const float boundary_width = 2000.0f;

	bndry_pts_list.push_back({ -boundary_width / 2.0f, -boundary_width / 2.0f, 0 });
	bndry_pts_list.push_back({ -boundary_width / 2.0f, boundary_width / 2.0f, 0 });
	bndry_pts_list.push_back({ boundary_width / 2.0f, boundary_width / 2.0f, 0 });
	bndry_pts_list.push_back({ boundary_width / 2.0f, -boundary_width / 2.0f, 0 });

	//_____________________________________________________________________________________
	// Create the model boundary
	boundary_lines.init(&geom_param, true, true, false);

	// Add the boundary points
	boundary_lines.add_mesh_point(0, -boundary_width / 2.0f, -boundary_width / 2.0f);
	boundary_lines.add_mesh_point(1, -boundary_width / 2.0f, boundary_width / 2.0f);
	boundary_lines.add_mesh_point(2, boundary_width / 2.0f, boundary_width / 2.0f);
	boundary_lines.add_mesh_point(3, boundary_width / 2.0f, -boundary_width / 2.0f);

	// Add the boundary lines
	boundary_lines.add_mesh_lines(0, 0, 1);
	boundary_lines.add_mesh_lines(1, 1, 2);
	boundary_lines.add_mesh_lines(2, 2, 3);
	boundary_lines.add_mesh_lines(3, 3, 0);


	// Set the boundary of the geometry
	std::pair<glm::vec3, glm::vec3> result = geom_parameters::findMinMaxXY(bndry_pts_list);
	this->geom_param.min_b = result.first;
	this->geom_param.max_b = result.second;
	this->geom_param.geom_bound = geom_param.max_b - geom_param.min_b;

	// Set the center of the geometry
	this->geom_param.center = geom_parameters::findGeometricCenter(bndry_pts_list);
	//_____________________________________________________________________________________
	
	// Load the simulation data
	simData = load_simulation_data(infile);


	// Create the geometry objects for the model






	// Set the geometry
	update_model_matrix();
	update_model_zoomfit();



	// Pass the data to simulate window
	op_window->set_simulation_time_ptr(&this->simulation_time_t);





	this->step_i = 0;
	this->simulation_time_t = 0;


	op_window->is_update_dipl_scale = true;

	//_______________________________________________________________________________________________


	is_geometry_set = true;


	// Set the geometry buffers
	this->boundary_lines.set_buffer();


}



void geom_store::fini()
{
	// Deinitialize
	is_geometry_set = false;
}


void geom_store::update_WindowDimension(const int& window_width, const int& window_height)
{
	// Update the window dimension
	this->geom_param.window_width = window_width;
	this->geom_param.window_height = window_height;

	if (is_geometry_set == true)
	{
		// Update the model matrix
		update_model_matrix();
		// !! Zoom to fit operation during window resize is handled in mouse event class !!
	}
}


void geom_store::update_model_matrix()
{
	// Set the model matrix for the model shader
	// Find the scale of the model (with 0.9 being the maximum used)
	int max_dim = geom_param.window_width > geom_param.window_height ? geom_param.window_width : geom_param.window_height;

	double normalized_screen_width = 1.6f * (static_cast<double>(geom_param.window_width) / static_cast<double>(max_dim));
	double normalized_screen_height = 1.6f * (static_cast<double>(geom_param.window_height) / static_cast<double>(max_dim));


	geom_param.geom_scale = std::min(normalized_screen_width / geom_param.geom_bound.x,
		normalized_screen_height / geom_param.geom_bound.y);

	// Translation
	glm::vec3 geom_translation = glm::vec3(-1.0f * (geom_param.max_b.x + geom_param.min_b.x) * 0.5f * geom_param.geom_scale,
		-1.0f * (geom_param.max_b.y + geom_param.min_b.y) * 0.5f * geom_param.geom_scale,
		0.0f);

	glm::mat4 g_transl = glm::translate(glm::mat4(1.0f), geom_translation);

	geom_param.modelMatrix = g_transl * glm::scale(glm::mat4(1.0f), glm::vec3(static_cast<float>(geom_param.geom_scale)));

	// Update the screen point origin
	glm::vec2 screen_topleft = glm::vec2(-1.0f * 0.6f * normalized_screen_width,
		0.5f * normalized_screen_height);


	// Update the model matrix
	boundary_lines.update_opengl_uniforms(true, false, true);


	op_window->is_update_dipl_scale = true;

}

void geom_store::update_model_zoomfit()
{
	if (is_geometry_set == false)
		return;

	// Set the pan translation matrix
	geom_param.panTranslation = glm::mat4(1.0f);

	// Set the zoom scale
	geom_param.zoom_scale = 1.0f;

	// Update the zoom scale and pan translation
	boundary_lines.update_opengl_uniforms(false, true, false);


}

void geom_store::update_model_pan(glm::vec2& transl)
{
	if (is_geometry_set == false)
		return;

	// Pan the geometry
	geom_param.panTranslation = glm::mat4(1.0f);

	geom_param.panTranslation[0][3] = -1.0f * transl.x;
	geom_param.panTranslation[1][3] = transl.y;

	// Update the pan translation
	boundary_lines.update_opengl_uniforms(false, true, false);


}


void geom_store::update_model_zoom(double& z_scale)
{
	if (is_geometry_set == false)
		return;

	// Zoom the geometry
	geom_param.zoom_scale = z_scale;

	// Update the Zoom
	boundary_lines.update_opengl_uniforms(false, true, false);


}

void geom_store::update_model_transperency(bool is_transparent)
{
	if (is_geometry_set == false)
		return;

	if (is_transparent == true)
	{
		// Set the transparency value
		geom_param.geom_transparency = 0.2f;
	}
	else
	{
		// remove transparency
		geom_param.geom_transparency = 1.0f;
	}

	// Update the model transparency
	boundary_lines.update_opengl_uniforms(false, false, true);

}


void geom_store::paint_geometry()
{

	if (is_geometry_set == false)
		return;

	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	// Update the simulation
	update_simulation();

	// Paint the model
	paint_model();


}



void geom_store::update_simulation()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	accumulatedTime += deltaTime.count() * (1.0f / op_window->time_scale);

	// Render at fixed time steps (e.g., 100 Hz)
	while (accumulatedTime >= this->vm_solver.timeinterval)
	{
		simulation_time_t += this->vm_solver.timeinterval;


		if (this->step_i >= vm_solver.total_data_count)
		{
			this->step_i = 0;
			this->simulation_time_t = 0;

			this->graph_mass_mx.reset_points();
			this->graph_phase_portrait.reset_points();

		}


		const simresult_store& step = this->vm_solver.result_datas[this->step_i];
		double scaled_displ_at_t = geom_param.get_remap(this->vm_solver.abs_max_displacement, -1.0 * this->vm_solver.abs_max_displacement,
			1.0f, -1.0f, step.displacement);


		// Update the geometry for the current step
		double scaled_mass_size = (step.mval / this->vm_solver.max_mass);


		model_mass.update_mass_displacement(0, glm::vec2(0.0, zero_disply + (op_window->displ_scale * scaled_displ_at_t)), scaled_mass_size);
		model_spring.update_spring_displacement(0, glm::vec2(0.0, -800.0),
			glm::vec2(0.0, zero_disply + (op_window->displ_scale * scaled_displ_at_t)));


		// Show the mass vs displacement graph
		if (op_window->is_show_massgraph == true)
		{
			// Accumulate time for graph updates
			this->graph_mass_mx.graph_update_time_accum += this->vm_solver.timeinterval;

			// Update the graph points
			if (this->graph_mass_mx.graph_update_time_accum >= this->graph_mass_mx.graph_update_interval_time)
			{		
				// scale the graph points
					double scaled_mass_intensity = geom_param.get_remap(this->vm_solver.max_mass, this->vm_solver.min_mass,
			1.0f, 0.0f, step.mval);

				graph_mass_mx.update_head_point(glm::vec2(-400.0 - (op_window->velo_scale * this->vm_solver.max_velocity_ratio) - (op_window->displ_scale * scaled_mass_intensity),
					zero_disply + (op_window->displ_scale * scaled_displ_at_t)), scaled_mass_intensity);

				// Reset the accumulator after updating the graph
				this->graph_mass_mx.graph_update_time_accum = 0.0f;
			}
		}
		
		// Show the phase portrait graph
		if (op_window->is_show_phaseportrait == true)
		{
			// Accumulate time for graph updates
			this->graph_phase_portrait.graph_update_time_accum += this->vm_solver.timeinterval;


			// Update the graph points
			if (this->graph_phase_portrait.graph_update_time_accum >= this->graph_phase_portrait.graph_update_interval_time)
			{
				// scale the graph points
				double scaled_velo_at_t = geom_param.get_remap(this->vm_solver.abs_max_velocity, -1.0 * this->vm_solver.abs_max_velocity,
					1.0f, -1.0f, step.velocity);

				// Phase amplitude depends on the velocity (convert it 0 to 1)
				double phase_ampl = std::abs(scaled_velo_at_t);
		
				graph_phase_portrait.update_head_point(glm::vec2(0.0 + (op_window->velo_scale * scaled_velo_at_t),
					zero_disply + (op_window->displ_scale * scaled_displ_at_t)), phase_ampl);

				// Reset the accumulator after updating the graph
				this->graph_phase_portrait.graph_update_time_accum = 0.0f;
			}
		}

		if (op_window->is_update_graph_length_factor == true)
		{
			// Update the graph length factor

			this->graph_mass_mx.adjust_graph_length(op_window->graph_mass_length_factor);

			this->graph_phase_portrait.adjust_graph_length(op_window->graph_phase_length_factor);

			// End the event
			op_window->is_update_graph_length_factor = false;
		}

		
		if (op_window->is_update_dipl_scale == true)
		{
			// update the displacement scale for the envelope lines

			if (op_window->is_show_text == true)
			{

				// update the maximum and minimum displacment line
				max_displ_line.update_mesh_point(0, 0.0, zero_disply + (this->vm_solver.max_displacement_ratio * op_window->displ_scale));
				max_displ_line.update_mesh_point(1, -400.0 - (op_window->velo_scale * this->vm_solver.max_velocity_ratio),
					zero_disply + (this->vm_solver.max_displacement_ratio * op_window->displ_scale));

				min_displ_line.update_mesh_point(0, 0.0, zero_disply + (this->vm_solver.min_displacement_ratio * op_window->displ_scale));
				min_displ_line.update_mesh_point(1, -400.0 - (op_window->velo_scale * this->vm_solver.max_velocity_ratio),
					zero_disply + (this->vm_solver.min_displacement_ratio * op_window->displ_scale));

				max_displ_line.update_mesh_buffer();
				min_displ_line.update_mesh_buffer();


				// string max, min
				std::string str_max_displ = geom_param.format_displacement_string(this->vm_solver.max_displacement);
				std::string str_min_displ = geom_param.format_displacement_string(this->vm_solver.min_displacement);

				glm::vec2 str_max_displ_loc = glm::vec2(-400.0 - (op_window->velo_scale * this->vm_solver.max_velocity_ratio), 
					zero_disply + (this->vm_solver.max_displacement_ratio * op_window->displ_scale));
				glm::vec2 str_min_displ_loc = glm::vec2(-400.0 - (op_window->velo_scale * this->vm_solver.max_velocity_ratio), 
					zero_disply + (this->vm_solver.min_displacement_ratio * op_window->displ_scale));

				txt_max_displ.update_text(str_max_displ, str_max_displ_loc);
				txt_min_displ.update_text(str_min_displ, str_min_displ_loc);

				txt_max_displ.update_buffer();
				txt_min_displ.update_buffer();

				// End the event
				op_window->is_update_dipl_scale = false;
			}
		}

		if (op_window->is_show_displ_at_t == true)
		{
			// Get the text displacement time t
			std::string str_displ = geom_param.format_displacement_string(step.displacement);
			glm::vec2 str_displ_loc = glm::vec2(200.0, zero_disply + (op_window->displ_scale * scaled_displ_at_t));

			txt_displ_at_t.update_text(str_displ, str_displ_loc);
		}

		this->step_i++;  // Move to the next time step in the block
		accumulatedTime -= this->vm_solver.timeinterval;
	}
}



void geom_store::paint_model()
{

	
	glLineWidth(1.1f);

	if (op_window->is_show_boundarybox == true)
	{
		// Paint the boundaries
		boundary_lines.paint_static_mesh();

	}
	

	if (op_window->is_show_time_text == true)
	{
		// Paint the dynamic time text
		text_timedata.paint_dynamic_texts();

	}

	if (op_window->is_show_displ_at_t == true)
	{
		// Paint the displacement at time t
		txt_displ_at_t.paint_dynamic_texts();

	}
	

	//_______________________________________________
	glLineWidth(2.1f);

	if (op_window->is_show_massgraph == true)
	{
		// Paint the mass at time t graph
		graph_mass_mx.paint_static_graph();

	}

	if (op_window->is_show_phaseportrait == true)
	{
		// Paint the phase portrait at time t graph
		graph_phase_portrait.paint_static_graph();

	}

	// Paint the model
	model_spring.paint_spring();

	model_fixedends.paint_fixed_end();

	model_mass.paint_pointmass();



}


