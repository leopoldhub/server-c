#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "socket.h"

void initialiser_signaux(void);

int main(int argc, char** argv){
  initialiser_signaux();
  if(argc>1 && strcmp(argv[1],"-serv") == 0){
    creer_serveur(8080,0,"");
    return 0;
  }
  else if(argc>1 && strcmp(argv[1],"-delais") == 0){
    creer_serveur(8080,1,"");
  }
  else{
    creer_serveur(8080,2,argv[1]);
  }
  printf("nothing, type -serv for normal and -delais to get 10 line by 10 line\n");
  return 0;


}
/*
void initialiser_signaux(void){
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}*/

  
void initialiser_signaux(void){
  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if(sigaction(SIGCHLD, &sa, NULL) == -1)
    perror("sigaction(SIGCHLD)");
    
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}
