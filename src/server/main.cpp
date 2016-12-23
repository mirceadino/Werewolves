#include <bits/stdc++.h>

#include "controller.h"
#include "client_handler.h"

using namespace server;
using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);

  Controller controller;
  while (true) {
    ClientHandler* handler = controller.AcceptNewClient(port);
    controller.ServeClient(handler);
  }

  return 0;
}
