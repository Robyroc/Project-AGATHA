cd ..
mkdir bin
cd src
mkdir build
cd ..
cd target
mpic++ -std=c++11 -fopenmp -o ../src/build/target.out matrixmpi.cpp
mpic++ -std=c++11 -fopenmp -o ../bin/target.out matrixmpi.cpp
