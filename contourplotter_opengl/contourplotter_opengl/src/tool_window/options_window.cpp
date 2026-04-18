#include "options_window.h"

options_window::options_window()
{
	// Empty constructor
}

options_window::~options_window()
{
	// Empty destructor
}

void options_window::init()
{
	// Initialize the options
	is_show_window = false;
	is_show_time_text = true;
	is_show_mesh_boundary = true;
	is_show_mesh_points = true;
	is_show_mesh_tris = true;
	is_show_boundarybox = false;

	selected_color_theme = 1;
	is_color_theme_changed = false;

	time_scale = 1.0;
}


void options_window::set_simulation_time_ptr(double* simulation_time_t_ptr)
{
	// Simulation time data
	this->simulation_time_t_ptr = simulation_time_t_ptr;

}


void options_window::render_window()
{
	if (is_show_window == false)
		return;

	// Create a new ImGui options window
	ImGui::Begin("View Options");

	
	// Input box to give input via text
	static bool timescale_input_mode = false;
	static char timescale_str[16] = ""; // buffer to store input time scale string
	static int timescale_input = 0; // buffer to store input time scale value

	// Button to switch to input mode
	if (!timescale_input_mode)
	{
		if (ImGui::Button("Time Scale"))
		{
			timescale_input_mode = true;
			snprintf(timescale_str, 16, "%d", this->time_scale_ms); // set the buffer to current deformation scale value
		}
	}
	else // input mode
	{
		// Text box to input value
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##Time Scale", timescale_str, IM_ARRAYSIZE(timescale_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// Temporarily convert input to float for validation
			int temp_value = atoi(timescale_str);

			// Ensure the value is valid (non-zero, positive)
			if (temp_value > 99)  // Minimal threshold to avoid zero or near-zero issues
			{
				// convert the input string to int
				timescale_input = atoi(timescale_str);
				// set the time scale value to input value
				this->time_scale_ms = timescale_input;
			}
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			timescale_input_mode = false;
		}
	}

	// Text for time scale
	ImGui::SameLine();
	ImGui::Text(" %d", this->time_scale_ms);

	// Slider for Time scale
	int time_scale_flt = static_cast<int>(this->time_scale_ms);

	ImGui::Text("Time Scale");
	ImGui::SameLine();
	ImGui::SliderInt("*", &time_scale_flt, 100, 10000, "%d");
	this->time_scale_ms = time_scale_flt;

	this->time_scale = this->time_scale_ms * 0.001;


	ImGui::Spacing();
	ImGui::Spacing();

	// Display the dynamic heading
	ImGui::Text("%d milli second(s) in real time = 1 second in simulation time", this->time_scale_ms);

	ImGui::Spacing();
	ImGui::Text("%.3f second(s) in real time = 1 second in simulation time", this->time_scale);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Checkbox("Show Time", &is_show_time_text);
	ImGui::Checkbox("Show Mesh Boundary", &is_show_mesh_boundary);
	ImGui::Checkbox("Show Mesh Points", &is_show_mesh_points);
	ImGui::Checkbox("Show Mesh Triangles", &is_show_mesh_tris);
	ImGui::Checkbox("Show bounding box", &is_show_boundarybox);

	ImGui::Spacing();

	// Drop down menu for mesh color map (cmap)
	// Rainbow, Jet, Turbo, Virdis, Plasma, Inferno, Magma, Cividis, Twilight, Twilight Shifted, Sinebow
	// Hot, Cool, Spring, Summer, Autumn, Winter, Gray, Bone, Copper, Pink
	static int current_cmap = this->selected_color_theme;

	const char* cmap_items[] = {
		"Rainbow", "Jet", "Turbo", "Viridis", "Plasma", "Inferno", "Magma", "Cividis",
		"Twilight", "Twilight Shifted", "Sinebow",
		"Hot", "Cool", "Spring", "Summer", "Autumn", "Winter",
		"Gray", "Bone", "Copper", "Pink"
	};

	int cmap_count = IM_ARRAYSIZE(cmap_items);

	ImGui::Spacing();

	ImGui::Text("Mesh Color Map");

	if (ImGui::BeginCombo("##cmap_dropdown", cmap_items[current_cmap]))
	{
		for (int i = 0; i < cmap_count; i++)
		{
			bool is_selected = (current_cmap == i);

			if (ImGui::Selectable(cmap_items[i], is_selected))
				current_cmap = i;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (this->selected_color_theme != current_cmap)
	{
		// Color map change detected, update the color theme in geom_parameters
		this->selected_color_theme = current_cmap;
		is_color_theme_changed = true;
	}
	


	//_________________________________________________________________________________________

	ImGui::Spacing();

	ImGui::Text("Simulation time t = %.3f s", *this->simulation_time_t_ptr);

	// Display the frame rate
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


	ImGui::Spacing();
	ImGui::Spacing();

	// Add a "Close" button
	if (ImGui::Button("Close"))
	{
		is_show_window = false;
	}

	ImGui::End();
}