#include "client_handler.h"

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

ClientHandler::ClientHandler() {
  socket_ = std::make_unique<tcp::socket>(io_service_);
}

void ClientHandler::AcceptConnection(int port) {
  tcp::endpoint end_point(tcp::v4(), port);
  tcp::acceptor acceptor(io_service_, end_point);
  acceptor.accept(*socket_);
}

void ClientHandler::AskUsername() {
  while (true) {
    username_ = ReceiveMessage();

    if (UsernameIsValid(username_)) {
      break;
    }

    SendMessage("CODE_INVALID");
  }

  SendMessage("CODE_OK");
}

void ClientHandler::SendMessage(const basic_string<char>& message) {
  char buffer[kMaxDim];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, message.c_str());
  socket_->write_some(boost::asio::buffer(buffer, kMaxDim));
}

bool ClientHandler::HasMessage() {
  return socket_->available();
}

string ClientHandler::ReceiveMessage() {
  char buffer[kMaxDim];
  socket_->read_some(boost::asio::buffer(buffer, kMaxDim));
  return Trim(string(buffer, kMaxDim));
}

void ClientHandler::CloseConnection() {
  socket_->close();
}

// ----------------------------------------------------------------------------
// ----- PRIVATE --------------------------------------------------------------
// ----------------------------------------------------------------------------

string ClientHandler::Trim(string message) {
  boost::trim(message);
  while (!message.empty() && message.back() == 0) {
    message.pop_back();
  }
  boost::trim(message);
  return message;
}

bool ClientHandler::UsernameIsValid(const string& username) {
  return username.size() > 3;
}

}
