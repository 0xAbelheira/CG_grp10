#define _USE_MATH_DEFINES
#include <cmath>

#include "calcVertices.hpp"

// Função que cria o Plano. Recebe o tamanho dele e o número de divisões (sub-quadrado)
figure generate::createPlane(float size, float nrDiv){
    float x, z, ax, az;
    x = z = (float)size/2;
    ax = az = (float)size/nrDiv;

    figure pl;

    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++) {

            pl.addPoint(-x ,0.0f, -z);
            pl.addPoint(-x,0.0f, -z + az);
            pl.addPoint(-x + ax,0.0f, -z);

            pl.addPoint(-x + ax ,0.0f, -z + az);
            pl.addPoint(-x + ax,0.0f, -z);
            pl.addPoint(-x,0.0f, -z + az);

            x -= ax;
        }
        z -= az;
        x += nrDiv*ax;
    }
    return pl;
}

// Função que cria a Box. Recebe o tamanho dela e o número de divisões (sub-quadrados) de cada face
figure generate::createBox(float size, float nrDiv){
    float x, y, z, ax, ay, az;
    x = y = z = (float)size/2;
    ax = ay = az = (float)size/nrDiv;

    figure box;

    // Piso da caixa
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x + ax, -y, -z);
            box.addPoint(-x, -y, -z + az);
            box.addPoint(-x , -y, -z);

            box.addPoint(-x, -y, -z + az);
            box.addPoint(-x + ax, -y, -z);
            box.addPoint(-x + ax , -y, -z + az);

            x -= ax;
        }
        z -= az;
        x += nrDiv*ax;
    }

    z += nrDiv*az;

    // Teto da caixa
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x , y, -z);
            box.addPoint(-x, y, -z + az);
            box.addPoint(-x + ax, y, -z);

            box.addPoint(-x + ax , y, -z + az);
            box.addPoint(-x + ax, y, -z);
            box.addPoint(-x, y, -z + az);

            x -= ax;
        }
        z -= az;
        x += nrDiv*ax;
    }

    z += nrDiv*az;

    // Parede da frente
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x, -y, z);
            box.addPoint(-x + ax, -y, z);
            box.addPoint(-x,-y + ay, z);

            box.addPoint(-x + ax, -y + ay,z);
            box.addPoint(-x, -y + ay, z);
            box.addPoint(-x + ax, -y, z);

            x -= ax;
        }
        y -= ay;
        x += nrDiv*ax;
    }

    y += nrDiv*ay;

    // Parede de trás
    for(int i = 0; i < nrDiv; i++) {
        for(int j = 0; j < nrDiv; j++) {

            box.addPoint(-x,-y + ay, -z);
            box.addPoint(-x + ax, -y, -z);
            box.addPoint(-x, -y, -z);

            box.addPoint(-x + ax, -y, -z);
            box.addPoint(-x, -y + ay, -z);
            box.addPoint(-x + ax, -y + ay,-z);

            x -= ax;
        }
        y -= ay;
        x += nrDiv*ax;
    }

    y += nrDiv*ay;

    // Parede da direita
    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++){

            box.addPoint(x, -y, -z);
            box.addPoint(x, -y + ay, -z);
            box.addPoint(x, -y, -z + az);

            box.addPoint(x, -y + ay, -z + az);
            box.addPoint(x, -y, -z + az);
            box.addPoint(x, -y + ay, -z);

            y -= ay;
        }
        z -= az;
        y += nrDiv*ay;
    }

    z += nrDiv*az;

    // Parede da esquerda
    for(int i = 0; i < nrDiv; i++){
        for(int j = 0; j < nrDiv; j++){

            box.addPoint(-x, -y, -z + az);
            box.addPoint(-x, -y + ay, -z);
            box.addPoint(-x, -y, -z);

            box.addPoint(-x, -y + ay, -z);
            box.addPoint(-x, -y, -z + az);
            box.addPoint(-x, -y + ay, -z + az);

            y -= ay;
        }
        z -= az;
        y += nrDiv*ay;
    }
    return box;
}

