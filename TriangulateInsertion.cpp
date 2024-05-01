#include "common.h"

INITIALIZE_EASYLOGGINGPP

Edge* locate(Vertex *x, Edge *e)
{
    std::vector<Edge*> processed;
    // E = Locate(x)
    while (true)
    {
        if((std::find(processed.begin(), processed.end(), e) != processed.end()))
        {
            return e;
        }
        else
        {
        
            LOG(INFO) << "in while loop" ;
            if(e->getOrigin() != nullptr)
            {
                LOG(INFO) << "e: " << e->getOrigin()->x << ", " << e->getOrigin()->y ;
            }
            if (x == e->getOrigin() || x == e->getDest())
            {
                LOG(INFO) << "found e" ;

                return e;
            }
            else if (RightOf(e, x))
            {
                LOG(INFO) << "RightOf" ;

                e = e->sym();
                Edge* temp = e;
                processed.push_back(temp);

            }
            else if (!RightOf(e->oNext(), x))
            {
                LOG(INFO) << "!RightOf" ;

                e = e->oNext();
                Edge* temp = e;
                processed.push_back(temp);
            }
            else if (!RightOf(e->dPrev(), x))
            {
                LOG(INFO) << "!RightOf prev" ;

                e = e->dPrev();
                Edge* temp = e;
                processed.push_back(temp);
            }
            else
            {
                LOG(INFO) << "else" ;

                return e;
            }
        }
    }
}

Edge* connect(Edge *e1, Edge *e2)
{
    Edge *e = makeEdge();
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

void swapEdge(Edge* e)
{
    Edge* a = e->oPrev();
    Edge* b = e->sym()->oPrev();

    splice(e, a);
    splice(e->sym(), b);
    splice(e, a->lNext());
    splice(e->sym(), b->lNext());

    e->setOrigin(a->getDest());
    e->setDest(b->getDest());
}

std::vector<Vertex*> makePoints(int n)
{
	// Generate a field of random vertices for debug/demonstration

	srand(time(NULL));

	std::vector<std::vector<int> > buffer;

	// Build a buffer list
	for (int i = 0; i < n; i++)
	{
		std::vector<int> xy = { rand() % 512, rand() % 512 };
		buffer.push_back(xy);
	}

	// Sort it lexicographically; we need this step
	std::sort(buffer.begin(), buffer.end());
	buffer.erase(std::unique(buffer.begin(), buffer.end()), buffer.end());

    std::vector<Vertex*> points;

	// Turn it into Verts for the convenience of our algorithm
	for (int i = 0; i < buffer.size(); i++)
	{
		points.push_back(new Vertex(buffer[i][0], buffer[i][1]));
	}

    return points;
}


void deleteEdge(Edge* edge)
{
    splice(edge, edge->oPrev());
    splice(edge->sym(), edge->sym()->oPrev());

}


void InitializeLogger()
{


   el::Configurations defaultConf;
   defaultConf.setToDefault();
    // Values are always std::string
   defaultConf.set(el::Level::Info,
            el::ConfigurationType::Format, "%datetime %level %loc %msg");
    // default logger uses default configurations
    el::Loggers::reconfigureLogger("default", defaultConf);
}

int main()
{
    InitializeLogger();
    std::vector<Vertex *> points = makePoints(8);
    std::vector<Vertex *> unprocessedPoints;
    std::vector<QuadEdge *> edgeList;
    Edge *startingEdge;
    Triangulation tri(points[0], points[1], points[2]);
    LOG(INFO) << "made triangle" ;

    for (int i = 3; i < points.size(); i++)
    {
        LOG(INFO) << "point: " << i + 1 ;
        LOG(INFO) << points[i]->x << ", " << points[i]->y ;
        tri.insertSite(points[i]);
            
    }



    return 0;
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
	double m[4][4] = {	{ a->x, b->x, c->x, d->x },
						{ a->y, b->y, c->y, d->y },
						{ a->lengthSquared(), b->lengthSquared(), c->lengthSquared(), d->lengthSquared() },
						{ 1, 1, 1, 1 } };

	// Return true if our determinant is positive
	return Det4x4(m[0], m[1], m[2], m[3]) > 0;
}