#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

struct HomesRecord {
    char property_url[130]{'\0'};
    char property_id[10]{'\0'};
    char address[114]{'\0'};
    char street_name[105]{'\0'};
    char apartment[31]{'\0'}; 
    char city[26]{'\0'};
    char state[3]{'\0'};
    float latitude{}; 
    float longitude{}; 
    char postcode[7]{'\0'};
    float price{};
    float bedroom_number{}; 
    float bathroom_number{}; 
    float price_per_unit{}; 
    float living_space{}; 
    float land_space{};
    char land_space_unit[5]{'\0'};
    char broker_id[8]{'\0'}; 
    char property_type[13]{'\0'};
    char property_status[8]{'\0'};
    int year_build{}; 
    int total_num_units{}; 
    float listing_age{}; 
    char RunDate[19]{'\0'};
    char agency_name[81]{'\0'};
    char agent_name[81]{'\0'};
    char agent_phone[16]{'\0'}; 
    bool is_owned_by_zillow{}; 
    int nextpos{-1};
    bool deleted{false};

    void setData(const char* url, const char* id, const char* addr, const char* street, const char* apt, const char* cty, const char* st, float lat, float lon, const char* post, float prc, float bdrm, float bath, float prc_unit, float living, float lspace, const char* lunit, const char* broker, const char* ptype, const char* pstatus, int year, int total_units, float age, const char* rdate, const char* agency, const char* a_name, const char* a_phone, bool is_zillow) {
        strcpy(property_url, url);
        strcpy(property_id, id);
        strcpy(address, addr);
        strcpy(street_name, street);
        strcpy(apartment, apt);
        strcpy(city, cty);
        strcpy(state, st);
        latitude = lat;
        longitude = lon;
        strcpy(postcode, post);
        price = prc;
        bedroom_number = bdrm;
        bathroom_number = bath;
        price_per_unit = prc_unit;
        living_space = living;
        land_space = lspace;
        strcpy(land_space_unit, lunit);
        strcpy(broker_id, broker);
        strcpy(property_type, ptype);
        strcpy(property_status, pstatus);
        year_build = year;
        total_num_units = total_units;
        listing_age = age;
        strcpy(RunDate, rdate);
        strcpy(agency_name, agency);
        strcpy(agent_name, a_name);
        strcpy(agent_phone, a_phone);
        is_owned_by_zillow = is_zillow;
        nextpos = -1;
        deleted = false;
    }

    void showData() {
        cout << "Property URL: " << property_url << endl;
        cout << "Property ID: " << property_id << endl;
        cout << "Address: " << address << endl;
        cout << "Street Name: " << street_name << endl;
        cout << "Apartment: " << apartment << endl;
        cout << "City: " << city << endl;
        cout << "State: " << state << endl;
        cout << "Latitude: " << latitude << endl;
        cout << "Longitude: " << longitude << endl;
        cout << "Postcode: " << postcode << endl;
        cout << "Price: " << price << endl;
        cout << "Bedroom Number: " << bedroom_number << endl;
        cout << "Bathroom Number: " << bathroom_number << endl;
        cout << "Price Per Unit: " << price_per_unit << endl;
        cout << "Living Space: " << living_space << endl;
        cout << "Land Space: " << land_space << endl;
        cout << "Land Space Unit: " << land_space_unit << endl;
        cout << "Broker ID: " << broker_id << endl;
        cout << "Property Type: " << property_type << endl;
        cout << "Property Status: " << property_status << endl;
        cout << "Year Build: " << year_build << endl;
        cout << "Total Number of Units: " << total_num_units << endl;
        cout << "Listing Age: " << listing_age << endl;
        cout << "Run Date: " << RunDate << endl;
        cout << "Agency Name: " << agency_name << endl;
        cout << "Agent Name: " << agent_name << endl;
        cout << "Agent Phone: " << agent_phone << endl;
        cout << "Owned by Zillow: " << is_owned_by_zillow << endl;
        cout << "Next Position: " << nextpos << endl;
        cout << "Deleted: " << deleted << endl;
    }
};

