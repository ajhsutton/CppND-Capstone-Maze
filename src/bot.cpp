#include "bot.h"
#include "edge.h"
#include <iostream>

void Bot::Update() {
}

void Bot::setNode(Node * node){
    this->node = node;
}

Point Bot::getPosition(){
    return this->node->getLocation();
}

void Bot::moveDirection(Direction dir){
    // Find edge for direction.
    for (auto & edge : node->edges){
        if ((edge->isOpen())&(edge->getDirection() == dir)){
            // Move Bot
            switch (dir)
            {
            case Direction::kUp:
                std::cout << "UP" << std::endl;
                break;
            case Direction::kDown:
                std::cout << "DOWN" << std::endl;
                break;
            case Direction::kLeft:
                std::cout << "LEFT" << std::endl;
                break;
            case Direction::kRight:
                std::cout << "RIGHT" << std::endl;
                break;
            default:
                break;
            }
            std::cout << "     Move Bot to " << edge->child->p.x << ", " << edge->child->p.y << std::endl;
            
            Node * newNode = edge->child;
            node->moveBot(newNode);
        }
    }
}

// Bot::Bot(Bot && bot){

// }

// Bot & Bot::operator=(Bot &&bot){
//     // Move Assignment Opterator
//     return 
// } 