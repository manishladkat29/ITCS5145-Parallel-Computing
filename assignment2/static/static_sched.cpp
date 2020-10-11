#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>

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

pthread_mutex_t mut;

void *static_worker_thread(void *p){
    Parameters m = *(Parameters *) p;
    m.local_result += tmp;
}


void *static_worker_iter(void *p){
    Parameters m = *(Parameters *) p;
    pthread_mutex_lock(&mut);
    m.global_result += tmp;
    pthread_mutex_unlock(&mut);
}

int main (int argc, char* argv[]) {
    
    if (argc < 8) {
        std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
        return -1;
    }
    
    int functionid = atoi(argv[1]);
    float a = atof(argv[2]);
    float b = atof(argv[3]);
    float n = atof(argv[4]);
    
    int intensity = atoi(argv[5]);
    int num_threads = atoi(argv[6]);
    const char *sync = argv[7];
    
    float result = 0f;
    float step_size = (b - a) / n;
    
    pthread_mutex_init(&mut, NULL);
    
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    //calculate your result here
    pthread_t ths[num_threads];
    Parameters params[num_threads];
    
    int steps_each_thread = int(n / num_threads);
    if ((int) n % num_threads != 0)
        steps_each_thread += 1;
    
    for (int i = 0; i < num_threads; i++){
        params[i].intensity = intensity;
        params[i].step_size = step_size;
        params[i].start = a + i * step_size * steps_each_thread;
        params[i].end = a + (i + 1) * step_size * steps_each_thread;
        if (i == num_threads - 1)
            params[i].end = b;
        
        params[i].local_result = 0;
        params[i].global_result = &result;
        
        pthread_create(&ths[i], NULL, static_worker, (void *) &params[i]);
    }
    
    for (int i = 0; i < num_threads; i++){
        pthread_join(ths[i], NULL);
    }
    
    for (int i = 0; i < num_threads; i++){
        result += params[i].local_result;
    }
    
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    
    // display result and time
    std::cout<<result<<std::endl;
    std::cerr<<elapsed_seconds.count()<<std::endl;
    
    return 0;
}
