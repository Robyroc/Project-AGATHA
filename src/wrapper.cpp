#include <margot.hpp>
#include <stdlib.h>
#include <string>
#include <numeric>
#include <chrono>
#include <list>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <astrm.hpp>


using namespace std;

int main()
{
	margot::init();
	string stdname = "";
	string mpiname = "";
	int num_thread = 1;
	int num_process = 1;
	int type_bind = 1;
	int repetitions;
	string command = "";
	string actual_type_bind;

	if(margot::bar::update(num_thread, num_process, type_bind))
		margot::bar::manager.configuration_applied();

	switch(type_bind)
	{
		case 1:
			actual_type_bind = string("SPREAD");
			break;
		case 2:
			actual_type_bind = string("CLOSE");
			break;
		case 3:
			actual_type_bind = string("MASTER");
			break;
		case 4:
			actual_type_bind = string("FALSE");
			break;
		default:
			actual_type_bind = string("TRUE");
			break;
	}

	command += "mpirun -np ";
	command += std::to_string(num_process);
	command += " ./target.out ";
	
	ifstream run_commands;
	string line;
	run_commands.open("../../target/command_line_parameters.txt");
	getline(run_commands, line);
	command += line;
	
	command += " > output.txt";

	//setup global variables
	system((string("export OMP_NUM_THREADS=") + to_string(num_thread)).c_str());
	system((string("export OMP_PROC_BIND=") + actual_type_bind).c_str());

	margot::bar::start_monitor();
	system(command.c_str());
	margot::bar::stop_monitor();
	margot::bar::log();
}
