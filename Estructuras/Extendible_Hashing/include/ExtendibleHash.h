#include "IndexRecord.h"

template <typename KeyType,typename RecordType>
class ExtendibleHash {
private:
    bucketSize_t bucket_max_size;
    filename_t index_filename;
    filename_t data_filename;
    // Index
    vector<IndexRecord> index_table;
    depth_t max_depth;
    void add_entry_index(IndexRecord index_record);

    void init_index();

    void load_index();

    void save_index();
    // Compare Hash, verificar si los depth bits menos significativos de ambos hashes coinciden
    // de esta manera determinamos si un registro pertenece a un bucket especifico.
    bool compare_hash_bit_to_bit(suffix_t key_hash, suffix_t sufix, depth_t depth);

    suffix_t generate_hash(T key); // T

    IndexRecord* match_index_record(suffix_t key_hash);

public:
    ExtendibleHash(filename_t filename, bucketSize_t bucket_size, depth_t depth);
    ~ExtendibleHash();
    // Write new entry
    bool insert(RecordType record);

    bool remove(T key); // T

    vector<RecordType> search(T key); // T

    vector<RecordType> rangeSearch(T begin_key, T end_key); // T

    vector<RecordType> load();

    void write(vector<RecordType> input_records); //write in csv RECORD
};

template <typename KeyType,typename RecordType>
void ExtendibleHash<KeyType,RecordType>::add_entry_index(IndexRecord index_record)  {
    index_table.push_back(index_record);
}

template <typename KeyType,typename RecordType>
void ExtendibleHash<KeyType,RecordType>::init_index() {
    Bucket<RecordType> empty_bucket{0,-1};
    // Reservamos dos buckets vacios en data_file
    position_t position0, position1;
    ofstream data_file(data_filename, ios::binary | ios::trunc);
    if (data_file.is_open()) {
        position0=0;
        empty_bucket.save(data_file, bucket_max_size);
        position1=data_file.tellp();
        empty_bucket.save(data_file, bucket_max_size);
        data_file.close();
    }
    else {
        data_file.close();
        cerr << "ERROR in Init Index File" << endl;
        exit(2);
    }
    // Una vez reservado, ahora creamos los dos buckets con sus prefijos iniciales
    IndexRecord init0{1,0,position0};
    IndexRecord init1{1,1,position1};
    add_entry_index(init0);
    add_entry_index(init1);
}

template <typename KeyType,typename RecordType>
void ExtendibleHash<KeyType,RecordType>::load_index() {
    ifstream input_index(index_filename, ios::binary);
    // global_depth = 0; // Start depth at 0
    bool file_exists = input_index.is_open();
    if (file_exists){
        IndexRecord index_record;
        index_record.load(input_index); // Guardando index record inicial
        while(input_index) {
            add_entry_index(index_record);
            index_record.load(input_index); // Guardando el siguiente index record
        }
    }
    input_index.close();
    // Sino existe index_file, inicializarlo
    if (!file_exists) {
        init_index();
    }
}

template <typename KeyType,typename RecordType>
void ExtendibleHash<KeyType,RecordType>::save_index() {
    ofstream output_index(index_filename, ios::binary | ios::trunc);
    if (output_index.is_open()) {
        for (auto &index_record : index_table) {
            index_record.save(output_index);
        }
    }
    output_index.close();
}

