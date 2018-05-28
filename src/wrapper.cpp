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

using namespace std;

int main()
{
	margot::init();
	string stdname = "";
	string mpiname = "";
	int num_thread = 1;
	int num_process = 1;
	int repetitions;
	string command = "";
	string compile_command = "";
	ifstream config_file;
	config_file.open("../target/target_data.txt");
	string line;
	if(config_file.is_open())
	{
		getline(config_file, line);
		getline(config_file, line);
		repetitions = stoi(line, nullptr, 10);
		getline(config_file, line);
		getline(config_file, stdname);
		getline(config_file, line);
		getline(config_file, mpiname);
		config_file.close();
	}
	else
	{
		return -1;
	}
	for(int i = 0; i < repetitions; i++)
	{
		if(margot::bar::update(num_thread, num_process))
			margot::bar::manager.configuration_applied();
		if(num_process == 1)
		{
			command = "";
			command += "./target.out ";
			command += std::to_string(num_thread);
			compile_command = "";
			if(num_thread == 1)
				compile_command += "g++ -o target.out ../target/";
			else
				compile_command += "g++ -fopenmp -o target.out ../target/";
			compile_command += stdname;
		}
		else
		{
			command = "";
			command += "mpirun -np ";
			command += std::to_string(num_process);
			command += " target.out ";
			command += std::to_string(num_thread);
			compile_command = "";
			if(num_thread == 1)
				compile_command += "mpic++ -o target.out ../target/";
			else
				compile_command += "mpic++ -fopenmp -o target.out ../target/";
			compile_command += mpiname;
		}
		system(compile_command.c_str());
		margot::bar::start_monitor();
		system(command.c_str());
		margot::bar::stop_monitor();
		margot::bar::log();
	}
	system("rm target.out");
}
