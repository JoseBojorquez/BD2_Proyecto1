#include "Bucket.h"


struct IndexRecord {
    depth_t depth;
    suffix_t sufix;
    position_t bucket_position;
    void load(ifstream &file);
    void save(ofstream &file);
    void print();
};

void IndexRecord::load(ifstream &file) {
    file.read( (char*) &(this->depth), sizeof(depth_t) );
    file.read( (char*) &(this->sufix), sizeof(suffix_t) );
    file.read( (char*) &(this->bucket_position), sizeof(position_t) );
}

void IndexRecord::save(ofstream &file) {
    file.write( (char*) &(this->depth), sizeof(depth_t) );
    file.write( (char*) &(this->sufix), sizeof(suffix_t) );
    file.write( (char*) &(this->bucket_position), sizeof(position_t) );
}

void IndexRecord::print() {
    cout << "Depth: " << depth << " | "
         << "Sufix: " << sufix << " | "
         << "BucketP: " << bucket_position;
}

