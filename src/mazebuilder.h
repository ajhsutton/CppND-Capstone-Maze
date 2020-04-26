#ifndef MAZEBUILDER_H
#define MAZEBUILDER_H

#include <vector>
#include <array>
#include <memory>
#include "maze.h"

class MazeBuilder {
    public:
        Maze * maze;
        MazeBuilder(Maze * maze):maze(maze){};
        void buildMaze(Node * startNode);
    private:
        void visitNode(Node * node);
        void expandEdge(Edge * edge);
        std::vector<Edge *> _frontier;
};

#endif