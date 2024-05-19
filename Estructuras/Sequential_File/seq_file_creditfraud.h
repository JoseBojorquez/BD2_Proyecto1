#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
using namespace std;

struct Record{
    int id;
    double V1;
    double V2;
    double V3;
    double V4;
    double V5;
    double V6;
    double V7;
    double V8;
    double V9;
    double V10;
    double V11;
    double V12;
    double V13;
    double V14;
    double V15;
    double V16;
    double V17;
    double V18;
    double V19;
    double V20;
    double V21;
    double V22;
    double V23;
    double V24;
    double V25;
    double V26;
    double V27;
    double V28;
    float Amount;
    bool Class;
    int nextpos{-1};
    bool deleted{false};

    void setData(int id, double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9, double V10, double V11, double V12, double V13, double V14, double V15, double V16, double V17, double V18, double V19, double V20, double V21, double V22, double V23, double V24, double V25, double V26, double V27, double V28, float Amount, bool Class) {
    this->id = id;
    this->V1 = V1;
    this->V2 = V2;
    this->V3 = V3;
    this->V4 = V4;
    this->V5 = V5;
    this->V6 = V6;
    this->V7 = V7;
    this->V8 = V8;
    this->V9 = V9;
    this->V10 = V10;
    this->V11 = V11;
    this->V12 = V12;
    this->V13 = V13;
    this->V14 = V14;
    this->V15 = V15;
    this->V16 = V16;
    this->V17 = V17;
    this->V18 = V18;
    this->V19 = V19;
    this->V20 = V20;
    this->V21 = V21;
    this->V22 = V22;
    this->V23 = V23;
    this->V24 = V24;
    this->V25 = V25;
    this->V26 = V26;
    this->V27 = V27;
    this->V28 = V28;
    this->Amount = Amount;
    this->Class = Class;
    this->nextpos = -1;
    this->deleted = false;
}

void showData() {
    cout << "ID: " << id << endl;
    cout << "V1: " << V1 << endl;
    cout << "V2: " << V2 << endl;
    cout << "V3: " << V3 << endl;
    cout << "V4: " << V4 << endl;
    cout << "V5: " << V5 << endl;
    cout << "V6: " << V6 << endl;
    cout << "V7: " << V7 << endl;
    cout << "V8: " << V8 << endl;
    cout << "V9: " << V9 << endl;
    cout << "V10: " << V10 << endl;
    cout << "V11: " << V11 << endl;
    cout << "V12: " << V12 << endl;
    cout << "V13: " << V13 << endl;
    cout << "V14: " << V14 << endl;
    cout << "V15: " << V15 << endl;
    cout << "V16: " << V16 << endl;
    cout << "V17: " << V17 << endl;
    cout << "V18: " << V18 << endl;
    cout << "V19: " << V19 << endl;
    cout << "V20: " << V20 << endl;
    cout << "V21: " << V21 << endl;
    cout << "V22: " << V22 << endl;
    cout << "V23: " << V23 << endl;
    cout << "V24: " << V24 << endl;
    cout << "V25: " << V25 << endl;
    cout << "V26: " << V26 << endl;
    cout << "V27: " << V27 << endl;
    cout << "V28: " << V28 << endl;
    cout << "Amount: " << Amount << endl;
    cout << "Class: " << Class << endl;
    cout << "Nextpos: " << nextpos << endl;
    cout << "Deleted: " << deleted << endl;
}

};

struct Bloque{
    vector<Record> registros;
    int M = 10;
};

struct SeqFile{
    string filename;
    string auxname;
    int K = 30;  //Limite maximo de registros en el archivo auxiliar
    const int MAX_PRIMARY_RECORDS = 100; //Límite de registros en el archivo principal
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
      notFound.deleted = true;
      return notFound;
    }

