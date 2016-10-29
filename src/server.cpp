#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <deque>
#include <thread>

#include "game.h"
#include "utils.h"

using std::deque;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::thread;
using std::mutex;

int establish_server(int argc, char* argv[], SockAddrIn* server);
void accept_clients(int server_socket);
int accept_client(int server_socket);
void register_client(int client_socket);
void run_server();
void receive_from_client(int client_socket);
void send_to_client(int client_socket, const string& message);

deque<ClientInfo*> client_queue;
mutex client_queue_mtx;
deque<string> message_queue;
mutex message_queue_mtx;

int main(int argc, char* argv[]) {
  SockAddrIn server;
  const int server_socket = establish_server(argc, argv, &server);

  cout << "Server was successfully established." << endl;

  thread accepter(accept_clients, server_socket);
  thread runner(run_server);

  runner.join();
  accepter.join();

  close(server_socket);

  return 0;
}

int establish_server(int argc, char* argv[], SockAddrIn* server) {
  const int port = atoi(argv[1]);
  const int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0) {
    cerr << "Error: the server socket could not be established." << endl;
    exit(-1);
  }

  memset(server, 0, sizeof(*server));
  server->sin_port = htons(port);
  server->sin_family = AF_INET;
  server->sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (SockAddr*) server, sizeof(*server)) < 0) {
    cerr << "Error: a name could not be bound to the server socket." << endl;
    exit(-2);
  }

  return server_socket;
}

void accept_clients(int server_socket) {
  listen(server_socket, 10);

  while (true) {
    const int client_socket = accept_client(server_socket);
    if (client_socket > 0) {
      register_client(client_socket);
    }
  }
}

int accept_client(int server_socket) {
  SockAddr client;

  socklen_t socket_len = sizeof(client);
  memset(&client, 0, sizeof(client));
  const int client_socket = accept(server_socket, &client, &socket_len);

  if (client_socket < 0) {
    cerr << "Error: server could not accept a client." << endl;
    return -1;
  }

  cerr << "New client accepted (client socket:" << client_socket << ")." << endl;

  return client_socket;
}

void register_client(int client_socket) {
  ClientInfo* client = new ClientInfo(client_socket);

  client->set_username("john_doe");

  client_queue_mtx.lock();
  client_queue.push_back(client);
  client_queue_mtx.unlock();

  client->set_receiver(receive_from_client, client_socket);
}

void run_server() {
  while (true) {
    string message = "";

    message_queue_mtx.lock();
    if (!message_queue.empty()) {
      message = message_queue.front();
      message_queue.pop_front();

      cout << message << endl;
    }
    message_queue_mtx.unlock();

    if (!message.empty()) {
      client_queue_mtx.lock();
      for (ClientInfo* client : client_queue) {
        send_to_client(client->socket(), message);
      }
      client_queue_mtx.unlock();
    }
  }
}

void receive_from_client(int client_socket) {
  while (true) {
    char message[144];

    recv_str(client_socket, message);

    message_queue_mtx.lock();
    message_queue.push_back(string(message));
    message_queue_mtx.unlock();
  }
}

void send_to_client(int client_socket, const string& message) {
  send_str(client_socket, message.c_str());
}
