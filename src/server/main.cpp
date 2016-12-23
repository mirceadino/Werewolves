#include <bits/stdc++.h>

#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>

#include "client_handler.h"

using namespace std;

typedef boost::system::system_error boost_error;
using namespace server;
using namespace std;
using boost::thread_group;
using boost::bind;

unordered_map<ClientHandler*, queue<string>> message_queues;
boost::mutex mtx;

void broadcast(const basic_string<char>& message) {
  mtx.lock();
  for (auto& queue : message_queues) {
    queue.second.push(message);
  }
  mtx.unlock();
}

void receive_messages(ClientHandler* client_handler) {
  try {
    while (true) {
      string message = client_handler->ReceiveMessage();
      cout << message << endl;
      broadcast(message);
    }

  } catch (boost_error err) {
    return;
  }
}

void send_messages(ClientHandler* client_handler) {
  try {
    while (true) {
      mtx.lock();

      if (!message_queues.count(client_handler)) {
        mtx.unlock();
        return;
      }

      if (!message_queues[client_handler].empty()) {
        string message = message_queues[client_handler].front();
        message_queues[client_handler].pop();
        client_handler->SendMessage(message);
      }

      mtx.unlock();
    }
  } catch (boost_error err) {
    return;
  }
}

void serve_client(ClientHandler* client_handler) {
  client_handler->AskUsername();
  cout << client_handler->username() << " connected." << endl;
  broadcast(client_handler->username() + " connected.");

  boost::thread receiver;
  boost::thread sender;
  try {
    mtx.lock();
    message_queues[client_handler] = queue<string>();
    mtx.unlock();

    receiver = boost::thread(bind(receive_messages, client_handler));
    sender = boost::thread(bind(send_messages, client_handler));

    receiver.join();

  } catch (boost::thread_interrupted err) {
    cerr << "Thread interrupted." << endl;
  }

  mtx.lock();
  message_queues.erase(client_handler);
  mtx.unlock();

  receiver.interrupt();
  sender.interrupt();

  client_handler->CloseConnection();
  cout << client_handler->username() << " disconnected." << endl;
  broadcast(client_handler->username() + " disconnected.");

  delete client_handler;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <port>" << endl;
    return -1;
  }

  const int port = atoi(argv[1]);

  thread_group client_servers;

  // Continuously accept clients (one client at the time).
  while (true) {
    ClientHandler* client_handler;
    try {
      // Try accepting a client.
      client_handler = new ClientHandler();
      client_handler->AcceptConnection(port);

      // Serve client once it's accepted.
      client_servers.create_thread(bind(serve_client, client_handler));

    } catch (boost_error err) {
      // If the client could not be accepted or if there was a failure while
      // serving him, display the error.
      cerr << err.what() << endl;
      delete client_handler;
    }
  }

  return 0;
}
