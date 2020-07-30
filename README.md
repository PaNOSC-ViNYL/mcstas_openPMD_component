# Description
This package provies a McStas component for reading and writing neutrons from/to a file on disk.

# To do
 - complete the writing
 - implement the reading
 - add possibility to decide the file format (now hardcoded as HDF5)
 - neutrons are saved in chunks, the chunk size is hardcoded
   - to be optimized
   - to be made configurable by the user
 - define the openPMD extension
 - conform to the extension
 

# Dependencies
 - cmake 3.7.2
 - openPMD C++ API


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

