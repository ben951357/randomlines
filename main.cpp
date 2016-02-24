// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k = 1;
float tx = 0.0, ty = 0.0; 
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937_64 generator(seed);

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawRandomLine() {
	std::uniform_real_distribution<double> c(-1.0, 1.0);
	double x = c(generator);
	double y = c(generator); 
	double x2 = c(generator);
	double y2 = c(generator);

	glBegin(GL_LINE_STRIP);
	glVertex2f(x,y);
	glVertex2f(x2,y2);
	glEnd();
}

void drawRandomArc() {
	std::uniform_real_distribution<double> c(-1.0, 1.0);
	std::uniform_real_distribution<double> degree(0.0, 360.0);

	double x = c(generator);
	double y = c(generator);
	double r = c(generator);

	double start = degree(generator);
	if (start > 359) start -= 30.0;
	std::uniform_real_distribution<double> degree2(start + 1, 360.0);
	double end = degree2(generator);

	glBegin(GL_LINE_STRIP);
	for (int i = start; i <= end; ++i) {
		glVertex2f(x + r*sin(2.0*M_PI*i / 360.0), y + r*cos(2.0*M_PI*i / 360.0));
	}
	glEnd();

}

void drawRandom() {
	std::uniform_int_distribution<int> line(0, 1);
	if (line(generator)) {
		drawRandomLine();
	}
	else {
		drawRandomArc();
	}

}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);

	//draw stuff here!
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < 10; ++i) {
		glPushMatrix();
		drawRandom();
		glPopMatrix();
	}

	glPopMatrix();
	glFlush ();
}

void idle()
{
	//update animation here

	//glutDisplayFunc(display);
	//glutPostRedisplay();	//after updating, draw the screen again
}

void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;

		case 'r' :
			glutDisplayFunc(display);
			glutPostRedisplay();
			break;
		
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
