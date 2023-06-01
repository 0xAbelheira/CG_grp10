#include "../Utilities/utilities.hpp"
#include "../Utilities/matrix.hpp"
#include "catmullrom.hpp"
#include "drawFunc.hpp"
#include "../Utilities/tinyxml2/tinyxml2.h"
#include "../Utilities/camara.cpp"
#include "xmlReader.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>
#include <iterator>
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace tinyxml2;
using namespace draw;
using namespace std;
using namespace utilities;
using namespace catmull;
using namespace matrixUtils;

vector<float> vertices_vec;
GLuint vertices, current_vertice;
group grupos;
int window_size_w;
int window_size_h;
camera cam = camera();
float mouse_x = 0;
float mouse_y = 0; 
float px;
float py;
float pz;
float r;
float betaC ;
float alphaC;
float aux_y[3] = { 0,1,0 };
bool vbo_enable = true;
float fps_display;

int frame = 0, timebase = 0;

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(cam.fov, ratio, cam.near, cam.far);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawFigures(group* grupo)
{
	glPushMatrix();

	group r;

	if (grupo->transformations)
	{
		float x,y,z;
		for(auto i : grupo->transformations->order)
		{
			if (i == transformtype::TRANSLATE)
			{
				if (grupo->transformations->translate->points.points.size() < 4)
				{
					point temp = grupo->transformations->translate->points.points[0];
					glTranslatef(temp.x, temp.y, temp.z);
				}
				else
				{
					//point in the curve
					float res[3];
					//vector tangent to the curve
					float deriv[3];

					float glt = glutGet(GLUT_ELAPSED_TIME) / (grupo->transformations->translate->time * 1000);
					
					//render the curve
					glColor3f(1, 1, 1);
					glBegin(GL_LINE_LOOP);
					for (int i = 0; i < 100; i++) {
						getGlobalCatmullRomPoint(i/100.0f, grupo->transformations->translate->points.points, res, deriv);
						glVertex3fv(res);
					}
					glEnd();

					//animation without align to the curve
					if(grupo->transformations->translate->align){
						getGlobalCatmullRomPoint(glt,grupo->transformations->translate->points.points,res,deriv);
						glTranslatef(res[0], res[1], res[2]);

						//Xi = p'(t)
						//deriv = X
							
						//Zi = Xi * Yi-1
						float Z[3]; 
						cross(deriv,aux_y,Z);
						//Yi = Zi * Xi
						cross(Z,deriv,aux_y);

						//all vectors have to be normalized
						normalize(deriv);
						normalize(aux_y);
						normalize(Z);

						//OpenGL matrices are column major => so it's used the transpose of the rotation instead
						float rotationMatrix[16];
						buildRotMatrix(deriv,aux_y,Z,rotationMatrix);

						glMultMatrixf(rotationMatrix);
					}else{
						//animation without align to the curve
						getGlobalCatmullRomPoint(glt,grupo->transformations->translate->points.points,res,deriv);
						glTranslatef(res[0], res[1], res[2]);
					}
				}
			}
			else if (i == transformtype::ROTATE)
			{
				x = grupo->transformations->rotate_points->x; y = grupo->transformations->rotate_points->y; z = grupo->transformations->rotate_points->z;
				float angle;
				if(!grupo->transformations->rotate_time)
					angle = *(grupo->transformations->rotate_angle);
				else
            		angle = (((float)glutGet(GLUT_ELAPSED_TIME) / 1000)* 360) / *(grupo->transformations->rotate_time);
				glRotatef(angle, x, y, z);
			}
			else if (i == transformtype::SCALE)
			{
				x = grupo->transformations->scale->x; y = grupo->transformations->scale->y; z = grupo->transformations->scale->z;
				glScalef(x, y, z);
			}
		}
	}

	if (vbo_enable)
	{
		if (!grupo->models.empty())
		{
			GLuint figures_size = 0;
			for (auto i : grupo->models)
				figures_size += i.model.points.size();
			drawFiguresVBO(current_vertice, figures_size);
			current_vertice += figures_size;
		}
	}
	else
	{
		for (auto i : grupo->models)
		{
			figure value = i.model;
			drawFigure(value);
		}
	}

	
	for(int i = 0; i < grupo->groups.size(); i++)
	{
		r = grupo->groups[i];
		drawFigures(&r);
	}

	glPopMatrix();
}

