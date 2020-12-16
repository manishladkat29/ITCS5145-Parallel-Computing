#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif

int max(int x, int y) { return (x>y)? x :y; }

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

    if (argc < 4) {
        std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
        return -1;
    }

    struct timeval start, end;

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // get string data 
    char *X = new char[m];
    char *Y = new char[n];
    generateLCS(X, m, Y, n);


    //insert LCS code here.
    int result = -1; // length of common subsequence

    int nbthreads = atoi(argv[3]);
    omp_set_num_threads(nbthreads);
    int maxim = max(n,m);
    // get string data

    generateLCS(X, m, Y, n);
    int C[maxim+1][maxim+1];

    #pragma omp parallel
    {
        #pragma omp single
	{
        	for(int i=0;i<=maxim;i++){
        		C[0][i]=0;
        	}
	}
        #pragma omp single
	{

      		for(int i=0;i<=maxim;i++){
            		C[i][0]=0;
        	}
	}
    }

    //start the time
    gettimeofday(&start, NULL); 

    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int k=1; k<=maxim;k++)
            {
                int granularity = 500;
                if(n<=10)
                    granularity = 50;
                else 
                    granularity += 5*n*10;


                if (X[k-1] == Y[k-1]){
                        C[k][k] = C[k-1][k-1] + 1;
                }else{
                    C[k][k] = max(C[k][k-1],C[k-1][k]);
                }
                #pragma omp task shared(X , Y, C, k, maxim)
                {
                    #pragma omp parallel for schedule(guided, granularity)
                    for(int j = k; j<=maxim;j++)
                    {
                        if (X[k-1] == Y[j]){
                                C[k][j] = C[k-1][j-1] + 1;
                        }else{
                            C[k][j] = max(C[k][j-1],C[k-1][j]);
                        }
                    }
                }
                #pragma omp task shared(X, Y, C, k, maxim) 
                { 
                    #pragma omp parallel for schedule(guided, granularity)
                        for(int i = k;i<=maxim;i++)
                        {
                            if (X[i] == Y[k-1]){
                                    C[i][k] = C[i-1][k-1] + 1;
                            }else{
                                C[i][k] = max(C[i][k-1],C[i-1][k]);
                            }
                        }
                }
                #pragma omp taskwait

            }

            result = C[m][n];
        }
    }

        // end the time
    gettimeofday(&end, NULL); 


    double s_sec=start.tv_sec;
    double e_sec=end.tv_sec;
    double diff_sec= e_sec-s_sec;
    double s_usec=start.tv_usec;
    double e_usec=end.tv_usec;
    double diff_usec=(e_usec-s_usec)/1000000;
    double last_time= diff_sec+ diff_usec;

    std::cerr<<last_time<<std::endl;

    checkLCS(X, m, Y, n, result);


    return 0;
}
