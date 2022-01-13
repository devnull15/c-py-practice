#!/bin/bash

if [ ! -z "${BUILD_MODULE}" ]; then
	echo [+] BUILD_MODULE=${BUILD_MODULE}
fi;

if [ -e build ]; then
	rm -r build;
fi;

cmake -H. -Bbuild
cmake --build build
