#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
    
    if (argc < 3) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    //forces openmp to create the threads beforehand
    #pragma omp parallel
    {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
    }

    omp_set_dynamic(0);
    omp_set_num_threads(atoi(argv[2]));

    int i, n = atoi(argv[1]);
    int * arr = new int [n];
    int * pr = new int [n+1];
    pr[0]=0;
    int nbthreads = atoi(argv[2]);

    int *temp ;

    //insert prefix sum code here

#pragma omp_set_schedule(omp_sched_static, chunkSize);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    generatePrefixSumData (arr, atoi(argv[1]));
    
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int sum = 0;

#pragma omp single
        {
            nbthreads = omp_get_num_threads();
            temp = new int[nbthreads];
        }
        
#pragma omp for schedule(static)
        for ( i = 0; i<n; i++ )
        {
            sum += arr[i];
            pr[i+1] = sum;
        }
        
#pragma omp critical
        temp[id] = sum;

#pragma omp barrier
        int x = 0;
        for (i=0; i<id; i++)
        {
            x += temp[i];
        }

#pragma omp for schedule(static)
        for (i=0;i< n;i++ )
        {
            pr[i+1] += x;
        }
    }

    gettimeofday(&end, NULL);

    double s_sec=start.tv_sec;
    double e_sec=end.tv_sec;
    double diff_sec= e_sec-s_sec;
    double s_usec=start.tv_usec;
    double e_usec=end.tv_usec;
    double diff_usec=(e_usec-s_usec)/1000000;
    double last_time= diff_sec+ diff_usec;

    std::cerr<<last_time<<std::endl;
    
    checkPrefixSumResult(pr, n);

    delete[] arr;
    delete[] pr;
    delete[] temp;

    return 0;
}
