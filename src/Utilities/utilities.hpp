#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

namespace utilities{
    struct point {
        float x;
        float y;
        float z;
    };

    class figure {
        
    public:
        std::vector<point> points;
        std::vector<point> normais;
        void addPoint(float, float, float);
        void addNormal(float, float, float);
    };

    std::string getPath();

    class translate
    {
        private:
        public:
            utilities::figure points;
            float time;
            bool align;
            translate();
    };

    enum transformtype { TRANSLATE, ROTATE, SCALE };
    class transform
    {
        private:
        public:
            std::vector<transformtype> order;
            utilities::translate* translate;
            float* rotate_angle;
            float* rotate_time;
            utilities::point* rotate_points;
            utilities::point* scale;
            transform();
    };

    enum lighttype { POINT, DIRECTIONAL, SPOTLIGHT };
    class light
    {
        private:
        public:
            lighttype *type;
            point *pos;
            point *dir;
            int *cutoff;
            light();
    };

    enum colortype { DIFFUSE, AMBIENT, SPECULAR, EMISSIVE };
    class color
    {
        private:
        public:
            std::map<colortype,std::tuple<int,int,int,int>> colors;
            int *shininess;
            color();
    };

    class models
    {
        private:
        public:
            utilities::figure model;
            std::string *texture;
            utilities::color *color;
            models();
    };

    class group
    {
        private:
        public:
            std::vector<group> groups;
            transform* transformations;
            std::vector<utilities::models> models;
            group();
    };
}
