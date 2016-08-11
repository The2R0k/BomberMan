#include "network.h"

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("Incorrect input ! \nExample: %s 192.168.0.2 4444\n", argv[0]);
    exit(-1);
  }
  
  return 0;
}
