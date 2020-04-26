#ifndef BOT_H
#define BOT_H

#include <vector>
#include "SDL.h"

class Node;
struct Point;

class Bot {
 public:
  enum class Direction {kNone, kUp, kDown, kLeft, kRight };
  Node * node;
  void Update();
  
  Direction direction = Direction::kNone;
  
  void setNode(Node * node);
  Point getPosition();
  // Constructors for Move semantics
  // Bot();
  // Bot(Bot && bot);
  // Bot(Bot & bot) = delete; // Copy
  // Bot &operator=(Bot &bot) = delete; // Copy Assignment 
  // Bot &operator=(Bot &&bot); // Move Assignment Opterator
  // ~Bot();

  //int size{1};
  //Point p;
  void moveDirection(Direction dir);
 private:
  
};

#endif