#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

struct struct1
{
  float result;
  float sum;
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


void* numerical_integration(void* p)
{
	my_struct *struct_info = (my_struct*)p;

	if(0 == struct_info->sync.compare("iteration"))
	{
  		switch(struct_info->functionid)
  		{
    			case 1:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
          				//lock mutex
        				pthread_mutex_lock(&mut);
        				total_sum += f1(x, struct_info->intensity);
          				//unlock mutex
          				pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 2:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
          				//lock mutex

	        			pthread_mutex_lock(&mut);
          				//unlock mutex

        				total_sum +=f2(x, struct_info->intensity);
					pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 3:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				pthread_mutex_lock(&mut);
          				//lock mutex

				        total_sum +=f3(x, struct_info->intensity);
          				//unlock mutex
          				pthread_mutex_unlock(&mut);
      				}
      				break;

    			case 4:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				pthread_mutex_lock(&mut);
          				//lock mutex

        				total_sum +=f4(x, struct_info->intensity);
          				//unlock mutex after writer
					pthread_mutex_unlock(&mut);
      				}
      				break;
   		}
	}
	else{
  		switch(struct_info->functionid){
    			case 1:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				struct_info->sum += f1(x, struct_info->intensity);
      				}
      				break;

    			case 2:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				struct_info->sum +=f2(x, struct_info->intensity);
      				}
      				break;

    			case 3:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				struct_info->sum += f3(x, struct_info->intensity);
      				}
      				break;

    			case 4:
      				for (int i=struct_info->lb; i<=struct_info->ub; i++){
        				float x = (struct_info->a + (i + 0.5) * struct_info->mid);
        				struct_info->sum += f4(x, struct_info->intensity);
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
