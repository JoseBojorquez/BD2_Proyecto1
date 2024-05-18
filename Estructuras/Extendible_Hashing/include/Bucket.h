#include "../src/RecordTest.h"

struct Bucket {
    bsize_t bsize;
    pos_t next_bucket;
    vector<Record> records;

    void load_from_file(fstream &file, pos_t bucket_pos, bsize_t bucket_size) {
        file.seekg(bucket_pos, ios::beg);
        file.read( (char*) &(this->bsize), sizeof(bsize_t) );
        file.read( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Load to vector only valid data
        vector<Record> tmp;
        Record record;
        for (int i=0; i<bsize; i++) {
            record.load_from_file(file);
            tmp.push_back(record);
        }
        records = tmp;
    }
    void save_to_file(fstream &file, pos_t bucket_pos, bsize_t bucket_size) {
        file.seekp(bucket_pos, ios::beg);
        file.write( (char*) &(this->bsize), sizeof(bsize_t) );
        file.write( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Write all to reserve space
        Record record;
        for (int i=0; i<bucket_size; i++) {
            if (i<records.size()) {
                record = records[i];
            }
            record.save_to_file(file);
        }
    }

    // DUPLICATE FOR OFSTREAM
    void save_to_file(ofstream &file, bsize_t bucket_size) {
        file.write( (char*) &(this->bsize), sizeof(bsize_t) );
        file.write( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Write all to reserve space
        Record record;
        for (int i=0; i<bucket_size; i++) {
            if (i<records.size()) {
                record = records[i];
            }
            record.save_to_file(file);
        }
    }
};