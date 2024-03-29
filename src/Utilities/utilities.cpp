#include "utilities.hpp"
#include <iostream>

#if  defined(__unix__) || defined(__APPLE__) 
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#define getDir getcwd
static const std::string slash = "/";
#else
#include <direct.h>
#include <Windows.h>
#define getDir _getcwd
static const std::string slash = "\\";
#endif

using namespace std;
using namespace utilities;


// cria um ponto com as dadas coordenadas, e adiciona-o à lista de pontos da figura
void figure::addPoint(float a, float b, float c){
    point p;
    p.x = a;
    p.y = b;
    p.z = c;
    points.push_back(p);
}

void figure::addNormal(float a, float b, float c){
    point p;
    float l = sqrt(a*a + b * b + c * c);
    p.x = a/l;
    p.y = b/l;
    p.z = c/l;
    normais.push_back(p);
}

void figure::addText(float a, float b, float c){
    point p;
    p.x = a;
    p.y = b;
    p.z = c;
    text.push_back(p);
}

void figure::calTextPlain(float size) {
    for(auto a : this->points) {
        this->addText((a.x+(size/2))/size, (a.z+(size/2))/size, 0);
    }
}

void figure::calTextBox(float size) {
    int s = this->points.size();
    int k = 0;
    for (auto a : this->points)
    {
        if(k < s/3) this->addText((a.x+(size/2))/size, (a.z+(size/2))/size, 0);
        else if (k < 2*s/3) this->addText((a.x+(size/2))/size, (a.y+(size/2))/size, 0);
        else this->addText((a.z+(size/2))/size, (a.y+(size/2))/size, 0);
        
        ++k;
    }
}


#if  defined(__unix__) || defined(__APPLE__) 
bool folderExists(const std::string& folderPath)
{
    DIR* dir = opendir(folderPath.c_str());
    if (dir != nullptr)
    {
        closedir(dir);
        return true;
    }
    else
    {
        return false;
    }
}
#else
bool folderExists(const std::string& folderPath)
{
    DWORD fileAttributes = GetFileAttributesA(folderPath.c_str());
    return (fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}
#endif

std::string utilities::getPath()
{
    char path[128];
    getDir(path, 90);
    if(folderExists(path + slash + "3dFiles"))
        return path + slash + "3dFiles" + slash;
    if(folderExists(path + slash + ".." + slash + "3dFiles"))
        return path + slash + ".." + slash + "3dFiles" + slash;
    if(folderExists(path + slash + ".." + slash + ".." + slash + "3dFiles"))
        return path + slash + ".." + slash + ".." + slash + "3dFiles" + slash;
    return "";
}

utilities::translate::translate()
{
    this->time = 0;
    this->align = false;
}

utilities::transform::transform()
{
    this->translate = nullptr;
    this->rotate_angle = nullptr;
    this->rotate_time = nullptr;
    this->rotate_points = nullptr;
    this->scale = nullptr;
    this->order = vector<transformtype>();
}

utilities::light::light()
{
    this->type = nullptr;
    this->pos = nullptr;
    this->dir = nullptr;
    this->cutoff = nullptr;
}

utilities::color::color()
{
    this->colors = map<colortype,std::tuple<int,int,int,int>>();
    this->shininess = nullptr;
}

utilities::models::models()
{
    this->texture = nullptr;
    this->color = nullptr;
    this->textID = nullptr;
}

utilities::group::group()
{
    this->groups = vector<group>();
    this->transformations = nullptr;
    this->models = vector<utilities::models>();
}
