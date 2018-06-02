git clone https://gitlab.com/margot_project/core.git
cd core
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/home/roberto/Documents/mpi/OpenMp-MPI-Autotuning-mARGOt/core ..
make
make install
cd ..
cd ..
cp -r core/margot_heel/margot_heel_if/ .
rm margot_heel_if/config/*.conf
cp config/*.conf margot_heel_if/config
cd margot_heel_if/
mkdir build
cd build
cmake ..
make
cd ..
cd ..
mkdir build
cd build
cmake ..
make
make install
