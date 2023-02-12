#!/bin/bash
#CPD: Container Project Dir
CPD=/usr/src/cpplib-web
docker run -it \
    --rm \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -w ${CPD}/Debug \
     cpplib-web bash -c "
        echo \"Docker container created. About to run program main.\" && \
        ./main
     "