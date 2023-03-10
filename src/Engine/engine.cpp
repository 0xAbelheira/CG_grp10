#include "drawFunc.hpp"
#include "../Utilities/tinyxml2/tinyxml2.h"
#include "../Utilities/camara.cpp"
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

map<int, figure> figurasMap;

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

void renderScene(void){
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			cam.lx, cam.ly, cam.lz,
			cam.ux, cam.uy, cam.uz);


	drawReferencial();
	for (auto i : figurasMap) {
        figure value = i.second;
        drawFigure(value);
    }

	// End of frame
	glutSwapBuffers();
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

	cout << " prepared.\n";

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

void xml_models(XMLElement* models_e)
{
	XMLElement* model_e = models_e->FirstChildElement("model");
	int iterador = 0;
	while (model_e) {
		const char* filename = model_e->Attribute("file");
		fstream fs;
		fs.open(getPath()+filename);
		if (fs.is_open()) {
			figure figura;
			string line;
			float x1, y1, z1 = 0.0f; //Inicializa as coordenadas de cada ponto

			//Lê linha a linha do ficheiro, não esquecendo que cada linha é um vértice/ponto
			while (getline(fs, line)) {
				float cood[3]; //guarda num array as coordenadas de cada ponto

				std::string delimiter = " ";
				size_t pos = 0;
				std::string token;
				int i = 0;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					cood[i] = std::stof(token); //converte para float e guarda a coordenada
					i++;
					line.erase(0, pos + delimiter.length());
				}
				x1=cood[0],y1=cood[1],z1=cood[2];
				figura.addPoint(x1,y1,z1);
			}
			fs.close();
			figurasMap.insert(pair<int, figure>(iterador,figura));
			iterador++;
		}

		else{
			std::cout << "Can't open file!"<< std::endl;
		}
		model_e = model_e->NextSiblingElement("model");
	}
}

void xml_group(XMLElement* group_e) {
	XMLElement* models_e = group_e->FirstChildElement("models");
	if (models_e) {
		xml_models(models_e);
	}
}

void xml_camera(XMLElement* camera_e) {
	XMLElement* position_e = camera_e->FirstChildElement("position");
	if (position_e) {
		position_e->QueryAttribute("x", &cam.px);
		position_e->QueryAttribute("y", &cam.py);
		position_e->QueryAttribute("z", &cam.pz);
	}
	else {
		cout << "WARNING: \"position\" (element of \"camera\") not detected.";
	}
	XMLElement* lookAt_e = camera_e->FirstChildElement("lookAt");
	if (lookAt_e) {
		lookAt_e->QueryAttribute("x", &cam.lx);
		lookAt_e->QueryAttribute("y", &cam.ly);
		lookAt_e->QueryAttribute("z", &cam.lz);
	}
	else {
		cout << "WARNING: \"lookAt\" (element of \"camera\") not detected.";
	}
	XMLElement* up_e = camera_e->FirstChildElement("up");
	if (up_e) {
		up_e->QueryAttribute("x", &cam.ux);
		up_e->QueryAttribute("y", &cam.uy);
		up_e->QueryAttribute("z", &cam.uz);
	}
	else {
		cout << "WARNING: \"up\" (element of \"camera\") not detected.";
	}
	XMLElement* projection_e = camera_e->FirstChildElement("projection");
	if (projection_e) {
		projection_e->QueryAttribute("fov", &cam.fov);
		projection_e->QueryAttribute("near", &cam.near);
		projection_e->QueryAttribute("far", &cam.far);
	}
	else {
		cout << "WARNING: \"projection\" (element of \"camera\") not detected.";
	}
}

int xml_world(XMLElement* world_e) {
	XMLElement* window_e = world_e->FirstChildElement("window");
	if (window_e) {
		window_e->QueryAttribute("width", &window_size_w);
		window_e->QueryAttribute("height", &window_size_h);
	}
	else {
		cout << "WARNING: \"window\" not detected. Using default values...";
	}
	XMLElement* camera_e = world_e->FirstChildElement("camera");
	if (camera_e) {
		xml_camera(camera_e);
	}
	else {
		cout << "WARNING: \"camera\" not detected.";
	}
	XMLElement* group_e = world_e->FirstChildElement("group");
	if (group_e) {
		xml_group(group_e);
	}
	else {
		cout << "ERROR: \"group\" not detected.";
		return -1;
	}
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
			fprintf(stderr, "TINYXML FAILURE! Error code: %d\n", err);
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
	alphaC = asin(pz/(r*cos(betaC)));
	
	glut_main(argc, argv);

	return 0;
}
