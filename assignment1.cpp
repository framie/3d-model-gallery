//  ========================================================================
//  FILE NAME: assignment1.cpp
//  AUTHOR:    David Mayo - dam106 (35120341)
//  See Report.pdf for details.
//  ========================================================================

// For Testing with cout << msg << endl;
#include <iostream>
using namespace std;

#include <cmath>
#include <GL/freeglut.h>
#include "loadBMP.h"

GLuint txId[4];   //Texture ids
GLUquadric *q;

float white[4] = { 1.0, 1.0, 1.0, 1.0 };

int cam_hgt = 14;
float theta = 0;
float angle = 0, eye_x = 0, eye_z = 100; 
float look_x = eye_x + 100 * sin(angle);
float look_z = eye_z - 100 * cos(angle);
float look_y = cam_hgt;

float spinnerVal = 0;
float cradleVal = 20;
float cradleSpeed;
float helicopterHeight = 0;
float helicopterVal = 0;
float propellorSpeed = 0;
float helicopterDirection = 1;
float fishVal = 0;
float fishStep = 0.2;
float fishPos = 0;

float angStep = 10.0*3.1415926 / 180.0;  //Rotate in 10 deg steps (converted to radians)
const int N = 48;
float vx[N] = { 0.0, 5.0, 5.0, 4.5, 4.5, 3.5, 4.0, 3.9, 3.8, 3.7, 3.6, 3.5, 3.4, 3.3, 3.2, 3.1, 3.0, 
				2.9, 2.8, 2.7, 2.6, 2.5, 2.4, 2.3, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 
				3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 3.5, 4.5, 4.5, 5.0, 5.0, 0.0 };
float vy[N] = { 0.0, 0.0, 1.0, 1.0, 1.5, 1.5, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 
				4.3, 4.6, 4.9, 5.2, 5.5, 5.9, 6.4, 7.0, 7.5, 7.9, 8.2, 8.5, 8.8, 9.1, 9.4, 9.6, 9.8, 
				10.0, 10.2, 10.4, 10.6, 10.8, 11.0, 11.2, 11.4, 11.9, 11.9, 12.4, 12.4, 13.4, 13.4 };
float vz[N] = { 0 };

const int N2 = 18;
float v2x[N2] = { 0, 0.2, 0.4, 0.6, 0.9, 1.2, 1.5, 1.9, 2.3, 2.8, 3.3, 3.9, 4.5, 5.2, 5.2, 1.0, 1.0, 0.0};
float v2y[N2] = { 0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 8.5, 8.5, 10.0, 10.0};
float v2z[N2] = { 0 };

void loadTexture()
{
	glGenTextures(1, txId);   //Get 2 texture IDs 
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	//loadBMP("brick.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadBMP("ground.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture name for the following OpenGL texture
	loadBMP("roof.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture name for the following OpenGL texture
	loadBMP("sky.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/

}

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5, 0.);			//Floor colour

	for (int i = -50; i <= 50; i++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	}
}

//-- Ground Plane --------------------------------------------------------
void floor()
{
	float black[4] = { 0 };
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBegin(GL_QUADS);
	for (int i = -100; i < 100; i += 2)
	{
		for (int j = -50; j < 150; j += 2)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j + 2);
			glVertex3f(i + 2, 0, j + 2);
			glVertex3f(i + 2, 0, j);
		}
	}
	glEnd();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

void floor2()
{
	glColor3d(0.6, 0.25, 0.25);  //replace with a texture

	//glBindTexture(GL_TEXTURE_2D, txId[1]);

	glBegin(GL_QUADS);
	//glTexCoord2f(0.0, 16.0); 
	glVertex3f(-100, -1, -50);

	//glTexCoord2f(0.0, 0.0); 
	glVertex3f(-100, -1, 150);

	//glTexCoord2f(16.0, 0.0); 
	glVertex3f(100, -1, 150);

	//glTexCoord2f(16.0, 16.0); 
	glVertex3f(100, -1, -50);
	glEnd();
}


//-- Spinner Fucntions -----------------------------------------------------

void spinnerBase(float size)
{
	glColor4f(0.6, 0.6, 0.9, 1.0);
	glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glutSolidTorus(0.45 * size, 1 * size, 6, 20);
	glPopMatrix();

	glColor4f(0.2, 0.2, 0.2, 1.0);
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glutSolidSphere(0.6 * size, 20, 20);
	glPopMatrix();
	
	//
}

