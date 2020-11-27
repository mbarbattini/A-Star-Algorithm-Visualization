#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unistd.h>

using namespace std;
using namespace sf;

// adding class Spot files
#include "Spot.h"

// dimensions of the grid
const int numberRows = 75;
const int numberCols = 75;
const int spotSize = 20;

void removeFromVector(vector<Spot*>& vec, Spot* search) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec.at(i)->getx() == search->getx()) {
            vec.erase(vec.begin() + i);
        }
    }
}

bool search(vector<Spot*> vec, Spot* object) {
    // loop through each term in the vector to be searched
    for (int i = 0; i < vec.size(); i++) {
        Spot* term = vec.at(i);
        // if the spot object has the same position, return true
        if ( ( term->getx() == object->getx() ) && ( term->gety() == object->gety() ) ) {
            return true;
        }
    }
    // after looping through entire vector and is not found, return false
    return false;
}

// heuristic is a function that estimates the distance between the node and the end
int heuristic(Spot* from, Spot* to) {
  int horizontal = abs(from->getx() - to->getx());
  int vertical = abs(from->gety() - to->gety());
  int distance = sqrt(pow(horizontal, 2) + pow(vertical, 2));
  return distance;
}

int main() {

    srand(time(0));

    // create a 2D vector of spot objects
    vector< vector<Spot> > gridVector(numberRows, vector<Spot> (numberCols));

    // open set vector
    vector<Spot*> openSet;

    // closed set vector
    vector<Spot*> closedSet;

    // path vector
    vector<Spot*> path;

    // add all spot objects to the array
    for (int i = 0; i < numberCols; i++) {
        for (int j = 0; j < numberRows; j++) {
            Spot term(i, j);
            gridVector.at(i).at(j) = term;
        }
    }

    // add all the neighbors for each spot
    for (int i = 0; i < numberCols; i++) {
        for (int j = 0; j < numberRows; j++) {
            gridVector.at(i).at(j).addNeighbors(gridVector, numberCols, numberRows);
        }
    }

    // starting spot
    Spot* start = &gridVector.at(0).at(0);
    start->setWall(false);

    // ending spot
    Spot* end = &gridVector.at(numberCols - 1).at(numberRows - 1);
    end->setWall(false);

    // create the window
    RenderWindow window(VideoMode(numberRows * spotSize, numberCols * spotSize), "A* Algorithm");

    // draw loop
    openSet.push_back(start);
    while (window.isOpen()) {
        // close the window
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // A* Algorithm
        // iterates through each Spot object in the openSet
        // if the openSet is greater than 0, then there are still possible paths to evaluate
        // immediately add the start to the open set because it is the only known node

        if (openSet.size() > 0) {
            // updating the best next step based on f score
            int winnerIndex = 0;
            for (int i = 0; i < openSet.size(); i++ ) {
                if (openSet.at(i)->getf() < openSet.at(winnerIndex)->getf()) {
                    winnerIndex = i;
                }
            }

            // current node that is being evaluated
            Spot* current = openSet.at(winnerIndex);

            // if the next best step is the end, then the algorithm is complete
            if (current->getx() == end->getx() && current->gety() == end->gety()) {
                cout << "COMPLETED!" << endl;
                usleep(15000000);
                return 0;
            }

            // if not, add the node to the closed set
            closedSet.push_back(current);

            // remove the node from the openSet
            removeFromVector(openSet, current);

            // check each neighbor of the current node
            vector<Spot*> neighbors = current->getNeighbors();
//            vector<Spot> neighborsData = *neighborsPointer;
            for (int i = 0; i < neighbors.size(); i++) {
                Spot* neighbor = neighbors.at(i);
                // searching algorithm
                // if the neighbor is not in the closedSet already
                if (!search(closedSet, neighbor) && !neighbor->getWall()) {
                    // temporary g score
                    int tempG = current->getg() + heuristic(neighbor, current);
                    // is the neighbor node already in the openSet?
                    // assume that going to this neighbor is not the best option
                    bool newPath = false;
                    if (search(openSet, neighbor)) {
                        // is this a better way to get to the node than the previous way?
                        // is the temporary g score lower than the  previous g score?
                        if (tempG < neighbor->getg()) {
                            // assign the better g score
                            neighbor->setg(tempG);
                            // if so, then move to this neighbor and update the newPath
                            newPath = true;
                        }
                    } else {
                        // if not in the openSet, then assign the neighbor the tempG score
                        neighbor->setg(tempG);
                        // add the neighbor to the openSet because it has been evaluated
                        openSet.push_back(neighbor);
                        // was not in the openSet in the first place
                        newPath = true;
                    }
                    // only if going to this neighbor results in the lowest cost
                    if (newPath) {
                        // update the h score of the neighbor node
                        neighbor->seth(heuristic(neighbor, end));
                        neighbor->setf(neighbor->getg() + neighbor->geth());

                        // set the neighbor node's previous member pointer to the current node
                        neighbors.at(i)->setPrevious(current);
                    }
                }
            }

            // path reconstruction
            Spot* temp = current;

            path.push_back(temp);
            // while the node has a previous node, we can construct the path
            // the start node will not have a previous
            // pointer land
            while (temp->getPrevious() != nullptr) {
                // add the previous node to the path
                path.push_back(temp->getPrevious());
                // move to the previous node and repeat
                temp = temp->getPrevious();
            }

        } else {
            // no solution if open set is equal to 0 and not done
            cerr << "No solution" << endl;
            usleep(15000000);
            return -1;
        }

        // show the nodes that are not in the closed or open set
        for (int i = 0; i < numberCols; i++) {
            for (int j = 0; j < numberRows; j++) {
                gridVector.at(i).at(j).show(window, spotSize, Color(255, 255, 255));
            }
        }

        // show the nodes that are in the openSet as green
        for (int i = 0; i < closedSet.size(); i++) {
            Spot term = *closedSet.at(i);
            term.show(window, spotSize, Color(0, 255, 0));
        }

        // show the nodes that are in the closedSet as red
        for (int i = 0; i < openSet.size(); i++) {
            Spot term = *openSet.at(i);
            term.show(window, spotSize, Color(255, 0, 0));
        }

        // show the path
        for (int i = 0; i < path.size(); i++) {
            Spot term = *path.at(i);
            term.show(window, spotSize, Color(0,0,255));
        }

        // display the window
        window.display();

        path.clear();


        usleep(50000);
    }

}

