/*
 * AppUDPSender.cpp
 *
 *  Created on: 2019. 1. 11.
 *      Author: fineg
 */
//=================================
//=Variablen=======================
//=================================
//#define _WIN32_WINNT  0x501

//#ifndef WINVER
//#define WINVER WindowsXP
//#endif
//#ifndef WINVER
////#define WINVER _WIN32_WINNT
//#define WINVER WindowsXP
//#endif
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <sys/types.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <unistd.h>

//#pragma comment(lib, "ws2_32.lib")


#ifndef PACKETSIZE
#define PACKETSIZE 1024
#endif

#ifndef TABLE_LENGTH
#define TABLE_LENGTH 1024*1024
#endif

unsigned short myID = 0;
char *myIP= NULL;
char *myPORT = NULL;
char *nodeIP = NULL;
char *nodePORT = NULL;

int sock = 0;
struct sockaddr_in preAddr;
unsigned short preID = 0;
int preknown = 1;
struct sockaddr_in succAddr;
unsigned short succID = 0;

char packet[PACKETSIZE];
struct sockaddr_storage sender;
socklen_t sendersize = sizeof sender;
int tablefunctionstarted = 0;

char INTERN_MASK = 0b10000000;
char STABILIZE_MASK = 0b00000100;
char NOTIFY_MASK = 0b00000010;
char JOIN_MASK = 0b00000001;
char SNJ_MASK = 0b00000111;

//=================================
//=Functions=======================
//=================================


void setSocket();
int checkarguments(int argc, char **argv);
int letsgo();
void printMSG(char *msg, int msglength, int plain);
void handleintern(unsigned long msglength);
void handleoutside(unsigned long msglength);
void sendjoin(unsigned int ipaddress,unsigned short port, unsigned short preferredID);
void sendnotify(unsigned int ipaddress,unsigned short port, unsigned short preferredID);
void sendstabilize();
int inet_pton(int af, const char *src, void *dst);


int main(int argc, char **argv){
    printf("Starting server...\n");
    int einklinken = checkarguments(argc, argv);
    setSocket();
    if(einklinken) if(letsgo()){ perror("Problem while joining DHT network!\n"); exit(2);};

    /*
    //Hauptpacketannahmeverarbeitungsschleife
    long check = 0;
    while(1){
        printf("Server is waiting for packets... \n");
        if(-1 == (check = recvfrom(sock, packet, PACKETSIZE, 0, (struct sockaddr *)&sender, &sendersize))){
            perror("Error with new packet!\n");
            exit(2);
        }
        printf("Found packet... \n");
        //printMSG(packet, check, 1);

        if(0 != (packet[0] & INTERN_MASK)) handleintern(check);
        else handleoutside(check);
    }
    */
}


int checkarguments(int argc, char **argv){
    if(argc < 3){ fprintf( stderr,"Too few arguments! \nUsage: %s myip myport\nOptions:\n-id preferredID  (Your preferred id in the DHT network.)\n-new ip port    (IP address and port of a DHT network node.)", argv[0]); exit(1);}
    if(argc > 8){ fprintf( stderr,"Too many arguments! \nUsage: %s myip myport\nOptions:\n-id preferredID  (Your preferred id in the DHT network.)\n-new ip port    (IP address and port of a DHT network node.)", argv[0]); exit(1);}

    /* printf("Arguments:\n");
    for(int i = 0; i < argc; ++i){
        printf("%s\n", argv[i]);
    } */
    printf("Parsing arguments...\n");
    myIP = argv[1];
    myPORT = argv[2];
    printf("My ip: %s\n", myIP);
    printf("My port: %s\n", myPORT);

    int newNode = 0;
    int count = 3;
    while(argc > count){
        if(argv[count][0]!='-'){fprintf( stderr,"Wrong usage! \nUsage: %s myip myport\nOptions:\n-id preferredID  (Your preferred id in the DHT network.)\n-new ip port    (IP address and port of a DHT network node.)", argv[0]); exit(1);}
        if(argv[count][1]=='i'){
            ++count;
            errno = 0;
            long value = 0;
            value = strtol(argv[count++], NULL, 10);
            switch(errno){
                case ERANGE:
                    printf("The ID could not be represented.\n");
                    exit(1);
                case EINVAL:
                    printf("Unsupported base / radix.\n");
                    exit(1);
            }
            if(value > 65536){ perror("Your desired ID is over the max value of 65536!\n"); exit(1);}
            myID = (short) value;
            printf("My id: %u\n", myID);
        }
        else if(argv[count][1]=='n'){
            ++count;
            nodeIP = argv[count++];
            nodePORT = argv[count++];
            printf("The nodes ip: %s\n", nodeIP);
            printf("The nodes port: %s\n", nodePORT);
            newNode = 1;
        }
    }

    printf("Finished parsing arguments.\n");
    return newNode;
}


void setSocket(){

    printf("Configuring the socket...\n");
    struct addrinfo hints, *locations, *p;
//    SOCKADDR_IN hints, *locations, *p;
    int yes=1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int rv = getaddrinfo(NULL, myPORT, &hints, &locations);

    if(rv != 0){
        fprintf(stderr, "getaddrinfo threw an error! Maybe your port is used?\n");
        exit(1);
    }

    for(p = locations; p!=NULL; p=p->ai_next){

        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sock == -1){perror("SocketError");continue;}

        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1){perror("Can't clean socket!\n"); exit(1);}
