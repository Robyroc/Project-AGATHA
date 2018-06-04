echo "Credits to Max Hutchinson"
echo "https://github.com/maxhutch/homb"
git clone https://github.com/maxhutch/homb
cd ..
mkdir bin
cd src
mkdir build
cd ..
cd examples
cd homb
mpicc -o ../../bin/target.out src/homb.c -lm -O3 -fopenmp -lmpi
mpicc -o ../../src/build/target.out src/homb.c -lm -O3 -fopenmp -lmpi
cd ..
touch ../target/command_line_parameters.txt
echo "-NRC 2048 -vf output.txt -s" > ../target/command_line_parameters.txt
rm -r homb
