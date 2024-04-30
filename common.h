

#pragma once

#include "easylogging++.h"

#ifndef _GEOMETRY_H_
#include "Geometry.h"
#endif

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
double Det3x3(double* col_0, double* col_1, double* col_2);
double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3);
bool ccw(Vertex* a, Vertex* b, Vertex* c);
void splice(Edge *e1, Edge *e2);
Edge *makeQuadEdge(std::vector<QuadEdge *> &edgeList);
void deleteEdge(Edge *edge, std::vector<QuadEdge *> &edgeList);double Det3x3(double *col_0, double *col_1, double *col_2);
double Det4x4(double *col_0, double *col_1, double *col_2, double *col_3);bool ccw(Vertex *a, Vertex *b, Vertex *c);
bool LeftOf(Edge *e, Vertex *z);
bool RightOf(Edge *e, Vertex *z);
bool InCircle(Vertex *a, Vertex *b, Vertex *c, Vertex *d);