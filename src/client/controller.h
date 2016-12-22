#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>

namespace client {

class Controller {
  public:
    Controller();

    // Opens connection to the given host and on the given port.
    // Throws boost::system::system_error in case of failure.
    void OpenConnection(const std::basic_string<char>& host, int port);

    // Sends a message to the server.
    // Throws boost::system::system_error in case of failure.
    void SendMessage(const std::basic_string<char>& message);

    // Retrieves the new messages that the client received.
    // Throws boost::system::system_error in case of failure.
    std::vector<std::string> RetrieveNewMessages();

    // Closes the connection.
    // Throws boost::system::system_error in case of failure.
    void CloseConnection();

  private:
    // Checks whether or not there are new incoming messages.
    // Throws boost::system::system_error in case of failure.
    bool HasMessage();

    // Receives a message from the server.
    // Throws boost::system::system_error in case of failure.
    std::string ReceiveMessage();

    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
};

}
