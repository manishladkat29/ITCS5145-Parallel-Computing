#include <iostream>
#include <chrono>


void polynomial_expansion (float* poly, int degree,
			   int n, float* array) {
  //TODO: Write code to use the GPU here!
  //code should write the output back to array
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if( index < n )
  {
    float out = 0.0;
    float xtothepowerof = 1.0;
    for ( int i = 0; i < degree+1; ++i)
    {
      out += xtothepowerof * poly[i];
      xtothepowerof *= array[index];
    }
    array[index] = out;
  }
}


int main (int argc, char* argv[]) {
  //TODO: add usage
  
  if (argc < 3) {
     std::cerr<<"usage: "<<argv[0]<<" n degree"<<std::endl;
     return -1;
  }

  int n = atoi(argv[1]); //TODO: atoi is an unsafe function
  int degree = atoi(argv[2]);
  int nbiter = 1;

  float* array = new float[n];
  float* poly = new float[degree+1];
  for (int i=0; i<n; ++i)
    array[i] = 1.;

  for (int i=0; i<degree+1; ++i)
    poly[i] = 1.;

  float *dev_array, *dev_poly;
  
  std::chrono::time_point<std::chrono::system_clock> begin, end;
  begin = std::chrono::system_clock::now();
  
  //for (int iter = 0; iter<nbiter; ++iter)
    //polynomial_expansion (poly, degree, n, array);
    
  cudaMallocManaged(&dev_array, n*sizeof(float));
  cudaMallocManaged(&dev_poly, (degree+1)*sizeof(float));

  cudaMemcpy(dev_array, array, n*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_poly, poly, (degree+1)*sizeof(float), cudaMemcpyHostToDevice);

  polynomial_expansion<<<(n+255)/256, 256>>>(dev_poly, degree, n, dev_array);
  cudaMemcpy(array, dev_array, n*sizeof(float), cudaMemcpyDeviceToHost);
  cudaFree(dev_array);
  cudaFree(dev_poly);
  cudaDeviceSynchronize();

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end-begin)/nbiter;

  {
    bool correct = true;
    int ind;
    for (int i=0; i< n; ++i) {
      if (fabs(array[i]-(degree+1))>0.01) {
        correct = false;
	ind = i;
      }
    }
    if (!correct)
      std::cerr<<"Result is incorrect. In particular array["<<ind<<"] should be "<<degree+1<<" not "<< array[ind]<<std::endl;
  }
  

  std::cerr<<array[0]<<std::endl;
  std::cout<<n<<" "<<degree<<" "<<totaltime.count()<<std::endl;

  delete[] array;
  delete[] poly;

  return 0;
}
