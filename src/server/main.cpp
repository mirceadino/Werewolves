#include <bits/stdc++.h>

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

#include "controller.h"

using namespace std;

typedef boost::system::system_error boost_error;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace server;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);

  while (true) {
    // Continuously accept clients (one client at the time).

    try {
      // Try accepting a client.
      Controller controller;
      controller.AcceptConnection(port);
      controller.AskUsername();

      cout << controller.username() << " connected." << endl;

      try {
        // Continuously receives messages from the client and send them back.
        while (true) {
          string message = controller.ReceiveMessage();
          cout << message << endl;
          controller.SendMessage(message);
        }

      } catch (boost_error err) {
        // If there was a failure during sending or receiving messages, close
        // the connection.
        controller.CloseConnection();
        cout << controller.username() << " disconnected." << endl;
      }

    } catch (boost_error err) {
      // If client could not be accepted, there was a failure during asking for
      // username, or a failure at closing the connection, display the error.
      cerr << err.what() << endl;
    }
  }

  return 0;
}
