#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


float x,val=0,diff,t;
clock_t start,end;
double time_taken;
int functionId;
int a;
int b;
int n;
int intensity;  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    fprintf(stderr, "usage: %s <functionid> <a> <b> <n> <intensity>", argv[0]);
    return -1;
  }
  
  //argv[] array stores all the parameters; Note that, argv[0] is the program itself; arg[1] is the first argument (in our case <functionid>
  //use atof() and atoi() functions to turn the string into float or interger value

  start=clock(); // start clock
  int cnt;

  sscanf(argv[1],"%d", &functionId);
  sscanf(argv[2],"%d", &a);
  sscanf(argv[3],"%d", &b);
  sscanf(argv[4],"%d", &n);
  sscanf(argv[5],"%d", &intensity);

  diff=b-a;
  t=diff/n;
  for(cnt=0;cnt<=n-1;cnt++){
    val=a+((cnt+0.5)*t);

    switch (functionId) {
      case 1: val = val+f1(x,intensity);
        break;
      case 2: val = val+f2(x,intensity);
        break;
      case 3: val = val+f3(x,intensity);
        break;
      case 4: val = val+f4(x,intensity);
        break;
      default:printf("wrong id");
        break;
    }
  }
  val=val*t;
  end=clock();// end clock

  fprintf(stdout, "%f\n", val);
  time_taken =((double)end-start)/CLOCKS_PER_SEC;
  //fprint() allows you to tell which output you want to print to.
  fprintf(stderr, "%f\n", time_taken);
  return 0;
}

