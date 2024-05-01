
#include "easylogging++.h"

#include "QuadEdge.h"
#include "Triangulation.h"
#include "common.h"

// Creates a triangle given 3 vertices
Triangulation::Triangulation(Vertex* a, Vertex* b, Vertex* c)
{
    LOG(INFO) << "Creating triangulation";
    Edge* e1 = makeEdge();
    e1->setOrigin(a);
    e1->setDest(b);

    Edge* e2 = makeEdge();
    e2->setOrigin(b);
    e2->setDest(c);

    splice(e1->sym(), e2);

    Edge* e3 = makeEdge();
    e3->setOrigin(c);
    e3->setDest(a);
    splice(e2->sym(), e3);
    splice(e3->sym(), e1);
    LOG(INFO) << "Edge one: " << e1;
    LOG(INFO) << "Edge two: " << e2;
    LOG(INFO) << "Edge the: " << e3;
    m_edges.push_back(e1);
    m_edges.push_back(e2);
    m_edges.push_back(e3);

    this->m_startingEdge = e1;
}

void Triangulation::insertSite(Vertex* x)
{

    for (size_t i = 0; i < m_edges.size(); i++)
    {
        std::cout << "\n\t" << m_edges[i] << std::endl;
    }
    std::cout << "\n\n";

    Edge* e = locate(x, m_startingEdge);
    LOG(INFO) << "located point" << e;

    if (x == e->getOrigin() || x == e->getDest())
    {
        // X is origin or destination of edge -> ignore + return
        LOG(INFO) << "point was org or dest";
        return;
    }
    else if (e->hasPoint(x))
    {
        // X lies on edge e
        LOG(INFO) << "point on edge";
        e = e->oPrev();
        deleteEdge(e->oNext());
    }

    Edge* base = makeEdge();
    m_edges.push_back(base);

    base->setOrigin(e->getOrigin());
    base->setDest(x);

    LOG(INFO) << "Made QE" << base;

    splice(base, e);
    m_startingEdge = base;

    LOG(INFO) << "Made base";

    do
    {
        base = connect(e, base->sym());
        e = base->oPrev();

    } while (e->lNext() != m_startingEdge);

    size_t counter = 0;
    while (true)
    {
        LOG(DEBUG) << "Loop iter" << counter++;
        Edge* t = e->oPrev();
        if (RightOf(e, t->getDest()) && InCircle(e->getOrigin(), t->getDest(), e->getDest(), x))
        {
            swapEdge(e);
            e = e->oPrev();
        }
        else if (e->oNext() == m_startingEdge)
        {
            return;
        }
        else
        {
            e = e->oNext()->lPrev();
        }
    }
}
