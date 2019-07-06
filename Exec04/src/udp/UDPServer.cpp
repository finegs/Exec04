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

int main()
{
  long rc;
  SOCKET s;
  char buf[256];
  char buf2[300];
  char bufpermanent[100] = "Hello";
  SOCKADDR_IN addr;
  SOCKADDR_IN remoteAddr;
  int         remoteAddrLen=sizeof(SOCKADDR_IN);

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
  s=socket(AF_INET,SOCK_DGRAM,0);

  if(s==INVALID_SOCKET)
  {
    printf("Error: Socket could not be created, error code: %d\n",WSAGetLastError());
    return 1;
  }
  else
  {
    printf("UDP Socket created!\n");
  }

  addr.sin_family=AF_INET;
  addr.sin_port=htons(1234);
  addr.sin_addr.s_addr=ADDR_ANY;
  rc=bind(s,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));

  if(rc==SOCKET_ERROR)
  {
    printf("Error: bind, error code: %d\n",WSAGetLastError());
    return 1;
  }
  else
  {
    printf("Socket at Port 1234\n");
  }

  while(1)
  {
    rc=recvfrom(s,buf,256,0,(SOCKADDR*)&remoteAddr,&remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
      printf("Error: recvfrom, error code: %d\n",WSAGetLastError());
      return 1;
    }
    else
    {
      printf("%d Bytes received!\n", rc);
      buf[rc]='\0';
    }
    printf("Received data: %s\n",buf);

    //Answer
    //sprintf(buf2,"Hello %s",buf);
    rc=sendto(s,buf2,strlen(buf2),0,(SOCKADDR*)&remoteAddr,remoteAddrLen);
    if(rc==SOCKET_ERROR)
    {
      printf("Error: sendto, error code: %d\n",WSAGetLastError());
      return 1;
    }
    else
    {
      printf("%ld Bytes send!\n", rc);
    }
  }
  return 0;
}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,2),&wsa);
}



