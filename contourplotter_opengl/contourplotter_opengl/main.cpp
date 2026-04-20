#include <iostream>
#include <fstream> // for std::ifstream
#include <sstream> // for std::stringstream

#include "src/app_window.h"
#include "src/events_handler/Stopwatch_events.h"

#include <windows.h>
#include <filesystem>

std::filesystem::path get_exe_dir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
}



int main()
{

	auto exe_dir = get_exe_dir();
	auto input_file = exe_dir / "simulation_data.bin";
	
	// const char* input_file = "simulation_data.bin";   // Adjust path here

	std::ifstream infile(input_file, std::ios::binary);

	if (!infile.is_open())
	{
		std::cerr << "Error: Unable to open input file: " << input_file << std::endl;
		return 0;
	}

	Stopwatch_events stopwatch;
	std::stringstream stopwatch_elapsed_str;

	
	app_window app;

	// Initialize the application
	app.init();

	// Set the model
	app.set_system(infile);


	if (app.is_glwindow_success == true)
	{
		// Window creation successful (Hide the console window)
		// ShowWindow(GetConsoleWindow(), SW_HIDE); //SW_RESTORE to bring back
		app.app_render();
	}
	else
	{
		// Window creation failed
		// Display error in the console
		// Window creation failed
		std::cerr << "Failed to create OpenGL window." << std::endl;
	}

	app.fini();


	return 0;
}



