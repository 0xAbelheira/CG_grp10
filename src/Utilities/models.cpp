#include <IL/il.h>

class model {
public:
	vector<float> ps; //pontos do modelo
	vector<float> ns; //pontos das normais
	vector<unsigned int> idxs;
	GLuint indices, vertices, normals, textures, textureID;
	unsigned int indexCount;
	color c;
	bool hastx;
	char* texturename;
	char* name;

	model() {
		this->idxs;
		this->indexCount = 0;
		this->indices = 0;
		this->vertices = 0;
		this->normals = 0;
		this->c = color();
		this->name = "";
		this->hastx = false;
	}

	void add_name(const char * name) {
		this->name = _strdup(name);
	}

	void add_point(point p) {
		ps.push_back(p.getX());
		ps.push_back(p.getY());
		ps.push_back(p.getZ());
		ns.push_back(p.getNX());
		ns.push_back(p.getNY());
		ns.push_back(p.getNZ());
	}

	void add_index(int idx) {
		this->idxs.push_back(idx);
		this->indexCount += 1;
	}
};


class models {
public:
	vector<model> mds;

	models() {
		this->mds;
	}

	void add_model(model m) {
		this->mds.push_back(m);
	}


	void prepare_data() {
		for (vector<model>::iterator it = this->mds.begin(); it != this->mds.end(); ++it) {
			it->prepare_data();
		}
	}

	void render() {
		for (vector<model>::iterator it = this->mds.begin(); it != this->mds.end(); ++it) {
			it->render();
		}
	}
};

class group {
public:
	vector<group> gs;
	models ms;

	group(){
		this->ms = models();
	}

	void add_group(group g) {
		gs.push_back(g);
	}

	void add_models(models ms) {
		this->ms = ms;
	}
};