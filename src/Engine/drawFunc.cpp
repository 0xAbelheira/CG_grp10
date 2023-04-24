#include "../Utilities/utilities.hpp" 
#include "drawFunc.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace utilities;
using namespace std;

extern vector<float> vertices_vec;

// funçao auxiliar que desenha triangulos
void drawTriangle(point p1, point p2, point p3) {

	// glBegin(GL_TRIANGLES);

	// glColor3f(1,1,1);

	// //desenhar os 3 vertices do triangulo
	// glVertex3f(p1.x, p1.y, p1.z);
	// glVertex3f(p2.x, p2.y, p2.z);
	// glVertex3f(p3.x, p3.y, p3.z);

	// glEnd();

	vertices_vec.push_back(p1.x);
	vertices_vec.push_back(p1.y);
	vertices_vec.push_back(p1.z);

	vertices_vec.push_back(p2.x);
	vertices_vec.push_back(p2.y);
	vertices_vec.push_back(p2.z);

	vertices_vec.push_back(p3.x);
	vertices_vec.push_back(p3.y);
	vertices_vec.push_back(p3.z);
}


// recebe a lista de pontos, e o n�mero de pontos contidos na lista
void draw::drawFigure(figure f) {
	int i;
	int n = f.points.size();
	for (i = 0; i+2 <= n; i+=3) {
		//desenha os triangulos partindo da lista de pontos da figura
		drawTriangle(f.points[i], f.points[i + 1], f.points[i + 2]);
	}
}

void draw::drawReferencial() {
	// Desenhar referencial
	glBegin(GL_LINES);
	
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	
	glEnd();
}