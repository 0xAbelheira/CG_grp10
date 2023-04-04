#include <vector>

namespace group
{
    class transform
    {
        private:
        public:
            utilities::point translate;
            float rotate_angle;
            utilities::point rotate_points;
            utilities::point scale;
    };

    class group
    {
        private:
        public:
            std::vector<group> groups;
            transform transformations;
            std::vector<std::string> models;
    };
} // namespace group
