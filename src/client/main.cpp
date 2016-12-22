#include <iostream>

#include "controller.h"
#include "textview.h"

using namespace std;
using namespace client;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port> [<host>]" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);
  const string host = (argc == 3) ? string(argv[2]) : "127.0.0.1";

  TextView textView(new Controller());
  textView.Init(host, port);
  textView.RunMain();

  return 0;
}
