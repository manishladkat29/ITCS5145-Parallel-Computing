#!/bin/sh

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


# import params.
. ../params.sh
P=${PBS_NUM_PPN}
NP=$(expr ${PBS_NP} / ${PBS_NUM_PPN})


for N in ${NUM_INT_NS} ;
do
   for INTEN in ${INTENSITIES} ;
   do
       TIMEFILE=${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P}
       mpirun ${MPIRUN_PARAMS} ./mpi_num_int 1 0 10 ${N} ${INTEN} > /dev/null 2> ${TIMEFILE}

       process_time_file ${TIMEFILE}
   done
done
