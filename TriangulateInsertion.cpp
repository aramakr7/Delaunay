#ifndef _GEOMETRY_H_
#include "Geometry.h"
#endif

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

static void splice(Edge *e1, Edge *e2);
static Edge *makeQuadEdge(std::vector<QuadEdge *> &edgeList);
static void deleteEdge(Edge *edge, std::vector<QuadEdge *> &edgeList);

static double Det3x3(double *col_0, double *col_1, double *col_2);
static double Det4x4(double *col_0, double *col_1, double *col_2, double *col_3);

static bool ccw(Vertex *a, Vertex *b, Vertex *c);
static bool LeftOf(Edge *e, Vertex *z);
static bool RightOf(Edge *e, Vertex *z);
static bool InCircle(Vertex *a, Vertex *b, Vertex *c, Vertex *d);


Edge* locate(Vertex *x, Edge *e)
{
    std::vector<Edge*> processed;
    // E = Locate(x)
    while (true)
    {
        std::cout << "in while loop" << std::endl;
        std::cout << "e: " << e->getOrigin()->x << ", " << e->getOrigin()->y << std::endl;

        if (x == e->getOrigin() || x == e->getDest())
        {
            std::cout << "found e" << std::endl;

            return e;
        }
        else if (RightOf(e, x))
        {
            std::cout << "RightOf" << std::endl;

            e = e->sym();
            Edge* temp = e;
            processed.push_back(temp);

        }
        else if (!RightOf(e->oNext(), x))
        {
            std::cout << "!RightOf" << std::endl;

            e = e->oNext();
        }
        else if (!RightOf(e->dPrev(), x))
        {
            std::cout << "!RightOf prev" << std::endl;

            e = e->dPrev();
        }
        else
        {
            std::cout << "else" << std::endl;

            return e;
        }
    }
}

Edge* connect(Edge *e1, Edge *e2, std::vector<QuadEdge *> &edgeList)
{
    Edge *e = makeQuadEdge(edgeList);
    e->setOrigin(e1->getDest());
    e->setDest(e2->getOrigin());

    // Perform splice operations -- I'm still not quite sure why
    splice(e, e1->lNext());
    splice(e->sym(), e2);

    // Return a pointer to our new edge
    return e;
}

Edge* makeEdge(Vertex *a, Vertex *b, std::vector<QuadEdge *> &edgeList)
{
    Edge *e = makeQuadEdge(edgeList);
    e->setOrigin(a);
    e->setDest(b);

    return e;
}

