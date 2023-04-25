#include "../Utilities/matrix.hpp"
#include "../Utilities/utilities.hpp"

using namespace utilities;

namespace bezier{
    void getBezierPoint(float, float, float**, float**, float**, float*);
    figure generateBezierPatches(figure, std::vector<size_t>, size_t);
}