void renderText()
{
    // save projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // set projection so that coordinates match window pixels
    gluOrtho2D(0, window_size_w, window_size_h, 0);
    glMatrixMode(GL_MODELVIEW);

    // disable depth test (assuming text is written in the end)
    glDisable(GL_DEPTH_TEST);

    // set modelview matrix
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2d(10, 20); // text position in pixels

    // render text
    char text[100];
    sprintf(text, "FPS: %6.2f    |    ", fps_display);
	if (vbo_enable)
    	sprintf(text, "%sVBO Enable", text);
	else
    	sprintf(text, "%sVBO Disable", text);

    for (char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Restore matrices and reenable depth test
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void renderScene(void){

	float fps = 0;
	int time;
	char s[64];

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			cam.lx, cam.ly, cam.lz,
			cam.ux, cam.uy, cam.uz);


	drawReferencial();
	current_vertice = 0;
	drawFigures(&grupos);

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0;
		fps_display = fps;
	}

	renderText();

	// End of frame
	glutSwapBuffers();
}

void vbo_Choice(int choice)
{
	switch (choice) {
        case 0:
			vbo_enable = false;
            break;
        case 1:
			vbo_enable = true;
            break;
        default:
            break;
	}
}

void modeChoice(int choice)
{
    switch (choice) {
        case 0:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 2:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
}

void cameraMenu(){

	int vboMode = glutCreateMenu(vbo_Choice);
    glutAddMenuEntry("Disable", 0);
    glutAddMenuEntry("Enable", 1);

    int drawMode = glutCreateMenu(modeChoice);
    glutAddMenuEntry("Lines", 0);
    glutAddMenuEntry("Fill", 1);
    glutAddMenuEntry("Points", 2);

	glutAddSubMenu("VBOs", vboMode);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouseFunc(int button, int state,int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void motionFunc(int x, int y) {
	int dx = mouse_x - x;
	int dy = mouse_y - y;

	alphaC += dx*M_PI/400;
	betaC -= dy*M_PI/400;
    if(betaC >= M_PI/2) betaC = 1.57079;
    if(betaC <= -M_PI/2) betaC = -1.57079;


    px = r * cos(betaC) * sin(alphaC);
	py = r * sin(betaC);
	pz = r * cos(betaC) * cos(alphaC);

	glutPostRedisplay();

	mouse_x = x;
	mouse_y = y;
}

int glut_main(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_size_w, window_size_h);
	glutCreateWindow("CG_PROJECT");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
    glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	//  OpenGL settings
	#ifndef __APPLE__	
		// init GLEW
		glewInit();
	#endif	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);

	cameraMenu();

	// criar o VBO
	glGenBuffers(1, &vertices);

	// copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(
		GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
		sizeof(float) * vertices_vec.size(), // tamanho do vector em bytes
		vertices_vec.data(), // os dados do array associado ao vector
		GL_STATIC_DRAW // indicativo da utilização (estático e para desenho)
	);
	vertices_vec.clear();
	vertices_vec.~vector();

	cout << " prepared.\n";

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

int main(int argc, char** argv) {
	if (argc == 2) {
		if (argv[1]) {
			cout << "Loading " << argv[1] << " ...\n";
		}

		XMLDocument doc;
		XMLError err = doc.LoadFile(argv[1]);

		if (err) {
			fprintf(stderr, "TINYXML2 FAILURE! Error code: %d\n", err);
			return err;
		}

		//world engloba todo o xml
		XMLElement* world_e = doc.FirstChildElement("world");
		if (!world_e) {
			cout << "XML needs a field called \"world\"";
			return -1;
		}
		else {
			vertices_vec = vector<float>();
			int err = xml_world(world_e);
			if (err == -1) return -1;
		}
	}
	else {
		cout << "Invalid arguments!";
		return -1;
	}

	px = cam.px;
	py = cam.py;
	pz = cam.pz;
	r = hypot(hypot(px, py), pz);
	betaC = asin(py/r);
	alphaC = asin(px/(r*cos(betaC)));

	glut_main(argc, argv);

	return 0;
}
