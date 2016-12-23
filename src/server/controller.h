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

class Controller {
  public:
    Controller() {}

    ClientHandler* AcceptNewClient(int port);

    void ServeClient(ClientHandler* client_handler);

    void Broadcast(const std::basic_string<char>& message);

    void SendMessage(ClientHandler* client_handler,
                     const std::basic_string<char>& message);

    void RemoveClient(ClientHandler* client_handler);

  private:
    friend void Serve(Controller* controller, ClientHandler* client_handler);
    friend void Sender(Controller* controller, ClientHandler* client_handler);
    friend void Receiver(Controller* controller, ClientHandler* client_handler);

    std::unordered_set<ClientHandler*> client_handlers_;
    std::unordered_map<std::string, ClientHandler*> username_to_handler_;
    std::unordered_map<ClientHandler*, std::queue<std::string>> message_queues_;
    boost::mutex mutex_;
};

void Serve(Controller* controller, ClientHandler* client_handler);
void Sender(Controller* controller, ClientHandler* client_handler);
void Receiver(Controller* controller, ClientHandler* client_handler);

}

#endif
