#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

void merge(int arr[], int l, int m, int r);

// Utility function to find minimum of two integers
int min(int x, int y) { return (x<y)? x :y; }
 
 
/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(int* arr, int n, int nbthreads)
{
    
    omp_set_num_threads(nbthreads);

    int curr_size;  // For current size of subarrays to be merged
                   // curr_size varies from 1 to n/2
    int left_start; // For picking starting index of left subarray
                   // to be merged

    // Merge subarrays in bottom up manner.  First merge subarrays of
    // size 1 to create sorted subarrays of size 2, then merge subarrays
    // of size 2 to create sorted subarrays of size 4, and so on.
    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
    {
        // Pick starting point of different subarrays of current size
        #pragma omp parallel for schedule(static, 1)
        for (left_start=0; left_start<n-1; left_start += 2*curr_size)
        {
           // Find ending point of left subarray. mid+1 is starting 
           // point of right
           int mid = min(left_start + curr_size - 1, n-1);

           int right_end = min(left_start + 2*curr_size - 1, n-1);

           // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
           merge(arr, left_start, mid, right_end);
        }
    }
}

 
/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int* arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}



int main (int argc, char* argv[]) {
    
    if (argc < 3) { 
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    
    int n = atoi(argv[1]);
    int nbthreads = atoi(argv[2]);
    // get arr data
    int * arr = new int [n];
    
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
          std::cerr<<"something is amiss"<<std::endl;
        }
    }

    generateMergeSortData (arr, n);
    mergeSort(arr, n, nbthreads);    
    checkMergeSortResult (arr, n);

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

    return 0;
}
