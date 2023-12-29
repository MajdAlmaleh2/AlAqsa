
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include "texture.h"
#include "camera.h"
#include "PrimitiveDrawer.h"
#include "Point.h"

#include <fstream>




HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Cntext
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

Camera MyCamera;
//
int  image, image2;
PrimitiveDrawer r = PrimitiveDrawer();

// skybox
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN;

GLUquadric *quadric = gluNewQuadric();

void drawUnitCube() {

	glBegin(GL_QUADS);

	//Front Face
	glNormal3f(0, 0, 1);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);

	//Back Face
	glNormal3f(0, 0, -1);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);

	//Right Face
	glNormal3f(1, 0, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);

	//Left Face
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);

	//Top Face
	glNormal3f(0, 1, 0);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);

	//Bottom Face
	glNormal3f(0, -1, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);

	glEnd();
}

void drawMyCube(float xPos, float yPos, float zPos,
	float xSize, float ySize, float zSize,
	float xRot, float yRot, float zRot) {

	glPushMatrix();

	glTranslatef(xPos, yPos, zPos);
	glRotatef(xRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);
	glRotatef(zRot, 0, 0, 1);
	glScalef(xSize, ySize, zSize);
	drawUnitCube();

	glPopMatrix();

}



void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SKYFRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SKYBACK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SKYLEFT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SKYRIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SKYUP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SKYDOWN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glColor3f(1, 1, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void Key(bool* keys, float speed)
{
	if (keys['S'])
		MyCamera.RotateX(-1 * speed);
	if (keys['W'])
		MyCamera.RotateX(1 * speed);
	if (keys['D'])
		MyCamera.RotateY(-1 * speed);
	if (keys['Z'])
		MyCamera.RotateZ(1 * speed);
	if (keys['X'])
		MyCamera.RotateZ(-1 * speed);
	if (keys['A'])
		MyCamera.RotateY(1 * speed);
	if (keys[VK_UP])
		MyCamera.MoveForward(1 * speed);
	if (keys[VK_DOWN])
		MyCamera.MoveForward(-1 * speed);
	if (keys[VK_RIGHT])
		MyCamera.MoveRight(1 * speed);
	if (keys[VK_LEFT])
		MyCamera.MoveRight(-1 * speed);
	if (keys['O'])
		MyCamera.MoveUpward(1 * speed);
	if (keys['L'])
		MyCamera.MoveUpward(-1 * speed);
	
}
int image3, leftSideMosque1, leftSideMosque, topMosque,bottomMosque, rightSideMosque, frontSideMosque, ramp, rightSideMosqueFront1Front, rightSideMosqueFront1right, rightSideMosqueFront2front,
rightSideMosqueFront3front, roofTop, roofSide, roofSideRotated, frontFront, doomSphere, sidePrayer, sideMusiam
, frontMusiam, frontMusiam2,azan, azanRotated,darkWall;
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations



	glEnable(GL_TEXTURE_2D);

	image = LoadTexture("back.bmp", 255);
	leftSideMosque1 = LoadTexture("leftSideMosque1.bmp", 255);
	leftSideMosque = LoadTexture("leftSideMosque.bmp", 255);
	topMosque = LoadTexture("topMosque.bmp", 255);
	bottomMosque = LoadTexture("carpet.bmp", 255);
	rightSideMosque = LoadTexture("rightSideMosque.bmp", 255);
	frontSideMosque = LoadTexture("frontSideMosque.bmp", 255);
	rightSideMosqueFront1Front = LoadTexture("rightSideMosqueFront1Front.bmp", 255);
	rightSideMosqueFront1right = LoadTexture("rightSideMosqueFront1right.bmp", 255);
	rightSideMosqueFront2front = LoadTexture("wall2.bmp", 255);
	darkWall = LoadTexture("wall.bmp", 255);

	rightSideMosqueFront3front = LoadTexture("rightSideMosqueFront3front.bmp", 255);
	sideMusiam = LoadTexture("sideMusiam.bmp", 255);
	frontMusiam = LoadTexture("frontMusiam.bmp", 255);
	frontMusiam2 = LoadTexture("frontMusiam2.bmp", 255);
	azan = LoadTexture("azan.bmp", 255);
	azanRotated = LoadTexture("azanRotated.bmp", 255);








	sidePrayer = LoadTexture("sidePrayer.bmp", 255);
	doomSphere = LoadTexture("doomSphere.bmp", 255);
	frontFront = LoadTexture("frontFront.bmp", 255);
	roofSideRotated = LoadTexture("roofSideRotated.bmp", 255);
	roofTop = LoadTexture("roofTop.bmp", 255);
	roofSide = LoadTexture("roofSide.bmp", 255);



	ramp = LoadTexture("ramp.bmp", 255);
	image2 = LoadTexture("DU icon.bmp");
	image3 = LoadTexture("building.bmp", 255);

	// skybox
	SKYFRONT = LoadTexture("front.bmp", 255);
	SKYBACK = LoadTexture("back.bmp", 255);
	SKYLEFT = LoadTexture("left.bmp", 255);
	SKYRIGHT = LoadTexture("right.bmp", 255);
	SKYUP = LoadTexture("up.bmp", 255);
	SKYDOWN = LoadTexture("down.bmp", 255);
	// note if you load a image the opengl while on the GL_Texture_2D himself
	glDisable(GL_TEXTURE_2D);
	
	MyCamera = Camera();
	MyCamera.RotateY(-90);
	MyCamera.Position.x = -40;
	MyCamera.Position.y = 10;
	MyCamera.Position.z = 20;
	return TRUE;										// Initialization Went OK
}




