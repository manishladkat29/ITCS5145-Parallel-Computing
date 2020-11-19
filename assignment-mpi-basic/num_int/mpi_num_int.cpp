#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>
#include <sys/time.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

void numInt(int functionId, int a, int b, int n, int intensity){
    float mid = ((b - a) / n );
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    float sum = 0;
    for (int i=rank; i<n; i+=size){
        float x = (a + (i + 0.5) * mid);
        switch(functionid){
            case 1:
              sum +=f1(x, intensity);
              break;

            case 2:
              sum +=f2(x, intensity);
              break;

            case 3:
              sum +=f3(x, intensity);
              break;

            case 4:
              sum +=f4(x, intensity);
              break;
        }
    }
    MPI_Reduce(&sum, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
     cout<<mid*result;
    }
    
}


  
int main (int argc, char* argv[]) {    
    struct timeval start, end;
    gettimeofday(&start, NULL);

    MPI_Init(&argc, &argv);
    if (argc < 6) {
        std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
        return -1;
    }

    float result;
    int functionid = stoi(argv[1]);
    float a = stof(argv[2]);
    float b = stof(argv[3]);
    int n = stoi(argv[4]);
    int intensity = stoi(argv[5]);
    
    numInt(functionId, a, b, n, intensity);
    
    gettimeofday(&end, NULL);

    double s_sec=start.tv_sec;
    double e_sec=end.tv_sec;
    double diff_sec= e_sec-s_sec;
    double s_usec=start.tv_usec;
    double e_usec=end.tv_usec;
    double diff_usec=(e_usec-s_usec)/1000000;
    double last_time= diff_sec+ diff_usec;

    if(rank == 0)
    {
        std::cerr<<last_time<<std::endl;
        //cerr<<secs.count();
    }
    MPI_Finalize();
    
    return 0;
}
