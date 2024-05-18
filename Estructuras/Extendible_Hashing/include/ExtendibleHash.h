#include "IndexRecord.h"

//template <typename TKey,typename TRecord>
class ExtendibleHash {
private:
    bsize_t bucket_max_size;
    filename_t filename_index;
    filename_t filename_data;
    // Index
    vector<IndexRecord> index_table;
    depth_t max_depth;
    void add_entry_index(IndexRecord irecord);

    void init_index();

    void load_index();

    void save_index();
    // Compare Hash, verificar si los depth bits menos significativos de ambos hashes coinciden
    // de esta manera determinamos si un registro pertenece a un bucket especifico.
    bool validate_hash(sufix_t key_hash, sufix_t sufix, depth_t depth);

    sufix_t generate_hash(T key);

    IndexRecord* match_irecord(sufix_t key_hash);

public:
    ExtendibleHash(filename_t filename, bsize_t bucket_size, depth_t depth);
    ~ExtendibleHash();
    // Write new entry
    bool insert(Record record);

    bool remove(T key);

    vector<Record> search(T key);

    vector<Record> rangeSearch(T begin_key, T end_key);

    vector<Record> load();
    void write_csv(vector<Record> recibido);
};

void ExtendibleHash::add_entry_index(IndexRecord irecord)  {
    index_table.push_back(irecord);
}

void ExtendibleHash::init_index() {
    Bucket empty_bucket{0,-1};
    // Reservamos dos buckets vacios en data_file
    pos_t pos0, pos1;
    ofstream data_file(filename_data, ios::binary | ios::trunc);
    if (data_file.is_open()) {
        pos0=0;
        empty_bucket.save_to_file(data_file, bucket_max_size);
        pos1=data_file.tellp();
        empty_bucket.save_to_file(data_file, bucket_max_size);
        data_file.close();
    }
    else {
        data_file.close();
        cout << "[ERROR]: Cant Init Index File" << endl;
        exit(2);
    }
    // Una vez reservado, ahora creamos los dos buckets con sus prefijos iniciales
    IndexRecord init0{1,0,pos0};
    IndexRecord init1{1,1,pos1};
    add_entry_index(init0);
    add_entry_index(init1);
}

void ExtendibleHash::load_index() {
    ifstream input_index(filename_index, ios::binary);
    // global_depth = 0; // Start depth at 0
    bool file_exists = input_index.is_open();
    if (file_exists){
        IndexRecord irecord;
        irecord.load_from_ifile(input_index); // Guardando index record inicial
        while(input_index) {
            add_entry_index(irecord);
            irecord.load_from_ifile(input_index); // Guardando el siguiente index record
        }
    }
    input_index.close();
    // Sino existe index_file, inicializarlo
    if (!file_exists) {
        init_index();
    }
}

void ExtendibleHash::save_index() {
    ofstream out_index(filename_index, ios::binary | ios::trunc);
    if (out_index.is_open()) {
        for (auto &irecord : index_table) {
            irecord.save_to_ofile(out_index);
        }
    }
    out_index.close();
}

bool ExtendibleHash::validate_hash(sufix_t key_hash, sufix_t sufix, depth_t depth) {
    for (int i = 0; i<depth; i++) {
        // Extrayendo el bit menos significativo para keyHash y suffix bit
        sufix_t key_hash_bit = (key_hash%2 + 2) %2; key_hash = key_hash/2;
        sufix_t sufix_bit = sufix%2; sufix = sufix/2;
        if (key_hash_bit != sufix_bit) {
            return false;
        }
    }
    return true;
    /* Ejemplo _key_hash = 13(1101), _suffix = 5(0101) con depth = 3
        *
        * iter 1:  key_hash_bit = 1    (110'1')
        *          suffix_bit = 1      (010'1')
        * iter 2:  key_hash_bit = 0    (011'0')
        *          suffix_bit = 0      (001'0')
        * iter 3:  key_hash_bit = 1    (001'1')
        *          suffix_bit = 1      (000'1')
        */
}


