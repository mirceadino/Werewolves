#include "textview.h"

#include <iostream>
#include <memory>

#include <boost/algorithm/string.hpp>

namespace client {

typedef boost::system::system_error boost_error;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

// ----------------------------------------------------------------------------
// ----- PUBLIC ---------------------------------------------------------------
// ----------------------------------------------------------------------------

TextView::TextView(Controller* controller):
  controller_ (controller) {}

void TextView::Init(const std::basic_string<char>& host, int port) {
  try {
    AskUsername();
    controller_->OpenConnection(host, port);
    cout << "Connection opened." << endl;

  } catch (boost_error err) {
    cout << "Unable to connect." << endl;
    cerr << err.what() << endl;
  }
}

void TextView::RunMain() {
  while (true) {
    DisplayMainMenu();

    const int option = ChooseOption();

    try {
      switch (option) {
      case 1: {
        RunSendMessage();
        break;
      }

      case 2: {
        RunViewNewMessage();
        break;
      }

      case 0: {
        RunExit();
        break;
      }

      default: {
        cout << "Invalid option " << endl;
        break;
      }
      }
    } catch (boost_error err) {
      cerr << err.what() << endl;
    }
  }
}

// ----------------------------------------------------------------------------
// ----- PRIVATE --------------------------------------------------------------
// ----------------------------------------------------------------------------

void TextView::AskUsername() {
  cout << "Type username: ";

  getline(cin, username_);
  boost::trim(username_);
}

void TextView::DisplayMainMenu() {
  cout << "--------------------------" << endl;
  cout << "1. Send message" << endl;
  cout << "2. View new messages" << endl;
  cout << "0. Exit" << endl;
}

int TextView::ChooseOption() {
  cout << "Choose option: ";

  int option;
  cin >> option;
  cin.ignore(256, '\n');

  return option;
}

void TextView::RunSendMessage() {
  cout << "Type message: ";

  string message;
  getline(cin, message);
  boost::trim(username_);

  SendFullMessage(message);
}

void TextView::SendFullMessage(const std::basic_string<char>& message) {
  controller_->SendMessage(username_ + ": " + message);
}

void TextView::RunViewNewMessage() {
  vector<string> messages = controller_->RetrieveNewMessages();

  cout << "New messages (" << messages.size() << "):" << endl;
  cout << "--------------------------" << endl;
  if (messages.empty() ) {
    cout << "None" << endl;
  }

  for (const string& message : messages) {
    cout << message << endl;
  }
}

void TextView::RunExit() {
  try {
    controller_->CloseConnection();
    cout << "Good bye!" << endl;
    exit(0);

  } catch (boost_error err) {
    cout << "Unable to disconnect." << endl;
    cerr << err.what() << endl;
  }
}

}
