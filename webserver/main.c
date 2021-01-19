#include <stdio.h>
#include <string.h>
#include "socket.h"

int main(int argc, char** argv){
  if(argc>1 && strcmp(argv[1],"-serv") == 0){
    creer_serveur(8080);
    return 0;
  }
  printf("nothing, type -serv\n");
  return 0;


}
