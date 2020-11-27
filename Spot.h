#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

#ifndef SPOT_H
#define SPOT_H

class Spot {
public:
    Spot();
    Spot(int x, int y);

    RectangleShape getGridElement();
    int getx();
    int gety();
    int getf();
    int getg();
    int geth();
    vector<Spot*> getNeighbors();
    Spot* getPrevious();
    bool getWall();

    void setg(int);
    void setf(int);
    void seth(int);
    void setPrevious(Spot*);
    void setWall(bool);

    void addNeighbors(vector< vector<Spot> >&, int, int);
    void show(RenderWindow&, int, Color);
    int heuristic(Spot*);


private:
    int x;
    int y;
    int g;
    int f;
    // heuristic
    int h;
    vector<Spot*> neighbors;
    RectangleShape gridElement;
    Spot* previous;
    bool wall;

};

#endif
