#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


//merge function
void merge(int * arr, int start, int mid, int end, int * temp) {

//if same then stop and return
	if (start == end) 
        return;

	if (end-start == 1) {
		if (arr[start] > arr[end]) {
			int temp = arr[start];
			arr[start] = arr[end];
			arr[end] = temp;
		}
		return;
	}

	int i, j, k;

	int n = mid - start;

	
	for (i=0; i<n; ++i){
	    temp[start+i] = arr[start+i];
    }

	i = 0;    // left
	j = mid;  // right
	k = start;    // merge

	// merge
	while (i<n && j<=end) {
		if (temp[start+i] <= arr[j] ) {
			arr[k++] = temp[start+i];
			i++;
		} else {
			arr[k++] = arr[j++];
		}
	}


	while (i<n) {
		arr[k++] = temp[start+i];
		i++;
	}

}


void mergesort(int * arr, int start, int end, int n, int * temp) {
	long g = 250;
	
	if(n<=250)    //try changing the size and get better speed 
		g = 250;
	else if(n<1000 && n>=250){
		g = 5*n*0.1;
            }
	else if(n>=1000 && n < 100000){
		g = 5*n*0.01;
            }
	else if(n==100000){
		g = 5*n*0.01;
            }
	else if( n==1000000){
		g = 5*n*0.001;
            }
	else if( n==10000000){
		g = 5*n*0.0001;
            }
	else 
		g = 5000;
	
	if (end-start > g) {
		int mid = (start+end)/2;
		
		#pragma omp task
		mergesort(arr, start, mid, n, temp);
		
		#pragma omp task
		mergesort(arr, mid+1, end, n, temp);
		
		#pragma omp taskwait
		merge(arr, start, mid+1, end, temp);
	}
	else
	{
		if (start < end) {
		    int mid = (start+end)/2;
		    mergesort(arr, start, mid, n, temp);
		    mergesort(arr, mid+1, end, n, temp);
		    merge(arr, start, mid+1, end,temp);
		}
	}
}


int main (int argc, char* argv[]) {

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

    if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }

    long n = atol(argv[1]);

        // get arr data
    int * arr = new int [n];
    generateMergeSortData (arr, n);

        //insert sorting code here.

    int nbthreads = atoi(argv[2]);
        //set number of threads
   // omp_set_num_threads(nbthreads);
        
        //temp array
        int * temp = new int [n];
        generateMergeSortData (arr, n);



        //start the time
        struct timeval start, end;
        gettimeofday(&start, NULL);

        // start parallel 
        #pragma omp parallel
        {
            #pragma omp single
            {
                mergesort(arr, 0, n-1, n, temp ); //call mergesort
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
    
        checkMergeSortResult (arr, n);

        delete[] arr;
        delete[] temp;

        return 0;
}
