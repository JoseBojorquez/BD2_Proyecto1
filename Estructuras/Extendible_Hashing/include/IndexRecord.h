#include "Bucket.h"

struct IndexRecord {
    depth_t depth;
    sufix_t sufix;
    pos_t bucket_pos;
    void load_from_ifile(ifstream &file);
    void save_to_ofile(ofstream &file);
    void print_data();
};

void IndexRecord::load_from_ifile(ifstream &file) {
    file.read( (char*) &(this->depth), sizeof(depth_t) );
    file.read( (char*) &(this->sufix), sizeof(sufix_t) );
    file.read( (char*) &(this->bucket_pos), sizeof(pos_t) );
}

void IndexRecord::save_to_ofile(ofstream &file) {
    file.write( (char*) &(this->depth), sizeof(depth_t) );
    file.write( (char*) &(this->sufix), sizeof(sufix_t) );
    file.write( (char*) &(this->bucket_pos), sizeof(pos_t) );
}

void IndexRecord::print_data() {
    cout << "Depth: " << depth << " | "
         << "Sufix: " << sufix << " | "
         << "BucketP: " << bucket_pos;
}

