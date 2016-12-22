// This is a running scenario (for the moment).
#include <bits/stdc++.h>

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

using namespace std;

typedef boost::system::system_error boost_error;
using namespace boost::asio;
using namespace boost::asio::ip;

static const int kMaxDim = 256;

string receive(tcp::socket& socket) {
  char buffer[kMaxDim];
  socket.read_some(boost::asio::buffer(buffer, kMaxDim));
  return string(buffer, kMaxDim);
}

void send(tcp::socket& socket, const basic_string<char>& message) {
  char buffer[kMaxDim];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, message.c_str());
  socket.write_some(boost::asio::buffer(buffer, kMaxDim));
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);

  try {
    io_service service;
    tcp::endpoint end_point(tcp::v4(), port);
    tcp::socket socket(service);

    tcp::acceptor acceptor(service, end_point);
    acceptor.accept(socket);

    cout << receive(socket) << endl;
    cout << receive(socket) << endl;

    send(socket, "ana has apples");
    send(socket, "bob has apples");
    send(socket, "carl has apples");
    send(socket, "diana has apples");

    cout << receive(socket) << endl;

  } catch (boost_error err) {
    cerr << err.what() << endl;
  }

  return 0;
}
