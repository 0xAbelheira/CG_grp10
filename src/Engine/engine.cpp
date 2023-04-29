#include "../Utilities/utilities.hpp"
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

vector<float> vertices_vec;
GLuint vertices, actualVertice;
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

				}
			}
			else if (i == transformtype::ROTATE)
			{
				x = grupo->transformations->rotate_points->x; y = grupo->transformations->rotate_points->y; z = grupo->transformations->rotate_points->z;
				float angle;
				if(!grupo->transformations->rotate_time)
					angle = *(grupo->transformations->rotate_angle);
				else {
					int gluttime = glutGet(GLUT_ELAPSED_TIME); //vem em milisegundos
            		angle = (gluttime * 360) / *(grupo->transformations->rotate_time);
				}
				glRotatef(angle, x, y, z);
			}
			else if (i == transformtype::SCALE)
			{
				x = grupo->transformations->scale->x; y = grupo->transformations->scale->y; z = grupo->transformations->scale->z;
				glScalef(x, y, z);
			}
		}
	}

	// for (auto i : grupo->models)
	// {
    //     figure value = i;
    //     drawFigure(value);
    // }

	if (!grupo->models.empty())
	{
		GLuint maxVertice = 0;
		for (auto i : grupo->models)
			maxVertice += i.points.size();
		drawFiguresVBO(actualVertice, maxVertice);
		actualVertice += maxVertice;
	}
	
	for(int i = 0; i < grupo->groups.size(); i++)
	{
		r = grupo->groups[i];
		drawFigures(&r);
	}

	glPopMatrix();
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
	actualVertice = 0;
	drawFigures(&grupos);

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0;
		sprintf(s, "FPS: %6.2f", fps);
		glutSetWindowTitle(s);
	}

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
		// if (argv[1]) {
		// 	cout << "Loading " << argv[1] << " ...\n";
		// }

		XMLDocument doc;
		XMLError err = doc.LoadFile("../../test.xml");

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
		// cout << "Invalid arguments!";
		// return -1;
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
