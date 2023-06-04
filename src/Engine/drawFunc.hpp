#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

namespace draw {

    void drawFigure(utilities::figure);
    void drawFiguresVBO(GLuint, GLuint);
    void drawFiguresVBO(GLuint, GLuint, GLuint);
    void drawReferencial();
    void drawCatmull(std::vector<utilities::point>);
}