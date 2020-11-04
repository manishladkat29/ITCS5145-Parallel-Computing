void mergesort_parallel_omp (int a[], int size, int temp[])
{
    if (size < size_threshold) {
        mergesort_serial(a, size, temp);
        return;
    }
#pragma omp task
    mergesort_parallel_omp(a, size/2, temp);
    
    mergesort_parallel_omp(a + size/2, size - size/2, temp + size/2);
    
#pragma omp taskwait
    merge(a, size, temp);
}

#pragma omp parallel num_threads(num_threads)
{
#pragma omp single
    {
        lcs=LCS(X, m, Y, n);
    }
}

#pragma omp parallel shared(x, y, c, len_X, len_Y)
    private(rowIt,colIt,digIt) num_threads(numThreads)
for(digIt = 2; digIt <= (len_X+len_Y); digIt++)
{
#pragma omp for schedule(dynamic)
    for(rowIt =  MIN(len_X, digIt-1);
        rowIt >= MAX(1    , digIt-len_Y);
        rowIt--)
    {
    }
}

for (int i = 0; i < thread_n; i++){
#pragma omp task
    for (int j = (i * chunk_size);
         j < ((i+1) * chunk_size) || (i == (thread_n - 1) && j < n);
         j++){
        pr[i] += arr[j];
    }
}

for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - 1; j++) {
        if (a[j] > a[j + 1]) {
            swap(a[j], a[j + 1]);
        }
    }
}

for(int i = 0;  i < n;  i++ ){
#pragma omp parallel for shared(a)
    for(int j = i%2;  j < N-1;  j += 2){
        if(a[j] > a[j+1])
            swap(a[j],  a[j+1]);
    }
}


#pragma omp task
{
    x = sum(a, begin, end);
#pragma omp critical
    result+=x;
}
