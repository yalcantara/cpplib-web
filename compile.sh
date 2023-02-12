#!/bin/bash
#CPD: Container Project Dir
CPD=/usr/src/cpplib-web
docker run -it \
    --rm \
    -v ${PWD}/include:${CPD}/include \
    -v ${PWD}/CMAkeLists.txt:${CPD}/CMakeLists.txt \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -v ${PWD}/main.cpp:${CPD}/main.cpp \
    -w ${CPD}/Debug \
     cpplib-web bash -c "
        cmake -DCMAKE_BUILD_TYPE=Debug ../ && \
        make
     "