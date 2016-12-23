#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>

namespace server {

class ClientHandler {
  public:
    ClientHandler();

    // Accept a connection on the given port.
    // Throws boost::system::system_error in case of failure.
    void AcceptConnection(int port);

    // Asks the client for an username. Sends to the client a CODE_OK if the
    // username is valid. Otherwise, it sends CODE_INVALID and reasks for an
    // username.
    // Throws boost::system::system_error in case of failure.
    void AskUsername();

    // Sends a message to the connected client.
    // Throws boost::system::system_error in case of failure.
    void SendMessage(const std::basic_string<char>& message);

    // Checks whether or not there are new incoming messages.
    // Throws boost::system::system_error in case of failure.
    bool HasMessage();

    // Receives a message from the connected client.
    // Throws boost::system::system_error in case of failure.
    std::string ReceiveMessage();

    // Closes the connection.
    // Throws boost::system::system_error in case of failure.
    void CloseConnection();

    const std::string& username() const {
      return username_;
    }

    std::string& mutable_username() {
      return username_;
    }

  private:
    std::string Trim(std::string message);

    bool UsernameIsValid(const std::string& username);

    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    std::string username_;
};

}

#endif