void spinnerSides(float size)
{
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.2 * size, 0.6 * size, 6, 20);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.9, 1.0);
	glPushMatrix();
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.4 * size, 1 * size, 6, 20);
	glPopMatrix();

	//

	glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
	glRotatef(120, 0, 1, 0);
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.2 * size, 0.6 * size, 6, 20);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.9, 1.0);
	glPushMatrix();
	glRotatef(120, 0, 1, 0);
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.4 * size, 1 * size, 6, 20);
	glPopMatrix();

	//

	glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
	glRotatef(240, 0, 1, 0);
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.2 * size, 0.6 * size, 6, 20);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.9, 1.0);
	glPushMatrix();
	glRotatef(240, 0, 1, 0);
	glTranslatef(size * 2.3, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.4 * size, 1 * size, 6, 20);
	glPopMatrix();
}

void displaySpinner(float size)
{
	glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(size * 0.0, size * 0.0, size * -3.8);
		glPushMatrix();
			glRotatef(spinnerVal, 0, 1, 0);
			spinnerSides(size);
		glPopMatrix();

		spinnerBase(size);
	glPopMatrix();
}

//-- Cradle Functions ------------------------------------------------------

void cradleBase(float size)
{
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
		glScalef(size * 2.0, size * 0.15, size * 3.0);
		glutSolidCube(1);
	glPopMatrix();

	// 4 supports

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.8, size * 1.25, size * 1.3);
		glScalef(size * 0.1, size * 2.4, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(size * -0.8, size * 1.25, size * 1.3);
		glScalef(size * 0.1, size * 2.4, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(size * 0.8, size * 1.25, size * -1.3);
		glScalef(size * 0.1, size * 2.4, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(size * -0.8, size * 1.25, size * -1.3);
		glScalef(size * 0.1, size * 2.4, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	// Short bars

	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * 1.3);
		glScalef(size * 1.7, size * 0.1, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * -1.3);
		glScalef(size * 1.7, size * 0.1, size * 0.1);
		glutSolidCube(1);
	glPopMatrix();

	// Long bar

	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * 0.0);
		glScalef(size * 0.1, size * 0.1, size * 2.6);
		glutSolidCube(1);
	glPopMatrix();

	// 4 static balls

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * 0.2);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * 0.2);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * -0.2);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * -0.2);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * -0.6);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * -0.6);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * 0.6);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * 0.6);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();
}

void cradleLeft(float size)
{
	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * -1.0);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * -1.0);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();
}

void cradleRight(float size)
{
	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.5, size * 1.0);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 1.0, size * 1.0, size * 2.2);
		gluCylinder(q, size * 0.02, size * 0.02, 0.8, 20, 20);
	glPopMatrix();

	glColor4f(0.5, 0.5, 0.5, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.6, size * 1.0);
		glutSolidSphere(size * 0.2, 20, 20);
	glPopMatrix();
}

void displayCradle(float size)
{
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(size * 0.0, size * 0.1, size * 0.0);
		glPushMatrix();
			if (cradleVal > 0) {
				glTranslatef(size * 0.0, size * 2.5, size * -1.0);
				glRotatef(cradleVal, 1, 0, 0);
				glTranslatef(size * 0.0, size * -2.5, size * 1.0);
			}
			cradleLeft(size);
		glPopMatrix();

		glPushMatrix();
			if (cradleVal < 0) {
				glTranslatef(size * 0.0, size * 2.5, size * 1.0);
				glRotatef(cradleVal, 1, 0, 0);
				glTranslatef(size * 0.0, size * -2.5, size * -1.0);
			}
			cradleRight(size);
		glPopMatrix();

		cradleBase(size);
	glPopMatrix();
}

void updateCradle(float speed)
{
	if (cradleVal > 0) {
		cradleSpeed -= 0.0010;
	}
	else if (cradleVal < 0) {
		cradleSpeed += 0.0010;
	}
	cradleVal += cradleSpeed * speed;
}

//-- Helicopter Functions --------------------------------------------------

