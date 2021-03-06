#ifndef __SOCKET_H__
#define __SOCKET_H__
/** Crée une socket serveur qui écoute sur toute les interfaces IPv4de la machine sur le port passé en paramètre. 
 * La socket retournéedoit pouvoir être utilisée directement par un appel à accept.La fonction retourne -1 en cas 
 * d'erreur ou le descripteur de lasocket créée. */
int ecouter_serveur();
int creer_serveur(int port, int option, char* arg);
char* substring(const char s[], int p, int l);
void traitement_signal(int sig);
char* fgets_or_exit(char* buffer,int size,FILE* stream);
//void skip_headers(char* buffer,int size,FILE* stream);
void skip_headers(FILE*client);
void send_status(FILE* client,int code,const char* reason_phrase);
void send_response(FILE* client, int code, const char* reason_phrase, int length, const char* message_body);
int copy(FILE *in, FILE *out);
int get_file_size(int sockIn);
FILE*check_and_open(const char*target,const char*document_root);
char *rewrite_target(char *target);
#endif