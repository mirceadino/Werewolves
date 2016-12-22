#ifndef CLIENT_TEXTVIEW_H
#define CLIENT_TEXTVIEW_H

#include <memory>

#include "controller.h"

namespace client {

class TextView {
  public:
    TextView(Controller* controller);

    void Init(const std::basic_string<char>& host, int port);

    void RunMain();

  private:
    enum ConfirmationCode {
      CODE_OK,
      CODE_INVALID
    };

    void AskUsername();

    ConfirmationCode GetConfirmationCode(const std::basic_string<char>&
                                         confirmation);

    void DisplayMainMenu();

    int ChooseOption();

    void RunSendMessage();
    void SendFullMessage(const std::basic_string<char>& message);

    void RunViewNewMessage();

    void RunExit();

    std::string username_;
    std::unique_ptr<Controller> controller_;
};

};

#endif
