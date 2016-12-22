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

    int SendMessage(const std::basic_string<char>& message);

    std::vector<std::string> RetrieveNewMessages();

    // Closes the connection.
    // Throws boost::system::system_error in case of failure.
    void CloseConnection();

  private:
    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
};

}
