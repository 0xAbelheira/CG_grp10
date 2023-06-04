#define _USE_MATH_DEFINES
#include <cmath>

#include "calcVertices.hpp"

// Função que cria o Plano. Recebe o tamanho dele e o número de divisões (sub-quadrado)
figure generate::createPlane(float size, int nrDiv){
    float x, z, av;
    x = z = (float)size/2;
    av = (float)size/nrDiv;

    figure pl;

    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++) {

            pl.addPoint(-x ,0.0f, -z);
            pl.addPoint(-x,0.0f, -z + av);
            pl.addPoint(-x + av,0.0f, -z);

            pl.addPoint(-x + av ,0.0f, -z + av);
            pl.addPoint(-x + av,0.0f, -z);
            pl.addPoint(-x,0.0f, -z + av);

            pl.addNormal(0, 1, 0);
            pl.addNormal(0, 1, 0);
            pl.addNormal(0, 1, 0);
            pl.addNormal(0, 1, 0);
            pl.addNormal(0, 1, 0);
            pl.addNormal(0, 1, 0);

            x -= av;
        }
        z -= av;
        x += nrDiv*av;
    }

    pl.calTextPlain(size);

    return pl;
}

// Função que cria a Box. Recebe o tamanho dela e o número de divisões (sub-quadrados) de cada face
figure generate::createBox(float size, int nrDiv){
    float x, y, z, av;
    x = y = z = (float)size/2;
    av = (float)size/nrDiv;

    figure box;

    float baseH = 0;
    float baseV = 0;
    float hor = 1.0f / 3.0f;
    float ver = 1.0f / 2.0f;
    float textH = hor / (float)nrDiv;
    float textV = ver / (float)nrDiv;


    // Piso da caixa
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x , -y, -z);

            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x + av , -y, -z + av);

            box.addNormal(0, -1, 0);
            box.addNormal(0, -1, 0);
            box.addNormal(0, -1, 0);
            box.addNormal(0, -1, 0);
            box.addNormal(0, -1, 0);
            box.addNormal(0, -1, 0);

            x -= av;
        }
        z -= av;
        x += nrDiv*av;
    }

    z += nrDiv*av;


    // Teto da caixa
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x , y, -z);
            box.addPoint(-x, y, -z + av);
            box.addPoint(-x + av, y, -z);

            box.addPoint(-x + av , y, -z + av);
            box.addPoint(-x + av, y, -z);
            box.addPoint(-x, y, -z + av);

            box.addNormal(0, 1, 0);
            box.addNormal(0, 1, 0);
            box.addNormal(0, 1, 0);
            box.addNormal(0, 1, 0);
            box.addNormal(0, 1, 0);
            box.addNormal(0, 1, 0);

            x -= av;
        }
        z -= av;
        x += nrDiv*av;
    }

    z += nrDiv*av;

    // Parede da frente
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x, -y, z);
            box.addPoint(-x + av, -y, z);
            box.addPoint(-x,-y + av, z);

            box.addPoint(-x + av, -y + av,z);
            box.addPoint(-x, -y + av, z);
            box.addPoint(-x + av, -y, z);

            box.addNormal(0, 0, 1);
            box.addNormal(0, 0, 1);
            box.addNormal(0, 0, 1);
            box.addNormal(0, 0, 1);
            box.addNormal(0, 0, 1);
            box.addNormal(0, 0, 1);

            x -= av;
        }
        y -= av;
        x += nrDiv*av;
    }

    y += nrDiv*av;

    // Parede de trás
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x,-y + av, -z);
            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x, -y, -z);

            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x, -y + av, -z);
            box.addPoint(-x + av, -y + av,-z);

            box.addNormal(0, 0, -1);
            box.addNormal(0, 0, -1);
            box.addNormal(0, 0, -1);
            box.addNormal(0, 0, -1);
            box.addNormal(0, 0, -1);
            box.addNormal(0, 0, -1);

            x -= av;
        }
        y -= av;
        x += nrDiv*av;
    }

    y += nrDiv*av;

    // Parede da direita
    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++){

            box.addPoint(x, -y, -z);
            box.addPoint(x, -y + av, -z);
            box.addPoint(x, -y, -z + av);

            box.addPoint(x, -y + av, -z + av);
            box.addPoint(x, -y, -z + av);
            box.addPoint(x, -y + av, -z);

            box.addNormal(1, 0, 0);
            box.addNormal(1, 0, 0);
            box.addNormal(1, 0, 0);
            box.addNormal(1, 0, 0);
            box.addNormal(1, 0, 0);
            box.addNormal(1, 0, 0);

            y -= av;
        }
        z -= av;
        y += nrDiv*av;
    }

    z += nrDiv*av;

    // Parede da esquerda
    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++){

            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x, -y + av, -z);
            box.addPoint(-x, -y, -z);

            box.addPoint(-x, -y + av, -z);
            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x, -y + av, -z + av);

            box.addNormal(-1, 0, 0);
            box.addNormal(-1, 0, 0);
            box.addNormal(-1, 0, 0);
            box.addNormal(-1, 0, 0);
            box.addNormal(-1, 0, 0);
            box.addNormal(-1, 0, 0);

            y -= av;
        }
        z -= av;
        y += nrDiv*av;
    }

    box.calTextBox(size);

    return box;
}

