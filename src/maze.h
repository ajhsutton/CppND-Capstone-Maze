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

  //search
  std::vector<Node *> getBoundaryNodes();
  void printBoundary();
  std::vector<Node *> findPathToNode(Node * current,
     const Node * target, const Node * parent);
   int distanceToGoal(Node * node);
   
  // Bot
  Bot * bot;
  void addBot();

  Node * goal;

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<EdgeState>> edgestates;

 private:
  int _width;
  int _height;
  
  Node * getNodeForPoint(Point const p);
  void buildGrid();
  void connectNodes(Node* nodeA, Node* nodeB);

  bool containsnode(std::vector<Node *> nodes, Node * node);
  std::vector<Node *> getchildnodes(Node * current);
};

#endif