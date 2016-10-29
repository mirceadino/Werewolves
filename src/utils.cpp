#include "utils.h"

#include <stdio.h>
#include <sys/socket.h>

int recv_str(int socket, char* str) {
  int len;
  recv(socket, &len, sizeof(int), MSG_WAITALL);

  str[len] = 0;
  for (int i = 0; i < len; ++i) {
    recv(socket, &str[i], sizeof(char), MSG_WAITALL);
  }

  return len;
}

void send_str(int socket, const char* str) {
  int len = strlen(str);
  send(socket, &len, sizeof(int), MSG_WAITALL);

  for (int i = 0; i < len; ++i) {
    send(socket, &str[i], sizeof(char), MSG_WAITALL);
  }
}