sufix_t ExtendibleHash::generate_hash(T key) {
    return std::hash<T>()(key);
}

IndexRecord* ExtendibleHash::match_irecord(sufix_t key_hash){
    for (auto &irecord : index_table) {
        if (validate_hash(key_hash, irecord.sufix, irecord.depth)) {
            // Si hacen match
            return &irecord;
        }
    }
    // Si no hay match
    cout << "Error in matching record" <<endl;
    exit(2);
}

ExtendibleHash::ExtendibleHash(filename_t filename, bsize_t bucket_size, depth_t depth) {
    filename_data = filename + ".data";
    filename_index = filename + ".index";
    bucket_max_size = bucket_size;
    max_depth = depth;
    load_index();
}

ExtendibleHash::~ExtendibleHash(){
    save_index();
}

bool ExtendibleHash::insert(Record record) {
    auto key_hash = generate_hash(record.get_key());
    auto irecord = match_irecord(key_hash);
    auto bucket_pos = irecord->bucket_pos;
    Bucket bucket;
    fstream data_file(filename_data, ios::binary | ios::out | ios::in);
    if (data_file.is_open()){
        bucket.load_from_file(data_file, bucket_pos, bucket_max_size);
        // bucketisFull?
        if (bucket.bsize == bucket_max_size){
            // Bucket FULL:
            if (irecord->depth < max_depth) {
                // Bucket FULL AND depth < max_depth:SPLIT (local depth < global depth)
                // 0+suffix (old) && 1+suffix (new)
                irecord->depth++;
                // Copiar los nuevos vectores
                vector<Record> zro_sufix;
                vector<Record> one_sufix;
                for (auto &rec : bucket.records) {
                    auto rec_hash = generate_hash(rec.get_key());
                    if ( validate_hash(rec_hash, irecord->sufix, irecord->depth ) ) {
                        zro_sufix.push_back(rec);
                    }
                    else {
                        one_sufix.push_back(rec);
                    }
                }
                // Split index_record
                // Sobreescribiendo sobre el antiguo bucket
                bucket.records = zro_sufix;
                bucket.bsize = zro_sufix.size();
                // Creando y escribiendo un nuevo index_record(que es un bucket pair) para 1+sufix
                Bucket one_bucket;
                one_bucket.records = one_sufix;
                one_bucket.bsize = one_sufix.size();
                one_bucket.next_bucket = -1; // creo que podemos juntarlo To_do en uno con una funcion copy
                // WRITE to disk
                data_file.seekp(0, ios::end); // Go to eof
                pos_t pos_new = data_file.tellp(); // Save pos
                one_bucket.save_to_file(data_file, pos_new, bucket_max_size);
                bucket.save_to_file(data_file, bucket_pos, bucket_max_size); // Sobreescribe un juego bucket en el archivo
                // New index_record
                sufix_t sufix_new = irecord->sufix + (1 << (irecord->depth-1));
                IndexRecord irecord_new{irecord->depth, sufix_new, pos_new};
                add_entry_index(irecord_new);
                // añadimos recursivamente
                data_file.close();
                insert(record);
            }
            else {
                // Bucket FULL AND depth >= max_depth:OVERFLOW (local depth >= global depth)
                auto prev_bucket_pos = bucket_pos;
                bucket_pos = bucket.next_bucket;
                // Se inserta en un overflow page ya existente (si es que hay)
                while (bucket_pos != -1) {
                    bucket.load_from_file(data_file, bucket_pos, bucket_max_size);
                    if (bucket.bsize != bucket_max_size) { break; }
                    prev_bucket_pos = bucket_pos;
                    bucket_pos = bucket.next_bucket;
                }
                // // Si no existe un overflow page, lo creamos
                if (bucket_pos == -1) {
                    // hacer apuntar el bucket previo al final del archivo, donde la nueva overflow page sera creada
                    data_file.seekp(0, ios::end);
                    bucket_pos = data_file.tellp();
                    bucket.next_bucket = bucket_pos;
                    bucket.save_to_file(data_file, prev_bucket_pos, bucket_max_size);
                    // Incializando el bucket vacio.
                    Bucket empty_bucket{0,-1};
                    bucket = empty_bucket;
                } // insertamos como normalmente se hace
                bucket.records.push_back(record);
                bucket.bsize++;
            }
        }
        else {
            // Bucket not full
            bucket.records.push_back(record);
            bucket.bsize++;
        }
        // Sobreescribe en el bucket
        bucket.save_to_file(data_file, bucket_pos, bucket_max_size);
        data_file.close();
    }
    else {
        // si falla al abir
        data_file.close();
        return false;
    }
    return true;
}

