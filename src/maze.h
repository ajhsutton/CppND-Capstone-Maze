#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <memory>

#include "edge.h"

class Maze {
 public:
  Maze();
  Maze(int grid_width, int grid_height); // Constructor
  Maze(const Maze & maze) = delete; // Copy Constructor
  Maze(Maze && maze); // Move Constructor
  Maze &operator=(Maze &maze) = delete; // Copy Operator
  Maze &operator=(Maze &&maze); // Move Assignment Opterator
  ~Maze();

  void getOpenEdges(Node const *node, std::vector<Edge *> * openEdges);
  void printNodes();
  void printEdges();

  Bot * bot;
  void addBot();

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<EdgeState>> edgestates;

 private:
  int _width;
  int _height;
  
  Node * getNodeForPoint(Point const p);
  void buildGrid();
  void connectNodes(Node* nodeA, Node* nodeB);
};

#endif