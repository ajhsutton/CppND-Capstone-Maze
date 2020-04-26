#include <vector>
#include <memory>
#include <iostream>
#include "assert.h"
#include "mazebuilder.h"
#include "maze.h"

// Generate a maze using the provided start and end Nodes
void MazeBuilder::buildMaze(Node * startNode){
     // Step 1; Mark startNode as visited and add to frontier.
    visitNode(startNode);

    // Step 3; Expand Nodes on the Frontier (LIFO)
    while(_frontier.size() > 0){
        // Expand Next Node
        auto newEdge = _frontier.back();
        _frontier.pop_back();
        expandEdge(newEdge);
    }
}

void MazeBuilder::expandEdge(Edge * edge){
    // If edge leads to unvisited node,
    // - mark edge as open,
    // - visitnew node.
    if (edge->child->state == NodeStates::unvisited){
        // Valid edge: mark as open
        edge->openEdge();
        // Visit Child
        visitNode(edge->child);
    }
}

void MazeBuilder::visitNode(Node * node){
    // Mark node as visited
    node->state = visited;
    // Add edges to unvisited nodes to Frontier for expansion
    std::vector<Edge *> openEdges;
    maze->getOpenEdges(node, &openEdges);

    if (openEdges.size() > 0){
        for (auto edge : openEdges){
            assert(edge->child->state == unvisited);
            _frontier.push_back(edge);
        }
    }
}