bool ExtendibleHash::remove(T key) {
    auto key_hash = generate_hash(key);
    auto irecord = match_irecord(key_hash);
    auto bucket_pos = irecord->bucket_pos;
    Bucket bucket;
    // Buscar bucket hasta en las overflow pages, removeremos todas las apariciones
    fstream data_file(filename_data, ios::binary | ios::in | ios::out);
    if (data_file.is_open()){
        while (bucket_pos != -1) {
            bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
            // Removemos repeticiones
            vector<Record> res;
            for (auto &record :  bucket.records) {
                if (record.get_key() != key) {
                    res.push_back(record);
                }
            }
            bucket.records = res;
            bucket.bsize = res.size();
            // sobreescribimos el bucket
            bucket.save_to_file(data_file, bucket_pos, bucket_max_size);
            // Point to next bucket pos
            bucket_pos = bucket.next_bucket;
        }
        data_file.close();
    }
    else {
        // fail to open
        data_file.close();
        return false;
    }
    return true;
}

vector<Record> ExtendibleHash::search(T key) {
    vector<Record> result_records;
    auto key_hash = generate_hash(key);
    // Buscamos el bucket en index_record
    auto irecord = match_irecord(key_hash);
    auto bucket_pos = irecord->bucket_pos;
    Bucket bucket;
    // Buscamos buckets hasta en los overflow pages
    fstream data_file(filename_data, ios::binary | ios::in);
    if (data_file.is_open()){
        while (bucket_pos != -1) {
            bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
            // Search in bucket
            for (auto &record :  bucket.records) {
                if (record.get_key() == key) {
                    result_records.push_back(record);
                }
            }
            bucket_pos = bucket.next_bucket; // next bucket position
        }
    }
    data_file.close();
    return result_records;
}

vector<Record> ExtendibleHash::rangeSearch(T begin_key, T end_key) {
    vector<Record> result_records;
    fstream data_file(filename_data, ios::binary | ios::in);
    if (data_file.is_open()){
        // Iteramos en todos los buckets
        for (auto &irecord : index_table ){
            auto bucket_pos = irecord.bucket_pos;
            Bucket bucket;
            // buscamos en overflow pages
            while (bucket_pos != -1) {
                bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
                // Search in bucket
                for (auto &record :  bucket.records) {
                    if (record.get_key() >= begin_key && record.get_key() <= end_key) {
                        result_records.push_back(record);
                    }
                }
                bucket_pos = bucket.next_bucket; // next bucket position
            }
        }
    }
    data_file.close();
    return result_records;
}

vector<Record> ExtendibleHash::load() {
    vector<Record> result_records;
    fstream data_file(filename_data, ios::binary | ios::in);
    if (data_file.is_open()){
        // Iteramos en todos los buckets
        for (auto &irecord : index_table ){
            auto bucket_pos = irecord.bucket_pos;
            Bucket bucket;
            // buscamos en overflow pages
            while (bucket_pos != -1) {
                bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
                // Search in bucket
                for (auto &record :  bucket.records) {
                    result_records.push_back(record);
                }
                bucket_pos = bucket.next_bucket; // next bucket position
            }
        }
    }
    data_file.close();
    return result_records;
}

void ExtendibleHash::write_csv(vector<Record> recibido) {
    std::ofstream myfile;
    myfile.open ("ehash.csv");
    for (auto val : recibido){
        myfile<<val.print_csv();
    }
    myfile.close();
}