void helicopterBase(float size)
{
	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
	glTranslatef(size * 0.0, size * 1.5, size * 0.0);
		glScalef(size * 1.5, size * 1.0, size * 0.7);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.3, 0.3, 1);
		glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.0);
		glScalef(size * 0.4, size * 0.4, size * 0.2);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	// Spotlight

	glColor4f(0.3, 0.3, 0.3, 1);
	glPushMatrix();
		glTranslatef(size * -1.0, size * 0.6, size * 0.0);
		glRotatef(90, -1, 0, 0);
		glutSolidCone(size * 0.2, size * 0.4, 20, 20);
	glPopMatrix();

	// Supports

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.8, size * 0.0);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 0.1, size * 0.1, size * 0.4);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 1.3, size * 1.9, size * 0.0);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 0.1, size * 0.1, size * 2.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 1.2, size * 1.0, size * 0.0);
		glRotatef(90, -0.4, 1, 0);
		glScalef(size * 0.1, size * 0.1, size * 2.3);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
		glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.0);
		glRotatef(180, 0, 0, 1);
		glScalef(size * 0.05, size * 0.05, size * 0.4);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	// Landing gear

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * -1.5, size * 0.05, size * 0.6);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 0.1, size * 0.1, size * 3.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * -0.6, size * 1.0, size * 0.4);
		glRotatef(180, 0, -0.8, 1);
		glScalef(size * 0.1, size * 0.1, size * 1.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
		glPushMatrix();
		glTranslatef(size * 0.6, size * 1.0, size * 0.4);
		glRotatef(180, 0, -0.8, 1);
		glScalef(size * 0.1, size * 0.1, size * 1.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();



	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * -1.5, size * 0.05, size * -0.6);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 0.1, size * 0.1, size * 3.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 0.6, size * 0.1, size * -0.6);
		glRotatef(180, 0, 0.8, 1);
		glScalef(size * 0.1, size * 0.1, size * 1.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * -0.6, size * 0.1, size * -0.6);
		glRotatef(180, 0, 0.8, 1);
		glScalef(size * 0.1, size * 0.1, size * 1.0);
		gluCylinder(q, 1.0, 1.0, 1.0, 20, 20);
	glPopMatrix();
}

void helicopterMain(float size)
{
	glColor4f(0.9, 0.9, 0.9, 1);
		glPushMatrix();
		glTranslatef(size * 0.0, size * 2.8, size * 0.0);
		glScalef(size * 0.2, size * 0.1, size * 0.2);
		glutSolidTorus(1.0, 0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glTranslatef(size * 0.0, size * 2.8, size * 1.5);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 3.0, size * 0.05, size * 0.2);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glRotatef(135, 0, 1, 0);
		glTranslatef(size * 0.0, size * 2.8, size * 1.5);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 3.0, size * 0.05, size * 0.2);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glRotatef(225, 0, 1, 0);
		glTranslatef(size * 0.0, size * 2.8, size * 1.5);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 3.0, size * 0.05, size * 0.2);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glRotatef(315, 0, 1, 0);
		glTranslatef(size * 0.0, size * 2.8, size * 1.5);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 3.0, size * 0.05, size * 0.2);
		glutSolidCube(1.0);
	glPopMatrix();
}

void helicopterTail(float size)
{
	glColor4f(0.9, 0.9, 0.9, 1);
		glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.4);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 0.1, size * 0.05, size * 0.1);
		glutSolidTorus(1.0, 0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 3.5, size * 1.8, size * 0.4);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 0.7, size * 0.02, size * 0.1);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.4);
		glRotatef(120, 0, 0, 1);
		glTranslatef(size * -3.2, size * -1.8, size * -0.4);

		glTranslatef(size * 3.5, size * 1.8, size * 0.4);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 0.7, size * 0.02, size * 0.1);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.4);
		glRotatef(240, 0, 0, 1);
		glTranslatef(size * -3.2, size * -1.8, size * -0.4);

		glTranslatef(size * 3.5, size * 1.8, size * 0.4);
		glRotatef(90, 1, 0, 0);
		glScalef(size * 0.7, size * 0.02, size * 0.1);
		glutSolidCube(1.0);
	glPopMatrix();
}

