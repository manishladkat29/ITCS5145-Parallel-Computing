#!/bin/sh


RESULTDIR=result/
h=`hostname`

echo running on ${h}

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -e .passed_static ];
then
    echo "You must pass all test before submitting"
    mkdir ${RESULTDIR}
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi
    

# $INTENSITIES and $NSPLOT
source ../params.sh
SYNCS="iteration thread"


# debugging statments 
echo THREAD=${THREAD}: before starting time is $(date)
echo 
for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	for sync in ${SYNCS};
	do
 	    ./static_sched 1 0 10 ${n} ${intensity} ${THREAD} ${sync} 2>${RESULTDIR}/static_${n}_${intensity}_${THREAD}_${sync}  >/dev/null 
        done
    done
done

echo THREAD=${THREAD}: after ending time is $(date)
