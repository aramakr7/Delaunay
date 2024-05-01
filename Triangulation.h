#pragma once
#include "Edge.h"
#include "Vertex.h"
#include "common.h"
#include <vector>
#include <tuple>

typedef std::tuple<std::vector<Edge*>, std::vector<Edge*> > EdgeTuple;
class Triangulation
{
    
public:
    Triangulation(){};
    Triangulation(Vertex* a, Vertex* b, Vertex* c);
    
    void insertSite(Vertex* x);
    EdgeTuple divideAndConquer(std::vector<Vertex*> points);
    std::vector<Edge*> m_edges;

    Edge* connect(Edge* e1, Edge* e2)
    {
        Edge* e = makeEdge();
        m_edges.push_back(e);
        e->setOrigin(e1->getDest());
        e->setDest(e2->getOrigin());

        // Perform splice operations
        splice(e, e1->lNext());
        splice(e->sym(), e2);

        // Return a pointer to our new edge
        return e;
    }

    Edge* makeEdge()
    {
        QuadEdge* q = new QuadEdge();
        return q->edges;
    }

    void splice(Edge* e1, Edge* e2)
    {
        Edge* alpha = e1->oNext()->rot();
        Edge* beta = e2->oNext()->rot();

        Edge* t1 = e2->oNext();
        Edge* t2 = e1->oNext();
        Edge* t3 = beta->oNext();
        Edge* t4 = alpha->oNext();

        e1->setNext(t1);
        e2->setNext(t2);
        alpha->setNext(t3);
        beta->setNext(t4);
    }

    bool LeftOf(Edge* e, Vertex* z)
    {
        // Return true if the point is left of the oriented line defined by the edge
        return ccw(z, e->getOrigin(), e->getDest());
    }

    bool RightOf(Edge* e, Vertex* z)
    {
        // Return true if the point is right of the oriented line defined by the edge
        LOG(INFO) << "in right of" << e;
        return ccw(z, e->getDest(), e->getOrigin());
    }

    bool InCircle(Vertex* a, Vertex* b, Vertex* c, Vertex* d)
    {
        // Returns true if d is in the circle circumscribing the triangle [abc]
        // This reduces to a linear algebraic question; see Guibas and Stolfi

        // Set up our matrix
        double m[4][4] = {{a->x, b->x, c->x, d->x},
                          {a->y, b->y, c->y, d->y},
                          {a->lengthSquared(), b->lengthSquared(), c->lengthSquared(), d->lengthSquared()},
                          {1, 1, 1, 1}};

        // Return true if our determinant is positive
        return Det4x4(m[0], m[1], m[2], m[3]) > 0;
    }
    void deleteEdge(Edge *edge)
    {
        m_edges.erase(std::remove(m_edges.begin(), m_edges.end(), edge), m_edges.end());

        splice(edge, edge->oPrev());
        splice(edge->sym(), edge->sym()->oPrev());

    }

Edge *locate(Vertex *x, Edge *e)
{
    std::vector<Edge *> processed;
    // E = Locate(x)
    while (true)
    {
        if ((std::find(processed.begin(), processed.end(), e) != processed.end()))
        {
            return e;
        }
        else
        {

            LOG(INFO) << "in while loop";
            LOG(INFO) << "edge: " << e->getOrigin() << " " <<  e->getDest();
            if (x == e->getOrigin() || x == e->getDest())
            {
                LOG(INFO) << "found e";

                return e;
            }
            else if (RightOf(e, x))
            {
                LOG(INFO) << "RightOf";

                processed.push_back(e);
                e = e->sym();
            }
            else if (!RightOf(e->oNext(), x))
            {
                LOG(INFO) << "!RightOf";

                processed.push_back(e);
                e = e->oNext();
            }
            else if (!RightOf(e->dPrev(), x))
            {
                LOG(INFO) << "!RightOf prev";

                processed.push_back(e);
                e = e->dPrev();
            }
            else
            {
                LOG(INFO) << "else";

                return e;
            }
        }
    }
}


void swapEdge(Edge *e)
{
    Edge *a = e->oPrev();
    Edge *b = e->sym()->oPrev();

    splice(e, a);
    splice(e->sym(), b);
    splice(e, a->lNext());
    splice(e->sym(), b->lNext());

    e->setOrigin(a->getDest());
    e->setDest(b->getDest());
}
private:
    Edge* m_startingEdge;
};
