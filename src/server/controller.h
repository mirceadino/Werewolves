#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>

namespace server {

class Controller {
  public:
    Controller();

    // Accept a connection on the given port.
    // Throws boost::system::system_error in case of failure.
    void AcceptConnection(int port);

    void AskUsername();

    // Sends a message to the server.
    // Throws boost::system::system_error in case of failure.
    void SendMessage(const std::basic_string<char>& message);

    // Checks whether or not there are new incoming messages.
    // Throws boost::system::system_error in case of failure.
    bool HasMessage();

    // Receives a message from the server.
    // Throws boost::system::system_error in case of failure.
    std::string ReceiveMessage();

    // Closes the connection.
    // Throws boost::system::system_error in case of failure.
    void CloseConnection();

  private:
    std::string Trim(std::string message);

    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    std::string username_;
};

}

#endif