void displayHelicopter(float size)
{
	float lgt_pos[] = { size * -1.0, size * 0.5, size * 0.0 };
	float spotdir[] = { 0.0, -1.0, 0.0 };

	glTranslatef(0, size * helicopterHeight, 0);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
	glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos);

	helicopterBase(size);
	glPushMatrix();
		glRotatef(helicopterVal, 0, 1, 0);
		helicopterMain(size);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(size * 3.2, size * 1.8, size * 0.4);
		glRotatef(helicopterVal, 0, 0, 1);
		glTranslatef(size * -3.2, size * -1.8, size * -0.4);
		helicopterTail(size);
	glPopMatrix();
}

void updateHelicopter(float speed)
{
	if (helicopterHeight <= 0 && propellorSpeed <= 0)
	{
		helicopterDirection = 1;
	}
	if (helicopterDirection == 1 && propellorSpeed < 3.0)
	{
		propellorSpeed += 0.0002 * speed;
	}
	else if (helicopterDirection == -1 && (propellorSpeed >= 1.2 || helicopterHeight <= 0) )
	{
		propellorSpeed -= 0.0002 * speed;
	}
	if (helicopterHeight >= 6) {
		helicopterDirection = -1;
	}
	if (helicopterDirection == 1 && propellorSpeed >= 1.2)
	{
		helicopterHeight += propellorSpeed / 4000 * speed;
	}
	else if (helicopterDirection == -1 && propellorSpeed <= 2.4 && helicopterHeight >= 0)
	{
		helicopterHeight -= propellorSpeed / 2000 * speed;
	}
	helicopterVal += propellorSpeed;
}

//-- Fish Functions -----------------------------------------------------

void fishBase(float size)
{
	// Body

	glColor4f(1.0, 0.7, 0.7, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 1.5, size * 0.0);
		glScalef(size * 1.5, size * 1.0, size * 0.4);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	// Eyes

	glColor4f(0.9, 0.9, 0.9, 1);
		glPushMatrix();
		glTranslatef(size * 1.0, size * 1.8, size * 0.3);
		glScalef(size * 0.2, size * 0.2, size * 0.2);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.1, 0.1, 0.1, 1);
		glPushMatrix();
		glTranslatef(size * 1.1, size * 1.8, size * 0.45);
		glScalef(size * 0.1, size * 0.1, size * 0.05);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.9, 0.9, 0.9, 1);
	glPushMatrix();
		glTranslatef(size * 1.0, size * 1.8, size * -0.3);
		glScalef(size * 0.2, size * 0.2, size * 0.2);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glColor4f(0.1, 0.1, 0.1, 1);
	glPushMatrix();
		glTranslatef(size * 1.1, size * 1.8, size * -0.45);
		glScalef(size * 0.1, size * 0.1, size * 0.05);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	// 

	glColor4f(0.1, 0.1, 0.1, 1);
	glPushMatrix();
		glTranslatef(size * 0.9, size * 1.2, size * 0.0);
		glScalef(size * 0.6, size * 0.1, size * 0.3);
		glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}

void fishTail(float size)
{
	glColor4f(1.0, 0.7, 0.7, 1);
	glPushMatrix();
		glTranslatef(size * -2.5, size * 1.5, size * 0.0);
		glRotatef(90, 0, 1, 0);
		glScalef(size * 0.4, 1.0, 1.0);
		glutSolidCone(size * 1.0, size * 2.0, 20, 20);
	glPopMatrix();

	glColor4f(1.0, 0.7, 0.7, 1);
	glPushMatrix();
		glTranslatef(size * -2.5, size * 1.5, size * 0.0);
		glScalef(size * 0.1, 1.0, size * 0.4);
		glutSolidSphere(size * 1.0, 20, 20);
	glPopMatrix();
}

void displayFish(float size)
{
	glPushMatrix();
		glRotatef(fishPos, 0, 1, 0);
		glTranslatef(size * 0.0, size * 0.0, size * 4.0);

		glPushMatrix();
			glTranslatef(size * -1.2, size * 1.5, size * 0.0);
			glRotatef(fishVal, 0, 1, 0);
			glTranslatef(size * 1.2, size * -1.5, size * 0.0);
			fishTail(size);
		glPopMatrix();

		fishBase(size);
	glPopMatrix();
}

void updateFish(float speed)
{
	if (fishVal >= 30)
	{
		fishStep = -0.2;
	}
	else if (fishVal <= -30)
	{
		fishStep = 0.2;
	}
	fishVal += fishStep * speed;
	fishPos += 0.1 * speed;
}

