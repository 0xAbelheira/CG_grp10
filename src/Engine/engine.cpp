#include "drawFunc.cpp"
#include "../Utilities/tinyxml2/tinyxml2.cpp"
#include "../Utilities/camara.cpp"

#include <iostream>
#include <iterator>
#include <map>
#include <GL/glut.h>
#include <math.h>

using namespace tinyxml2;
using namespace draw;
using namespace std;

map<int, figure> figurasMap;
int ativarFig = 0; //Vai buscar a chave/identificador da figura para desenha-la após obter permissão

int window_size_w;
int window_size_h;
camera cam = camera();

float alpha = 0, beta_ = 0;
float r = 0;

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

int glut_main(int argc, char** argv) {
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG_PROJECT");
	// ilInit();
	// timebase = glutGet(GLUT_ELAPSED_TIME);

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	// glutKeyboardFunc(keyboardfunc);
	// glutMouseFunc(processMouseButtons);
	// glutMotionFunc(processMouseMotion);

	// init GLEW
// #ifndef __APPLE__
	// glewInit();
// #endif
	//  OpenGL settings
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	
	// if (nls > 0) {
	// 	glEnable(GL_LIGHTING);
	// 	for (int i = 0; i < nls; i++) {
	// 		glEnable(GL_LIGHT0 + i);
	// 	}
	// 	cout << "Preparing lights...";
	// 	ls.init_lights();
	// 	cout << " prepared.\n";
	// }
	// cout << "Preparing data...";
	// principal_g.prepare_data();

	cout << " prepared.\n";

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

void remake_lookAt() {
	r = sqrt(pow((cam.px - cam.lx), 2) + pow((cam.py - cam.ly), 2) + pow((cam.pz - cam.lz), 2));
	beta_ = (180 * asin((cam.py - cam.ly) / r)) / 3.14;
	float aux = (cam.px - cam.lx) / (r * cos((beta_ * 3.14) / 180));
	alpha = (180 * asin(aux)) / 3.14;
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
			cout << "------------Controls:---------------\n";
			cout << "Show coordinates system: press R\n";
			cout << "Activate a pointer of lookAt camera: press P\n";
			cout << "Change between GL_LINE and GL_FILL: press M\n";
			cout << "Zoom-in/Zoom-out: press MOUSE-2 and move\n";
			cout << "Move camera: press MOUSE-1 and move\n";
			cout << "Move lookAT camera: press\n";
			cout << " > W and S (Y axis)\n";
			cout << " > A and D (X axis)\n";
			cout << " > E and Q (Z axis)\n";
			cout << "------------------------------------\n";
		}
	}
	else {
		cout << "Invalid arguments!";
		return -1;
	}


	glut_main(argc, argv);

	return 0;
}