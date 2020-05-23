#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height) :
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  maze = Maze(static_cast<int>(grid_width), static_cast<int>(grid_height));
  //PlaceFood();
}

bool Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;  
  
  maze.addBot();
  maze.bot->maze = &maze;

  while (running) {
    frame_start = SDL_GetTicks();
    //std::cout << "Frame" << std::endl;

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, maze.bot);
    if (!Update()){
      running = false;
    }
    renderer.Render(&maze);
    
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(maze.bot->moves, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  return restart;
}

bool Game::Update() {
  // Update Visibility
  maze.bot->node->state = NodeStates::visible;
  for (auto &edge: maze.bot->node->edges){
    if (edge->state->isOpen()){
      edge->child->state =  NodeStates::visible;
    }
  }

  // If bot hasn't moed in 5 seconds, set to auto
  auto tNow = SDL_GetTicks();
   auto tUpdate = tNow - maze.bot->tLastMove;
  if (maze.bot->botMode == Bot::Mode::kUser){
    if (tUpdate > 2000){
      maze.bot->botMode = Bot::Mode::kAuto;
      std::cout << "Bot Mode: Auto" << std::endl;
    }
  }

  // if Bot in auto, move biot
  if (maze.bot->botMode == Bot::Mode::kAuto){
    if (tUpdate > 100){
      maze.bot->tLastMove = tNow;
      maze.bot->moveBot();
    }
  }

  // Check finish condition
  if (maze.goal->bot != nullptr){
    restart = true;
  }

  // Debug: print boundary nodes and find best path
  // if (tUpdate > 1000){ 
  //   std::cout << std::endl << std::endl;
  //   std::cout << "-------------------" << std::endl;
  //   std::cout << "    Boundary      " << std::endl;
  //   std::cout << "-------------------" << std::endl;
  //   maze.printBoundary();
  //   std::cout << "-------------------" << std::endl;
  //   std::cout << "Path for Bot @ " << maze.bot->node->p.x << ", " <<  maze.bot->node->p.y << std::endl;
  //   auto bdry = maze.getBoundaryNodes();
  //   for (Node * bdrynode : bdry){
  //     auto bdrypath = maze.findPathToNode(maze.bot->node, bdrynode, nullptr);
  //     int pathcost = bdrypath.size() + maze.distanceToGoal(bdrynode);
  //     std::cout << "    Boudary Node: (" <<  bdrynode->p.x << "," <<  bdrynode->p.y << "), Cost:" <<pathcost << std::endl;
  //     std::cout << "        Path";
  //     for (auto node_ : bdrypath){
  //       std::cout << " -> (" <<  node_->p.x << "," <<  node_->p.y << ")";
  //     }
  //     std::cout << std::endl;
  //   }
  //   std::cout << "-------------------" << std::endl;
  // }

  return !restart;
}

int Game::GetScore() const { return maze.bot->moves; }
int Game::GetSize() const { return 0; }