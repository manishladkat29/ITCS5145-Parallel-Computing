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

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


//calculate sum of array
int taskSum(int* arr, int start, int end)
{
    int task_sum = 0;
        // If there are more threads available, creating nested parallel section
        #pragma omp parallel for reduction(+:task_sum)
            for(int i=start;i<end;i++)
                {
                        task_sum += arr[i];
                            }
                                return task_sum;
                                }


int main (int argc, char* argv[]) {
    
  // struct timeval start, end;
   // gettimeofday(&start, NULL);
    
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

    if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
    }

    
    
    int n = atoi(argv[1]);
    int * arr = new int [n];

    //insert reduction code here

    int result = 0;
    int nbthreads = atoi(argv[2]);
    int granularity = n/(2*nbthreads);
    omp_set_dynamic(0);
    //set number of threads
    omp_set_num_threads(nbthreads);

    //take partial sum
    int * partial_sum = new int [nbthreads];

    generateReduceData (arr, atoi(argv[1]));


    struct timeval start, end;
    gettimeofday(&start, NULL);
    #pragma omp parallel for

    for(int i=0; i<n; i+=granularity){
    int j;
    int begin = i;
    static int x;
    int end = begin+granularity;
    if(end>n){
    end = n;
    }
    #pragma omp task
    {
    //call function to getr sum
    x = taskSum(arr, begin, end);
    #pragma omp critical
    result+=x;
    }
    } 

    //end time
    gettimeofday(&end, NULL);
    //double last_time = (end.tv_sec – start.tv_sec + (end.tv_usec - start.tv_usec)/1000000); // in seconds 
    
    double s_sec=start.tv_sec;
    double e_sec=end.tv_sec;
    double diff_sec= e_sec-s_sec;
    double s_usec=start.tv_usec;
    double e_usec=end.tv_usec;
    double diff_usec=(e_usec-s_usec)/1000000;
    double last_time= diff_sec+ diff_usec;

    //free array
    delete[] arr;

    // display result
    std::cout<<result<<std::endl;
    std::cerr<<last_time<<std::endl;

    delete[] arr;

    return 0;
}
