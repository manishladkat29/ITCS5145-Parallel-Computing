#include <unistd.h>
#include <stdio.h>

void validateName(int hostname){
  if(hostname == -1){
    print("Invalid hostname");
  }
}

int main () {
	
  char n[256];
  int hostname;

  hostname = gethostname(n, sizeof(n));
  validateName(hostname);

  printf("Hostname: %s\n", n);
  return 0;

}
