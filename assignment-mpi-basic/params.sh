#!/bin/bash

#all calls to mpirun should use these extra parameters.
#This particular parameter disables warning from openmpi that arise when prefered back ends are not available.
#MPIRUN_PARAMS="-mca btl_base_warn_component_unused 0"
#mpich does not need them
MPIRUN_PARAMS=""

#the time file should be a single line with a time in seconds.
#some mpi libraries add some debug information. This function strips them from $1
process_time_file () {
    # compare 
    sed -i '/Connect.*/d' $1
    sed -i '/Warning.*/d' $1
}


# common
NODES="6 5 4 3 2 1"
PS="1 2 4 6 8 10 12"


# mpi num int params 
NUM_INT_NS="1000000 10000000 100000000"
INTENSITIES="1 10 100 1000"


# mpi mat mult params
MAT_MUL_NP="1 2 3 4 5"
MAT_MUL_NS="7200 21600 72000 162000 216000" 
ITERS="2"
