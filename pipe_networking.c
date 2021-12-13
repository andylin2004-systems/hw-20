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
  char message[HANDSHAKE_BUFFER_SIZE];
  read(from_client, message, HANDSHAKE_BUFFER_SIZE);
  printf("server got: %s\n", message);
  close(from_client);
  char pidName[BUFFER_SIZE];
  sprintf(pidName, "%d", getpid());
  *to_client = open(pidName, O_WRONLY, 0777);
  write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
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
  *to_server = open(WKP, O_WRONLY, 0777);
  write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
  int from_server = open(pidName, O_RDONLY, 0777);
  char message[HANDSHAKE_BUFFER_SIZE];
  read(from_server, message, HANDSHAKE_BUFFER_SIZE);
  printf("client got: %s\n", message);
  close(getpid());
  write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
  return from_server;
}