// Função que cria o Esfera. Recebe o raio, slices e stacks
figure generate::createSphere(float radius, int slices, int stacks){
    figure f;

    float delta1 = M_PI / stacks;
    float delta2 = 2 * M_PI / slices;
    float theta1 = -M_PI / 2;
    float theta2 = 0.0f;
    int indexCount = 0;
    float textH = 1.0f / (float)slices;
    float textV = 1.0f / (float)stacks;

    for (int i = 0; i < stacks; i++) {

        for (int j = 0; j < slices; j++) {
            theta2 = j * delta2;
            f.addPoint(cos(theta1 + delta1) * sin(theta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2) * radius);
            f.addPoint(cos(theta1) * sin(theta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2) * radius);
            f.addPoint(cos(theta1) * sin(theta2 + delta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2 + delta2) * radius);

            f.addPoint(cos(theta1 + delta1) * sin(theta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2) * radius);
            f.addPoint(cos(theta1) * sin(theta2 + delta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2 + delta2) * radius);
            f.addPoint(cos(theta1 + delta1) * sin(theta2 + delta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2 + delta2) * radius);

            f.addNormal(cos(theta1 + delta1) * sin(theta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2) * radius);
            f.addNormal(cos(theta1) * sin(theta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2) * radius);
            f.addNormal(cos(theta1) * sin(theta2 + delta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2 + delta2) * radius);
            f.addNormal(cos(theta1 + delta1) * sin(theta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2) * radius);
            f.addNormal(cos(theta1) * sin(theta2 + delta2) * radius, sin(theta1) * radius, cos(theta1) * cos(theta2 + delta2) * radius);
            f.addNormal(cos(theta1 + delta1) * sin(theta2 + delta2) * radius, sin(theta1 + delta1) * radius, cos(theta1 + delta1) * cos(theta2 + delta2) * radius);

            f.addText(j * textH, i * textV + textV, 0);
            f.addText(j * textH, i * textV, 0);
            f.addText(j * textH + textH, i * textV, 0);
            f.addText(j * textH, i * textV + textV, 0);
            f.addText(j * textH + textH, i * textV, 0);
            f.addText(j * textH + textH, i * textV + textV, 0);
        }
        theta1 += delta1;
    }
    
    return f;
}

