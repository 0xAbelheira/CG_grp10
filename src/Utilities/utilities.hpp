#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
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

    class transform
    {
        private:
        public:
            utilities::point* translate;
            float* rotate_angle;
            utilities::point* rotate_points;
            utilities::point* scale;
            transform();
            transform(utilities::point, float, utilities::point, utilities::point);
            transform(const transform&);
    };

    class group
    {
        private:
        public:
            std::vector<group> groups;
            transform* transformations;
            std::vector<utilities::figure> models;
            group();
            group(std::vector<group>, transform, std::vector<utilities::figure>);
            // group(const group&);
    };
}
