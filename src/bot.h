#ifndef BOT_H
#define BOT_H

#include <vector>
#include "SDL.h"

class Node;
struct Point;
class Maze;

class Bot {
 public:
  enum class Direction {kNone, kUp, kDown, kLeft, kRight };
  enum class Mode {kUser, kAuto};
  Node * node;
  Maze * maze;
  int moves{0};

  // Time of last move
  Uint32 tLastMove = 0;
  Mode botMode = Mode::kUser;

  Direction direction = Direction::kNone;
  
  void Update();
  void moveBot();
  void setNode(Node * node);
  Point getPosition();
  // Constructors for Move semantics
  Bot(){};
  Bot(Bot && bot);
  // Bot(Bot & bot) = delete; // Copy
  // Bot &operator=(Bot &bot) = delete; // Copy Assignment 
  Bot &operator=(Bot &&bot); // Move Assignment Opterator
  // ~Bot();
  void moveDirection(Direction dir);
  void randomMove();

 private:
  void updateMap();
  class MapNode;
  std::vector<MapNode *> mazemap; // unexplored nodes
};

#endif