// Função que cria o Cone. Recebe o raio da base, altura, slices e stacks
figure generate::createCone(float radius, float height, int slices, int stacks){
	 figure f;

    float theta = 0;
    float nextTheta = 0;
    float delta = (2 * M_PI) / slices;
    float raio = radius / stacks;
    float alturas = height / stacks;
    int i, j;

    float textH = 1.0f / slices;
    float textV = 1.0f / stacks;

    float var = (2 * radius * M_PI) / slices;


    for (i = 0; i < slices; i++) {

        nextTheta = theta + delta;

        f.addPoint(0, 0, 0);
        f.addPoint(radius * sin(nextTheta), 0, radius * cos(nextTheta));
        f.addPoint(radius * sin(theta), 0, radius * cos(theta));

        f.addNormal(0, -1, 0);
        f.addNormal(0, -1, 0);
        f.addNormal(0, -1, 0);

        f.addText(var / 2, 1, 0);
        f.addText(i * var, 0, 0);
        f.addText((i + 1) * var, 0, 0);
        
        theta = nextTheta;
    }

    // Fazer as laterais
    float r1 = radius;
    float r2 = radius - raio;
    float alt1 = 0;
    float alt2 = alturas;
    theta = 0;
    nextTheta = 0;
    float ny = cos(atan(height / radius));

    for (i = 0; i < slices; i++) {

        nextTheta += delta;

        for (j = 0; j < stacks; j++) {
            f.addPoint(r1 * sin(nextTheta), alt1, r1 * cos(nextTheta));
            f.addPoint(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta));
            f.addPoint(r1 * sin(theta), alt1, r1 * cos(theta));

            f.addPoint(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta));
            f.addPoint(r2 * sin(theta), alt2, r2 * cos(theta));
            f.addPoint(r1 * sin(theta), alt1, r1 * cos(theta));

            f.addText((i + 1) * textH, j * textV, 0);
            f.addText((i + 1) * textH, (j + 1) * textV, 0);
            f.addText(i * textH, j * textV, 0);
            f.addText((i + 1) * textH, (j + 1) * textV, 0);
            f.addText(i * textH, (j + 1) * textV, 0);
            f.addText(i * textH, j * textV, 0);

            f.addNormal(sin(nextTheta), ny, cos(nextTheta));
            f.addNormal(sin(nextTheta), ny, cos(nextTheta));
            f.addNormal(sin(theta), ny, cos(theta));

            f.addNormal(sin(nextTheta), ny, cos(nextTheta));
            f.addNormal(sin(theta), ny, cos(theta));
            f.addNormal(sin(theta), ny, cos(theta));


            r1 -= raio;
            r2 -= raio;
            alt1 += alturas;
            alt2 += alturas;
        }
        r1 = radius;
        r2 = radius - raio;
        alt1 = 0;
        alt2 = alturas;
        theta = nextTheta;
    }
    return f;
}

figure generate::createTorus(float rIn, float rOut, int slices, int stacks) {

    float alpha = 2*M_PI/slices;
    float beta = 2*M_PI/stacks;

    figure torus;

    for(int i = 0; i < slices; i++) {
        for(int j = 0; j < stacks; j++) {
            torus.addPoint((rIn + rOut*cos(beta*(j+1))) * cos(alpha*(i+1)), rOut*sin(beta*(j+1)), (rIn + rOut*cos(beta*(j+1))) * sin(alpha*(i+1)));
            torus.addPoint((rIn + rOut*cos(beta*j)) * cos(alpha*(i+1)), rOut*sin(beta*j), (rIn + rOut*cos(beta*j)) * sin(alpha*(i+1)));
            torus.addPoint((rIn + rOut*cos(beta*j)) * cos(alpha*i), rOut*sin(beta*j), (rIn + rOut*cos(beta*j)) * sin(alpha*i));

            torus.addNormal(cos(beta*(j+1)) * cos(alpha*(i+1)), sin(beta*(j+1)), cos(beta*(j+1)) * sin(alpha*(i+1)));
            torus.addNormal(cos(beta*j) * cos(alpha*(i+1)), sin(beta*j), cos(beta*j) * sin(alpha*(i+1)));
            torus.addNormal(cos(beta*j) * cos(alpha*i), sin(beta*j), cos(beta*j) * sin(alpha*i));

            torus.addPoint((rIn + rOut*cos(beta*j)) * cos(alpha*i), rOut*sin(beta*j), (rIn + rOut*cos(beta*j)) * sin(alpha*i));
            torus.addPoint((rIn + rOut*cos(beta*(j+1))) * cos(alpha*i), rOut*sin(beta*(j+1)), (rIn + rOut*cos(beta*(j+1))) * sin(alpha*i));
            torus.addPoint((rIn + rOut*cos(beta*(j+1))) * cos(alpha*(i+1)), rOut*sin(beta*(j+1)), (rIn + rOut*cos(beta*(j+1))) * sin(alpha*(i+1)));

            torus.addNormal(cos(beta*j) * cos(alpha*i), sin(beta*j), cos(beta*j) * sin(alpha*i));
            torus.addNormal(cos(beta*(j+1)) * cos(alpha*i), sin(beta*(j+1)), cos(beta*(j+1)) * sin(alpha*i));
            torus.addNormal(cos(beta*(j+1)) * cos(alpha*(i+1)), sin(beta*(j+1)), cos(beta*(j+1)) * sin(alpha*(i+1)));
        }
    }

    return torus;
}