struct SeqFile {
string filename;
string auxname;
int K = 30;  // Limite maximo de registros en el archivo auxiliar
const int MAX_PRIMARY_RECORDS = 100; // Límite de registros en el archivo principal

SeqFile(string filename) {
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

int countAuxRecords() {
    fstream auxfile(auxname, ios::in | ios::binary);
    auxfile.seekg(0, ios::end);
    int count = auxfile.tellg() / sizeof(HomesRecord);
    auxfile.close();
    return count;
}

void mergeFiles() {
    fstream file(filename, ios::in | ios::out | ios::binary);
    fstream auxfile(auxname, ios::in | ios::binary);

    vector<HomesRecord> mainRecords;
    vector<HomesRecord> auxRecords;

    HomesRecord r;

    // Leer todos los registros del archivo principal
    while (file.read((char*)&r, sizeof(HomesRecord))) {
        mainRecords.push_back(r);
    }

    // Leer todos los registros del archivo auxiliar
    while (auxfile.read((char*)&r, sizeof(HomesRecord))) {
        auxRecords.push_back(r);
    }

    // Cerrar los archivos
    file.close();
    auxfile.close();

    // Merge de registros
    vector<HomesRecord> mergedRecords;
    size_t i = 0, j = 0;
    while (i < mainRecords.size() && j < auxRecords.size()) {
        if (strcmp(mainRecords[i].property_id, auxRecords[j].property_id) <= 0) {
            mergedRecords.push_back(mainRecords[i]);
            i++;
        } else {
            mergedRecords.push_back(auxRecords[j]);
            j++;
        }
    }

    // Agregar los registros restantes
    while (i < mainRecords.size()) {
        mergedRecords.push_back(mainRecords[i]);
        i++;
    }
    while (j < auxRecords.size()) {
        mergedRecords.push_back(auxRecords[j]);
        j++;
    }

    // Actualizar los punteros nextpos
    for (size_t k = 0; k < mergedRecords.size() - 1; ++k) {
        mergedRecords[k].nextpos = k + 1;
    }
    if (!mergedRecords.empty()) {
        mergedRecords.back().nextpos = -1; // El último registro apunta a -1
    }

    // Reescribir archivo principal con registros fusionados
    file.open(filename, ios::out | ios::binary | ios::trunc);
    for (auto& record : mergedRecords) {
        file.write((char*)&record, sizeof(HomesRecord));
    }
    file.close();

    // Limpiar el archivo auxiliar
    auxfile.open(auxname, ios::out | ios::binary | ios::trunc);
    auxfile.close();
}



HomesRecord find(const char* key) {
    fstream file(filename, ios::in | ios::binary);
    if(file.good()){
        int l = 0;
        file.seekg(0,ios::end);
        int u = (file.tellg() / sizeof(HomesRecord)) - 1;
        while(u >= l){
            int m = (l + u) / 2;
            HomesRecord r;
            file.seekg(m * sizeof(HomesRecord), ios::beg);
            file.read((char*) &r, sizeof(HomesRecord));
            if(strcmp(r.property_id, key) < 0) l = m + 1;
            else if (strcmp(r.property_id, key) > 0) u = m - 1;
            else if (!r.deleted) return r; // Verificar que no esté eliminado
        }
    }
    file.close();
    // Búsqueda en archivo auxiliar
    file.open(auxname, ios::in | ios::binary);
    HomesRecord res;
    while (file.read((char*)&res, sizeof(HomesRecord))) {
        if (strcmp(res.property_id, key) == 0 && !res.deleted) {
            file.close();
            return res;
        }
    }
    file.close();
    // Si no se encuentra el registro, devolver un registro con property_id vacío para indicar que no se encontró
    HomesRecord notFound;
    //notFound.setData("", "", "", "", "", "", "", 0, 0, "", "", "", "", "");
    notFound.deleted = true;
    return notFound;
}

void insert(HomesRecord reg) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    fstream auxfile(auxname, ios::in | ios::out | ios::binary | ios::app);
    HomesRecord r;
    long opos = 0;
    long prev_pos = -1; // Posición del registro anterior
    bool inserted = false;

    // Verificar la cantidad de registros en el archivo principal
    file.seekg(0, ios::end);
    int recordCount = file.tellg() / sizeof(HomesRecord);
    file.clear(); // Limpiar los flags de estado
    file.seekg(0, ios::beg); // Reiniciar la posición de lectura al inicio

    if (recordCount < MAX_PRIMARY_RECORDS) {
        // Buscar posición correcta en archivo principal
        while (file.read((char*)&r, sizeof(HomesRecord))) {
            if (strcmp(r.property_id, reg.property_id) > 0) break;
            prev_pos = opos;
            opos = file.tellg();
        }

        // Insertar el nuevo registro en la posición encontrada
        file.clear(); // Limpiar los flags de estado
        if (prev_pos != -1) {
            // Si hay un registro anterior, actualizar su nextpos
            file.seekg(prev_pos, ios::beg);
            file.read((char*)&r, sizeof(HomesRecord));
            r.nextpos = opos / sizeof(HomesRecord);
            file.seekp(prev_pos, ios::beg);
            file.write((char*)&r, sizeof(HomesRecord));
        }

        // Insertar el nuevo registro y actualizar su nextpos
        file.seekp(opos, ios::beg);
        reg.nextpos = (opos / sizeof(HomesRecord)) + 1;
        file.write((char*)&reg, sizeof(HomesRecord));

        // Si hay un siguiente registro, actualizar su nextpos
        file.seekg((opos + sizeof(HomesRecord)), ios::beg);
        if (file.read((char*)&r, sizeof(HomesRecord))) {
            reg.nextpos = opos / sizeof(HomesRecord) + 1;
            file.seekp(opos, ios::beg);
            file.write((char*)&reg, sizeof(HomesRecord));
        }
        inserted = true;
    }

    if (!inserted) {
        auxfile.write((char*)&reg, sizeof(HomesRecord));
        if (recordCount == MAX_PRIMARY_RECORDS) {
            file.seekg((recordCount - 1) * sizeof(HomesRecord), ios::beg);
            file.read((char*)&r, sizeof(HomesRecord));
            r.nextpos = MAX_PRIMARY_RECORDS; // Apuntar al primer registro del archivo auxiliar
            file.seekp((recordCount - 1) * sizeof(HomesRecord), ios::beg);
            file.write((char*)&r, sizeof(HomesRecord));
        }
    }

    file.close();
    auxfile.close();

    // Chequear si el archivo auxiliar ha excedido el límite de K registros
    if (countAuxRecords() > K) {
        mergeFiles();
    }
}

void remove(const char* key) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    HomesRecord r, prev;
    long pos = 0, prev_pos = -1;
    bool found = false;

