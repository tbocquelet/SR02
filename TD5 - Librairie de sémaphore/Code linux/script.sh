#!/bin/bash

gcc -c sem_pv.c
ar rvs libsempv.a sem_pv.o
gcc -o sem1 sem1.c -L. -lsempv
