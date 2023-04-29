#include <string.h>
#include "calcVertices.hpp"
#include "bezier.hpp"
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

figure createBezier(char* file, size_t t) {
    fstream f;
    f.open(getPath() + file);
    std::string line;      // String auxiliar que irá corresponder a uma 
    size_t numPatches = 0;
    size_t linhaNum = 0;
    std::string delimiter = ",";

    figure auxVertices;
    std::vector<size_t> auxIndices;
    if (f.is_open()) {
        while (getline(f, line)) {
            if (linhaNum == 0)
                numPatches = atoi(line.c_str());

            if (linhaNum >= 1 && linhaNum <= numPatches) {
                std::string token;
                size_t pos = 0;
                std::istringstream tokenizer(line);
                for (size_t i = 0; i < 15; i++) {
                    std::getline(tokenizer, token, ',');
                    auxIndices.push_back(stof(token));
                }
                std::getline(tokenizer, token);
                auxIndices.push_back(stof(token));
            }
            if (linhaNum > numPatches + 1) {
                std::string token;
                size_t pos = 0;
                size_t i = 0;
                float coord[3];
                string tokens[3];
                std::istringstream tokenizer(line);

                std::getline(tokenizer, tokens[0], ','); // then get the tokens from it
                std::getline(tokenizer, tokens[1], ',');
                std::getline(tokenizer, tokens[2]);

                auxVertices.addPoint(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
                
            }
            linhaNum++;
        }
    }
    else {
        std::cout << "File not found" << std::endl;
        figure fig;
        return fig;
    }
    f.close();
    
    
    figure fig = bezier::generateBezierPatches(auxVertices, auxIndices, t);

    return fig;
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

           //Gerar os vértices para o desenho do torus e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "torus") == 0) && (argc == 7)) {
            float rIn = std::stof(argv[2]);
            float rOut = std::stof(argv[3]);
            stringstream aux(argv[4]);
            int slices = 0;
            aux >> slices;
            stringstream aux2(argv[5]);
            int stacks = 0;
            aux2 >> stacks;

            f = createTorus(rIn, rOut, slices, stacks);

            createFileType(f.points, argv[6]);
                
        }

        //Gerar os vértices para o desenho da superficie e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "patch") == 0) && (argc == 5)) {
            char* patchFile = argv[2];
            stringstream aux(argv[3]);
            int tecelation = 0;
            aux >> tecelation;
            

            f = createBezier(patchFile,tecelation);
            if (!f.points.empty())
                createFileType(f.points, argv[4]);
        }

            //Tela de ajuda e comandos
        else if (strcmp(argv[1], "-help") == 0) {
            std::cout << "plane         [size] [divisions] [file.3d]\n"
                         "box           [size] [divisions per edge] [file.3d]\n"
                         "sphere        [radius] [slices] [stacks] [file.3d]\n"
                         "cone          [radius] [height] [slices] [stacks] [file.3d]\n"
                         "torus         [radius1] [radius2] [slices] [stacks] [file.3d]\n"
                         "patch         [file] [tecelation] [file.3d]" << std::endl;
        } else {
            std::cout << "\nInvalid arguments.\n" << std::endl;
        }
    }

    return 1;
}