#!/bin/bash

gcc -c hello-static.c
ar rc libhello-static.a hello-static.o
gcc hello-static-run.c -o res-static -L. -lhello-static

gcc -fPIC -c hello-dynamic.c
gcc -shared -o libhello-dynamic.so hello-dynamic.o
gcc hello-dynamic-run.c -o res-dynamic -L. -lhello-dynamic -Wl,-rpath,.

gcc -fPIC -c hello-dynamic-runtime.c
gcc -shared -o libhello-dynamic-runtime.so hello-dynamic-runtime.o
gcc hello-dynamic-runtime-run.c -o res-dynamic-runtime -ldl
