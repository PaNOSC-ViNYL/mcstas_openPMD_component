#!/bin/bash
mkdir -p build/
cd build/
if [ "$USER" == "shervin" ];then
    INSTALL_PREFIX="-DCMAKE_INSTALL_PREFIX=/tmp/devel/"
fi

if [ -z "$MCSTAS" ];then
    PREFIX=/usr/local/
    export MCSTAS=${PREFIX}/mcstas/2.6.1
    export MCSTAS_TOOLS=$PREFIX/mcstas/2.6.1/tools/Perl/
    export PATH=$PREFIX/mcstas/2.6.1/miniconda3/bin:$PREFIX/mcstas/2.6.1/bin:$PATH
fi

# if you want to use mpi
# load the version of MPI
source /usr/share/Modules/init/bash
# switch to ON if you want to use MPI and uncomment either lines to load the MPI version
MPI=OFF
MPI=ON
module load mpi/mpich-x86_64
#module load mpi/openmpi-x86_64

#mcstas-2.6.1-environment

cmake .. -DCMAKE_BUILD_TYPE=Release \
      -DopenPMD_USE_PYTHON=OFF \
      -DopenPMD_INSTALL=OFF \
      -DopenPMD_USE_INTERNAL_CATCH=ON \
      -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=ON \
      -DMPI_CXX_COMPILER=${MPI_BIN}/mpicxx \
      -DMPI_C_COMPILER=${MPI_BIN}/mpicc \
      -DHDF5_FIND_DEBUG=TRUE \
      -DMPI=${MPI} \
      -DTEST=ON \
      $INSTALL_PREFIX

cmake --build .
cmake --build . --target doc
