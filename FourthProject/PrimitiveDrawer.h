#pragma once
#include "Point.h"
#ifndef PRIMITIVEDRAWER_H
#define PRIMITIVEDRAWER_H

class PrimitiveDrawer {
public:
	PrimitiveDrawer();
	void drawSphere(Point center, float radius, int numSlices, int numStacks);
	void drawSphereWithTexture(Point center, float radius, int numSlices, int numStacks, int textureID);
	void drawCylinder(Point center, float radius, float height, int numSlices);
	void drawCylinderWithTexture(Point center, float radius, float height, int numSlices, int textureID);
	void DrawPoint(Point point);
	void DrawLine(Point start, Point end);

	void DrawQuad(Point v1, Point v2, Point v3, Point v4);
	void DrawQuadWithTexture(Point v1, Point v2, Point v3, Point v4, int image);
	void QuadWithHigh(Point v1, float wedth, float size, float high);
	void QuadWithHighAndTexture(Point v1, float width, float size, float height, int bottomImage, int frontImage, int rightImage, int leftImage, int backImage, int topImage);
	void DrawTr(Point v1, Point v2, Point v3);

	void DrawTrWithTexture(Point v1, Point v2, Point v3, int image);

};

#endif
