#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/thread.hpp>

#include "client_handler.h"

namespace server {

class Message {
  public:
    typedef enum Type {
      CONNECTION,
      DISCONNECTION,
      NARATIVE,
      NORMAL,
      WHISPER,
      ADMIN
    } Type;

    Message(const std::basic_string<char>& sender,
            const std::basic_string<char>& message,
            Type type):
      sender_(sender),
      message_(message),
      type_(type) {}

    const std::string& sender() const {
      return sender_;
    }

    const std::string& message() const {
      return message_;
    }

    const Type& type() const {
      return type_;
    }

  private:
    std::string sender_;
    std::string message_;
    Type type_;
};

class Controller {
  public:
    Controller();

    ClientHandler* AcceptNewClient(int port);

    void ServeClient(ClientHandler* client_handler);

    void Broadcast(const std::basic_string<char>& message);

    void SendMessage(ClientHandler* client_handler,
                     const std::basic_string<char>& message);

    void RemoveClient(ClientHandler* client_handler);

    void ProcessMessage(const std::basic_string<char>& sender,
                        const std::basic_string<char>& message);

  private:
    void Processer();

    void Server(ClientHandler* client_handler);

    void Sender(ClientHandler* client_handler);

    void Receiver(ClientHandler* client_handler);

    std::unordered_set<ClientHandler* > client_handlers_;
    std::unordered_map<std::string, ClientHandler* > username_to_handler_;
    std::unordered_map<ClientHandler*, std::queue<std::string>> message_queues_;
    std::queue<Message> processing_queue_;
    boost::mutex mutex_;
};

}

#endif
