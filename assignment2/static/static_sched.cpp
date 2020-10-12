#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

struct struct1
{
  float global_result;
  float local_result;
  int start;
  int end;
  int functionid;
  int intensity;
  string sync;
  float a;
  float b;
  float step_size;

};

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

pthread_mutex_t mut;
float total_sum = 0;

void* static_worker(void* p)
{
	struct1 *params = (struct1*)p;

	if(0 == params->sync.compare("iteration"))
	{
  		switch(params->functionid)
  		{
    			case 1:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
          				//lock mutex
        				pthread_mutex_lock(&mut);
        				total_sum += f1(x, params->intensity);
          				//unlock mutex
          				pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 2:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
          				//lock mutex
	        			pthread_mutex_lock(&mut);
          				//unlock mutex
        				total_sum +=f2(x, params->intensity);
					pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 3:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				pthread_mutex_lock(&mut);
          				//lock mutex
				        total_sum +=f3(x, params->intensity);
          				//unlock mutex
          				pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 4:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				pthread_mutex_lock(&mut);
          				//lock mutex
        				total_sum +=f4(x, params->intensity);
          				//unlock mutex after writer
					pthread_mutex_unlock(&mut);
      				}
      				break;
   		}
	}
	else{
  		switch(params->functionid){
    			case 1:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				params->local_result += f1(x, params->intensity);
      				}
      				break;

    			case 2:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				params->local_result +=f2(x, params->intensity);
      				}
      				break;

    			case 3:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				params->local_result += f3(x, params->intensity);
      				}
      				break;

    			case 4:
      				for (int i=params->start; i<=params->end; i++){
        				float x = (params->a + (i + 0.5) * params->step_size);
        				params->local_result += f4(x, params->intensity);
      				}
      				break;
   		}
   	}
  return NULL;
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
    	string sync = argv[7];
    
    	float result = 0;
    	float step_size = (b - a) / n;
    
    	pthread_mutex_init(&mut, NULL);
    
    	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    	//calculate your result here
    	pthread_t ths[num_threads];
    	struct struct1 params[num_threads];
    
    	int steps_each_thread = int(n / num_threads);
   	
	if(0 == sync.compare("iteration")){
    		pthread_mutex_init(&mut, NULL);
  	}
	int j = 0;
	for(int i=0;i<num_threads;i++){

		if(j<n){
    			params[i].a = a;
    			params[i].b = b;
    			params[i].functionid = functionid;
    			params[i].intensity = intensity;
    			params[i].start = j;
    			params[i].step_size = step_size;
    			params[i].local_result = 0;
    			params[i].sync = sync;

    			if((i+1) >= num_threads){
      				params[i].end = n - 1;
    			}
   			else{
	     			params[i].end = j + (steps_each_thread-1);
    			}

    			pthread_create(&ths[i], NULL, static_worker, (void*) &params[i]);
  		}
		j+=steps_each_thread;

	}
  	for(int i=0; i < num_threads; i+=1)
  	{
    		pthread_join(ths[i], NULL);
  	}

  	if(0 == sync.compare("iteration"))
   	{
      		pthread_mutex_destroy(&mut);
  	}
  	else
  	{
    		for(int i=0;i < num_threads;i+=1)
     		{
        		total_sum+= params[i].local_result;
     		}
  	}

 
    	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    	std::chrono::duration<double> elapsed_seconds = end-start;
    	// display result and time
    	std::cout<<total_sum*step_size<<std::endl;
    	std::cerr<<elapsed_seconds.count()<<std::endl;
    
    	return 0;
}
