#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "game.h"
#include "utils.h"

typedef struct sockaddr_in SockAddr, *PSockAddr;

int init_connection_from_arguments(PCommandLineArguments arguments,
                                   PSockAddr server);

int main(int argc, char** argv)
{
  int client_socket;
  CommandLineArguments arguments;
  SockAddr server;

  if (!parse_command_line_arguments(argc, argv, &arguments))
  {
    return 1;
  }

  client_socket = init_connection_from_arguments(&arguments, &server);
  if (client_socket < 0)
  {
    return -1;
  }

  printf("You successfully connected to the server.\n");

  char name[50];
  printf("Type your name: ");
  scanf("%s", name);

  send(client_socket, name, sizeof(name), 0);
  printf("Welcome, %s!\n", name);

  while (1)
  {
    char message[140];
    printf("%s: ", name);
    scanf("%s", message);
    send(client_socket, message, sizeof(message), 0);

    if (strcmp(message, "/quit") == 0)
    {
      break;
    }
  }

  printf("You quitted.\n");

  close(client_socket);

  return 0;
}

int init_connection_from_arguments(PCommandLineArguments arguments,
                                   PSockAddr server)
{
  int client_socket;

  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (client_socket < 0)
  {
    fprintf(stderr, "Error: the client socket could not be established.\n");
    return -1;
  }

  memset(server, 0, sizeof(*server));
  server->sin_port = htons(arguments->port);
  server->sin_family = AF_INET;
  server->sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(client_socket, (struct sockaddr*)server, sizeof(*server)) < 0)
  {
    fprintf(stderr, "Error: the client could not connect to the server.\n");
    return -1;
  }

  return client_socket;
}
