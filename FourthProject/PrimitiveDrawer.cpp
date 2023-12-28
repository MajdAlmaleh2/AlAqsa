#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>

#include "Point.h"
#include "PrimitiveDrawer.h"

#include "camera.h"

GLUquadricObj* b = gluNewQuadric();
PrimitiveDrawer::PrimitiveDrawer() {}

void PrimitiveDrawer::drawSphere(Point center, float radius, int numSlices, int numStacks) {
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);

    //const float PI = 3.14159265358979323846;

    for (int i = 0; i <= numSlices; ++i) {
        float lat0 = PI * (-0.5 + (float)(i - 1) / numSlices);
        float z0 = radius * sin(lat0);
        float zr0 = radius * cos(lat0);

        float lat1 = PI * (-0.5 + (float)i / numSlices);
        float z1 = radius * sin(lat1);
        float zr1 = radius * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numStacks; ++j) {
            float lng = 2 * PI * (float)(j - 1) / numStacks;
            float x = cos(lng);
            float y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);

            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }

    glPopMatrix();
}

void PrimitiveDrawer::drawCylinder(Point center, float radius, float height, int numSlices) {
    glPushMatrix();

    glTranslatef(center.x, center.y + 1, center.z);
    glRotated(90, 1, 0, 0);
    //const float PI = 3.14159265358979323846;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSlices; ++i) {
        float angle = 2.0 * PI * (float)i / numSlices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Bottom point
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(x, y, 0.0f);

        // Top point
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, y, height);
    }
    glEnd();

    // Draw the top and bottom circles
    glBegin(GL_POLYGON);
    for (int i = 0; i <= numSlices; ++i) {
        float angle = 2.0 * PI * (float)i / numSlices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Bottom circle
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(x, y, 0.0f);

        // Top circle
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, y, height);
    }
    glEnd();

    glPopMatrix();
}





// Draw a point at the given position.
void PrimitiveDrawer::DrawPoint(Point point) {
    glBegin(GL_POINTS);
    //  glColor3b(1, 0,0);
    glVertex3f(point.x, point.y, point.z);
    glEnd();
}

// Draw a line between the given two positions.
void PrimitiveDrawer::DrawLine(Point start, Point end) {
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}
void PrimitiveDrawer::DrawTr(Point v1, Point v2, Point v3)
{
    glBegin(GL_TRIANGLES);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glEnd();
}
void PrimitiveDrawer::DrawQuad(Point v1, Point v2, Point v3, Point v4) {
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}void PrimitiveDrawer::DrawQuadWithTexture(Point v1, Point v2, Point v3, Point v4,int image) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image);
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}









void PrimitiveDrawer::QuadWithHigh(Point v1, float width, float size, float height) {
    glBegin(GL_QUADS);
    // Bottom face
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glEnd();

    glBegin(GL_QUADS);
    // Front face
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glEnd();
    // Right face
    glBegin(GL_QUADS);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glEnd();
    // Left face
    glBegin(GL_QUADS);

    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glEnd();
    // Back face
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y + size, v1.z + height);
    glEnd();
    // Top face
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y + size, v1.z + height);

    glEnd();
}

void PrimitiveDrawer::QuadWithHighAndTexture(Point v1, float width, float size, float height, int bottomImage, int frontImage, int rightImage, int leftImage, int backImage, int topImage) {
   
    glEnable(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D, bottomImage);
    glBegin(GL_QUADS);
    // Bottom face
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,frontImage);
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // Right face

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightImage);
    glBegin(GL_QUADS);
    glVertex3f(v1.x + width, v1.y, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // Left face
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftImage);
    glBegin(GL_QUADS);

    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glVertex3f(v1.x, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // Back face

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backImage);
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y + size, v1.z + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // Top face
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, topImage);
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z + height);
    glVertex3f(v1.x + width, v1.y, v1.z + height);
    glVertex3f(v1.x + width, v1.y + size, v1.z + height);
    glVertex3f(v1.x, v1.y + size, v1.z + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
