

#pragma once

#include <vector>
#include <cmath>
#include <iostream>

class Edge;
class Vertex;
class QuadEdge;
class Triangulation;

class Edge
{
    friend QuadEdge;
    friend Triangulation;
    public:
        Edge(Vertex* origin);
        Edge();
        ~Edge();


        Edge* rot();
        Edge* invRot();
        Edge* sym();

        Edge* oNext();
        Edge* oPrev();
        Edge* dNext();
        Edge* dPrev();

        Edge* lNext();
        Edge* lPrev();
        Edge* rNext();
        Edge* rPrev();

        QuadEdge* QE(){return (QuadEdge*)(this - index);}

        void setNext(Edge* next);
        void setSym(Edge* sym);
        void flip();
        void setOrigin(Vertex* origin);
        Vertex* getOrigin();
        void setDest(Vertex* dest);
        Vertex* getDest();
        void setIndex(int index);
        int getIndex();
        float slope();
        bool hasPoint(Vertex* p);



        // Overloaded << operator as a friend function
        friend std::ostream& operator<<(std::ostream& os, const Edge* edge);
        Vertex* origin;
        Vertex* dest;
        Edge* next;
        int index;
};

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
};

class Triangulation
{
    public:
        Triangulation(Vertex* a, Vertex* b, Vertex* c);
        void insertSite(Vertex* x);

    private:
        Edge* startingEdge;


};