void doom()
{

	Point w = Point(-8, 6, 8.5);//
	Point b = Point(-8, 0, 8.5);
	Point d = Point(-8, 6, -8.5);
	Point z = Point(-8, 0, -8.5);

	Point a = Point(-8, 6, 8);
	Point x = Point(-8, 0, 8);//
	Point m = Point(-8, 0, -8.5);
	Point v = Point(-8, 6, -8.5);
	//r.DrawQuad(d, j7, g, z);r.DrawQuad(e6, f, b1, l7);
	Point j = Point(12, 6, 8.5);//
	Point i = Point(12, 0, 8.5);//
	Point j7 = Point(16, 6, -8.5);
	Point g = Point(16, 0, -8.5);
	Point f = Point(16, 0, 8);
	Point e6 = Point(16, 6, 8);
	//j7 e6 f g 
	Point l7 = Point(-8, 6, 8);
	Point b1 = Point(-8, 0, 8);
	//e6 f b1 l7
	//PrimitiveDrawer r = PrimitiveDrawer();
	//glColor3f(1, 0, 0);
	////a x m v
	//r.DrawQuad(d, w, b, z);
	//glColor3f(1, 0, 0);
	//r.DrawQuadWithTexture(w, a, x, b, image);
	//glColor3f(1, 1, 0);
	//r.DrawQuad(v, d, z, m);
	//a w m v
	//glColor3f(0, 1, 0);
	//r.DrawQuad(v, a, w, d);
	//glColor3f(0, 0, 1);
	//z m x b 
//	r.DrawQuad(z, m, x, b);
	//glColor3f(1, 0, 0);
	//front
	r.DrawQuadWithTexture(a, x, m, v, frontSideMosque);
	//glColor3f(1, 0, 1);
	//r.DrawQuad(w, j, i, x);
	//glColor3f(1, 1, 0);
	//back
	r.DrawQuadWithTexture( e6,f, g, j7   , leftSideMosque);
	//left
	r.DrawQuadWithTexture(e6, f, b1, l7, leftSideMosque);
	//w b j7 g
	//right
	r.DrawQuadWithTexture( d,z , g, j7, rightSideMosque);
	//rooooooooooooooooooooooooooof
//	glColor3f(0, 1, 0);
	// e6 l7 w7 j7
	//roof
	r.DrawQuadWithTexture( d,l7,  e6, j7 , ramp);
	//b1 z g f
	//groundddddddddddddddd
	r.DrawQuadWithTexture(b1, z, g, f, bottomMosque);
	Point a2 = Point(-4, 8, 2);
	Point k2 = Point(-4, 9, 0);
	Point j2 = Point(12, 9, 0);
	Point b2 = Point(12, 8, 2);
	//rampppppp
	//glColor3f(.5, .5, .5);
	r.DrawQuadWithTexture(a2, k2, j2, b2, ramp);
	//j2
	Point c2 = Point(12, 8, -2);
	Point d2 = Point(-4, 8, -2);
	r.DrawQuadWithTexture(j2, c2, d2, k2, ramp);
	//d2 k2 a2 
	//glColor3f(.5, .2, .9);
	r.DrawTrWithTexture(d2, k2, a2, ramp);//b2 j2 c2
	r.DrawTrWithTexture(b2, j2, c2, ramp);
	Point s11 = Point(-7.62, 2.5, 28.5);
	Point n11 = Point(-7.62, 3.73, 28.5);
	Point q8 = Point(10, 3.75, 28.5);
	Point p8 = Point(10, 2.5, 28.5);
	//glColor3f(0, 1, 0);
	//musiuam roof quads
	r.DrawQuadWithTexture(s11, n11, q8, p8, rightSideMosqueFront2front);
	Point r11 = Point(-7.62, 2.5, 24.5);
	Point e9 = Point(-7.62, 3.75, 24.5);
	r.DrawQuadWithTexture(s11, r11, e9, n11, rightSideMosqueFront2front);
}


