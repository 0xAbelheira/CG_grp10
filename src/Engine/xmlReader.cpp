#include <iterator>
#include <iostream>
#include <map>
#include "../Utilities/camara.cpp"
#include "../Utilities/tinyxml2/tinyxml2.h"
#include "../Utilities/utilities.hpp"
#include "xmlReader.hpp"

using namespace tinyxml2;
using namespace std;
using namespace utilities;

extern group grupos;
extern camera cam;
extern int window_size_w;
extern int window_size_h;
extern vector<float> vertices_vec;

transform xml_transform(XMLElement* models_e)
{
	transform r = transform();
    XMLElement* model_e = models_e->FirstChildElement();
	while(model_e)
	{
		if (strcmp(model_e->Value(), "translate") == 0)
		{
			r.order.push_back(transformtype::TRANSLATE);
			r.translate = new translate();
			if(model_e->QueryAttribute("time", &r.translate->time) != XML_NO_ATTRIBUTE)
			{
				model_e->QueryAttribute("align", &r.translate->align);
				XMLElement* points = model_e->FirstChildElement();
				while (points)
				{
					float x, y, z;
					points->QueryAttribute("x", &x);
					points->QueryAttribute("y", &y);
					points->QueryAttribute("z", &z);
					r.translate->points.addPoint(x,y,z);

					points = points->NextSiblingElement();
				}
			}
			else
			{
				float x, y, z;
				model_e->QueryAttribute("x", &x);
				model_e->QueryAttribute("y", &y);
				model_e->QueryAttribute("z", &z);
				r.translate->points.addPoint(x,y,z);
			}
		}
		else if (strcmp(model_e->Value(), "rotate") == 0)
		{
			r.order.push_back(transformtype::ROTATE);
			r.rotate_angle = new float;
			r.rotate_points = new point;
			if(model_e->QueryAttribute("angle", r.rotate_angle) == XML_NO_ATTRIBUTE) {
				r.rotate_time = new float;
				model_e->QueryAttribute("time", r.rotate_time);
			}
			model_e->QueryAttribute("x", &r.rotate_points->x);
			model_e->QueryAttribute("y", &r.rotate_points->y);
			model_e->QueryAttribute("z", &r.rotate_points->z);
		}
		else if (strcmp(model_e->Value(), "scale") == 0)
		{
			r.order.push_back(transformtype::SCALE);
			r.scale = new point;
			model_e->QueryAttribute("x", &r.scale->x);
			model_e->QueryAttribute("y", &r.scale->y);
			model_e->QueryAttribute("z", &r.scale->z);
		}
		model_e = model_e->NextSiblingElement();
	}
    return r;
}

vector<figure> xml_models(XMLElement* models_e)
{
	vector<figure> r = vector<figure>();
	XMLElement* model_e = models_e->FirstChildElement("model");
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
				vertices_vec.push_back(x1);
				vertices_vec.push_back(y1);
				vertices_vec.push_back(z1);
			}
			fs.close();
			r.push_back(figura);
		}

		else{
			std::cout << "Can't open file!"<< std::endl;
		}
		model_e = model_e->NextSiblingElement("model");
	}
	
	XMLElement* model_color_e = model_e->FirstChildElement("texture");
	if (model_color_e)
	{
		string texture = model_color_e->Attribute("file");
		// adicionar a estrutura
	}
	
	model_color_e = model_e->FirstChildElement("color");
	if (model_color_e)
	{
		XMLElement* diffuse = model_color_e->FirstChildElement("diffuse");
		int diffuse_r, diffuse_g, diffuse_b;
		diffuse->QueryAttribute("R", &diffuse_r);
		diffuse->QueryAttribute("G", &diffuse_g);
		diffuse->QueryAttribute("B", &diffuse_b);

		XMLElement* ambient = model_color_e->FirstChildElement("ambient");
		int ambient_r, ambient_g, ambient_b;
		ambient->QueryAttribute("R", &ambient_r);
		ambient->QueryAttribute("G", &ambient_g);
		ambient->QueryAttribute("B", &ambient_b);

		XMLElement* specular = model_color_e->FirstChildElement("specular");
		int specular_r, specular_g, specular_b;
		specular->QueryAttribute("R", &specular_r);
		specular->QueryAttribute("G", &specular_g);
		specular->QueryAttribute("B", &specular_b);

		XMLElement* emissive = model_color_e->FirstChildElement("emissive");
		int emissive_r, emissive_g, emissive_b;
		emissive->QueryAttribute("R", &emissive_r);
		emissive->QueryAttribute("G", &emissive_g);
		emissive->QueryAttribute("B", &emissive_b);

		XMLElement* shininess = model_color_e->FirstChildElement("shininess");
		float shininess_value;
		shininess->QueryAttribute("value", &shininess_value);
	}
	else
	{
		int diffuse_r = 200, diffuse_g = 200, diffuse_b = 200;
		int ambient_r = 50, ambient_g = 50, ambient_b = 50;
		int specular_r = 0, specular_g = 0, specular_b = 0;
		int emissive_r = 0, emissive_g = 0, emissive_b = 0;
		float shininess_value = 0;
	}
	
	return r;
}

group xml_group(XMLElement* group_e)
{
	group grupos_ = group();
	XMLElement* models_e = group_e->FirstChildElement();
	while (models_e)
	{
		if (strcmp(models_e->Value(), "transform") == 0)
		{
			grupos_.transformations = new transform;
			*(grupos_.transformations) = xml_transform(models_e);
		}
		else if (strcmp(models_e->Value(), "models") == 0)
		{
			grupos_.models = vector<figure>(xml_models(models_e));
		}
		else if (strcmp(models_e->Value(), "group") == 0)
		{
			grupos_.groups.push_back(xml_group(models_e));
		}
		models_e = models_e->NextSiblingElement();
	}
	return grupos_;
}

void xml_lights(XMLElement* lights_e)
{
	XMLElement* light_e = lights_e->FirstChildElement();
	while (light_e)
	{
		if (strcmp(light_e->Attribute("type"), "point") == 0)
		{
			float posX, posY, posZ;
			light_e->QueryAttribute("posX", &posX);
			light_e->QueryAttribute("posY", &posY);
			light_e->QueryAttribute("posZ", &posZ);
		}
		else if (strcmp(light_e->Attribute("type"), "directional") == 0)
		{
			float dirX, dirY, dirZ;
			light_e->QueryAttribute("dirX", &dirX);
			light_e->QueryAttribute("dirY", &dirY);
			light_e->QueryAttribute("dirZ", &dirZ);
		}
		else if (strcmp(light_e->Attribute("type"), "spotlight") == 0)
		{
			float 	posX, posY, posZ,
					dirX, dirY, dirZ,
					cutoff;
			light_e->QueryAttribute("posX", &posX);
			light_e->QueryAttribute("posY", &posY);
			light_e->QueryAttribute("posZ", &posZ);

			light_e->QueryAttribute("dirX", &dirX);
			light_e->QueryAttribute("dirY", &dirY);
			light_e->QueryAttribute("dirZ", &dirZ);

			light_e->QueryAttribute("cutoff", &cutoff);
		}
		
		light_e = light_e->NextSiblingElement();
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
		grupos = xml_group(group_e);
	}
	else {
		cout << "ERROR: \"group\" not detected.";
		return -1;
	}
	return 1;
}