#pragma once
#include "Point.h"
#ifndef PRIMITIVEDRAWER_H
#define PRIMITIVEDRAWER_H

class PrimitiveDrawer {
public:
	PrimitiveDrawer();
	void DrawPoint(Point point);
	void DrawLine(Point start, Point end);

	void DrawQuad(Point v1, Point v2, Point v3, Point v4);
	void QuadWithHigh(Point v1, float wedth, float size, float high);
	void DrawTr(Point v1, Point v2, Point v3);
};

#endif