void insert(Record reg) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    fstream auxfile(auxname, ios::in | ios::out | ios::binary | ios::app);
    Record r;
    long opos = 0;
    long prev_pos = -1; // Posición del registro anterior
    bool inserted = false;

    // Verificar la cantidad de registros en el archivo principal
    file.seekg(0, ios::end);
    int recordCount = file.tellg() / sizeof(Record);
    file.clear(); // Limpiar los flags de estado
    file.seekg(0, ios::beg); // Reiniciar la posición de lectura al inicio

    if (recordCount < MAX_PRIMARY_RECORDS) {
        // Buscar posición correcta en archivo principal
        while (file.read((char*)&r, sizeof(Record))) {
            if (r.id > reg.id) break; // Manejo de archivo auxiliar
            prev_pos = opos;
            opos = file.tellg();
        }

        // Insertar el nuevo registro en la posición encontrada
        file.clear(); // Limpiar los flags de estado
        if (prev_pos != -1) {
            // Si hay un registro anterior, actualizar su nextpos
            file.seekg(prev_pos, ios::beg);
            file.read((char*)&r, sizeof(Record));
            r.nextpos = opos / sizeof(Record);
            file.seekp(prev_pos, ios::beg);
            file.write((char*)&r, sizeof(Record));
        }

        // Insertar el nuevo registro y actualizar su nextpos
        file.seekp(opos, ios::beg);
        reg.nextpos = (opos / sizeof(Record)) + 1;
        file.write((char*)&reg, sizeof(Record));

        // Si hay un siguiente registro, actualizar su nextpos
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
        if (recordCount == MAX_PRIMARY_RECORDS) {
            file.seekg((recordCount - 1) * sizeof(Record), ios::beg);
            file.read((char*)&r, sizeof(Record));
            r.nextpos = MAX_PRIMARY_RECORDS; // Apuntar al primer registro del archivo auxiliar
            file.seekp((recordCount - 1) * sizeof(Record), ios::beg);
            file.write((char*)&r, sizeof(Record));
        }
    }

    file.close();
    auxfile.close();

    // Chequear si el archivo auxiliar ha excedido el límite de K registros
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

void loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo CSV." << endl;
        return;
    }
    // Leer y descartar el encabezado
    string header;
    if (getline(file, header)) {
    // Encabezado leído y descartado
    } else {
        cout << "Error: El archivo CSV está vacío." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        // Leer los campos de la línea separados por comas
        vector<string> fields;
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

        // Crear un nuevo registro y establecer sus datos desde los campos
        if (fields.size() == 31) {  //Asumiendo que hay 31 campos en cada línea
            Record newRecord;
            int id = stoi(fields[0]);  //Convertir el primer campo a entero

            // Convertir los campos de cadena a double o float
            double V1 = stod(fields[1]);
            double V2 = stod(fields[2]);
            double V3 = stod(fields[3]);
            double V4 = stod(fields[4]);
            double V5 = stod(fields[5]);
            double V6 = stod(fields[6]);
            double V7 = stod(fields[7]);
            double V8 = stod(fields[8]);
            double V9 = stod(fields[9]);
            double V10 = stod(fields[10]);
            double V11 = stod(fields[11]);
            double V12 = stod(fields[12]);
            double V13 = stod(fields[13]);
            double V14 = stod(fields[14]);
            double V15 = stod(fields[15]);
            double V16 = stod(fields[16]);
            double V17 = stod(fields[17]);
            double V18 = stod(fields[18]);
            double V19 = stod(fields[19]);
            double V20 = stod(fields[20]);
            double V21 = stod(fields[21]);
            double V22 = stod(fields[22]);
            double V23 = stod(fields[23]);
            double V24 = stod(fields[24]);
            double V25 = stod(fields[25]);
            double V26 = stod(fields[26]);
            double V27 = stod(fields[27]);
            double V28 = stod(fields[28]);

            float Amount = stof(fields[29]);  
            bool Class = stoi(fields[30]);    

            // Establecer los datos en el nuevo registro
            newRecord.setData(id, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, Amount, Class);

            // Insertar el nuevo registro en el archivo principal
            insert(newRecord);
        } else {
            cout << "Error: La línea no tiene el formato esperado." << endl;
        }
    }

    file.close();
}

};
