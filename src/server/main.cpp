#include <bits/stdc++.h>

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

using namespace std;

typedef boost::system::system_error boost_error;
using namespace boost::asio;
using namespace boost::asio::ip;

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

  } catch (boost_error err) {
    cerr << err.what() << endl;
  }

  return 0;
}
