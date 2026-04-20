
#include <iostream>
#include <fstream> // for std::ifstream
#include <sstream> // for std::stringstream

#include "src/app_window.h"
#include "src/events_handler/Stopwatch_events.h"


// Function to open the contour plotter from C# or Python
extern "C" __declspec(dllexport) int contourplotterGL(const char* input_file)
{

	std::ifstream infile(input_file, std::ios::binary);

	if (!infile.is_open())
	{
		std::cerr << "Error: Unable to open input file: " << input_file << std::endl;
		return -1;
	}

	Stopwatch_events stopwatch;
	std::stringstream stopwatch_elapsed_str;

	try
	{
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
			return -2;
		}

		app.fini();
		//
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return -10;
	}
	catch (...)
	{
		std::cerr << "Unknown exception in DLL.\n";
		return -11;
	}


	return 0; // Success
}


