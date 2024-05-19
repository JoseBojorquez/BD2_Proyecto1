#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

struct Record{
    int id;
    char dato[10];
    int nextpos;
    bool principal;

    void setData(int id, char* dato1){
        this->id = id;
        strcpy(dato,dato1);
        this->nextpos = -1;
        this->principal = true;
    }
    void showData(){
        cout << id << endl;
        cout << dato << endl;
        cout << nextpos << endl;
        cout << principal << endl;
    }
};

struct Bloque{
    vector<Record> registros;
    int M = 10;
};

struct SeqFile{
    string filename;
    string auxname;
    SeqFile(string filename){
        this->filename = filename;
        this->auxname = "aux" + filename;
        fstream file(filename, ios::in | ios::binary);
        file.close();
        if(!file.good()){
            fstream file(filename, ios::out | ios::binary);
            file.close();
        }
    }
    Record find(int key){
        fstream file(filename, ios::in | ios::binary);
        if(file.good()){
            int l = 0;
            file.seekg(0,ios::end);
            int u = (file.tellg()/sizeof(Record)) - 1;
            while(u >= l){
                int m = (l + u)/2;
                Record r;
                file.seekg(m*sizeof(Record), ios::beg);
                file.read((char*) &r, sizeof(Record));
                if(r.id < key) l = m + 1;
                else if(r.id > key) u = m - 1;
                else return r;
            }
        }
        file.close();
        file.open(auxname, ios::in | ios::binary);
        Bloque bloque;
        Record res;
        file.read((char*) &bloque, sizeof(Bloque));
        for(int i = 0; i < bloque.registros.size(); i++){
            if(bloque.registros[i].id == key){
                res = bloque.registros[i];
                return res;
            }
        }
        return res;
    }
    void insert(Record reg){
        fstream file(filename, ios::in | ios::binary);
        Record r;
        long opos = 0;
        while(file.read((char*) &r, sizeof(Record))){
            if(r.id > reg.id) break; // manejo de archivo auxiliar
            opos = file.tellg();
        }
        opos = opos - sizeof(Record);
        file.seekg(opos, ios::beg);
        file.read((char*) &r, sizeof(Record));
        r.nextpos = (opos + sizeof(Record))/sizeof(Record);
        file.close();
        file.open(filename, ios::out | ios::binary);
        file.seekp(opos, ios::beg);
        file.write((char*) &r, sizeof(Record));
        file.close();
        file.open(filename, ios::app | ios::binary);
        file.seekg(0, ios::end);
        cout << file.tellp() << endl;
        file.write((char*) &reg, sizeof(Record));
    }
};

int main(){
    SeqFile seq("datos.dat");
    Record r1;
    Record r2;
    Record r3;
    Record r4;
    r1.setData(1,(char*)"aaa");
    r2.setData(2,(char*)"bbb");
    r3.setData(3,(char*)"ccc");
    r4.setData(4,(char*)"ddd");
    seq.insert(r1);
    seq.insert(r2);
    seq.insert(r3);
    seq.insert(r4);
    Record r = seq.find(3);
    r.showData();
}
