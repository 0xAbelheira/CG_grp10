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
        void addPoint(float, float, float);
    };

    std::string getPath();

    class translate
    {
        private:
        public:
            utilities::figure points;
            float time;
            bool align;
            std::map<int,float> point_time;
            translate();

    };

    enum transformtype { TRANSLATE, ROTATE, SCALE };
    class transform
    {
        private:
        public:
            std::vector<transformtype> order;
            translate* translate;
            float* rotate_angle;
            float* rotate_time;
            utilities::point* rotate_points;
            utilities::point* scale;
            transform();
    };

    class group
    {
        private:
        public:
            std::vector<group> groups;
            transform* transformations;
            std::vector<utilities::figure> models;
            group();
    };
}
