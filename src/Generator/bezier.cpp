#include "../Utilities/matrix.hpp"
#include "../Utilities/utilities.hpp"
#include "bezier.hpp"

using namespace matrixUtils;


void bezier::getBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos) {

    // bezier matrix
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vetorU[4] = { u * u * u, u * u, u, 1 };
    float vetorV[4] = { v * v * v, v * v, v, 1 };

    float vetorAux[4];
    float px[4];
    float py[4];
    float pz[4];

    float mx[4];
    float my[4];
    float mz[4];

    multMatrixVector((float*)bezierMatrix, vetorV, vetorAux);
    multMatrixVector((float*)matrixX, vetorAux, px);
    multMatrixVector((float*)matrixY, vetorAux, py);
    multMatrixVector((float*)matrixZ, vetorAux, pz);

    multMatrixVector((float*)bezierMatrix, px, mx);
    multMatrixVector((float*)bezierMatrix, py, my);
    multMatrixVector((float*)bezierMatrix, pz, mz);


    pos[0] = (vetorU[0] * mx[0]) + (vetorU[1] * mx[1]) + (vetorU[2] * mx[2]) + (vetorU[3] * mx[3]);
    pos[1] = (vetorU[0] * my[0]) + (vetorU[1] * my[1]) + (vetorU[2] * my[2]) + (vetorU[3] * my[3]);
    pos[2] = (vetorU[0] * mz[0]) + (vetorU[1] * mz[1]) + (vetorU[2] * mz[2]) + (vetorU[3] * mz[3]);

}

void bezier::getBezierNormalPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos) {
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vetorU[4] = { u * u * u, u * u, u, 1 };
    float vetorV[4] = { v * v * v, v * v, v, 1 };

    float vetorDerivU[4] = { 3 * u * u, 2 * u, 1, 0 };
    float vetorDerivV[4] = { 3 * v * v, 2 * v, 1, 0 };

    float vetorAux[4];
    float px[4];
    float py[4];
    float pz[4];

    float mxAux[4];
    float myAux[4];
    float mzAux[4];

    float uRes[3];

    //Calcular u
    

    multMatrixVector((float*)bezierMatrix, vetorDerivV, vetorAux);
    multMatrixVector((float*)matrixX, vetorAux, px);
    multMatrixVector((float*)matrixY, vetorAux, py);
    multMatrixVector((float*)matrixZ, vetorAux, pz);

    multMatrixVector((float*)bezierMatrix, px, mxAux);
    multMatrixVector((float*)bezierMatrix, py, myAux);
    multMatrixVector((float*)bezierMatrix, pz, mzAux);

    uRes[0] = (mxAux[0] * vetorU[0]) + (mxAux[1] * vetorU[1]) + (mxAux[2] * vetorU[2]) + (mxAux[3] * vetorU[3]);
    uRes[2] = (mzAux[0] * vetorU[0]) + (mzAux[1] * vetorU[1]) + (mzAux[2] * vetorU[2]) + (mzAux[3] * vetorU[3]);
    uRes[1] = (myAux[0] * vetorU[0]) + (myAux[1] * vetorU[1]) + (myAux[2] * vetorU[2]) + (myAux[3] * vetorU[3]);


    //Calcular v
    float vetorAux2[4];
    float px2[4];
    float py2[4];
    float pz2[4];

    float mxAux2[4];
    float myAux2[4];
    float mzAux2[4];

    float vRes[3];

    multMatrixVector((float*)bezierMatrix, vetorV, vetorAux2);
    multMatrixVector((float*)matrixX, vetorAux2, px2);
    multMatrixVector((float*)matrixY, vetorAux2, py2);
    multMatrixVector((float*)matrixZ, vetorAux2, pz2);

    multMatrixVector((float*)bezierMatrix, px2, mxAux2);
    multMatrixVector((float*)bezierMatrix, py2, myAux2);
    multMatrixVector((float*)bezierMatrix, pz2, mzAux2);
    
    vRes[0] = (mxAux2[0] * vetorDerivU[0]) + (mxAux2[1] * vetorDerivU[1]) + (mxAux2[2] * vetorDerivU[2]) + (mxAux2[3] * vetorDerivU[3]);
    vRes[1] = (myAux2[0] * vetorDerivU[0]) + (myAux2[1] * vetorDerivU[1]) + (myAux2[2] * vetorDerivU[2]) + (myAux2[3] * vetorDerivU[3]);
    vRes[2] = (mzAux2[0] * vetorDerivU[0]) + (mzAux2[1] * vetorDerivU[1]) + (mzAux2[2] * vetorDerivU[2]) + (mzAux2[3] * vetorDerivU[3]);

    cross(uRes, vRes, pos);
    normalize(pos);

}


figure bezier::generateBezierPatches(figure pVertices, std::vector<size_t> pIndexes, size_t tecelagem) {
    figure fig;
    float pos[4][3];
    float matrixX[4][4];
    float matrixY[4][4];
    float matrixZ[4][4];

    float u = 0;
    float v = 0;
    float inc = 1 / (float)tecelagem;

    for (size_t p = 0; p < pIndexes.size(); p += 16) {
        for (size_t i = 0; i < tecelagem; i++) {
            for (size_t j = 0; j < tecelagem; j++) {
                u = inc * i;
                v = inc * j;
                float u2 = inc * (i + 1);
                float v2 = inc * (j + 1);


                for (size_t a = 0; a < 4; a++) {
                    for (size_t b = 0; b < 4; b++) {

                        matrixX[a][b] = pVertices.points.at(pIndexes.at(p + a * 4 + b)).x;
                        matrixY[a][b] = pVertices.points.at(pIndexes.at(p + a * 4 + b)).y;
                        matrixZ[a][b] = pVertices.points.at(pIndexes.at(p + a * 4 + b)).z;

                    }
                }

                getBezierPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[0]);
                getBezierPoint(u, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[1]);
                getBezierPoint(u2, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[2]);
                getBezierPoint(u2, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[3]);

                fig.addPoint(pos[3][0], pos[3][1], pos[3][2]);
                fig.addPoint(pos[2][0], pos[2][1], pos[2][2]);
                fig.addPoint(pos[0][0], pos[0][1], pos[0][2]);

                fig.addPoint(pos[0][0], pos[0][1], pos[0][2]);
                fig.addPoint(pos[1][0], pos[1][1], pos[1][2]);
                fig.addPoint(pos[3][0], pos[3][1], pos[3][2]);

                getBezierNormalPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[0]);
                getBezierNormalPoint(u, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[1]);
                getBezierNormalPoint(u2, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[2]);
                getBezierNormalPoint(u2, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[3]);


                fig.addNormal(pos[3][0], pos[3][1], pos[3][2]);
                fig.addNormal(pos[2][0], pos[2][1], pos[2][2]);
                fig.addNormal(pos[0][0], pos[0][1], pos[0][2]);

                fig.addNormal(pos[0][0], pos[0][1], pos[0][2]);
                fig.addNormal(pos[1][0], pos[1][1], pos[1][2]);
                fig.addNormal(pos[3][0], pos[3][1], pos[3][2]);
            }
        }
    }

    return fig;

}