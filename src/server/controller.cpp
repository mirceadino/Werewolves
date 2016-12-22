#include "controller.h"

#include <cstring>

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

namespace server {

// ----------------------------------------------------------------------------
// ----- PUBLIC ---------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef boost::system::system_error boost_error;
using std::basic_string;
using std::string;
using std::vector;
using namespace boost::asio;
using namespace boost::asio::ip;

static const int kMaxDim = 256;

Controller::Controller() {
  socket_ = std::make_unique<tcp::socket>(io_service_);
}

void Controller::OpenConnection(const basic_string<char>& host, int port) {
  tcp::endpoint end_point(address::from_string(host), port);
  socket_->connect(end_point);
}

void Controller::SendMessage(const basic_string<char>& message) {
  char buffer[kMaxDim];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, message.c_str());
  socket_->write_some(boost::asio::buffer(buffer, kMaxDim));
}

bool Controller::HasMessage() {
  return socket_->available();
}

string Controller::ReceiveMessage() {
  char buffer[kMaxDim];
  socket_->read_some(boost::asio::buffer(buffer, kMaxDim));
  return Trim(string(buffer, kMaxDim));
}

vector<string> Controller::RetrieveNewMessages() {
  vector<string> messages;
  while (HasMessage()) {
    messages.push_back(ReceiveMessage());
  }
  return messages;
}

void Controller::CloseConnection() {
  socket_->close();
}

// ----------------------------------------------------------------------------
// ----- PRIVATE --------------------------------------------------------------
// ----------------------------------------------------------------------------

string Controller::Trim(string message) {
  boost::trim(message);
  while (!message.empty() && message.back() == 0) {
    message.pop_back();
  }
  boost::trim(message);
  return message;
}

}
