#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

struct Record{
    int id;
    char dato[10];
    int nextpos;
    bool deleted;

    void setData(int id, char* dato1){
        this->id = id;
        strcpy(dato,dato1);
        this->nextpos = -1;
        this->deleted = false;
    }
    void showData(){
        cout << id << endl;
        cout << dato << endl;
        cout << nextpos << endl;
        cout << deleted << endl;
    }
};

struct Bloque{
    vector<Record> registros;
    int M = 10;
};

struct SeqFile{
    string filename;
    string auxname;
    int K = 10;  //Limite maximo de registros en el archivo auxiliar
    SeqFile(string filename){
        this->filename = filename;
        this->auxname = "aux" + filename;
        fstream file(filename, ios::in | ios::binary);
        file.close();
        if(!file.good()){
            fstream file(filename, ios::out | ios::binary);
            file.close();
        }
        fstream auxfile(auxname, ios::in | ios::binary);
        auxfile.close();
        if (!auxfile.good()) {
            fstream auxfile(auxname, ios::out | ios::binary);
            auxfile.close();
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
                else if (r.id > key) u = m - 1;
                else if (!r.deleted) return r; //Verificar que no esté eliminado
            }
        }
        file.close();
      //Búsqueda en archivo auxiliar
      file.open(auxname, ios::in | ios::binary);
      Record res;
      while (file.read((char*)&res, sizeof(Record))) {
          if (res.id == key && !res.deleted) {
              file.close();
              return res;
          }
      }
      file.close();
      //Si no se encuentra el registro, devolver un registro con id -1 para indicar que no se encontró
      Record notFound;
      notFound.setData(-1, (char*)"");
      notFound.deleted = true;
      return notFound;
    }


    void insert(Record reg){
        fstream file(filename, ios::in | ios::binary);
      fstream auxfile(auxname, ios::in | ios::out | ios::binary | ios::app);
        Record r;
        long opos = 0;
        long prev_pos = -1; //Posición del registro anterior
        bool inserted = false;
      //Buscar posición correcta en archivo principal
        while(file.read((char*) &r, sizeof(Record))){
            if(r.id > reg.id) break; // manejo de archivo auxiliar
            prev_pos = opos;
            opos = file.tellg();
        }
      
      if (file){
        //Si hay un registro anterior, actualizar su nextpos
        if (prev_pos != -1) {
            file.seekg(prev_pos, ios::beg);
            file.read((char*)&r, sizeof(Record));
            r.nextpos = opos / sizeof(Record);
            file.seekp(prev_pos, ios::beg);
            file.write((char*)&r, sizeof(Record));
        }

        //Insertar el nuevo registro y actualizar su nextpos
        file.seekp(opos, ios::beg);
        reg.nextpos = opos / sizeof(Record) + 1;
        file.write((char*)&reg, sizeof(Record));

        //Si hay un siguiente registro, actualizar su nextpos
        file.seekg((opos + sizeof(Record)), ios::beg);
        if (file.read((char*)&r, sizeof(Record))) {
            reg.nextpos = opos / sizeof(Record) + 1;
            file.seekp(opos, ios::beg);
            file.write((char*)&reg, sizeof(Record));
        }
        inserted = true;
      }
        if (!inserted) {
            auxfile.write((char*)&reg, sizeof(Record));
        }
        file.close();
        auxfile.close();

      //Chequear si el archivo auxiliar ha excedido el límite de K registros
      if (countAuxRecords() > K) {
          mergeFiles();
      }
    }

