#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket.h"


int socket_serveur;
int opt;
int creer_serveur(int port, int option){
    opt = option;
    socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; /* Socket ipv4 */
    saddr.sin_port = htons(port); /* Port d'écoute */
    saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

    int optval = 1;
    if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval,sizeof(int)) == -1)
        perror("Can not set SO_REUSEADDR option");

    if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("bind socker_serveur");
        return -1;
        /* traitement de l'erreur */
    }
    //10 c'est le backlog donc le nombre d'utilisateurs en simultanées
    if (listen(socket_serveur, 10) == -1){ 
        perror("listen socket_serveur");
        return -1;
        /* traitement d'erreur */
    }

    if(socket_serveur == -1){/* traitement de l'erreur */
        perror("socket_serveur");
        return -1;
    }

    while(1){
        if(ecouter_serveur() == -1){
            perror("ecouter_serveur");
            return -1;
        }
    }

    return 0;
}

int ecouter_serveur(){
    int socket_client;
    socket_client = accept(socket_serveur, NULL, NULL);

    if (socket_client == -1){
        perror("accept");
        return -1;
        /* traitement d'erreur */
    }
    int pidPapa = getpid();
    fork();
    if(getpid()==pidPapa){
        close(socket_client);
    }
    /* On peut maintenant dialoguer avec le client */
    const char *message_bienvenue = "NNNNNXXKOxollllcc:;;;;,,;:cldxxxxdollllccc:cdOKNWWWWWNNNXK0Oxdoc:::;;;;::clodxOKXXNXXNXXXXXXKKKKKKK0\nNNNNXK0kdlc:::::::::;;,,,,;:loooddxxkkkkxdodxkOOOkkkxddoolc;,,'''.............';codk0KXXXXXXXXXXXXKK\nNNXXKOxoc::::::::c:::;;,,;;;:clloddddddollccc::;;,,,,,,'''..........................,:okKXXXXXXKKKKK\nNNXKOxlc::::cccccc:::;;;,,;ccllllccccc:;;,,,,''''......................................':ok0KXXKKKKK\nNXX0OkkOkollloollc::;,,,;:cllc:;;;::;,''''................................................':xKXXKKKK\nNXK0KXNNN0xdddddolc;;;:ccccc;;;;;,''.........................             ..................'ckKXKKK\nNXK00XNWNKxooddddollcccc:;,,,'''..................                         ...................;kKKKK\nXXKOkOKKKOoccloooolc::;,''....................                                .................c0KKK\nXXKOxkO0Oxc;:clllc:;,,'....................                                     ...............c0KKK\nNXKOkO0K0xc:cc:::,''....................     ....................                .............'xXKKK\nXX0OOKXXKkolc:;,''...................   ....................................      ...........'dKXKKK\nXK00KXNN0xoc:,'..................     ...........''''''''''''''''.............      ........;xKXXKKK\nXKKXNNNKxoc;'..............'...     ........''''''''''''',,'',,'''''''''''......     ......cOXXXXKKK\nXKXNWWNkl:,''...............     ...........'''''''''',,,,,',,,,,,,'''''''''.....     ....oKXXXXXKKK\nKXNWWW0l;,''.............      ............''''''''''''''''''''',,,'''''''''......     ..:0NXXXXXKKK\n0XNWWXx:''''......'...        .........''''''''''''''''''''''''''''''''''''''''''..    .;kXXXXXXKKKK\nOKNNN0l,'''......''..         ........'''''''''''''',,,,,,,,''''''''''''''''''''''..   ;OXXXXXXXXKKK\nOKXNN0c,'''.''.'''..         ......'''''''''',,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,'.. .dXXXXXXXXKKKK\nO0KXX0c,'''''''''.      .   .....''''''''''''''''',,,,,,,,,,,,,,,,,,,,,,,,,,,,,''',...;OXXXXXXXXKKKK\nkO0KXKl,'''''','.    .. ........',,,,,,,,''..............'''',,,,,,,,,,,,'''''........c0XXXXXXXXKK00\nkO0KXXO:'',,','.     ..........',,,,,,,,'.....................''',,,,,,'..............:kKKXXXXXXKK00\nkO0KXWNk;',;,'.  .  ..........'',,,,,,,'.........  ..............',,'''.....        ..'',;lOXXXXK000\nkOKXNWMXd;,;,.  ... ..........',,,,,,,'.......      .......  ........... ..... .......'....'lkKXK000\nkOKXNWMWKo:;'................''',,,''........          ..... ................  ......'.....'oOKKK000\nxxk0NWMMNOl;'................,,,,,,,,,,'.....          ...    ....',,,,.....     ....'.....c0XKKK000\nddx0NMMWNOo:;,'......  .....';;;;;;;;;,,........                ..',,,,,'..       ........,xKXKK00OO\nxkOKNWWNOl;;;;;;,'..........,;;;;;;;;;;,'........              ..',,,,,,,..       .......;xKXXKK0000\nxk0KXNN0l,,,,;::;;,'........,;;;;;;;;;;,,,'.....................',,,,,,,,,...........'lxOKXXKKKKKKKK\nxkO0KK0d:,,,,;::;;;;'.......,;;;;;;;;;;;;,,,'''''..........''''',,,,,,,,,,,'.....'''';dKXXKKKKKKKKKK\nkO0KK0ko:,'',;:::;;;,''.....,;;;;;;;;;;;;;;;,,,,,''''''',,,,,,'''',,,,,,,,,,''''''',,,dKKKKKKKKK0000\ndk0000koc;'',;::;,',;;;,...',;;;;;;;;;;:;;;;;;;;;,,,,,,,,,,,,''''',,,,,,;,,,,,,'',,,,;xKKKKKKK000000\nloxkO00Okd:,,;:;,'';::;;;,,;;;;;;;;:;;::::::::;;;;;;;;;,,,;,,'',;;,,,,;;;;;,,,,,',,,,:xKKKK00000000O\ncldOKNNWWNk:,,;,''',;::;;;;;;;;;;;:::::::::::::;;;;;;;;;;;,,'',,,.....'',,,,''''',,,,ckK0000000000OO\n:okKNWWMMMNOc,,,''''',;:::;;;;;;;;;;::::::::::::;;;;;;,,,,,,,''''....'',,,'..'''',,,,oO00000OOOOOOOO\n:oOXNWWWMMMWKd:,,,,;;;;;:::;;;;;;;;;;;;:::::::::;;;;,,,,,,,,,,'''''''''''''..''''',,:d0OOOOOOOOOOOkk\n:o0XNWWWMMMMWN0d:;;;::;;;:::;;;;;;;;;;;;;;::::::;;,,,,,,,''',''''''',''''''...'''',,ckOOOOOOOkkkkkkk\ncd0XNNNNNNNWWWWNKxl::::;,;:::;;;;;;;;;;;;;;::::;;;,,,,,,,'''''''''','''''''..''''',,lOOkkkkkkkkkkkkx\ncx0XXX0OOO0KKKXXNX0xl:::::cc::;;;;;;;;;;;;;;:;;;;;;;,,,,,,,,,,,,,,,,,,',,''''''''',;okkkxxxxkkkkxxxx\ncokOOkxdxkO00000KKKKkc',:cccc::;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,''''''''''.'''',,ckK0Oxdoddddddddd\n:cllllooxkOO000000000xc;:c:ccc::;;;;;;;;;;;;;;;;;;;;,,''''....................'',,;xNMWWKxlccccccccl\n::::ccloxkOO0000000000kocc::cc:::::;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,''''''''''',,lKMMMMWKdlcccccccc\n;::::clodxkkOOOOOOOOOOOkolc::cc::::::;;;;;;;;;;;;;;,,,,,,,;;;;;;;;,,,'''''',,,'',c0WMMMMMXkooooooooo\n;:;;:cclodxkkkkOkOOOOOkkxdc:::ccc::::::::;;::;;;;,,,,,,,,;;;;;;;,,,,''''''',,,,,cOWMMMMMMNOdoooooooo\n:c::::clodxxkkkkOOOOOkkkkxl::::::::::::::;::;;;;;;;;;,,,,;;;;;;,,,,'''''''',,,,c0WMMMMMMMNOolllooool\noddlcloddxkkOOOOOOOOOOOkOkl::;:::::::::;;;::;;;;;;;;,,,;;;;,,,,,,,'''''''''',,lKWMMMMMMMMNx:;:ccllll\ndkkxxOKK0O0000000KKKKKKKKkoc:;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,'''',,,lKMMMMMMMMMMXd;,,,,,;;;\n:oodkKXX0O000OOOO00KKKKKKOoc::;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,c0WMMMMMMMMMMXd;,'......\n..';okOxooddollldkO000000koc:;;;;;;;;;;;;,,;;;,,,,,,,,,;,,,,,,,,,,,,'',,,,':OWMMMMMMMMMMMXo,........\n ...:ll:;clc;.',:oxO0KKKKOdc:;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,''',,,',cOWMMMMMMMMMMMMKl'........\n ...',,''clc,...',cdk0XXXKxc:;;;;;;;;;;;;;;;;;,,,,,,,,'''''''''','''','',oKWMMMMMMMMMMMMMKl'........\n";
    //const char *message_bienvenue = "hello world!";
    if (opt == 1){
        for (int i = 0; i < (int)((strlen(message_bienvenue)+9)/10); i++){
            int nmb = strlen(message_bienvenue) < (unsigned long)((i+1)*10) ? (int)(strlen(message_bienvenue)%10)+1 : 10;

            write(socket_client,substring(message_bienvenue,(i)*10,nmb),10);
            sleep(1);
        }
        return 0;
    }
    
    
    
    write(socket_client, message_bienvenue, strlen(message_bienvenue));
    return 0;
}

char* substring(const char s[], int p, int l) {
   int c = 0;
   char* sub = malloc(l);
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
   return sub;
}
