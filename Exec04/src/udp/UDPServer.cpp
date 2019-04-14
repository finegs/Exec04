/*
 * UDPServer.cpp
 *
 *  Created on: 2018. 11. 2.
 *      Author: finegs
 */
//Server
#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>
#include <map>
#include <unordered_map>

using namespace std;

unordered_map<string, string> envMap;

//Prototypes
int startWinsock(void);

int main(int argc, char* argv[])
{
  long rc;
  SOCKET sock;
  int bindPort;
  int bufLen = 1024;
  char rcvBuf[bufLen];
  char sndBuf[bufLen];
//  char bufpermanent[bufLen] = "Hello";
  SOCKADDR_IN addr;
  SOCKADDR_IN remoteAddr;
  int         remoteAddrLen=sizeof(SOCKADDR_IN);

  if(argc != 2)
  {
	  printf("Usage : US [Bind Port No]\n");
	  return EXIT_FAILURE;
  }

  bindPort = atoi(argv[1]);

  rc=startWinsock();
  if(rc!=0)
  {
    printf("Error: startWinsock, error code: %ld\n",rc);
    return 1;
  }
  else
  {
    printf("Winsock started!\n");
  }

  //UDP Socket erstellen
  sock=socket(AF_INET,SOCK_DGRAM,0);

  if(sock == INVALID_SOCKET)
  {
    printf("Error: Socket could not be created, error code: %d\n",WSAGetLastError());
    return EXIT_FAILURE;
  }
  else
  {
    printf("UDP Socket created!\n");
  }

  addr.sin_family=AF_INET;
  addr.sin_port=htons(bindPort);
  addr.sin_addr.s_addr=ADDR_ANY;
  rc=bind(sock,(SOCKADDR*)&addr, remoteAddrLen);

  if(rc==SOCKET_ERROR)
  {
    printf("Error: bind, error code: %d\n",WSAGetLastError());
    return 1;
  }
  else
  {
    printf("Binding Socket at Port : %d\n", bindPort);
  }

  while(1)
  {
	memset(rcvBuf, '\0', bufLen); // init rcvBuf
	memset(sndBuf, '\0', bufLen); // init sndBuf

    rc=recvfrom(sock,rcvBuf, bufLen,0,(SOCKADDR*)&remoteAddr,&remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
      printf("Error: recvfrom, error code: %d\n",WSAGetLastError());
      return EXIT_FAILURE;
    }
    else
    {
      rcvBuf[rc] = '\0';
      printf("[R] : [%ld] : %s\n", rc, rcvBuf);
    }

    //Answer
    //sprintf(buf2,"Hello %s",buf);
    memcpy(sndBuf, rcvBuf, rc);
    rc=sendto(sock,sndBuf,strlen(sndBuf),0,(SOCKADDR*)&remoteAddr,remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
      printf("Error: sendto, error code: %d\n",WSAGetLastError());
      return EXIT_FAILURE;
    }
    else
    {
      printf("[S] : [%ld] : %s\n", rc, sndBuf);
    }
  }
  return 0;
}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,2),&wsa);
}



