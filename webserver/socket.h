#ifndef __SOCKET_H__
#define __SOCKET_H__
/** Crée une socket serveur qui écoute sur toute les interfaces IPv4de la machine sur le port passé en paramètre. 
 * La socket retournéedoit pouvoir être utilisée directement par un appel à accept.La fonction retourne -1 en cas 
 * d'erreur ou le descripteur de lasocket créée. */
int ecouter_serveur();
int creer_serveur(int port, int option); 
char* substring(const char s[], int p, int l);
#endif