//        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){perror("Can't clean socket!\n"); exit(1);}

        if(bind(sock, p->ai_addr, p->ai_addrlen) == -1){perror("BindError");close(sock);continue;}

        break;
    }

    if(p==NULL){
        perror("Failed to bind!\n");
        exit(2);
    }

    int check = 0;
    if(-1 == (check = inet_pton(AF_INET, myIP, &(preAddr.sin_addr)))){ perror("Wrong ip format!\n"); exit(1);}
    if(-1 == (check = inet_pton(AF_INET, myIP, &(succAddr.sin_addr)))){ perror("Wrong ip format!\n"); exit(1);}
    memcpy(&(succAddr.sin_port), &(((struct sockaddr_in *)p->ai_addr)->sin_port), 2);
    memcpy(&(preAddr.sin_port), &(((struct sockaddr_in *)p->ai_addr)->sin_port), 2);
    memset(preAddr.sin_zero, 0, 8);
    memset(succAddr.sin_zero, 0, 8);
    preID = myID;
    succID = myID;

    printf("Finished configuring the socket.\n");

    freeaddrinfo(locations);
}


int letsgo(){
    printf("Trying to join an existent DHT network...\n");
    preknown = 0;


    int check = 0;
    succAddr.sin_family = AF_INET;

//    inet_pton((int)AF_INET, (const char*)nodeIP, (void*)(&succAddr.sin_addr));
//    inet_pton((int)AF_INET, (const char*)nodeIP, NULL);

    if(-1 == (check = inet_pton(AF_INET, (const char*)nodeIP, (void*)&(succAddr.sin_addr)))){ perror("Wrong ip format!\n"); exit(1);}
//    if(-1 == (check = inet_pton(AF_INET, nodeIP, &(succAddr.sin_addr)))){ perror("Wrong ip format!\n"); exit(1);}
    errno = 0;
    long value = 0;
    value = strtol(nodePORT, NULL, 10);
    switch(errno){
        case ERANGE:
            printf("The node port could not be represented.\n");
            exit(1);
        case EINVAL:
            printf("Unsupported base / radix.\n");
            exit(1);
    }
    if(value > 65536){ perror("Your node port is over the max value of 65536!\n"); exit(1);}
    succAddr.sin_port = htons((short) value);

    unsigned int ipaddress = preAddr.sin_addr.s_addr;
    unsigned short port = preAddr.sin_port;
    unsigned short preferredID = 0;

    sendjoin(ipaddress, port, myID);

    while(1){
        printf("Wait for notify answer... \n");
        if(-1 == (check = recvfrom(sock, packet, PACKETSIZE, 0, (struct sockaddr *)&sender, &sendersize))){
            perror("Error with new packet!\n");
            exit(2);
        }
        printf("Got answer... \n");

        if(0 == (packet[0] & INTERN_MASK)){ printf("Can't understand the packet.\n"); continue;}
        if(0 == ((packet[0] & NOTIFY_MASK) || check < 8)){ printf("Not a notify packet.\n"); continue;}

        memcpy(&preferredID, &packet[7], 2);
        preferredID = ntohs(preferredID);
        if(preferredID > TABLE_LENGTH){ perror("Preferred id is too big!\n"); continue;}
        memcpy(&ipaddress, &packet[1], 4);
        memcpy(&port, &packet[5], 2);

        succAddr.sin_addr.s_addr = ipaddress;
        succAddr.sin_port = port;
        succID = preferredID;
        break;
    }


    return 0;
}


void sendjoin(unsigned int ipaddress, unsigned short port, unsigned short preferredID){
    printf("Sending join-request...\n");
    char msg[9];
    memset(&msg, 0, 9);
    msg[0] = JOIN_MASK | INTERN_MASK;
//    ipaddress = ipaddress;
//    port = port;
    preferredID = htons(preferredID);
    memcpy(&msg[1], &ipaddress, 4);
    memcpy(&msg[5], &port, 2);
    memcpy(&msg[7], &preferredID, 2);

    errno = 0;

    //printMSG((void *)&succAddr, sizeof(succAddr), 0);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    if(0 > sendto(sock, msg, 9, 0, (struct sockaddr *)&succAddr, addrlen)){
        perror("Error while sending a packet.\n");
        switch(errno){
            case EFAULT:
                printf("Invalid user space address.\n");
                break;
            case EAGAIN:
                printf("Something with blocking.\n");
                break;
            case EBADF:
                printf("Invalid descriptor.\n");
                break;
            case EINVAL:
                printf("Invalid argument.\n");
                break;
            case EDESTADDRREQ:
                printf("No Peer address.\n");
                break;
            case EISCONN:
                printf("Connection mode socket.\n");
                break;
            case ENOTSOCK:
                printf("The given socket is not a socket.\n");
                break;
            case ENOTCONN:
                printf("No Target.\n");
                break;
            case ENOBUFS:
                printf("Network output is full.\n");
                break;
            default:
                printf("No spezific error.\n");
        }
        exit(1);
    }
    printf("Send to: %lu %d\n", ntohl(succAddr.sin_addr.s_addr), ntohs(succAddr.sin_port));
}



int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
	  switch(af) {
	  case AF_INET:
		  *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
		  return 1;
	  case AF_INET6:
		  *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
		  return 1;
	  }
  }
  return 0;
}
