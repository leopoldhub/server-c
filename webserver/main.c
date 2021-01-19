#include <stdio.h>
#include<string.h>

int main(int argc, char** argv){
  if(argc>1 && strcmp(argv[1],"-advice") == 0){
    printf("Don't panic!\n");
    return 42;
  }
  printf("need an advice ?\n");
  return 0;


}
