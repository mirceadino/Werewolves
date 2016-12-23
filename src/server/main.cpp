// This is a running scenario (for the moment).
#include <bits/stdc++.h>

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

#include "controller.h"

using namespace std;

typedef boost::system::system_error boost_error;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace server;

static const int kMaxDim = 256;

bool UsernameIsValid(const string& username) {
  return true | !username.empty();
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);

  for (;;) {
    try {
      Controller controller;
      controller.AcceptConnection(port);
      controller.AskUsername();

      while (true) {
        string message = controller.ReceiveMessage();
        cout << message << endl;
        controller.SendMessage(message);
      }

      controller.CloseConnection();

    } catch (boost_error err) {
      cerr << err.what() << endl;
    }
  }

  return 0;
}
