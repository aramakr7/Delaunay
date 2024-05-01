#pragma once

#include <iostream>
class Edge;

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
