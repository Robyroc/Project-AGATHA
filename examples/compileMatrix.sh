cd ..
mkdir bin
cd src
mkdir build
cd ..
mpic++ -std=c++11 -fopenmp -o src/build/target.out examples/matrixmpi.cpp
mpic++ -std=c++11 -fopenmp -o bin/target.out examples/matrixmpi.cpp
touch target/command_line_parameters.txt
