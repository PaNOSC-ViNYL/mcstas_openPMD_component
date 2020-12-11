# Description
This package provies a McStas component for reading and writing neutrons from/to a file on disk.

# To do
 * [X] complete the writing
 * [X] implement the reading
 * [ ] add possibility to decide the file format (now hardcoded as HDF5)
 * [ ] neutrons are saved in chunks, the chunk size is hardcoded
   - to be optimized
   - to be made configurable by the user
 * [ ] define the openPMD extension
 * [ ] conform to the extension:
   - `https://github.com/DavidSagan/openPMD-standard/blob/EXT_BeamPhysics/EXT_BeamPhysics.md`
 * [X] add licence file: GPLv2
 * [ ] add authors and commented header in all files
 * [ ] put on github
 * [ ] make it compatible with MPI
   - [X] install MPICH: instructions for setting environment modules added to the README
   - [ ] add compilation with mpicc and execution tests with mpirun
   - [ ] read the openPMD-api documentation about MPI
   - [ ] output of different MPI nodes as separate "iterations" in openPMD (maybe it does not need merging)
 * [X] Implement the repeat_count functionality of MCPL
 * [ ] Implement the energy/position/velocity smearing capability
 * [ ] Make comparison with MCPL
   * [X] Disk space
   * [ ] Speed: it should be a bit faster because there is not GZIP compression
 * [ ] Make it possible to select MCPL or openPMD in the Test_CppWrap.instr from the command line
 * [ ] Test compilation with mcrun and the way McStas users would do
 
# Comparison of openPMD vs MCPL
## Output (save)
   - speed
   - disk space: H5 is almost twice as big as compressed MCPL
| # neutrons | mcpl_output.mcpl.gz | openPMD_out.h5 |
|------------|---------------------|----------------|
| 1e6        | 27  MB               | 46  MB          |
| 1e7        | 263 MB               | 458 MB          |


# Dependencies
 - cmake 3.11.2
 - openPMD C++ API
 - hdf5-mpi
 
### CENTOS 8
The CMake version needed for openPMD api, with the options reported in the following instructions, is less stringent than what is marked in the CMakeLists.txt and it is above the version available on CENTOS 8. So a patch is needed:
```
cd openPMD-api
patch  -p1 < ../cmake_api.patch
cd -
```

Don't need both version of mpi, just choose one
```
yum install -y hdf5-{devel,static} hdf5-mpich* hdf5-openmpi*
```

You need to select the mpi version you want to use. To see the available modules:
```
source /usr/share/Modules/init/$SHELL
module avail
```
Then, either `module load mpi/mpich-x86_64` or `module load mpi/openmpi-x86_64`

Don't forget to load the mcstas environment variables:
```
mcstas-2.6.1-environment
```


### Ubuntu 18
You need a more recent version of CMake
```
sudo apt-get update
sudo apt-get install apt-transport-https ca-certificates gnupg software-properties-common wget

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null

sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
sudo apt-get update
```

# How to compile
The openPMD API is compiled from source.

```
#!/bin/bash
mkdir build/
cd build/
cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
#set -x LD_LIBRARY_PATH $PWD/build/lib/
cmake --build .
```


## Tests
```
cd build/
ctest --verbose
```

# Structure of the repository
 - `mcstas_component/` : contains an example instrument and the McStas component
 - `include/`          : contains the public header files used by the component
 - `src/`              : contains the implementation of the C wrapper, the definition and implementation of the C++ library that uses the openPMD API and auxiliary classes

Check the wrap.h and wrap.cc dependency graph for more information.
