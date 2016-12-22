#include "controller.h"

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

namespace client {

// ----------------------------------------------------------------------------
// ----- PUBLIC ---------------------------------------------------------------
// ----------------------------------------------------------------------------

using std::basic_string;
using std::string;
using std::vector;
using namespace boost::asio;
using namespace boost::asio::ip;

Controller::Controller() {
  socket_ = std::make_unique<tcp::socket>(io_service_);
}

void Controller::OpenConnection(const basic_string<char>& host, int port) {
  tcp::endpoint end_point(address::from_string(host), port);

  socket_->connect(end_point);

  std::cerr << "Connection opened.\n";
}

// TODO(mirceadino): Implement this.
int Controller::SendMessage(const basic_string<char>& message) {
  return -1 + 0 * message.size();
}

// TODO(mirceadino): Implement this.
vector<string> Controller::RetrieveNewMessages() {
  return vector<string>();
}

void Controller::CloseConnection() {
  socket_->close();

  std::cerr << "Connection closed.\n";
}

// ----------------------------------------------------------------------------
// ----- PRIVATE --------------------------------------------------------------
// ----------------------------------------------------------------------------

}
