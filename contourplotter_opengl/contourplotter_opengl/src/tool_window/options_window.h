#pragma once
#include <iostream>
#include "../../resources/ImGui/imgui.h"
#include "../../resources/ImGui/imgui_impl_glfw.h"
#include "../../resources/ImGui/imgui_impl_opengl3.h"

class options_window
{
public:

	double time_scale = 1.0;

	bool is_show_time_text = true;
	bool is_show_mesh_boundary = true;
	bool is_show_mesh_points = true;
	bool is_show_mesh_tris = true;
	bool is_show_boundarybox = false;
	
	int selected_color_theme = 1;
	bool is_color_theme_changed = false;

	// Window
	bool is_show_window = false;

	options_window();
	~options_window();
	void init();
	void set_simulation_time_ptr(double* simulation_time_t_ptr);
	void render_window();
private:
	int time_scale_ms = 1000;

	double* simulation_time_t_ptr = nullptr;
};