#!/bin/bash
mkdir -p build/
cd build/
if [ "$USER" == "shervin" ];then
    INSTALL_PREFIX="-DCMAKE_INSTALL_PREFIX=/tmp/devel/"
fi
cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=ON $INSTALL_PREFIX

cmake --build .
cmake --build . --target doc
