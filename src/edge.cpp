#include "maze.h"
#include "edge.h"
#include "bot.h"
#include <iostream>
#include <memory>

Edge::Edge(EdgeState *state_, Node * child_, Node * parent_){
    state = state_;
    child = child_;
    parent = parent_;
}

Edge::Edge(Edge& edge){
    state = edge.state;
    child = edge.child;
    parent = edge.parent;
}

Edge::Edge(Edge&& edge){
    state = edge.state;
    child = edge.child;
    parent = edge.parent;
}

Edge& Edge::operator=(Edge& edge){
    state = edge.state;
    child = edge.child;
    parent = edge.parent;
}

Edge& Edge::operator=(Edge&& edge){
    state = edge.state;
    child = edge.child;
    parent = edge.parent;
}

void Edge::openEdge(){
    state->openEdgeState();
}

bool Edge::isOpen(){
    return state->isOpen();
}

Bot::Direction Edge::getDirection(){
    if ((parent->p.x == child->p.x)){
        // UP or DOWN
        if ((parent->p.y > child->p.y)){
            return Bot::Direction::kDown;
        } else {return Bot::Direction::kUp;}
    } else {
         if ((parent->p.x > child->p.x)){
            return Bot::Direction::kLeft;
        } else {return Bot::Direction::kRight;}
    }

}


EdgeState::EdgeState(Node* node1, Node* node2)
{
    // std::cout << "EdgeState for nodes at:" << node1 <<" and " << node2 << std::endl;
    // Initialize state
    open = std::make_unique<bool>(false);
    
    // Initialize Edges
    edge_1 = std::make_shared<Edge>(Edge(this, node1, node2));
    edge_1->parent->edges.emplace_back(edge_1);
    // std::cout << "   Edge 1:" << edge_1->parent << "->" << edge_1->child << std::endl;

    edge_2 = std::make_shared<Edge>(Edge(this, node2, node1));
    edge_2->parent->edges.emplace_back(edge_2);
    // std::cout << "   Edge 2:" << edge_2->parent << "->" << edge_2->child << std::endl;
}

void Edge::print(){
    std::cout << "Edge: (" << parent->p.x << "," << parent->p.y << ") ->  (" << child->p.x << "," << child->p.y << ")" << std::endl;
}

// Static Variable
int Node::_idCnt = 0;

// Constructors
Node::Node():state(NodeStates(unvisited)){
    id = _idCnt;
    _idCnt ++;
    p.x = 0;
    p.y = 0;
}

Node::Node(int id_, int px, int py):state(NodeStates(unvisited)){
    id = _idCnt;
    _idCnt ++;
    p.x = px;
    p.y = py;
}

void Node::printEdges(){
    std::cout << "    Print Node Edges: "<< this << " (" << p.x << "," << p.y << ") S= "<< edges.size() << std::endl;
    if (edges.size() > 0){
        for (auto edge: this->edges){
            std::cout << "        Edge:" << edge->child << "   " << edge->child->p.x << "," << edge->child->p.y << std::endl;
        }
    } else std::cout << "        Empty" << std::endl<< std::endl;
}

bool Node::isConnectedToNode(Node const *nodeB){
    if (edges.size() > 0){
        int ii = 1;
        for (auto edge: edges){
            if (edge->child == nodeB){
                return true;
            }
        }
    }
    return false;
}

bool Node::isValidMove(Node const *nodeB){
    if (edges.size() > 0){
        int ii = 1;
        for (auto edge: edges){
            if (edge->child == nodeB){
                return edge->state->isOpen();
            }
        }
    }
    return false;
}

std::vector<Point> Node::getAdjacentPositions(){
    std::vector<Point> out;
    for (int pos =0; pos < 4; pos++){
        switch (pos)
        {
        case 0:
            out.emplace_back(Point{p.x,p.y+1});
            break;
        case 1:
            out.emplace_back(Point{p.x+1,p.y});
            break;
        case 2:
            out.emplace_back(Point{p.x,p.y-1});
            break;
        case 3:
            out.emplace_back(Point{p.x-1,p.y});
            break;
        default:
            break;
        }
    }
    return out;
}

std::vector<Edge *> Node::getValidEdges(){
    std::vector<Edge *> validEdges;
    for (auto & edge : edges){
        if (edge->state->isOpen()){
            validEdges.push_back(edge.get());
        }
    }
    return validEdges;
}

void Node::moveBot(Node *newNode){
    //std::cout << "moveBot" << std::endl;
    if (isValidMove(newNode)){
        newNode->bot = std::move(bot);
        newNode->bot->node = newNode;
    }
}
