/*
 * AppCodeTest.cpp
 *
 *  Created on: 2018. 12. 16.
 *      Author: finegs
 */
#include <iostream>
#include <string>


#if 1
#include <stdio.h>
#include <sys/types.h>
#include <winsock.h>
#include <unistd.h>

#define a1 172
#define a2 30
#define a3 1
#define a4 46
#define PORT 3000

void closeAll(WSADATA w, int sd) {

    if (sd != (int)INVALID_SOCKET)
    	closesocket(sd);
    if(0x00 != w.wHighVersion || 0x00 != w.wVersion)
    	WSACleanup();
    exit(0);
}

int main() {
	int rc;

    /* Open windows connection */
    WSADATA w;
    if (WSAStartup(0x0202, &w) != 0)
    {
        printf("Could not open Windows connection.\n");
        closeAll(w, 0);
    }

    /* Clear out server struct */
    SOCKADDR_IN server;
    memset((void *)&server, '\0', sizeof(struct sockaddr_in));

    /* Set family and port */
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)a1;
    server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)a2;
    server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)a3;
    server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)a4;

    /* Open a datagram socket */
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == (int)INVALID_SOCKET)
    {
        printf("Could not create socket.\n");
        closeAll(w, sd);
    }

    /* Bind address to socket */
    if (-1 == (rc = bind(sd, (struct sockaddr *)&server, sizeof(SOCKADDR_IN))))
    {
        printf("Could not bind name to socket. ec=%d\n", rc);
        closeAll(w, sd);
    }

    /* Receive */
    char data[1024];
    printf("Waiting to receive...\n");
    if (recv(sd, (char *)&data, (int)sizeof(data), 0))
    {
        printf("Error receiving data.\n");
        closeAll(w, sd);
    }

    printf("Data: %s", data);

    return 0;
}
#endif