template <typename KeyType,typename RecordType>
bool ExtendibleHash<KeyType,RecordType>::compare_hash_bit_to_bit(suffix_t key_hash, suffix_t suffix, depth_t depth) {
    for (int i = 0; i<depth; i++) {
        // Extrayendo el bit menos significativo para keyHash y suffix hasta cierta profundidad
        suffix_t key_hash_bit = (key_hash%2 + 2) %2;
        key_hash = key_hash/2;

        suffix_t suffix_bit = suffix%2;
        suffix = suffix/2;

        if (key_hash_bit != suffix_bit) {
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

template <typename KeyType,typename RecordType>
suffix_t ExtendibleHash<KeyType,RecordType>::generate_hash(T key) { // T
    return std::hash<KeyType>()(key); // T
}

template <typename KeyType,typename RecordType>
IndexRecord* ExtendibleHash<KeyType,RecordType>::match_index_record(suffix_t key_hash){
    for (auto &index_record : index_table) {
        if (compare_hash_bit_to_bit(key_hash, index_record.sufix, index_record.depth)) {
            // Si hacen match
            return &index_record;
        }
    }
    // Si no hay match
    cerr << "Error in matching record" <<endl;
    exit(2);
}

template <typename KeyType,typename RecordType>
ExtendibleHash<KeyType,RecordType>::ExtendibleHash(filename_t filename, bucketSize_t bucket_size, depth_t depth) {
    data_filename = filename + ".data";
    index_filename = filename + ".index";
    bucket_max_size = bucket_size;
    max_depth = depth;
    load_index();
}

template <typename KeyType,typename RecordType>
ExtendibleHash<KeyType,RecordType>::~ExtendibleHash(){
    save_index();
}

template <typename KeyType,typename RecordType>
bool ExtendibleHash<KeyType,RecordType>::insert(RecordType record) { //?
    auto key_hash = generate_hash(record.get_key());
    auto index_record = match_index_record(key_hash);
    auto bucket_position = index_record->bucket_position;
    Bucket<RecordType> bucket;
    fstream data_file(data_filename, ios::binary | ios::out | ios::in);
    if (data_file.is_open()){
        bucket.load(data_file, bucket_position, bucket_max_size);
        // bucketisFull?
        if (bucket.bsize == bucket_max_size){
            // Bucket FULL:
            if (index_record->depth < max_depth) {
                // Bucket FULL AND depth < max_depth:SPLIT (local depth < global depth)
                // 0+suffix (old) && 1+suffix (new)
                index_record->depth++;
                // Copiar los nuevos vectores
                vector<RecordType> zero_suffix;
                vector<RecordType> one_suffix;
                for (auto &rec : bucket.records) {
                    auto rec_hash = generate_hash(rec.get_key());
                    if ( compare_hash_bit_to_bit(rec_hash, index_record->sufix, index_record->depth ) ) {
                        zero_suffix.push_back(rec);
                    }
                    else {
                        one_suffix.push_back(rec);
                    }
                }
                // Split index_record
                // Sobreescribiendo sobre el antiguo bucket
                bucket.records = zero_suffix;
                bucket.bsize = zero_suffix.size();
                // Creando y escribiendo un nuevo index_record(que es un bucket pair) para 1+sufix
                Bucket<RecordType> one_bucket;
                one_bucket.records = one_suffix;
                one_bucket.bsize = one_suffix.size();
                one_bucket.next_bucket = -1; // creo que podemos juntarlo To_do en uno con una funcion copy
                // WRITE to disk
                data_file.seekp(0, ios::end); // Go to eof
                position_t pos_new = data_file.tellp(); // Save pos
                one_bucket.save(data_file, pos_new, bucket_max_size);
                bucket.save(data_file, bucket_position, bucket_max_size); // Sobreescribe un juego bucket en el archivo
                // New index_record
                suffix_t suffix_new = index_record->sufix + (1 << (index_record->depth-1)); // ?
                IndexRecord index_record_new{index_record->depth, suffix_new, pos_new};
                add_entry_index(index_record_new);
                // añadimos recursivamente
                data_file.close();
                insert(record);
            }
            else {
                // Bucket FULL AND depth >= max_depth:OVERFLOW (local depth >= global depth)
                auto prev_bucket_position = bucket_position;
                bucket_position = bucket.next_bucket;
                // Se inserta en un overflow page ya existente (si es que hay)
                while (bucket_position != -1) {
                    bucket.load(data_file, bucket_position, bucket_max_size);
                    if (bucket.bsize != bucket_max_size) { break; }
                    prev_bucket_position = bucket_position;
                    bucket_position = bucket.next_bucket;
                }
                // // Si no existe un overflow page, lo creamos
                if (bucket_position == -1) {
                    // hacer apuntar el bucket previo al final del archivo, donde la nueva overflow page sera creada
                    data_file.seekp(0, ios::end);
                    bucket_position = data_file.tellp();
                    bucket.next_bucket = bucket_position;
                    bucket.save(data_file, prev_bucket_position, bucket_max_size);
                    // Incializando el bucket vacio.
                    Bucket<RecordType> empty_bucket{0,-1};
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
        bucket.save(data_file, bucket_position, bucket_max_size);
        data_file.close();
    }
    else {
        // si falla al abir
        data_file.close();
        return false;
    }
    return true;
}

template <typename KeyType,typename RecordType>
bool ExtendibleHash<KeyType,RecordType>::remove(T key) { // KeyType o lo dejamos en T (funciona si ambos son int)
    auto key_hash = generate_hash(key);
    auto index_record = match_index_record(key_hash);
    auto bucket_position = index_record->bucket_position;
    Bucket<RecordType> bucket;
    // Buscar bucket hasta en las overflow pages, removeremos todas las apariciones
    fstream data_file(data_filename, ios::binary | ios::in | ios::out);
    if (data_file.is_open()){
        while (bucket_position != -1) {
            bucket.load(data_file, bucket_position, bucket_max_size); // Read bucket
            // Removemos repeticiones
            vector<RecordType> res;
            for (auto &record :  bucket.records) {
                if (record.get_key() != key) {
                    res.push_back(record);
                }
            }
            bucket.records = res;
            bucket.bsize = res.size();
            // sobreescribimos el bucket
            bucket.save(data_file, bucket_position, bucket_max_size);
            // Point to next bucket pos
            bucket_position = bucket.next_bucket;
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

template <typename KeyType,typename RecordType>
vector<RecordType> ExtendibleHash<KeyType,RecordType>::search(T key) {
    vector<RecordType> result_records;
    auto key_hash = generate_hash(key);
    // Buscamos el bucket en index_record
    auto index_record = match_index_record(key_hash);
    auto bucket_position = index_record->bucket_position;
    Bucket<RecordType> bucket;
    // Buscamos buckets hasta en los overflow pages
    fstream data_file(data_filename, ios::binary | ios::in);
    if (data_file.is_open()){
        while (bucket_position != -1) {
            bucket.load(data_file, bucket_position, bucket_max_size); // Read bucket
            // Search in bucket
            for (auto &record :  bucket.records) {
                if (record.get_key() == key) {
                    result_records.push_back(record);
                }
            }
            bucket_position = bucket.next_bucket; // next bucket position
        }
    }
    data_file.close();
    return result_records;
}

template <typename KeyType,typename RecordType>
vector<RecordType> ExtendibleHash<KeyType,RecordType>::rangeSearch(T begin_key,T end_key) {
    vector<RecordType> result_records;
    fstream data_file(data_filename, ios::binary | ios::in);
    if (data_file.is_open()){
        // Iteramos en todos los buckets
        for (auto &index_record : index_table ){
            auto bucket_position = index_record.bucket_position;
            Bucket<RecordType> bucket;
            // buscamos en overflow pages
            while (bucket_position != -1) {
                bucket.load(data_file, bucket_position, bucket_max_size); // Read bucket
                // Search in bucket
                for (auto &record :  bucket.records) { // puede ser que falle
                    if (record.get_key() >= begin_key && record.get_key() <= end_key) {
                        result_records.push_back(record);
                    }
                }
                bucket_position = bucket.next_bucket; // next bucket position
            }
        }
    }
    data_file.close();
    return result_records;
}

template <typename KeyType,typename RecordType>
vector<RecordType> ExtendibleHash<KeyType,RecordType>::load() {
    vector<RecordType> result_records;
    fstream data_file(data_filename, ios::binary | ios::in);
    if (data_file.is_open()){
        // Iteramos en todos los buckets
        for (auto &index_record : index_table ){
            auto bucket_position = index_record.bucket_position;
            Bucket<RecordType> bucket;
            // buscamos en overflow pages
            while (bucket_position != -1) {
                bucket.load(data_file, bucket_position, bucket_max_size); // Read bucket
                // Search in bucket
                for (auto &record :  bucket.records) {
                    result_records.push_back(record);
                }
                bucket_position = bucket.next_bucket; // next bucket position
            }
        }
    }
    data_file.close();
    return result_records;
}

template <typename KeyType,typename RecordType>
void ExtendibleHash<KeyType,RecordType>::write(vector<RecordType> input_records) { // hacer un if para que ehash.csv
    std::string directory;

    if (typeid(RecordType) == typeid(TestRecord)) {
        directory = "./results/Test/";
    } else if (typeid(RecordType) == typeid(HomesRecord)) {
        directory = "./results/Homes/";
    } else {
        std::cerr << "Unknown RecordType" << std::endl;
        return;
    }
    std::string file_path = directory + "ehash.csv";
    std::ofstream output_file_csv(file_path);
    for (auto val : input_records){
        output_file_csv<<val.print_csv();
    }
    output_file_csv.close();
}