    // Buscar el registro a eliminar en el archivo principal
    while (file.read((char*)&r, sizeof(HomesRecord))) {
        if (strcmp(r.property_id, key) == 0) {
            found = true;
            break;
        }
        prev_pos = pos;
        pos = file.tellg();
    }

    if (found) {
        // Marcar el registro como eliminado
        r.deleted = true;
        file.seekp(pos, ios::beg);
        file.write((char*)&r, sizeof(HomesRecord));

        // Actualizar el puntero nextpos del registro anterior
        if (prev_pos != -1) {
            file.seekg(prev_pos, ios::beg);
            file.read((char*)&prev, sizeof(HomesRecord));
            prev.nextpos = r.nextpos;
            file.seekp(prev_pos, ios::beg);
            file.write((char*)&prev, sizeof(HomesRecord));
        }
    }

    file.close();

    // Buscar y eliminar el registro del archivo auxiliar si no se encontró en el principal
    if (!found) {
        fstream auxfile(auxname, ios::in | ios::out | ios::binary);
        pos = 0, prev_pos = -1;
        while (auxfile.read((char*)&r, sizeof(HomesRecord))) {
            if (strcmp(r.property_id, key) == 0) {
                r.deleted = true;
                auxfile.seekp(pos, ios::beg);
                auxfile.write((char*)&r, sizeof(HomesRecord));
                break;
            }
            prev_pos = pos;
            pos = auxfile.tellg();
        }
        auxfile.close();
    }
}

vector<HomesRecord> rangeSearch(const char* beginKey, const char* endKey) {
    vector<HomesRecord> results;
    fstream file(filename, ios::in | ios::binary);
    fstream auxfile(auxname, ios::in | ios::binary);
    HomesRecord r;

    // Leer registros en el rango del archivo principal
    while (file.read((char*)&r, sizeof(HomesRecord))) {
        if (strcmp(r.property_id, beginKey) >= 0 && strcmp(r.property_id, endKey) <= 0 && !r.deleted) {
            results.push_back(r);
        }
    }

    // Leer registros en el rango del archivo auxiliar
    while (auxfile.read((char*)&r, sizeof(HomesRecord))) {
        if (strcmp(r.property_id, beginKey) >= 0 && strcmp(r.property_id, endKey) <= 0 && !r.deleted) {
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
        if (fields.size() == 28) {
            HomesRecord newRecord;

            // Establecer los datos en el nuevo registro
            newRecord.setData(fields[0].c_str(), fields[1].c_str(), fields[2].c_str(), fields[3].c_str(), fields[4].c_str(),
                               fields[5].c_str(), fields[6].c_str(), stof(fields[7]), stof(fields[8]), fields[9].c_str(),
                               stof(fields[10]), stof(fields[11]), stof(fields[12]), stof(fields[13]), stof(fields[14]),
                               stof(fields[15]), fields[16].c_str(), fields[17].c_str(), fields[18].c_str(),
                               fields[19].c_str(), stoi(fields[20]), stof(fields[21]), stof(fields[22]), fields[23].c_str(),
                               fields[24].c_str(), fields[25].c_str(), fields[26].c_str(), stoi(fields[27]));

            // Insertar el nuevo registro en el archivo principal
            insert(newRecord);
        } else {
            cout << "Error: La línea no tiene el formato esperado." << endl;
        }
    }

    file.close();
}


};
