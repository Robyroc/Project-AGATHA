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
	double time;
	int quantity;
} list_entry;

std::list<list_entry> data;

void add_to_file(int, int, double);
void printfile();

int main()
{
	int num_thread;
	int num_process;
	int repetitions = 30;
	string command = "";
	std::list<int> num_thread_values;
	std::list<int> num_process_values;
	num_thread_values.push_back(1);
	num_thread_values.push_back(2);
	num_thread_values.push_back(4);
	num_thread_values.push_back(8);
	num_thread_values.push_back(16);
	num_thread_values.push_back(32);
	num_process_values.push_back(1);
	num_process_values.push_back(2);
	num_process_values.push_back(4);
	num_process_values.push_back(8);
	num_process_values.push_back(16);
	num_process_values.push_back(32);
	for(std::list<int>::iterator k = num_process_values.begin(); k != num_process_values.end(); ++k)
	{
		for(std::list<int>::iterator j = num_thread_values.begin(); j != num_thread_values.end(); ++j)
		{
			cout << "Working on case: " << *k << " processes with " << *j << " threads each.\n";
			for(int i = 0; i < repetitions; i++)
			{
				num_thread = *j;
				num_process = *k;
				if(num_process == 1)
				{
					command = "";
					command += "./matrix.out ";
					command += std::to_string(num_thread);
					if(num_thread == 1)
						system("g++ -o matrix.out matrix.cpp");
					else
						system("g++ -fopenmp -o matrix.out matrix.cpp");
				}
				else
				{
					command = "";
					command += "mpirun -np ";
					command += to_string(num_process);
					command += " matrix.out ";
					command += to_string(num_thread);
					if(num_thread == 1)
						system("mpic++ -o matrix.out matrixmpi.cpp");
					else
						system("mpic++ -fopenmp -o matrix.out matrixmpi.cpp");
				}
				/* chrono way
				auto start = std::chrono::system_clock::now();
				system(command.c_str());
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> diff = (end - start);
				add_to_file(num_thread, num_process, diff.count());
				*/
				/* clock way
				clock_t t;
				t = clock();
				system(command.c_str());
				t = clock() - t;
				add_to_file(num_thread, num_process, ((double)t) / CLOCKS_PER_SEC);
				*/
				struct timeval tv1, tv2;
				gettimeofday(&tv1, NULL);
				system(command.c_str());
				gettimeofday(&tv2, NULL);
				add_to_file(num_thread, num_process, ((double) (tv2.tv_usec - tv1.tv_usec)) / 1000000 + ((double)(tv2.tv_sec - tv1.tv_sec)));
			}

		}
	}
	printfile();
}

void add_to_file(int num_thread, int num_process, double time) 
{
	bool found = false;
	for(std::list<list_entry>::iterator it = data.begin(); it != data.end() && !found; ++it)
	{
		if(it->thread == num_thread && it->process == num_process)
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
		temp.quantity = 1;
		temp.time = time;
		data.push_back(temp);
	}
}

void printfile()
{
	ofstream file;
	file.open("oplist.conf");
	file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	file << "<points xmlns=\"http://www.multicube.eu/\" version=\"1.3\" block=\"foo\"\n>";
	for(std::list<list_entry>::iterator it = data.begin(); it != data.end(); ++it)
	{
		file << "\t<point>\n";
		file << "\t\t<parameters>\n";
		file << "\t\t\t<parameter name = \"num_thread\" value=\"" << it->thread << "\"/>\n";
		file << "\t\t\t<parameter name = \"num_process\" value=\"" << it->process << "\"/>\n";
		file << "\t\t</parameters>\n";
		file << "\t\t<system_metrics>\n";
		file << "\t\t\t<system_metric name=\"exec_time\" value=\"" << it->time << "\" var=\"1.0\"/>\n";
		file << "\t\t</system_metrics>\n";
		file << "\t</point>\n";
	}
	file << "</points>\n";
	file.close();
}