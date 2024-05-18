#include "../utils/libs.h"

struct Record{

    int codigo;
    char nombre[30];
    long longitud;


    void set_data_from_string(string _params) {
        stringstream ss(_params);
        string _codigo;
        string _nombre;
        string _longitud;
        getline(ss, _codigo, ',');
        getline(ss, _nombre, ',');
        getline(ss, _longitud, ',');

        set_data(stoi(_codigo),_nombre, atol(_longitud.c_str()));
    }

    void set_data(int _codigo, string _nombre, long _longitud){
        codigo = _codigo;
        strcpy(nombre, _nombre.c_str());
        longitud = _longitud;
    }

    void print_data(){
        cout << codigo<< " | "
             << string(nombre) << " | "
             << longitud;
    }

    void load_from_file(fstream &file) {
        file.read( (char*) &codigo, sizeof(codigo) );
        file.read( (char*) nombre, sizeof(nombre) );
        file.read( (char*) &longitud, sizeof(longitud) );
    }

    void save_to_file(fstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) nombre, sizeof(nombre) );
        file.write( (char*) &longitud, sizeof(longitud) );

    }

    void save_to_file(ofstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) nombre, sizeof(nombre) );
        file.write( (char*) &longitud, sizeof(longitud) );
    }

    T get_key() {

        return (codigo);
    }

    string print_csv() {
        return
                to_string(codigo)+","+
                string(nombre)+","+
                to_string(longitud)+"\n";
    }
};