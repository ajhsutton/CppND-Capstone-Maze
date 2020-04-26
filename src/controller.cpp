#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "bot.h"

void Controller::MoveBot(Bot *bot, Bot::Direction input) const {
  bot->moveDirection(input);
  return;
}

void Controller::HandleInput(bool &running, Bot * bot) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          MoveBot(bot, Bot::Direction::kDown);
          std::cout << "UP" << std::endl;
          break;

        case SDLK_DOWN:
          MoveBot(bot, Bot::Direction::kUp);
          std::cout << "DOWN" << std::endl;
          break;

        case SDLK_LEFT:
          MoveBot(bot, Bot::Direction::kLeft);
          std::cout << "LEFT" << std::endl;
          break;

        case SDLK_RIGHT:
          MoveBot(bot, Bot::Direction::kRight);
          std::cout << "RIGHT" << std::endl;
          break;
      }
    }
  }
}