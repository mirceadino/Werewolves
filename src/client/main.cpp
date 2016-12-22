// This is a running scenario (for the moment).
#include <bits/stdc++.h>

#include "controller.h"

#include <boost/system/system_error.hpp>

using namespace std;

typedef boost::system::system_error boost_error;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const char host[] = "127.0.0.1";
  const int port = atoi(argv[1]);

  try {
    client::Controller ctrl;

    ctrl.OpenConnection(host, port);

    ctrl.SendMessage("hello");
    ctrl.SendMessage("this is a message");

    sleep(1);

    vector<string> messages = ctrl.RetrieveNewMessages();
    for (string message : messages) {
      cout << message << endl;
    }

    ctrl.SendMessage("exit");

    ctrl.CloseConnection();

  } catch (boost_error err) {
    cerr << err.what() << endl;
  }

  return 0;
}
