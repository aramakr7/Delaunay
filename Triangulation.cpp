
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


    splice(base, e);
    m_startingEdge = base;
    LOG(INFO) << "Made base" << base;


    do
    {
        base = connect(e, base->sym());
        LOG(INFO) << "Made edge" << base;
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

EdgeTuple Triangulation::divideAndConquer(std::vector<Vertex*> points)
{
    if (points.size() == 2)
    {
        Edge* e = makeEdge();

        e->setOrigin(points[0]);
        e->setDest(points[1]);
        m_edges.push_back(e);

        return EdgeTuple({e}, {e->sym()});
    }

    if (points.size() == 3)
    {
        LOG(INFO) << "Creating triangulation";
        Edge* e1 = makeEdge();
        e1->setOrigin(points[0]);
        e1->setDest(points[1]);

        Edge* e2 = makeEdge();
        e2->setOrigin(points[1]);
        e2->setDest(points[2]);

        splice(e1->sym(), e2);

        Edge* e3 = makeEdge();
        e3->setOrigin(points[2]);
        e3->setDest(points[0]);
        splice(e2->sym(), e3);
        splice(e3->sym(), e1);
        LOG(INFO) << "Edge one: " << e1;
        LOG(INFO) << "Edge two: " << e2;
        LOG(INFO) << "Edge the: " << e3;

        m_edges.push_back(e1);
        m_edges.push_back(e2);
        m_edges.push_back(e3);

        return EdgeTuple({e1}, {e2->sym()});
    }

    int half = points.size()/2;
    std::vector<Vertex*> pL(points.begin(), points.begin() + half);
    std::vector<Vertex*> pR(points.begin() + half, points.end());

    EdgeTuple left = divideAndConquer(pL);
    EdgeTuple right = divideAndConquer(pR);

    Edge* li = std::get<1>(left)[0];
    Edge* ri = std::get<0>(right)[0];

    Edge* lo = std::get<0>(left)[0];
    Edge* ro = std::get<1>(right)[0];

    while (true)
    {
        if (LeftOf(li, ri->getOrigin()))
        {
            li = li->lNext();
        }

        else if (RightOf(ri, li->getOrigin()))
        {
            ri = ri->rPrev();
        }
        else
        {
            break;
        }
    }

    Edge* base = makeEdge();
    base->setOrigin(ri->sym()->getDest());
    base->setDest(li->getOrigin());

    splice(base, ri->sym()->lNext());
    splice(base->sym(), li);


    if (li->getOrigin() == lo->getOrigin())
	{
		lo = base->sym();
	}
	if (ri->getOrigin() == ro->getOrigin())
	{
		ro = base;
	}

    std::cout << "Here merge" << std::endl;

    while (true)
    {
        std::cout << "Here while loop" << std::endl;

        Edge* lCand = base->sym()->oNext();
        
        if(RightOf(base, lCand->getDest()))
        {
            while(InCircle(base->getDest(), base->getOrigin(), lCand->getDest(), lCand->oNext()->getDest()))
            {
                Edge* temp = lCand->oNext();
                deleteEdge(lCand);
                lCand = temp;
            }
        }

        Edge* rCand = base->oPrev();
        if(RightOf(base, rCand->getDest()))
        {
            while(InCircle(base->getDest(), base->getOrigin(), rCand->getDest(), rCand->oPrev()->getDest()))
            {
                Edge* temp = rCand->oPrev();
                deleteEdge(rCand);
                rCand = temp;
            }
        }

        if(!RightOf(base, lCand->getDest()) && !RightOf(base, rCand->getDest()))
        {
            break;
        }

        if(RightOf(base, lCand->getDest()) 
            || (RightOf(base, rCand->getDest()) && InCircle(lCand->getDest(), lCand->getOrigin(), rCand->getOrigin(), rCand->getDest())))
        {
            Edge* tempEdge = makeEdge();
            tempEdge->setOrigin(rCand->getDest());
            tempEdge->setDest(base->sym()->getOrigin());

            splice(tempEdge, rCand->lNext());
            splice(tempEdge->sym(), base->sym());

            base = tempEdge;
        }
        else
        {

            Edge* tempEdge = makeEdge();
            tempEdge->setOrigin(base->sym()->getDest());
            tempEdge->setDest(lCand->sym()->getOrigin());

            splice(tempEdge, base->sym()->lNext());
            splice(tempEdge->sym(), lCand->sym());

            base = tempEdge;
        }

    }

    return EdgeTuple({lo}, {ro});


}