void remaster()
{

	Point e7 = Point(6.5, 0, -10);
	//glColor3f(1, 0, 0);

	Point c7 = Point(6.5, 5, -10);
	//glColor3f(1, 1, 0);
	//r.QuadWithHigh(c7, 3.5, 1.5, 2);
	r.QuadWithHighAndTexture(e7, 3.5, 6, 4, rightSideMosqueFront1Front, rightSideMosqueFront1Front, rightSideMosqueFront1right, rightSideMosqueFront1right, rightSideMosqueFront2front, rightSideMosqueFront2front);
	Point q7 = Point(10, 0, -11);//4 
	
	r.QuadWithHighAndTexture(q7, 4, 3.75, 3, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front);
	
	Point d11 = Point(10, 3.75, -11);
	//r.QuadWithHigh(d11, 1, 1.25, 2);
	r.QuadWithHighAndTexture(d11, 1, 1.25, 2, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front);

	Point f6 = Point(13, 3.75, -11);
	//r.QuadWithHigh(f6, 1, 1.25, 2);
	r.QuadWithHighAndTexture(f6, 1, 1.25, 2, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front);


	Point v6 = Point(14, 0, -17.5);
	r.QuadWithHighAndTexture(v6, 2, 3, 9, rightSideMosqueFront2front, rightSideMosqueFront3front, rightSideMosqueFront3front, rightSideMosqueFront3front, rightSideMosqueFront2front, rightSideMosqueFront2front);

	Point d1 = Point(-4, 6, -8);//y=1 z=16 x=4
	
	r.QuadWithHighAndTexture(d1, 16, 1, 12, roofSide, roofTop, roofSideRotated, roofSideRotated, roofTop, roofSide);
	Point h1 = Point(-4, 7, -2);//y1 z4 x

	//glColor3f(1, 0, 0);
	r.QuadWithHighAndTexture(h1, 16, 1, 4, roofSide, roofTop, roofSideRotated, roofSideRotated, roofTop, roofSide);
	Point z3 = Point(-8, 6, -2.2);//1 4 .3
	//glColor3f(1, 0, 0);
	r.QuadWithHighAndTexture(z3, .3, 1, 3.89,frontFront, frontFront, frontFront, frontFront, frontFront, frontFront);
	
	Point u3 = Point(-7.5, 6, -1);//.5 2 4 
	r.QuadWithHighAndTexture(u3, 3.5, .5, 2, roofSide, roofTop, roofSideRotated, roofSideRotated, roofTop, roofSide);
	Point l = Point(12, 6, -4);
	r.QuadWithHighAndTexture(l, 4, 1.5, 12, roofSide, roofTop, roofSideRotated, roofSideRotated, roofTop, roofSide);
	Point w4 = Point(-5.85, 6.5, 0);
	r.drawSphereWithTexture(w4, .9, 50, 50, rightSideMosqueFront2front);

	Point f2 = Point(14, 8, 0);
	//glColor3f(1, 0, 0);
	r.drawSphereWithTexture(f2, 1.9, 50, 50, ramp);
	Point j1 = Point(14, 7, 0);
	//glColor3f(1, 1, 1);
	r.drawCylinderWithTexture(j1, 1.9, 1.0, 50, roofSide);

//	glColor3f(1, 0, 0);
	//Point n3 = Point(-8, 0, -8.5);
	//r.QuadWithHigh(n3, 4, 6, .5);
	glPushMatrix();
	glTranslated(0, 0, -0.5);
	//
	Point b6 = Point(10, 0, 8.5);
	r.QuadWithHighAndTexture(b6, 6, 5, 16.5, rightSideMosqueFront2front, rightSideMosqueFront2front , rightSideMosqueFront2front , sidePrayer, rightSideMosqueFront2front, rightSideMosqueFront2front);
	//glColor3f(.4, 0, 0);
	//
	Point c8 = Point(10, 0, 25);
	r.QuadWithHighAndTexture(c8, 6, 5, 4, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front);
	//glColor3f(.5, 0, .7);
	//glColor3f(1, 0, 0);
	Point g9 = Point(-7.62, 0, 18.81);
	r.QuadWithHighAndTexture(g9, 17.62, 3.75, 6.19, sideMusiam, rightSideMosqueFront2front, sideMusiam, frontMusiam, rightSideMosqueFront2front, rightSideMosqueFront2front);
	//glColor3f(.5, .1, .9);
//	glColor3f(1, 0, 0);
	Point u11 = Point(-7.62, 0, 25);
	r.QuadWithHighAndTexture(u11, 17.62, 2.5, 4, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, frontMusiam2 , rightSideMosqueFront2front, rightSideMosqueFront2front);
	//glColor3f(.5, .1, .9);
	//glColor3f(1, 0, 0);
	Point b9 = Point(10, 5, 17.78);
	r.QuadWithHighAndTexture(b9, 2.06, 7.18, 2.06, azan, azanRotated, azanRotated, azanRotated, azan, azan);

	Point m11 = Point(1.19, 3, 21.91);
	//glColor3f(1, 0, 0);
	r.drawSphereWithTexture(m11, 2, 50, 50, darkWall);
	Point s10 = Point(1.19, 3.75, 21.91);
//	glColor3f(1, 0, 0);
//	r.drawCylinder(s10, 2, 1.7, 50);

	//glColor3f(1, 0, 1);
	Point o10 = Point(11.03, 12.8, 18.81);
	r.drawSphereWithTexture(o10, .6, 50, 50,ramp);
	Point o11 = Point(11.03, 11.5, 18.81);
	//glColor3f(1, 1, 1);
	r.QuadWithHighAndTexture(u11, 17.62, 2.5, 4, rightSideMosqueFront2front, rightSideMosqueFront2front, rightSideMosqueFront2front, frontMusiam2 , rightSideMosqueFront2front, rightSideMosqueFront2front);
	r.drawCylinderWithTexture(o11, .6, .4, 50, rightSideMosqueFront2front);

	glPopMatrix();

}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	MyCamera.Render();
	Key(keys, 0.2);
	
