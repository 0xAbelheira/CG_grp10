#include <string.h>
#include "calcVertices.hpp"
#include <iostream>

using namespace std;
using namespace generate;


int createFileType (vector<point> vertices, string name){

    fstream file;

    file.open(utilities::getPath() + name,ios::out);
    for(point p : vertices){
        file << p.x << " ";
        file << p.y << " ";
        file << p.z << " ";
        file << "\n";
    }

    if(!file){
        cout<<"\n!!!Error in creating file!!!\n";
        return -1;
    }
    cout<<"File created successfully!" << endl;
    file.close();
    return 0;
}

int main(int argc, char* argv[]) {
    figure f;

    if(argc<=1) cout << "Missing Arguments.." << endl;
    else {

        //Gerar os vértices para o desenho do plano e transcrever para o ficheiro .3d
        if ((strcmp(argv[1], "plane") == 0) && (argc == 5)) {

            float x = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int camadas = 0;
            aux >> camadas;

            f = createPlane(x, camadas);
            createFileType(f.points, argv[4]);
        }

            //Gerar os vértices para o desenho do cubo/caixa e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "box") == 0) && (argc == 5)) {
            float l = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int camadas = 0;
            aux >> camadas;

            f = createBox(l , camadas);
            createFileType(f.points, argv[4]);
        }

            //Gerar os vértices para o desenho da esfera e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "sphere") == 0) && (argc == 6)) {
            float radius = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int slices = 0;
            aux >> slices;
            stringstream aux2(argv[4]);
            int stacks = 0;
            aux2 >> stacks;

            f = createSphere(radius, slices, stacks);

            createFileType(f.points, argv[5]);

        }

            //Gerar os vértices para o desenho do cone e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "cone") == 0) && (argc == 7)) {
            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            stringstream aux(argv[4]);
            int slices = 0;
            aux >> slices;
            stringstream aux2(argv[5]);
            int stacks = 0;
            aux2 >> stacks;

            f = createCone(radius, height, slices, stacks);

            createFileType(f.points, argv[6]);
                
        }

            //Tela de ajuda e comandos
        else if (strcmp(argv[1], "-help") == 0) {
            std::cout << "plane         [size] [divisions] [file.3d]\n"
                         "box           [size] [divisions per edge] [file.3d]\n"
                         "sphere        [radius] [slices] [stacks] [file.3d]\n"
                         "cone          [radius] [height] [slices] [stacks] [file.3d]\n" << std::endl;
        } else {
            std::cout << "\nInvalid arguments.\n" << std::endl;
        }
    }

    return 1;
}