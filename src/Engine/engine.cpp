#include "../Utilities/utilities.hpp"
#include "drawFunc.hpp"
#include "../Utilities/tinyxml2/tinyxml2.h"
#include "../Utilities/camara.cpp"
#include "xmlReader.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
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

void renderFigures(group* grupo)
{
	glPushMatrix();

	group r;

	if (grupo->transformations)
	{
		float x,y,z;
		for(auto i : grupo->transformations->order)
		{
			if (i == transformtype::translate)
			{
				x = grupo->transformations->translate->x; y = grupo->transformations->translate->y; z = grupo->transformations->translate->z;
				glTranslatef(x, y, z);
			}
			else if (i == transformtype::rotate)
			{
				x = grupo->transformations->rotate_points->x; y = grupo->transformations->rotate_points->y; z = grupo->transformations->rotate_points->z;
				float angle = *(grupo->transformations->rotate_angle);
				glRotatef(angle, x, y, z);
			}
			else if (i == transformtype::scale)
			{
				x = grupo->transformations->scale->x; y = grupo->transformations->scale->y; z = grupo->transformations->scale->z;
				glScalef(x, y, z);
			}
		}
	}
	for (auto i : grupo->models) {
        figure value = i;
        drawFigure(value);
    }
	
	for(int i = 0; i < grupo->groups.size(); i++)
	{
		r = grupo->groups[i];
		renderFigures(&r);
	}

	glPopMatrix();
}

void renderScene(void){
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			cam.lx, cam.ly, cam.lz,
			cam.ux, cam.uy, cam.uz);


	drawReferencial();
	renderFigures(&grupos);
	// End of frame
	glutSwapBuffers();
}

void menuCamChoice(int choice){
    switch (choice) {
        case 0:
            cam.mode = 0;
			px = cam.px;
			py = cam.py;
			pz = cam.pz;
            break;
        case 1:
            cam.mode = 1;
            betaC = 0;

            // cam.explorerCenter = point(cam.px, cam.py, cam.pz);
            cam.lx = px;
            cam.ly = py;
            cam.lz = pz;

            break;
        case 2:
            cam.mode = 2;
            betaC = 0;
            // cam.firstTime = true;
            break;
        default:
            break;
    }
}

void modeChoice(int choice){
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
    int modeMenu = glutCreateMenu(modeChoice);
    glutAddMenuEntry("Lines", 0);
    glutAddMenuEntry("Fill", 1);
    glutAddMenuEntry("Points", 2);

    // glutAddSubMenu("Mode", modeMenu);
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
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
    glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	cameraMenu();

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
