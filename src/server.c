#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "game.h"
#include "utils.h"

typedef struct sockaddr_in SockAddr, *PSockAddr;

int init_server_from_arguments(PCommandLineArguments arguments,
                               PSockAddr server);
void* serve_client(void* arguments);

typedef struct ServeClientArguments
{
  int client_socket;
  socklen_t client_size;
  SockAddr client;
} ServeClientArguments;

int main(int argc, char** argv)
{
  int server_socket;
  CommandLineArguments arguments;
  SockAddr server;

  if (!parse_command_line_arguments(argc, argv, &arguments))
  {
    return 1;
  }

  server_socket = init_server_from_arguments(&arguments, &server);
  if (server_socket < 0)
  {
    return -1;
  }

  printf("Server was successfully established.\n");

  listen(server_socket, 10);

  while (1)
  {
    pthread_t thread_id;
    ServeClientArguments arguments;

    arguments.client_size = sizeof(arguments.client);
    memset(&arguments.client, 0, sizeof(arguments.client));
    arguments.client_socket = accept(server_socket,
                                     (struct sockaddr*)&arguments.client,
                                     &arguments.client_size);

    if (arguments.client_socket < 0)
    {
      fprintf(stderr, "Warning: server could not accept a client.\n");
      continue;
    }

    pthread_create(&thread_id, NULL, serve_client, (void*)&arguments);
  }

  return 0;
}

int init_server_from_arguments(PCommandLineArguments arguments,
                               PSockAddr server)
{
  int server_socket;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0)
  {
    fprintf(stderr, "Error: the server socket could not be established.\n");
    return -1;
  }

  memset(server, 0, sizeof(*server));
  server->sin_port = htons(arguments->port);
  server->sin_family = AF_INET;
  server->sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr*)server, sizeof(*server)) < 0)
  {
    fprintf(stderr, "Error: a name could not be bound to the server socket.\n");
    return -2;
  }

  return server_socket;
}

void* serve_client(void* _arguments)
{
  ServeClientArguments arguments = *(ServeClientArguments*)_arguments;
  int client_socket = arguments.client_socket;

  char name[50];
  recv(client_socket, name, sizeof(name), MSG_WAITALL);
  printf("%s connected.\n", name);

  while (1)
  {
    char message[140];
    recv(client_socket, message, sizeof(message), MSG_WAITALL);

    if (strcmp(message, "/quit") == 0)
    {
      break;
    }

    printf("%s: %s\n", name, message);
  }

  printf("%s quitted.\n", name);

  return NULL;
}
