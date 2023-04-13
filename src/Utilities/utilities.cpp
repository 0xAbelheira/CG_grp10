#include "utilities.hpp"
#include <iostream>

#if  defined(__unix__) || defined(__APPLE__) 
#include <unistd.h>
#include <dirent.h>
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

std::string utilities::getPath(){
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

utilities::transform::transform()
{
    this->translate = nullptr;
    this->rotate_angle = nullptr;
    this->rotate_points = nullptr;
    this->scale = nullptr;
}

utilities::transform::transform(utilities::point translate,
                                float rotate_angle,
                                utilities::point rotate_points,
                                utilities::point scale)
{
    this->translate = new utilities::point; *(this->translate) = translate;
    this->rotate_angle = new float; *(this->rotate_angle) = rotate_angle;
    this->rotate_points = new utilities::point; *(this->rotate_points) = rotate_points;
    this->scale = new utilities::point; *(this->scale) = scale;
}

utilities::transform::transform(const transform& transform)
{
    this->translate = transform.translate;
    this->rotate_angle = transform.rotate_angle;
    this->rotate_points = transform.rotate_points;
    this->scale = transform.scale;
    this->order = vector<transformtype>();
}

utilities::group::group()
{
    this->groups = vector<group>();
    this->transformations = nullptr;
    this->models = vector<figure>();
}

utilities::group::group(vector<group> grupos, transform transformacoes, vector<figure> modelos)
{
    this->groups = vector<group>(grupos);
    this->transformations = new transform; *(this->transformations) = transformacoes;
    this->models = vector<figure>(modelos);
}

// utilities::group::group(const group& group)
// {
//     this->groups = group.groups;
//     this->transformations = grou
//     this->models = 
// }
