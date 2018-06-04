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

typedef struct 
{
	int thread;
	int process;
	int type_bind;
	double time;
	int quantity;
} list_entry;

std::list<list_entry> data;

void add_to_file(int, int, int, double);
void printfile();

int main()
{
	string compiler_name = "";
	int num_thread;
	int num_process;
	int repetitions;
	string command = "";
	string compile_command = "";
	std::list<int> num_thread_values;
	std::list<int> num_process_values;
	std::list<int> type_bind_values;
	string actual_type_bind;

	ifstream config_file;
	config_file.open("../../target/target_data.txt");
	string line;
	if(config_file.is_open())
	{
		getline(config_file, line);

		getline(config_file, line); 
		int values_number = stoi(line,nullptr, 10);

		for(int i = 0; i < values_number; i++) 
		{
			getline(config_file, line);
			num_thread_values.push_back(stoi(line, nullptr, 10));
		}

		getline(config_file, line); 

		getline(config_file, line); 
		values_number = stoi(line,nullptr, 10);

		for(int i = 0; i < values_number; i++) 
		{
			getline(config_file, line);
			num_process_values.push_back(stoi(line, nullptr, 10));
		}

		getline(config_file, line); 

		getline(config_file, line); 
		values_number = stoi(line,nullptr, 10);

		for(int i = 0; i < values_number; i++) 
		{
			getline(config_file, line);
			type_bind_values.push_back(stoi(line, nullptr, 10));
		}

		getline(config_file,line); 

		getline(config_file,line); 
		repetitions = stoi(line, nullptr, 10);

		config_file.close();
	}
	else
	{
		return -1;
	}
	for(std::list<int>::iterator k = num_process_values.begin(); k != num_process_values.end(); ++k)
	{
		for(std::list<int>::iterator j = num_thread_values.begin(); j != num_thread_values.end(); ++j)
		{
			for(std::list<int>::iterator l = type_bind_values.begin(); l != type_bind_values.end(); l++)
			{
				switch(*l)
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

				cout << "Working on case: " << *k << " processes with " << *j << " threads each, binding = " << actual_type_bind << "\n";
				for(int i = 0; i < repetitions; i++)
				{
					num_thread = *j;
					num_process = *k;
					command = "";
					command += "mpirun -np ";
					command += to_string(num_process);
					command += " ./target.out ";

					ifstream run_commands;
					run_commands.open("../../target/command_line_parameters.txt");
					getline(run_commands, line);
					command += line;

					//setup global variables
					system((string("export OMP_NUM_THREADS=") + to_string(num_thread)).c_str());
					system((string("export OMP_PROC_BIND=") + actual_type_bind).c_str());

					auto start = std::chrono::system_clock::now();
					system(command.c_str());
					auto end = std::chrono::system_clock::now();
					std::chrono::duration<double> diff = (end - start);
					add_to_file(num_thread, num_process, *l, diff.count());
				}
			}
		}
	}
	printfile();
}

void add_to_file(int num_thread, int num_process, int type_bind, double time) 
{
	bool found = false;
	for(std::list<list_entry>::iterator it = data.begin(); it != data.end() && !found; ++it)
	{
		if(it->thread == num_thread && it->process == num_process && it->type_bind == type_bind)
		{
			it->time = (it->time * it->quantity + time) / (it->quantity + 1);
			it->quantity += 1;
			found = true;
		}
	}
	if(!found)
	{
		list_entry temp;
		temp.thread = num_thread;
		temp.process = num_process;
		temp.type_bind = type_bind;
		temp.quantity = 1;
		temp.time = time;
		data.push_back(temp);
	}
}

void printfile()
{
	ofstream file;
	file.open("../../config/oplist.conf");
	file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	file << "<points xmlns=\"http://www.multicube.eu/\" version=\"1.3\" block=\"bar\">\n";
	for(std::list<list_entry>::iterator it = data.begin(); it != data.end(); ++it)
	{
		file << "\t<point>\n";
		file << "\t\t<parameters>\n";
		file << "\t\t\t<parameter name = \"num_thread\" value=\"" << it->thread << "\"/>\n";
		file << "\t\t\t<parameter name = \"num_process\" value=\"" << it->process << "\"/>\n";
		file << "\t\t\t<parameter name = \"type_bind\" value=\"" << it->type_bind << "\"/>\n";
		file << "\t\t</parameters>\n";
		file << "\t\t<system_metrics>\n";
		file << "\t\t\t<system_metric name=\"exec_time\" value=\"" << it->time << "\" var=\"1.0\"/>\n";
		file << "\t\t</system_metrics>\n";
		file << "\t</point>\n";
	}
	file << "</points>\n";
	file.close();
}
