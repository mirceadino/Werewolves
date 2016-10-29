#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "game.h"
#include "utils.h"

using namespace std;

int connect_to_server(int argc, char* argv[], SockAddrIn* server);
void send_to_server(int client_socket);
void receive_from_server(int client_socket);

string name;

int main(int argc, char* argv[]) {
  SockAddrIn server;
  const int client_socket = connect_to_server(argc, argv, &server);

  cout << "Connection to the server was established." << endl;

  cout << "Type your name: ";
  cin >> name;

  string full_message = name + " joined.";
  send_str(client_socket, full_message.c_str());

  thread sender(send_to_server, client_socket);
  thread receiver(receive_from_server, client_socket);

  sender.join();
  receiver.join();

  close(client_socket);

  return 0;
}

int connect_to_server(int argc, char* argv[], SockAddrIn* server) {
  const int port = atoi(argv[1]);
  const int client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (client_socket < 0) {
    cerr << "Error: the client socket could not be established." << endl;
    exit(-1);
  }

  memset(server, 0, sizeof(*server));
  server->sin_port = htons(port);
  server->sin_family = AF_INET;
  server->sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(client_socket, (SockAddr*)server, sizeof(*server)) < 0) {
    cerr << "Error: the client could not connect to the server." << endl;
    exit(-2);
  }

  return client_socket;
}

void send_to_server(int client_socket) {
  cin.ignore();

  while (true) {
    string message;
    getline(cin, message);

    if (message == "/quit") {
      string full_message = name + " quitted.";
      send_str(client_socket, full_message.c_str());
      break;
    }

    string full_message = name + ": " + message;
    send_str(client_socket, full_message.c_str());
  }
}

void receive_from_server(int client_socket) {
  while (true) {
    char message[144];

    recv_str(client_socket, message);

    if (string(message) == name + " quitted.") {
      break;
    }

    cout << message << endl;
  }
}
