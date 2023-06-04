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
extern vector<light> ls;
extern int window_size_w;
extern int window_size_h;
extern vector<float> vertices_vec, normais_vec, text_vec;
extern int nLights;

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

vector<models> xml_models(XMLElement* models_e)
{
	vector<models> r = vector<models>();
	XMLElement* model_e = models_e->FirstChildElement("model");
	while (model_e) {
		const char* filename = model_e->Attribute("file");
		fstream fs;
		fs.open(getPath()+filename);
		if (fs.is_open()) {
			models temp;
			figure figura;
			string line;
			int f = 0;
			float x1, y1, z1 = 0.0f; //Inicializa as coordenadas de cada ponto

			//Lê linha a linha do ficheiro, não esquecendo que cada linha é um vértice/ponto
			while(getline(fs, line)) {
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
				if(i != 3) ++f;
				else
				{
					x1=cood[0],y1=cood[1],z1=cood[2];

					if(f == 0) {
						figura.addPoint(x1,y1,z1);
						vertices_vec.push_back(x1);
						vertices_vec.push_back(y1);
						vertices_vec.push_back(z1);
					}
					else if (f == 1)
					{
						// figura.addNormal(x1,y1,z1);
						normais_vec.push_back(x1);
						normais_vec.push_back(y1);
						normais_vec.push_back(z1);
					}
					else if (f == 2) {
						// figura.addText(x1,y1,z1);
						text_vec.push_back(x1);
						text_vec.push_back(y1);
						// text_vec.push_back(z1);
					}
				}
			}
			fs.close();
			temp.model = figura;
			
			
			XMLElement* model_color_e = model_e->FirstChildElement("texture");
			if (model_color_e)
			{
				string texture = model_color_e->Attribute("file");
				temp.texture = new string(texture);
			}
			
			temp.color = new color();
			model_color_e = model_e->FirstChildElement("color");
			if (model_color_e)
			{

				XMLElement* diffuse = model_color_e->FirstChildElement("diffuse");
				if (diffuse)
				{
					int diffuse_r, diffuse_g, diffuse_b;
					diffuse->QueryAttribute("R", &diffuse_r);
					diffuse->QueryAttribute("G", &diffuse_g);
					diffuse->QueryAttribute("B", &diffuse_b);
					temp.color->colors[DIFFUSE] = tuple<int,int,int,int>(diffuse_r, diffuse_g, diffuse_b, 1);
				}
				else {
					int diffuse_r = 200, diffuse_g = 200, diffuse_b = 200;
					temp.color->colors[DIFFUSE] = tuple<int,int,int,int>(diffuse_r, diffuse_g, diffuse_b, 1);
				}

				XMLElement* ambient = model_color_e->FirstChildElement("ambient");
				if (ambient)
				{
					int ambient_r, ambient_g, ambient_b;
					ambient->QueryAttribute("R", &ambient_r);
					ambient->QueryAttribute("G", &ambient_g);
					ambient->QueryAttribute("B", &ambient_b);
					temp.color->colors[AMBIENT] = tuple<int,int,int,int>(ambient_r, ambient_g, ambient_b, 1);
				}
				else {
					int ambient_r = 50, ambient_g = 50, ambient_b = 50;
					temp.color->colors[AMBIENT] = tuple<int,int,int,int>(ambient_r, ambient_g, ambient_b, 1);
				}

				XMLElement* specular = model_color_e->FirstChildElement("specular");
				if (specular)
				{
					int specular_r, specular_g, specular_b;
					specular->QueryAttribute("R", &specular_r);
					specular->QueryAttribute("G", &specular_g);
					specular->QueryAttribute("B", &specular_b);
					temp.color->colors[SPECULAR] = tuple<int,int,int,int>(specular_r, specular_g, specular_b, 1);
				}
				else {
					int specular_r = 0, specular_g = 0, specular_b = 0;
					temp.color->colors[SPECULAR] = tuple<int,int,int,int>(specular_r, specular_g, specular_b, 1);
				}

				XMLElement* emissive = model_color_e->FirstChildElement("emissive");
				if (emissive)
				{
					int emissive_r, emissive_g, emissive_b;
					emissive->QueryAttribute("R", &emissive_r);
					emissive->QueryAttribute("G", &emissive_g);
					emissive->QueryAttribute("B", &emissive_b);
					temp.color->colors[EMISSIVE] = tuple<int,int,int,int>(emissive_r, emissive_g, emissive_b, 1);
				}
				else {
					int emissive_r = 0, emissive_g = 0, emissive_b = 0;
					temp.color->colors[EMISSIVE] = tuple<int,int,int,int>(emissive_r, emissive_g, emissive_b, 1);
				}

				XMLElement* shininess = model_color_e->FirstChildElement("shininess");
				if (shininess)
				{
					temp.color->shininess = new int;
					shininess->QueryAttribute("value", temp.color->shininess);
				}
				else {
					temp.color->shininess = new int;
					temp.color->shininess = 0;
				}
			}
			else
			{
				int diffuse_r = 200, diffuse_g = 200, diffuse_b = 200;
				int ambient_r = 50, ambient_g = 50, ambient_b = 50;
				int specular_r = 0, specular_g = 0, specular_b = 0;
				int emissive_r = 0, emissive_g = 0, emissive_b = 0;
				temp.color->shininess = new int;
				temp.color->shininess = 0;
				temp.color->colors[DIFFUSE] = tuple<int,int,int,int>(diffuse_r, diffuse_g, diffuse_b, 1);
				temp.color->colors[AMBIENT] = tuple<int,int,int,int>(ambient_r, ambient_g, ambient_b, 1);
				temp.color->colors[SPECULAR] = tuple<int,int,int,int>(specular_r, specular_g, specular_b, 1);
				temp.color->colors[EMISSIVE] = tuple<int,int,int,int>(emissive_r, emissive_g, emissive_b, 1);
			}
			r.push_back(temp);
		}

		else{
			std::cout << "Can't open file!"<< std::endl;
		}
		model_e = model_e->NextSiblingElement("model");
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
			grupos_.models = vector<models>(xml_models(models_e));
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
	nLights = 0;
	XMLElement* light_e = lights_e->FirstChildElement();
	while (light_e)
	{
		light *l = new light();
		l->type = new lighttype;
		if (strcmp(light_e->Attribute("type"), "point") == 0)
		{
			l->pos = new point;
			light_e->QueryAttribute("posx", &l->pos->x);
			light_e->QueryAttribute("posy", &l->pos->y);
			light_e->QueryAttribute("posz", &l->pos->z);
			*l->type = POINT;
		}
		else if (strcmp(light_e->Attribute("type"), "directional") == 0)
		{
			l->dir = new point;
			light_e->QueryAttribute("dirx", &l->dir->x);
			light_e->QueryAttribute("diry", &l->dir->y);
			light_e->QueryAttribute("dirz", &l->dir->z);
			*l->type = DIRECTIONAL;
		}
		else if (strcmp(light_e->Attribute("type"), "spot") == 0)
		{
			l->pos = new point;
			l->dir = new point;
			l->cutoff = new int;
			light_e->QueryAttribute("posx", &l->pos->x);
			light_e->QueryAttribute("posy", &l->pos->y);
			light_e->QueryAttribute("posz", &l->pos->z);

			light_e->QueryAttribute("dirx", &l->dir->x);
			light_e->QueryAttribute("diry", &l->dir->y);
			light_e->QueryAttribute("dirz", &l->dir->z);

			light_e->QueryAttribute("cutoff", l->cutoff);

			*l->type = SPOTLIGHT;
		}
		else
			throw new exception("Erro no nome das luzes");
		
		light_e = light_e->NextSiblingElement();
		ls.push_back(*l);
		++nLights;
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
	XMLElement* lights_e = world_e->FirstChildElement("lights");
	if (lights_e) {
		xml_lights(lights_e);
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