#include "renderer.h"
#include <iostream>
#include <string>
#include "edge.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, 2, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Maze const *maze) {
  
  SDL_Rect block;
  block.w = (screen_width) / (grid_width +1);
  block.h = (screen_height) / (grid_height + 1);

  // Border Offset from frame
  // TODO: SDL *may* allow new origin definition
  auto offset_x = block.w/2;
  auto offset_y = block.h/2;

  SDL_Rect node_block;
  node_block.w = ((screen_width  - block.w) - grid_width) / grid_width;
  node_block.h = ((screen_height - block.h) - grid_height) / grid_height;
  
  SDL_Rect bot_block;
  bot_block.w = node_block.w / 2;
  bot_block.h = node_block.h / 2;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  //SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  //block.x = food.x * block.w;
  //block.y = food.y * block.h;
  //SDL_RenderFillRect(sdl_renderer, &block);

  // Render maze cells
  for (auto const & node : maze->nodes) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (node->state == NodeStates::visible){
      node_block.x = node->p.x * block.w + offset_x;
      node_block.y = node->p.y * block.h + offset_y;
      SDL_RenderFillRect(sdl_renderer, &node_block);
    }
  }

  // Draw Connectivity lines between boxes
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  for (auto const & node : maze->nodes) {
    if (node->state == NodeStates::visible){
      for (auto const edge : node->edges){
        if (edge->isOpen()){
          auto p1_x = node->p.x* block.w        ;
          auto p2_x = edge->child->p.x* block.w ;
          auto p1_y = node->p.y* block.h        ;
          auto p2_y = edge->child->p.y* block.h ;
          
          
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
          
          auto mid_x = (p1_x + p2_x)/2;
          auto mid_y = (p1_y + p2_y)/2;
          node_block.x = mid_x + offset_x;
          node_block.y = mid_y + offset_y;
          SDL_RenderFillRect(sdl_renderer, &node_block);
        } 
      }
    }
  }

  // Draw Goal
  if (maze->goal->state == NodeStates::visible){
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    Point goal_pos = maze->goal->p;
    bot_block.x = (goal_pos.x + 0.25) * block.w + offset_x; // + block.w;
    bot_block.y = (goal_pos.y + 0.25) * block.h + offset_y;// + block.h;
    SDL_RenderFillRect(sdl_renderer, &bot_block);
  }

  // Draw Line
  // for (auto const & node : maze->nodes) {
  //   for (auto const edge : node->edges){
  //     if (edge->isOpen()){
  //       auto p1_x = node->p.x* block.w        ;
  //       auto p2_x = edge->child->p.x* block.w ;
  //       auto p1_y = node->p.y* block.h        ;
  //       auto p2_y = edge->child->p.y* block.h ;
  //       SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0x10);
  //       SDL_RenderDrawLine(sdl_renderer, 
  //         p1_x+ block.w/2 + offset_x, 
  //         p1_y+ block.h/2 + offset_y,
  //         p2_x+ block.w/2 + offset_x, 
  //         p2_y+ block.h/2 + offset_x);
  //     }
  //   }
  // }

  // Render Bot
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
  Point bot_pos = maze->bot->getPosition();
  bot_block.x = (bot_pos.x + 0.25) * block.w + offset_x; // + block.w;
  bot_block.y = (bot_pos.y + 0.25) * block.h + offset_y;// + block.h;
  SDL_RenderFillRect(sdl_renderer, &bot_block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Game Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
