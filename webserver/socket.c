#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "socket.h"


int socket_serveur;
int opt;
int creer_serveur(int port, int option){
    opt = option;
    socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
    /*####DOMAINS####
        AF_UNIX  UNIX domain sockets
        AF_UNSPEC  Unspecified
        AF_INET  Internet domain sockets
    */
    /*####TYPES####
        SOCK_STREAM
            Provides sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a transmission mechanism for out-of-band data.
        SOCK_DGRAM
            Provides datagrams, which are connectionless-mode, unreliable messages of fixed maximum length.
        SOCK_SEQPACKET
            Provides sequenced, reliable, bidirectional, connection-mode transmission path for records.
    */
    /*####PROTOCOLS####
        ip      0       IP              # internet protocol, pseudo protocol number
        hopopt  0       HOPOPT          # hop-by-hop options for ipv6
        icmp    1       ICMP            # internet control message protocol
        igmp    2       IGMP            # internet group management protocol
        ggp     3       GGP             # gateway-gateway protocol
        ipencap 4       IP-ENCAP        # IP encapsulated in IP (officially ``IP'')
        st      5       ST              # ST datagram mode
        tcp     6       TCP             # transmission control protocol
        cbt     7       CBT             # CBT, Tony Ballardie <A.Ballardie@cs.ucl.ac.uk>
        egp     8       EGP             # exterior gateway protocol
        igp     9       IGP             # any private interior gateway (Cisco: for IGRP)
        bbn-rcc 10      BBN-RCC-MON     # BBN RCC Monitoring
    */

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
    }
    //10 c'est le backlog donc le nombre d'utilisateurs en simultanées
    if (listen(socket_serveur, 10) == -1){ 
        perror("listen socket_serveur");
        return -1;
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
    struct sockaddr_in client;
    size_t size = sizeof(client);
    int socket_client = accept(socket_serveur, (struct sockaddr *)&client, (unsigned int *)&size);

    if (socket_client == -1){
        perror("accept");
        return -1;
    }

    int frk = fork();//si frk == -1 => erreur   si frk == 0 => on est dans le fils   sinon frk = PID fils et on est dans le père
    if(frk == -1){
        perror("fork");
        return -1;
    }else if(frk > 0){
        return 0;
    } 

    printf("incoming connection from %s:%u on PID %u\n", inet_ntoa(client.sin_addr), (unsigned int)ntohs(client.sin_port), getpid());

    const char *message_bienvenue = "NNNNNXXKOxollllcc:;;;;,,;:cldxxxxdollllccc:cdOKNWWWWWNNNXK0Oxdoc:::;;;;::clodxOKXXNXXNXXXXXXKKKKKKK0\nNNNNXK0kdlc:::::::::;;,,,,;:loooddxxkkkkxdodxkOOOkkkxddoolc;,,'''.............';codk0KXXXXXXXXXXXXKK\nNNXXKOxoc::::::::c:::;;,,;;;:clloddddddollccc::;;,,,,,,'''..........................,:okKXXXXXXKKKKK\nNNXKOxlc::::cccccc:::;;;,,;ccllllccccc:;;,,,,''''......................................':ok0KXXKKKKK\nNXX0OkkOkollloollc::;,,,;:cllc:;;;::;,''''................................................':xKXXKKKK\nNXK0KXNNN0xdddddolc;;;:ccccc;;;;;,''.........................             ..................'ckKXKKK\nNXK00XNWNKxooddddollcccc:;,,,'''..................                         ...................;kKKKK\nXXKOkOKKKOoccloooolc::;,''....................                                .................c0KKK\nXXKOxkO0Oxc;:clllc:;,,'....................                                     ...............c0KKK\nNXKOkO0K0xc:cc:::,''....................     ....................                .............'xXKKK\nXX0OOKXXKkolc:;,''...................   ....................................      ...........'dKXKKK\nXK00KXNN0xoc:,'..................     ...........''''''''''''''''.............      ........;xKXXKKK\nXKKXNNNKxoc;'..............'...     ........''''''''''''',,'',,'''''''''''......     ......cOXXXXKKK\nXKXNWWNkl:,''...............     ...........'''''''''',,,,,',,,,,,,'''''''''.....     ....oKXXXXXKKK\nKXNWWW0l;,''.............      ............''''''''''''''''''''',,,'''''''''......     ..:0NXXXXXKKK\n0XNWWXx:''''......'...        .........''''''''''''''''''''''''''''''''''''''''''..    .;kXXXXXXKKKK\nOKNNN0l,'''......''..         ........'''''''''''''',,,,,,,,''''''''''''''''''''''..   ;OXXXXXXXXKKK\nOKXNN0c,'''.''.'''..         ......'''''''''',,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,'.. .dXXXXXXXXKKKK\nO0KXX0c,'''''''''.      .   .....''''''''''''''''',,,,,,,,,,,,,,,,,,,,,,,,,,,,,''',...;OXXXXXXXXKKKK\nkO0KXKl,'''''','.    .. ........',,,,,,,,''..............'''',,,,,,,,,,,,'''''........c0XXXXXXXXKK00\nkO0KXXO:'',,','.     ..........',,,,,,,,'.....................''',,,,,,'..............:kKKXXXXXXKK00\nkO0KXWNk;',;,'.  .  ..........'',,,,,,,'.........  ..............',,'''.....        ..'',;lOXXXXK000\nkOKXNWMXd;,;,.  ... ..........',,,,,,,'.......      .......  ........... ..... .......'....'lkKXK000\nkOKXNWMWKo:;'................''',,,''........          ..... ................  ......'.....'oOKKK000\nxxk0NWMMNOl;'................,,,,,,,,,,'.....          ...    ....',,,,.....     ....'.....c0XKKK000\nddx0NMMWNOo:;,'......  .....';;;;;;;;;,,........                ..',,,,,'..       ........,xKXKK00OO\nxkOKNWWNOl;;;;;;,'..........,;;;;;;;;;;,'........              ..',,,,,,,..       .......;xKXXKK0000\nxk0KXNN0l,,,,;::;;,'........,;;;;;;;;;;,,,'.....................',,,,,,,,,...........'lxOKXXKKKKKKKK\nxkO0KK0d:,,,,;::;;;;'.......,;;;;;;;;;;;;,,,'''''..........''''',,,,,,,,,,,'.....'''';dKXXKKKKKKKKKK\nkO0KK0ko:,'',;:::;;;,''.....,;;;;;;;;;;;;;;;,,,,,''''''',,,,,,'''',,,,,,,,,,''''''',,,dKKKKKKKKK0000\ndk0000koc;'',;::;,',;;;,...',;;;;;;;;;;:;;;;;;;;;,,,,,,,,,,,,''''',,,,,,;,,,,,,'',,,,;xKKKKKKK000000\nloxkO00Okd:,,;:;,'';::;;;,,;;;;;;;;:;;::::::::;;;;;;;;;,,,;,,'',;;,,,,;;;;;,,,,,',,,,:xKKKK00000000O\ncldOKNNWWNk:,,;,''',;::;;;;;;;;;;;:::::::::::::;;;;;;;;;;;,,'',,,.....'',,,,''''',,,,ckK0000000000OO\n:okKNWWMMMNOc,,,''''',;:::;;;;;;;;;;::::::::::::;;;;;;,,,,,,,''''....'',,,'..'''',,,,oO00000OOOOOOOO\n:oOXNWWWMMMWKd:,,,,;;;;;:::;;;;;;;;;;;;:::::::::;;;;,,,,,,,,,,'''''''''''''..''''',,:d0OOOOOOOOOOOkk\n:o0XNWWWMMMMWN0d:;;;::;;;:::;;;;;;;;;;;;;;::::::;;,,,,,,,''',''''''',''''''...'''',,ckOOOOOOOkkkkkkk\ncd0XNNNNNNNWWWWNKxl::::;,;:::;;;;;;;;;;;;;;::::;;;,,,,,,,'''''''''','''''''..''''',,lOOkkkkkkkkkkkkx\ncx0XXX0OOO0KKKXXNX0xl:::::cc::;;;;;;;;;;;;;;:;;;;;;;,,,,,,,,,,,,,,,,,,',,''''''''',;okkkxxxxkkkkxxxx\ncokOOkxdxkO00000KKKKkc',:cccc::;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,''''''''''.'''',,ckK0Oxdoddddddddd\n:cllllooxkOO000000000xc;:c:ccc::;;;;;;;;;;;;;;;;;;;;,,''''....................'',,;xNMWWKxlccccccccl\n::::ccloxkOO0000000000kocc::cc:::::;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,''''''''''',,lKMMMMWKdlcccccccc\n;::::clodxkkOOOOOOOOOOOkolc::cc::::::;;;;;;;;;;;;;;,,,,,,,;;;;;;;;,,,'''''',,,'',c0WMMMMMXkooooooooo\n;:;;:cclodxkkkkOkOOOOOkkxdc:::ccc::::::::;;::;;;;,,,,,,,,;;;;;;;,,,,''''''',,,,,cOWMMMMMMNOdoooooooo\n:c::::clodxxkkkkOOOOOkkkkxl::::::::::::::;::;;;;;;;;;,,,,;;;;;;,,,,'''''''',,,,c0WMMMMMMMNOolllooool\noddlcloddxkkOOOOOOOOOOOkOkl::;:::::::::;;;::;;;;;;;;,,,;;;;,,,,,,,'''''''''',,lKWMMMMMMMMNx:;:ccllll\ndkkxxOKK0O0000000KKKKKKKKkoc:;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,'''',,,lKMMMMMMMMMMXd;,,,,,;;;\n:oodkKXX0O000OOOO00KKKKKKOoc::;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,c0WMMMMMMMMMMXd;,'......\n..';okOxooddollldkO000000koc:;;;;;;;;;;;;,,;;;,,,,,,,,,;,,,,,,,,,,,,'',,,,':OWMMMMMMMMMMMXo,........\n ...:ll:;clc;.',:oxO0KKKKOdc:;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,''',,,',cOWMMMMMMMMMMMMKl'........\n ...',,''clc,...',cdk0XXXKxc:;;;;;;;;;;;;;;;;;,,,,,,,,'''''''''','''','',oKWMMMMMMMMMMMMMKl'........\n";

    if (opt == 1){
        for (int i = 0; i < (int)((strlen(message_bienvenue)+9)/10); i++){
            int nmb = strlen(message_bienvenue) < (unsigned long)((i+1)*10) ? (int)(strlen(message_bienvenue)%10)+1 : 10;
            write(socket_client,substring(message_bienvenue,(i)*10,nmb),10);
            sleep(1);
        }
    }else{
        write(socket_client, message_bienvenue, strlen(message_bienvenue));
    }
    kill(frk,9);

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

void traitement_signal(int sig){
    int status;
    while(waitpid(-1/*getpid()*/,&status,WNOHANG)>0){
        if(WIFEXITED(status)){printf("terminé par signal %d\n", status);}
        if(WIFSIGNALED(status)){printf("terminé par signal %d\n", status);}
        if(WTERMSIG(status)){printf("terminé par signal %d\n", status);}
        if(WEXITSTATUS(status)){printf("terminé par signal %d\n", status);}
    }
    printf("Signal %d reçu\n", sig);
}

/*####SOCKET INFOS####
https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
*/