#include <string>
#include <thread>
#include <utility>

typedef struct sockaddr_in SockAddrIn;
typedef struct sockaddr SockAddr;

class ClientInfo {
  public:
    ClientInfo(int client_socket):
      client_socket_(client_socket) {}

    ~ClientInfo() {
      if (receiver_.joinable()) {
        receiver_.join();
      }
    }

    const int socket() {
      return client_socket_;
    }

    void set_username(const std::string& username) {
      username_ = username;
    }

    const std::string& username() {
      return username_;
    }

    template<class Fn, class... Args>
    void set_receiver(Fn&& fn, Args&& ... args) {
      receiver_ = std::thread(fn, std::forward<Args>(args)...);
    }

    std::thread& receiver() {
      return receiver_;
    }

  private:
    const int client_socket_;
    std::string username_;
    std::thread receiver_;
};

int recv_str(int socket, char* str);
void send_str(int socket, const char* str);
