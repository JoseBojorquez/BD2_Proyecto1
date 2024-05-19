#include "../src/RecordTest.h"

struct HomesRecord {
    //char property_url[130]; // delete
    char property_id[15]; // id
    char address[114]; // address
    //char street_name[105]; // delete
    char city[26]; // city
    char state[3]; // state
    char postcode[7]; // zip
    float price; // price
    // beds borrar porque hay muchos valores nulos
    // baths borrar porque hay muchos valores nulos
    //float land_space; // agregar?
    //char land_space_unit[5]; // agregar?
    char property_type[13]; // property_type
    char property_status[15]; // property_status
    //char RunDate[19]; // agregar ?
    //char agency_name[81]; // agregar ?

    void set_data_from_string(string _params) {
        stringstream ss(_params);
        string _property_id;
        string _address;
        string _city;
        string _state;
        string _postcode;
        string _price;
        string _property_type;
        string _property_status;
        getline(ss,_property_id,',');
        getline(ss,_address,',');
        getline(ss,_city,',');
        getline(ss,_state ,',');
        getline(ss,_postcode ,',');
        getline(ss,_price ,',');
        getline(ss,_property_type ,',');
        getline(ss,_property_status ,',');

        set_data(_property_id,_address,_city,_state,_postcode,stof(_price),_property_type,_property_status);
    }

    void set_data(string _property_id, string _address,
                  string _city, string _state, string _postcode,
                  float _price, string _property_type,
                  string _property_status) {
        //strncpy(property_url, _property_url, sizeof(property_url));
        strncpy(property_id, _property_id.c_str(), sizeof(property_id));
        strncpy(address, _address.c_str(), sizeof(address));
        //strncpy(street_name, _street_name, sizeof(street_name));
        strncpy(city, _city.c_str(), sizeof(city));
        strncpy(state, _state.c_str(), sizeof(state));
        strncpy(postcode, _postcode.c_str(), sizeof(postcode));
        price = _price;
        //land_space = _land_space;
        //strncpy(land_space_unit, _land_space_unit, sizeof(land_space_unit));
        strncpy(property_type, _property_type.c_str(), sizeof(property_type));
        strncpy(property_status, _property_status.c_str(), sizeof(property_status));
        //strncpy(RunDate, _RunDate, sizeof(RunDate));
        //strncpy(agency_name, _agency_name, sizeof(agency_name));
    }

    void print_data() const {
        std::cout //<< property_url << " | "
                << string(property_id) << " | "
                << string(address) << " | "
                //<< street_name << " | "
                << string(city) << " | "
                << string(state) << " | "
                << string(postcode) << " | "
                << price << " | "
                //<< land_space << " | "
                //<< land_space_unit << " | "
                << string(property_type) << " | "
                << string(property_status) << std::endl;
        //<< RunDate << " | "
        //<< agency_name << std::endl;
    }

    void load(std::fstream& file) {
        //file.read(property_url, sizeof(property_url));
        file.read(property_id, sizeof(property_id));
        file.read(address, sizeof(address));
        //file.read(street_name, sizeof(street_name));
        file.read(city, sizeof(city));
        file.read(state, sizeof(state));
        file.read(postcode, sizeof(postcode));
        file.read((char*)&price, sizeof(price));
        //file.read(reinterpret_cast<char*>(&land_space), sizeof(land_space));
        //file.read(land_space_unit, sizeof(land_space_unit));
        file.read(property_type, sizeof(property_type));
        file.read(property_status, sizeof(property_status));
        //file.read(RunDate, sizeof(RunDate));
        //file.read(agency_name, sizeof(agency_name));
    }

    void save(std::fstream& file) const {
        //file.write(property_url, sizeof(property_url));
        file.write(property_id, sizeof(property_id));
        file.write(address, sizeof(address));
        //file.write(street_name, sizeof(street_name));
        file.write(city, sizeof(city));
        file.write(state, sizeof(state));
        file.write(postcode, sizeof(postcode));
        file.write((char*)&price, sizeof(price));
        //file.write(reinterpret_cast<const char*>(&land_space), sizeof(land_space));
        //file.write(land_space_unit, sizeof(land_space_unit));
        file.write(property_type, sizeof(property_type));
        file.write(property_status, sizeof(property_status));
        //file.write(RunDate, sizeof(RunDate));
        //file.write(agency_name, sizeof(agency_name));
    }

    void save(std::ofstream& file) const {
        //file.write(property_url, sizeof(property_url));
        file.write(property_id, sizeof(property_id));
        file.write(address, sizeof(address));
        //file.write(street_name, sizeof(street_name));
        file.write(city, sizeof(city));
        file.write(state, sizeof(state));
        file.write(postcode, sizeof(postcode));
        file.write((char*)&price, sizeof(price));
        //file.write(reinterpret_cast<const char*>(&land_space), sizeof(land_space));
        //file.write(land_space_unit, sizeof(land_space_unit));
        file.write(property_type, sizeof(property_type));
        file.write(property_status, sizeof(property_status));
        //file.write(RunDate, sizeof(RunDate));
        //file.write(agency_name, sizeof(agency_name));
    }

    T get_key() const {
        return std::stoi(property_id);
    }

    std::string print_csv() const {
        return
                string(property_id)+","+
                string(address)+","+
                string(city)+","+
                string(state)+","+
                string(postcode)+","+
                to_string(price)+","+
                string(property_type)+","+
                string(property_status)+"\n";
    }
};