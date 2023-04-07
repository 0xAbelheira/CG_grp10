#include "utilities.hpp"

#if  defined(__unix__) || defined(__APPLE__) 
#include <unistd.h>
#define getDir getcwd
static const std::string slash = "/";
#else
#include <direct.h>
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

std::string utilities::getPath(){
    char path[90];
    return getDir(path, 90) + slash + ".." + slash + "3dFiles" + slash;
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
