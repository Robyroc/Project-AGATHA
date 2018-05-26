#include <margot.hpp>
#include <stdlib.h>
#include <string>


int main()
{
	margot::init();
	int num_thread = 1;
	int num_process = 1;
	int repetitions = 100;
	string command = "";
	for(int i = 0; i < repetitions; i++)
	{
		if(margot::bar::update(num_thread) || margot::bar::update(num_process))
			margot::bar::manager.configuration_applied();
		char temp[3];
		temp[0] = ' ';
		temp[1] = code;
		temp[2] = '\0';
		if(num_process == 1)
		{
			command = "";
			command += "./matrix.out "
			command += std::to_String(num_thread);
			if(num_thread == 1)
				system("g++ -o matrix.out matrix.cpp");
			else
				system("g++ -fopenmp -o matrix.out matrix.cpp");
		}
		else
		{
			command = "";
			command += "mpirun -np ";
			command += std::to_String(num_process);
			command += " matrix.out ";
			command += std::to_String(num_thread);
			if(num_thread == 1)
				system("mpic++ -o matrix.out matrixmpi.cpp");
			else
				system("mpic++ -fopenmp -o matrix.out matrixmpi.cpp");
		}
		margot::bar::start_monitor();
		system(command);
		margot::bar::stop_monitor();
		margot::bar::log();
	}
}
