#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include<stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif
    void generateReduceData (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
    //forces openmp to create the threads beforehand
    if (argc < 5) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
        return -1;
    }
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
    
    omp_set_num_threads(atoi(argv[2]));
    std::string staticStr ("static");
    std::string dynamicStr ("dynamic");
    
    int n = atoi(argv[1]);
    int * arr = new int [n];
    
    // insert reduction code here
    
    int chunkSize = atoi(argv[4]);

    generateReduceData (arr, n);

    struct timeval start, end;
    std::string sched_type = argv[3];
    //std::cout<<scheduling_type<<std::endl;

    int result = arr[0];
    if(staticStr.compare(sched_type)==0){
        omp_set_schedule(omp_sched_static, chunkSize);
    }
    else if(dynamicStr.compare(sched_type)==0){
        omp_set_schedule(omp_sched_dynamic, chunkSize);
    }
    else
    {
        omp_set_schedule(omp_sched_guided, chunkSize);
    }
    gettimeofday(&start, NULL);

#pragma omp parallel reduction(+:result)
    {


#pragma omp for schedule(runtime)
        for(int i=1; i < n; i++ )
            result += arr[i];
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
    
    delete[] arr;
    std::cout<<result<<std::endl; 
    return 0;
}
