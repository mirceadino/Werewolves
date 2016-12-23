#include <bits/stdc++.h>

#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>

#include "controller.h"

using namespace std;

typedef boost::system::system_error boost_error;
using namespace server;
using namespace std;
using boost::thread_group;
using boost::bind;

unordered_map<Controller*, queue<string>> message_queues;
boost::mutex mtx;

void broadcast(const basic_string<char>& message) {
  mtx.lock();
  for (auto& queue : message_queues) {
    queue.second.push(message);
  }
  mtx.unlock();
}

void receive_messages(Controller* controller) {
  try {
    while (true) {
      string message = controller->ReceiveMessage();
      cout << message << endl;
      broadcast(message);
    }

  } catch (boost_error err) {
    return;
  }
}

void send_messages(Controller* controller) {
  try {
    while (true) {
      mtx.lock();

      if (!message_queues.count(controller)) {
        mtx.unlock();
        return;
      }

      if (!message_queues[controller].empty()) {
        string message = message_queues[controller].front();
        message_queues[controller].pop();
        controller->SendMessage(message);
      }

      mtx.unlock();
    }
  } catch (boost_error err) {
    return;
  }
}

void serve_client(Controller* controller) {
  controller->AskUsername();
  cout << controller->username() << " connected." << endl;
  broadcast(controller->username() + " connected.");

  boost::thread receiver;
  boost::thread sender;
  try {
    mtx.lock();
    message_queues[controller] = queue<string>();
    mtx.unlock();

    receiver = boost::thread(bind(receive_messages, controller));
    sender = boost::thread(bind(send_messages, controller));

    receiver.join();

  } catch (boost::thread_interrupted err) {
    cerr << "Thread interrupted." << endl;
  }

  mtx.lock();
  message_queues.erase(controller);
  mtx.unlock();

  receiver.interrupt();
  sender.interrupt();

  controller->CloseConnection();
  cout << controller->username() << " disconnected." << endl;
  broadcast(controller->username() + " disconnected.");

  delete controller;
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
    Controller* controller;
    try {
      // Try accepting a client.
      controller = new Controller();
      controller->AcceptConnection(port);

      // Serve client once it's accepted.
      client_servers.create_thread(bind(serve_client, controller));

    } catch (boost_error err) {
      // If the client could not be accepted or if there was a failure while
      // serving him, display the error.
      cerr << err.what() << endl;
      delete controller;
    }
  }

  return 0;
}
