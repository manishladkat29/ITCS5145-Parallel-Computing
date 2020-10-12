#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <cmath>

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

struct struct1{
  	string sync;
  	float a;
 	float b;
  	float step_size;
  	int start;
  	int end;
  	int functionid;
  	int intensity;
  	int n;
};

int steps_left = 0, n, gstart=0, gend=0, granularity=0;
pthread_mutex_t mut1,mut2;
float total_sum = 0.0, mid =0.0;


bool done()
{
  	pthread_mutex_lock(&mut2);
  	if(steps_left == 0)
  	{
    		pthread_mutex_unlock(&mut2);
    		return true;
  	}
  	else
  	{
		if(steps_left < granularity)
      		{
         		steps_left = 0;
      		}
      		else
      		{
          		steps_left = steps_left - granularity;
      		}
      		pthread_mutex_unlock(&mut2);
      		return false;
  	}
}

void getnext (int *start, int *end)
{
	pthread_mutex_lock(&mut2);
	*start = gstart;
	*end = gend;
	if(granularity <= (n-gend))
	{
		gstart = *end;
		gend = gstart + granularity;
	}
	pthread_mutex_unlock(&mut2);
}


void* iteration_level_integration(void* p)
{
   	int start1, end1;
   	while(!done()){
   		struct1 *params = (struct1*)p;

   		getnext(&start1, &end1);

   		for (int i=start1; i<end1; i++){

     			float x = (params->a + (i + 0.5) * params->step_size);

     			switch(params->functionid){
       				case 1:
					pthread_mutex_lock(&mut1);
         				total_sum +=(f1(x, params->intensity) * params->step_size);
	 				pthread_mutex_unlock(&mut1);
         				break;

       				case 2:
	 				pthread_mutex_lock(&mut1);
        	 			total_sum +=(f2(x, params->intensity) * params->step_size);
	 				pthread_mutex_unlock(&mut1);
         				break;

      				case 3:
	 				pthread_mutex_lock(&mut1);
         				total_sum +=(f3(x, params->intensity) * params->step_size);
	 				pthread_mutex_unlock(&mut1);
         				break;

       				case 4:
	 				pthread_mutex_lock(&mut1);
         				total_sum +=(f4(x, params->intensity) * params->step_size);
	 				pthread_mutex_unlock(&mut1);
         				break;
     			}
   		}
  	}
}


void* chunk_level_integration(void* p)
{
  	while(!done())
  	{
     		struct1 *params = (struct1*)p;
     		int start1, end1;
     		float sum = 0.0;
     		getnext(&start1, &end1);
     		for (int i=start1; i<end1; i++)
     		{
       			float x = (params->a + (i + 0.5) * params->step_size);

       			switch(params->functionid){
         			case 1:
           				sum +=(f1(x, params->intensity));
           				break;

         			case 2:
           				sum +=(f2(x, params->intensity));
           				break;

         			case 3:
           				sum +=(f3(x, params->intensity));
           				break;

         			case 4:
           				sum +=(f4(x, params->intensity));
           				break;
       			}
     		}
     
     		pthread_mutex_lock(&mut1);
     		total_sum += sum;
     		pthread_mutex_unlock(&mut1);
  	}
}

void* thread_level_integration(void* p)
{

   	float sum = 0.0;
   	int start1, end1;
   	struct1 *params = (struct1*)p;
   	while(!done()){
     		getnext(&start1, &end1);
     		for (int i=start1; i<end1; i++)
     		{
       			float x = (params->a + (i + 0.5) * params->step_size);
       			switch(params->functionid)
       			{
         			case 1:
           				sum +=(f1(x, params->intensity));
           				break;

         			case 2:
           				sum +=(f2(x, params->intensity));
           				break;

         			case 3:
           				sum +=(f3(x, params->intensity));
           				break;

         			case 4:
           				sum +=(f4(x, params->intensity));
           				break;
       			}
     		} 
   	}
   	pthread_mutex_lock(&mut1);
   	total_sum += sum;
   	pthread_mutex_unlock(&mut1);
}

int main (int argc, char* argv[]) {

  	if (argc < 9) {
    		std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    		return -1;
  	}
    
  	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  	// do your calculation here

  	int functionid = atoi(argv[1]);
  	float a = atof(argv[2]);
  	float b = atof(argv[3]);
  	n = atoi(argv[4]);
  	int intensity = atoi(argv[5]);
  	float step_size = ((b - a) / n );
  	int num_threads = atoi(argv[6]);
  	string sync = argv[7];
  	granularity = atoi(argv[8]);
  	gend = granularity;
  	struct struct1 params[num_threads];
  	pthread_t ths[num_threads];
  	steps_left = n;

  	//thread init
  	pthread_mutex_init(&mut1, NULL);
  	pthread_mutex_init(&mut2, NULL);

  	if(0 == sync.compare("iteration"))
  	{
    		for(int i= 0; i<num_threads;i++)
    		{
      			params[i].a = a;
      			params[i].b = b;
      			params[i].functionid = functionid;
      			params[i].intensity = intensity;

      			params[i].step_size = step_size;
      			params[i].sync = sync;
      			params[i].n = n;
     			pthread_create(&ths[i], NULL, iteration_level_integration, (void*) &params[i]);
    		}
  	}
  	else if(0 == sync.compare("thread"))
  	{
    		for(int i= 0; i<num_threads;i++)
      		{
        		params[i].a = a;
        		params[i].b = b;
        		params[i].functionid = functionid;
        		params[i].intensity = intensity;

        		params[i].step_size = step_size;
        		params[i].sync = sync;
        		params[i].n = n;
          		//create threads
        		pthread_create(&ths[i], NULL, thread_level_integration,(void*) &params[i]);
      		}
  	}
  	else if(0 == sync.compare("chunk"))
  	{
    		for(int i= 0; i<num_threads;i++)
      		{
        		params[i].a = a;
        		params[i].b = b;
        		params[i].functionid = functionid;
        		params[i].intensity = intensity;
        		params[i].step_size = step_size;
        		params[i].sync = sync;
        		params[i].n = n;
          		//create threads
        		pthread_create(&ths[i], NULL, chunk_level_integration, (void*) &params[i]);
  		}
  	}
    
  	for(int i=0; i <num_threads; i++)
  	{
		pthread_join(ths[i], NULL);
  	}

  	pthread_mutex_destroy(&mut1);
  	pthread_mutex_destroy(&mut2);

  	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  	std::chrono::duration<double> elapsed_seconds = end-start;

  	// report reult and time
  	std::cout<<total_sum*step_size<<std::endl;
  	std::cerr<<elapsed_seconds.count()<<std::endl;

  	return 0;
}
