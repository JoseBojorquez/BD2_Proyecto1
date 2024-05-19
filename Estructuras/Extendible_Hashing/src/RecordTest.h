#include "../utils/libs.h"

struct TestRecord{

    int codigo;
    char nombre[30];
    long telefono;


    void set_data_from_string(string _params) {
        stringstream ss(_params);
        string _codigo;
        string _nombre;
        string _telefono;
        getline(ss, _codigo, ',');
        getline(ss, _nombre, ',');
        getline(ss, _telefono, ',');

        set_data(stoi(_codigo),_nombre, atol(_telefono.c_str()));
    }

    void set_data(int _codigo, string _nombre, long _telefono){
        codigo = _codigo;
        strcpy(nombre, _nombre.c_str());
        telefono = _telefono;
    }

    void print_data(){
        cout << codigo<< " | "
             << string(nombre) << " | "
             << telefono << std::endl;
    }

    void load(fstream &file) {
        file.read( (char*) &codigo, sizeof(codigo) );
        file.read( (char*) nombre, sizeof(nombre) );
        file.read( (char*) &telefono, sizeof(telefono) );
    }

    void save(fstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) nombre, sizeof(nombre) );
        file.write( (char*) &telefono, sizeof(telefono) );

    }

    void save(ofstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) nombre, sizeof(nombre) );
        file.write( (char*) &telefono, sizeof(telefono) );
    }

    T get_key() {

        return (codigo);
    }

    string print_csv() {
        return
                to_string(codigo)+","+
                string(nombre)+","+
                to_string(telefono)+"\n";
    }
};