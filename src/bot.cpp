#include "bot.h"
#include "edge.h"
#include "maze.h"
#include <iostream>
#include <random>

void Bot::Update() {
}

void Bot::setNode(Node * node){
    this->node = node;
}

Point Bot::getPosition(){
    return this->node->getLocation();
}

void Bot::moveDirection(Direction dir){
    // Reset mode if command received
    if  (botMode == Mode::kAuto){
        botMode = Mode::kUser;
        std::cout << "Bot Mode: User" << std::endl;
    }
    tLastMove = SDL_GetTicks();

    // Find edge for direction.
    for (auto & edge : node->edges){
        if ((edge->isOpen())&(edge->getDirection() == dir)){
            // Move Bot
            // switch (dir) {
            //     case Direction::kUp:
            //         std::cout << "UP" << std::endl;
            //         break;
            //     case Direction::kDown:
            //         std::cout << "DOWN" << std::endl;
            //         break;
            //     case Direction::kLeft:
            //         std::cout << "LEFT" << std::endl;
            //         break;
            //     case Direction::kRight:
            //         std::cout << "RIGHT" << std::endl;
            //         break;
            //     default:
            //         break;
            // }
            // std::cout << "     Move Bot to " << edge->child->p.x << ", " << edge->child->p.y << std::endl;
            
            // Increment move counter
            moves++;

            Node * newNode = edge->child;
            node->moveBot(newNode);
        }
    }
}


// Randomly move the Bot along a node's edges
void Bot::randomMove(){  

    // Valid edges
    std::vector<Edge *> validEdges = node->getValidEdges();

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<int> distribution(0,validEdges.size()-1);
    int num = distribution(mt);

    Edge * newEdge = validEdges[num];
    Node * newNode = newEdge->child;
    node->moveBot(newNode);
}

void Bot::moveBot(){
    std::vector<Node *> bestpath;
    int bestcost = INT16_MAX;
    if (maze->goal->state == NodeStates::visible){
        auto goalpath = maze->findPathToNode(node, maze->goal, nullptr);
        if (goalpath.size()>0){
            if (goalpath.back() == this->node){
                goalpath.pop_back();
                //std::cout << "Next Move:  " << bestpath.back()->p.x << "," << bestpath.back()->p.y << std::endl;
                node->moveBot(goalpath.back());
                return;
            }
        }
    } else {
        // find nexg-best node
        auto bdry = maze->getBoundaryNodes();
        for (Node * bdrynode : bdry){
            auto bdrypath = maze->findPathToNode(node, bdrynode, nullptr);
            int pathcost = bdrypath.size() + maze->distanceToGoal(bdrynode);
            if (pathcost < bestcost){
                bestcost = pathcost;
                bestpath = bdrypath;
            }
        }
        // Select the next move.
        if (bestpath.size() > 0){
            if (bestpath.back() == this->node){
                bestpath.pop_back();
                //std::cout << "Next Move:  " << bestpath.back()->p.x << "," << bestpath.back()->p.y << std::endl;
                node->moveBot(bestpath.back());
                return;
            }
        } 
    }
    // No defined move;
    randomMove();
}

Bot::Bot(Bot && bot){
    moves = bot.moves;
    bot.moves = -1;
}

Bot & Bot::operator=(Bot &&bot){
    // Move Assignment Opterator
    if (this == &bot){
        return *this;
    }
    moves = bot.moves;
    bot.moves = -1;
    return *this;
} 