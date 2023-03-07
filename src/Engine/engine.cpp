#include "drawFunc.hpp"
#include "../tinyXML/tinyxml.h"

#include <iostream>
#include <iterator>
#include <map>

using namespace draw;
using namespace std;

int window_size_w;
int window_size_h;

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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void){
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);


	drawReferencial();
    auto pos = figurasMap.find(ativarFig);
    if (pos == figurasMap.end()) {
        cout << "Não existem mais figuras." << endl; // alterar este bloco 
        glutDestroyWindow(0);
    } else {
        figure value = pos->second;
        drawFigure(value);
    }

	// End of frame
	glutSwapBuffers();
}

models xml_models(XMLElement* models_e) {
	models ms = models();
	XMLElement* model_e = models_e->FirstChildElement("model");
	while (model_e) {
		bool exists = true;
		model m = model();
		const char* filename = model_e->Attribute("file");
		ifstream file(filename);
		m.add_name(filename);
		if (file.is_open()) { //abre o ficheiro
			cout << "Loading file model " << filename << "...";
			string line;
			while (getline(file, line)) { //l� linha a linha
				if (line[0] == 'i') {
					int idx = stoi(line.substr(1, line.length()));
					m.add_index(idx); //cada indice corresponde a 3 coords.
				}
				else {
					point p = point(line);
					m.add_point(p);
				}
			}
			file.close();
			cout << " loaded.\n";
			XMLElement* texture_e = model_e->FirstChildElement("texture");
			if (texture_e) {
				const char* texturename = texture_e->Attribute("file");
				m.add_texture(texturename);
			}
			XMLElement* color_e = model_e->FirstChildElement("color");
			if (color_e) {
				m.add_color(xml_color(color_e));
			}
			ms.add_model(m);
		}
		else {
			exists = false;
			cout << "WARNING! File model " << filename << " does not exist!(IGNORED)\n";
		}
		model_e = model_e->NextSiblingElement("model");
	}
	return ms;
}

group xml_group(XMLElement* group_e) {
	group g = group();
	XMLElement* models_e = group_e->FirstChildElement("models");
	if (models_e) {
		g.add_models(xml_models(models_e));
	}
	XMLElement* othergroup_e = group_e->FirstChildElement("group");
	while (othergroup_e) {
		g.add_group(xml_group(othergroup_e));
		othergroup_e = othergroup_e->NextSiblingElement("group");
	}
	return g;
}

void xml_camera(XMLElement* camera_e) {
	XMLElement* position_e = camera_e->FirstChildElement("position");
	if (position_e) {
		position_e->QueryAttribute("x", &cam.px);
		position_e->QueryAttribute("y", &cam.py);
		position_e->QueryAttribute("z", &cam.pz);
	}
	else {
		cout << "WARNING: \"position\" (element of \"camera\") not detected. Using default values...";
	}
	XMLElement* lookAt_e = camera_e->FirstChildElement("lookAt");
	if (lookAt_e) {
		lookAt_e->QueryAttribute("x", &cam.lx);
		lookAt_e->QueryAttribute("y", &cam.ly);
		lookAt_e->QueryAttribute("z", &cam.lz);
		remake_lookAt();
	}
	else {
		cout << "WARNING: \"lookAt\" (element of \"camera\") not detected. Using default values...";
	}
	XMLElement* up_e = camera_e->FirstChildElement("up");
	if (up_e) {
		up_e->QueryAttribute("x", &cam.ux);
		up_e->QueryAttribute("y", &cam.uy);
		up_e->QueryAttribute("z", &cam.uz);
	}
	else {
		cout << "WARNING: \"up\" (element of \"camera\") not detected. Using default values...";
	}
	XMLElement* projection_e = camera_e->FirstChildElement("projection");
	if (projection_e) {
		projection_e->QueryAttribute("fov", &cam.fov);
		projection_e->QueryAttribute("near", &cam.near);
		projection_e->QueryAttribute("far", &cam.far);
	}
	else {
		cout << "WARNING: \"projection\" (element of \"camera\") not detected. Using default values...";
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
		cout << "WARNING: \"camera\" not detected. Using default values...";
	}
	XMLElement* group_e = world_e->FirstChildElement("group");
	if (group_e) {
		principal_g = xml_group(group_e);
	}
	else {
		cout << "ERROR: \"group\" not detected.";
		return -1;
	}
	return 1;
}

int main(int argc, char** argv){
    if (argc <=1) {
        std::cout << "\nMissing arguments\n" << std::endl;
    }
    else if(argc > 2){
        std::cout << "\nToo many arguments\n" << std::endl;
    }
    else {
        std::cout << "\nReading .... ..... ....\n" << std::endl;
        if(lerFicheiroXML(argv[1])==0) { // alterar esta função

            // put GLUT init here
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
            glutInitWindowPosition(100, 100);
            glutInitWindowSize(800, 800);
            glutCreateWindow("Projeto-CG");

            // put callback registry here
            glutDisplayFunc(renderScene);
            glutReshapeFunc(changeSize);

            glutKeyboardFunc(nextFigureKey); // alterar esta função

            // some OpenGL settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

            // enter GLUT's main cycle
            glutMainLoop();
        }
    }

	return 1;
}