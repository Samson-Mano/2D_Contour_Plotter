#include <iostream>
#include <fstream> // for std::ifstream
#include <sstream> // for std::stringstream


#include "src/events_handler/Stopwatch_events.h"

int main()
{
	
	const char* input_file = "simulation_results.bin";   // Adjust path here

	std::ifstream infile(input_file, std::ios::binary);

	if (!infile.is_open())
	{
		std::cerr << "Error: Unable to open input file: " << input_file << std::endl;
		return 0;
	}

	Stopwatch_events stopwatch;
	std::stringstream stopwatch_elapsed_str;

	





	return 0;
}