// Função que cria o Esfera. Recebe o raio, slices e stacks
figure generate::createSphere(float radius, int slices, int stacks){
    float alpha = (float)2 * M_PI / slices;
	float beta = (float)M_PI / stacks;

    figure sphere;

	for(int i = 0; i < slices; i++) {
		//glVertex3f(0, radius, 0);
		//glVertex3f(radius * sin(beta) * sin(alpha*i), radius * cos(beta), radius * sin(beta) * cos(alpha*i));
		//glVertex3f(radius * sin(beta) * sin(alpha*(i+1)), radius * cos(beta), radius * sin(beta) * cos(alpha*(i+1)));

		for(int j = 0; j < stacks; j++) {
			sphere.addPoint(radius * sin(beta*j) * sin(alpha*i), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*i), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*j) * sin(alpha*(i+1)), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*(i+1)));

			sphere.addPoint(radius * sin(beta*j) * sin(alpha*(i+1)), radius * cos(beta*j), radius * sin(beta*j) * cos(alpha*(i+1)));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*i), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*i));
			sphere.addPoint(radius * sin(beta*(j+1)) * sin(alpha*(i+1)), radius * cos(beta*(j+1)), radius * sin(beta*(j+1)) * cos(alpha*(i+1)));
		}

		//glVertex3f(radius * sin(beta) * sin(alpha*(i+1)), -radius * cos(beta), radius * sin(beta) * cos(alpha*(i+1)));
		//glVertex3f(radius * sin(beta) * sin(alpha*i), -radius * cos(beta), radius * sin(beta) * cos(alpha*i));
		//glVertex3f(0, -radius, 0);
	}
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

        for(int j = 0; j < stacks; j++) {
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
	}

    return cone;
}

/* ESPERAR PELA AULA PRÁTICA

// Função que cria a esfera 
figure generate::createSphere(float radius, int slices, int stacks){
    figure esf;

    float d1 = M_PI / stacks;
    float d2 = 2 * M_PI / slices;

    for (float i = -M_PI / 2; i < M_PI / 2; i += d1) {

        float aux1 = i + d1;


        for (float j = 0; j < 2 * M_PI - d2; j += d2) {

            float aux2 = j + d2;

            //Triângulo 1
            esf.addPoint(cos(aux1) * sin(j) * radius, sin(aux1) * radius, cos(aux1)* cos(j)* radius);
            esf.addPoint(cos(i) * sin(j) * radius, sin(i) * radius, cos(i) * cos(j) * radius);
            esf.addPoint(cos(i) * sin(aux2) * radius,sin(i) * radius, cos(i) * cos(aux2) * radius);

            //Triângulo 2
            esf.addPoint(cos(aux1) * sin(j) * radius, sin(aux1) * radius, cos(aux1)* cos(j)* radius);
            esf.addPoint(cos(i) * sin(aux2) * radius, sin(i) * radius, cos(i)* cos(aux2)* radius);
            esf.addPoint(cos(aux1) * sin(aux2) * radius, sin(aux1) * radius, cos(aux1)* cos(aux2)* radius);

        }
    }

    return esf;
}

figure generate::createCone(float radius, float height, int slices, int stacks) {
    figure f;

    float theta = 0;
    float nextTheta = 0;
    float delta = (2 * M_PI) / slices;
    float raio = radius / stacks;
    float alturas = height / stacks;
    int i, j;

    //fazer a circunferência da base
    for (i = 0; i < slices; i++) {

        nextTheta = theta + delta;

        f.addPoint(0, 0, 0);
        f.addPoint(radius * sin(nextTheta), 0, radius * cos(nextTheta));
        f.addPoint(radius * sin(theta), 0, cos(theta));

        theta = nextTheta;
    }

    // Fazer as laterais
    float r1 = radius;
    float r2 = radius - raio;
    float alt1 = 0;
    float alt2 = alturas;
    theta = 0;
    nextTheta = 0;

    for (i = 0; i < slices; i++) {

        nextTheta += delta;

        for (j = 0; j < stacks; j++) {

            f.addPoint(r1 * sin(nextTheta), alt1, r1 * cos(nextTheta));
            f.addPoint(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta));
            f.addPoint(r1 * sin(theta), alt1, r1 * cos(theta));

            f.addPoint(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta));
            f.addPoint(r2 * sin(theta), alt2, r2 * cos(theta));
            f.addPoint(r1 * sin(theta), alt1, r1 * cos(theta));

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
    return cone;
}
*/