#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct data{
    int tid;
    int n_thread;
};

pthread_mutex_t mut;

void* print(void *ptr){
    struct data *local = (struct data *) ptr;
    int t_id = local->tid;
    int n_thread = local->n_thread;
    pthread_mutex_lock(&mut);
    cout << "I am thread " << t_id << " in " << n_thread << endl;
    pthread_mutex_unlock(&mut);
    return NULL;
}

int main (int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
        return -1;
    }
    
    int n_thread = atoi(argv[1]);
    pthread_mutex_init(&mut, NULL);
    
    pthread_t* threads = (pthread_t*)(new pthread_t[n_thread]);
    struct data d[n_thread];
    
    for (int i = 0; i < n_thread; i++){
        d[0].tid = i;
        d[0].n_thread = n_thread;
        pthread_create(&threads[i], NULL, print, (void *)&d[0]);
        //create the thread
    }
    
    for (int i = 0; i < n_thread; i++){
        //wait for all threads to finish
        pthread_join(thread[i], NULL);
    }
    
    return 0;
}
