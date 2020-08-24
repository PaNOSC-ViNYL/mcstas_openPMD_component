# Description
This package provies a McStas component for reading and writing neutrons from/to a file on disk.

# To do
 * [ ] complete the writing
 * [X] implement the reading
 * [ ] add possibility to decide the file format (now hardcoded as HDF5)
 * [ ] neutrons are saved in chunks, the chunk size is hardcoded
   - to be optimized
   - to be made configurable by the user
 * [ ] define the openPMD extension
 * [ ] conform to the extension
 * [ ] add licence file: GPLv3? AGPL? EUPL v2?
 * [ ] add authors and commented header in all files
 * [ ] put on github
 

# Dependencies
 - cmake 3.11.2
 - openPMD C++ API

### CENTOS 8
The CMake version needed for openPMD api, with the options reported in the following instructions, is less stringent than what is marked in the CMakeLists.txt and it is above the version available on CENTOS 8. So a patch is needed:
```
cd openPMD-api
patch  -p1 < ../cmake_api.patch
cd -
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