//-- Stand Functions ---------------------------------------------------

void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
	ny = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
	nz = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);

	glNormal3f(nx, ny, nz);
}

void displayStand(float size)
{
	float wx[N], wy[N], wz[N];

	glPushMatrix();
		glTranslatef(0.0, size * -13.4, 0.0);
		glColor4f(0.8, 0.8, 0.8, 1);
		for (int j = 0; j < 36; j++) //36 slices in 10 deg steps
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0; i < N; i++)
			{
				normal(-wx[i - 1], -wy[i - 1], -wz[i - 1],
					vx[i - 1], vy[i - 1], vz[i - 1],
					vx[i], vy[i], vz[i]);
				glVertex3f(size * vx[i], size * vy[i], size * vz[i]);

				wx[i] = (vx[i] * cos(angStep)) + (vz[i] * sin(angStep));
				wy[i] = vy[i];
				wz[i] = (-vx[i] * sin(angStep)) + (vz[i] * cos(angStep));

				glVertex3f(size * wx[i], size * wy[i], size * wz[i]);
				vx[i] = wx[i];
				vy[i] = wy[i];
				vz[i] = wz[i];
			}
			glEnd();
		}
	glPopMatrix();
}

//-- Spinning Top Functions -------------------------------------------

void displaySpinningTop(float size)
{
	float wx[N2], wy[N2], wz[N2];

	glRotatef(spinnerVal / 2, 0, 1, 0);
	glPushMatrix();
	for (int j = 0; j < 36; j++) //36 slices in 10 deg steps
	{
		if (j % 4 == 0 || j % 4 == 1) 
		{
			 glColor4f(0.9, 0.9, 0.9, 1);
		}
		else
		{
			glColor4f(1.0, 0.7, 0.7, 1);
		}
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < N2; i++)
		{

			wx[i] = (v2x[i] * cos(angStep)) + (v2z[i] * sin(angStep));
			wy[i] = v2y[i];
			wz[i] = (-v2x[i] * sin(angStep)) + (v2z[i] * cos(angStep));

			normal(-wx[i - 1], -wy[i - 1], -wz[i - 1],
				v2x[i - 1], v2y[i - 1], v2z[i - 1],
				v2x[i], v2y[i], v2z[i]);

			glVertex3f(size * v2x[i], size * v2y[i], size * v2z[i]);

			glVertex3f(size * wx[i], size * wy[i], size * wz[i]);
			v2x[i] = wx[i];
			v2y[i] = wy[i];
			v2z[i] = wz[i];
		}
		glEnd();
	}
	glPopMatrix();
}

//-- Spinning Top Functions -------------------------------------------

