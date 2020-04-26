#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bot.h"

class Controller {
 public:
  void HandleInput(bool &running, Bot *Bot) const;
 
  private:
  void MoveBot(Bot *bot, Bot::Direction input) const;
};

#endif