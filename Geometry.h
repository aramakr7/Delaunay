

#pragma once
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H

#include <vector>
#include <cmath>

class Edge;
class Vertex;
class QuadEdge;

class Edge
{
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


    private:
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

    private:
        Edge* edge;

};


class QuadEdge
{
    public:
        QuadEdge();
        Edge edges[4];
};





#endif
