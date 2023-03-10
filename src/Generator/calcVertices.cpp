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

            x -= av;
        }
        z -= av;
        x += nrDiv*av;
    }
    return pl;
}

// Função que cria a Box. Recebe o tamanho dela e o número de divisões (sub-quadrados) de cada face
figure generate::createBox(float size, int nrDiv){
    float x, y, z, av;
    x = y = z = (float)size/2;
    av = (float)size/nrDiv;

    figure box;

    // Piso da caixa
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x , -y, -z);

            box.addPoint(-x, -y, -z + av);
            box.addPoint(-x + av, -y, -z);
            box.addPoint(-x + av , -y, -z + av);

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

            y -= av;
        }
        z -= av;
        y += nrDiv*av;
    }
    return box;
}

// Função que cria o Esfera. Recebe o raio, slices e stacks
figure generate::createSphere(float radius, int slices, int stacks){
    float alpha = (float)2 * M_PI / slices;
	float beta = (float)M_PI / stacks;

    figure sphere;
    //Desenhar apenas um triangulo nas extremidades para evitar pontos desnecessarios

	for(int i = 0; i < slices; i++) {
        //topo
		sphere.addPoint(0, radius, 0);
		sphere.addPoint(radius * sin(beta) * sin(alpha*i), radius * cos(beta), radius * sin(beta) * cos(alpha*i));
		sphere.addPoint(radius * sin(beta) * sin(alpha*(i+1)), radius * cos(beta), radius * sin(beta) * cos(alpha*(i+1)));

		for(int j = 1; j < stacks-1; j++) {
			sphere.addPoint(radius * sin(beta*j) * sin(alpha*i), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*i), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*j) * sin(alpha*(i+1)), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*(i+1)));

			sphere.addPoint(radius * sin(beta*j) * sin(alpha*(i+1)), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*(i+1)));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*i), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*(i+1)), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*(i+1)));
		}

        //parte baixo
		sphere.addPoint(radius * sin(beta) * sin(alpha*(i+1)), -radius * cos(beta), radius * sin(beta) * cos(alpha*(i+1)));
		sphere.addPoint(radius * sin(beta) * sin(alpha*i), -radius * cos(beta), radius * sin(beta) * cos(alpha*i));
		sphere.addPoint(0, -radius, 0);
	}

    return sphere;
}

// Função que cria o Cone. Recebe o raio da base, altura, slices e stacks
figure generate::createCone(float radius, float height, int slices, int stacks){
	float alpha = 2 * M_PI / slices;
    float hTop, hLow;
    float difH = height/stacks;
    float difR = radius/stacks;
    float rTop, rLow;

    figure cone;

	for(int i = 0; i < slices; i++) {
        hLow = 0;
        rLow = radius;
        //base
		cone.addPoint(radius*sin(alpha*(i+1)), 0, radius*cos(alpha*(i+1)));
		cone.addPoint(radius*sin(alpha*i), 0, radius*cos(alpha*i));
		cone.addPoint(0, 0, 0);

        for(int j = 0; j < stacks-1; j++) {
            rTop = rLow - difR;
            hTop = hLow + difH;

		    cone.addPoint(rLow*sin(alpha*i), hLow, rLow*cos(alpha*i));
            cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
		    cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));

            cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
            cone.addPoint(rTop*sin(alpha*(i+1)), hTop, rTop*cos(alpha*(i+1)));
            cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));
            rLow = rTop;
            hLow += difH;
        }

        rTop = rLow - difR;
        hTop = hLow + difH;

        cone.addPoint(rLow*sin(alpha*i), hLow, rLow*cos(alpha*i));
        cone.addPoint(rLow*sin(alpha*(i+1)), hLow, rLow*cos(alpha*(i+1)));
		cone.addPoint(rTop*sin(alpha*i), hTop, rTop*cos(alpha*i));
	}

    return cone;
}