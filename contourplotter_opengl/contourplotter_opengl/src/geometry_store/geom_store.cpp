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
	boundary_lines.init(&geom_param);

	// Add the boundary points
	std::vector<float> temp_z_values{1.0f, 0.0f}; // Temporary z values for the boundary points

	boundary_lines.add_mesh_point(0, -boundary_width / 2.0f, -boundary_width / 2.0f, temp_z_values);
	boundary_lines.add_mesh_point(1, -boundary_width / 2.0f, boundary_width / 2.0f, temp_z_values);
	boundary_lines.add_mesh_point(2, boundary_width / 2.0f, boundary_width / 2.0f, temp_z_values);
	boundary_lines.add_mesh_point(3, boundary_width / 2.0f, -boundary_width / 2.0f, temp_z_values);

	// Add the boundary lines
	boundary_lines.add_mesh_wireframe(0, 1);
	boundary_lines.add_mesh_wireframe(1, 2);
	boundary_lines.add_mesh_wireframe(2, 3);
	boundary_lines.add_mesh_wireframe(3, 0);


	// Set the boundary of the geometry
	std::pair<glm::vec3, glm::vec3> result = geom_parameters::findMinMaxXY(bndry_pts_list);
	this->geom_param.min_b = result.first;
	this->geom_param.max_b = result.second;
	this->geom_param.geom_bound = geom_param.max_b - geom_param.min_b;

	// Set the center of the geometry
	this->geom_param.center = geom_parameters::findGeometricCenter(bndry_pts_list);
	//_____________________________________________________________________________________
	
	// Load the simulation data
	mesh_data.init(&geom_param); // Initialize the mesh data for points, lines and triangles

	mesh_data.load_simulation_data(infile, boundary_width);
	
	//_____________________________________________________________________________________
	// Initialize the time text data
	text_timedata.init(&geom_param);

	std::string str_time_t = "0000000000";
	glm::vec2 str_time_loc(-1000.0f, 1000.0f);


	text_timedata.add_text(str_time_t, str_time_loc);


	// Set the geometry
	update_model_matrix();
	update_model_zoomfit();



	// Pass the data to simulate window
	op_window->set_simulation_time_ptr(&this->simulation_time_t);


	this->simulation_time_t = 0;

	//_______________________________________________________________________________________________

	is_geometry_set = true;


	// Set the geometry buffers
	this->boundary_lines.create_buffer();
	this->boundary_lines.update_buffer(0);
	this->text_timedata.set_buffer();

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
	mesh_data.update_opengl_uniforms(true, false, true);
	text_timedata.update_opengl_uniforms(true, false, true);

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
	mesh_data.update_opengl_uniforms(false, true, false);
	text_timedata.update_opengl_uniforms(false, true, false);

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
	mesh_data.update_opengl_uniforms(false, true, false);
	text_timedata.update_opengl_uniforms(false, true, false);

}


void geom_store::update_model_zoom(double& z_scale)
{
	if (is_geometry_set == false)
		return;

	// Zoom the geometry
	geom_param.zoom_scale = z_scale;

	// Update the Zoom
	boundary_lines.update_opengl_uniforms(false, true, false);
	mesh_data.update_opengl_uniforms(false, true, false);
	text_timedata.update_opengl_uniforms(false, true, false);

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
	mesh_data.update_opengl_uniforms(false, false, true);
	text_timedata.update_opengl_uniforms(false, false, true);

}


void geom_store::paint_geometry()
{

	if (is_geometry_set == false)
		return;

	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	// Option control change
	option_cmap_change();

	// Update the simulation
	update_simulation();

	// Paint the model
	paint_model();


}

void geom_store::option_cmap_change()
{
	// Options control change
	if (op_window->is_color_theme_changed == true)
	{
		geom_param.CmapOption = op_window->selected_color_theme;

		op_window->is_color_theme_changed = false;	

		boundary_lines.update_opengl_uniforms(true, false, false);
		mesh_data.update_opengl_uniforms(true, false, false);
	}

}

void geom_store::update_simulation()
{
	// Return if the geometry is not set
	if(mesh_data.simType == 0) // Static plot
		return;	

	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	accumulatedTime += deltaTime.count() * (1.0f / op_window->time_scale);


	// Render at fixed time steps (e.g., 100 Hz)

	while (mesh_data.step_i < mesh_data.totalFrames &&
		accumulatedTime >= mesh_data.time_points[mesh_data.step_i])
	{
		simulation_time_t = mesh_data.time_points[mesh_data.step_i];

		mesh_data.update_buffer(mesh_data.step_i);

		mesh_data.step_i++;

		if (mesh_data.step_i >= mesh_data.totalFrames)
		{
			mesh_data.step_i = 0;
			accumulatedTime = 0.0f;
			break;
		}

		if (op_window->is_show_time_text)
		{
			std::string str_time_t = geom_param.format_time_string(simulation_time_t);

			glm::vec2 str_time_loc(-1000.0f, 1000.0f);

			text_timedata.update_text(str_time_t, str_time_loc);
		}
	}
}



void geom_store::paint_model()
{

	
	glLineWidth(1.1f);

	if (op_window->is_show_boundarybox == true)
	{
		// Paint the boundaries
		boundary_lines.paint_mesh(false, true, true);

	}
	

	if (op_window->is_show_time_text == true)
	{
		// Paint the dynamic time text
		text_timedata.paint_dynamic_texts();

	}
		

	// Paint the mesh
	mesh_data.paint_mesh(op_window->is_show_mesh_tris, 
		op_window->is_show_mesh_boundary, op_window->is_show_mesh_points);



}


