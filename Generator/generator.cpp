#include "../tinyXML/tinyxml.h"
#include "calculaVertices.hpp"

using namespace std;
using namespace generate;
TiXmlDocument doc;

int write_XML(string file, string fxml) {

    string xml = getPath() + fxml;

    if(!(doc.LoadFile(xml.c_str()))){
        TiXmlElement *element = new TiXmlElement("Scene");
        doc.LinkEndChild(element);
        TiXmlElement *element2 = new TiXmlElement("Model");
        element2->SetAttribute("file", file.c_str());
        element->LinkEndChild(element2);
        std::cout <<"\nXML File Created.\n"<< std::endl;
    }
    else{
        TiXmlHandle docHandle(&doc);
        TiXmlElement* fileLog = docHandle.FirstChild("Scene").ToElement();
        TiXmlElement* root = doc.RootElement();
        if (fileLog) {
            /*
            //Verifica se já existe uma entrada para esse mesmo ficheiro .3d no XML
            for(TiXmlElement* aux = root->FirstChild()->ToElement(); aux!=nullptr; aux = aux->NextSiblingElement()) {
                const char* ficheiro1 = aux->Attribute("file");
                if ( strcmp( ficheiro1, file.c_str()) == 0 ){
                    std::cout <<"\nFicheiro já existente atualizado em XML\n"<< std::endl;
                    return -1;
                }
            }*/
            TiXmlElement newCategory2("Model");
            newCategory2.SetAttribute("file", file.c_str());
            fileLog->InsertEndChild(newCategory2);
        }
    }
    doc.SaveFile(xml.c_str());
    return 0;
}


int createFileType (vector<point> vertices, string name){

    fstream file;

    // in out(write) mode
    // ios::out Open for output operations.
    file.open(getPath() + name,ios::out);
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
    
    return 0;
}