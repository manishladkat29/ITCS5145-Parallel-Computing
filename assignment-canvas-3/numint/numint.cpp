#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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



int main (int argc, char* argv[]) {
    
    if (argc < 9) {
        std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
        return -1;
    }
    
    int functionid = stoi(argv[1]);
    float a = stof(argv[2]);
    float b = stof(argv[3]);
    int n = stoi(argv[4]);
    int intensity = stoi(argv[5]);
    float mid = ((b - a) / n );
    int nbthreads = stoi(argv[6]);
    string scheduling = argv[7];
    int  granularity = stoi(argv[8]);
    float result = 0.0;

    omp_set_num_threads(nbthreads);


    if("static" == scheduling){
        omp_set_schedule(omp_sched_static, granularity);
    } else if("dynamic" == scheduling){
        omp_set_schedule(omp_sched_dynamic, granularity);
    }else if("guided" == scheduling){
        omp_set_schedule(omp_sched_guided, granularity);
    }
    
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    //forces openmp to create the threads beforehand
    #pragma omp parallel
    {
        int fd = open (argv[0], O_RDONLY);
        if (fd != -1) {
          close (fd);
        }
        else {
          std::cerr<<"something is a miss"<<std::endl;
        }
        
        #pragma omp for reduction (+ : result) schedule (runtime)

            for (int i=0; i<n; i++){
                float x = (a + (i + 0.5) * mid);
                if (functionid == 1){
                    res +=(f1(x, intensity));
                }else if(functionid == 2){
                    res +=(f2(x, intensity));
                }else if(functionid == 3){
                    res +=(f3(x, intensity));
                }else if(functionid == 4){
                    res +=(f4(x, intensity));
                }
            }
    }
  
    //insert code here
    gettimeofday(&end, NULL);

    double s_sec=start.tv_sec;
    double e_sec=end.tv_sec;
    double diff_sec= e_sec-s_sec;
    double s_usec=start.tv_usec;
    double e_usec=end.tv_usec;
    double diff_usec=(e_usec-s_usec)/1000000;
    double last_time= diff_sec+ diff_usec;

    std::cerr<<last_time<<std::endl;

    return 0;
}
