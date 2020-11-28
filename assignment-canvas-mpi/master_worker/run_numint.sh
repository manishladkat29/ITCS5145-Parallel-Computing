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


echo starting time is $(date)

for N in ${NUM_INT_NS} ;
do
   for INTEN in ${INTENSITIES} ;
   do
       TIMEFILE=${RESULTDIR}/mpi_master_worker_${N}_${INTEN}_${NP}_${P}
       
       mpirun ${MPIRUN_PARAMS}  ./mpi_master_worker 1 0 10 ${N} ${INTEN} > /dev/null 2> ${TIMEFILE}

       process_time_file ${TIMEFILE}
   done
done

echo ending time is $(date)
