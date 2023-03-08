#include "calcVertices.cpp"
#include <string.h>

using namespace std;
using namespace generate;



int createFileType (vector<point> vertices, string name){

    fstream file;
    cout << utilities::getPath() << "asdasdasd" << endl;

    file.open(utilities::getPath() + name,ios::out);
    for(point p : vertices){
        file << p.x << " ";
        file << p.y << " ";
        file << p.z << " ";
        file << "\n";
    }

    if(!file){
        cout<<"\n       Error in creating file!!!\n";
        return -1;
    }
    cout<<"File created successfully.";
    file.close();
    return 0;
}

int main(int argc, char* argv[]) {
    figure f;

    if(argc<=1) cout << "Missing Arguments" << endl;
    else {

        //Gerar os vértices para o desenho do plano e transcrever para o ficheiro .3d
        if ((strcmp(argv[1], "Plane") == 0) && (argc == 5)) {

            float x = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int camadas = 0;
            aux >> camadas;

            f = createPlane(x, camadas);

            if (createFileType(f.points, argv[4]) == 0) {
                std::cout << "Done\n" << std::endl;
            }

        }

            //Gerar os vértices para o desenho do cubo/caixa e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "Box") == 0) && (argc == 5)) {
            float l = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int camadas = 0;
            aux >> camadas;

            f = createBox(l , camadas);

            if (createFileType(f.points, argv[4]) == 0) {
                std::cout << "Done\n" << std::endl;
            }
        }

            //Gerar os vértices para o desenho da esfera e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "Sphere") == 0) && (argc == 6)) {
            float radius = std::stof(argv[2]);
            stringstream aux(argv[3]);
            int slices = 0;
            aux >> slices;
            stringstream aux2(argv[4]);
            int stacks = 0;
            aux2 >> stacks;

            f = createSphere(radius, stacks, slices);

            if (createFileType(f.points, argv[5]) == 0) {
                //write_XML(argv[5],argv[6]);
                std::cout << "Done\n" << std::endl;
            }

        }

            //Gerar os vértices para o desenho do cone e transcrever para o ficheiro .3d
        else if ((strcmp(argv[1], "Cone") == 0) && (argc == 7)) {
            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            stringstream aux(argv[4]);
            int slices = 0;
            aux >> slices;
            stringstream aux2(argv[5]);
            int stacks = 0;
            aux2 >> stacks;

            f = createCone(radius, height, stacks, slices);

            if (createFileType(f.points, argv[6]) == 0) {
                //write_XML(argv[6],argv[7]);
                std::cout << "Done\n" << std::endl;
            }
        }

            //Tela de ajuda e comandos
        else if (strcmp(argv[1], "-help") == 0) {
            std::cout << "Plane         [x] [y] [file.3d] [file.xml]\n"
                         "Box           [x] [y] [z] [divisions per edge] [file.3d] [file.xml]\n"
                         "Sphere        [radius] [slices] [stacks] [file.3d] [file.xml]\n"
                         "Cone          [radius] [height] [stacks] [slices] [file.3d] [file.xml]\n" << std::endl;
        } else {
            std::cout << "\nMissing arguments\n" << std::endl;
        }
    }

    return 1;
}