Edge* makeTriangle(Vertex *a, Vertex *b, Vertex *c, std::vector<QuadEdge *> &edgeList)
{
    Edge *e1 = makeEdge(a, b, edgeList);
    Edge *e2 = makeEdge(b, c, edgeList);

    splice(e1->sym(), e2);

    Edge* e3 = makeEdge(c, a, edgeList);
    splice(e2->sym(), e3);
    splice(e3->sym(), e1);


    return e1;
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

void insertSite(Vertex *x, std::vector<QuadEdge *> &edgeList, Edge* startingEdge)
{
    Edge *e = locate(x, startingEdge);
    std::cout << "located point" << std::endl;

    if (x == e->getOrigin() || x == e->getDest())
    {
        // X is origin or destination of edge -> ignore + return
        std::cout << "point was org or dest" << std::endl;
        return;
    }
    else if (e->hasPoint(x))
    {
        // X lies on edge e
        std::cout << "point on edge" << std::endl;
        e = e->oPrev();
        deleteEdge(e->oNext(), edgeList);
    }

    Edge *base = makeQuadEdge(edgeList);
    base->setOrigin(e->getOrigin());
    base->setDest(e->getDest());

    splice(base, e);
    startingEdge = base;

    do
    {
        base = connect(e, base->sym(), edgeList);
        e = base->oPrev();

    } while (e->lNext() != startingEdge);

    while (true)
    {
        Edge* t = e->oPrev();
        if (RightOf(e, t->getDest()) &&
            InCircle(e->getOrigin(), t->getDest(), e->getDest(), x))
            {
                swapEdge(e);
                e = e->oPrev();
            }
        else if (e->oNext() == startingEdge)
        {
            return;
        }
        else
        {
            e = e->oNext()->lPrev();
        }

    }
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

int main()
{
    std::vector<Vertex *> points = makePoints(5);
    std::vector<Vertex *> unprocessedPoints;
    std::vector<QuadEdge *> edgeList;
    Edge *startingEdge;

    for (int i = 0; i < points.size(); i++)
    {

        if (i == 2)
        {
            startingEdge = makeTriangle(points[0], points[1], points[2], edgeList);
            std::cout << "made triangle" << std::endl;
            std::cout << startingEdge->getOrigin()->x << ", " << startingEdge->getOrigin()->y << std::endl;

        }
        else if (i > 2)
        {
            for (QuadEdge *e : edgeList)
            {
                std::cout << "point: " << i + 1 << std::endl;
                std::cout << points[i]->x << ", " << points[i]->y << std::endl;
                insertSite(points[i], edgeList, startingEdge);
            }
        }
    }



    return 0;
}


static void splice(Edge* e1, Edge* e2)
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

static Edge* makeQuadEdge(std::vector<QuadEdge*>& edgeList)
{
    edgeList.push_back(new QuadEdge());
    return edgeList.back()->edges;
}

static void deleteEdge(Edge* edge, std::vector<QuadEdge*>& edgeList)
{
    splice(edge, edge->oPrev());
    splice(edge->sym(), edge->sym()->oPrev());

    QuadEdge* raw = (QuadEdge*)(edge - (edge->getIndex()));
	edgeList.erase(std::remove(edgeList.begin(), edgeList.end(), raw));
	delete raw;
}


// https://github.com/jtwaugh/Delaunay/blob/master/linal.h
static double Det3x3(double* col_0, double* col_1, double* col_2)
{
	// Gets the determinant of a 3x3 matrix, where the arguments are 3-long column vectors

	// Names all the objects in the matrix for my convenience
	double a = col_0[0];
	double b = col_1[0];
	double c = col_2[0];
	double d = col_0[1];
	double e = col_1[1];
	double f = col_2[1];
	double g = col_0[2];
	double h = col_1[2];
	double i = col_2[2];

	// Return the alternating sum of the 2x2 determinants of the coproducts
	double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
	return det;
}

static double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3)
{
	// Gets the determinant of a 4x4 matrix, where the arguments are 4-long column vectors

	// Name all the objects in the matrix for my convenience
	double a = col_0[0];
	double b = col_1[0];
	double c = col_2[0];
	double d = col_3[0];
	double e = col_0[1];
	double f = col_1[1];
	double g = col_2[1];
	double h = col_3[1];
	double i = col_0[2];
	double j = col_1[2];
	double k = col_2[2];
	double l = col_3[2];
	double m = col_0[3];
	double n = col_1[3];
	double o = col_2[3];
	double p = col_3[3];

	// Compute 3x3 determinants
	double adet = a * ((f * k * p) - (f * l * o) - (g * j * p) + (g * l * n) + (h * j * o) - (h * k * n));
	double bdet = b * ((e * k * p) - (e * l * o) - (g * i * p) + (g * l * m) + (h * i * o) - (h * k * m));
	double cdet = c * ((e * j * p) - (e * l * n) - (f * i * p) + (f * l * m) + (h * i * n) - (h * j * m));
	double ddet = d * ((e * j * o) - (e * k * n) - (f * i * o) + (f * k * m) + (g * i * n) - (g * j * m));

	// Return their alternating sum
	double det = adet - bdet + cdet - ddet;
	return det;
}

static bool ccw(Vertex* a, Vertex* b, Vertex* c)
{
	// Returns true if c lies above the line through a and b
	// Bear in mind that this is mirrored when rendering because of SFML conventions
	// This reduces to a linear algebraic question; see Guibas and Stolfi

	// This following bit might not be necessary
    std::cout << "CCW" << std::endl;
	float a_x = a->x;
	float a_y = a->y;
	float b_x = b->x;
	float b_y = b->y;
	float c_x = c->x;
	float c_y = c->y;

	// Set up a matrix
	double m[3][3] = { { a_x, b_x, c_x }, { a_y, b_y, c_y }, { 1, 1, 1 } };

	// Return true if our determinant is positive
	return Det3x3(m[0], m[1], m[2]) > 0;
}

static bool LeftOf(Edge* e, Vertex* z)
{
	// Return true if the point is left of the oriented line defined by the edge
	return ccw(z, e->getOrigin(), e->getDest());
}

static bool RightOf(Edge* e, Vertex* z)
{
	// Return true if the point is right of the oriented line defined by the edge
    std::cout << "in right of" << std::endl;
	return ccw(z, e->getDest(), e->getOrigin());
}

static bool InCircle(Vertex* a, Vertex* b, Vertex* c, Vertex* d)
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