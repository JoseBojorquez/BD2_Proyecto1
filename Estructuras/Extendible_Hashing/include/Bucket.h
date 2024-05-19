// #include "../src/RecordTest.h"
#include "../src/CreditCardRecord.h"


template <typename RecordType>
struct Bucket {
    bucketSize_t bsize;
    position_t next_bucket;
    vector<RecordType> records;

    void load(fstream &file, position_t bucket_position, bucketSize_t bucket_size);

    void save(fstream &file, position_t bucket_pos, bucketSize_t bucket_size);

    void save(ofstream &file, bucketSize_t bucket_size);
};

template <typename RecordType>
void Bucket<RecordType>::load(fstream& file, position_t bucket_position, bucketSize_t bucket_size) {
    file.seekg(bucket_position, ios::beg);
    file.read( (char*) &(this->bsize), sizeof(bucketSize_t) );
    file.read( (char*) &(this->next_bucket), sizeof(position_t) );
    // Load to vector only valid data
    records.clear();
    for (int i=0; i<bsize; i++) {
        RecordType record;
        record.load(file);
        records.push_back(record);
    }
}

template <typename RecordType>
void Bucket<RecordType>::save(fstream &file, position_t bucket_pos, bucketSize_t bucket_size) {
    file.seekp(bucket_pos, ios::beg);
    file.write( (char*) &(this->bsize), sizeof(bucketSize_t) );
    file.write( (char*) &(this->next_bucket), sizeof(position_t) );
    // Write all to reserve space
    RecordType record;
    for (int i=0; i<bucket_size; i++) {
        if (i<records.size()) {
            record = records[i];
        }
        record.save(file);
    }
}

template <typename RecordType>
void Bucket<RecordType>::save(ofstream &file, bucketSize_t bucket_size) {
    file.write( (char*) &(this->bsize), sizeof(bucketSize_t) );
    file.write( (char*) &(this->next_bucket), sizeof(position_t) );
    // Write all to reserve space
    RecordType record;
    for (int i=0; i<bucket_size; i++) {
        if (i<records.size()) {
            record = records[i];
        }
        record.save(file);
    }
}