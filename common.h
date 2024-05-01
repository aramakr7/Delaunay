

#pragma once

#include "easylogging++.h"
#include "Vertex.h"
#include "Edge.h"
#include "QuadEdge.h"

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
double Det3x3(double *col_0, double *col_1, double *col_2);
double Det4x4(double *col_0, double *col_1, double *col_2, double *col_3);
bool ccw(Vertex *a, Vertex *b, Vertex *c);

Edge *locate(Vertex *x, Edge *e);
Edge *connect(Edge *e1, Edge *e2);
void deleteEdge(Edge *edge);
void swapEdge(Edge *e);
bool LeftOf(Edge *e, Vertex *z);
bool RightOf(Edge *e, Vertex *z);
bool InCircle(Vertex *a, Vertex *b, Vertex *c, Vertex *d);