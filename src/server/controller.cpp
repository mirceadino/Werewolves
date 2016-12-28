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

Controller::Controller() {
  boost::thread processer(boost::bind(&Controller::Processer, this));
}

ClientHandler* Controller::AcceptNewClient(int port) {
  ClientHandler* handler = new ClientHandler();
  handler->AcceptConnection(port);
  client_handlers_.insert(handler);
  return handler;
}

void Controller::ServeClient(ClientHandler* client_handler) {
  boost::thread server(boost::bind(&Controller::Server, this, client_handler));
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

void Controller::ProcessMessage(const std::basic_string<char>& sender,
                                const std::basic_string<char>& message) {
  mutex_.lock();

  Message msg(sender, message, Message::NORMAL);

  if (sender == "") {
    msg = Message("", message, Message::ADMIN);
  } else {
    if (message == "/connect") {
      msg = Message(sender, "", Message::CONNECTION);
    } else if (message == "/disconnect") {
      msg = Message(sender, "", Message::DISCONNECTION);
    } else if (message.find("/me") == 0) {
      msg = Message(sender, message.substr(3), Message::NARATIVE);
    } else if (message.find("/whisper") == 0) {
      msg = Message(sender, message.substr(8), Message::WHISPER);
    }
  }

  processing_queue_.push(msg);
  mutex_.unlock();
}

void Controller::Processer() {
  while (true) {
    if (!processing_queue_.empty()) {
      mutex_.lock();
      const Message message = processing_queue_.front();
      processing_queue_.pop();
      mutex_.unlock();

      switch (message.type()) {
      case Message::CONNECTION: {
        Broadcast(message.sender() + " connected.");
        break;
      }

      case Message::DISCONNECTION: {
        Broadcast(message.sender() + " disconnected.");
        break;
      }

      case Message::NARATIVE: {
        Broadcast(message.sender() + message.message());
        break;
      }

      case Message::NORMAL: {
        Broadcast(message.sender() + ": " + message.message());
        break;
      }

      case Message::WHISPER: {
        SendMessage(username_to_handler_[message.sender()],
                    "You can\'t whisper yet.");
        break;
      }

      case Message::ADMIN: {
        Broadcast(message.message());
        break;
      }
      }
    }
  }
}

void Controller::Server(ClientHandler* client_handler) {
  client_handler->AskUsername();
  const string username = client_handler->username();
  ProcessMessage(username, "/connect");

  mutex_.lock();
  username_to_handler_[username] = client_handler;
  mutex_.unlock();

  boost::thread sender = boost::thread(boost::bind(&Controller::Sender, this,
                                       client_handler));
  boost::thread receiver = boost::thread(boost::bind(&Controller::Receiver, this,
                                         client_handler));

  receiver.join();
  SendMessage(client_handler, "/exit");
  sender.join();

  client_handler->CloseConnection();
  ProcessMessage(username, "/disconnect");

  RemoveClient(client_handler);
}

void Controller::Sender(ClientHandler* client_handler) {
  while (true) {
    if (!message_queues_[client_handler].empty()) {
      mutex_.lock();
      string message = message_queues_[client_handler].front();
      message_queues_[client_handler].pop();
      mutex_.unlock();

      if (message == "/exit") {
        break;
      }

      client_handler->SendMessage(message);
    }

  }
}

void Controller::Receiver(ClientHandler* client_handler) {
  while (true) {
    string message = client_handler->ReceiveMessage();

    if (message == "/exit") {
      break;
    }

    ProcessMessage(client_handler->username(), message);
  }
}

}