void displayBuilding(float size)
{

	// Walls

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glColor4f(0.6, 0.6, 0.6, 1);
	glPushMatrix();
		glTranslatef(size * 1.2, size * 0.75, size * 1.95);
		glScalef(size * 1.55, size * 1.5, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.6, 1);
	glPushMatrix();
		glTranslatef(size * -1.2, size * 0.75, size * 1.95);
		glScalef(size * 1.55, size * 1.5, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.6, 1);
		glPushMatrix();
		glTranslatef(size * 0.0, size * 1.3, size * 1.95);
		glScalef(size * 0.85, size * 0.4, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.4, 0.2, 1);
	glPushMatrix();
		glTranslatef(size * -0.8, size * 0.55, size * 2.20);
		glRotatef(30, 0, 1, 0);
		glScalef(size * 0.85, size * 1.1, size * 0.1);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.6, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.75, size * -1.95);
		glScalef(size * 4.0, size * 1.5, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.6, 1);
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(size * 0.0, size * 0.75, size * -2.0);
		glScalef(size * 4.0, size * 1.5, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.6, 0.6, 0.6, 1);
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(size * 0.0, size * 0.75, size * 2.0);
		glScalef(size * 4.0, size * 1.5, size * 0.1);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glutSolidCube(1.0);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	// Roof

	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 1.55, size * 0.0);
		glScalef(size * 5.0, size * 0.1, size * 4.0);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 2.45, size * 0.0);
		glScalef(size * 0.2, size * 1.7, size * 4.0);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glRotatef(35, 0, 0, 1);
		glTranslatef(size * 0.3, size * 2.70, size * 0.0);
		glScalef(size * 3.0, size * 0.1, size * 4.0);
		glutSolidCube(1.0);
	glPopMatrix();

	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glRotatef(35, 0, 0, -1);
		glTranslatef(size * -0.3, size * 2.70, size * 0.0);
		glScalef(size * 3.0, size * 0.1, size * 4.0);
		glutSolidCube(1.0);
	glPopMatrix();

	
	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(size * -2.0, size * 2.15, size * 0.0);
		glScalef(size * 0.1, size * 1.2, size * 3.0);
		glutSolidTetrahedron();	glPopMatrix();	glColor4f(0.9, 0.3, 0.3, 1);
	glPushMatrix();
		glRotatef(270, 0, 1, 0);
		glTranslatef(size * -2.0, size * 2.15, size * 0.0);
		glScalef(size * 0.1, size * 1.2, size * 3.0);
		glutSolidTetrahedron();	glPopMatrix();
	// Floor
	glColor4f(0.6, 0.4, 0.2, 1);
	glPushMatrix();
		glTranslatef(size * 0.0, size * 0.005, size * 0.0);
		glScalef(size * 4.0, size * 0.01, size * 4.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.

void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, 14, eye_z + 20, look_x, look_y, look_z, 0, 1, 0);
	//gluLookAt(12 * sin(theta / 30), cam_hgt, 12 * cos(theta / 30), 0, 12, 0, 0, 1, 0);  //Camera position and orientation

	//gluLookAt(12 * sin(theta / 30), cam_hgt - 10, 12 * cos(theta / 30), 0, 0, 0, 0, 1, 0);


	float lpos[4] = { 30.0, 30.0, 50.0, 1.0 };
	float lpos2[4] = { 0.0, 15.0, 0.0, 1.0 };
	
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT2, GL_POSITION, lpos2);

			
	glColor3f(0.0, 1.0, 1.0);

	gluQuadricDrawStyle(q, GLU_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float blank[4] = { 0, 0, 0, 0 };
	

	glPushMatrix();
		glTranslatef(-20.0, 0.7 * 13.4, -10.0);
		displayStand(0.7);
		displaySpinner(0.7);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 0.7 * 13.4, -10.0);
		displayStand(0.7);
		displayCradle(1.6);
	glPopMatrix();
	

	glPushMatrix();
		glTranslatef(0.0, 0.7 * 13.4, -10.0);
		displayStand(0.7);
		displayHelicopter(1);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(10.0, 0.7 * 13.4, -10.0);
		displayStand(0.7);
		displayFish(0.8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, 0.7 * 13.4, -10.0);
		displayStand(0.7);
		displaySpinningTop(0.4);
	glPopMatrix();

	

	floor2();
		
	displayBuilding(17.0);


	glutSwapBuffers();   //Useful for animation
}

//----------------------------------------------------------------------

void initialize(void)
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };

	q = gluNewQuadric();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(q, GL_TRUE);
	loadTexture();

	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// Helicopter Spotlight
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100);

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);   //Camera Frustum
}


//--Special keyboard event callback function ---------

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		angle -= 0.1;
		theta--;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		angle += 0.1;
		theta++;
	}
	else if (key == GLUT_KEY_DOWN)
	{  //Move backward
		cam_hgt--;
		eye_x -= 0.5*sin(angle) * 2;
		eye_z += 0.5*cos(angle) * 2;
	}
	else if (key == GLUT_KEY_UP)
	{ //Move forward
		cam_hgt++;
		eye_x += 0.5*sin(angle) * 2;
		eye_z -= 0.5*cos(angle) * 2;
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{ //look up
		look_y += 5;
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
	{ //look down
		look_y -= 5;
	}
	look_x = eye_x + 100 * sin(angle);
	look_z = eye_z - 100 * cos(angle);
	glutPostRedisplay();
}
void myTimer(int value)
{
	spinnerVal += 0.6;
	updateCradle(3.0);
	updateHelicopter(3.0);
	updateFish(3.0);

	glutPostRedisplay();
	glutTimerFunc(1, myTimer, 0);
}

//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("My crib");
	initialize();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutTimerFunc(1, myTimer, 0);
	glutMainLoop();
	return 0;
}