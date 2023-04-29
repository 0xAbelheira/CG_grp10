#include "../Utilities/utilities.hpp"
#include "../Utilities/matrix.hpp"
#include "catmullrom.hpp"
#include <math.h>

using namespace std;
using namespace matrixUtils;


void catmull::getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

        // catmull-rom matrix
        float M[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                        { 1.0f, -2.5f,  2.0f, -0.5f},
                        {-0.5f,  0.0f,  0.5f,  0.0f},
                        { 0.0f,  1.0f,  0.0f,  0.0f} };

        // T 
        float T[4] = { t * t * t, t * t, t, 1 };


        //P matrix that contains all componments of P{p0,p1,p2,p3}
        float P[3][4];
        for (int i = 0; i < 3; i++) {
            P[i][0] = p0[i];
            P[i][1] = p1[i];
            P[i][2] = p2[i];
            P[i][3] = p3[i];
        }


        // compute MP = M * P
        float MP[3][4];
        multMatrixVector((float*)M, P[0], MP[0]);
        multMatrixVector((float*)M, P[1], MP[1]);
        multMatrixVector((float*)M, P[2], MP[2]);


        // compute pos = T * A
        for (int i = 0; i < 3; i++) {
            pos[i] = 0;
            for (int j = 0; j < 4; j++)
                pos[i] += T[j] * MP[i][j];
        }

        // compute deriv = dT * A
        float dT[4] = { 3 * t * t, 2 * t, 1, 0 };
        for (int i = 0; i < 3; i++) {
            deriv[i] = 0;
            for (int j = 0; j < 4; j++)
                deriv[i] += dT[j] * MP[i][j];
        }
    }


// given global t, returns the point in the curve
 void catmull::getGlobalCatmullRomPoint(float glt, vector<utilities::point> ps, float* pos, float* deriv) {

        int POINT_COUNT = ps.size();
        float t = glt * POINT_COUNT; // this is the real global t
        int index = floor(t);  // which segment
        t = t - index; // where within  the segment

        // indices store the points
        int indices[4];
        indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
        indices[1] = (indices[0] + 1) % POINT_COUNT;
        indices[2] = (indices[1] + 1) % POINT_COUNT;
        indices[3] = (indices[2] + 1) % POINT_COUNT;

        float p[4][3];

        for (int i = 0; i < 4; i++) {
            p[i][0] = ps[indices[i]].x;
            p[i][1] = ps[indices[i]].y;
            p[i][2] = ps[indices[i]].z;
        }

        catmull::getCatmullRomPoint(t, p[0], p[1], p[2], p[3], pos, deriv);
    }