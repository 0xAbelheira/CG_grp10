#include "../Utilities/utilities.cpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace utilities;

namespace draw {

    void drawFigure(figure);
    void drawReferencial();

}