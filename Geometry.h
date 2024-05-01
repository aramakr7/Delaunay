

#pragma once

#include <vector>
#include <cmath>
#include <iostream>

#include "edge.h"
class Edge;
class Vertex;
class QuadEdge;
class Triangulation;


class Vertex
{
    public:
        Vertex(float x, float y);
        ~Vertex();

        float x;
        float y;
        Edge* getEdge();
        void addEdge(Edge* edge);
        float lengthSquared();

        // Overloaded << operator as a friend function
        friend std::ostream& operator<<(std::ostream& os, const Vertex* edge);
        Edge* edge;

};


class QuadEdge
{
    public:
        QuadEdge();
        Edge edges[4];

    friend Edge* makeEdge(Vertex *a, Vertex *b);
        // Overloaded << operator as a friend function
        friend std::ostream& operator<<(std::ostream& os, const QuadEdge& edge);
};

class Triangulation
{
    public:
        Triangulation(Vertex* a, Vertex* b, Vertex* c);
        void insertSite(Vertex* x);

    private:
        Edge* startingEdge;


};


