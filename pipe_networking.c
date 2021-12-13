#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo(WKP, 0777);

  int from_client = open(WKP, O_RDONLY, 0777);
  char pidName[HANDSHAKE_BUFFER_SIZE];
  read(from_client, pidName, HANDSHAKE_BUFFER_SIZE);
  printf("server got: %s\n", pidName);
  remove(WKP);

  *to_client = open(pidName, O_WRONLY, 0777);
  write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);

  read(from_client, pidName, HANDSHAKE_BUFFER_SIZE);
  printf("server got: %s\n", pidName);

  printf("from_client file indice: %d\n", from_client);

  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char pidName[BUFFER_SIZE];
  sprintf(pidName, "%d", getpid());
  mkfifo(pidName, 0777);

  *to_server = open(WKP, O_WRONLY, 0777);
  write(*to_server, pidName, HANDSHAKE_BUFFER_SIZE);

  char message[HANDSHAKE_BUFFER_SIZE];
  int from_server = open(pidName, O_RDONLY, 0777);
  read(from_server, message, HANDSHAKE_BUFFER_SIZE);
  printf("client got: %s\n", message);
  printf("client pid %d \n", getpid());
  remove(pidName);

  write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);

  printf("from_server file indice: %d\n", from_server);
  
  return from_server;
}
