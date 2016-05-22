#!/bin/bash

rm sem_pv.o
rm libsempv.a
gcc -c sem_pv.c
ar rvs libsempv.a sem_pv.o
gcc -o sem1 sem1.c -L. -lsempv
