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
	float alpha = 2 * M_PI / slices;
    float hTop, hLow;
    float difH = height/stacks;
    float difR = radius/stacks;
    float rTop, rLow;
    float ny = cos(atan(height / radius));

    figure cone;

	for(int i = 0; i < slices; i++) {
        hLow = 0;
        rLow = radius;
        //base
		cone.addPoint(radius*sin(alpha*(i+1)), 0, radius*cos(alpha*(i+1)));
		cone.addPoint(radius*sin(alpha*i), 0, radius*cos(alpha*i));
		cone.addPoint(0, 0, 0);

        cone.addNormal(0, -1, 0);
        cone.addNormal(0, -1, 0);
        cone.addNormal(0, -1, 0);

        cone.addText((i+1)/slices,0,0);
        cone.addText(i/slices,0,0);
        cone.addText(i/slices,0,0);

        for(int j = 0; j < stacks-1; j++) {
            rTop = rLow - difR;
            hTop = hLow + difH;

		    cone.addPoint(rLow*sin(alpha*i), hLow, rLow*cos(alpha*i));
            cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
		    cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));

            cone.addNormal(sin(alpha*i), ny, cos(alpha*i));
            cone.addNormal(sin(alpha*(i+1)), ny, cos(alpha*(i+1)));
		    cone.addNormal(sin(alpha*i), ny, cos(alpha*i));

            cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
            cone.addPoint(rTop*sin(alpha*(i+1)), hTop, rTop*cos(alpha*(i+1)));
            cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));

            cone.addNormal(sin(alpha*(i+1)), ny, cos(alpha*(i+1)));
            cone.addNormal(sin(alpha*(i+1)), ny, cos(alpha*(i+1)));
            cone.addNormal(sin(alpha*i), ny, cos(alpha*i));

            cone.addText(i/slices, (j+1)/stacks,0);
            cone.addText((i+1)/slices, (j+1)/stacks,0);
            cone.addText(i/slices, (j+2)/stacks,0);

            cone.addText((i+1)/slices, (j+1)/stacks,0);
            cone.addText((i+1)/slices, (j+2)/stacks,0);
            cone.addText(i/slices, (j+2)/stacks,0);

            rLow = rTop;
            hLow += difH;
        }

        rTop = rLow - difR;
        hTop = hLow + difH;

        cone.addPoint(rLow*sin(alpha*i), hLow, rLow*cos(alpha*i));
        cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
		cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));

        cone.addNormal(sin(alpha*i), ny, cos(alpha*i));
        cone.addNormal(sin(alpha*(i+1)), ny, cos(alpha*(i+1)));
		cone.addNormal(sin(alpha*i), ny, cos(alpha*i));

        cone.addText(i/slices, (stacks-1)/stacks,0);
        cone.addText((i+1)/slices, (stacks-1)/stacks,0);
        cone.addText(i/slices, 1,0);
	}

    return cone;
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