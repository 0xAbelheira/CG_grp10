#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

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

    enum transformtype { translate, rotate, scale };
    class transform
    {
        private:
        public:
            std::vector<transformtype> order;
            utilities::point* translate;
            float* rotate_angle;
            float* time;
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
