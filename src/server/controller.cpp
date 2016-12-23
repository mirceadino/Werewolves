#include "controller.h"

#include <iostream>

#include "client_handler.h"

namespace server {

typedef boost::system::system_error boost_error;
using std::basic_string;
using std::queue;
using std::string;
using boost::thread_group;
using std::cerr;
using std::endl;
using namespace boost::asio;
using namespace boost::asio::ip;

ClientHandler* Controller::AcceptNewClient(int port) {
  ClientHandler* handler = new ClientHandler();
  handler->AcceptConnection(port);
  client_handlers_.insert(handler);
  return handler;
}

void Controller::ServeClient(ClientHandler* client_handler) {
  boost::thread server(boost::bind(Serve, this, client_handler));
}

void Controller::Broadcast(const basic_string<char>& message) {
  std::cout << message << endl;
  for (ClientHandler* client_handler : client_handlers_) {
    SendMessage(client_handler, message);
  }
}

void Controller::SendMessage(ClientHandler* client_handler,
                             const std::basic_string<char>& message) {
  mutex_.lock();
  message_queues_[client_handler].push(message);
  mutex_.unlock();
}

void Controller::RemoveClient(ClientHandler* client_handler) {
  mutex_.lock();
  client_handlers_.erase(client_handler);
  username_to_handler_.erase(client_handler->username());
  message_queues_.erase(client_handler);
  mutex_.unlock();

  delete client_handler;
}

void Serve(Controller* controller, ClientHandler* client_handler) {
  client_handler->AskUsername();
  const string username = client_handler->username();
  controller->Broadcast(username + " connected.");

  controller->mutex_.lock();
  controller->username_to_handler_[username] = client_handler;
  controller->mutex_.unlock();

  boost::thread sender = boost::thread(boost::bind(Sender, controller,
                                       client_handler));
  boost::thread receiver = boost::thread(boost::bind(Receiver, controller,
                                         client_handler));

  receiver.join();
  controller->SendMessage(client_handler, "EXIT");
  sender.join();

  client_handler->CloseConnection();
  controller->Broadcast(username + " disconnected.");

  controller->RemoveClient(client_handler);
}

void Sender(Controller* controller, ClientHandler* client_handler) {
  while (true) {
    controller->mutex_.lock();

    if (!controller->message_queues_[client_handler].empty()) {
      string message = controller->message_queues_[client_handler].front();
      if (message == "EXIT") {
        controller->mutex_.unlock();
        break;
      }

      controller->message_queues_[client_handler].pop();
      client_handler->SendMessage(message);
    }

    controller->mutex_.unlock();
  }
}

void Receiver(Controller* controller, ClientHandler* client_handler) {
  while (true) {
    string message = client_handler->ReceiveMessage();
    if (message == "EXIT") {
      break;
    }
    controller->Broadcast(message);
  }
}

}
