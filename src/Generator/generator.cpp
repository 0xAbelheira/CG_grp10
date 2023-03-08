#include "calcVertices.cpp"

using namespace std;
using namespace generate;

int createFileType (vector<point> vertices, string name){

    fstream file;

    // in out(write) mode
    // ios::out Open for output operations.
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
    
    return 0;
}