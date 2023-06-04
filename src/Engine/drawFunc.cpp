#include "../Utilities/utilities.hpp" 
#include "drawFunc.hpp"
#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace utilities;
using namespace std;

extern GLuint vertices, normais;
extern float dark[4];
extern float white[4];
extern float black[4];


// funçao auxiliar que desenha triangulos
void drawTriangle(point p1, point p2, point p3) {

	glBegin(GL_TRIANGLES);

	//desenhar os 3 vertices do triangulo
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);

	glEnd();
}


// recebe a lista de pontos, e o n�mero de pontos contidos na lista
void draw::drawFigure(figure f) {
	int i;
	int n = f.points.size();
	glColor3f(1.0f,1.0f,1.0f);
	for (i = 0; i+2 <= n; i+=3) {
		//desenha os triangulos partindo da lista de pontos da figura
		drawTriangle(f.points[i], f.points[i + 1], f.points[i + 2]);
	}
}

// recebe a lista de pontos, e o n�mero de triangulos a ser desenhados pelo VBO
void draw::drawFiguresVBO(GLuint from, GLuint size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normais);
	glNormalPointer(GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, from, size);
}

void draw::drawReferencial() {
	// Desenhar referencial
	glBegin(GL_LINES);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
}

void draw::drawCatmull(vector<point> pontos) {
    glBegin(GL_LINES);
    for (size_t i = 0; i < pontos.size() - 1; i++) {
        float pos1[3] = { pontos[i].x, pontos[i].y, pontos[i].z };
        glVertex3fv(pos1);
        float pos2[3] = { pontos[i + 1].x, pontos[i + 1].y, pontos[i + 1].z };
        glVertex3fv(pos2);
    }
    glEnd();
}