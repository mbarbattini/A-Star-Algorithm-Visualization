#include "Spot.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

// default constructor
Spot::Spot() {
    x = 0;
    y = 0;
    g = 0;
    f = 0;
    h = 0;
    previous = nullptr;
    wall = false;
}

// constructor assigns a location to the spot
Spot::Spot(int xPos, int yPos) {
    x = xPos;
    y = yPos;
    g = 0;
    f = 0;
    h = 0;
    previous = nullptr;
    wall = false;
    if ((rand() % 100 ) < 15) {
        wall = true;
    }
}

// getters

RectangleShape Spot::getGridElement() {
    return gridElement;
}

int Spot::getx() {
    return x;
}

int Spot::gety() {
    return y;
}

int Spot::getf() {
    return f;
}

int Spot::geth() {
    return h;
}

int Spot::getg() {
    return g;
}

vector<Spot*> Spot::getNeighbors() {
    return neighbors;
}

Spot* Spot::getPrevious() {
    return previous;
}

bool Spot::getWall() {
    return wall;
}




// setters

void Spot::setg(int _g) {
    g = _g;
}

void Spot::setf(int _f) {
    f = _f;
}

void Spot::seth(int _h) {
    h = _h;
}

void Spot::setPrevious(Spot* term) {
    previous = term;
}

void Spot::setWall(bool _v) {
    wall = _v;
}





// member functions

// add the neighboring spots for a single spot object
// pass in the entire array to have access to all possible neighbors
// takes into account if the neighbor is on the edges and attempting to access outside of array
void Spot::addNeighbors(vector< vector<Spot> > &gridArray, int col, int row) {
    if (x < col - 1) {
        neighbors.push_back(&gridArray.at(x + 1).at(y));
    }
    if (x > 0) {
        neighbors.push_back(&gridArray.at(x - 1).at(y));
    }
    if (y < row - 1) {
        neighbors.push_back(&gridArray.at(x).at(y + 1));
    }
    if (y > 0) {
        neighbors.push_back(&gridArray.at(x).at(y - 1));
    }
}

// 83d8a0

void Spot::show(RenderWindow& window, int spotSize, Color color) {
    gridElement.setSize(Vector2f(spotSize - 1, spotSize - 1));
    gridElement.setOutlineThickness(1);
    gridElement.setOutlineColor(Color::Black);
    gridElement.setPosition(Vector2f(spotSize * x, spotSize * y));
    gridElement.setFillColor(color);
    if (wall) {
        gridElement.setFillColor(Color(0,0,0));
    }
    window.draw(gridElement);
}

// heuristic is a function that estimates the distance between the node and the end
int Spot::heuristic(Spot* end) {
//  int horizontal = abs(end.x - x);
//  int vertical = abs(end.y - y);
//  int distance = sqrt(pow(horizontal, 2) + pow(vertical, 2));
    int distance = abs(end->x - x) + abs(end->y - y);
    return distance;
}