//	Draw_Skybox(0, 0, 0, 100, 100, 100);

	
	glPushMatrix();
	doom();
	remaster();
	glPopMatrix();

	glPushMatrix();

	glPopMatrix();
    
/*	glEnable(GL_TEXTURE_2D);
     	glBindTexture(GL_TEXTURE_2D, image2);
           drawMyCylinder(quadric, 0.0f, 0.0f, 0.0f, 1, 1, 5, 90.0f, 0.0f, 0.0f);
	glDisable(GL_TEXTURE_2D);*/

	
	//
	
	/*glPushMatrix();
	glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, image3);
				auxSolidSphere(1);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
*/
	/*glPushMatrix();
	    glRotated(a,0,1,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, image3);    
			glBegin(GL_QUADS);
			glTexCoord2f(-1, -1);
			glVertex3f(5, -5, 0);
			glTexCoord2f(-1, 0);
			glVertex3f(5, 5, 0);


			glTexCoord2f(0, 0); 
			
			glVertex3f(-5, 5, 0);
			glTexCoord2f(0, -1); 
			glVertex3f(-5, -5, 0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	a+=0.01f;*/

	
	
	
	//glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, image);
	//		glBegin(GL_QUADS);
	//			glTexCoord2f(0, 0);       glVertex3f(5, -5, 0);
	//			glTexCoord2f(-1, 0);       glVertex3f(5, 5, 0);
	//			glTexCoord2f(-1, -1);       glVertex3f(-5, 5, 0);
	//			glTexCoord2f(0, -1);       glVertex3f(-5, -5, 0);
	//		glEnd();
	//glDisable(GL_TEXTURE_2D);
	
	/*glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, image2);
			glBegin(GL_QUADS);
				glTexCoord2f(0.5, 0);       glVertex3f(5, -5, 0);
				glTexCoord2f(0.5, 0.5);       glVertex3f(5, 5, 0);
				glTexCoord2f(0, 0.5);       glVertex3f(-5, 5, 0);
				glTexCoord2f(0, 0);       glVertex3f(-5, -5, 0);
			glEnd();
	glDisable(GL_TEXTURE_2D);*/

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL template", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
													if (!HIWORD(wParam))					// Check Minimization State
													{
														active = TRUE;						// Program Is Active
													}
													else
													{
														active = FALSE;						// Program Is No Longer Active
													}

													return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
													switch (wParam)							// Check System Calls
													{
													case SC_SCREENSAVE:					// Screensaver Trying To Start?
													case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
														return 0;							// Prevent From Happening
													}
													break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
													PostQuitMessage(0);						// Send A Quit Message
													return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
													keys[wParam] = TRUE;					// If So, Mark It As TRUE
													return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
													keys[wParam] = FALSE;					// If So, Mark It As FALSE
													return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
													ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
													return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Example", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("OpenGL template", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

