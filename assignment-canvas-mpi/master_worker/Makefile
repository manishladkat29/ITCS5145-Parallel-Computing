CXX=mpicxx
CC=mpicxx
LD=mpicxx
CXXFLAGS=-O2 -std=c++11
LDLIBS=libfunctions.a 

mpi_master_worker:

bench: mpi_master_worker
	./queue.sh

plot:
	./plot.sh

test: mpi_master_worker ../approx
	./test.sh

clean:
	-rm *.o
	-rm mpi_master_worker

distclean:
	rm *.sh.*