void remove(int key) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    Record r, prev;
    long pos = 0, prev_pos = -1;
    bool found = false;

    //Buscar el registro a eliminar en el archivo principal
    while (file.read((char*)&r, sizeof(Record))) {
        if (r.id == key) {
            found = true;
            break;
        }
        prev_pos = pos;
        pos = file.tellg();
    }

    if (found) {
        //Marcar el registro como eliminado
        r.deleted = true;
        file.seekp(pos, ios::beg);
        file.write((char*)&r, sizeof(Record));

        //Actualizar el puntero nextpos del registro anterior
        if (prev_pos != -1) {
            file.seekg(prev_pos, ios::beg);
            file.read((char*)&prev, sizeof(Record));
            prev.nextpos = r.nextpos;
            file.seekp(prev_pos, ios::beg);
            file.write((char*)&prev, sizeof(Record));
        }
    }

    file.close();

    //Buscar y eliminar el registro del archivo auxiliar si no se encontró en el principal
    if (!found) {
        fstream auxfile(auxname, ios::in | ios::out | ios::binary);
        pos = 0, prev_pos = -1;
        while (auxfile.read((char*)&r, sizeof(Record))) {
            if (r.id == key) {
                r.deleted = true;
                auxfile.seekp(pos, ios::beg);
                auxfile.write((char*)&r, sizeof(Record));
                break;
            }
            prev_pos = pos;
            pos = auxfile.tellg();
        }
        auxfile.close();
    }
}



    int countAuxRecords() {
        fstream auxfile(auxname, ios::in | ios::binary);
        auxfile.seekg(0, ios::end);
        int count = auxfile.tellg() / sizeof(Record);
        auxfile.close();
        return count;
    }

    void mergeFiles() {
        fstream file(filename, ios::in | ios::out | ios::binary);
        fstream auxfile(auxname, ios::in | ios::binary);

        vector<Record> mainRecords;
        vector<Record> auxRecords;

        Record r;

        //Leer todos los registros del archivo principal
        while (file.read((char*)&r, sizeof(Record))) {
            mainRecords.push_back(r);
        }

        //Leer todos los registros del archivo auxiliar
        while (auxfile.read((char*)&r, sizeof(Record))) {
            auxRecords.push_back(r);
        }

        //Cerrar los archivos
        file.close();
        auxfile.close();

        //Merge de registros
        vector<Record> mergedRecords;
        size_t i = 0, j = 0;
        while (i < mainRecords.size() && j < auxRecords.size()) {
            if (mainRecords[i].id <= auxRecords[j].id) {
                mergedRecords.push_back(mainRecords[i]);
                i++;
            } else {
                mergedRecords.push_back(auxRecords[j]);
                j++;
            }
        }

        //Agregar los registros restantes
        while (i < mainRecords.size()) {
            mergedRecords.push_back(mainRecords[i]);
            i++;
        }
        while (j < auxRecords.size()) {
            mergedRecords.push_back(auxRecords[j]);
            j++;
        }

        //Actualizar los punteros nextpos
        for (size_t k = 0; k < mergedRecords.size() - 1; ++k) {
            mergedRecords[k].nextpos = k + 1;
        }
        if (!mergedRecords.empty()) {
            mergedRecords.back().nextpos = -1; //El último registro apunta a -1
        }

        //Reescribir archivo principal con registros fusionados
        file.open(filename, ios::out | ios::binary | ios::trunc);
        for (auto& record : mergedRecords) {
            file.write((char*)&record, sizeof(Record));
        }
        file.close();

        //Limpiar el archivo auxiliar
        auxfile.open(auxname, ios::out | ios::binary | ios::trunc);
        auxfile.close();
    }

  vector<Record> rangeSearch(int beginKey, int endKey) {
      vector<Record> results;
      fstream file(filename, ios::in | ios::binary);
      fstream auxfile(auxname, ios::in | ios::binary);
      Record r;

      //Leer registros en el rango del archivo principal
      while (file.read((char*)&r, sizeof(Record))) {
          if (r.id >= beginKey && r.id <= endKey && !r.deleted) {
              results.push_back(r);
          }
      }

      //Leer registros en el rango del archivo auxiliar
      while (auxfile.read((char*)&r, sizeof(Record))) {
          if (r.id >= beginKey && r.id <= endKey && !r.deleted) {
              results.push_back(r);
          }
      }

      file.close();
      auxfile.close();
    
      return results;
  }

};